#pragma once

#include "pwm/interfaces/pwm.hpp"

#include <memory>

namespace pwm
{

class Factory
{
  public:
    template <typename T, typename C>
    static std::shared_ptr<PwmIf> create(const C& config)
    {
        return std::shared_ptr<T>(new T(config));
    }
};

} // namespace pwm
