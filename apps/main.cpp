#include "app.h"

int WINAPI wWinMain(HINSTANCE hInst, HINSTANCE, PWSTR, int) {

    App app;

    if (!app.init(hInst)) {
        MessageBoxW(nullptr, L"Failed to application", L"Error", MB_OK);
        return -1;
    }

    app.runTasks();

    return 0;
}