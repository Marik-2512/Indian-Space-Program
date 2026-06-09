#pragma once
#include <iostream>

class Rocket {
public:
    Rocket()
        : mVelocityX(0.0f), mVelocityY(0.0f), mDistance(0.0f), mAltitude(0.0f), mMaxDistance(0.0f), mIsFlying(false), mGravity(9.81f) {
    }

    // Запуск: передаем результат QTE (0.0 - 1.0)
    void launch(float qteMultiplier) {
        float power = 500.0f + (1500.0f * qteMultiplier);

        mVelocityX = power * 0.7f; // Летим вперед
        mVelocityY = power * 0.7f; // Летим вверх

        mDistance = 0.0f;
        mAltitude = 0.0f;
        mMaxDistance = 0.0f;
        mIsFlying = true;

        std::cout << "[Lot] Wystrzal! Sila poczatkowa: " << power << std::endl;
    }

    // Обновление физики (каждый кадр)
    void update(float dt) {
        if (!mIsFlying) return;

        mDistance += mVelocityX * dt;

        mVelocityY -= mGravity * 100.0f * dt;
        mAltitude += mVelocityY * dt;

        if (mDistance > mMaxDistance) {
            mMaxDistance = mDistance;
        }

        // Упали на землю
        if (mAltitude <= 0.0f) {
            mAltitude = 0.0f;
            mVelocityX = 0.0f;
            mVelocityY = 0.0f;
            mIsFlying = false;

            std::cout << "[Lot] Ladowanie! Przeleciales: " << mMaxDistance << " m" << std::endl;
        }
    }

    bool isFlying() const { return mIsFlying; }
    float getDistance() const { return mDistance; }

private:
    float mVelocityX, mVelocityY;
    float mDistance, mAltitude;
    float mMaxDistance;
    bool mIsFlying;
    float mGravity;
};