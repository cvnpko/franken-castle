#include <app/Main.hpp>
#include <engine/core/Engine.hpp>

namespace engine::main::app {
void MainApp::app_setup() {
}
}// namespace engine::main::app
int main(int argc, char **argv) {
    return std::make_unique<engine::main::app::MainApp>()->run(argc, argv);
}
