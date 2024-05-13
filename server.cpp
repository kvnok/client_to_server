#include <iostream>     // For input/output operations
#include <cstring>      // For string manipulation functions
#include <sys/socket.h> // For socket related functions
#include <netinet/in.h> // For internet address structures
#include <unistd.h>     // For close() function

int main() {
    int serverSocket, clientSocket; // socket fds
    struct sockaddr_in serverAddr, clientAddr; // address structures
    socklen_t clientAddrLen = sizeof(clientAddr); // client address length
    char buffer[1024]; // buffer to store data
    // Create socket for server, tcp socket
    if ((serverSocket = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        std::cerr << "Error creating socket\n";
        return 1;
    }
    // Fill server address information
    serverAddr.sin_family = AF_INET; // Use IPv4
    serverAddr.sin_port = htons(12345); // Use port 12345
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY); // Bind to all available interfaces
    memset(serverAddr.sin_zero, '\0', sizeof(serverAddr.sin_zero)); // Fill remaining bytes with null characters
    // Bind the socket to the server address
    if (bind(serverSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == -1) {
        std::cerr << "Error binding socket\n";
        return 1;
    }
    // Listen for connections on the server socket
    if (listen(serverSocket, 5) == -1) {
        std::cerr << "Error listening\n";
        return 1;
    }
    std::cout << "Server started. Listening on port 12345...\n";
    // Accept incoming connections from clients
    if ((clientSocket = accept(serverSocket, (struct sockaddr *)&clientAddr, &clientAddrLen)) == -1) {
        std::cerr << "Error accepting connection\n";
        return 1;
    }
    std::cout << "Client connected\n";
    // Receive and echo messages
    while (true) {
        int bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0);
        if (bytesReceived <= 0) {
            std::cerr << "Connection closed by client\n";
            break;
        }
        buffer[bytesReceived] = '\0'; // Null-terminate the received data
        std::cout << "Received: " << buffer << std::endl;
        // Echo back to client
        if (send(clientSocket, buffer, bytesReceived, 0) == -1) {
            std::cerr << "Error sending data\n";
            break;
        }
    }
    // Close sockets
    close(clientSocket);
    close(serverSocket);
    return 0;
}
