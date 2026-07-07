#pragma once

#include <string>
#include <unordered_map>
#include <mutex>
#include <chrono>

namespace WebAdmin
{
   class SessionManager
   {
   public:
      static SessionManager& Instance();

      std::string CreateSession(int userId);
      int ValidateSession(const std::string& token);
      void InvalidateSession(const std::string& token);
      void CleanupExpired();

   private:
      SessionManager() = default;
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
