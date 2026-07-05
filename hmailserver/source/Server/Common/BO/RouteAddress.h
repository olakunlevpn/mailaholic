// Copyright (c) 2025 Mailaholic contributors (based on hMailServer).
// https://github.com/olakunlevpn/mailaholic

#pragma once

namespace MA
{
   class RouteAddress : public BusinessObject<RouteAddress>
   {
   public:
	   RouteAddress();
	   virtual ~RouteAddress();

      String GetName() const {return address_; }

      __int64 GetRouteID() const {return route_id_;}
      void SetRouteID(__int64 iNewVal) {route_id_ = iNewVal; }

      String GetAddress() const {return address_; }
      void SetAddress(const String &sAddress) {address_ = sAddress; } 
   
      bool XMLStore(XNode *pNode, int iOptions);
      bool XMLLoad(XNode *pNode, int iOptions);
      bool XMLLoadSubItems(XNode *pNode, int iOptions) {return true;}

   protected:

      __int64 route_id_;
      String address_;
      

   };
}