#include "Game.hpp"
#include "GameObject.hpp"
#include <cstdlib>
#include <ctime>

Game::Game()
    : mWindow(sf::VideoMode(1280, 720), "Warsztat Jugaad")
    , mCurrentState(GameState::MENU)
    , mCoins(0)
    , mWasFlying(false)
    , mCurrentMotorIndex(0)
    , mCurrentTireIndex(0)
    , mCharacterCurrentFrame(0)
    , mIsCelebrating(false)
    , mFrameDuration(0.12f)         // Скорость анимации радости
    , mCelebrateStartFrame(2)       // Радость начинается с 3-го кадра (индекс 2)
{
    srand(static_cast<unsigned int>(time(0)));
    mWindow.setFramerateLimit(60);
    loadProgress();

    if (!mFont.loadFromFile("VT323-Regular.ttf")) {
        std::cout << "[Blad] Brak czcionki!" << std::endl;
    }

    // --- НАСТРОЙКА ИНТЕРФЕЙСА СЧЕТА ---
    mScoreBackground.setSize(sf::Vector2f(200.0f, 45.0f));
    mScoreBackground.setFillColor(sf::Color(35, 45, 56));
    mScoreBackground.setOutlineThickness(3.0f);
    mScoreBackground.setOutlineColor(sf::Color::White);
    mScoreBackground.setPosition(20.0f, 20.0f);

    mScoreText.setFont(mFont);
    mScoreText.setCharacterSize(30);
    mScoreText.setFillColor(sf::Color::White);
    mScoreText.setPosition(35.0f, 22.0f);

    mCoinIcon.setRadius(12.0f);
    mCoinIcon.setFillColor(sf::Color(255, 215, 0));
    mCoinIcon.setPosition(180.0f, 30.0f);

    if (!mBackgroundTexture.loadFromFile("game_bg.png")) {
        std::cout << "[Blad] Nie mozna zaladowac game_bg.png!" << std::endl;
    }
    else {
        mBackgroundSprite.setTexture(mBackgroundTexture);
        float scaleX = 1280.0f / mBackgroundTexture.getSize().x;
        float scaleY = 720.0f / mBackgroundTexture.getSize().y;
        mBackgroundSprite.setScale(scaleX, scaleY);
    }

    // --- НАСТРОЙКА ДВИГАТЕЛЯ-ЛАУНЧЕРА ---
    if (!mLauncherTexture.loadFromFile("engine1.png")) {
        std::cout << "[Blad] Nie mozna zaladowac engine1.png!" << std::endl;
    }
    else {
        mLauncherSprite.setTexture(mLauncherTexture);
        mLauncherSprite.setScale(0.45f, 0.45f);
        mLauncherSprite.setOrigin(608.0f, 352.0f);
        mLauncherSprite.setPosition(210.0f, 600.0f);
    }

    // --- НАСТРОЙКА ПЕРСОНАЖА (МЕХАНИКА ПО ТОЧНЫМ КООРДИНАТАМ) ---
    if (!mCharacterTexture.loadFromFile("character_sheet.png")) {
        std::cout << "[Blad] Nie mozna zaladowac character_sheet.png!" << std::endl;
    }
    else {
        mCharacterSprite.setTexture(mCharacterTexture);

        // Точные выверенные прямоугольники для каждого кадра (Высота 600)
        mCharacterFrames.clear();
        mCharacterFrames.push_back(sf::IntRect(16, 0, 171, 600));
        mCharacterFrames.push_back(sf::IntRect(209, 0, 191, 600));
        mCharacterFrames.push_back(sf::IntRect(411, 0, 211, 600));
        mCharacterFrames.push_back(sf::IntRect(627, 0, 191, 600));

        // Устанавливаем базовый первый кадр
        mCharacterSprite.setTextureRect(mCharacterFrames[0]);

        // Ставим точку привязки (Origin) строго по ЦЕНТРУ кадра
        mCharacterSprite.setOrigin(mCharacterFrames[0].width / 2.0f, mCharacterFrames[0].height / 2.0f);

        // Позиционируем послушно за пусковой установкой.
        // КООРДИНАТА Y ТЕПЕРЬ СВОБОДНО РЕГУЛИРУЕТСЯ ЗДЕСЬ (Увеличивай, чтобы опустить на землю)
        mCharacterSprite.setPosition(110.0f, 520.0f);
        mCharacterSprite.setScale(0.45f, 0.45f);
    }

    // --- НАСТРОЙКА СПРАЙТА КОЛЕСА ---
    if (!mTireTexture.loadFromFile("tire1.png")) {
        std::cout << "[Blad] Nie mozna zaladowac tire1.png!" << std::endl;
    }
    else {
        mTireVisual.setTexture(mTireTexture);
        mTireVisual.setOrigin(mTireTexture.getSize().x / 2.0f, mTireTexture.getSize().y / 2.0f);
        mTireVisual.setScale(0.18f, 0.18f);
    }

    mMotors.push_back(Motor("Stary Silnik", 0, true, 0.5f));
    mMotors.push_back(Motor("Silnik Diesla", 150, false, 1.0f));
    mMotors.push_back(Motor("Turbo Jugaad", 350, false, 2.0f));

    mTires.push_back(Tire("Lysa Opona", 0, true, 1.5f));
    mTires.push_back(Tire("Opona od Riksz", 150, false, 1.0f));
    mTires.push_back(Tire("Opona Sportowa", 350, false, 0.4f));

    applyUpgrades();

    for (int i = 0; i < 3; ++i) {
        mTargets.push_back(nullptr);
        spawnTarget(i);
    }
}

    void Game::spawnTarget(int index) {
        float randomY = 100.0f + static_cast<float>(rand() % 400);

        int chance = rand() % 100; // Число от 0 до 99

        if (chance < 50) {
            // 50% шанс спавна Птицы (0 - 49)
            mTargets[index] = std::make_unique<Bird>();
        }
        else if (chance < 80) {
            // 30% шанс спавна Дрона (50 - 79)
            mTargets[index] = std::make_unique<Drone>();
        }
        else {
            // 20% шанс спавна Коровы-препятствия (80 - 99)
            mTargets[index] = std::make_unique<Cow>();
        }

        int side = rand() % 2;
        float baseSpeed = 100.0f + static_cast<float>(rand() % 150);

        if (side == 0) {
            mTargets[index]->setPosition(-50.0f, randomY);
            mTargets[index]->setSpeed(baseSpeed);
        }
        else {
            mTargets[index]->setPosition(1330.0f, randomY);
            mTargets[index]->setSpeed(-baseSpeed);
        }
    }

