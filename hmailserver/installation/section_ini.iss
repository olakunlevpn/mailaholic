[INI]
Filename: "{app}\Bin\Mailaholic.ini"; Section: "Directories"; Key: "ProgramFolder"; String: "{app}";
Filename: "{app}\Bin\Mailaholic.ini"; Section: "Directories"; Key: "DatabaseFolder"; String: "{app}\Database";  Flags: createkeyifdoesntexist; Components: server;
Filename: "{app}\Bin\Mailaholic.ini"; Section: "Directories"; Key: "DataFolder"; String: "{app}\Data";  Flags: createkeyifdoesntexist; Components: server;
Filename: "{app}\Bin\Mailaholic.ini"; Section: "Directories"; Key: "LogFolder"; String: "{app}\Logs"; Flags: createkeyifdoesntexist; Components: server;
Filename: "{app}\Bin\Mailaholic.ini"; Section: "Directories"; Key: "TempFolder"; String: "{app}\Temp"; Flags: createkeyifdoesntexist; Components: server;
Filename: "{app}\Bin\Mailaholic.ini"; Section: "Directories"; Key: "EventFolder"; String: "{app}\Events"; Flags: createkeyifdoesntexist; Components: server;

; Languages
Filename: "{app}\Bin\Mailaholic.ini"; Section: "GUILanguages"; Key: "ValidLanguages"; String: "english,swedish";
Filename: "{app}\Bin\Mailaholic.ini"; Section: "Security"; Key: "AdministratorPassword"; String: "{code:GetHashedPassword}"; Flags: createkeyifdoesntexist; Components: server;
