[Setup]
AppName=Blendwerk Procesador de CFDI
AppVersion=0.1.0
AppPublisher=Blendwerk
AppPublisherURL=https://github.com/fernandoagomezf/app_cfdi_cpp
DefaultDirName={autopf}\Blendwerk CFDI
DefaultGroupName=Blendwerk
UninstallDisplayIcon={app}\CFDI.exe
Compression=lzma2
SolidCompression=yes
OutputDir=..\build
OutputBaseFilename=BlendwerkCFDI-v0.1.0
WizardStyle=modern
PrivilegesRequired=admin
ArchitecturesAllowed=x64compatible
ArchitecturesInstallIn64BitMode=x64compatible

[Languages]
Name: "spanish"; MessagesFile: "compiler:Languages\Spanish.isl"
Name: "english"; MessagesFile: "compiler:Default.isl"

[Tasks]
Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}"

[Files]
Source: "..\build\msvc\Release\CFDI.exe"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\build\msvc\Release\*.dll"; DestDir: "{app}"; Flags: ignoreversion

[Icons]
Name: "{group}\Procesador de CFDI"; Filename: "{app}\CFDI.exe"
Name: "{group}\{cm:UninstallProgram,Procesador de CFDI}"; Filename: "{uninstallexe}"
Name: "{autodesktop}\Procesador de CFDI"; Filename: "{app}\CFDI.exe"; Tasks: desktopicon

[Run]
Filename: "{app}\CFDI.exe"; Description: "{cm:LaunchProgram,Procesador de CFDI}"; Flags: nowait postinstall skipifsilent

[Code]
procedure CurStepChanged(CurStep: TSetupStep);
begin
  if CurStep = ssPostInstall then
  begin
    // Add any post-install tasks here
  end;
end;