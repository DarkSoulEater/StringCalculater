#include <iostream>
#include <set>
#include <unordered_map>
#include <SFML/Graphics.hpp>
#include "UndergroundType.h"
#include "Analysis.h"
#include <chrono>
#include "graphics.h"
#include "calculator.h"

int main() {
    std::unordered_map<std::string, int> function_list;
    std::map<std::string, std::string> variable_list;
    std::map<std::string, bool> var_name_map;

    function_list["sin"] = 1;
    function_list["cos"] = 1;
    function_list["log"] = 2;
    function_list["KEK"] = 3;

    sf::RenderWindow window(sf::VideoMode(1920, 1080), "WildCalcu", sf::Style::None);

    ScrollEditPanel input(0, 1300, 600, 15, 15);
    SetVarPanel var(575, 600, 1330, 15);
    ScrollEditPanel output(2, 1890, 435, 15, 630);

    while (window.isOpen()) {
        for (auto it = var_name_map.begin(); it != var_name_map.end(); ++it) {
            it->second = 0;
        }
        std::vector<std::string> data = input.getData();
        std::vector<std::string> output_log(data.size(), ""), expression(data.size(), "");

        variable_list["PI"] = "3.1415926535";
        variable_list["e"] = "1.7182818284";
        for (int i = 0; i < data.size(); ++i) {
            try {
                expression[i] = DFA(data[i], function_list, var_name_map);
                output_log[i] = Calculate(expression[i], variable_list);
            } catch (const std::string& err) {
                output_log[i] = err;
            }
        }

        var_name_map["PI"] = 0;
        var_name_map["e"] = 0;
        var.upadteVarData(var_name_map);
        variable_list = var.getVarData();
        output.setData(output_log);

        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) window.close();
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Escape) {
                    window.close();
                }
            }

            input.catchEvent(event);
            var.catchEvent(event);
        }

        window.clear(sf::Color(199, 199, 199, 255));
        input.draw(window);
        var.draw(window);
        output.draw(window);
        window.display();
    }

    return 0;
}