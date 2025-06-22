#pragma once
#include "I2SManager.h"

class I2SBlockingGuard {
public:
  I2SBlockingGuard(I2SMode mode) {
    I2SManager::acquireBlocking(mode);
  }

  ~I2SBlockingGuard() {
    I2SManager::release();
  }
};
