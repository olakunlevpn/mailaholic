#include "stdafx.h"
#include "WebSessionManager.h"
#include <random>
#include <sstream>
#include <iomanip>

namespace WebAdmin
{
   WebSessionManager& WebSessionManager::Instance()
   {
      static WebSessionManager instance;
      return instance;
   }

   std::string WebSessionManager::GenerateToken()
   {
      std::random_device rd;
      std::mt19937_64 gen(rd());
      std::uniform_int_distribution<uint64_t> dis;

      std::ostringstream oss;
      oss << std::hex << std::setfill('0');
      oss << std::setw(16) << dis(gen);
      oss << std::setw(16) << dis(gen);
      return oss.str();
   }

   std::string WebSessionManager::CreateSession(int userId)
   {
      std::lock_guard<std::mutex> lock(mutex_);

      std::string token = GenerateToken();
      auto expires = std::chrono::steady_clock::now() +
                     std::chrono::hours(SESSION_DURATION_HOURS);

      sessions_[token] = {userId, expires};
      return token;
   }

   int WebSessionManager::ValidateSession(const std::string& token)
   {
      std::lock_guard<std::mutex> lock(mutex_);

      auto it = sessions_.find(token);
      if (it == sessions_.end())
         return -1;

      if (std::chrono::steady_clock::now() > it->second.expires)
      {
         sessions_.erase(it);
         return -1;
      }

      return it->second.userId;
   }

   void WebSessionManager::InvalidateSession(const std::string& token)
   {
      std::lock_guard<std::mutex> lock(mutex_);
      sessions_.erase(token);
   }

   void WebSessionManager::CleanupExpired()
   {
      std::lock_guard<std::mutex> lock(mutex_);
      auto now = std::chrono::steady_clock::now();

      for (auto it = sessions_.begin(); it != sessions_.end();)
      {
         if (now > it->second.expires)
            it = sessions_.erase(it);
         else
            ++it;
      }
   }
}
