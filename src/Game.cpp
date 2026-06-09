#include "Game.hpp"

Game::Game()
    : mWindow(sf::VideoMode(1280, 720), "Indian Space Program")
    , mCurrentState(GameState::MENU)
    , mCoins(0)
    , mWasFlying(false)
{
    mWindow.setFramerateLimit(60);
    loadProgress(); // Загружаем монеты при старте
}

void Game::run() {
    sf::Clock clock;
    while (mWindow.isOpen()) {
        sf::Time deltaTime = clock.restart();
        processEvents();
        update(deltaTime);
        render();
    }
}

void Game::processEvents() {
    sf::Event event;
    while (mWindow.pollEvent(event)) {
        if (event.type == sf::Event::Closed) mWindow.close();

        if (event.type == sf::Event::KeyPressed) {
            if (event.key.code == sf::Keyboard::Escape) {
                if (mCurrentState == GameState::PLAY) mCurrentState = GameState::PAUSE;
                else if (mCurrentState == GameState::PAUSE) mCurrentState = GameState::PLAY;
            }
            if (event.key.code == sf::Keyboard::Num1) mCurrentState = GameState::MENU;
            if (event.key.code == sf::Keyboard::Num2) mCurrentState = GameState::SHOP;
            if (event.key.code == sf::Keyboard::Num3) mCurrentState = GameState::PLAY;

            if (event.key.code == sf::Keyboard::Space && mCurrentState == GameState::PLAY) {
                if (!mQteManager.isActive() && !mRocket.isFlying()) {
                    mQteManager.start();
                    std::cout << "[QTE] Start! Przytrzymaj spacje..." << std::endl;
                }
            }
        }

        if (event.type == sf::Event::KeyReleased) {
            if (event.key.code == sf::Keyboard::Space && mCurrentState == GameState::PLAY) {
                if (mQteManager.isActive()) {
                    float result = mQteManager.stop();
                    std::cout << "[QTE] Stop! Mnoznik sily: " << result << std::endl;
                    mRocket.launch(result);
                }
            }
        }
    }
}

void Game::update(sf::Time deltaTime) {
    switch (mCurrentState) {
    case GameState::MENU: break;
    case GameState::SHOP: break;
    case GameState::PLAY: {
        if (!mRocket.isFlying()) {
            mQteManager.update(deltaTime);
        }
        else {
            // ЗДЕСЬ БУДЕТ ПРОВЕРКА СТОЛКНОВЕНИЙ!
            // Если ракета летит, мы каждый кадр проверяем, не коснулась ли она птицы из списка mTargets
            // Если коснулась -> mCoins += target->getReward();
        }

        mRocket.update(deltaTime.asSeconds());

        // Ракета приземлилась
        bool currentlyFlying = mRocket.isFlying();
        if (mWasFlying && !currentlyFlying) {
            // Базовая награда за дистанцию (на бензин)
            int distanceReward = static_cast<int>(mRocket.getDistance() / 10.0f);
            mCoins += distanceReward;

            std::cout << "[Gra] Ladowanie! Otrzymujesz " << distanceReward << " monet za dystans. Stan konta: " << mCoins << std::endl;
            saveProgress(); // Сохраняем все монеты (включая те, что выбьем из птиц)
        }
        mWasFlying = currentlyFlying;

        break;
    }
    case GameState::PAUSE: break;
    }
}

void Game::render() {
    sf::Color bgColor;
    switch (mCurrentState) {
    case GameState::MENU:  bgColor = sf::Color(30, 30, 50);   break;
    case GameState::SHOP:  bgColor = sf::Color(50, 50, 30);   break;
    case GameState::PLAY:  bgColor = sf::Color(30, 50, 30);   break;
    case GameState::PAUSE: bgColor = sf::Color(50, 30, 30);   break;
    }
    mWindow.clear(bgColor);
    mWindow.display();
}

void Game::saveProgress() {
    std::ofstream file("save.txt");
    if (file.is_open()) {
        file << mCoins << "\n";
        file.close();
        std::cout << "[Zapis] Gra zapisana pomyslnie." << std::endl;
    }
}

void Game::loadProgress() {
    std::ifstream file("save.txt");
    if (file.is_open()) {
        file >> mCoins;
        file.close();
        std::cout << "[Odczyt] Wczytano gre. Stan konta: " << mCoins << std::endl;
    }
    else {
        mCoins = 0;
        std::cout << "[Odczyt] Brak pliku zapisu. Nowa gra." << std::endl;
    }
}