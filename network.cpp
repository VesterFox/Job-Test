#include <cstring>
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <vector>
#include "domain.h"
#include "log.h"

using namespace std;

int main(int argc, char* argv[])
{
    if(argc != 2)
        handleError("Необходимо указать значение State как аргумент (Normal / Failed).");

    if(SetState(argv[1]) == false)
        handleError("Недопустимое значение State.");

    // Создание UDP сокета сервера
    int serverSocket = socket(AF_INET, SOCK_DGRAM, 0);
    if(serverSocket < 0)
        handleError("Ошибка создания сокета сервера.");

    // Заполнение структуры адреса (порт 5555, адрес любой)
    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(5555);
    serverAddress.sin_addr.s_addr = INADDR_ANY;

    // Сокету присваивается созданный адрес
    if(bind(serverSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0)
        handleError("Ошибка присовения адреса сокету.");
    
    while(true)
    {
        uint8_t buffer [2];
        sockaddr_in clientAddress{};
        socklen_t clientAddrLen = sizeof(clientAddress);

        //Получение сообщения
        ssize_t bytesRead = recvfrom(serverSocket, buffer, sizeof(buffer), 0,
            (struct sockaddr*)&clientAddress, &clientAddrLen);
        
        if(bytesRead < 0)
        {
            handleError("Ошибка получения сообщения.", false);
            continue;
        }

        // Вывод сообщения ответа
        cout << "Получено сообщение: ";
        for(const auto& byte : buffer) 
        {
            printf("%02X ", byte);
        }
        cout << endl;

        // Подготовка ответа
        uint8_t response[3] = {0xFC, 0x01};

        // Проверка корректности ID
        if(bytesRead < 2 || buffer[0] != 0xCF || buffer[1] != 0x01)
        {
            handleError("Получено сообщение с некорректным ID.", false);
            continue;
        }
        else
        {
            // Если ID корректный, то происходит получение состояния
            // и его преобразование для отправки
            State state = GetState();

            response[2] = state.general == "Failed" ? 0x01 : 0x02;

            if(sendto(serverSocket, response, sizeof(response), 0,
                (struct sockaddr*)&clientAddress, clientAddrLen) != sizeof(response))
                handleError("Ошибка отправки ответа.", false);
        }

    }

    close(serverSocket);

    return EXIT_SUCCESS;
}