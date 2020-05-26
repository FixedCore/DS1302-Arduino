#include "DS1302.h"
DS1302::DS1302(uint8_t chipEnablePin, uint8_t dataPin, uint8_t clockPin)
    : chipEnablePin(chipEnablePin), dataPin(dataPin), clockPin(clockPin)
{
}

void begin() const
{
    pinMode(chipEnablePin, OUTPUT);
    pinMode(dataPin, OUTPUT);
    pinMode(clockPin, OUTPUT);
}

static uint8_t DS1302::toBCD(uint8_t val) const
{
    return ((val / 10) << 4 | (val % 10));
}

static uint8_t DS1302::fromBCD(uint8_t val) const
{
    return (BCDHI(val) >> 4) * 10 + BCDLO(val);
}

void DS1302::sendByte(uint8_t val) const
{
    for (uint8_t i = 0; i < 8; i++)
    {
        digitalWrite(dataPin, (val >> i) & 1);
        digitalWrite(clockPin, 1);
        delayMicroseconds(1);
        digitalWrite(clockPin, 0);
    }
}
uint8_t DS1302::getByte() const
{
    uint8_t ret = 0;
    for (uint8_t i = 0; i < 8; i++)
    {
        ret |= (digitalRead(dataPin) << i);
        digitalWrite(clockPin, 1);
        delayMicroseconds(1);
        digitalWrite(clockPin, 0);
    }
    return ret;
}

void DS1302::sendMessage(uint8_t addr, uint8_t val) const
{
    digitalWrite(ChipEnablePin, HIGH);
    pinMode(dataPin, OUTPUT);
    sendByte(addr);
    sendByte(val);
    digitalWrite(ChipEnablePin, LOW);
}
uint8_t DS1302::getMessage(uint8_t addr) const
{
    digitalWrite(ChipEnablePin, HIGH);
    pinMode(dataPin, OUTPUT);
    sendByte(addr + 1);
    pinMode(dataPin, INPUT);
    uint8_t ret = getByte();
    digitalWrite(ChipEnablePin, LOW);
    return ret;
}

uint8_t DS1302::getSeconds() const
{
    return fromBCD(getMessage(addr::SECONDS) & 0x7f);
    //magic to discard the CH bit
}

uint8_t DS1302::getMinutes() const
{
    return fromBCD(getMessage(addr::MINUTES));
    //no extra bits here
}

uint8_t DS1302::getHours() const
{
    uint8_t raw = getMessage(addr::HOURS);
    if (raw & 0x80)                 //12 hour mode
        return fromBCD(raw & 0x1F); //5 bits
    else                            //24 hour mode
        return fromBCD(raw & 0x3F); //6 bits
}

uint8_t DS1302::getMonthDay() const
{
    return fromBCD(getMessage(addr::MONTHDAY));
    //no extra bits here
}

uint8_t DS1302::getMonth() const
{
    return fromBCD(getMessage(addr::MONTH));
    //no extra bits here
}

uint8_t DS1302::getWeekDay() const
{
    return fromBCD(getMessage(addr::WEEKDAY));
    //no extra bits here
}

uint8_t DS1302::getYear() const
{
    return fromBCD(getMessage(addr::YEAR));
    //no extra bits here
}

void DS1302::setSeconds(uint8_t val) const
{
    uint8_t backup = getMessage(addr::SECONDS);
    sendMessage(addr::SECONDS, toBCD(val % 60) | (backup & 0x80));
    //Bitmagic to preserve the ClockHalt bit
}

void DS1302::setMinutes(uint8_t val) const
{
    sendMessage(addr::MINUTES, toBCD(val % 60));
}

void DS1302::setHours(uint8_t val) const
{
    uint8_t backup = getMessage(addr::HOURS);
    uint8_t cap = backup & 0x80 ? 12 : 24; //12- or 24 hour mode
    if (cap == 24)
    {
        val %= 24;
    }
    else
    {
        val %= 13;
        if (val == 0)
            val = 12;
    }
    sendMessage(addr::HOURS, toBCD(val));
}

void DS1302::setMonthDay(uint8_t val) const //This is unnecessarily complicated
{
    uint8_t month = fromBCD(getMessage(addr::MONTH));
    uint8_t cap = 32;
    switch (month)
    {
    case 4:
    case 6:
    case 9:
    case 11:
        cap = 31;
        break;
    case 2:
        month = fromBCD(getMessage(addr::YEAR));                        //month is now year, deal with it
        if ((month % 4 == 0 && month % 100 != 0) || (month % 400 == 0)) //Leap year
            cap = 30;
        else
            cap = 29;
    }
    if (val == 0)
        val = cap - 1;
    else
        val %= cap;
    sendMessage(addr::MONTHDAY, toBCD(val));
}

void DS1302::setMonth(uint8_t val) const
{
    if (val == 0)
        val = 12;
    sendMessage(addr::MONTH, toBCD(val % 13));
}

void DS1302::setWeekDay(uint8_t val) const
{
    if (val == 0)
        val = 7;
    sendMessage(addr::WEEKDAY, toBCD(val % 8));
}

void DS1302::setYear(uint8_t val) const
{
    sendMessage(addr::YEAR, val % 100);
}

void DS1302::startClock() const
{
    uint8_t secs = getMessage(addr::SECONDS);
    if (!(secs & 0x80))
    {
        secs |= 0x80;
        sendMessage(addr::SECONDS, secs);
    }
}
void DS1302::stopClock() const
{
    uint8_t secs = getMessage(addr::SECONDS);
    if ((secs & 0x80))
    {
        secs &= ~0x80;
        sendMessage(addr::SECONDS, secs);
    }
}

void DS1302::setHourMode(DS1302::Mode mode) const
{
    uint8_t hour = 0;
    if (mode == Mode::Hour12)
        hour = 0x80;
    sendMessage(addr::HOURS, hour);
}

uint8_t DS1302::getRam(uint8_t index) const
{
    return getMessage(addr::RAM + (index % 32) * 2);
}

void DS1302::setRam(uint8_t index, uint8_t value) const
{
    sendMessage(addr::RAM, value);
}