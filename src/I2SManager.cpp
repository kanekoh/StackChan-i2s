#include "I2SManager.h"

SemaphoreHandle_t I2SManager::mutex = nullptr;
I2SMode I2SManager::currentMode = I2SMode::Idle;
unsigned long I2SManager::lastLockMillis = 0;
