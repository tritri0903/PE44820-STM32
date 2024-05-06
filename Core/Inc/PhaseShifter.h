/**
 * @file PhaseShifter.h
 * @author nguyenmanhthao996tn (manhthao.spm@gmail.com)
 * @brief A compact Arduino library to control digital phase shifter PE44820
 * @version 1.0.0
 * @date 2022-05-13
 *
 * @copyright Copyright (c) 2022
 *
 */

#ifndef __PHASE_SHIFTER_H__
#define __PHASE_SHIFTER_H__

/**
 * IMPORTANT: This option is calculated for ST P-NUCLEO-WB55 Board which have CPU clock speed at 64MHz by default (Arduino Core). If you are using different hardware or running at another CPU clock speed, disable this feature by uncommenting the #define USE_NOP_DELAY below OR recalculate the function _PS_delay_100ns();
 */
//#define USE_NOP_DELAY

typedef enum
{
  MODE_BINARY_WEIGHTED = 0,
  MODE_OPT_1700_2200,
  MODE_OPT_2600
} mode_e;

typedef union
{
  struct
  {
    uint8_t data;
    uint8_t opt : 1;
    uint8_t RFU : 7;
  } data;
  uint16_t raw;
} data_u;

class PhaseShifter
{
private:
  data_u _data;
  uint8_t _address;
  mode_e _mode;

  GPIO_InitTypeDef _si_pin, _le_pin, _clk_pin;
  GPIO_TypeDef* _si_port;
  GPIO_TypeDef* _le_port;
  GPIO_TypeDef* _clk_port;

  /**
   * @brief Delay ~109nS function with nop function at F_CPU=64MHz
   *
   * @param val The delay circle(s). Delay time = 109.375nS * val.
   */
  inline void _PS_delay_100ns(uint32_t val)
  {
#ifdef USE_NOP_DELAY
    while (val-- > 0)
    {
      __asm__("nop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\t");
    }
#else
    HAL_Delay(val);
#endif
  }

public:
  PhaseShifter(void);
  PhaseShifter(GPIO_TypeDef* si_port, GPIO_InitTypeDef si_pin, GPIO_TypeDef* le_port, GPIO_InitTypeDef le_pin, GPIO_TypeDef* clk_port, GPIO_InitTypeDef clk_pin, uint8_t address = 0, mode_e mode = MODE_BINARY_WEIGHTED);

  void begin(void);
  void dataShiftOut(uint8_t addr, data_u data);
  uint16_t findAngle(float angle, mode_e mode, float *foundAngle = NULL);

  /**
   * @brief Find the nearest available shift angle & Set the phase shifter angle
   *
   * @param angle Target angle in degree (0 - 360)
   * @return float Found angle in degree
   */
  float setAngle(float angle);

  void setData(data_u data);
  data_u getData(void);
  void setAddress(uint8_t address);
  uint8_t getAddress(void);
  void setMode(mode_e mode);
  mode_e getMode(void);
  void setPins(GPIO_InitTypeDef si_pin, GPIO_InitTypeDef le_pin, GPIO_InitTypeDef clk_pin);
  void getPins(GPIO_InitTypeDef *si_pin, GPIO_InitTypeDef *le_pin, GPIO_InitTypeDef *clk_pin);
};

#endif /* __PHASE_SHIFTER_H__ */
