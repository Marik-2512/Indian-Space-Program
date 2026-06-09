#pragma once
#include <SFML/Graphics.hpp>

// --- БАЗОВЫЙ ИГРОВОЙ ОБЪЕКТ ---
class GameObject {
public:
    GameObject() = default;
    virtual ~GameObject() = default;

    virtual void update(sf::Time deltaTime) = 0;
    virtual void draw(sf::RenderWindow& window) = 0;

    void setPosition(float x, float y) {
        position = sf::Vector2f(x, y);
        sprite.setPosition(position);
    }
    sf::Vector2f getPosition() const { return position; }

protected:
    sf::Sprite sprite;
    sf::Vector2f position;
};

// --- ПРОМЕЖУТОЧНЫЙ КЛАСС: ЦЕЛЬ ---
class Target : public GameObject {
public:
    Target(int reward) : coinReward(reward) {}

    int getReward() const { return coinReward; }

protected:
    int coinReward;
};

// --- КОНКРЕТНЫЕ ЦЕЛИ (НАСЛЕДНИКИ) ---

class Bird : public Target {
public:
    Bird(int reward) : Target(reward) {
        // Позже здесь загрузим текстуру птицы
    }

    void update(sf::Time deltaTime) override {
        // Логика из плана: полет по sinusoidzie (синусоиде)
        // Напишем математику позже, когда добавим физику
    }

    void draw(sf::RenderWindow& window) override {
        window.draw(sprite);
    }
};

class Drone : public Target {
public:
    Drone(int reward) : Target(reward) {
        // Позже здесь загрузим текстуру дрона
    }

    void update(sf::Time deltaTime) override {
        // Логика из плана: jednostajny ruch (равномерное движение)
    }

    void draw(sf::RenderWindow& window) override {
        window.draw(sprite);
    }
};