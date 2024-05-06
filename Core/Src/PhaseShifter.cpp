/**
 * @file PhaseShifter.cpp
 * @author nguyenmanhthao996tn (manhthao.spm@gmail.com)
 * @brief A compact Arduino library to control digital phase shifter PE44820
 * @version 1.0.0
 * @date 2022-05-13
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include "main.h"
#include "PhaseShifter.h"
#include "mapping.h"

#define abs(x) ((x) > 0 ? (x) : -(x))

PhaseShifter::PhaseShifter(GPIO_TypeDef* si_port, GPIO_InitTypeDef si_pin, GPIO_TypeDef* le_port, GPIO_InitTypeDef le_pin, GPIO_TypeDef* clk_port, GPIO_InitTypeDef clk_pin, uint8_t address, mode_e mode)
{
  this->_data.raw = 0;
  this->_address = address;
  this->_mode = mode;

  this->_si_pin = si_pin;
  this->_le_pin = le_pin;
  this->_clk_pin = clk_pin;

  this->_si_port = si_port;
  this->_le_port = le_port;
  this->_clk_port = clk_port;

  begin();
}

void PhaseShifter::begin(void)
{
  /* GPIOs Initialize */
	//HAL_GPIO_Init(&this->_si_port, &this->_si_pin);
	HAL_GPIO_WritePin(this->_si_port, this->_si_pin.Pin, GPIO_PIN_RESET);

	//HAL_GPIO_Init(this->_le_port, this->_le_pin);
	HAL_GPIO_WritePin(this->_le_port, this->_le_pin.Pin, GPIO_PIN_RESET);

	//HAL_GPIO_Init(this->_clk_port,this->_clk_pin);
	HAL_GPIO_WritePin(this->_clk_port, this->_clk_pin.Pin, GPIO_PIN_SET);
}

void PhaseShifter::dataShiftOut(uint8_t addr, data_u data)
{
  int8_t bit_index;

  HAL_GPIO_WritePin(this->_le_port, this->_le_pin.Pin, GPIO_PIN_RESET);
  _PS_delay_100ns(1); // Tsettle > 10ns

  // Data
  for (bit_index = 0; bit_index < 8; bit_index++)
  {
	  HAL_GPIO_WritePin(this->_clk_port, this->_clk_pin.Pin, GPIO_PIN_SET);
	  HAL_GPIO_WritePin(this->_si_port, this->_si_pin.Pin,(((data.data.data >> bit_index) & 0x01) != 0) ? GPIO_PIN_SET : GPIO_PIN_RESET);

    _PS_delay_100ns(3); // Tclkh > 30ns

	HAL_GPIO_WritePin(this->_clk_port, this->_clk_pin.Pin, GPIO_PIN_RESET);
    _PS_delay_100ns(3); // Tclkl > 30ns
  }

  // OPT
  HAL_GPIO_WritePin(this->_clk_port, this->_clk_pin.Pin, GPIO_PIN_SET);
  HAL_GPIO_WritePin(this->_si_port, this->_si_pin.Pin, (data.data.opt != 0) ? GPIO_PIN_SET : GPIO_PIN_RESET);
  _PS_delay_100ns(3); // Tclkh > 30ns

  HAL_GPIO_WritePin(this->_clk_port, this->_clk_pin.Pin, GPIO_PIN_RESET);
  _PS_delay_100ns(3); // Tclkl > 30ns

  // Address
  for (bit_index = 0; bit_index < 4; bit_index++)
  {
	  HAL_GPIO_WritePin(this->_clk_port, this->_clk_pin.Pin, GPIO_PIN_SET);
	  HAL_GPIO_WritePin(this->_si_port, this->_si_pin.Pin, (((addr >> bit_index) & 0x01) != 0) ? GPIO_PIN_SET : GPIO_PIN_RESET);
    _PS_delay_100ns(3); // Tclkh > 30ns

    HAL_GPIO_WritePin(this->_clk_port, this->_clk_pin.Pin, GPIO_PIN_RESET);
    _PS_delay_100ns(3); // Tclkl > 30ns
  }

  // Reset SI & SLK Pin
  HAL_GPIO_WritePin(this->_si_port, this->_si_pin.Pin, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(this->_clk_port, this->_clk_pin.Pin, GPIO_PIN_SET);

  _PS_delay_100ns(1); // Tsettle > 10ns
  HAL_GPIO_WritePin(this->_le_port, this->_le_pin.Pin, GPIO_PIN_SET);
}

uint16_t PhaseShifter::findAngle(float angle, mode_e mode, float *foundAngle)
{
  uint16_t idx = 0;
  int16_t targetAngle = angle * 10;

  for (idx = 0; idx <= 255; idx++)
  {
    if (targetAngle <= PHASE_SHIFT_VALUE[idx])
    {
      break;
    }
  }

  if ((idx != 0) && (idx != 255))
  {
    uint16_t value1 = abs(targetAngle - PHASE_SHIFT_VALUE[idx - 1]);
    uint16_t value2 = abs(targetAngle - PHASE_SHIFT_VALUE[idx]);

    if (value1 < value2)
    {
      idx -= 1;
    }
  }
  if (foundAngle != NULL)
  {
    (*foundAngle) = PHASE_SHIFT_VALUE[idx] / 10.0;
  }

  uint16_t retVal = 0;
  switch (mode)
  {
  case MODE_BINARY_WEIGHTED:
    retVal = BINARY_WEIGHTED_DATA[idx];
    break;
  case MODE_OPT_1700_2200:
    retVal = OPT_1700_2200_DATA[idx];
    break;
  case MODE_OPT_2600:
    retVal = OPT_2600_DATA[idx];
    break;
  default:
    retVal = 0x0000;
    break;
  }

  return retVal;
}

float PhaseShifter::setAngle(float angle)
{
  float foundAngle = 0;

  if (angle < 0)
    angle = 0;
  if (angle > 360)
    angle = 360;

  this->_data.raw = findAngle(angle, this->_mode, &foundAngle);
  this->dataShiftOut(this->_address, this->_data);

  return foundAngle;
}

void PhaseShifter::setData(data_u data)
{
  this->_data.raw = data.raw;
}

data_u PhaseShifter::getData(void)
{
  return (this->_data);
}

void PhaseShifter::setAddress(uint8_t address)
{
  this->_address = address & 0x0F;
}

uint8_t PhaseShifter::getAddress(void)
{
  return (this->_address);
}

void PhaseShifter::setMode(mode_e mode)
{
  if ((mode != MODE_BINARY_WEIGHTED) && (mode != MODE_OPT_1700_2200) && (mode != MODE_OPT_2600))
  {
    return; // Invalid mode parameter
  }

  this->_mode = mode;
}

mode_e PhaseShifter::getMode(void)
{
  return (this->_mode);
}

void PhaseShifter::setPins(GPIO_InitTypeDef si_pin, GPIO_InitTypeDef le_pin, GPIO_InitTypeDef clk_pin)
{
  this->_si_pin = si_pin;
  this->_le_pin = le_pin;
  this->_clk_pin = clk_pin;
}

void PhaseShifter::getPins(GPIO_InitTypeDef *si_pin, GPIO_InitTypeDef *le_pin, GPIO_InitTypeDef *clk_pin)
{
  if (si_pin != NULL)
  {
    (*si_pin) = this->_si_pin;
  }

  if (le_pin != NULL)
  {
    (*le_pin) = this->_le_pin;
  }

  if (clk_pin != NULL)
  {
    (*clk_pin) = this->_clk_pin;
  }
}
