#include "Include/Window.hpp"

// Main code
int main(int, char**)
{
    Window wnd(1280, 720, L"Name");

    while (!false)
    {
        wnd.frame_start();
        wnd.render();
        wnd.end_start();
    }

    return 0;
}
