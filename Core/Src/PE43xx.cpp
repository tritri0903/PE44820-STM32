/*
 * MIT License
//
//Copyright (c) 2019 Tatu Wikman
//
//Permission is hereby granted, free of charge, to any person obtaining a copy
//of this software and associated documentation files (the "Software"), to deal
//in the Software without restriction, including without limitation the rights
//to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//copies of the Software, and to permit persons to whom the Software is
//furnished to do so, subject to the following conditions:
//
//The above copyright notice and this permission notice shall be included in all
//copies or substantial portions of the Software.
//
//THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
//SOFTWARE.
 *
 *
 *
 */




#include "main.h"
#include "PE43xx.h"


PE43xx::PE43xx(GPIO_TypeDef* si_port, GPIO_InitTypeDef si_pin, GPIO_TypeDef* le_port, GPIO_InitTypeDef le_pin, GPIO_TypeDef* clk_port, GPIO_InitTypeDef clk_pin, uint8_t data, uint8_t type)
{

	_si_pin = si_pin;
	_le_pin = le_pin;
	_clk_pin = clk_pin;

	_si_port = si_port;
	_le_port = le_port;
	_clk_port = clk_port;

	begin();
}


void PE43xx::begin() {
    // set pins
	HAL_GPIO_WritePin(_si_port, _si_pin.Pin, GPIO_PIN_RESET);

    // set our internal max and step values
    switch (_type) {
        case PE4302:
        case PE4312:
            _max = 31.5;
            _step = 0.5;
            break;
        case PE4306:
            _max = 30;
            _step = 1;
            break;
        default:
            _step = 1;
            _max = 30;
            break;
    }

    // set level to 0
    setLevel(0);
}

bool PE43xx::setLevel(float level) {
    // bounds check
    if(level < 0 || level > getMax() ) {
        return false;
    }

    _level = level;
    _writeLevel();

    return true;
}

void PE43xx::_writeLevel() {

    // PE4302 and PE4312 wants something like this
    //
    // C16  C8  C4  C2  C1  C0.5  =>  result
    //  0    0   0   0   0    0   =>  0db
    //  0    0   0   0   0    1   =>  0.5db
    //  0    0   1   1   0    1   =>  6.5db
    //  1    1   1   1   1    1   => 31.5db

    // for PE4306 (which has only 1db resolution)
    //
    // C16  C8  C4  C2  C1  =>  result
    //  0    0   0   0   0  =>  0db
    //  0    0   0   0   1  =>  1db
    //  0    1   0   0   1  =>  9db
    //  1    1   1   1   1  => 31db

    // so the big different is that one can set the .5 bit or not

    // get the integer part, it will be the same for both chips
    int intlevel = int(_level);
    // default to 6 bits
    int bits = 6;

    // if our step is less than 1 the chip supports the 0.5 decimal
    /*
    if(_step < 1) {
        // one more bit
        bits = 6;

        // if the decimal is not 0 shift 1 into our level
        if( (_level*10 - intlevel*10) > 0) {
            intlevel = intlevel << 1;
            bitWrite(intlevel, 0, 1);
        } else {
            intlevel = intlevel << 1;
            bitWrite(intlevel, 0, 0);
        }
    }*/

    // LE and CLOCK down to get the chip listen
    HAL_GPIO_WritePin(_le_port, _le_pin.Pin, GPIO_PIN_RESET);
    HAL_Delay(3);

    // Write the level out MSB first
    int b;
    for (int bit = bits; bit >= 0; bit--) {
        b = ((intlevel << 1) >> bit) & 0x01;

        HAL_GPIO_WritePin(_clk_port, _clk_pin.Pin, GPIO_PIN_SET);
        HAL_GPIO_WritePin(_si_port, _si_pin.Pin, (b != 0) ? GPIO_PIN_SET : GPIO_PIN_RESET);

        HAL_Delay(1);

        HAL_GPIO_WritePin(_clk_port, _clk_pin.Pin, GPIO_PIN_RESET);

        HAL_Delay(1);
    }

    // toggle LE to latch
    HAL_GPIO_WritePin(_le_port, _le_pin.Pin, GPIO_PIN_SET);
}

// getters
float PE43xx::getLevel() {
    return _level;
}

float PE43xx::getMax() {
    return _max;
}

float PE43xx::getStep() {
    return _step;
}
