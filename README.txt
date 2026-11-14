HPJ INVENTORY - PORTABLE WINDOWS APP BUILD KIT

What this creates
- HPJ_Inventory.exe: native Windows window hosting the supplied inventory HTML
- HPJ.ico: application icon embedded into the EXE
- Inventory.html: the existing updated inventory application
- HPJ_Data\WebView2: portable WebView2 profile/data folder

Target PC requirements
- Windows 10/11 x64
- Microsoft Edge WebView2 Evergreen Runtime installed
- No separate installation of the HPJ Inventory app is needed

Build requirement (only on the PC used to compile)
- Visual Studio 2022 C++ Build Tools with Windows SDK
- Internet connection for downloading NuGet/WebView2 SDK on first build

Build steps
1. Extract this folder on a Windows development PC.
2. Open "x64 Native Tools Command Prompt for VS 2022".
3. Change directory to this folder.
4. Run: Build-Windows.bat
5. On success, the portable app is in the output folder.
6. Copy the entire output folder to the target PC/USB drive. Do not separate the EXE from Inventory.html.

Data portability
- WebView2 user profile is stored under HPJ_Data beside the EXE.
- Keep the HPJ_Data folder when backing up or moving the app to another PC.

Important
- The EXE is designed to be small and uses the already-installed WebView2 Runtime.
- The runtime itself is not bundled. If WebView2 Runtime is missing on the target PC, the app will show an error instead of opening.
- This kit preserves the supplied HTML application; it does not rewrite its inventory/business logic.
