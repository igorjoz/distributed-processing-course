#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#define ODCZYT 0
#define ZAPIS 1

int main() {
    int potok[2];
    int liczba;

    if (pipe(potok) == -1) {
        perror("Nie udalo sie stworzyc potoku");
        return 1;
    }

    if (fork() == 0) { // Proces potomny
        close(potok[ZAPIS]);
        read(potok[ODCZYT], &liczba, sizeof(liczba));
        printf("Potomek: Otrzymalem liczbe %d\n", liczba);
        close(potok[ODCZYT]);
    } else { // Proces macierzysty
        close(potok[ODCZYT]);
        liczba = 42;
        write(potok[ZAPIS], &liczba, sizeof(liczba));
        printf("Rodzic: Wyslalem liczbe %d\n", liczba);
        close(potok[ZAPIS]);
    }

    return 0;
}





1