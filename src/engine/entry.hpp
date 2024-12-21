#pragma once

class Program {
protected:
    bool running = true;
public:
    Program() { }
    virtual ~Program() { }
    virtual void Run(void) { }
    const bool IsRunning(void) { return running; }
};

#ifdef ENTRY_IMPL
#undef ENTRY_IMPL

#include <iostream>
#include <stdexcept>

#ifndef NOMINMAX
#   define NOMINMAX
#   define _NOMINMAX
#endif
#ifndef WIN32_LEAN_AND_MEAN
#   define WIN32_LEAN_AND_MEAN
#   define _WIN32_LEAN_AND_MEAN
#endif
#include <Windows.h>
#ifdef _NOMINMAX
#   undef NOMINMAX
#   undef _NOMINMAX
#endif
#ifdef _WIN32_LEAN_AND_MEAN
#   undef WIN32_LEAN_AND_MEAN
#   undef _WIN32_LEAN_AND_MEAN
#endif

Program* MainProgram(int argc, char* argv[]);

int WINAPI WinMain(
    HINSTANCE hInstance, 
    HINSTANCE hPrevInstance,
    PSTR lpCmdLine, 
    int nCmdShow
) {
    Program* main = nullptr;

    try { 
        main = MainProgram(__argc, __argv);
        if (main == nullptr)
            throw std::runtime_error("Failed to initialize the main program.");
        
        while (main->IsRunning())
            main->Run();
    } catch(const std::exception& e) {
        std::cerr << e.what() << '\n';
    }
    
    delete main;
    return 0;
}

#endif // ENTRY_IMPL