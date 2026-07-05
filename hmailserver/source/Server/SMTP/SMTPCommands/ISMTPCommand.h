// ISMTPCommand.h: interface for the ISMTPCommand class.
//
//////////////////////////////////////////////////////////////////////


#pragma once

namespace MA
{
   class SMTPConnection;

   class ISMTPCommand  
   {
   public:
	   ISMTPCommand();
	   virtual ~ISMTPCommand();

      virtual void ExecuteCommand(SMTPConnection* pSMTPConnection ) = 0;

   };
}
