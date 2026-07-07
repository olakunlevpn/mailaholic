#include "stdafx.h"
#include "AssetHandler.h"
#include "EmbeddedAssets.h"

namespace WebAdmin
{
   bool AssetHandler::ServeAsset(const std::string& path,
                                  std::string& content,
                                  std::string& mimeType,
                                  bool& isCompressed)
   {
      std::string assetPath = path;

      if (assetPath == "/" || assetPath.empty())
         assetPath = "index.html";
      else if (assetPath[0] == '/')
         assetPath = assetPath.substr(1);

      const unsigned char* data;
      size_t size;
      const char* mime;

      if (!GetAsset(assetPath, data, size, mime))
         return false;

      content = std::string(reinterpret_cast<const char*>(data), size);
      mimeType = mime;
      isCompressed = false;

      return true;
   }
}
