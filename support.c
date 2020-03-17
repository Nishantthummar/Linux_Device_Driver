#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>

#define IOCTL_READ _IOR('N', 'a', char *)
#define IOCTL_WRITE _IOW('N', 'b', char *)

int main()
{
	int i_fd;
	char *c_send;
	i_fd = open("/dev/ioctl_cdriver", O_RDWR);
	if(i_fd < 0) {
		printf("\nCannot open file");
		return 0;
	}

	printf("\nEnter data To send");
	scanf(" %s", c_send);
	printf("\nWriting into Driver");
	ioctl(i_fd, IOCTL_WRITE, (char *) &c_send);

	printf("\nReading from Driver");
	ioctl(i_fd, IOCTL_READ, (char *) &c_send);
	printf("\nData is %s", c_send);
	close(i_fd);
}
