#include <iostream>
#include <stdexcept>

#include "entry.hpp"

Program* main_program(int argc, char* argv[]);

int main(int argc, char* argv[]) {
    Program* main = nullptr;

    try { 
        main = main_program(argc, argv);
        if (main == nullptr)
            throw std::runtime_error("Failed to initialize the main program.");
        
        while (main->running)
            main->Run();
    } catch(const std::exception& e) {
        std::cerr << e.what() << '\n';
    }
    
    delete main;
    return 0;
}