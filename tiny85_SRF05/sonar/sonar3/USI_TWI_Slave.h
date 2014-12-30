

#define USI_TWI_SLAVE_


void          USI_TWI_Slave_Initialise( unsigned char );
void          USI_TWI_Transmit_Byte( unsigned char );
unsigned char USI_TWI_Receive_Byte( void );
unsigned char USI_TWI_Data_In_Receive_Buffer( void );
void          Timer_Init(void);

void USI_Start_Condition_ISR(void);
void USI_Counter_Overflow_ISR(void);

#define TRUE                1
#define FALSE               0

typedef     unsigned char       uint8_t;


#define TWI_RX_BUFFER_SIZE  (16)
#define TWI_RX_BUFFER_MASK ( TWI_RX_BUFFER_SIZE - 1 )

#if ( TWI_RX_BUFFER_SIZE & TWI_RX_BUFFER_MASK )
        #error TWI RX buffer size is not a power of 2
#endif

// 1,2,4,8,16,32,64,128 or 256 bytes are allowed buffer sizes

#define TWI_TX_BUFFER_SIZE  (16)
#define TWI_TX_BUFFER_MASK ( TWI_TX_BUFFER_SIZE - 1 )

#if ( TWI_TX_BUFFER_SIZE & TWI_TX_BUFFER_MASK )
        #error TWI TX buffer size is not a power of 2
#endif



#define USI_SLAVE_CHECK_ADDRESS                (0x00)
#define USI_SLAVE_SEND_DATA                    (0x01)
#define USI_SLAVE_REQUEST_REPLY_FROM_SEND_DATA (0x02)
#define USI_SLAVE_CHECK_REPLY_FROM_SEND_DATA   (0x03)
#define USI_SLAVE_REQUEST_DATA                 (0x04)
#define USI_SLAVE_GET_DATA_AND_SEND_ACK        (0x05)


    #define DDR_USI             DDRB
    #define PORT_USI            PORTB
    #define PIN_USI             PINB
    #define PORT_USI_SDA        PB0
    #define PORT_USI_SCL        PB2
    #define PIN_USI_SDA         PINB0
    #define PIN_USI_SCL         PINB2
    #define USI_START_COND_INT  USISIF
    #define USI_START_VECTOR    USI_START_vect
    #define USI_OVERFLOW_VECTOR USI_OVF_vect


//! Functions implemented as macros
#define SET_USI_TO_SEND_ACK()                                                                                 \
{                                                                                                             \
        USIDR    =  0;                                              /* Prepare ACK                         */ \
        DDR_USI |=  (1<<PORT_USI_SDA);                              /* Set SDA as output                   */ \
        USISR    =  (0<<USI_START_COND_INT)|(1<<USIOIF)|(1<<USIPF)|(1<<USIDC)|  /* Clear all flags, except Start Cond  */ \
                    (0x0E<<USICNT0);                                /* set USI counter to shift 1 bit. */ \
}

#define SET_USI_TO_READ_ACK()                                                                                 \
{                                                                                                             \
        DDR_USI &=  ~(1<<PORT_USI_SDA);                             /* Set SDA as intput */                   \
        USIDR    =  0;                                              /* Prepare ACK        */                  \
        USISR    =  (0<<USI_START_COND_INT)|(1<<USIOIF)|(1<<USIPF)|(1<<USIDC)|  /* Clear all flags, except Start Cond  */ \
                    (0x0E<<USICNT0);                                /* set USI counter to shift 1 bit. */ \
}

#define SET_USI_TO_TWI_START_CONDITION_MODE()                                                                                     \
{                                                                                                                                 \
  USICR    =  (1<<USISIE)|(0<<USIOIE)|                        /* Enable Start Condition Interrupt. Disable Overflow Interrupt.*/  \
              (1<<USIWM1)|(0<<USIWM0)|                        /* Set USI in Two-wire mode. No USI Counter overflow hold.      */  \
              (1<<USICS1)|(0<<USICS0)|(0<<USICLK)|            /* Shift Register Clock Source = External, positive edge        */  \
              (0<<USITC);                                                                                                         \
  USISR    =  (0<<USI_START_COND_INT)|(1<<USIOIF)|(1<<USIPF)|(1<<USIDC)|  /* Clear all flags, except Start Cond                            */ \
              (0x0<<USICNT0);                                                                                                     \
}

#define SET_USI_TO_SEND_DATA()                                                                               \
{                                                                                                            \
    DDR_USI |=  (1<<PORT_USI_SDA);                                  /* Set SDA as output                  */ \
    USISR    =  (0<<USI_START_COND_INT)|(1<<USIOIF)|(1<<USIPF)|(1<<USIDC)|      /* Clear all flags, except Start Cond */ \
                (0x0<<USICNT0);                                     /* set USI to shift out 8 bits        */ \
}

#define SET_USI_TO_READ_DATA()                                                                               \
{                                                                                                            \
    DDR_USI &= ~(1<<PORT_USI_SDA);                                  /* Set SDA as input                   */ \
    USISR    =  (0<<USI_START_COND_INT)|(1<<USIOIF)|(1<<USIPF)|(1<<USIDC)|      /* Clear all flags, except Start Cond */ \
                (0x0<<USICNT0);                                     /* set USI to shift out 8 bits        */ \
}


