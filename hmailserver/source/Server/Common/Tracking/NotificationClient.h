// Copyright (c) 2025 Mailaholic contributors (based on hMailServer).
// https://github.com/olakunlevpn/mailaholic

#pragma once

namespace MA
{
   class ChangeNotification;

   class NotificationClient
   {
   public:
      NotificationClient();

      virtual void OnNotification(std::shared_ptr<ChangeNotification> notification) = 0;
   };
}
