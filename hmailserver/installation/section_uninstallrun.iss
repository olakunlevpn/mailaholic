[UninstallRun]
Filename: "{app}\Bin\Mailaholic.exe"; Parameters: "/Unregister"; Flags: runhidden;
Filename: "{sys}\net.exe"; Parameters: "STOP Mailaholic"; Flags: runhidden;
Filename: "{sys}\net.exe"; Parameters: "STOP MailaholicMySQL"; Flags: runhidden;
Filename: "{app}\MySQL\Bin\mysqld-nt.exe"; Parameters: "--remove MailaholicMySQL"; Flags: runhidden;
Filename: "{app}\Bin\hSMTPServer.exe"; Parameters: "unregister"; Flags: runhidden;
Filename: "{app}\Bin\hPOP3Server.exe"; Parameters: "unregister"; Flags: runhidden;
