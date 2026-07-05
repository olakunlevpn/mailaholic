[Icons]
Name: "{group}\Mailaholic Database Setup"; Filename: "{app}\Bin\DBSetup.exe"; Components: server;
Name: "{group}\Mailaholic Administrator"; Filename: "{app}\Bin\hMailAdmin.exe"; Components: admintools;
Name: "{group}\Addons\Data Directory Synchronizer"; Filename: "{app}\Addons\DataDirectorySynchronizer\DataDirectorySynchronizer.exe"; Components: server;
Name: "{group}\Installation\Uninstall Mailaholic"; Filename: "{app}\unins000.exe"; Components: admintools server;
Name: "{group}\Service\Start service"; Filename: "{sys}\net.exe"; Parameters: "START Mailaholic"; Components: server;
Name: "{group}\Service\Stop service"; Filename: "{sys}\net.exe"; Parameters: "STOP Mailaholic"; Components: server;
