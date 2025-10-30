; ===== DL-Tidy Inno Setup script =====
#define AppName       "DL-Tidy"
#define AppVersion    "1.0.0"
#define AppPublisher  "cksgud991006"
#define AppExeName    "Dl-Tidy.exe"

; Build outputs we’ll look for (MinGW)
#define MyAppExe "Dl-Tidy.exe"
#pragma message SourcePath: {#SourcePath}
#ifexist SourcePath + "\\build\\" + MyAppExe
  #define BuildBin SourcePath + "\\build\\"
#else
  #error Could not find build output at compile time. Build Release or adjust BuildBin.
#endif

; Pick an icon for the installer UI (optional)
; If you have a standalone .ico, set: #define InstallerIco "resources\\app.ico"
; Otherwise we’ll reuse the EXE’s icon at compile time.
#ifndef InstallerIco
  #define InstallerIco "assets\\dl-tidy.ico"
#endif

[Setup]
AppId={{427E5720-3E70-4C2B-B272-A0F13DF2ADA8}       ; generate once and keep stable
AppName={#AppName}
AppVersion={#AppVersion}
AppPublisher={#AppPublisher}
DefaultDirName={pf}\{#AppName}
DefaultGroupName={#AppName}
OutputBaseFilename={#AppName}-Setup-{#AppVersion}
Compression=lzma
SolidCompression=yes
WizardStyle=modern
SetupIconFile={#InstallerIco}
ArchitecturesInstallIn64BitMode=x64
DisableProgramGroupPage=yes
UninstallDisplayIcon={app}\bin\{#AppExeName}
UninstallDisplayName={#AppName} {#AppVersion}

; If you want per-user install without admin, uncomment:
; PrivilegesRequired=lowest
; UsePreviousPrivileges=yes

[Languages]
Name: "english"; MessagesFile: "compiler:Default.isl"

[Files]
; --- Main executable: include only the one that exists ---
Source: "{#BuildBin}\{#MyAppExe}"; DestDir: "{app}\bin"; DestName: "{#AppExeName}"; Flags: ignoreversion;

; --- Optional runtime/data files: add or remove as you need ---
; Examples: configs, license, README, icons you want to ship, etc.
Source: "LICENSE"; DestDir: "{app}"; Flags: ignoreversion;

[Icons]
; Start Menu shortcut
Name: "{group}\{#AppName}"; Filename: "{app}\bin\{#AppExeName}"

; Desktop shortcut (optional — behind a task)
Name: "{commondesktop}\{#AppName}"; Filename: "{app}\bin\{#AppExeName}"; Tasks: desktopicon

; Startup shortcut (optional — runs at login)
Name: "{userstartup}\{#AppName}"; Filename: "{app}\bin\{#AppExeName}"; Tasks: startup

[Tasks]
Name: "desktopicon"; Description: "Create a &desktop icon"; GroupDescription: "Additional icons:"
Name: "startup"; Description: "Run {#AppName} on &startup"; GroupDescription: "Autostart:"

[Run]
; Offer to launch after install
Filename: "{app}\bin\{#AppExeName}"; Description: "Launch {#AppName}"; Flags: nowait postinstall skipifsilent

[UninstallRun]
; (Optional) Stop background/tray instance gracefully on uninstall if you add a --shutdown flag
; Filename: "{app}\bin\{#AppExeName}"; Parameters: "--shutdown"; Flags: skipifdoesntexist

[Code]
function InitializeSetup(): Boolean;
begin
  Result := True;
end;
