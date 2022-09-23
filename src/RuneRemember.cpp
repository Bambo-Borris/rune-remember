#include "RuneRemember.hpp"

#include <SFML/Graphics/Text.hpp>
#include <SFML/System/String.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/VideoMode.hpp>
#include <spdlog/spdlog.h>

#include <cassert>
#include <fstream>

constexpr auto RUNE_IMAGE_SIZE { sf::Vector2i { 32, 32 } };
constexpr auto BUTTON_HOVER_COLOR { sf::Color { 237 / 2, 233 / 2, 149 / 2 } };
constexpr auto HOVER_BUTTON_FONT_SIZE { 34u };
constexpr auto BUTTON_FONT_SIZE { 32u };
constexpr auto TITLE_TEXT_FONT_SIZE { 48u };
constexpr auto GUESS_BUTTON_BACKDROP_SIZE { sf::Vector2f { 150.f, 50.f } };
constexpr auto TITLE_TEXT_Y_OFFSET { 100.f };
constexpr auto POST_GUESS_WAIT { sf::seconds(3.f) };

inline void CentreTextOrigin(sf::Text& text)
{
    const auto localBounds = sf::Vector2f(text.getLocalBounds().left, text.getLocalBounds().top);
    const auto globalOrigin = sf::Vector2f(text.getGlobalBounds().width / 2.0f, text.getGlobalBounds().height / 2.0f);

    text.setOrigin(localBounds + globalOrigin);
}

RuneRemember::RuneRemember()
    : m_randomEngine(m_randomDevice())
{
    spdlog::set_level(spdlog::level::debug);

    sf::ContextSettings ctxt;
    ctxt.antialiasingLevel = 16;
    m_window.create(sf::VideoMode({ 800, 600 }), "Rune Remember!", sf::Style::Default, ctxt);
    m_window.setFramerateLimit(30);

    // Perform our disk operations
    // TODO utilise std::async here
    if (!m_runeSheet.loadFromFile("bin/textures/runes.png"))
        throw std::runtime_error("Unable to load runes sheet!");

    if (!m_mainFont.loadFromFile("bin/fonts/Montserrat-Medium.ttf"))
        throw std::runtime_error("Unable to load main font");

    if (!loadRuneInfoFromDisk())
        throw std::runtime_error("Unable to load rune info");

    setupSelectMode();
    setupRandomTestMode();
}

void RuneRemember::run()
{
    sf::Clock loopClock;

    while (m_window.isOpen()) {
        const auto dt = loopClock.restart();

        sf::Event event;
        while (m_window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                m_window.close();

            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Escape)
                    m_window.close();
            }

            if (event.type == sf::Event::MouseMoved) {
                m_mousePosition = m_window.mapPixelToCoords({ event.mouseMove.x, event.mouseMove.y });
            }
            switch (m_currentMode) {
            case AppMode::Select:
                handleEventSelectMode(event);
                break;
            case AppMode::RandomTest:
                handleEventRandomTestMode(event);
                break;
            default:
                assert(false);
            }
        }

        update(dt);

        m_window.clear({ 175, 175, 175 });
        switch (m_currentMode) {
        case AppMode::Select:
            drawSelectMode();
            break;
        case AppMode::RandomTest:
            drawRandomTestMode();
            break;
        default:
            assert(false);
            break;
        }
        m_window.display();
    }
}

bool RuneRemember::loadRuneInfoFromDisk()
{
    std::ifstream runeInfoFile;
    runeInfoFile.open("bin/data/rune_info.txt");

    if (runeInfoFile.fail())
        return false;

    size_t index = 0;
    while (!runeInfoFile.eof()) {
        std::string data;
        runeInfoFile >> data;
        m_runesInfo.push_back({ index, sf::String::fromUtf8(data.begin(), data.end()) });
        ++index;
    }
    return true;
}

void RuneRemember::update(const sf::Time& dt)
{
    switch (m_currentMode) {
    case AppMode::Select:
        updateSelectMode();
        break;
    case AppMode::RandomTest:
        updateRandomTestMode(dt);
        break;
    default:
        assert(false);
        break;
    }
}

void RuneRemember::updateRuneSprite() { m_activeRune.setTextureRect({ { static_cast<int32_t>(m_runeIndex) * RUNE_IMAGE_SIZE.x, 0 }, RUNE_IMAGE_SIZE }); }

