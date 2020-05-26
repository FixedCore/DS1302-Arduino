#ifndef DS1302_LIB
#define DS1302_LIB
#define BCDHI(x) ((x)&0xF0)
#define BCDLO(x) ((x)&0x0F)
#define TOBIT(x) ((x) != 0)
//#define uint8_t unsigned short int
//#define int8_t short int

/*This is a library for communication with the
//Maxim DS1302 real time clock IC. It's made for use with the ArduinoIDE and
//whatever microcontroller or development board it supports.
//I use the Arduino DigitalWrite/Read functions because I'm too lazy to 
//deal with selecting the right port for each pin number, and so that
//there's no need to port anything.
*/

class DS1302
{
    //public constants:
    //these are actually just commands for writing data to the IC
    //Incrementing them by 1 makes them read data.
public:
    const static uint8_t SECONDS = 0x80;
    const static uint8_t MINUTES = 0x82;
    const static uint8_t HOURS = 0x84;
    const static uint8_t DATE = 0x86;
    const static uint8_t MONTH = 0x88;
    const static uint8_t WEEKDAY = 0x8A;
    const static uint8_t YEAR = 0x8C;

    enum Mode
    {
        12Hour,
        24Hour
    };

private:
    uint8_t chipEnablePin;
    uint8_t dataPin;
    uint8_t clockPin;

public:
    DS1302(uint8_t chipEnablePin, uint8_t dataPin, uint8_t clockPin)
        : chipEnablePin(chipEnablePin), dataPin(dataPin), clockPin(clockPin){};

    //These functions return their values as regular 8 bit unsigned values.
    //No extra conversions required.
    //There's no error checking,
    //when just starting to use this you have to make sure your setup works.
    uint8_t getSeconds() const;
    uint8_t getMinutes() const;
    uint8_t getHours() const;
    uint8_t getMonthDay() const;
    uint8_t getMonth() const;
    uint8_t getWeekDay() const;
    uint8_t getYear() const;

    //These functions take a single 8 bit unsigned value, in regular binary format.
    //No extra conversions required.
    //No error checking either.
    void setSeconds(uint8_t val) const;
    void setMinutes(uint8_t val) const;
    void setHours(uint8_t val) const;
    void setMonthDay(uint8_t val) const;
    void setMonth(uint8_t val) const;
    void setWeekDay(uint8_t val) const;
    void setYear(uint8_t val) const;

    //These can start and stop the timekeeping function of the DS1302.
    //When the clock is stopped, the time is no longer kept
    //but current usage is less tan 100nA.
    void startClock();
    void stopClock();


};

#endif //DS1302_LIB