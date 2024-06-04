#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

#define FIFO_NAME "/tmp/moj_potok"

int main() {
    int fd;
    int liczba;

    fd = open(FIFO_NAME, O_RDONLY);
    read(fd, &liczba, sizeof(liczba));
    printf("Proces odczytu: Odczytalem liczbe %d\n", liczba);
    close(fd);

    return 0;
}
