#include "pwm/interfaces/rpi/pca9685/pwm.hpp"

#include "sysfs/interfaces/linux/sysfs.hpp"

#include <algorithm>
#include <functional>
#include <future>
#include <ranges>

namespace pwm::rpi::pca9685
{

using namespace sysfs::lnx;

struct Pwm::Handler
{
  public:
    explicit Handler(const config_t& config) :
        id{std::get<0>(config)}, dutymax{std::get<2>(config)},
        sysfs{sysfs::Factory::create<Sysfs, configexportrw_t>(
            {std::get<4>(config), "pwm", id})}
    {
        setduty(std::get<1>(config));
        setperiod(std::get<2>(config));
        setpolarity(std::get<3>(config));
        start();
    }

    ~Handler()
    {
        setduty(0);
        setperiod(0);
        stop();
    }

    bool setduty(uint32_t duty)
    {
        return sysfs->write("duty_cycle", std::to_string(getduty(duty)));
    }

  private:
    uint32_t id;
    const uint32_t dutypctmin{0}, dutypctmax{100};
    const uint32_t dutymax;
    std::shared_ptr<sysfs::SysfsIf> sysfs;

    uint32_t getduty(uint32_t duty)
    {
        if (duty <= dutymax)
            return duty;
        throw std::runtime_error(
            "duty cycle out of range: " + std::to_string(duty) + "/" +
            std::to_string(dutymax));
    }

    bool setperiod(uint32_t period)
    {
        sysfs->write("period", std::to_string(period));
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
};

Pwm::Pwm(const config_t& config) : handler{std::make_unique<Handler>(config)}
{}
Pwm::~Pwm() = default;

bool Pwm::setduty(uint32_t duty)
{
    return handler->setduty(duty);
}

} // namespace pwm::rpi::pca9685
