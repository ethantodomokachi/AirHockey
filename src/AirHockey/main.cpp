#include "pch.h"
#include "main.h"
#include "App.h"

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR cmdLine, int cmdShow)
{
    CPU_RUN(1024, 512);
    return 0;
}