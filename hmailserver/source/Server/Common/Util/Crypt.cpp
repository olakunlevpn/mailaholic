// Copyright (c) 2025 Mailaholic contributors (based on hMailServer).
// https://github.com/olakunlevpn/mailaholic

#include "stdafx.h"
#include "Crypt.h"
#include "BlowFish.h"
#include "Hashing/HashCreator.h"

#include <openssl/evp.h>
#include <openssl/rand.h>
#include <openssl/crypto.h>
#include "../Mime/MimeCode.h"
#include "../Application/IniFileSettings.h"

#include <vector>
#include <stdexcept>

#ifdef _DEBUG
#define DEBUG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
#define new DEBUG_NEW
#endif

namespace MA
{
   namespace
   {
      const int PBKDF2_ITERATIONS = 600000;
      const int PBKDF2_SALT_LEN = 16;
      const int PBKDF2_KEY_LEN = 32;

      // Base64-encode raw bytes to a single unbroken line. Line folding is
      // disabled so the output never contains CRLF, which would otherwise be
      // (a) truncated on decode and (b) mangled by XML attribute normalization
      // in message-store backups. Applies to values of any length.
      AnsiString Base64EncodeRaw(const unsigned char *data, int len)
      {
         MimeCodeBase64 encoder;
         encoder.AddLineBreak(false);
         encoder.SetInput((const char*)data, len, true);
         AnsiString encoded;
         encoder.GetOutput(encoded);
         return encoded;
      }

      // Returns decoded byte vector; empty vector signals decode failure/empty.
      std::vector<unsigned char> Base64DecodeRaw(const AnsiString &input)
      {
         MimeCodeBase64 decoder;
         decoder.SetInput(input.c_str(), input.GetLength(), false);
         AnsiString decoded;
         decoder.GetOutput(decoded);
         return std::vector<unsigned char>(decoded.begin(), decoded.end());
      }

      // Returns the value 0-15 of a single hex character, or -1 if not hex.
      int HexNibble(char c)
      {
         if (c >= '0' && c <= '9') return c - '0';
         if (c >= 'a' && c <= 'f') return c - 'a' + 10;
         if (c >= 'A' && c <= 'F') return c - 'A' + 10;
         return -1;
      }

      // Decode a hex string (e.g. the 64-char SecretKey) to raw bytes. Returns
      // an empty vector on odd length or a non-hex character.
      std::vector<unsigned char> HexToBytes(const AnsiString &hex)
      {
         int len = hex.GetLength();
         if (len % 2 != 0)
            return std::vector<unsigned char>();

         std::vector<unsigned char> bytes(len / 2);
         for (int i = 0; i < len / 2; i++)
         {
            int hi = HexNibble(hex[i * 2]);
            int lo = HexNibble(hex[i * 2 + 1]);
            if (hi < 0 || lo < 0)
               return std::vector<unsigned char>();
            bytes[i] = (unsigned char)((hi << 4) | lo);
         }
         return bytes;
      }
   }

   Crypt::Crypt()
   {
      blow_fish_ = new BlowFishEncryptor();
   }  

   Crypt::~Crypt()
   {
      delete blow_fish_;
   }

   String
   Crypt::EnCrypt(const String &sInput,EncryptionType iType) const
   {
      switch (iType)
      {
      case ETNone:
         return sInput;
      case ETBlowFish:
         {
            if (sInput.IsEmpty())
               return "";

            return blow_fish_->EncryptToString(sInput);
         }
      case ETMD5:
         {
            HashCreator crypter(HashCreator::MD5);
            String sResult = crypter.GenerateHashNoSalt(sInput, HashCreator::hex);
            return sResult;
         }
      case ETSHA256:
         {
            HashCreator encrypter(HashCreator::SHA256);
            AnsiString result = encrypter.GenerateHash(sInput, "");
            return result;
         }
      case ETPBKDF2:
         {
            return GeneratePBKDF2_(sInput);
         }
      case ETAESGCM:
         {
            if (sInput.IsEmpty())
               return "";

            return EncryptAESGCM_(sInput);
         }
      default:
         {
            assert(0);
         }
      }

      return "";

   }

