#include "RuneRemember.hpp"

#include <SFML/Graphics/Text.hpp>
#include <SFML/System/String.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/VideoMode.hpp>
#include <spdlog/spdlog.h>

#include <fstream>

constexpr auto RUNE_IMAGE_SIZE { sf::Vector2i { 32, 32 } };

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

    if (!m_runeSheet.loadFromFile("bin/textures/runes.png")) {
        throw std::runtime_error("Unable to load runes sheet!");
    }

    m_activeRune.setTexture(m_runeSheet);
    m_activeRune.setTextureRect({ { 0, 0 }, RUNE_IMAGE_SIZE });
    m_activeRune.setScale({ 10, 10 });
    m_activeRune.setOrigin(sf::Vector2f { RUNE_IMAGE_SIZE } * 0.5f);
    m_activeRune.setPosition(sf::Vector2f { m_window.getSize() } * 0.5f);

    if (!m_mainFont.loadFromFile("bin/fonts/Montserrat-Medium.ttf")) {
        throw std::runtime_error("Unable to load main font");
    }

    if (!loadRuneInfoFromDisk()) {
        throw std::runtime_error("Unable to load rune info");
    }

    m_questionText.setFont(m_mainFont);
    m_questionText.setString(m_runesInfo[0].name);
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
        }

        m_window.clear({ 175, 175, 175 });
        m_window.draw(m_activeRune);
        m_window.draw(m_questionText);
        m_window.display();
    }
}

bool RuneRemember::loadRuneInfoFromDisk()
{
    std::ifstream runeInfoFile;
    runeInfoFile.open("bin/data/rune_info.txt");

    if (runeInfoFile.fail()) {
        return false;
    }

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
