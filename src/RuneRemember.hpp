#pragma once

#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/String.hpp>
#include <vector>

class RuneRemember {
public:
    RuneRemember();
    void run();

private:
    struct Rune {
        size_t index;
        sf::String name;
    };

    bool loadRuneInfoFromDisk();

    sf::RenderWindow m_window;
    sf::Texture m_runeSheet;
    sf::Sprite m_activeRune;
    sf::Text m_questionText;
    sf::Font m_mainFont;
    std::vector<Rune> m_runesInfo;
    size_t m_runeIndex { 0 };
};
