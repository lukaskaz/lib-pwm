#include "pwm/interfaces/rpi/pca9685/pwm.hpp"

#include "sysfs/interfaces/linux/sysfs.hpp"

#include <chrono>
#include <unordered_map>

namespace pwm::rpi::pca9685
{

using namespace sysfs::lnx;
using namespace std::chrono_literals;

struct Pwm::Handler
{
  public:
    explicit Handler(const config_t& config) :
        id{std::get<0>(config)}, period{freqtoperiod(std::get<2>(config))},
        sysfs{sysfs::Factory::create<Sysfs, configexportrw_t>(
            {std::get<4>(config), type, id})}
    {
        setpolarity(std::get<3>(config));
        setfreq(std::get<2>(config));
        setduty(std::get<1>(config));
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

    bool setduty(double duty)
    {
        return sysfs->write("duty_cycle", std::to_string(perctoduty(duty)));
    }

  private:
    const uint32_t id;
    const std::string type{"pwm"};
    const uint32_t dutymin{0}, dutymax{100};
    const std::chrono::nanoseconds period;
    const std::shared_ptr<sysfs::SysfsIf> sysfs;

    bool setfreq(uint32_t freqhz)
    {
        sysfs->write("period", std::to_string(freqtoperiod(freqhz)));
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

    uint64_t freqtoperiod(uint32_t freqhz)
    {
        static constexpr std::chrono::nanoseconds timehz{1s};
        return std::chrono::nanoseconds(
                   (uint64_t)(timehz.count() / (double)freqhz))
            .count();
    }

    uint64_t perctoduty(double duty)
    {
        if (duty >= dutymin && duty <= dutymax)
            return (uint64_t)((decltype(duty))period.count() * duty / dutymax);
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

bool Pwm::setduty(double duty)
{
    return handler->setduty(duty);
}

} // namespace pwm::rpi::pca9685