void Game::applyUpgrades() {
    float enginePwr = mMotors[mCurrentMotorIndex].getPowerMultiplier();
    float tireDrg = mTires[mCurrentTireIndex].getPowerMultiplier();
    mRocket.upgrade(enginePwr, tireDrg);
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
            if (event.key.code == sf::Keyboard::Num1) mCurrentState = GameState::MENU;
            if (event.key.code == sf::Keyboard::Num2) mCurrentState = GameState::SHOP;
            if (event.key.code == sf::Keyboard::Num3) mCurrentState = GameState::PLAY;
            if (event.key.code == sf::Keyboard::Escape) {
                if (mCurrentState == GameState::PLAY) mCurrentState = GameState::PAUSE;
            }

            if (mCurrentState == GameState::SHOP) {
                if (event.key.code == sf::Keyboard::E && mCurrentMotorIndex < static_cast<int>(mMotors.size()) - 1) {
                    int nextIdx = mCurrentMotorIndex + 1;
                    if (!mMotors[nextIdx].getIsUnlocked() && mCoins >= mMotors[nextIdx].getPrice()) {
                        mCoins -= mMotors[nextIdx].getPrice();
                        mMotors[nextIdx].unlock();
                        mCurrentMotorIndex = nextIdx;
                        applyUpgrades();
                    }
                    else if (mMotors[nextIdx].getIsUnlocked()) {
                        mCurrentMotorIndex = nextIdx;
                        applyUpgrades();
                    }
                }
                if (event.key.code == sf::Keyboard::T && mCurrentTireIndex < static_cast<int>(mTires.size()) - 1) {
                    int nextIdx = mCurrentTireIndex + 1;
                    if (!mTires[nextIdx].getIsUnlocked() && mCoins >= mTires[nextIdx].getPrice()) {
                        mCoins -= mTires[nextIdx].getPrice();
                        mTires[nextIdx].unlock();
                        mCurrentTireIndex = nextIdx;
                        applyUpgrades();
                    }
                    else if (mTires[nextIdx].getIsUnlocked()) {
                        mCurrentTireIndex = nextIdx;
                        applyUpgrades();
                    }
                }
            }

            if (event.key.code == sf::Keyboard::Space && mCurrentState == GameState::PLAY) {
                if (!mQteManager.isActive() && !mRocket.isFlying()) {
                    mQteManager.start();
                }
            }
        }

        if (event.type == sf::Event::KeyReleased) {
            if (event.key.code == sf::Keyboard::Space && mCurrentState == GameState::PLAY) {
                if (mQteManager.isActive()) {
                    float result = mQteManager.stop();
                    mRocket.launch(result);
                }
            }
        }
    }
}

