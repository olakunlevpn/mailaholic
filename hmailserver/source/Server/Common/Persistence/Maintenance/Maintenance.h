// Copyright (c) 2025 Mailaholic contributors (based on hMailServer).
// https://github.com/olakunlevpn/mailaholic

#pragma once


namespace MA
{
   class Maintenance
   {
   public:
	   Maintenance();
	   virtual ~Maintenance();

      enum MaintenanceOperation
      {
         RecalculateFolderUID = 1001 
      };

      bool Perform(MaintenanceOperation operation);

   private:

      bool RecalculateFolderUID_();
   };


}