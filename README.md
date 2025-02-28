# lib-pwm
Library for controlling pwm output
<br><br>
**Example of the output (w/o and w/ debug logs):**<br>
pi@raspberrypi:\~/git/lib-pwm/build $ ./examples/rpi/pca9685/prog 50 1000 0<br>
PWMs scenario started<br>
[INFO][bool sysfs::lnx::Sysfs::Handler::create()] Created node: /sys/class/pwm/pwmchip2/pwm0<br>
[INFO][pwm::rpi::pca9685::Pwm::Handler::Handler(const pwm::rpi::pca9685::config_t&)] Created pwm [id/polarity/freq/period/duty]: 0/0/1000/1000000/50.000000<br>
[INFO][bool sysfs::lnx::Sysfs::Handler::create()] Created node: /sys/class/pwm/pwmchip2/pwm1<br>
[INFO][pwm::rpi::pca9685::Pwm::Handler::Handler(const pwm::rpi::pca9685::config_t&)] Created pwm [id/polarity/freq/period/duty]: 1/0/1000/1000000/50.000000<br>
PWMs initiated<br>
Press [enter]<br>
Press [enter]<br>
Press [enter]<br>
PWMs released<br>
[INFO][pwm::rpi::pca9685::Pwm::Handler::~Handler()] Removed pwm: 1<br>
[INFO][bool sysfs::lnx::Sysfs::Handler::remove()] Removed node: /sys/class/pwm/pwmchip2/pwm1<br>
[INFO][pwm::rpi::pca9685::Pwm::Handler::~Handler()] Removed pwm: 0<br>
[INFO][bool sysfs::lnx::Sysfs::Handler::remove()] Removed node: /sys/class/pwm/pwmchip2/pwm0<br>
<br>
pi@raspberrypi:\~/git/lib-pwm/build $ ./examples/rpi/pca9685/prog 50 1000 1<br>
PWMs scenario started<br>
[DBG][uint64_t pwm::rpi::pca9685::Pwm::Handler::periodfromfreq(uint32_t)] Period[0]: '1000000' from freq '1000'<br>
[DBG][bool sysfs::lnx::Sysfs::Handler::write(const std::string&, const std::string&, bool) const] Written[0]: '0' to 'export'<br>
[INFO][bool sysfs::lnx::Sysfs::Handler::create()] Created node: /sys/class/pwm/pwmchip2/pwm0<br>
[DBG][bool sysfs::lnx::Sysfs::Handler::write(const std::string&, const std::string&, bool) const] Written[0]: 'normal' to 'polarity'<br>
[DBG][uint64_t pwm::rpi::pca9685::Pwm::Handler::periodfromfreq(uint32_t)] Period[0]: '1000000' from freq '1000'<br>
[DBG][bool sysfs::lnx::Sysfs::Handler::write(const std::string&, const std::string&, bool) const] Written[0]: '1000000' to 'period'<br>
[DBG][uint64_t pwm::rpi::pca9685::Pwm::Handler::dutyfrompct(double)] Duty[0]: '500000' from pct '50.000000'<br>
[DBG][bool sysfs::lnx::Sysfs::Handler::write(const std::string&, const std::string&, bool) const] Written[0]: '500000' to 'duty_cycle'<br>
[DBG][bool sysfs::lnx::Sysfs::Handler::write(const std::string&, const std::string&, bool) const] Written[0]: '1' to 'enable'<br>
[INFO][pwm::rpi::pca9685::Pwm::Handler::Handler(const pwm::rpi::pca9685::config_t&)] Created pwm [id/polarity/freq/period/duty]: 0/0/1000/1000000/50.000000<br>
[DBG][uint64_t pwm::rpi::pca9685::Pwm::Handler::periodfromfreq(uint32_t)] Period[1]: '1000000' from freq '1000'<br>
[DBG][bool sysfs::lnx::Sysfs::Handler::write(const std::string&, const std::string&, bool) const] Written[1]: '1' to 'export'<br>
[INFO][bool sysfs::lnx::Sysfs::Handler::create()] Created node: /sys/class/pwm/pwmchip2/pwm1<br>
[DBG][bool sysfs::lnx::Sysfs::Handler::write(const std::string&, const std::string&, bool) const] Written[1]: 'normal' to 'polarity'<br>
[DBG][uint64_t pwm::rpi::pca9685::Pwm::Handler::periodfromfreq(uint32_t)] Period[1]: '1000000' from freq '1000'<br>
[DBG][bool sysfs::lnx::Sysfs::Handler::write(const std::string&, const std::string&, bool) const] Written[1]: '1000000' to 'period'<br>
[DBG][uint64_t pwm::rpi::pca9685::Pwm::Handler::dutyfrompct(double)] Duty[1]: '500000' from pct '50.000000'<br>
[DBG][bool sysfs::lnx::Sysfs::Handler::write(const std::string&, const std::string&, bool) const] Written[1]: '500000' to 'duty_cycle'<br>
[DBG][bool sysfs::lnx::Sysfs::Handler::write(const std::string&, const std::string&, bool) const] Written[1]: '1' to 'enable'<br>
[INFO][pwm::rpi::pca9685::Pwm::Handler::Handler(const pwm::rpi::pca9685::config_t&)] Created pwm [id/polarity/freq/period/duty]: 1/0/1000/1000000/50.000000<br>
PWMs initiated<br>
Press [enter]<br>
![Ex. of two pwms, first case](pictures/pwm_onboard_first.jpg "Measurement of two PWM signals with freq 1000 hz and duty level 50%/50%")<br>
[DBG][uint64_t pwm::rpi::pca9685::Pwm::Handler::dutyfrompct(double)] Duty[0]: '250000' from pct '25.000000'<br>
[DBG][bool sysfs::lnx::Sysfs::Handler::write(const std::string&, const std::string&, bool) const] Written[0]: '250000' to 'duty_cycle'<br>
[DBG][uint64_t pwm::rpi::pca9685::Pwm::Handler::dutyfrompct(double)] Duty[1]: '750000' from pct '75.000000'<br>
[DBG][bool sysfs::lnx::Sysfs::Handler::write(const std::string&, const std::string&, bool) const] Written[1]: '750000' to 'duty_cycle'<br>
Press [enter]<br>
![Ex. of two pwms, second case](pictures/pwm_onboard_second.jpg "Measurement of two PWM signals with freq 1000 hz and duty level 25%/75%")<br>
[DBG][uint64_t pwm::rpi::pca9685::Pwm::Handler::dutyfrompct(double)] Duty[0]: '750000' from pct '75.000000'<br>
[DBG][bool sysfs::lnx::Sysfs::Handler::write(const std::string&, const std::string&, bool) const] Written[0]: '750000' to 'duty_cycle'<br>
[DBG][uint64_t pwm::rpi::pca9685::Pwm::Handler::dutyfrompct(double)] Duty[1]: '250000' from pct '25.000000'<br>
[DBG][bool sysfs::lnx::Sysfs::Handler::write(const std::string&, const std::string&, bool) const] Written[1]: '250000' to 'duty_cycle'<br>
Press [enter]<br>
![Ex. of two pwms, third case](pictures/pwm_onboard_third.jpg "Measurement of two PWM signals with freq 1000 hz and duty level 75%/25%")<br>
PWMs released<br>
[DBG][uint64_t pwm::rpi::pca9685::Pwm::Handler::dutyfrompct(double)] Duty[1]: '0' from pct '0.000000'<br>
[DBG][bool sysfs::lnx::Sysfs::Handler::write(const std::string&, const std::string&, bool) const] Written[1]: '0' to 'duty_cycle'<br>
[DBG][uint64_t pwm::rpi::pca9685::Pwm::Handler::periodfromfreq(uint32_t)] Period[1]: '0' from freq '0'<br>
[DBG][bool sysfs::lnx::Sysfs::Handler::write(const std::string&, const std::string&, bool) const] Written[1]: '0' to 'period'<br>
[DBG][bool sysfs::lnx::Sysfs::Handler::write(const std::string&, const std::string&, bool) const] Written[1]: '0' to 'enable'<br>
[INFO][pwm::rpi::pca9685::Pwm::Handler::~Handler()] Removed pwm: 1<br>
[DBG][bool sysfs::lnx::Sysfs::Handler::write(const std::string&, const std::string&, bool) const] Written[1]: '1' to 'unexport'<br>
[INFO][bool sysfs::lnx::Sysfs::Handler::remove()] Removed node: /sys/class/pwm/pwmchip2/pwm1<br>
[DBG][uint64_t pwm::rpi::pca9685::Pwm::Handler::dutyfrompct(double)] Duty[0]: '0' from pct '0.000000'<br>
[DBG][bool sysfs::lnx::Sysfs::Handler::write(const std::string&, const std::string&, bool) const] Written[0]: '0' to 'duty_cycle'<br>
[DBG][uint64_t pwm::rpi::pca9685::Pwm::Handler::periodfromfreq(uint32_t)] Period[0]: '0' from freq '0'<br>
[DBG][bool sysfs::lnx::Sysfs::Handler::write(const std::string&, const std::string&, bool) const] Written[0]: '0' to 'period'<br>
[DBG][bool sysfs::lnx::Sysfs::Handler::write(const std::string&, const std::string&, bool) const] Written[0]: '0' to 'enable'<br>
[INFO][pwm::rpi::pca9685::Pwm::Handler::~Handler()] Removed pwm: 0<br>
[DBG][bool sysfs::lnx::Sysfs::Handler::write(const std::string&, const std::string&, bool) const] Written[0]: '0' to 'unexport'<br>
[INFO][bool sysfs::lnx::Sysfs::Handler::remove()] Removed node: /sys/class/pwm/pwmchip2/pwm0<br>
