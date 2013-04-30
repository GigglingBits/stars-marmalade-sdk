[Setup]
OutputBaseFilename=Kidbook Installer
AppName=KIDBOOK animals
AppVerName=KIDBOOK animals
AppReadmeFile=help.txt
DefaultGroupName=Kidbooks
DefaultDirName={pf}\KIDBOOK animals
UninstallDisplayIcon=Kidbook.exe
Compression=lzma
SolidCompression=yes
WizardImageFile=C:/Users/pan/Desktop/Development/GigglingBits/Kidbook/marketing/splashscreen-320x480.bmp
SourceDir=c:\Users\pan\Desktop\Development\GigglingBits\Kidbook\build_kidbook_vc10\deployments\Windows(x86)\win32\release
OutputDir=c:\Users\pan\Desktop\Development\GigglingBits\Kidbook\build_kidbook_vc10\deployments\Windows(x86)\win32
SetupIconFile=C:/Users/pan/Desktop/Development/GigglingBits/Kidbook/marketing/icons/icon.ICO
[Messages]
WelcomeLabel2=KIDBOOK animals is a fun picture book for children to discover animals and their sounds. It contains 80 different animals, with high quality images that look great on any display. Children from 1 to 6 years will enjoy exploring the app.
[Files]
Source: "*.*"; Excludes: "Setup.iss"; DestDir: "{app}"; Flags:replacesameversion
Source: "angle\*.*"; Excludes: ""; DestDir: "{app}\angle"; Flags:replacesameversion
Source: "buttons\*.*"; Excludes: ""; DestDir: "{app}\buttons"; Flags:replacesameversion
Source: "fonts\*.*"; Excludes: ""; DestDir: "{app}\fonts"; Flags:replacesameversion
Source: "images\*.*"; Excludes: ""; DestDir: "{app}\images"; Flags:replacesameversion
Source: "sounds\*.*"; Excludes: ""; DestDir: "{app}\sounds"; Flags:replacesameversion
Source: "thumbnails\*.*"; Excludes: ""; DestDir: "{app}\thumbnails"; Flags:replacesameversion
[Icons]
Name: "{group}\KIDBOOK animals"; Filename: "{app}\Kidbook.exe"
Name: "{group}\Uninstall KIDBOOK animals"; Filename: "{uninstallexe}"
