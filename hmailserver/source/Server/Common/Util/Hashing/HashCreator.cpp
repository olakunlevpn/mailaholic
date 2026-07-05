// Copyright (c) 2025 Mailaholic contributors (based on hMailServer).
// https://github.com/olakunlevpn/mailaholic

#include <stdafx.h>

#include "../PasswordGenerator.h"
#include "../../Mime/MimeCode.h"
#include "../Crypt.h"

#include <openssl/sha.h>
#include <openssl/evp.h>
#include <openssl/md5.h>
#include <openssl/crypto.h>

#include <stdexcept>

#include "HashCreator.h"

#ifdef _DEBUG
#define DEBUG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
#define new DEBUG_NEW
#endif

namespace MA
{
   HashCreator::HashCreator(HashCreator::HashType hashType) :
      hash_type_(hashType)
   {

   }

   AnsiString HashCreator::GenerateHash(const AnsiString &inputString, const AnsiString &salt)
   {
      AnsiString saltString = salt;
      if (saltString.GetLength() == 0 && hash_type_ == SHA256)
      {     
         AnsiString randomString = PasswordGenerator::Generate();
         saltString = GetHash_(randomString, hex);
         saltString = saltString.Mid(0, SALT_LENGTH);
      }

      AnsiString value = saltString + GetHash_(saltString + inputString, hex);
      return value;
   }

   AnsiString 
   HashCreator::GenerateHashNoSalt(const AnsiString &inputString, RequestedEncoding encoding)
   {
      return GetHash_(inputString, encoding);
   }

   AnsiString 
   HashCreator::GenerateHashNoSalt(unsigned char *input, int inputLength, RequestedEncoding encoding)
   {
      return GetHash_Raw(input, inputLength, encoding);
   }


   bool 
   HashCreator::ValidateHash(const AnsiString &password, const AnsiString &originalHash, bool useSalt)
   {
      AnsiString result;
      if (useSalt)
      {
         AnsiString salt = GetSalt_(originalHash);
         result = GenerateHash(password, salt);
      }
      else
      {
         result = GetHash_(password, hex);
      }

      // Constant-time comparison to avoid timing side channels.
      if (result.GetLength() != originalHash.GetLength())
         return false;

      return CRYPTO_memcmp(result.c_str(), originalHash.c_str(), result.GetLength()) == 0;
   }

   AnsiString HashCreator::GetSalt_(const AnsiString &inputString)
   {
      AnsiString result = inputString.Mid(0,SALT_LENGTH);
      return result;
   }

   AnsiString HashCreator::GetHash_(const AnsiString &sInputString, HashCreator::RequestedEncoding encoding)
   {
      AnsiString temp = sInputString;
      return GetHash_Raw((unsigned char*) temp.GetBuffer(), temp.GetLength(), encoding);
   }

   AnsiString HashCreator::GetHash_Raw(const unsigned char *input, int inputLength, HashCreator::RequestedEncoding encoding)
   {
      int digestLength = 0;

      const EVP_MD *md = NULL;
      switch (hash_type_)
      {
      case SHA1:
         md = EVP_sha1();
         break;
      case SHA256:
         md = EVP_sha256();
         break;
      case MD5:
         md = EVP_md5();
         break;
      }

      if (md == NULL)
         throw std::runtime_error("HashCreator: unknown hash type");

      digestLength = EVP_MD_get_size(md);

      unsigned char *results = new unsigned char[digestLength];

      EVP_MD_CTX *context = EVP_MD_CTX_new();
      unsigned int outLength = (unsigned int)digestLength;
      if (context == NULL ||
          EVP_DigestInit_ex(context, md, NULL) != 1 ||
          EVP_DigestUpdate(context, input, inputLength) != 1 ||
          EVP_DigestFinal_ex(context, results, &outLength) != 1)
      {
         if (context != NULL)
            EVP_MD_CTX_free(context);
         delete [] results;
         throw std::runtime_error("HashCreator: EVP digest computation failed");
      }
      EVP_MD_CTX_free(context);


      MA::AnsiString retVal;
      if (encoding == hex)
      {
         char buffer[3];
         buffer[2] = '\0';

         for (int i = 0; i < digestLength; i++)
         {
            sprintf_s(buffer, 3, "%02x", results[i]);

            retVal += buffer;
         }

      }
      else if (encoding == base64)
      {
         MimeCodeBase64 encoder;
         encoder.SetInput((const char*) results, digestLength, true);

         AnsiString sEncodedValue;
         encoder.GetOutput(sEncodedValue);

         retVal = sEncodedValue;
         retVal = retVal.Mid(0, retVal.GetLength()-2);
      }

      delete [] results;

      return retVal;
   }

