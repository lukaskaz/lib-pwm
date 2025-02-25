#include "pwm/interfaces/rpi/pca9685/pwm.hpp"

#include <algorithm>
#include <iostream>
#include <ranges>

int main(int argc, char** argv)
{
    try
    {
        if (argc >= 3)
        {
            const std::string pwmpath{"/sys/class/pwm/pwmchip2"};
            auto duty = (uint32_t)atoi(argv[1]);
            auto period = (uint32_t)atoi(argv[2]);

            using namespace pwm::rpi::pca9685;
            auto iface_0 = pwm::Factory::create<Pwm, config_t>(
                {0, duty, period, polaritytype::normal, pwmpath});
            auto iface_1 = pwm::Factory::create<Pwm, config_t>(
                {1, duty, period, polaritytype::normal, pwmpath});
            std::cout << "Press [enter]" << std::flush;
            getchar();

            iface_0->setduty(duty / 2);
            iface_1->setduty(duty / 4);
            std::cout << "Press [enter]" << std::flush;
            getchar();

            iface_0->setduty(duty + duty / 4);
            iface_1->setduty(duty + duty / 2);
            std::cout << "Press [enter]" << std::flush;
            getchar();
            std::cout << "Clearing pwm\n";
        }
        if (argc == 4)
        {}
    }
    catch (std::exception& err)
    {
        std::cerr << "[ERROR] " << err.what() << '\n';
    }
    return 0;
}
