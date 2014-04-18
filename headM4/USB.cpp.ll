#include "USB.hpp"

//I copied USBSerial.h to USBSerial2.h. Only changed the constructor so we can
//input the circular buffer size (buf). It would be a good idea for the buffer
//size to be a multiple of the incoming packet size. Haven't tested for packets
//bigger than 64 byte.
//The circular buffer fills up transparently with a callback, so we don't have
//to worry about missing packets.
USBSerial serial(40);

Serial pcu(USBTX, USBRX);

uint8_t buf[21] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

uint8_t ww = 0;

void flip()
{
    led1 = !led1;
    //serial.scanf("%s", buf);  //no IO allowed
    //printf("%s\r\n", buf);  //no IO allowed

//    t2.reset();
//    t2.start();
//    if (serial.available() && ww<20) {    //doesn't check array limits !
//        buf[ww] = serial._getc();
//        ww++;
//    }
//    else if (serial.available() && ww >= 20) {
//        ww=0;
//        for (int i=0; i<20; i++)
//           buf[i] = 0; //fills the array with '\0' (clears previous values and is end of C string)
//        buf[ww] = serial._getc();
//        ww++;
//    }
//    t2.stop();
    led2 = !led2;
}


int main(void)
{
//    uint8_t buf[4];

    while(1) {
//        serial.scanf("%s", buf);
        pcu.printf("I am a virtual serial port\r\n");

        uint8_t rep[64];
        uint8_t rep2[64];
        uint8_t rep3[64];
        uint8_t rep4[63];
        //Fill the report
        for (int m=0; m<62 ; m++)
            rep[m]=(uint8_t)(m+33);

        for (int m=0; m<62 ; m++)
            rep2[m]=(uint8_t)(m+34);
        for (int m=0; m<62 ; m++)
            rep3[m]=(uint8_t)(m+35);
        for (int m=0; m<61 ; m++)
            rep4[m]=(uint8_t)(m+36);

        rep[62]='\r';
        rep2[62]='\r';
        rep3[62]='\r';
        rep4[61]='\r';
        rep[63]='\n';
        rep2[63]='\n';
        rep3[63]='\n';
        rep4[62]='\n';

        wait_us(500000);

//        serial.attach(&flip);

        for (int i=0; i<20;i++)
            buf[i] = 0; //fills the array with '\0' (clears previous values and is end of C string)
        int w=0;

        while (serial.available() && w<20) {  //C++ doesn't check array limits !
            buf[w] = serial._getc();
            w++;
        }
        buf[w] = 0; //Xreiazetai, mporei na epikalyf8oun ta epomena printf

        printf("%d",w);
        printf("String taken was: %s \r\n", buf);   //prints a c-string

        //wait(3);
        printf("meh");
        //writeBlock() waits for the host to connect or until buffer is full
        if (serial.writeBlock(rep, 64) == 0) printf("sflama\r\n");
//        wait_us(1000);
        if (serial.writeBlock(rep2, 64) == 0) printf("sflama\r\n");
//        wait_us(1000);
        if (serial.writeBlock(rep3, 64) == 0) printf("sflama\r\n");
//        wait_us(1000);
        if (serial.writeBlock(rep4, 63) == 0) printf("sflama\r\n");
//        wait_us(1000);

    }
}
