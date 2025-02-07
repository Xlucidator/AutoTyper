#include <windows.h>
#include <iostream>
#include <psapi.h>  // 需要用于 GetModuleFileNameEx

void printActiveWindowInfo() {
    // 获取前台窗口
    HWND hwnd = GetForegroundWindow();
    if (!hwnd) {
        std::cerr << "Error: 无法获取前台窗口!" << std::endl;
        return;
    }

    // 获取窗口标题
    char windowTitle[256] = {0};
    GetWindowTextA(hwnd, windowTitle, sizeof(windowTitle));

    // 获取窗口类名
    char className[256] = {0};
    GetClassNameA(hwnd, className, sizeof(className));

    // 获取窗口所属进程 ID
    DWORD pid = 0;
    GetWindowThreadProcessId(hwnd, &pid);

    // 获取进程路径
    char processPath[MAX_PATH] = {0};
    HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, pid);
    if (hProcess) {
        GetModuleFileNameExA(hProcess, NULL, processPath, MAX_PATH);
        CloseHandle(hProcess);
    }

    // 打印窗口信息
    std::cout << "窗口句柄: " << hwnd << std::endl;
    std::cout << "窗口标题: " << windowTitle << std::endl;
    std::cout << "窗口类名: " << className << std::endl;
    std::cout << "进程 ID: " << pid << std::endl;
    std::cout << "进程路径: " << processPath << std::endl;
}

int main() {
    SetConsoleOutputCP(CP_UTF8); // 设置控制台输出编码为 UTF-8
    SetConsoleCP(CP_UTF8);       // 设置控制台输入编码为 UTF-8
    Sleep(3000);
    printActiveWindowInfo();
    return 0;
}