void RuneRemember::setupSelectMode()
{
    const auto windowCentre { sf::Vector2f { m_window.getSize() } * 0.5f };
    m_selectModeTitleText.setFont(m_mainFont);
    m_selectModeTitleText.setCharacterSize(TITLE_TEXT_FONT_SIZE);
    m_selectModeTitleText.setString("Select Rune Remember Mode");
    m_selectModeTitleText.setStyle(sf::Text::Style::Bold);
    CentreTextOrigin(m_selectModeTitleText);
    m_selectModeTitleText.setPosition({ windowCentre.x, TITLE_TEXT_Y_OFFSET });

    m_selectRunThroughButton.setFont(m_mainFont);
    m_selectRunThroughButton.setCharacterSize(BUTTON_FONT_SIZE);
    m_selectRunThroughButton.setString("Rune Run Through");
    m_selectRunThroughButton.setStyle(sf::Text::Style::Underlined);
    CentreTextOrigin(m_selectRunThroughButton);
    m_selectRunThroughButton.setPosition({ windowCentre.x, m_selectModeTitleText.getPosition().y + 150.f });

    m_selectRandomTestButton.setFont(m_mainFont);
    m_selectRandomTestButton.setCharacterSize(BUTTON_FONT_SIZE);
    m_selectRandomTestButton.setString("Rune Random Test");
    m_selectRandomTestButton.setStyle(sf::Text::Style::Underlined);
    CentreTextOrigin(m_selectRandomTestButton);
    m_selectRandomTestButton.setPosition({ windowCentre.x, m_selectRunThroughButton.getPosition().y + 75.f });
}

void RuneRemember::drawSelectMode()
{
    m_window.draw(m_selectModeTitleText);
    m_window.draw(m_selectRunThroughButton);
    m_window.draw(m_selectRandomTestButton);
}

void RuneRemember::updateSelectMode()
{
    if (m_selectRunThroughButton.getGlobalBounds().contains(m_mousePosition)) {
        m_selectRunThroughButton.setFillColor(BUTTON_HOVER_COLOR);
        m_selectRunThroughButton.setCharacterSize(HOVER_BUTTON_FONT_SIZE);
    } else {
        m_selectRunThroughButton.setFillColor(sf::Color::White);
        m_selectRunThroughButton.setCharacterSize(BUTTON_FONT_SIZE);
    }
    CentreTextOrigin(m_selectRunThroughButton);

    if (m_selectRandomTestButton.getGlobalBounds().contains(m_mousePosition)) {
        m_selectRandomTestButton.setFillColor(BUTTON_HOVER_COLOR);
        m_selectRandomTestButton.setCharacterSize(HOVER_BUTTON_FONT_SIZE);

    } else {
        m_selectRandomTestButton.setFillColor(sf::Color::White);
        m_selectRandomTestButton.setCharacterSize(BUTTON_FONT_SIZE);
    }
    CentreTextOrigin(m_selectRandomTestButton);
}

void RuneRemember::handleEventSelectMode(const sf::Event& event)
{
    if (event.type != sf::Event::MouseButtonPressed)
        return;

    if (event.mouseButton.button == sf::Mouse::Left) {
        if (m_selectRunThroughButton.getGlobalBounds().contains(m_mousePosition)) {
            m_currentMode = AppMode::SlowRunThrough;
        } else if (m_selectRandomTestButton.getGlobalBounds().contains(m_mousePosition)) {
            m_currentMode = AppMode::RandomTest;
            randomiseTestGuesses();
        }
    }
}

void RuneRemember::setupRandomTestMode()
{
    m_activeRune.setTexture(m_runeSheet);
    m_activeRune.setTextureRect({ { 0, 0 }, RUNE_IMAGE_SIZE });
    m_activeRune.setScale({ 10, 10 });
    m_activeRune.setOrigin(sf::Vector2f { RUNE_IMAGE_SIZE } * 0.5f);
    m_activeRune.setPosition(sf::Vector2f { m_window.getSize() } * 0.5f);

    const auto runeBounds { m_activeRune.getGlobalBounds() };

    m_questionText.setFont(m_mainFont);
    m_questionText.setCharacterSize(TITLE_TEXT_FONT_SIZE);
    m_questionText.setString("What rune is this?");
    m_questionText.setStyle(sf::Text::Style::Bold);
    CentreTextOrigin(m_questionText);
    // Offset the question text to be x units from the top of the
    // rune image
    const auto windowCentre { sf::Vector2f { m_window.getSize() } * 0.5f };
    m_questionText.setPosition({ windowCentre.x, runeBounds.getPosition().y - 50.f });

    const auto yPosition { runeBounds.getPosition().y + runeBounds.getSize().y };

    m_guessButtons[0].text.setPosition({ windowCentre.x, yPosition + 50.f });
    m_guessButtons[1].text.setPosition(m_guessButtons[0].text.getPosition() - sf::Vector2f { 250.f, 0.f });
    m_guessButtons[2].text.setPosition(m_guessButtons[0].text.getPosition() + sf::Vector2f { 250.f, 0.f });

    for (auto& gb : m_guessButtons) {
        gb.text.setFont(m_mainFont);
        gb.text.setCharacterSize(BUTTON_FONT_SIZE);
        gb.backdrop.setPosition(gb.text.getPosition());
        gb.backdrop.setSize(GUESS_BUTTON_BACKDROP_SIZE);
        gb.backdrop.setOrigin(GUESS_BUTTON_BACKDROP_SIZE * 0.5f);
        gb.backdrop.setFillColor(sf::Color::Black);
    }
}

