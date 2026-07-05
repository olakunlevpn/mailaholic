// Copyright (c) 2025 Mailaholic contributors (based on hMailServer).
// https://github.com/olakunlevpn/mailaholic

#include "Collection.h"

#include "../Persistence/PersistentRouteAddress.h"
#include "RouteAddress.h"

namespace MA
{
   class RouteAddresses : public Collection<RouteAddress, PersistentRouteAddress>
   {
   public:
	   RouteAddresses(__int64 iRouteID);
	   virtual ~RouteAddresses();

      void Refresh();

      void DeleteByAddress(const String &sAddress);

      __int64 GetRouteID() {return route_id_; }
      
   protected:
      virtual String GetCollectionName() const {return "RouteAddresses"; }
      bool PreSaveObject(std::shared_ptr<RouteAddress> routeAddress, XNode *node);

   private:
      __int64 route_id_;
   };

}
