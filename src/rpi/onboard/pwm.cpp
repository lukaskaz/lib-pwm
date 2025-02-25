#include "pwm/interfaces/rpi/onboard/pwm.hpp"

#include "sysfs/interfaces/linux/sysfs.hpp"

#include <algorithm>
#include <functional>
#include <future>
#include <ranges>

namespace pwm::rpi::onboard
{

using namespace sysfs::lnx;

struct Pwm::Handler
{
  public:
    explicit Handler(const config_t& config) :
        id{std::get<0>(config)}, period{freqtoperiod(std::get<2>(config))},
        sysfs{sysfs::Factory::create<Sysfs, configexportrw_t>(
            {std::get<4>(config), type, id})}
    {
        setduty(std::get<1>(config));
        setfreq(std::get<2>(config));
        setpolarity(std::get<3>(config));
        start();
    }

    ~Handler()
    {
        setduty(0);
        setfreq(0);
        stop();
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

    bool setduty(uint32_t duty)
    {
        return sysfs->write("duty_cycle", std::to_string(perctoduty(duty)));
    }

  private:
    const uint32_t id;
    const std::string type{"pwm"};
    const uint32_t dutymin{0}, dutymax{100};
    const std::chrono::nanoseconds period;
    std::shared_ptr<sysfs::SysfsIf> sysfs;

    bool setfreq(uint32_t freqhz)
    {
        sysfs->write("period", std::to_string(freqtoperiod(freqhz)));
        return true;
    }

    bool setpolarity(polaritytype polarity)
    {
        std::unordered_map<polaritytype, std::string> map = {
            {polaritytype::normal, "normal"},
            {polaritytype::inversed, "inversed"}};
        sysfs->write("polarity", map[polarity]);
        return true;
    }

    uint64_t freqtoperiod(uint32_t freqhz)
    {
        return std::chrono::nanoseconds(
                   (uint64_t)(1000000000.f / (double)freqhz))
            .count();
    }

    uint64_t perctoduty(uint32_t duty)
    {
        if (duty >= dutymin && duty <= dutymax)
            return period.count() * duty / dutymax;
        throw std::runtime_error("duty cycle out of range: " +
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

bool Pwm::setduty(uint32_t duty)
{
    return handler->setduty(duty);
}

} // namespace pwm::rpi::onboard
