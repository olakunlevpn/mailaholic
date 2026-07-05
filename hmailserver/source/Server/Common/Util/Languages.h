// Copyright (c) 2025 Mailaholic contributors (based on hMailServer).
// https://github.com/olakunlevpn/mailaholic

#pragma once

namespace MA
{
   class Language;

   class Languages : public Singleton<Languages>
   {
   public:
      Languages(void);
      ~Languages(void);

      void Load();

      std::shared_ptr<Language> GetLanguage(const String &sLanguage);
      std::shared_ptr<Language> GetLanguage(int index);
      size_t GetCount() { return languages_.size(); }

     
   private:

      bool IsValidLangauge_(const String &sLanguage) const;

      std::map<String, std::shared_ptr<Language> > languages_;


      
   };
}