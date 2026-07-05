// Copyright (c) 2025 Mailaholic contributors (based on hMailServer).
// https://github.com/olakunlevpn/mailaholic

#pragma once

namespace MA
{
   class BlowFishEncryptor;
   
   class Crypt : public Singleton<Crypt>
   {
   public:
	   Crypt();
	   virtual ~Crypt();

      enum EncryptionType
      {
         ETNone = 0,
         ETBlowFish = 1,
         ETMD5 = 2,
         ETSHA256 = 3,
         ETPBKDF2 = 4,
         ETAESGCM = 5
      };

      EncryptionType GetHashType(const String &hash);

      String EnCrypt(const String &sInput, EncryptionType iType) const;
      String DeCrypt(const String &sInput, EncryptionType iType) const;

      bool Validate(const String &password, const String &originalHash, EncryptionType iType) const;

   private:

      BlowFishEncryptor *blow_fish_;

      String GeneratePBKDF2_(const String &password) const;
      bool ValidatePBKDF2_(const String &password, const String &stored) const;

      String EncryptAESGCM_(const String &plaintext) const;
      String DecryptAESGCM_(const String &stored) const;

   };
}
