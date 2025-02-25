#pragma once

#include "pwm/factory.hpp"

#include <string>
#include <tuple>

namespace pwm::rpi::onboard
{

enum class polaritytype
{
    normal,
    inverted
};

using config_t = std::tuple<uint32_t, uint32_t, polaritytype, std::string>;

class Pwm : public PwmIf
{
  public:
    ~Pwm();
    bool setduty() override;

  private:
    friend class pwm::Factory;
    explicit Pwm(const config_t&);

    struct Handler;
    std::unique_ptr<Handler> handler;
};

} // namespace pwm::rpi::onboard
