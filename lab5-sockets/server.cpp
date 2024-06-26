// prosty moduł serwera
// komunikacji za pomocą Winsock
// użycie: server.exe

#define _WINSOCK_DEPRECATED_NO_WARNINGS
#pragma comment(lib, "Ws2_32.lib")

#define _CRT_SECURE_NO_WARNINGS

#include <winsock2.h>
#include <stdio.h>
#include <stdlib.h>

#define DEFAULT_PORT 5000
#define DEFAULT_BUFFER 4096

// tylko Visual C++
#pragma comment(lib, "ws2_32.lib")

// funkcja: wątek do komunikacji z klientem
DWORD WINAPI ClientThread(LPVOID lpParam)
{
    SOCKET socket = (SOCKET)lpParam;
    char buffer[DEFAULT_BUFFER];
    int ret = 0;
    int nLeft;
    int idx;

    //FILE* file = fopen("file.txt", "rb");
    //fseek(file, 0, SEEK_END);
    //unsigned long filesize = ftell(file);
    //char* buffer = (char*)malloc(sizeof(char) * filesize);
    //rewind(file);
    //// store read data into buffer
    //fread(buffer, sizeof(char), filesize, file);

    // serwer będzie oczekiwał na informacje od klienta
    while (true)
    {
        // najpierw odbierz dane 
        ret = recv(socket, buffer, DEFAULT_BUFFER, 0);
        if (ret == 0) {
            break;
        }
        else if (ret == SOCKET_ERROR) {
            printf("błąd funkcji recv(): %d\n", WSAGetLastError());
            break;
        }

        buffer[ret] = '\0';
        printf("RECV: '%s'\n", buffer);

        // następnie odeślij te dane, poporcjuj jeśli trzeba
        // (niestety send() może nie wysłać wszystkiego)
        nLeft = ret;
        idx = 0;
        while (nLeft > 0)
        {
            ret = send(socket, &buffer[idx], nLeft, 0);
            //ret = send(socket, buffer, filesize, 0);
            if (ret == 0)
                break;
            else if (ret == SOCKET_ERROR)
            {
                printf("błąd funkcji send(): %d\n", WSAGetLastError());
                break;
            }
            nLeft -= ret;
            idx += ret;
        }
    }
    return 0;
}


int main(int argc, char* argv[])
{
    WSADATA wsd;
    SOCKET sListen,
        sClient;
    int iAddrSize;
    HANDLE hThread;
    DWORD dwThreadID;
    struct sockaddr_in local, client;
    struct hostent* host = NULL;

    // inicjuj Winsock 2.2 
    if (WSAStartup(MAKEWORD(2, 2), &wsd) != 0)
    {
        printf("Błąd ładowania Winsock 2.2!\n");
        return 1;
    }

    // twórz gniazdo do nasłuchu połączeń klientów
    sListen = socket(AF_INET, SOCK_STREAM, IPPROTO_IP);
    if (sListen == SOCKET_ERROR)
    {
        printf("Błąd funkcji socket(): %d\n", WSAGetLastError());
        return 1;
    }

    // wybierz interfejs (warstwę komunikacyjną)
    local.sin_addr.s_addr = htonl(INADDR_ANY);
    local.sin_family = AF_INET;
    local.sin_port = htons(DEFAULT_PORT);
    if (bind(sListen, (struct sockaddr*)&local, sizeof(local)) == SOCKET_ERROR)
    {
        printf("Błąd funkcji bind(): %d\n", WSAGetLastError());
        return 1;
    }


    // nasłuch
    host = gethostbyname("localhost");
    if (host == NULL)
    {
        printf("Nie udało się wydobyć nazwy serwera\n");
        return 1;
    }

    listen(sListen, 8);
    printf("Serwer nasłuchuje.\n");
    printf("Adres: %s, port: %d\n", host->h_name, DEFAULT_PORT);

    // akceptuj nadchodzące połączenia
    while (1)
    {
        iAddrSize = sizeof(client);
        sClient = accept(sListen, (struct sockaddr*)&client, &iAddrSize);
        if (sClient == INVALID_SOCKET)
        {
            printf("Błąd funkcji accept(): %d\n", WSAGetLastError());
            return 1;
        }
        printf("Zaakceptowano połączenie: serwer %s, port %d\n",
            inet_ntoa(client.sin_addr), ntohs(client.sin_port));
        hThread = CreateThread(NULL, 0, ClientThread,
            (LPVOID)sClient, 0, &dwThreadID);
        if (hThread == NULL)
        {
            printf("Błąd funkcji CreateThread(): %d\n", WSAGetLastError());
            return 1;
        }
        CloseHandle(hThread);
    }
    closesocket(sListen);

    WSACleanup();
    return 0;
}
