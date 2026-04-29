/*
 * @Description: None
 * @Author: LILYGO_L
 * @Date: 2025-07-31 16:06:23
 * @LastEditTime: 2026-04-29 10:25:34
 * @License: GPL 3.0
 */
#include "radiolib_cpp_bus_driver_library.h"

struct InterruptArg {
  std::function<void(void)> interrupt_function;
};

std::unordered_map<uint8_t, std::unique_ptr<InterruptArg>> g_interrupt_map;

static void IRAM_ATTR InterruptCallbackTemplate(void* arg) {
  auto* local_arg = static_cast<InterruptArg*>(arg);
  if (local_arg->interrupt_function) {
    local_arg->interrupt_function();
  }
}

void inline RadiolibCppBusDriverHal::pinMode(uint32_t pin, uint32_t mode) {
  if (pin == RADIOLIB_NC) {
    bus_->LogMessage(cpp_bus_driver::Tool::LogLevel::kInfo, __FILE__, __LINE__,
        "Invalid argument\n");
    return;
  }
  bus_->SetGpioMode(pin, static_cast<cpp_bus_driver::Tool::GpioMode>(mode));
}

void inline RadiolibCppBusDriverHal::digitalWrite(
    uint32_t pin, uint32_t value) {
  if (pin == RADIOLIB_NC) {
    // bus_->LogMessage(cpp_bus_driver::Tool::LogLevel::kInfo, __FILE__,
    // __LINE__, "Invalid argument\n");
    return;
  }
  bus_->GpioWrite(pin, value);
}

uint32_t inline RadiolibCppBusDriverHal::digitalRead(uint32_t pin) {
  if (pin == RADIOLIB_NC) {
    // bus_->LogMessage(cpp_bus_driver::Tool::LogLevel::kInfo, __FILE__,
    // __LINE__, "Invalid argument\n");
    return 0;
  }
  return bus_->GpioRead(pin);
}

void inline RadiolibCppBusDriverHal::attachInterrupt(
    uint32_t interruptNum, void (*interruptCb)(void), uint32_t mode) {
  if (interruptNum == RADIOLIB_NC) {
    bus_->LogMessage(cpp_bus_driver::Tool::LogLevel::kInfo, __FILE__, __LINE__,
        "Invalid argument\n");
    return;
  }

  auto arg = std::make_unique<InterruptArg>(interruptCb);
  g_interrupt_map[interruptNum] = std::move(arg);

  if (!bus_->InitGpioInterrupt(interruptNum,
          static_cast<cpp_bus_driver::Tool::InterruptMode>(mode),
          InterruptCallbackTemplate, g_interrupt_map[interruptNum].get())) {
    bus_->LogMessage(cpp_bus_driver::Tool::LogLevel::kBus, __FILE__, __LINE__,
        "InitGpioInterrupt fail\n");
  }
}

void inline RadiolibCppBusDriverHal::detachInterrupt(uint32_t interruptNum) {
  if (interruptNum == RADIOLIB_NC) {
    bus_->LogMessage(cpp_bus_driver::Tool::LogLevel::kInfo, __FILE__, __LINE__,
        "Invalid argument\n");
    return;
  }
  if (!bus_->DeinitGpioInterrupt(interruptNum)) {
    bus_->LogMessage(cpp_bus_driver::Tool::LogLevel::kBus, __FILE__, __LINE__,
        "DeleteGpioInterrupt fail\n");
  }
}

void inline RadiolibCppBusDriverHal::delay(RadioLibTime_t ms) {
  bus_->DelayMs(static_cast<uint32_t>(ms));
}

void inline RadiolibCppBusDriverHal::delayMicroseconds(RadioLibTime_t us) {
  bus_->DelayUs(static_cast<uint32_t>(us));
}

RadioLibTime_t inline RadiolibCppBusDriverHal::millis() {
  return bus_->GetSystemTimeMs();
}

RadioLibTime_t inline RadiolibCppBusDriverHal::micros() {
  return bus_->GetSystemTimeUs();
}

long inline RadiolibCppBusDriverHal::pulseIn(
    uint32_t pin, uint32_t state, RadioLibTime_t timeout) {
  if (pin == RADIOLIB_NC) {
    bus_->LogMessage(cpp_bus_driver::Tool::LogLevel::kInfo, __FILE__, __LINE__,
        "Invalid argument\n");
    return 0;
  }

  bus_->SetGpioMode(pin, cpp_bus_driver::Tool::GpioMode::kInput);
  uint32_t start = bus_->GetSystemTimeUs();
  uint32_t curtick = bus_->GetSystemTimeUs();

  while (bus_->GpioRead(pin) == state) {
    if ((bus_->GetSystemTimeUs() - curtick) > timeout) {
      return 0;
    }
  }

  return (bus_->GetSystemTimeUs() - start);
}

void inline RadiolibCppBusDriverHal::spiBegin() {
  if (!bus_->Init(_freq_hz, _cs)) {
    bus_->LogMessage(cpp_bus_driver::Tool::LogLevel::kBus, __FILE__, __LINE__,
        "Init fail\n");
  }
}

void inline RadiolibCppBusDriverHal::spiBeginTransaction() { return; }

void RadiolibCppBusDriverHal::spiTransfer(
    uint8_t* out, size_t len, uint8_t* in) {
  if (!bus_->WriteRead(out, in, len)) {
    bus_->LogMessage(cpp_bus_driver::Tool::LogLevel::kBus, __FILE__, __LINE__,
        "WriteRead fail\n");
  }
}

void inline RadiolibCppBusDriverHal::spiEndTransaction() { return; }

void inline RadiolibCppBusDriverHal::spiEnd() { return; }

void RadiolibCppBusDriverHal::init() {
  if (!bus_->Init(_freq_hz, _cs)) {
    bus_->LogMessage(cpp_bus_driver::Tool::LogLevel::kBus, __FILE__, __LINE__,
        "Init fail\n");
  }
}

void inline RadiolibCppBusDriverHal::yield() { bus_->DelayMs(10); }
