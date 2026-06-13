#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include <vector>
#include <ctime>
#include <memory> // Для умных указателей
#include "QTEManager.hpp"
#include "Rocket.hpp"
#include "GameObject.hpp"
#include "Equipment.hpp" // Подключаем магазин напарника

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
    void applyUpgrades();
    void spawnTarget(int index); // Функция для создания цели (Птица или Дрон)

private:
    sf::RenderWindow mWindow;
    GameState mCurrentState;
    int mCoins;
    bool mWasFlying;

    QTEManager mQteManager;
    Rocket mRocket;
    sf::Texture mTireTexture;
    sf::Sprite mTireVisual;

    // -- - НОВОЕ: Текстура и Спрайт Персонажа - Механика-- -
        sf::Texture mCharacterTexture;
    sf::Sprite mCharacterSprite;

    // --- НОВОЕ: Параметры анимации механика ---
    std::vector<sf::IntRect> mCharacterFrames; // Прямоугольники кадров
    int mCharacterCurrentFrame;                 // Текущий кадр
    sf::Clock mAnimationClock;                 // Таймер для смены кадров
    bool mIsCelebrating;                       // Флаг: празднуем ли мы сейчас?
    float mFrameDuration;                      // Длительность одного кадра
    int mCelebrateStartFrame;                 // Кадр, с которого начинается радость (3-й)

    std::vector<std::unique_ptr<Target>> mTargets;

    sf::Font mFont;

    // --- НОВОЕ: Элементы UI для счета ---
    sf::RectangleShape mScoreBackground;
    sf::Text mScoreText;
    sf::CircleShape mCoinIcon;

    std::vector<Motor> mMotors;
    std::vector<Tire> mTires;
    int mCurrentMotorIndex;
    int mCurrentTireIndex;

    sf::Texture mBackgroundTexture;
    sf::Sprite mBackgroundSprite;

    sf::Texture mLauncherTexture;
    sf::Sprite mLauncherSprite;
};