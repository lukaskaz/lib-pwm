#include "log/interfaces/console/logs.hpp"
#include "pwm/interfaces/rpi/soft/pwm.hpp"

#include <algorithm>
#include <iostream>
#include <ranges>

int main(int argc, char** argv)
{
    try
    {
        if (argc == 5)
        {
            std::cout << "PWMs scenario started\n";
            const std::string pwmpath{"/sys/class/pwm/pwmchip2"};
            auto pin = (uint32_t)atoi(argv[1]);
            auto duty = (uint32_t)atoi(argv[2]);
            auto freq = (uint32_t)atoi(argv[3]);
            auto loglvl =
                (bool)atoi(argv[3]) ? logs::level::debug : logs::level::info;

            using namespace pwm::rpi::soft;
            auto logif = logs::Factory::create<logs::console::Log,
                                               logs::console::config_t>(loglvl);
            auto pwm =
                pwm::Factory::create<Pwm, config_t>({pin, duty, freq, logif});

            std::cout << "PWM initiated\n";
            std::cout << "Press [enter]" << std::flush;
            getchar();

            pwm->setduty(25);
            std::cout << "Press [enter]" << std::flush;
            getchar();

            pwm->setduty(75);
            std::cout << "Press [enter]" << std::flush;
            getchar();
            std::cout << "PWM released\n";
        }
    }
    catch (std::exception& err)
    {
        std::cerr << "[ERROR] " << err.what() << '\n';
    }
    return 0;
}
