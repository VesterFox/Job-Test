#include <cstring>
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <vector>
#include "../log.h"

using namespace std;

int SendRequest()
{
    // Создание сокета
    int clientSocket = socket(AF_INET, SOCK_DGRAM, 0);
    if(clientSocket < 0)
    {
        handleError("Ошибка создания сокета.");
    }

    // Заполнение структуры адреса
    sockaddr_in serverAddress{};
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(5555);
    serverAddress.sin_addr.s_addr = INADDR_ANY;

    uint8_t message[] = {0xCF, 0x01};

    //Отправка сообщения
    if (sendto(clientSocket, message, sizeof(message), 0, 
        (struct sockaddr*)&serverAddress, sizeof(serverAddress)) != sizeof(message)) 
    {
        close(clientSocket);
        handleError("Ошибка отправки сообщения.");
    } 
    else 
    {
        cout << "Тестовое сообщение отправлено." << endl;
    }

    uint8_t response [3];
    sockaddr_in fromAddress{};
    socklen_t fromAddrLen = sizeof(fromAddress);

    ssize_t bytesRead = recvfrom(clientSocket, response, sizeof(response), 
        0, (struct sockaddr*)&fromAddress, &fromAddrLen);
    if(bytesRead < 0) 
    {
        close(clientSocket);
        handleError("Ошибка получения ответа");
    }

    // Вывод сообщения ответа
    cout << "Получен ответ: ";
    for(const auto& byte : response) 
    {
        printf("%02X ", byte);
    }
    cout << endl;

    close(clientSocket);
    return EXIT_SUCCESS;
}

int main()
{
    SendRequest();
}