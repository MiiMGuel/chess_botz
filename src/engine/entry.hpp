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

Program* MainProgram(int argc, char* argv[]);

int main(int argc, char* argv[]) {
    Program* main = nullptr;

    try { 
        main = MainProgram(argc, argv);
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