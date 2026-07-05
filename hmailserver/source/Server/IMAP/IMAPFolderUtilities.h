// Copyright (c) 2025 Mailaholic contributors (based on hMailServer).
// https://github.com/olakunlevpn/mailaholic

#pragma once

namespace MA
{
   class IMAPFolders;
   class IMAPFolder;

   class IMAPFolderUtilities
   {
   private:
	   IMAPFolderUtilities();
	   virtual ~IMAPFolderUtilities();

   public:

      static bool IsPublicFolder(const std::vector<String> &vecFolderPath);

      static std::shared_ptr<IMAPFolder> GetTopMostExistingFolder(std::shared_ptr<IMAPFolders> pContainer, const std::vector<String> &vecFolderPath);
   };
}
