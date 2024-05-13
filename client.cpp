#include <iostream>     // For input/output operations
#include <cstring>      // For string manipulation functions
#include <sys/socket.h> // For socket related functions
#include <netinet/in.h> // For internet address structures
#include <arpa/inet.h>  // For manipulating IP addresses
#include <unistd.h>     // For close() function

int main() {
    int clientSocket;                           // Socket descriptor for the client
    struct sockaddr_in serverAddr;              // Structure to hold server address information
    char buffer[1024];                          // Buffer to store data to be sent/received
    // Create a socket for the client, tcp socket
    if ((clientSocket = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        std::cerr << "Error creating socket\n";
        return 1;
    }
    // Fill server address information
    serverAddr.sin_family = AF_INET;            // Set address family to IPv4
    serverAddr.sin_port = htons(12345);         // Set port number to connect to
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1"); // Set IP address to connect to
    memset(serverAddr.sin_zero, '\0', sizeof(serverAddr.sin_zero)); // Fill remaining bytes with null characters
    // Connect to the server, using the server address information
    if (connect(clientSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == -1) {
        std::cerr << "Error connecting to server\n";
        return 1;
    }
    std::cout << "Connected to server\n"; // Print success message after successful connection
    // Loop to send and receive data
    while (true) {
        std::cout << "Enter message: ";
        std::cin.getline(buffer, sizeof(buffer)); // Read user input from console
        // Send data to server
        if (send(clientSocket, buffer, strlen(buffer), 0) == -1) {
            std::cerr << "Error sending data\n";
            break;
        }
        int bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0); // Receive data from server
        if (bytesReceived <= 0) {
            std::cerr << "Connection closed by server\n";
            break;
        }
        buffer[bytesReceived] = '\0'; // Null-terminate the received data
        std::cout << "Server response: " << buffer << std::endl; // Print the server's response
    }
    close(clientSocket); // Close the socket
    return 0;
}
