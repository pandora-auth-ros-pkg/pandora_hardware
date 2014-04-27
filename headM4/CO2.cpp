#include "CO2.hpp"

static Serial2 *co2uart;

/**
 * CO2 sensor buffer.
 * @note buffer_size=19 (worst case scenario with 4 byte stuffings)
 * @note Without the buffer, we would have to process data in the time between 2 consecutive
 * incoming bytes.
 */
static Queue<uint8_t, 19> CO2queue;

void CO2valueSet(float value) {
	if (value>=0 && value<5) {
		HealthyCO2valueSet(value);
	}
}

//Both putc and printf() use serial_putc() in serial_api.c. Transmit FIFO size
//is 16. If we transmit new data and FIFO is full the new data gets lost.
//Same goes for Receive FIFO according to user manual (Overrun Error). UART
//peripheral starts to transmit immediately after feeding it. If we don't
//feed with new data fast enough and transmit buffer gets empty we have a THRE
//interrupt.
//Required time to transmit n bytes is (1/Baud)*(1+8+1)*n


void RX_isr() {
	uint8_t CO2recv = co2uart->getcNB();	//Reading UnRBR clears the interrupt. If we don't clear it
										//->the ISR would retrigger infinitely.
	CO2queue.put((uint8_t *)CO2recv);
}

void CO2Init(PinName tx, PinName rx) {
	co2uart = new Serial2(tx, rx);
	co2uart->baud(38400);	///Baud 38400, 8N1
	co2uart->attach(&RX_isr, Serial::RxIrq);
}

void CO2Trigger() {
	/**
	 * @brief Live Data Simple request to CO2 sensor
	 * We use the fact that uC UART FIFO is 16 bytes long and we send this 7 byte message
	 * periodically after a long period of time (from UART's point of view) to achieve
	 * maximum performance without using DMA or interrupts.
	 */
	uint8_t co2TransmitBuffer[7] = { DLE, RD, Var_ID, DLE, EOFF, Checksum_hi,
	Checksum_lo };

	for (int i = 0; i < 7; ++i) {
		co2uart->putcNB(co2TransmitBuffer[i]);//Message must be maximum 16 bytes
											 //->(FIFO size)
	}
}

void CO2Task(void const *args) {
	uint8_t state = 0;
	uint8_t DATaPacket = 0;
	uint8_t NAKPacket = 0;
	uint8_t DataLength;	///Data field length minus version and status flag bytes
	uint8_t vi = 0, si = 0, gi = 0;
	uint8_t gj = 0;	///Index in uint8ieee[4]
	uint8_t StatusError =0;	///When Status flag !=0 indicates Status Error
	uint8_t uint8ieee[4];	///Contains the IEEE-754 as integers (CPU is little Endian)
	float GasReading;
	uint8_t ByteStuffing = 0;
	uint8_t ChecksumReceived[2];
	uint16_t ChecksumCalculated;
	uint8_t CO2SensorError = 0;
	while (1) {
		osEvent evt = CO2queue.get();	//If queue empty, stops here and lets other threads run
		int recv_char = evt.value.v;	///Received character from CO2UART
		if (state>0 && state<=7) ChecksumCalculated += recv_char;
		switch (state) {
		case 0:
			if (recv_char == DLE) {
				DATaPacket = 0;
				NAKPacket = 0;
				StatusError =0;
				ChecksumCalculated = recv_char;	//We don't initialize Checksum in the last state
												//->in case we didn't go there because of noise.
				CO2SensorError = 0;
				state++;
			}
			break;
		case 1:	//If noise corrupts a byte in the FSM sequence, we loop between state
				//->0 and 1, until we have a valid sequence of DLE&DAT or DLE&NAK
			if (recv_char == DAT) {
				DATaPacket = 1;
				state++;
			} else if (recv_char == NAK) {
				NAKPacket = 1;
				state++;
			} else
				state = 0;
			break;
		case 2:
			if (NAKPacket)
				state = 0;
			else if (DATaPacket) {
				DataLength = recv_char - 4;
				if (DataLength <4) state = 0;	//We check for the upper limit in state 5.
				else state++;
			}
			else state = 0;
			break;
		case 3:
			if (vi < 2 - 1)
				vi++;
			else {
				vi = 0;
				state++;
			}
			break;
		case 4:
			if (si < 2 - 1) {
				if (recv_char != 0)
					StatusError = 1;
				si++;
			} else {
				if (recv_char != 0)
					StatusError = 1;
				si = 0;
				state++;
			}
			break;
		case 5:
			if (gi < DataLength - 1){
				if (recv_char != DLE || ByteStuffing) {
					uint8ieee[gj] = recv_char;
					ByteStuffing = 0;
					gj++;
					if (gj >= 4) state=0;	//Don't violate array limits if DataLength corrupt
				}
				else ByteStuffing = 1;
				gi++;
			} else {
				uint8ieee[gj] = recv_char;
				GasReading = *(float *)uint8ieee;
				ByteStuffing = 0;
				gi = 0;
				gj = 0;
				state++;
			}
			break;
		case 6:	//DLE
			if (recv_char == DLE)
				state++;
			else state = 0;
			break;
		case 7:	//EOF
			if (recv_char == EOFF)
				state++;
			else state = 0;
			break;
		case 8:	//ChecksumReceived high
			ChecksumReceived[1] = recv_char;
			state++;
			break;
		case 9:	//ChecksumReceived low
			ChecksumReceived[0] = recv_char;
			if (ChecksumCalculated != *(uint16_t *)ChecksumReceived || StatusError)
				CO2SensorError = 1;
			if (!CO2SensorError) CO2valueSet(GasReading);
			state = 0;
			break;
		}
	}
}
