#include "Game.hpp"
#include "Company.hpp"
#include "Renderer.hpp"
#include "StartupMenu.hpp"
#include <SFML/Graphics.hpp>
#include <vector>

int main() {
    StartupMenu initialStartupMenu;
    initialStartupMenu.StartMenuLoop();

    return 0;
}