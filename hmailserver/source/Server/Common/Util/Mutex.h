// Mutex.h: interface for the Mutex class.
//
//////////////////////////////////////////////////////////////////////

#pragma once


namespace MA
{
   class Mutex  
   {
   public:
	   Mutex();
	   virtual ~Mutex();

      void Wait();
      void Release();

   private:
      HANDLE mutex_;
   };

}
