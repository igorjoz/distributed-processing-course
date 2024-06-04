#include <unistd.h>
#include <stdio.h>

int main() {
    int potok[2];
    pipe(potok);

    if (fork() == 0) { // Proces potomny
        close(potok[1]); // Zamykamy końcówkę zapisu
        dup2(potok[0], STDIN_FILENO);
        close(potok[0]);
        execlp("grep", "grep", "main", NULL);
    } else { // Proces macierzysty
        close(potok[0]); // Zamykamy końcówkę odczytu
        dup2(potok[1], STDOUT_FILENO);
        close(potok[1]);
        execlp("ls", "ls", NULL);
    }

    return 0;
}





potok w potoku