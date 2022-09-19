#pragma once

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <array>

class RuneRemember {
public:
    RuneRemember();

    void run();

private:
    sf::RenderWindow m_window;
    sf::Texture m_runeSheet;
    sf::Sprite m_activeRune;
    size_t m_runeIndex { 0 };
};
