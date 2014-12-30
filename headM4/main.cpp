/** @file
 * @author Taras Nikos
 * @brief Initializes and starts tasks.
 */
#include "mbed.h"
#include "rtos.h"
#include "statistics.hpp"
#include "grideye.hpp"
#include "CO2.hpp"
#include "USB.hpp"
#include "health.hpp"
#include "conf.h"
//#include "gpdma.h"
//#include "dsp.h"


// Data[0] STATUS
// Data[1] IR
// Data[2] HI DISTANCE (0 or 1)
// Data[3] LO DISTANCE
// Sensor sends distance in centimeters


const int sonar_addr = 0x80;
#define N 4  //number of i2c reading bits

I2C i2c(p9, p10);  // SDA in pin9, SCL in pin10

Serial pc(USBTX, USBRX);

void read_sonar(char* data)
{
  /*
   * Request a reading from the sonar using I2C protocol.
   * data[0] -> status
   * data[1]:data[2] -> distance reading of sonar
   * data[3] -> IR reading
   * */

  char cmd_request = 0x80; // command to send to the sonar to request a distance reading
  char cmd_read = 0x40;    // command to send to the sonar to receive the reading
  int d;

  d = i2c.write(sonar_addr, &cmd_request, 1);   // request a distance reading from the sonar
  //printf("1=%d\n\r",d);
  wait_ms(100);                              // wait 50ms for the sonar to calculate the distance
  d = i2c.write(sonar_addr, &cmd_read, 1);      // request to receive the reading from the sonar
  //printf("2=%d\n\r",d);
  wait_ms(50);
  d=i2c.read(sonar_addr, data, N);            // receive the reading and store it in the "data" array
  //printf("3=%d\n\r",d);

}

void printData(char* data)
{
    int j;
    for (j=0;j<N;j++){
        pc.printf("\r\nData[%d]: %u",j,data[j]);
    }
}

int main(void)
{

  unsigned short dist_reading=0;

  char* data;

  data = (char*)malloc(N * sizeof(char)); //allocate memory for "data" array

  while (true){

    read_sonar(data);
    printData(data);

    dist_reading = (data[2]<<8) + data[3];

    pc.printf("\r\nDistance reading: %u",dist_reading);
    wait(1); // wait 1sec
  }

}
