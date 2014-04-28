#include "grideye.hpp"

static Mutex i2c0_mutex;
static Mutex i2c1_mutex;

static Thread *tGridEYECenter;
static Thread *tGridEYELeft;
static Thread *tGridEYERight;

static Thread *tGridEYEHealth;

void GridEYEInit(I2C *i2c0_obj, I2C *i2c1_obj) {
	i2c0_obj->frequency(400000);
	i2c1_obj->frequency(400000);
    I2C0_queue_create();
    I2C1_queue_create();

	i2c_sensor_t temp_sens1;	//If we pass starting arguments to threads we must be sure that their memory location
	i2c_sensor_t temp_sens2;	//-> doesn't change long enough for the threads to be created. So we create a temp
	i2c_sensor_t temp_sens3;	//-> variable for each thread.

	temp_sens1.i2c_obj = i2c0_obj;
	temp_sens1.i2c_periph_num = 0;
	temp_sens1.i2c_addr = GRIDEYE_I2C_ADDR_GND;
	temp_sens1.grideye_num = GEYE_CENTER;
	tGridEYECenter = new Thread(GridEYETask, (void *)&temp_sens1);

    temp_sens2.i2c_obj = i2c0_obj;
    temp_sens2.i2c_periph_num = 0;
    temp_sens2.i2c_addr = GRIDEYE_I2C_ADDR_VDD;
    temp_sens2.grideye_num = GEYE_LEFT;
    tGridEYELeft = new Thread(GridEYETask, (void *)&temp_sens2);

    temp_sens3.i2c_obj = i2c1_obj;
    temp_sens3.i2c_periph_num = 1;
    temp_sens3.i2c_addr = GRIDEYE_I2C_ADDR_GND;
    temp_sens3.grideye_num = GEYE_RIGHT;
    tGridEYERight = new Thread(GridEYETask, (void *)&temp_sens3);

    tGridEYEHealth = new Thread(GridEYEHealthTask);
}

void GridEYETaskCaller(void const *args) {
	//I2C sensors in the same I2C bus have maximum distance ie 50ms in a 100ms loop
    while (true) {
    	clearHealthyGridEYE();

    	tGridEYECenter->signal_set(GRIDEYE_I2C_SIGNAL);

		Thread::wait(25);
		tGridEYERight->signal_set(GRIDEYE_I2C_SIGNAL);

		Thread::wait(25);
		tGridEYELeft->signal_set(GRIDEYE_I2C_SIGNAL);

		Thread::wait(40);
		tGridEYEHealth->signal_set(HEALTH_SIGNAL);

		Thread::wait(10);
    }
}

void GridEYETask(void const *args) {
	const i2c_sensor_t *temp=(const i2c_sensor_t *)args;
	I2C *i2c_obj = temp->i2c_obj;
	uint8_t i2c_periph_num = temp->i2c_periph_num;
	uint8_t i2c_addr = temp->i2c_addr;
	uint8_t grideye_num = temp->grideye_num;

	char cmd[2];

	union {
		char thermistor_echo[2];	//1 LSB = 0.0625 C , result 12-bit as signed absolute value
		uint16_t therm_echo_uint16;	//little endian
	};
	float thermistor_value;

	union {
		char temper_echo[2*PIXELS_COUNT];	//1 LSB = 0.25 C , result 12-bit as 2's complement
		uint16_t temper_echo_uint16[PIXELS_COUNT];	//little endian
	};
	float temper_values[PIXELS_COUNT];

#if ENABLE_RGB_LEDMATRIX

	DigitalOut SPI_ss(p8);	///Slave Select
	SPI_ss = 1;	//Make sure the RG matrix is deactivated, maybe this should be first line executed
	SPI RGB_LEDMatrix(p5, p6, p7); /// mosi, miso, sclk

#endif

	while (1) {
		Thread::signal_wait(GRIDEYE_I2C_SIGNAL);

		cmd[0] = GRIDEYE_I2C_THERM_ADDR;
		i2c_lock(i2c_periph_num);
		i2c_obj->write(i2c_addr, cmd, 1, true);	//Repeated start is true in i2c_obj->write, so it must be true in
		i2c_obj->read(i2c_addr, thermistor_echo, 2, true); //-> the following read, too.
		i2c_unlock(i2c_periph_num);

		if (therm_echo_uint16 & 0x800) {  //if negative
			thermistor_value = - 0.0625 * (0x7FF & therm_echo_uint16);
		} else {	//else if positive
			thermistor_value = 0.0625 * (0x7FF & therm_echo_uint16);
		}

		cmd[0] = GRIDEYE_I2C_TEMP_ADDR;
		i2c_lock(i2c_periph_num);
		i2c_obj->write(i2c_addr, cmd, 1, true);
		i2c_obj->read(i2c_addr, temper_echo, 2*PIXELS_COUNT, true);
		i2c_unlock(i2c_periph_num);

		for (int i = 0; i < PIXELS_COUNT; ++i) {
			if (temper_echo_uint16[i] & 0x800) {  //if negative
				temper_values[i] = 0.25 * (int16_t)(0xF000 | temper_echo_uint16[i]);
			} else {	//else if positive
				temper_values[i] = 0.25 * (0x7FF & temper_echo_uint16[i]);
			}
		}

		GridEYEvaluesSet(temper_values, grideye_num);


#if ENABLE_RGB_LEDMATRIX
		
		if (grideye_num == GEYE_CENTER) {
			char ledArray [64];
			int celsius;

			RGB_LEDMatrix.format(8,0);
			RGB_LEDMatrix.frequency(125000);

			//Determine LED Color for Pixel
			for (int pixel = 0; pixel < PIXELS_COUNT; ++pixel) {
				celsius = temper_values[pixel];
				if (celsius < 26) {
					ledArray[pixel] = GREEN;
				} else if (celsius >= 26 && celsius <= 30) {
					ledArray[pixel] = ORANGE;
				} else {
					ledArray[pixel] = RED;
				}
			}

			//Transfer LED Data
			SPI_ss = 0;
			wait_us(500);
			RGB_LEDMatrix.write(0x26);	//Resets RGBMatrixBackpack index. (see sparkfun's github). It shouldn't be needed
										//-> but it doesn't work without it...
			for (int pixel = 0; pixel < PIXELS_COUNT; ++pixel) {
				RGB_LEDMatrix.write(ledArray[pixel]);
			}
			wait_us(500);
			SPI_ss = 1;
		}

#endif
	}
}

void GridEYEvaluesSet(float values[], uint8_t grideye_num) {
	uint8_t GridEYEvalues[PIXELS_COUNT];
	uint8_t OutOfBounds = 0;

	for (int i = 0; i < PIXELS_COUNT; ++i) {
		if (values[i] >= 0 && values[i] < 80) {
			GridEYEvalues[i] = (uint8_t)(values[i] + 0.5);	//rounding to nearest Celsius degree
		} else {
			OutOfBounds =1;
		}
	}

	if (!OutOfBounds) {
		HealthyGridEYEvaluesSet(GridEYEvalues, grideye_num);
	}
}

void i2c_lock(uint8_t i2c_periph_num) {
	switch (i2c_periph_num) {
		case 0:
			i2c0_mutex.lock();
			break;
		case 1:
			i2c1_mutex.lock();
			break;
	}
}

void i2c_unlock(uint8_t i2c_periph_num) {
	switch (i2c_periph_num) {
		case 0:
			i2c0_mutex.unlock();
			break;
		case 1:
			i2c1_mutex.unlock();
			break;
	}
}
