#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include <vector>
#include "QTEManager.hpp"
#include "Rocket.hpp"
#include "GameObject.hpp" // Подключаем наших птиц и дронов

enum class GameState { MENU, SHOP, PLAY, PAUSE };

class Game {
public:
    Game();
    void run();

private:
    void processEvents();
    void update(sf::Time deltaTime);
    void render();

    void saveProgress();
    void loadProgress();

private:
    sf::RenderWindow mWindow;
    GameState mCurrentState;
    int mCoins;
    bool mWasFlying;

    QTEManager mQteManager;
    Rocket mRocket;

    // Список всех целей (птиц и дронов), которые сейчас есть на карте
    std::vector<Target*> mTargets;
};