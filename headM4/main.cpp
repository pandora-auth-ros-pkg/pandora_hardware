/** @file
 * @author Orestis Zachariadis
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

/** @brief Program entry point
 *
 * Initializes and starts tasks.
 */

SPI spi(p5, p6, p7); // mosi, miso, sclk
DigitalOut cs(p8);

#define ARRAY_SIZE(a) (sizeof(a) / sizeof((a)[0]))
#define VOSPI_FRAME_SIZE (164)
uint8_t lepton_frame_packet[VOSPI_FRAME_SIZE];
static unsigned int lepton_image[80][80];


int transfer(int fd)
{
    int ret;
    int i;
    int frame_number;
    uint8_t tx[VOSPI_FRAME_SIZE] = {0 };


//    ret = ioctl(fd, SPI_IOC_MESSAGE(1), &tr);
//    if (ret < 1)
//        pabort("can't send spi message");

    if(((lepton_frame_packet[0]&0xf) != 0x0f))
    {
        frame_number = lepton_frame_packet[1];

        if(frame_number < 60 )
        {
            for(i=0;i<80;i++)
            {
                lepton_image[frame_number][i] = (lepton_frame_packet[2*i+4] << 8 | lepton_frame_packet[2*i+5]);
            }
        }
    }
    return frame_number;
}

int main(void) {
    // Chip must be deselected
        cs = 1;

        // Setup the spi for 8 bit data, high steady state clock,
        // second edge capture, with a 1MHz clock rate
        spi.format(8,3);
        spi.frequency(16000000);

        // Select the device by seting chip select low
        cs = 0;

        // Send a dummy byte to receive the contents of the WHOAMI register
        int whoami = spi.write(0x00);
        printf("WHOAMI register = 0x%X\n", whoami);

        // Deselect the device
        cs = 1;
}
