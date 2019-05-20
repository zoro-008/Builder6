
#pragma once

enum ENeptuneGPIO {
	NEPTUNE_GPIO_LINE0 = 0,		// GPIO 0
	NEPTUNE_GPIO_LINE1			// GPIO 1
};


enum ENeptuneGPIOSource {
	NEPTUNE_GPIO_SOURCE_STROBE = 0,	// strobe
	NEPTUNE_GPIO_SOURCE_USER		// user defined
};


enum ENeptuneGPIOValue {
	NEPTUNE_GPIO_VALUE_LOW = 0,		// low level
	NEPTUNE_GPIO_VALUE_HIGH			// high level
};


struct NEPTUNE_GPIO {
	ENeptuneGPIO			Gpio;		// GPIO index
	ENeptuneGPIOSource		Source;		// GPIO source
	ENeptuneGPIOValue		Value;		// GPIO value

	NEPTUNE_GPIO()
	{
		Gpio = NEPTUNE_GPIO_LINE1;
		Source = NEPTUNE_GPIO_SOURCE_STROBE;
		Value = NEPTUNE_GPIO_VALUE_LOW;
	}
};
typedef NEPTUNE_GPIO*		PNEPTUNE_GPIO;


