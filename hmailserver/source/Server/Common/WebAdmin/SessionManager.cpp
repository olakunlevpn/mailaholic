#include "stdafx.h"
#include "SessionManager.h"
#include <random>
#include <sstream>
#include <iomanip>

namespace WebAdmin
{
   SessionManager& SessionManager::Instance()
   {
      static SessionManager instance;
      return instance;
   }

   std::string SessionManager::GenerateToken()
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

   std::string SessionManager::CreateSession(int userId)
   {
      std::lock_guard<std::mutex> lock(mutex_);

      std::string token = GenerateToken();
      auto expires = std::chrono::steady_clock::now() +
                     std::chrono::hours(SESSION_DURATION_HOURS);

      sessions_[token] = {userId, expires};
      return token;
   }

   int SessionManager::ValidateSession(const std::string& token)
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

   void SessionManager::InvalidateSession(const std::string& token)
   {
      std::lock_guard<std::mutex> lock(mutex_);
      sessions_.erase(token);
   }

   void SessionManager::CleanupExpired()
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
