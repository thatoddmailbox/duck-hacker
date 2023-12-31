[Setup]
AppName=Duck Hacker
AppVersion=1.0
VersionInfoVersion=1.0
WizardStyle=modern
DefaultDirName={autopf}\Dogo\Duck Hacker
DefaultGroupName=Dogo
OutputBaseFilename=DuckHackerSetup
UninstallDisplayIcon={app}\duck-hacker.exe
Compression=bzip
SolidCompression=yes
ArchitecturesAllowed=x64
ArchitecturesInstallIn64BitMode=x64

[Files]
Source: "Input/duck-hacker.exe"; DestDir: "{app}"
Source: "Input/data.zip"; DestDir: "{app}"
Source: "Input/libpng16.dll"; DestDir: "{app}"
Source: "Input/lua5.4.6.dll"; DestDir: "{app}"
Source: "Input/physfs.dll"; DestDir: "{app}"
Source: "Input/SDL2.dll"; DestDir: "{app}"
Source: "Input/SDL2_mixer.dll"; DestDir: "{app}"
Source: "Input/wxbase32u_vc14x_x64.dll"; DestDir: "{app}"
Source: "Input/wxmsw32u_core_vc14x_x64.dll"; DestDir: "{app}"
Source: "Input/wxmsw32u_stc_vc14x_x64.dll"; DestDir: "{app}"

[Icons]
Name: "{group}\Duck Hacker"; Filename: "{app}\duck-hacker.exe"
