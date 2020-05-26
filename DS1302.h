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
public:
    enum Mode
    {
        12Hour,
        24Hour
    };

    //Constructor takes the 3 MCU pin numbers connected to the DS1302 chip.
    //Initialization is handled inside, you don't have to use pinMode().
    DS1302(uint8_t chipEnablePin, uint8_t dataPin, uint8_t clockPin);

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
    //Values are internally capped to their natural bounds.
    //Range: <0,59>
    void setSeconds(uint8_t val) const;
    //Range: <0,59>
    void setMinutes(uint8_t val) const;
    //Range: <0,23> for 24 hour mode
    //Range: <1,12> for 12 hour mode
    void setHours(uint8_t val) const;
    //Range <1, n> where n is the number of days in a given month.
    //Leap years are taken into account.
    void setMonthDay(uint8_t val) const;
    //Range: <1,12>
    void setMonth(uint8_t val) const;
    //Range: <1,7>
    void setWeekDay(uint8_t val) const;
    //Range: <00, 99>, corresponding to <2000, 2099>
    void setYear(uint8_t val) const;

    //These can start and stop the timekeeping function of the DS1302.
    //When the clock is stopped, the time is no longer kept
    //but current usage is less tan 100nA.
    void startClock() const;
    void stopClock() const;

    //This function selects either the 12- or 24 hour mode of the clock.
    //Using this functions reset the hour register, so it has to be rewritten afterwards.
    void setHourMode(DS1302::Mode mode) const;

    //These functions interact with the 32 bytes of RAM on the chip.
    //The index is a number of register in the range <0,31>.
    //The value is a regular 8bit value.
    uint8_t getRam(uint8_t index) const;
    void setRam(uint8_t index, uint8_t value) const;

private:
    uint8_t chipEnablePin;
    uint8_t dataPin;
    uint8_t clockPin;

    enum addr
    {
        SECONDS = 0x80,
        MINUTES = 0x82,
        HOURS = 0x84,
        MONTHDAY = 0x86,
        MONTH = 0x88,
        WEEKDAY = 0x8A,
        YEAR = 0x8C,
        WP = 0x8E,
        TCS = 90,
        RAM = 0xC0
    };

    static uint8_t
    toBCD(uint8_t val) const;
    static uint8_t fromBCD(uint8_t val) const;

    void sendByte(uint8_t val) const;
    uint8_t getByte() const;
    void sendMessage(uint8_t addr, uint8_t val) const;
    uint8_t getMessage(uint8_t addr) const;
};

#endif //DS1302_LIB