void RuneRemember::drawRandomTestMode()
{
    m_window.draw(m_questionText);
    m_window.draw(m_activeRune);
    for (const auto& gb : m_guessButtons) {
        m_window.draw(gb.backdrop);
        m_window.draw(gb.text);
    }
}

void RuneRemember::updateRandomTestMode(const sf::Time& dt)
{
    _CRT_UNUSED(dt);
    if (!m_guessMade) {
        for (auto& gb : m_guessButtons) {
            if (gb.text.getGlobalBounds().contains(m_mousePosition)) {
                gb.text.setFillColor(BUTTON_HOVER_COLOR);
            } else {
                gb.text.setFillColor(sf::Color::White);
            }
        }
    } else {
        m_postGuessTimer += dt;
        if (m_postGuessTimer >= POST_GUESS_WAIT) {
            randomiseTestGuesses();
        }
    }
}

void RuneRemember::handleEventRandomTestMode(const sf::Event& event)
{
    if (event.type == sf::Event::MouseButtonPressed) {
        if (event.mouseButton.button == sf::Mouse::Left) {
            // The button was clicked so we'll iterate over all buttons
            // and then when we found the clicked one we'll highlight
            // its colour depending on whether it was correct or incorrect.
            // If it was incorrect we'll also highlight the correct one
            for (auto& gb : m_guessButtons) {
                if (gb.text.getGlobalBounds().contains(m_mousePosition)) {
                    if (gb.text.getString() == m_runesInfo[m_runeIndex].name) {
                        spdlog::debug("Correct");
                        gb.text.setFillColor(sf::Color::Green);
                    } else {
                        spdlog::debug("Incorrect");
                        gb.text.setFillColor(sf::Color::Red);
                        auto result = std::find_if(m_guessButtons.begin(), m_guessButtons.end(), [this](const GuessButton& button) {
                            return button.text.getString() == m_runesInfo[m_runeIndex].name;
                        });

                        // We shouldn't reach a situation where we don't find a
                        // correct answer, if we do then uhh debug this!
                        if (result == m_guessButtons.end())
                            assert(false);

                        result->text.setFillColor(sf::Color::Green);
                    }
                    m_guessMade = true;
                    break; // We found a guess so we should stop
                }
            }
        }
    }
}

void RuneRemember::randomiseTestGuesses()
{
    static auto guessDist { std::uniform_int_distribution<size_t>(0, m_guessButtons.size() - 1) };
    static auto runeDist { std::uniform_int_distribution<size_t>(0, m_runesInfo.size() - 1) };
    std::array<sf::String, 2> randomRuneNames;

    m_correctAnswerIndex = guessDist(m_randomEngine);
    m_runeIndex = runeDist(m_randomEngine);
    updateRuneSprite();

    do {
        randomRuneNames[0] = m_runesInfo[runeDist(m_randomEngine)].name;
        randomRuneNames[1] = m_runesInfo[runeDist(m_randomEngine)].name;
    } while (randomRuneNames[0] == randomRuneNames[1] && randomRuneNames[0] != m_runesInfo[m_runeIndex].name
             && randomRuneNames[1] != m_runesInfo[m_runeIndex].name);

    // Setup the correct answer
    m_guessButtons[m_correctAnswerIndex].text.setString(m_runesInfo[m_runeIndex].name);
    CentreTextOrigin(m_guessButtons[m_correctAnswerIndex].text);

    // Setup the incorrect guesses
    auto allocatedFirst { false };
    for (size_t i { 0 }; i < m_guessButtons.size(); ++i) {
        if (i == m_correctAnswerIndex) {
            continue;
        }

        if (!allocatedFirst) {
            m_guessButtons[i].text.setString(randomRuneNames[0]);
            allocatedFirst = true;
        } else {
            m_guessButtons[i].text.setString(randomRuneNames[1]);
        }

        CentreTextOrigin(m_guessButtons[i].text);
    }

    m_guessMade = false;

    for (auto& gb : m_guessButtons) {
        gb.text.setFillColor(sf::Color::White);
    }

    m_postGuessTimer = sf::Time::Zero;
}
