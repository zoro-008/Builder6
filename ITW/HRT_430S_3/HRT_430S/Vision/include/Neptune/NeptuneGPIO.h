
#pragma once

typedef enum _tagENeptuneGPIO {
	NEPTUNE_GPIO_LINE0 = 0,		// GPIO 0
	NEPTUNE_GPIO_LINE1			// GPIO 1
} ENeptuneGPIO;


typedef enum _tagENeptuneGPIOSource {
	NEPTUNE_GPIO_SOURCE_STROBE = 0,	// strobe
	NEPTUNE_GPIO_SOURCE_USER		// user defined
} ENeptuneGPIOSource;


typedef enum _tagENeptuneGPIOValue {
	NEPTUNE_GPIO_VALUE_LOW = 0,		// low level
	NEPTUNE_GPIO_VALUE_HIGH			// high level
} ENeptuneGPIOValue;


typedef struct _tagNEPTUNE_GPIO {
	ENeptuneGPIO			Gpio;		// GPIO index
	ENeptuneGPIOSource		Source;		// GPIO source
	ENeptuneGPIOValue		Value;		// GPIO value

	_tagNEPTUNE_GPIO()
	{
		Gpio = NEPTUNE_GPIO_LINE1;
		Source = NEPTUNE_GPIO_SOURCE_STROBE;
		Value = NEPTUNE_GPIO_VALUE_LOW;
	}
} NEPTUNE_GPIO, *PNEPTUNE_GPIO;


