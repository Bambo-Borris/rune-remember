#include "RuneRemember.hpp"

#include <SFML/System/String.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/VideoMode.hpp>

RuneRemember::RuneRemember()
{
    sf::ContextSettings ctxt;
    ctxt.antialiasingLevel = 16;
    // m_window.create(sf::VideoMode({ 600, 600 }), "Hello World", sf::Style::Default);
    m_window.create(sf::VideoMode({ 800, 600 }), "WINDOW_TITLE", sf::Style::Default, ctxt);
    m_window.setFramerateLimit(30);
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

        m_window.clear();
        m_window.display();
    }
}
