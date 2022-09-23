#pragma once

#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/String.hpp>
#include <array>
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
    
    struct GuessButton {
        sf::Text text;
        sf::RectangleShape backdrop;
    };
    enum class AppMode { Select, RandomTest, SlowRunThrough, Complete };

    /* Misc Methods */
    bool loadRuneInfoFromDisk();
    void update(const sf::Time& dt);

    /* Select Mode Methods */
    void setupSelectMode();
    void drawSelectMode();
    void updateSelectMode();
    void handleEventSelectMode(const sf::Event& event);

    /* Random Test Mode Methods*/
    void setupRandomTestMode();
    void drawRandomTestMode();
    void updateRandomTestMode(const sf::Time& dt);

    /* Misc Members */
    sf::RenderWindow m_window;
    sf::Texture m_runeSheet;
    sf::Font m_mainFont;
    std::vector<Rune> m_runesInfo;
    size_t m_runeIndex { 0 };
    sf::Vector2f m_mousePosition;

    /* Select Mode Members */
    sf::Text m_selectModeTitleText;
    sf::Text m_selectRunThroughButton;
    sf::Text m_selectRandomTestButton;

    /* Random Test Mode Members */
    std::array<GuessButton, 3> m_guessButtons;
    sf::Text m_questionText;

    sf::Sprite m_activeRune;

    AppMode m_currentMode { AppMode::Select };
};
