#include "Include/Window.hpp"
#include "Include/GUI.hpp"

// Main code
int main(int, char**)
{
    Window wnd;
    GUI ui(&wnd);

    while (!false)
    {
        wnd.frame_start();
        
        ui.render();
        wnd.render();
        
        wnd.end_start();
    }

    return 0;
}
