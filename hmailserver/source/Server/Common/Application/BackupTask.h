// Copyright (c) 2025 Mailaholic contributors (based on hMailServer).
// https://github.com/olakunlevpn/mailaholic

#pragma once

#include "..\Threading\Task.h"

namespace MA
{
   class BackupTask : public Task
   {
   public:
      BackupTask(bool bDoBackup);
      ~BackupTask(void);

      virtual void DoWork();

      void SetBackupToRestore(std::shared_ptr<Backup> pBackup);

   private:

      bool do_backup_;
      
      std::shared_ptr<Backup> backup_;
   };
}