#pragma once

class Program {
public:
    bool running = true;
    
    Program() { }
    virtual ~Program() { }
    virtual void Run(void) { }
};