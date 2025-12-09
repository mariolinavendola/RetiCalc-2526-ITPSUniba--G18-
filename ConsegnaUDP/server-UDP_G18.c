#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>
#include <string.h>

#pragma comment(lib, "ws2_32.lib")

#define PORTA 3333
#define BUFFER 1024

int main() {
    WSADATA wsa;
    WSAStartup(MAKEWORD(2,2), &wsa);

    SOCKET serverSock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

    struct sockaddr_in serverAddr, clientAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORTA);
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    bind(serverSock, (struct sockaddr*)&serverAddr, sizeof(serverAddr));

    printf("SERVER UDP ATTIVO SULLA PORTA %d...\n", PORTA);

    while(1) {
        char buffer[BUFFER];
        int clientLen = sizeof(clientAddr);

        int recvLen = recvfrom(serverSock, buffer, BUFFER-1, 0,
                               (struct sockaddr*)&clientAddr, &clientLen);
        buffer[recvLen] = '\0';

        char copia[BUFFER];
        strcpy(copia, buffer);
        char* lettera = strtok(copia, ",");
        char* num1_str = strtok(NULL, ",");
        char* num2_str = strtok(NULL, ",");

        printf("Gestendo il client %s\n", inet_ntoa(clientAddr.sin_addr));

        int risultato = 0;
        if (!lettera || !num1_str || !num2_str || strlen(lettera) != 1) {
            strcpy(buffer, "TERMINE PROCESSO CLIENT");
            sendto(serverSock, buffer, strlen(buffer)+1, 0,
                   (struct sockaddr*)&clientAddr, clientLen);
            printf("Risultato inviato: TERMINE PROCESSO CLIENT\n");
            printf("Gestione del client terminata.\n\n");
            continue;
        }

        int a = atoi(num1_str);
        int b = atoi(num2_str);
        printf("Lettera ricevuta: %c\n", lettera[0]);
        printf("Numeri ricevuti: %d, %d\n", a, b);

        switch(lettera[0]) {
            case 'A': case 'a': risultato = a + b; break;
            case 'S': case 's': risultato = a - b; break;
            case 'M': case 'm': risultato = a * b; break;
            case 'D': case 'd': risultato = (b == 0) ? 0 : a / b; break;
            default:
                strcpy(buffer, "TERMINE PROCESSO CLIENT");
                sendto(serverSock, buffer, strlen(buffer)+1, 0,
                       (struct sockaddr*)&clientAddr, clientLen);
                printf("Risultato inviato: TERMINE PROCESSO CLIENT\n");
                printf("Gestione del client terminata.\n\n");
                continue;
        }

        sprintf(buffer, "%d", risultato);
        sendto(serverSock, buffer, strlen(buffer)+1, 0,
               (struct sockaddr*)&clientAddr, clientLen);
        printf("Risultato inviato: %d\n", risultato);
        printf("Gestione del client terminata.\n\n");
    }

    closesocket(serverSock);
    WSACleanup();
    return 0;
}
