#include "grideye.hpp"

#define ENABLE_RGB_LEDMATRIX 0

#if ENABLE_RGB_LEDMATRIX

#define GREEN 0x1C
#define RED 0xE0
#define ORANGE 0xFC

#endif

Serial pcg(USBTX, USBRX);	//TODO delete this , and following printf, or make debug output

static uint8_t GridEYECenterI2C0values[64];
static uint8_t GridEYELeftI2C0values[64];
static uint8_t GridEYERightI2C1values[64];

void GridEYEInit(I2C *i2c0_obj, I2C *i2c1_obj) {
	i2c0_obj->frequency(400000);
	i2c1_obj->frequency(400000);
    I2C0_queue_create();
    I2C1_queue_create();
}

void GridEYETask(void const *args) {
	const i2c_sensor_t *temp=(const i2c_sensor_t *)args;
	I2C *i2c_obj = temp->i2c_obj;
	uint8_t i2c_addr = temp->i2c_addr;

	char cmd[2];

	union {
		char thermistor_echo[2];	//1 LSB = 0.0625 C , result 12-bit as signed absolute value
		uint16_t therm_echo_uint16;	//little endian
	};
	float thermistor_value;

	union {
		char temp_echo[2*PIXELS_COUNT];	//1 LSB = 0.25 C , result 12-bit as 2's complement
		uint16_t temp_echo_uint16[PIXELS_COUNT];	//little endian
	};
	float temp_values[PIXELS_COUNT];

#if ENABLE_RGB_LEDMATRIX

	DigitalOut SPI_ss(p8);	///Slave Select
	SPI_ss = 1;	//Make sure the RG matrix is deactivated, maybe this should be first line executed
	SPI RGB_LEDMatrix(p5, p6, p7); /// mosi, miso, sclk

#endif

	while (1) {
		Thread::signal_wait(GRIDEYE_I2C_SIGNAL);

		pcg.printf("GridEye\r\n");

		cmd[0] = GRIDEYE_I2C_THERM_ADDR;
		i2c_obj->write(i2c_addr, cmd, 1, true);	//Repeated start is true in i2c_obj->write, so it must be true in
		i2c_obj->read(i2c_addr, thermistor_echo, 2, true); //-> the following read, too.

		if (therm_echo_uint16 & 0x800) {  //if negative
			thermistor_value = - 0.0625 * (0x7FF & therm_echo_uint16);
		} else {	//else if positive
			thermistor_value = 0.0625 * (0x7FF & therm_echo_uint16);
		}

		pcg.printf("Termistor Temp = %f\r\n", thermistor_value);

		cmd[0] = GRIDEYE_I2C_TEMP_ADDR;
		i2c_obj->write(i2c_addr, cmd, 1, true);
		i2c_obj->read(i2c_addr, temp_echo, 2*PIXELS_COUNT, true);

		for (int i = 0; i < PIXELS_COUNT; ++i) {
			if (temp_echo_uint16[i] & 0x800) {  //if negative
				temp_values[i] = 0.25 * (int16_t)(0xF000 | temp_echo_uint16[i]);
			} else {	//else if positive
				temp_values[i] = 0.25 * (0x7FF & temp_echo_uint16[i]);
			}
		}

		pcg.printf("Grid Temp =\r\n");

#if ENABLE_RGB_LEDMATRIX
		
		if (i2c_addr == 0b1101001 << 1) {	//TODO Use the address of the GridEYE with the unique address
			char ledArray [64];
			int celsius;

			RGB_LEDMatrix.format(8,0);
			RGB_LEDMatrix.frequency(125000);

			//Determine LED Color for Pixel
			for (int pixel = 0; pixel < PIXELS_COUNT; ++pixel) {
				celsius = temp_values[pixel];
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
