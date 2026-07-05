// Copyright (c) 2025 Mailaholic contributors (based on hMailServer).
// https://github.com/olakunlevpn/mailaholic

#pragma once

namespace MA
{
   class NotificationClient;

   class NotificationClientSubscription
   {
   public:
      NotificationClientSubscription(__int64 subscriptionKey, std::weak_ptr<NotificationClient> client);
      virtual ~NotificationClientSubscription();

      __int64 GetSubscriptionKey();
      std::weak_ptr<NotificationClient> GetSubscribedClient();

   private:

      __int64 subscription_key_;
      std::weak_ptr<NotificationClient> client_;

   };
}