   bool
   Crypt::Validate(const String &password, const String &originalHash, EncryptionType iType) const
   {
      switch (iType)
      {
      case ETMD5:
         {
            // Salts are not used for the MD5 hashes.
            HashCreator encrypter(HashCreator::MD5);
            bool result = encrypter.ValidateHash(password, originalHash, false);
            return result;
         }
      case ETSHA256:
         {
            // Salts are always used for the SHA256 hashes.
            HashCreator encrypter(HashCreator::SHA256);
            bool result = encrypter.ValidateHash(password, originalHash, true);
            return result;
         }
      case ETPBKDF2:
         {
            return ValidatePBKDF2_(password, originalHash);
         }
      default:
         {
            assert(0);
         }
      }

      return false;

   }

   Crypt::EncryptionType
   Crypt::GetHashType(const String &hash)
   {
      if (hash.StartsWith(_T("$PBKDF2$")))
         return ETPBKDF2;

      int length = hash.GetLength();
      if (length == 32)
         return ETMD5;
      else if (length == 70)
         return ETSHA256;
      else
         return ETNone;
   }

   String
   Crypt::GeneratePBKDF2_(const String &password) const
   {
      unsigned char salt[PBKDF2_SALT_LEN];
      if (RAND_bytes(salt, PBKDF2_SALT_LEN) != 1)
         throw std::runtime_error("RAND_bytes failed generating PBKDF2 salt");

      AnsiString passwordAnsi = password;

      unsigned char key[PBKDF2_KEY_LEN];
      if (PKCS5_PBKDF2_HMAC(passwordAnsi.c_str(), passwordAnsi.GetLength(),
                            salt, PBKDF2_SALT_LEN,
                            PBKDF2_ITERATIONS, EVP_sha256(),
                            PBKDF2_KEY_LEN, key) != 1)
         throw std::runtime_error("PKCS5_PBKDF2_HMAC failed");

      String result;
      result.Format(_T("$PBKDF2$%d$%s$%s"),
                    PBKDF2_ITERATIONS,
                    String(Base64EncodeRaw(salt, PBKDF2_SALT_LEN)).c_str(),
                    String(Base64EncodeRaw(key, PBKDF2_KEY_LEN)).c_str());
      return result;
   }

   bool
   Crypt::ValidatePBKDF2_(const String &password, const String &stored) const
   {
      // Format: $PBKDF2$<iterations>$<salt-b64>$<hash-b64>
      std::vector<String> parts = StringParser::SplitString(stored, _T("$"));
      // Leading '$' yields an empty first element: ["", "PBKDF2", iters, salt, hash]
      if (parts.size() != 5)
         return false;
      if (parts[1] != _T("PBKDF2"))
         return false;

      int iterations = _ttoi(parts[2].c_str());
      if (iterations <= 0)
         return false;

      std::vector<unsigned char> salt = Base64DecodeRaw(AnsiString(parts[3]));
      std::vector<unsigned char> expected = Base64DecodeRaw(AnsiString(parts[4]));
      if (salt.empty() || expected.empty())
         return false;

      AnsiString passwordAnsi = password;
      std::vector<unsigned char> actual(expected.size());
      if (PKCS5_PBKDF2_HMAC(passwordAnsi.c_str(), passwordAnsi.GetLength(),
                            &salt[0], (int)salt.size(),
                            iterations, EVP_sha256(),
                            (int)actual.size(), &actual[0]) != 1)
         return false;

      return CRYPTO_memcmp(&actual[0], &expected[0], actual.size()) == 0;
   }

