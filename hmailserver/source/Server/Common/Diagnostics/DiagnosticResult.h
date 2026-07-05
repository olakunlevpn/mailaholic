// Copyright (c) 2025 Mailaholic contributors (based on hMailServer).
// https://github.com/olakunlevpn/mailaholic

#pragma once

namespace MA
{

   class DiagnosticResult
   {
   public:
      
      DiagnosticResult(String name, String description, bool Success, String Details);
      DiagnosticResult();

      void SetName(const String &name) {name_ = name;}
      void SetDescription(const String &description) {description_ = description;}
      void SetDetails(const String &details) {details_ = details;}
      void SetSuccess(bool success) {success_ = success;}

      String GetName() const 
      {
         return name_;
      }

      String GetDescription() const
      {
         return description_;
      }

      String GetDetails() const
      {
         return details_;
      }

      bool GetSuccess() const
      {
         return success_;
      }

   private:
      
      String name_;
      String description_;
      bool success_;
      String details_;

   };


}
