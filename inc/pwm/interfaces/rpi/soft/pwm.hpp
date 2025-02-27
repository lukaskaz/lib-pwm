#pragma once

#include "log/interfaces/logging.hpp"
#include "pwm/factory.hpp"

#include <tuple>

namespace pwm::rpi::soft
{

using config_t =
    std::tuple<uint32_t, double, uint32_t, std::shared_ptr<logging::LogIf>>;

class Pwm : public PwmIf
{
  public:
    ~Pwm();
    bool start() override;
    bool stop() override;
    bool setduty(double) override;

  private:
    friend class pwm::Factory;
    explicit Pwm(const config_t&);

    struct Handler;
    std::unique_ptr<Handler> handler;
};

} // namespace pwm::rpi::soft
