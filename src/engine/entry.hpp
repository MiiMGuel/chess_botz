#pragma once

namespace Engine {
    class Program {
    protected:
        bool running = true;
    public:
        Program() { }
        virtual ~Program() { }
        virtual void Run(void) { }
        const bool IsRunning(void) { return running; }
    };

    Program* MainProgram(int argc, char* argv[]);
}