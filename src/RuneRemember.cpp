#include "RuneRemember.hpp"

#include <SFML/System/String.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/VideoMode.hpp>

constexpr auto RUNE_IMAGE_SIZE { sf::Vector2i { 32, 32 } };

RuneRemember::RuneRemember()
{
    sf::ContextSettings ctxt;
    ctxt.antialiasingLevel = 16;
    // m_window.create(sf::VideoMode({ 600, 600 }), "Hello World", sf::Style::Default);
    m_window.create(sf::VideoMode({ 800, 600 }), "WINDOW_TITLE", sf::Style::Default, ctxt);
    m_window.setFramerateLimit(30);

    if (!m_runeSheet.loadFromFile("bin/textures/runes.png")) {
        throw std::runtime_error("Unable to load runes sheet!");
    }

    m_activeRune.setTexture(m_runeSheet);
    m_activeRune.setTextureRect({ { 0, 0 }, RUNE_IMAGE_SIZE });
    m_activeRune.setScale({ 10, 10 });
    m_activeRune.setOrigin(sf::Vector2f { RUNE_IMAGE_SIZE } * 0.5f);
    m_activeRune.setPosition(sf::Vector2f { m_window.getSize() } * 0.5f);
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
        }

        m_window.clear({ 175, 175, 175 });
        m_window.draw(m_activeRune);
        m_window.display();
    }
}
