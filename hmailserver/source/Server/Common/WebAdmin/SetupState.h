#pragma once

#include <string>

namespace WebAdmin
{
   enum class UsageType
   {
      Personal = 0,
      Organization = 1,
      HighVolume = 2
   };

   class SetupState
   {
   public:
      static SetupState& Instance();

      int GetStep() const { return step_; }
      void SetStep(int step) { step_ = step; }

      std::string GetDomain() const { return domain_; }
      void SetDomain(const std::string& domain) { domain_ = domain; }

      UsageType GetUsageType() const { return usageType_; }
      void SetUsageType(UsageType type) { usageType_ = type; }

      std::string GetAdminEmail() const { return adminEmail_; }
      void SetAdminEmail(const std::string& email) { adminEmail_ = email; }

      std::string GetAdminPassword() const { return adminPassword_; }
      void SetAdminPassword(const std::string& password) { adminPassword_ = password; }

      bool IsComplete() const;
      void MarkComplete();

   private:
      SetupState() = default;

      int step_{1};
      std::string domain_;
      UsageType usageType_{UsageType::Personal};
      std::string adminEmail_;
      std::string adminPassword_;
   };
}
