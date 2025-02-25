#pragma once

#include <chrono>
#include <cstdint>

namespace pwm
{

class PwmIf
{
  public:
    virtual ~PwmIf() = default;
    virtual bool start() = 0;
    virtual bool stop() = 0;
    virtual bool setduty(uint32_t) = 0;
};

} // namespace pwm
