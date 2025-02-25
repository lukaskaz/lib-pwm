#pragma once

#include "pwm/factory.hpp"

#include <tuple>

namespace pwm::rpi::pca9685
{

enum class polaritytype
{
    normal,
    inversed
};

using config_t =
    std::tuple<uint32_t, uint32_t, uint32_t, polaritytype, std::string>;

class Pwm : public PwmIf
{
  public:
    ~Pwm();
    bool setduty(uint32_t) override;

  private:
    friend class pwm::Factory;
    explicit Pwm(const config_t&);

    struct Handler;
    std::unique_ptr<Handler> handler;
};

} // namespace pwm::rpi::pca9685
