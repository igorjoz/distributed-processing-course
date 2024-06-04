#include <unistd.h>
#include <stdio.h>

#define ODCZYT 0
#define ZAPIS 1

int main() {
    int potok[2];
    int x;

    puts("Program pipes startuje");
    puts("Tworze potok");

    if (pipe(potok) == -1) {
        perror("Nie udalo sie stworzyc potoku");
        return 1;
    }

    if (fork() == 0) { // Proces potomny
        close(potok[ZAPIS]);
        read(potok[ODCZYT], &x, sizeof(x));
        printf("Potomek: Otrzymalem liczbe %d\n", x);
        close(potok[ODCZYT]);
    } else { // Proces macierzysty
        close(potok[ODCZYT]);
        x = 42;
        write(potok[ZAPIS], &x, sizeof(x));
        printf("Rodzic: Wyslalem liczbe %d\n", x);
        close(potok[ZAPIS]);
    }

    return 0;
}





potok anonimowy