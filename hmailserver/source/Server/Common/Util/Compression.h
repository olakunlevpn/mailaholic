// Copyright (c) 2025 Mailaholic contributors (based on hMailServer).
// https://github.com/olakunlevpn/mailaholic

#pragma once

namespace MA
{
   class Compression
   {
   public:
      Compression();
      ~Compression(void);

      bool AddDirectory(const String &zipFile, const String &directoryToAdd);
      bool AddFile(const String &zipFile, const String &fileToAdd);

      bool Uncompress(const String &zipFile, const String &targetDirectory);
      bool Uncompress(const String &zipFile, const String &targetDirectory, const String &wildCard);

   private:
 
      bool LaunchCommand_(const String &commandLine);
      String GetExecutableFullPath_();

   };
}