#pragma once
#include <unordered_map>

#include <SFML/Window.hpp>


class GameManager;

class InputManager
{
    struct States {
        bool isDown = false;
        bool isHeld = false;
        bool isRelease = false;
    };

private:



    //clavier
    std::unordered_map<sf::Keyboard::Key, States> m_keysStates;

    //souris
    std::unordered_map<sf::Mouse::Button, States> m_mouseStates;
    sf::Vector2i m_mousePos = { 0, 0 };


    //singleton
    InputManager(const InputManager&) = delete;
    InputManager& operator=(const InputManager&) = delete;


    InputManager() = default;
public:

    static InputManager& getInstance()
    {
        static InputManager instance = InputManager();
        return instance;
    }

    void update();


    //getter
    bool isKeyDown(const sf::Keyboard::Key _key) const ;
    bool isKeyHeld(const sf::Keyboard::Key _key) const ;
    bool isKeyRelease(const sf::Keyboard::Key _key) const ;

    bool isMouseButtonDown(const sf::Mouse::Button _btn) const;
    bool isMouseButtonHeld(const sf::Mouse::Button _btn) const;
    bool isMouseButtonRelease(const sf::Mouse::Button _btn) const;
};

