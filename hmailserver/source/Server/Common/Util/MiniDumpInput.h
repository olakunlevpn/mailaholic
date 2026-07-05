// Copyright (c) 2025 Mailaholic contributors (based on hMailServer).
// https://github.com/olakunlevpn/mailaholic

#pragma once

namespace MA
{
   class MiniDumpInput
   {
   public:
      int ProcessId;
      int ThreadId;
      EXCEPTION_RECORD ExceptionRecord;
      CONTEXT ContextRecord;

      wchar_t DumpFile[2048];

      static const std::string SharedMemoryName;
   };

   
}