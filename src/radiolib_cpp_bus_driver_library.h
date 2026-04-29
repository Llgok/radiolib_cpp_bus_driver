/*
 * @Description: radiolib_bridge_driver
 * @Author: LILYGO_L
 * @Date: 2025-07-31 16:06:23
 * @LastEditTime: 2026-04-24 16:50:00
 * @License: GPL 3.0
 */
#pragma once

#undef ARDUINO

#include "RadioLib.h"
#include "cpp_bus_driver_library.h"

class RadiolibCppBusDriverHal : public RadioLibHal {
 public:
  std::shared_ptr<cpp_bus_driver::BusSpiGuide> bus_;
  int32_t _freq_hz, _cs;

  explicit RadiolibCppBusDriverHal(
      std::shared_ptr<cpp_bus_driver::BusSpiGuide> bus, int32_t freq_hz = -1,
      int32_t cs = -1)
      : RadioLibHal(
            static_cast<uint32_t>(cpp_bus_driver::Tool::GpioMode::kInput),
            static_cast<uint32_t>(cpp_bus_driver::Tool::GpioMode::kOutput), 0,
            1,
            static_cast<uint32_t>(cpp_bus_driver::Tool::InterruptMode::kRising),
            static_cast<uint32_t>(
                cpp_bus_driver::Tool::InterruptMode::kFalling)),
        bus_(bus),
        _freq_hz(freq_hz),
        _cs(cs) {}

  // implementations of pure virtual RadioLibHal methods
  void pinMode(uint32_t pin, uint32_t mode) override;
  void digitalWrite(uint32_t pin, uint32_t value) override;
  uint32_t digitalRead(uint32_t pin) override;
  void attachInterrupt(
      uint32_t interruptNum, void (*interruptCb)(void), uint32_t mode) override;
  void detachInterrupt(uint32_t interruptNum) override;
  void delay(RadioLibTime_t ms) override;
  void delayMicroseconds(RadioLibTime_t us) override;
  RadioLibTime_t millis() override;
  RadioLibTime_t micros() override;
  long pulseIn(uint32_t pin, uint32_t state, RadioLibTime_t timeout) override;
  void spiBegin() override;
  void spiBeginTransaction() override;
  void spiTransfer(uint8_t* out, size_t len, uint8_t* in) override;
  void spiEndTransaction() override;
  void spiEnd() override;

  // implementations of virtual RadioLibHal methods
  void init() override;
  // void term() override;
  // void tone(uint32_t pin, unsigned int frequency, RadioLibTime_t duration =
  // 0) override; void noTone(uint32_t pin) override;
  void yield() override;
  // uint32_t pinToInterrupt(uint32_t pin) override;
};
