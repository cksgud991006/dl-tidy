#pragma once

#include <windows.h>

class Window {
    private:
        WNDCLASSW wc;
        static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

    public:
        bool init(HINSTANCE hInst);
        void hideWindow();
};