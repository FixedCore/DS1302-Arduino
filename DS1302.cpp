#include "DS1302.h"

int8_t DS1302::setValue(uint8_t field, uint8_t value)
{
    if (field < SECONDS || field > YEAR || field % 2 != 0)
        return -1; // invalid field

    uint8_t valueToSend = 0;
    switch (field)
    {
    case SECONDS:
        if (value > 59)
            return -2; // invalid value
        uint8_t tmp = getByte(field+1);
        if(tmp > 200) return -3; // Error in communication
        valueToSend = toBCD(value) | (tmp & 0x80);
        break;

    case MINUTES:
        if(value > 59)
            return -2; // invalid value
        valueToSend = toBCD(value);
        break;

    case HOURS:
        if(value > 23)
            return -2; // invalid value
        uint8_t tmp = getByte(field+1);
        if(tmp > 200) return -3; // Error in communication
        valueToSend = toBCD(value) | (tmp & 0xe0);
        break;

    case DATE:
        if(value < 1 || value > 31) // TODO: better date checking
            return -2; // invalid value
        valueToSend = toBCD(value);
        break;

    case MONTH:
        if(value < 1 || value > 12) 
            return -2; // invalid value
        valueToSend = toBCD(value);
        break;
    case WEEKDAY:
        if(value < 1 || value > 7) 
            return -2; // invalid value
        valueToSend = toBCD(value);
        break;

    case YEAR:
        if(value > 99) 
            return -2; // invalid value
        valueToSend = toBCD(value);
        break;
    default:
        break;
    }
    uint8_t result = setByte(field, valueToSend);
    if(result > 200)
        return -3; // error in communication
    else return 1;
}

uint8_t DS1302::toBCD(uint8_t val)
{
    return ((val / 10) << 4) | (val % 10);
}
uint8_t DS1302::fromBCD(uint8_t val)
{
    return ((BCDHI(val) >> 4) * 10) + BCDLO(val);
}

uint8_t DS1302::setByte(uint8_t field, uint8_t value)
{

}

int8_t DS1302::getByte(uint8_t field)
{

}