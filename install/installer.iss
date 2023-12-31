[Setup]
AppName=Duck Hacker
AppVersion=1.0
WizardStyle=modern
DefaultDirName={autopf}\Dogo\Duck Hacker
DefaultGroupName=Dogo
UninstallDisplayIcon={app}\duck-hacker.exe
Compression=lzma2
SolidCompression=yes
; OutputDir=userdocs:Inno Setup Examples Output

[Files]
Source: "duck-hacker.exe"; DestDir: "{app}"
Source: "data.zip"; DestDir: "{app}"
Source: "libpng16.dll"; DestDir: "{app}"
Source: "lua5.4.6.dll"; DestDir: "{app}"
Source: "physfs.dll"; DestDir: "{app}"
Source: "SDL2.dll"; DestDir: "{app}"
Source: "SDL2_mixer.dll"; DestDir: "{app}"
Source: "wxbase32u_vc14x_x64.dll"; DestDir: "{app}"
Source: "wxmsw32u_core_vc14x_x64.dll"; DestDir: "{app}"
Source: "wxmsw32u_stc_vc14x_x64.dll"; DestDir: "{app}"

[Icons]
Name: "{group}\Duck Hacker"; Filename: "{app}\duck-hacker.exe"
