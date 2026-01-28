#include "digital_io.h"

static inline uint32_t digitalPinToPad(uint8_t pin){
    if (pin >= (sizeof(digital_pin_to_pad) / sizeof(digital_pin_to_pad[0])))
        return NOT_A_PIN;
    return digital_pin_to_pad[pin];
}

static inline void setPadmuxToGPIO(uint32_t pad_index) {
    uint32_t reg_addr;
    uint32_t shift = (pad_index % 16) * 2;
    if (pad_index < 16) {
		reg_addr = PADMUX_0;
	}	else {
			reg_addr = PADMUX_1;
		}
    
    uint32_t reg_val = pulp_read32(reg_addr);
    reg_val &= ~(0b11 << shift);
    reg_val |= (0b01 << shift);
    pulp_write32(reg_addr, reg_val);
}

int digitalRead(int pin){
    isetPadmuxToGPIO(digitalPinToPad(pin));
	uint32_t padin = hal_gpio_get_value();
	int val = padin & (1 << pin);
	return val ? HIGH : LOW;
}

void digitalWrite(int pin, int value){
    setPadmuxToGPIO(digitalPinToPad(pin));

	if (val == LOW) {
		hal_gpio_set_pin_value(pin, 0);
	} else {
		hal_gpio_set_pin_value(pin, 1);
	}
}

void pinMode(int pin, uint8_t mode){
    uint32_t pad = digitalPinToPad(pin);
	if (pad == NOT_A_PIN) return;

	setPadmuxToGPIO(pad);

	hal_gpio_en_set(1<<pin);

	if (mode == OUTPUT) {
		hal_gpio_set_dir(1<<pin, 1);
	} else {
		hal_gpio_set_dir(1<<pin, 0);
	}
}
