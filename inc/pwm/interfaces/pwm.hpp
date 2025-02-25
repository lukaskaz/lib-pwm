#pragma once

#include <cstdint>
#include <vector>

namespace pwm
{

class PwmIf
{
  public:
    virtual ~PwmIf() = default;
    virtual bool setduty(uint32_t) = 0;
};

} // namespace pwm
