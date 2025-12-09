#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>

#pragma comment(lib, "ws2_32.lib")

#define PORTA 3333
#define BUFFER 1024

int main() {
    WSADATA wsa;
    WSAStartup(MAKEWORD(2,2), &wsa);

    SOCKET clientSock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

    struct sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORTA);
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    char lettera;
    int num1, num2;
    char buffer[BUFFER];

    printf("Inserisci una lettera (A/S/M/D): ");
    scanf(" %c", &lettera);
    printf("Inserisci il primo numero: ");
    scanf("%d", &num1);
    printf("Inserisci il secondo numero: ");
    scanf("%d", &num2);

    sprintf(buffer, "%c,%d,%d", lettera, num1, num2);

    sendto(clientSock, buffer, strlen(buffer)+1, 0,
           (struct sockaddr*)&serverAddr, sizeof(serverAddr));

    int addrLen = sizeof(serverAddr);
    int recvLen = recvfrom(clientSock, buffer, BUFFER-1, 0,
                           (struct sockaddr*)&serverAddr, &addrLen);
    buffer[recvLen] = '\0';

    printf("Risultato: %s\n", buffer);

    closesocket(clientSock);
    WSACleanup();
    return 0;
}
