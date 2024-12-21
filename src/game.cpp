#include <stdexcept>

#define ENTRY_IMPL
#include "engine/types.hpp"
#include "engine/entry.hpp"

class Game : public Program {
public:
    Game(int argc, char* argv[]) {
    }

    ~Game() override {
    }

    void Run(void) override {
        running = false;
    }
};

Program* MainProgram(int argc, char* argv[]) {
    Program* main = new Game(argc, argv);
    return main;
}