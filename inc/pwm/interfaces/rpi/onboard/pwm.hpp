#pragma once

#include "log/interfaces/logs.hpp"
#include "pwm/factory.hpp"

#include <string>
#include <tuple>

namespace pwm::rpi::onboard
{

enum class polaritytype
{
    normal,
    inversed
};

using config_t = std::tuple<uint32_t, double, uint32_t, polaritytype,
                            std::string, std::shared_ptr<logs::LogIf>>;

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

} // namespace pwm::rpi::onboard
