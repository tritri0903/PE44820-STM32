#ifndef __PE43xx_H__
#define __PE43xx_H__


// Define types of sensors.
#define PE4302 02
// untested as I don't own these
#define PE4306 06
#define PE4312 12

class PE43xx
{
  public:
    PE43xx(GPIO_TypeDef* si_port, GPIO_InitTypeDef si_pin, GPIO_TypeDef* le_port, GPIO_InitTypeDef le_pin, GPIO_TypeDef* clk_port, GPIO_InitTypeDef clk_pin, uint8_t data = 0, uint8_t type = 0);

    void begin();

    // set attenuatator level. Range from 0 to 31
    // returns false if level was out of range or setting failed
    bool setLevel(float level);

    // get the current attenuator level
    float getLevel();

    // maximum level of attenuation. This varies between the chips
    // usually 31db or 31.5db
    float getMax();

    // min step. Varies between chips. Usually 1db or 0.5db
    float getStep();

  private:
    // type of the chip
    uint8_t _type;
    //GPIO
    GPIO_InitTypeDef _si_pin, _le_pin, _clk_pin;
    GPIO_TypeDef* _si_port;
    GPIO_TypeDef* _le_port;
    GPIO_TypeDef* _clk_port;

    // current attenuator level value
    float _level;

    // step and max value
    float _step;
    float _max;

    // write the level out to the chip
    void _writeLevel();
};

#endif /* __PE43xx_H__ */
