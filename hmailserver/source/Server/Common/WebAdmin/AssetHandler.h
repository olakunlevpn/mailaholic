#pragma once

#include <string>

namespace WebAdmin
{
   class AssetHandler
   {
   public:
      static bool ServeAsset(const std::string& path,
                             std::string& content,
                             std::string& mimeType,
                             bool& isCompressed);
   };
}
