#include "Include/Window.hpp"
#include "Include/GUI.hpp"

// Main code
int main(int, char**)
{
    Window wnd;
    GUI ui;

    while (!false)
    {
        wnd.frame_start();
        
        wnd.render();
        ui.render();
        
        wnd.end_start();
    }

    return 0;
}