   void
   HashCreatorTester::Test()
   {
      // Run basic test.
      HashCreator hasher(HashCreator::SHA256);
      AnsiString result = hasher.GenerateHash("The quick brown fox jumps over the lazy dog", "");

      if (!hasher.ValidateHash("The quick brown fox jumps over the lazy dog", result, true))
         throw 0;

      // Check that same password hashed twice yealds separate hashes.
      AnsiString test1 = hasher.GenerateHash("The quick brown fox jumps over the lazy dog", "");
      AnsiString test2 = hasher.GenerateHash("The quick brown fox jumps over the lazy dog", "");
      if (test1 == test2)
         throw 0;

      for (int i = 0; i < 250; i++)
      {
         HashCreator memoryTester(HashCreator::SHA256);

         String temp;
         temp.Format(_T("%d"), i);
         AnsiString hashableString = temp;

         hasher.GenerateHash(hashableString, "test");
      }

      // PBKDF2 round-trip: a generated hash must validate against its password
      // and reject a wrong password. Fail-closed on malformed input.
      {
         Crypt* crypt = Crypt::Instance();

         String stored = crypt->EnCrypt("correct horse battery staple", Crypt::ETPBKDF2);

         if (crypt->GetHashType(stored) != Crypt::ETPBKDF2)
            throw 0;
         if (!stored.StartsWith(_T("$PBKDF2$600000$")))
            throw 0;
         if (!crypt->Validate("correct horse battery staple", stored, Crypt::ETPBKDF2))
            throw 0;
         if (crypt->Validate("wrong password", stored, Crypt::ETPBKDF2))
            throw 0;

         // Two hashes of the same password differ (random salt).
         String stored2 = crypt->EnCrypt("correct horse battery staple", Crypt::ETPBKDF2);
         if (stored == stored2)
            throw 0;

         // Malformed stored values must fail closed, never throw uncaught, never validate.
         if (crypt->Validate("x", _T("$PBKDF2$notanumber$aa$bb"), Crypt::ETPBKDF2))
            throw 0;
         if (crypt->Validate("x", _T("garbage"), Crypt::ETPBKDF2))
            throw 0;

         // Legacy hash formats must not be misdetected as PBKDF2.
         if (crypt->GetHashType(_T("5f4dcc3b5aa765d61d8327deb882cf99")) == Crypt::ETPBKDF2)
            throw 0;
         String sha256like = _T("abcdef");
         while (sha256like.GetLength() < 70) sha256like += _T("a");
         if (crypt->GetHashType(sha256like) == Crypt::ETPBKDF2)
            throw 0;
      }

      // AES-256-GCM reversible encryption: round-trip, tamper rejection, and
      // a fresh IV per call. Uses IniFileSettings::GetSecretKey (generated and
      // persisted on first use).
      {
         Crypt* crypt = Crypt::Instance();

         String secret = "hunter2 relay password";
         String blob = crypt->EnCrypt(secret, Crypt::ETAESGCM);

         if (!blob.StartsWith(_T("$AESGCM$")))
            throw 0;
         if (crypt->DeCrypt(blob, Crypt::ETAESGCM) != secret)
            throw 0;

         // Tampering with the trailing ciphertext must fail the GCM tag.
         String tampered = blob.Mid(0, blob.GetLength() - 2) + _T("AA");
         if (crypt->DeCrypt(tampered, Crypt::ETAESGCM) == secret)
            throw 0;

         // Two encryptions of the same secret differ (random IV).
         if (crypt->EnCrypt(secret, Crypt::ETAESGCM) == blob)
            throw 0;

         // Empty input round-trips to empty (matches legacy behavior).
         if (!crypt->EnCrypt(_T(""), Crypt::ETAESGCM).IsEmpty())
            throw 0;

         // A long secret (ciphertext base64 > 76 chars) must round-trip: the
         // base64 encoder must not fold in CRLF, which would corrupt the blob.
         String longSecret;
         while (longSecret.GetLength() < 200) longSecret += _T("A");
         String longBlob = crypt->EnCrypt(longSecret, Crypt::ETAESGCM);
         if (longBlob.Find(_T("\r")) >= 0 || longBlob.Find(_T("\n")) >= 0)
            throw 0;
         if (crypt->DeCrypt(longBlob, Crypt::ETAESGCM) != longSecret)
            throw 0;
      }
   }
}