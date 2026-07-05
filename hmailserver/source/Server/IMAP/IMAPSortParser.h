// Copyright (c) 2025 Mailaholic contributors (based on hMailServer).
// https://github.com/olakunlevpn/mailaholic

#pragma once

namespace MA
{
   class IMAPSortParser
   {
   public:
      IMAPSortParser(void);
      ~IMAPSortParser(void);

      void Parse(const String &sExpression);

      std::vector<std::pair<bool,String> > GetSortTypes () {return sort_types_; }
   private:

      // pair: ascending, criteria
      
      std::vector<std::pair<bool,String> > sort_types_;
   };
}