void Game::update(sf::Time deltaTime) {
    if (mCurrentState != GameState::PLAY) return;

    // --- ОБНОВЛЯЕМ ТЕКСТ СЧЕТЧИКА ---
    mScoreText.setString("MONETY: " + std::to_string(mCoins));

    // --- ОБНОВЛЕНИЕ АНИМАЦИИ ПЕРСОНАЖА ---
    if (mIsCelebrating) {
        if (mAnimationClock.getElapsedTime().asSeconds() > mFrameDuration) {
            mAnimationClock.restart();
            mCharacterCurrentFrame++;

            if (mCharacterCurrentFrame >= static_cast<int>(mCharacterFrames.size())) {
                mCharacterCurrentFrame = 0;
                mIsCelebrating = false;
            }

            mCharacterSprite.setTextureRect(mCharacterFrames[mCharacterCurrentFrame]);

            // Центрируем динамически по середине каждого кадра, чтобы не было тряски
            mCharacterSprite.setOrigin(mCharacterFrames[mCharacterCurrentFrame].width / 2.0f,
                mCharacterFrames[mCharacterCurrentFrame].height / 2.0f);
        }
    }
    else {
        mCharacterSprite.setTextureRect(mCharacterFrames[0]);
        mCharacterSprite.setOrigin(mCharacterFrames[0].width / 2.0f, mCharacterFrames[0].height / 2.0f);
    }

    // --- ОБНОВЛЕНИЕ ЦЕЛЕЙ ---
    for (size_t i = 0; i < mTargets.size(); ++i) {
        if (!mTargets[i]->isActive()) continue;

        mTargets[i]->update(deltaTime);

        sf::Vector2f pos = mTargets[i]->getPosition();
        float spd = mTargets[i]->getSpeed();

        bool flewRight = (spd > 0.0f && pos.x > 1300.0f);
        bool flewLeft = (spd < 0.0f && pos.x < -100.0f);

        if (flewRight || flewLeft) {
            spawnTarget(static_cast<int>(i));
        }
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && !mRocket.isFlying()) {
        if (!mQteManager.isActive()) {
            mQteManager.start();
        }
    }

    if (!mRocket.isFlying()) mQteManager.update(deltaTime);

    mRocket.update(deltaTime.asSeconds());

    // --- ЛОГИКА ПОЛЕТА И ПОПАДАНИЙ ---
    if (mRocket.isFlying()) {
        sf::Vector2f startPos(290.0f, 550.0f);
        mTireVisual.setPosition(startPos.x + mRocket.getDistance(), startPos.y - mRocket.getAltitude());

        mTireVisual.rotate(360.0f * deltaTime.asSeconds());

        sf::FloatRect tireBox = mTireVisual.getGlobalBounds();
        for (size_t i = 0; i < mTargets.size(); ++i) {
            if (mTargets[i]->isActive() && tireBox.intersects(mTargets[i]->getBounds())) {
                mTargets[i]->setActive(false);
                mCoins += mTargets[i]->getReward();
                std::cout << "[Gra] Trafienie! + " << mTargets[i]->getReward() << " monet! Stan: " << mCoins << std::endl;

                // Радуемся только в первый раз за выстрел (если анимация еще не запущена)
                if (!mIsCelebrating && mCharacterCurrentFrame == 0) {
                    mIsCelebrating = true;
                    mCharacterCurrentFrame = mCelebrateStartFrame;
                    mCharacterSprite.setTextureRect(mCharacterFrames[mCharacterCurrentFrame]);
                    mAnimationClock.restart();
                }

                spawnTarget(static_cast<int>(i));
            }
        }
    }

    bool currentlyFlying = mRocket.isFlying();
    if (mWasFlying && !currentlyFlying) {
        mCoins += static_cast<int>(mRocket.getDistance() / 10.0f);
        saveProgress();
    }
    mWasFlying = currentlyFlying;
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

    if (mCurrentState == GameState::SHOP) {
        sf::Text shopText;
        shopText.setFont(mFont);
        shopText.setCharacterSize(30);
        shopText.setFillColor(sf::Color::White);
        shopText.setPosition(50.f, 50.f);

        int nxtM = (mCurrentMotorIndex < static_cast<int>(mMotors.size()) - 1) ? mCurrentMotorIndex + 1 : mCurrentMotorIndex;
        std::string mTxt = (mCurrentMotorIndex < static_cast<int>(mMotors.size()) - 1) ? "Cena: " + std::to_string(mMotors[nxtM].getPrice()) : "MAX";

        int nxtT = (mCurrentTireIndex < static_cast<int>(mTires.size()) - 1) ? mCurrentTireIndex + 1 : mCurrentTireIndex;
        std::string tTxt = (mCurrentTireIndex < static_cast<int>(mTires.size()) - 1) ? "Cena: " + std::to_string(mTires[nxtT].getPrice()) : "MAX";

        shopText.setString(
            "--- WARSZTAT JUGAAD ---\n"
            "Dostepne monety: " + std::to_string(mCoins) + "\n\n"
            "[E] Silnik: " + mMotors[mCurrentMotorIndex].getName() + "\n"
            "     -> Nastepny: " + mMotors[nxtM].getName() + " | " + mTxt + "\n\n"
            "[T] Opony: " + mTires[mCurrentTireIndex].getName() + "\n"
            "     -> Nastepny: " + mTires[nxtT].getName() + " | " + tTxt + "\n\n"
            "Nacisnij 3 aby wrocic na ulice"
        );
        mWindow.draw(shopText);
    }

    if (mCurrentState == GameState::PLAY) {
        mWindow.draw(mBackgroundSprite);

        // --- РЕНДЕР ПЕРСОНАЖА СТРОГО ПОЗАДИ МОТОРА ---
        mWindow.draw(mCharacterSprite);
        mWindow.draw(mLauncherSprite);

        for (const auto& target : mTargets) {
            target->draw(mWindow);
        }

        if (mQteManager.isActive() || mRocket.isFlying()) {
            sf::Vector2f startPos(290.0f, 550.0f);
            float currentMult = mRocket.isFlying() ? mRocket.getLastQte() : mQteManager.getMultiplier();

            auto points = mRocket.getTrajectoryPoints(currentMult, startPos);
            for (const auto& pos : points) {
                sf::CircleShape dot(3.0f);
                dot.setOrigin(3.0f, 3.0f);
                dot.setPosition(pos);
                mWindow.draw(dot);
            }
        }

        if (mRocket.isFlying()) {
            mWindow.draw(mTireVisual);
        }
        else {
            mTireVisual.setPosition(290.0f, 550.0f);
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
                mTireVisual.rotate(1200.0f * (1.0f / 60.0f));
            }
            else {
                mTireVisual.setRotation(0.0f);
            }
            mWindow.draw(mTireVisual);
        }

        mWindow.draw(mScoreBackground);
        mWindow.draw(mScoreText);
        mWindow.draw(mCoinIcon);
    }

    mWindow.display();
}

void Game::saveProgress() { std::ofstream file("save.txt"); if (file.is_open()) { file << mCoins; file.close(); } }
void Game::loadProgress() { std::ifstream file("save.txt"); if (file.is_open()) { file >> mCoins; file.close(); } }