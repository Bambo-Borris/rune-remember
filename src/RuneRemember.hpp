#pragma once

#include <SFML/Graphics/RenderWindow.hpp>

class RuneRemember {
public:
    RuneRemember();

    void run();

private:
    sf::RenderWindow m_window;
};
