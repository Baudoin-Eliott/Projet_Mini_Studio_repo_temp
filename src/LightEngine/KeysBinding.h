#pragma once

#include <variant>
#include <vector>
#include "SFML/Window.hpp"

enum class TriggerState { Down, Held, Released };

using Input = std::variant<sf::Keyboard::Key, sf::Mouse::Button>;

struct KeyBinding {
    std::vector<std::pair<Input, TriggerState>> inputs;
};