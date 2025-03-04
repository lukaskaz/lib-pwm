#include "pwm/interfaces/rpi/onboard/pwm.hpp"

#include "sysfs/interfaces/linux/sysfs.hpp"

#include <chrono>
#include <cmath>
#include <source_location>
#include <unordered_map>

namespace pwm::rpi::onboard
{

using namespace sysfs::lnx;
using namespace std::chrono_literals;

struct Pwm::Handler
{
  public:
    explicit Handler(const config_t& config) :
        logif{std::get<5>(config)}, id{std::get<0>(config)},
        period{periodfromfreq(std::get<2>(config))},
        sysfs{sysfs::Factory::create<Sysfs, configexportrw_t>(
            {std::get<4>(config), type, id, logif})}
    {
        setpolarity(std::get<3>(config));
        setfreq(std::get<2>(config));
        setduty(std::get<1>(config));
        start();
        log(logs::level::info,
            "Created pwm [id/polarity/freq/period/duty]: " +
                std::to_string(id) + "/" +
                std::to_string((int32_t)std::get<3>(config)) + "/" +
                std::to_string(std::get<2>(config)) + "/" +
                std::to_string(period.count()) + "/" +
                std::to_string(std::get<1>(config)));
    }

    ~Handler()
    {
        setduty(0);
        setfreq(0);
        stop();
        log(logs::level::info, "Removed pwm: " + std::to_string(id));
    }

    bool start()
    {
        sysfs->write("enable", std::to_string(1));
        return true;
    }

    bool stop()
    {
        sysfs->write("enable", std::to_string(0));
        return true;
    }

    bool setduty(double duty)
    {
        return sysfs->write("duty_cycle", std::to_string(dutyfrompct(duty)));
    }

  private:
    const std::shared_ptr<logs::LogIf> logif;
    const uint32_t id;
    const std::string type{"pwm"};
    const uint32_t dutymin{0}, dutymax{100};
    const std::chrono::nanoseconds period;
    const std::shared_ptr<sysfs::SysfsIf> sysfs;

    bool setfreq(uint32_t freqhz)
    {
        sysfs->write("period", std::to_string(periodfromfreq(freqhz)));
        return true;
    }

    bool setpolarity(polaritytype polarity)
    {
        static const std::unordered_map<polaritytype, std::string> map = {
            {polaritytype::normal, "normal"},
            {polaritytype::inversed, "inversed"}};
        sysfs->write("polarity", map.at(polarity));
        return true;
    }

    uint64_t periodfromfreq(uint32_t freqhz)
    {
        static constexpr std::chrono::nanoseconds timehz{1s};
        const auto period =
            freqhz ? std::chrono::nanoseconds(
                         std::llround(timehz.count() / (double)freqhz))
                         .count()
                   : 0;
        log(logs::level::debug, "Period[" + std::to_string(id) + "]: '" +
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
            log(logs::level::debug, "Duty[" + std::to_string(id) + "]: '" +
                                        std::to_string(duty) + "' from pct '" +
                                        std::to_string(dutypct) + "'");
            return duty;
        }
        throw std::runtime_error("Duty cycle out of range[" +
                                 std::to_string(id) +
                                 "]: " + std::to_string(dutypct));
    }

    void log(
        logs::level level, const std::string& msg,
        const std::source_location loc = std::source_location::current()) const
    {
        if (logif)
            logif->log(level, std::string{loc.function_name()}, msg);
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

} // namespace pwm::rpi::onboard
