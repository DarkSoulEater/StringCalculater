#include <iostream>
#include <vector>
#include <SFML/Graphics.hpp>
//#include "graphic.h"
#include "graphics.h"


/*int main() {
    sf::RenderWindow window(sf::VideoMode(1920, 1080), "WildCalcu", sf::Style::None);

    ScrollEditPanel main(600, 400, 15, 15);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) window.close();
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Escape) {
                    window.close();
                }
            }

            main.catchEvent(event);
        }

        window.clear(sf::Color(199, 199, 199, 255));
        main.draw(window);
        //window.draw(text);
        //window.draw(line);
        window.display();

        std::vector<std::string> data = main.getData();
        for (int i = 0; i < data.size(); ++i) {
            ///std::cout << data[i] << "\n";
        }
    }

    return 0;
}*/