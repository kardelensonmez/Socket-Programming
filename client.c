
#include <stdio.h>
#include <winsock.h>
#include <time.h>

// Winsock library
#pragma comment(lib, "ws2_32.lib")

#define RECEIVE_BUFFER_SIZE 32

void error(char *message)
{
    fprintf(stderr, "%s: %d", message, WSAGetLastError());
    exit(1);
    return;
}

void close(SOCKET so)
{
    closesocket(so);
    WSACleanup();
    return;
}

int main(int argc, char *argv[])
{
    SOCKET so;
    WSADATA wsa;
    char *server_ip;
    char message[200];
    char *data = "DATATIME";
    char *c_false = "Time is false";
    char *c_true = "Time is true";
    char buffer[RECEIVE_BUFFER_SIZE];
    unsigned short server_port;
    int bytes_received;
    struct sockaddr_in server;
    unsigned long end_t = 0;
    unsigned long start_t = 0;
    time_t a;
    //struct tm *time_detay;
    a = time(NULL);
    

    if (argc != 3)
    {
        printf("Usage: %s <server_ip> <server_port>\n", argv[0]);
        return 1;
    }

    server_ip = argv[1];
    server_port = atoi(argv[2]);

    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
        error("Failed WSAStartup");

    printf("Winsock Initialised.\n");

    if ((so = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET)
        error("Error on created the socket");

    printf("Socket created.\n");

    memset(&server, 0, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr(server_ip);
    server.sin_port = htons(server_port);

    if (connect(so, (struct sockaddr *)&server, sizeof server) < 0)
        error("Failed to connect");

    printf("Connected to %s:%d\n", server_ip, server_port);

    while (strcmp(message, "QUIT") != 0)
    {
        end_t = clock() / CLOCKS_PER_SEC;
        if (end_t - start_t >= 20){
            send(so, data, strlen(data) + 1, 0);
            start_t = clock() / CLOCKS_PER_SEC; 
        }
        if ((bytes_received = recv(so, buffer, RECEIVE_BUFFER_SIZE, 0)) == SOCKET_ERROR)
            error("Failed on receive");

        buffer[bytes_received] = '\0';
        printf("[Server]: %s\n", buffer);
        char b = ctime(&a);
        if (strcmp(buffer, "ISYOURTIMESET") == 0)
        {
            send(so, "OK", 2, 0);
        }else if(strcmp(buffer, b) == 0){
            send(so, c_true, strlen(c_true)+1, 0);
        }
    }
    closesocket(so);
    WSACleanup();

    return 0;
}