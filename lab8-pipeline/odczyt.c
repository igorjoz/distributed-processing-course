#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

#define FIFO_NAME "/tmp/moj_potok"

int main() {
    int fd;
    int x;

    fd = open(FIFO_NAME, O_RDONLY);
    read(fd, &x, sizeof(x));
    printf("Odczytana liczba: %d\n", x);
    close(fd);

    return 0;
}






odczyt