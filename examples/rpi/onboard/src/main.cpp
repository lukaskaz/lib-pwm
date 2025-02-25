#include "pwm/interfaces/rpi/onboard/pwm.hpp"

#include <algorithm>
#include <iostream>
#include <ranges>

int main(int argc, char** argv)
{
    try
    {
        if (argc >= 3)
        {
            std::cout << "PWMs scenario started\n";
            const std::string pwmpath{"/sys/class/pwm/pwmchip2"};
            auto duty = (uint32_t)atoi(argv[1]);
            auto freq = (uint32_t)atoi(argv[2]);

            using namespace pwm::rpi::onboard;
            auto pwm0 = pwm::Factory::create<Pwm, config_t>(
                {0, duty, freq, polaritytype::normal, pwmpath});
            auto pwm1 = pwm::Factory::create<Pwm, config_t>(
                {1, duty, freq, polaritytype::normal, pwmpath});

            std::cout << "PWMs initiated\n";
            std::cout << "Press [enter]" << std::flush;
            getchar();

            pwm0->setduty(25);
            pwm1->setduty(75);
            std::cout << "Press [enter]" << std::flush;
            getchar();

            pwm0->setduty(75);
            pwm1->setduty(25);
            std::cout << "Press [enter]" << std::flush;
            getchar();
            std::cout << "PWMs released\n";
        }
    }
    catch (std::exception& err)
    {
        std::cerr << "[ERROR] " << err.what() << '\n';
    }
    return 0;
}
