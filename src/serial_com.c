/*

Compile: gcc serial_com.c -o serial_com
Run: sudo ./serial_com

Opcodes: 1 - light-up
         2 - light-down
         3 - blink 3-times
         4 - calcualte square on board and recieve the result

*/

#include <stdio.h>
#include <fcntl.h> // File control
#include <termios.h> // POSIX terminal control
#include <unistd.h> // UNIX standard
#include <errno.h> // Error number

int main()
{
  /* ---------- Opening serial port and configration ---------- */

  char port_name[] = "/dev/ttyACM0"; // Port name that board connected.
  int port = open(port_name, O_RDWR | O_NOCTTY | O_NDELAY); // File descriptor

  if(port == -1) // Error checking
    printf("Error in opening ttyACM0.\n");
  else
    printf("ttyACM0 opened.\n");

  struct termios SerialPortSettings; // Structure

  tcgetattr(port, &SerialPortSettings); // Current attributes of port
  cfsetispeed(&SerialPortSettings, B9600); // Baudrate 9600
  cfsetospeed(&SerialPortSettings, B9600); // Baudrate 9600

  SerialPortSettings.c_cflag &= ~PARENB; // Disable parity
  SerialPortSettings.c_cflag &= ~CSTOPB; // Stop bit
  SerialPortSettings.c_cflag &= ~CSIZE; // Clears the mask for data size
  SerialPortSettings.c_cflag |=  CS8; // Data bits = 8
  SerialPortSettings.c_cflag &= ~CRTSCTS; // No hardware flow control
  SerialPortSettings.c_cflag |= CREAD | CLOCAL; // Enable reciever
  SerialPortSettings.c_iflag &= ~(IXON | IXOFF | IXANY); // Disable flow control
  SerialPortSettings.c_iflag &= ~(ICANON | ECHO | ECHOE | ISIG); // Non connonical mode
  SerialPortSettings.c_oflag &= ~OPOST; // No output processing
  SerialPortSettings.c_cc[VMIN] = 5; // Read at least 5 character
  SerialPortSettings.c_cc[VTIME] = 0; // Wait indefinetly

  if((tcsetattr(port, TCSANOW, &SerialPortSettings)) != 0) // Set the attributes
  {
    printf("Error in setting attributes.\n");
    return -1;
  }

  printf("\n-------------------------\n\n");

  /* ---------- Main part ---------- */

  const char* opcode1 = "1";
  const char* opcode2 = "2";
  const char* opcode3 = "3";
  const char* opcode4 = "4";
  char number[5];
  char temp;
  int opcode;

  while(1)
  {
    printf("Enter opcode: "); scanf("%d", &opcode);

    if(opcode == 1)
      write(port, opcode1, 1);

    else if(opcode == 2)
      write(port, opcode2, 1);

    else if(opcode == 3)
      write(port, opcode3, 1);

    else if(opcode == 4)
    {
      printf("\nEnter a number: "); scanf("%s", number); // Only works for x <= 255
      write(port, opcode4, 1);
      write(port, number, sizeof(number));

      tcflush(port, TCIFLUSH); // Discard old data in the buffer
      sleep(1); // Wait for 1 second

      printf("Square of %s = ", number);

      while(1) // Get bytes until end flag, end_flag = '*'
      {
        read(port, &temp, 1);

        if(temp != '*') // End?
            printf("%c", temp);
        else
          break;
      }

      printf("\n\n");
    }

    else if(opcode == 0)
      break;
  }

  close(port);
  printf("\n-------------------------\nttyACM0 closed.\n");
}
