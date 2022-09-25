#pragma once

#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/String.hpp>

#include <array>
#include <random>
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
    enum class AppMode { Select, RandomTest, SlowRunThrough };

    /* Misc Methods */
    bool loadRuneInfoFromDisk();
    void update(const sf::Time& dt);
    void updateRuneSprite();

    /* Select Mode Methods */
    void setupSelectMode();
    void drawSelectMode();
    void updateSelectMode();
    void handleEventSelectMode(const sf::Event& event);

    /* Random Test Mode Methods*/
    void setupRandomTestMode();
    void drawRandomTestMode();
    void updateRandomTestMode(const sf::Time& dt);
    void handleEventRandomTestMode(const sf::Event& event);
    void randomiseTestGuesses();

    /* Slow Run Through Methods */
    void setupSlowRunThroughMode();
    void drawSlowRunThroughMode();
    void updateSlowRunThroughMode(const sf::Time& dt);
    void handleEventSlowRunThroughMode(const sf::Event& event);

    /* Misc Members */
    sf::RenderWindow m_window;
    sf::Texture m_runeSheet;
    sf::Font m_mainFont;
    std::vector<Rune> m_runesInfo;
    size_t m_runeIndex { 0 };
    sf::Vector2f m_mousePosition;
    std::default_random_engine m_randomEngine;
    std::random_device m_randomDevice;
    sf::Sprite m_runeSprite;

    /* Select Mode Members */
    sf::Text m_selectModeTitleText;
    sf::Text m_selectRunThroughButton;
    sf::Text m_selectRandomTestButton;

    /* Random Test Mode Members */
    std::array<GuessButton, 3> m_guessButtons;
    size_t m_correctAnswerIndex { 0 };
    sf::Text m_questionText;
    AppMode m_currentMode { AppMode::Select };
    sf::Time m_postGuessTimer;
    bool m_guessMade { false };

    /* Slow Run Through Mode */
    sf::Text m_runeNameText;
    sf::Time m_slowModeTimer;
};
