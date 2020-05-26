#include "DS1302.h"
DS1302::DS1302(uint8_t chipEnablePin, uint8_t dataPin, uint8_t clockPin)
    : chipEnablePin(chipEnablePin), dataPin(dataPin), clockPin(clockPin)
{
    pinMode(chipEnablePin, OUTPUT);
    pinMode(dataPin, OUTPUT);
    pinMode(clockPin, OUTPUT);
};

static uint8_t toBCD(uint8_t val) const
{
    return ((val / 10) << 4 | (val % 10));
}

static uint8_t fromBCD(uint8_t val) const
{
    return (BCDHI(val) >> 4) * 10 + BCDLO(val);
}

void sendByte(uint8_t val) const
{
    for (uint8_t i = 0; i < 8; i++)
    {
        digitalWrite(dataPin, (val >> i) & 1);
        digitalWrite(clockPin, 1);
        delayMicroseconds(1);
        digitalWrite(clockPin, 0);
    }
}
uint8_t getByte() const
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

uint8_t DS1302::getSeconds() const;
uint8_t DS1302::getMinutes() const;
uint8_t DS1302::getHours() const;
uint8_t DS1302::getMonthDay() const;
uint8_t DS1302::getMonth() const;
uint8_t DS1302::getWeekDay() const;
uint8_t DS1302::getYear() const;