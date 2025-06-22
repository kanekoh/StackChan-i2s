#pragma once
#include <Arduino.h>
#include <M5Unified.h>

enum class I2SMode {
  Idle,
  Recording,
  Playing
};

class I2SManager {
public:
  static void begin() {
    if (!mutex) {
      mutex = xSemaphoreCreateRecursiveMutex();
    }
  }

  static void acquireBlocking(I2SMode mode) {
    begin();
    while (true) {
      if (xSemaphoreTakeRecursive(mutex, portMAX_DELAY) == pdTRUE) {
        currentMode = mode;
        lastLockMillis = millis();
        Serial.printf("[I2SManager] Lock acquired (blocking): %s\n", modeToStr(mode));
        return;
      }
    }
  }

  static void release() {
    if (xSemaphoreGetMutexHolder(mutex) == xTaskGetCurrentTaskHandle()) {
      currentMode = I2SMode::Idle;
      xSemaphoreGiveRecursive(mutex);
      Serial.println("[I2SManager] Lock released");
    } else {
      Serial.println("⚠️ I2SManager: Tried to release from wrong task");
    }
  }

  static I2SMode getCurrentMode() { return currentMode; }

private:
  static SemaphoreHandle_t mutex;
  static I2SMode currentMode;
  static unsigned long lastLockMillis;

  static const char* modeToStr(I2SMode mode) {
    switch (mode) {
      case I2SMode::Recording: return "Recording";
      case I2SMode::Playing: return "Playing";
      default: return "Idle";
    }
  }
};
