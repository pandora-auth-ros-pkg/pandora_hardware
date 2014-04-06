#include <pololu/orangutan.h> 


char receive_buffer[32];  //receive bytes on USB_COMM

unsigned char receive_buffer_position = 0; //receive buffer position

char send_buffer[32]; //sen buffer on USB_COMM

void wait_for_sending_to_finish() //waits for the bytes transmitting on USB_COMM
{
	while(!serial_send_buffer_empty(USB_COMM))
		serial_check();		// USB_COMM port is always in SERIAL_CHECK mode
}

void process_received_byte(char byte) { // responds to a byte that has been received

    int speed = (int)byte;
    int transformedSpeed;
	if(speed >= 0)
	{
		 transformedSpeed=-(127-speed)*2;
	}
	else
	{
		 transformedSpeed=(128+speed)*2;
	}
	set_m1_speed(transformedSpeed);
    
    set_m2_speed(transformedSpeed);
    
    red_led(1);
    
    
	
}


void check_for_new_bytes_received()
{
	while(serial_get_received_bytes(USB_COMM) != receive_buffer_position)
	{
		// Process the new byte that has just been received.
		process_received_byte(receive_buffer[receive_buffer_position]);

		// Increment receive_buffer_position, but wrap around when it gets to
		// the end of the buffer. 
		if (receive_buffer_position == sizeof(receive_buffer)-1)
		{
			receive_buffer_position = 0;
		}
		else
		{
			receive_buffer_position++;
		}
	}
}


int main(){
	
	serial_set_baud_rate(USB_COMM, 9600);
	
	serial_receive_ring(USB_COMM, receive_buffer, sizeof(receive_buffer));
	
	while(1){
		
		serial_check();
		
		check_for_new_bytes_received();
		
		}
}
