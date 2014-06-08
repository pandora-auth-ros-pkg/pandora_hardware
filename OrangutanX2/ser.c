#include <pololu/orangutan.h>

#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))

char receive_buffer[255];  //receive bytes on USB_COMM
char packet_buffer[10];  //stores a packet

unsigned char receive_buffer_position = 0; //receive buffer position
unsigned char packet_buffer_position = 0; //packet buffer position

char send_buffer[32]; //sen buffer on USB_COMM

unsigned char recording_packet = 0;

int find_sign(char sign)
{
  int i = 0;
  if (sign == '-')
    i = -1;
  if (sign == '+')
    i = 1;
  return i;
}

void process_packet() 
{
  // packet format: $L-001R+100
  
  if (packet_buffer[0] != 'L' || packet_buffer[5] != 'R')
  {
    return; // we got a wrong packet, no point to continue
  }
  
  int speed_left = (int)(packet_buffer[4] - '0') + 10*(int)(packet_buffer[3] - '0') + 100*(int)(packet_buffer[2] - '0');
  int speed_right = (int)(packet_buffer[9] - '0') + 10*(int)(packet_buffer[8] - '0') + 100*(int)(packet_buffer[7] - '0');
  
  speed_left = find_sign(packet_buffer[1]) * speed_left;
  speed_right = find_sign(packet_buffer[6]) * speed_right;
  
  speed_left = MAX(MIN(speed_left, 255), -255); //bound speeds to (-255, 255)
  speed_right = MAX(MIN(speed_right, 255), -255); 
  
  set_m1_speed(speed_left);
  set_m2_speed(speed_right);
  
}

void increment_receive_buffer_position()
{
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

void check_for_new_bytes_received()
{
  while(serial_get_received_bytes(USB_COMM) != receive_buffer_position)
  {
    increment_receive_buffer_position();
    // check for start of packet
    if (receive_buffer[receive_buffer_position-1] == '$' && !recording_packet)
    {
      packet_buffer_position = 0;
      recording_packet = 1;
      continue;
    }
    
    if (recording_packet)
    {
      packet_buffer[packet_buffer_position++] = receive_buffer[receive_buffer_position-1];
      
      // we have a whole packet
      if (packet_buffer_position == sizeof(packet_buffer))
      {
        recording_packet = 0;
        process_packet();
      }
    }
  }
  
}


int main()
{
  serial_set_baud_rate(USB_COMM, 9600);
  
  serial_receive_ring(USB_COMM, receive_buffer, sizeof(receive_buffer));
  
  while(1){
    
    // calling OrangutanSerial::check() periodically to trigger 
    // reception and transmission of new bytes.
    serial_check();
    
    check_for_new_bytes_received();
  }
}
