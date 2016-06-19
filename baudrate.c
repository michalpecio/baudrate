#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <unistd.h>

#include <asm/termbits.h>

static int get_rate(const int fd) {
	struct termios2 tio;
	if (ioctl(fd, TCGETS2, &tio) == -1) {
		perror("Can't get rate");
		return 1;
	}
	printf("%u\n", tio.c_ispeed);
	if (tio.c_ospeed != tio.c_ispeed) {
		printf("%u\n", tio.c_ospeed);
	}
	return 0;
}

static int set_rate(const int fd, const int rate) {
	struct termios2 tio;
	if (ioctl(fd, TCGETS2, &tio) == -1) {
		perror("Can't get rate");
		return 1;
	}
	tio.c_cflag &= ~CBAUD;
	tio.c_cflag |= BOTHER;
	tio.c_ispeed = rate;
	tio.c_ospeed = rate;
	if (ioctl(fd, TCSETS2, &tio) == -1) {
		perror("Can't set rate");
		return 1;
	}
	return 0;
}

int main(int argc, char **argv) {
	if (argc < 2 || argc > 3) {
		fprintf(stderr, "USAGE: %s <device> [<rate>]\n", argv[0]);
		return 1;
	}
	const int rate = (argc == 3)? atoi(argv[2]): 0;
	const int fd = open(argv[1], O_RDWR);
	if (fd < 0) {
		perror("Can't open device");
		return 1;
	}
	if (argc == 2)
		return get_rate(fd);
	else
		return set_rate(fd, rate) || get_rate(fd);
}
