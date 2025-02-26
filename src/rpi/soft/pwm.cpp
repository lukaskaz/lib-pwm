#include "pwm/interfaces/rpi/soft/pwm.hpp"

#include <softPwm.h>
#include <wiringPi.h>

#include <chrono>
#include <cmath>

namespace pwm::rpi::soft
{

using namespace std::chrono_literals;

struct Pwm::Handler
{
  public:
    explicit Handler(const config_t& config) :
        pin{std::get<0>(config)}, period{freqtoperiod(std::get<2>(config))}
    {
        if (wiringPiSetup())
            throw std::runtime_error("Cannot initialize wiringpi");
        start();
        setduty(std::get<1>(config));
    }

    ~Handler()
    {
        setduty(0);
        stop();
    }

    bool start()
    {
        if (!softPwmCreate(pin, 0, (uint32_t)period.count()))
            return true;
        return false;
    }

    bool stop()
    {
        softPwmStop(pin);
        return true;
    }

    bool setduty(double duty)
    {
        softPwmWrite(pin, (uint32_t)perctoduty(duty));
        return true;
    }

  private:
    const uint32_t pin;
    const uint32_t dutymin{0}, dutymax{100};
    const std::chrono::microseconds period;

    uint64_t freqtoperiod(uint32_t freqhz)
    {
        static constexpr std::chrono::microseconds timehz{10ms};
        return std::chrono::microseconds(
                   std::llround(timehz.count() / (double)freqhz))
            .count();
    }

    uint64_t perctoduty(double duty)
    {
        if (duty >= dutymin && duty <= dutymax)
            return std::llround((decltype(duty))period.count() * duty /
                                dutymax);
        throw std::runtime_error("Duty cycle out of range: " +
                                 std::to_string(duty));
    }
};

Pwm::Pwm(const config_t& config) : handler{std::make_unique<Handler>(config)}
{}
Pwm::~Pwm() = default;

bool Pwm::start()
{
    return handler->start();
}

bool Pwm::stop()
{
    return handler->stop();
}

bool Pwm::setduty(double duty)
{
    return handler->setduty(duty);
}

} // namespace pwm::rpi::soft
