// Client.cpp

#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

int main() {
    int clientSocket;
    struct sockaddr_in serverAddr;
    char buffer[1024];

    // Create socket
    if ((clientSocket = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        std::cerr << "Error creating socket\n";
        return 1;
    }

    // Connect to server
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(12345); // Connect to port 12345
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1"); // Connect to localhost
    memset(serverAddr.sin_zero, '\0', sizeof(serverAddr.sin_zero));

    if (connect(clientSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == -1) {
        std::cerr << "Error connecting to server\n";
        return 1;
    }

    std::cout << "Connected to server\n";

    // Send and receive messages
    while (true) {
        std::cout << "Enter message: ";
        std::cin.getline(buffer, sizeof(buffer));

        // Send message to server
        if (send(clientSocket, buffer, strlen(buffer), 0) == -1) {
            std::cerr << "Error sending data\n";
            break;
        }

        // Receive response from server
        int bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0);
        if (bytesReceived <= 0) {
            std::cerr << "Connection closed by server\n";
            break;
        }

        buffer[bytesReceived] = '\0'; // Null-terminate the received data
        std::cout << "Server response: " << buffer << std::endl;
    }

    // Close socket
    close(clientSocket);

    return 0;
}
