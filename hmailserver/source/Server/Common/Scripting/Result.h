// Copyright (c) 2025 Mailaholic contributors (based on hMailServer).
// https://github.com/olakunlevpn/mailaholic

#pragma once


namespace MA
{
   class Result 
   {
   public:
      Result(void);
      ~Result(void);

      void SetValue(long lNewVal){value_ = lNewVal; }
      long GetValue() const; 

      void SetParameter(int lNewVal){parameter_ = lNewVal; }
      int GetParameter() const; 


      void SetMessage(const String& sValue){message_ = sValue; }
      String GetMessage() const;

   private:
      long value_;
      int parameter_;

      String message_;
   };
}