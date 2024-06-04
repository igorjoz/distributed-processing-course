#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>

#define FIFO_NAME "/tmp/moj_potok"

int main() {
    int fd;
    int liczba = 123;

    mkfifo(FIFO_NAME, 0666);
    fd = open(FIFO_NAME, O_WRONLY);
    write(fd, &liczba, sizeof(liczba));
    printf("Proces zapisu: Wyslalem liczbe %d\n", liczba);
    close(fd);

    return 0;
}
