#include <stdio.h>
#include <string.h>
#include <fcntl.h> // Contains file controls like 0_RDWR
#include <errno.h> // Error integer and strerror() function
#include <termios.h> // POSIX terminal control definitions
#include <unistd.h> // write(), read(), close()

class Serial {
public:
	int serial_port;
	struct termios tty;

	Serial();
	int setup(const char* filename);
};


