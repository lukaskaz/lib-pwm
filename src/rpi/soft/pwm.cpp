#include "pwm/interfaces/rpi/soft/pwm.hpp"

#include <softPwm.h>
#include <wiringPi.h>

#include <chrono>
#include <cmath>
#include <source_location>

namespace pwm::rpi::soft
{

using namespace std::chrono_literals;

struct Pwm::Handler
{
  public:
    explicit Handler(const config_t& config) :
        logif{std::get<3>(config)}, pin{std::get<0>(config)},
        period{periodfromfreq(std::get<2>(config))}

    {
        if (wiringPiSetup())
            throw std::runtime_error("Cannot initialize wiringpi");
        start();
        setduty(std::get<1>(config));
        log(logging::type::info,
            "Created pwm [pin/freq/period/duty]: " + std::to_string(pin) + "/" +
                std::to_string(std::get<2>(config)) + "/" +
                std::to_string(period.count()) + "/" +
                std::to_string(std::get<1>(config)));
    }

    ~Handler()
    {
        setduty(0);
        stop();
        log(logging::type::info, "Removed pwm pin: " + std::to_string(pin));
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
        softPwmWrite(pin, (uint32_t)dutyfrompct(duty));
        return true;
    }

  private:
    const std::shared_ptr<logging::LogIf> logif;
    const uint32_t pin;
    const uint32_t dutymin{0}, dutymax{100};
    const std::chrono::microseconds period;

    uint64_t periodfromfreq(uint32_t freqhz)
    {
        static constexpr std::chrono::microseconds timehz{10ms};
        const auto period =
            freqhz ? std::chrono::microseconds(
                         std::llround(timehz.count() / (double)freqhz))
                         .count()
                   : 0;
        log(logging::type::debug, "Period[" + std::to_string(pin) + "]: '" +
                                      std::to_string(period) + "' from freq '" +
                                      std::to_string(freqhz) + "'");
        return period;
    }

    uint64_t dutyfrompct(double dutypct)
    {
        if (dutypct >= dutymin && dutypct <= dutymax)
        {
            const auto duty = std::llround((decltype(dutypct))period.count() *
                                           dutypct / dutymax);
            log(logging::type::debug,
                "Duty[" + std::to_string(pin) + "]: '" + std::to_string(duty) +
                    "' from pct '" + std::to_string(dutypct) + "'");
            return duty;
        }
        throw std::runtime_error("Duty cycle out of range[" +
                                 std::to_string(pin) +
                                 "]: " + std::to_string(dutypct));
    }

    void log(
        logging::type type, const std::string& msg,
        const std::source_location loc = std::source_location::current()) const
    {
        if (logif)
            logif->log(type, std::string{loc.function_name()}, msg);
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
