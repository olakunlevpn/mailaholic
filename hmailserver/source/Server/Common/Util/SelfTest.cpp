// Copyright (c) 2026 Mailaholic.
#include "stdafx.h"
#include "SelfTest.h"

#include <cstdio>

#include "../Mime/MimeTester.h"
#include "Parsing/StringParser.h"
#include "Strings/Formatter.h"
#include "VariantDateTime.h"
#include "Encoding/Base64.h"
#include "Encoding/ModifiedUTF7.h"
#include "Hashing/HashCreator.h"
#include "Parsing/AddresslistParser.h"
#include "../../IMAP/IMAPSimpleCommandParser.h"

#ifdef _DEBUG
#define DEBUG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
#define new DEBUG_NEW
#endif

namespace MA
{
   int
   SelfTest::RunAll()
   {
      int failures = 0;

      struct NamedTest { const char* name; std::function<void()> run; };

      std::vector<NamedTest> tests =
      {
         { "MimeTester",                []{ MimeTester t; t.Test(); } },
         { "StringParserTester",        []{ StringParserTester t; t.Test(); } },
         { "FormatterTester",           []{ FormatterTester t; t.Test(); } },
         { "DateTimeTests",             []{ DateTimeTests t; t.Test(); } },
         { "Base64Tester",              []{ Base64Tester t; t.Test(); } },
         { "ModifiedUTF7Tester",        []{ ModifiedUTF7Tester t; t.Test(); } },
         { "HashCreatorTester",         []{ HashCreatorTester t; t.Test(); } },
         { "AddresslistParserTester",   []{ AddresslistParserTester t; t.Test(); } },
         { "IMAPSimpleCommandParserTester", []{ IMAPSimpleCommandParserTester t; t.Test(); } },
      };

      for (auto& test : tests)
      {
         try
         {
            test.run();
            printf("[ PASS ] %s\n", test.name);
         }
         catch (...)
         {
            printf("[ FAIL ] %s\n", test.name);
            failures++;
         }
         fflush(stdout);
      }

      printf("SelfTest: %d/%d passed\n", (int)tests.size() - failures, (int)tests.size());
      fflush(stdout);
      return failures == 0 ? 0 : 1;
   }
}
