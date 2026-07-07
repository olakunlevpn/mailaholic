#pragma once

#include <string>
#include <unordered_map>
#include <mutex>
#include <chrono>

namespace WebAdmin
{
   class WebSessionManager
   {
   public:
      static WebSessionManager& Instance();

      std::string CreateSession(int userId);
      int ValidateSession(const std::string& token);
      void InvalidateSession(const std::string& token);
      void CleanupExpired();

   private:
      WebSessionManager() = default;
      std::string GenerateToken();

      struct Session
      {
         int userId;
         std::chrono::steady_clock::time_point expires;
      };

      std::unordered_map<std::string, Session> sessions_;
      std::mutex mutex_;

      static constexpr int SESSION_DURATION_HOURS = 24;
   };
}
