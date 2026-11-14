#define UNICODE
#define _UNICODE
#include <windows.h>
#include <shlobj.h>
#include <shlwapi.h>
#include <string>
#include <filesystem>
#include <wrl.h>
#include "WebView2.h"
#include "resource.h"

using namespace Microsoft::WRL;
namespace fs = std::filesystem;
static HWND g_hwnd = nullptr;
static ComPtr<ICoreWebView2Controller> g_controller;
static ComPtr<ICoreWebView2> g_webview;
static fs::path g_appDir;

static void ResizeWebView() {
    if (!g_controller || !g_hwnd) return;
    RECT r{}; GetClientRect(g_hwnd, &r);
    g_controller->put_Bounds(r);
}

static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp) {
    switch (msg) {
    case WM_SIZE: ResizeWebView(); return 0;
    case WM_SETFOCUS:
        if (g_controller) g_controller->MoveFocus(COREWEBVIEW2_MOVE_FOCUS_REASON_PROGRAMMATIC);
        break;
    case WM_DESTROY: PostQuitMessage(0); return 0;
    }
    return DefWindowProc(hwnd, msg, wp, lp);
}

int WINAPI wWinMain(HINSTANCE hInst, HINSTANCE, PWSTR, int nCmdShow) {
    wchar_t exePath[MAX_PATH]{};
    GetModuleFileNameW(nullptr, exePath, MAX_PATH);
    g_appDir = fs::path(exePath).parent_path();
    const fs::path htmlPath = g_appDir / L"Inventory.html";
    const fs::path dataDir = g_appDir / L"HPJ_Data";
    const fs::path profileDir = dataDir / L"WebView2";
    std::error_code ec;
    fs::create_directories(profileDir, ec);

    if (!fs::exists(htmlPath)) {
        MessageBoxW(nullptr, L"Inventory.html file is missing. Keep it in the same folder as HPJ_Inventory.exe.", L"HPJ Inventory", MB_ICONERROR | MB_OK);
        return 1;
    }

    const wchar_t CLASS_NAME[] = L"HPJInventoryPortableWindow";
    WNDCLASSEXW wc{ sizeof(wc) };
    wc.lpfnWndProc = WndProc; wc.hInstance = hInst; wc.lpszClassName = CLASS_NAME;
    wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wc.hIcon = LoadIconW(hInst, MAKEINTRESOURCEW(IDI_APP));
    wc.hIconSm = wc.hIcon;
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    RegisterClassExW(&wc);

    g_hwnd = CreateWindowExW(0, CLASS_NAME, L"SRIRAM GEMS & JEWELLERY - Inventory", WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 1280, 820, nullptr, nullptr, hInst, nullptr);
    if (!g_hwnd) return 1;
    ShowWindow(g_hwnd, nCmdShow); UpdateWindow(g_hwnd);

    HRESULT hr = CreateCoreWebView2EnvironmentWithOptions(
        nullptr, profileDir.c_str(), nullptr,
        Callback<ICoreWebView2CreateCoreWebView2EnvironmentCompletedHandler>(
            [](HRESULT result, ICoreWebView2Environment* env) -> HRESULT {
                if (FAILED(result) || !env) {
                    MessageBoxW(g_hwnd, L"Microsoft Edge WebView2 Runtime is required. Install the Evergreen WebView2 Runtime and reopen this app.", L"HPJ Inventory", MB_ICONERROR | MB_OK);
                    return S_OK;
                }
                env->CreateCoreWebView2Controller(g_hwnd,
                    Callback<ICoreWebView2CreateCoreWebView2ControllerCompletedHandler>(
                        [](HRESULT result2, ICoreWebView2Controller* controller) -> HRESULT {
                            if (FAILED(result2) || !controller) return S_OK;
                            g_controller = controller;
                            g_controller->get_CoreWebView2(&g_webview);
                            ComPtr<ICoreWebView2Settings> settings;
                            if (SUCCEEDED(g_webview->get_Settings(&settings)) && settings) {
                                settings->put_IsStatusBarEnabled(FALSE);
                                settings->put_AreDefaultContextMenusEnabled(TRUE);
                                settings->put_IsZoomControlEnabled(TRUE);
                            }
                            g_webview->SetVirtualHostNameToFolderMapping(L"hpj.local", g_appDir.c_str(), COREWEBVIEW2_HOST_RESOURCE_ACCESS_KIND_ALLOW);
                            ResizeWebView();
                            g_webview->Navigate(L"https://hpj.local/Inventory.html");
                            return S_OK;
                        }).Get());
                return S_OK;
            }).Get());
    if (FAILED(hr)) {
        MessageBoxW(g_hwnd, L"Could not start WebView2. Please verify the WebView2 Runtime is installed.", L"HPJ Inventory", MB_ICONERROR | MB_OK);
        return 1;
    }

    MSG msg{};
    while (GetMessageW(&msg, nullptr, 0, 0)) { TranslateMessage(&msg); DispatchMessageW(&msg); }
    return (int)msg.wParam;
}
