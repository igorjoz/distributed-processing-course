#include<windows.h>
#include<stdio.h>
#include<conio.h>

#pragma argsused

struct dane_dla_watku  {
	char nazwa[50];
	int parametr;
} dane[5] = {
		{ "[1]", 6 },
		{ "[2]", 9 },
		{ "[3]", 12 },
		{ "[4]", 15 },
		{ "[5]", 18 }
	};

// function gotoxy
// source: https://stackoverflow.com/questions/7955145/which-header-file-i-need-to-include-to-use-gotoxy-function
// author: Benjamin Lindley
void gotoxy(int x, int y) {
	static HANDLE h = NULL;
	if (!h)
		h = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD c = { x, y };
	SetConsoleCursorPosition(h, c);
}

int priorytety[5] = {
	THREAD_PRIORITY_BELOW_NORMAL,
	THREAD_PRIORITY_LOWEST,
	THREAD_PRIORITY_ABOVE_NORMAL,
	THREAD_PRIORITY_NORMAL,
	THREAD_PRIORITY_HIGHEST,
};

HANDLE watki[5];

DWORD WINAPI funkcja_watku(void* argumenty);

int main(int argc, char** argv)
{
	int i;
	DWORD id; // identyfikator watku
	system("cls");
	printf("Uruchomienie programu\n");

	for (i = 0; i < 5; i++) {
		watki[i] = CreateThread(
			NULL,				// atrybuty bezpieczenstwa
			0,					// inicjalna wielkosc stosu
			funkcja_watku,		// funkcja watku
			(void*)&dane[i],	// dane dla funkcji watku
			0,					// flagi utworzenia
			&id);

		if (watki[i] != INVALID_HANDLE_VALUE) {
			printf("\nUtworzylem watek %s o id %x \n\n", dane[i].nazwa, id);
			SetThreadPriority(watki[i], priorytety[i]);
		}

		/*if (i == 3) {
			LPDWORD threadExitCode;
			GetExitCodeThread(watki[3], threadExitCode);
			ExitThread((DWORD)threadExitCode);
		}*/
	}

	Sleep(20000); // uspienie watku glownego na 20 s
	return 0;
}

DWORD WINAPI funkcja_watku(void* argumenty) {
	unsigned int licznik = 0;
	// rzutowanie struktury na wlasny wskaznik
	struct dane_dla_watku* moje_dane = (struct dane_dla_watku*)argumenty;
	// wyswietlenie informacji o uruchomieniu
	gotoxy(1, moje_dane->parametr);
	printf("%s", moje_dane->nazwa);

	HANDLE currenthThread = GetCurrentThread();
	DWORD currentThreadId = GetCurrentThreadId();
	LPDWORD threadExitCode = 0;
	//GetExitCodeThread(currenthThread, threadExitCode);

	gotoxy(3, moje_dane->parametr + 1);
	printf("%d", currentThreadId);

	Sleep(1000);

	// praca , watki sa terminowane przez zakonczenie program - funkcji main
	while (true) {
		gotoxy(licznik++ / 5000 + 10, moje_dane->parametr);
		printf("x");
	}

	return 0;
}