   String
   Crypt::EncryptAESGCM_(const String &plaintext) const
   {
      std::vector<unsigned char> key = HexToBytes(AnsiString(IniFileSettings::Instance()->GetSecretKey()));
      if (key.size() != 32)
         throw std::runtime_error("SecretKey is not 32 bytes; cannot AES-256-GCM encrypt");

      unsigned char iv[12];
      if (RAND_bytes(iv, sizeof(iv)) != 1)
         throw std::runtime_error("RAND_bytes failed generating AES-GCM IV");

      AnsiString plainAnsi = plaintext;
      int ptLen = plainAnsi.GetLength();

      // Guard against a non-empty String that narrows to zero bytes; empty
      // ciphertext would take &vec[0] on an empty vector below.
      if (ptLen == 0)
         return "";

      std::vector<unsigned char> ciphertext(ptLen);
      unsigned char tag[16];

      EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
      if (!ctx)
         throw std::runtime_error("EVP_CIPHER_CTX_new failed");

      int ok = 1;
      int outLen = 0;
      ok &= EVP_EncryptInit_ex(ctx, EVP_aes_256_gcm(), NULL, NULL, NULL);
      ok &= EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_GCM_SET_IVLEN, sizeof(iv), NULL);
      ok &= EVP_EncryptInit_ex(ctx, NULL, NULL, &key[0], iv);
      if (ptLen > 0)
         ok &= EVP_EncryptUpdate(ctx, &ciphertext[0], &outLen, (const unsigned char*)plainAnsi.c_str(), ptLen);
      int finalLen = 0;
      unsigned char finalDummy;
      ok &= EVP_EncryptFinal_ex(ctx, ptLen > 0 ? &ciphertext[0] + outLen : &finalDummy, &finalLen);
      ok &= EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_GCM_GET_TAG, sizeof(tag), tag);
      EVP_CIPHER_CTX_free(ctx);

      if (ok != 1)
         throw std::runtime_error("AES-256-GCM encryption failed");

      String result;
      result.Format(_T("$AESGCM$%s$%s$%s"),
                    String(Base64EncodeRaw(iv, sizeof(iv))).c_str(),
                    String(Base64EncodeRaw(tag, sizeof(tag))).c_str(),
                    String(Base64EncodeRaw(&ciphertext[0], ptLen)).c_str());
      return result;
   }

   String
   Crypt::DecryptAESGCM_(const String &stored) const
   {
      // Format: $AESGCM$<iv-b64>$<tag-b64>$<ciphertext-b64>
      std::vector<String> parts = StringParser::SplitString(stored, _T("$"));
      // Leading '$' yields an empty first element: ["", "AESGCM", iv, tag, ct]
      if (parts.size() != 5)
         return "";
      if (parts[1] != _T("AESGCM"))
         return "";

      std::vector<unsigned char> iv = Base64DecodeRaw(AnsiString(parts[2]));
      std::vector<unsigned char> tag = Base64DecodeRaw(AnsiString(parts[3]));
      std::vector<unsigned char> ciphertext = Base64DecodeRaw(AnsiString(parts[4]));
      if (iv.size() != 12 || tag.size() != 16 || ciphertext.empty())
         return "";

      std::vector<unsigned char> key = HexToBytes(AnsiString(IniFileSettings::Instance()->GetSecretKey()));
      if (key.size() != 32)
         return "";

      std::vector<unsigned char> plaintext(ciphertext.size());

      EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
      if (!ctx)
         return "";

      int ok = 1;
      int outLen = 0;
      ok &= EVP_DecryptInit_ex(ctx, EVP_aes_256_gcm(), NULL, NULL, NULL);
      ok &= EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_GCM_SET_IVLEN, (int)iv.size(), NULL);
      ok &= EVP_DecryptInit_ex(ctx, NULL, NULL, &key[0], &iv[0]);
      ok &= EVP_DecryptUpdate(ctx, &plaintext[0], &outLen, &ciphertext[0], (int)ciphertext.size());
      ok &= EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_GCM_SET_TAG, (int)tag.size(), &tag[0]);
      int finalLen = 0;
      int finalRet = EVP_DecryptFinal_ex(ctx, &plaintext[0] + outLen, &finalLen);
      EVP_CIPHER_CTX_free(ctx);

      // EVP_DecryptFinal_ex returns > 0 only when the GCM tag verifies. Any
      // tamper, wrong key, or truncation fails the tag -> return empty.
      if (ok != 1 || finalRet <= 0)
         return "";

      return AnsiString((const char*)&plaintext[0], plaintext.size());
   }

   String
   Crypt::DeCrypt(const String &sInput, EncryptionType iType) const
   {
      switch (iType)
      {
         case ETNone:
            return sInput;
         case ETBlowFish:
            {
               if (sInput.IsEmpty())
                  return "";

               return blow_fish_->DecryptFromString(sInput);
            }
            break;
         case ETAESGCM:
            {
               if (sInput.IsEmpty())
                  return "";

               return DecryptAESGCM_(sInput);
            }
            break;
         default:
            assert(0);
      }

      return "";
   }
}
