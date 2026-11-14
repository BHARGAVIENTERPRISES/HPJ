HPJ INVENTORY - ONLINE WINDOWS EXE BUILD

This folder is prepared for GitHub Actions cloud compilation. You do NOT need Visual Studio or C++ Build Tools on your own computer.

STEPS
1. Sign in to https://github.com (create a free account if needed).
2. Create a new PRIVATE repository. Example name: hpj-inventory-portable
3. Upload the CONTENTS of this HPJ_Portable_BuildKit folder into the repository root (not the parent folder).
   Important: upload the hidden .github folder too. It contains .github/workflows/build-windows.yml.
4. Open the repository's Actions tab.
5. Select "Build HPJ Inventory Portable EXE" from the left side.
6. Click "Run workflow" and confirm "Run workflow".
7. Wait for the run to finish with a green check mark.
8. Open that completed run and download the artifact named:
   HPJ-Inventory-Portable-Windows
9. Extract the downloaded ZIP. Run HPJ_Inventory.exe. Keep Inventory.html and the HPJ_Data folder beside it.

TARGET PC REQUIREMENT
- Windows 10/11 x64
- Microsoft Edge WebView2 Evergreen Runtime must already be installed.
- No installation of the HPJ Inventory app is required.

NOTES
- The cloud build uses Microsoft's hosted Windows build environment and downloads the WebView2 SDK during compilation.
- The final EXE size is not guaranteed to be under 10 MB until the cloud build completes; the portable ZIP also contains Inventory.html and runtime data folder is created on first launch.
- Keep the whole extracted app folder together. Back up HPJ_Data to preserve the app's local browser profile/data.
- Keep the GitHub repository Private because it contains your inventory application source.
