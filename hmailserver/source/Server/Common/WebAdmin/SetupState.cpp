#include "stdafx.h"
#include "SetupState.h"
#include "../Application/IniFileSettings.h"

namespace WebAdmin
{
   SetupState& SetupState::Instance()
   {
      static SetupState instance;
      return instance;
   }

   bool SetupState::IsComplete() const
   {
      return MA::IniFileSettings::Instance()->GetSetupComplete();
   }

   void SetupState::MarkComplete()
   {
      MA::IniFileSettings::Instance()->SetSetupComplete(true);
   }
}
