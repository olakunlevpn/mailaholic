// Copyright (c) 2025 Mailaholic contributors (based on hMailServer).
// https://github.com/olakunlevpn/mailaholic

#pragma once

#include "Collection.h"

#include "..\Persistence\PersistentRuleCriteria.h"
#include "RuleCriteria.h"

namespace MA
{
   class RuleCriterias : public Collection<RuleCriteria, PersistentRuleCriteria>
   {
   public:
      RuleCriterias(__int64 iRuleID);
      ~RuleCriterias(void);

      void Refresh();

      __int64 GetRuleID() const {return rule_id_; }

   protected:
      virtual String GetCollectionName() const {return "RuleCriterias"; }
      virtual bool PreSaveObject(std::shared_ptr<RuleCriteria> pRuleCriteria, XNode *node);
   private:
      __int64 rule_id_;
   };
}