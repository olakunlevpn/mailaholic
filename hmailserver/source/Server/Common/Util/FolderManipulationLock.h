// Copyright (c) 2025 Mailaholic contributors (based on hMailServer).
// https://github.com/olakunlevpn/mailaholic

#pragma once

namespace MA
{
   class FolderManipulationLock
   {
   public:
      FolderManipulationLock(int iAccountID, int iFolderID);
      ~FolderManipulationLock(void);

      void Lock();

   private:

      static void Acquire(std::pair<int, int> lockPair);
      static void Release(std::pair<int, int> lockPair);

      static std::set<std::pair<int, int> > folders_;
      
      static boost::recursive_mutex mutex_; 

      bool has_lock_;

      std::pair<int, int> lock_pair_;
   };
}