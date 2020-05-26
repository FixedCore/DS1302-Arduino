#include "DS1302.h"
DS1302::DS1302(uint8_t chipEnablePin, uint8_t dataPin, uint8_t clockPin)
    : chipEnablePin(chipEnablePin), dataPin(dataPin), clockPin(clockPin)
{
    pinMode(chipEnablePin, OUTPUT);
    pinMode(dataPin, OUTPUT);
    pinMode(clockPin, OUTPUT);
};

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
    pinMode(dataPin, OUTPUT);
    sendByte(addr);
    sendByte(val);
}
uint8_t DS1302::getMessage(uint8_t addr) const
{
    pinMode(dataPin, OUTPUT);
    sendByte(addr + 1);
    pinMode(dataPin, INPUT);
    return getByte();
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