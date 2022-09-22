#include "RuneRemember.hpp"

#include <SFML/Graphics/Text.hpp>
#include <SFML/System/String.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/VideoMode.hpp>
#include <spdlog/spdlog.h>

#include <cassert>
#include <fstream>

constexpr auto RUNE_IMAGE_SIZE { sf::Vector2i { 32, 32 } };
constexpr auto BUTTON_HOVER_COLOR { sf::Color { 237, 233, 149 } };
constexpr auto HOVER_BUTTON_FONT_SIZE { 34u };
constexpr auto BUTTON_FONT_SIZE { 32u };

inline void CentreTextOrigin(sf::Text& text)
{
    const auto localBounds = sf::Vector2f(text.getLocalBounds().left, text.getLocalBounds().top);
    const auto globalOrigin = sf::Vector2f(text.getGlobalBounds().width / 2.0f, text.getGlobalBounds().height / 2.0f);

    text.setOrigin(localBounds + globalOrigin);
}

RuneRemember::RuneRemember()
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

    m_activeRune.setTexture(m_runeSheet);
    m_activeRune.setTextureRect({ { 0, 0 }, RUNE_IMAGE_SIZE });
    m_activeRune.setScale({ 10, 10 });
    m_activeRune.setOrigin(sf::Vector2f { RUNE_IMAGE_SIZE } * 0.5f);
    m_activeRune.setPosition(sf::Vector2f { m_window.getSize() } * 0.5f);

    setupSelectMode();
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
            case AppMode::SelectMode:
                handleEventSelectMode(event);
                break;
            default:
                assert(false);
            }
        }

        update(dt);

        m_window.clear({ 175, 175, 175 });
        switch (m_currentMode) {
        case AppMode::SelectMode:
            drawSelectMode();
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
        m_runesInfo.push_back({
            index,
            sf::String::fromUtf8(data.begin(), data.end()),
        });
        ++index;
    }
    return true;
}

void RuneRemember::update(const sf::Time& dt)
{
    switch (m_currentMode) {
    case AppMode::SelectMode:
        updateSelectMode();
        break;
    default:
        assert(false);
        break;
    }
}

void RuneRemember::setupSelectMode()
{
    const auto windowCentre { sf::Vector2f { m_window.getSize() } * 0.5f };
    m_selectModeTitleText.setFont(m_mainFont);
    m_selectModeTitleText.setCharacterSize(48);
    m_selectModeTitleText.setString("Select Rune Remember Mode");
    m_selectModeTitleText.setStyle(sf::Text::Style::Bold);
    CentreTextOrigin(m_selectModeTitleText);
    m_selectModeTitleText.setPosition({ windowCentre.x, 100.f });

    m_selectOption1.setFont(m_mainFont);
    m_selectOption1.setCharacterSize(BUTTON_FONT_SIZE);
    m_selectOption1.setString("Rune Run Through");
    m_selectOption1.setStyle(sf::Text::Style::Underlined);
    CentreTextOrigin(m_selectOption1);
    m_selectOption1.setPosition({ windowCentre.x, m_selectModeTitleText.getPosition().y + 150.f });

    m_selectOption2.setFont(m_mainFont);
    m_selectOption2.setCharacterSize(BUTTON_FONT_SIZE);
    m_selectOption2.setString("Rune Random Test");
    m_selectOption2.setStyle(sf::Text::Style::Underlined);
    CentreTextOrigin(m_selectOption2);
    m_selectOption2.setPosition({ windowCentre.x, m_selectOption1.getPosition().y + 75.f });
}

void RuneRemember::drawSelectMode()
{
    m_window.draw(m_selectModeTitleText);
    m_window.draw(m_selectOption1);
    m_window.draw(m_selectOption2);
}

void RuneRemember::updateSelectMode()
{
    if (m_selectOption1.getGlobalBounds().contains(m_mousePosition)) {
        m_selectOption1.setFillColor(BUTTON_HOVER_COLOR);
        m_selectOption1.setCharacterSize(HOVER_BUTTON_FONT_SIZE);
    } else {
        m_selectOption1.setFillColor(sf::Color::White);
        m_selectOption1.setCharacterSize(BUTTON_FONT_SIZE);
    }
    CentreTextOrigin(m_selectOption1);

    if (m_selectOption2.getGlobalBounds().contains(m_mousePosition)) {
        m_selectOption2.setFillColor(BUTTON_HOVER_COLOR);
        m_selectOption2.setCharacterSize(HOVER_BUTTON_FONT_SIZE);

    } else {
        m_selectOption2.setFillColor(sf::Color::White);
        m_selectOption2.setCharacterSize(BUTTON_FONT_SIZE);
    }
    CentreTextOrigin(m_selectOption2);
}

void RuneRemember::handleEventSelectMode(const sf::Event& event) { }
