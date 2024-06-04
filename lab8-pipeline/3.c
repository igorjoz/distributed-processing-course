#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#define ODCZYT 0
#define ZAPIS 1

void producent(int potok[]) {
    close(potok[ODCZYT]); // Zamykamy końcówkę odczytu
    int liczby[] = {1, 2, 3, 4, 5};

    for (int i = 0; i < 5; ++i) {
        write(potok[ZAPIS], &liczby[i], sizeof(int));
        printf("Producent: Wyslalem liczbe %d\n", liczby[i]);
    }

    close(potok[ZAPIS]);
    exit(0);
}

void konsument(int potok[]) {
    close(potok[ZAPIS]); // Zamykamy końcówkę zapisu
    int liczba;

    while (read(potok[ODCZYT], &liczba, sizeof(int)) > 0) {
        printf("Konsument: Otrzymalem liczbe %d\n", liczba);
    }

    close(potok[ODCZYT]);
    exit(0);
}

int main() {
    int potok[2];

    if (pipe(potok) == -1) {
        perror("Nie udalo sie stworzyc potoku");
        return 1;
    }

    if (fork() == 0) { // Proces potomny jako konsument
        konsument(potok);
    } else { // Proces macierzysty jako producent
        producent(potok);
    }

    return 0;
}








3