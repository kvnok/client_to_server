#include <iostream>     // For input/output operations
#include <cstring>      // For string manipulation functions
#include <sys/socket.h> // For socket related functions
#include <netinet/in.h> // For internet address structures
#include <arpa/inet.h>  // For manipulating IP addresses
#include <unistd.h>     // For close() function

void loop(int clientSocket, char *buffer)
{
    while (true) {
        // Prompt user for a request
        std::string userRequest;
        std::cout << "Enter your request (GET, POST, DELETE) (or type 'quit' to exit): ";
        std::getline(std::cin, userRequest);
        if (userRequest == "quit") {
            break;
        }
        // Construct the corresponding HTTP request based on user's input
        std::string httpRequest;
        if (userRequest == "GET") {
            httpRequest = "GET / HTTP/1.1\r\n"
                          "Host: localhost\r\n"
                          "Connection: close\r\n\r\n";
        } else if (userRequest == "POST") {
            // Construct POST request (replace with actual POST request as needed)
            httpRequest = "POST / HTTP/1.1\r\n"
                          "Host: localhost\r\n"
                          "Content-Length: 0\r\n"
                          "Connection: close\r\n\r\n";
        } else if (userRequest == "DELETE") {
            // Construct DELETE request (replace with actual DELETE request as needed)
            httpRequest = "DELETE / HTTP/1.1\r\n"
                          "Host: localhost\r\n"
                          "Connection: close\r\n\r\n";
        } else {
            std::cerr << "Invalid request\n";
            continue;
        }

        // Send the constructed HTTP request to the server
        if (send(clientSocket, httpRequest.c_str(), httpRequest.size(), 0) == -1) {
            std::cerr << "Error sending data\n";
            break;
        }
        // Receive and print server response
        int bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0);
        if (bytesReceived <= 0) {
            std::cerr << "Connection closed by server\n";
            break;
        }
        buffer[bytesReceived] = '\0'; // Null-terminate the received data
        std::cout << "Server response: " << buffer << std::endl;
    }
}

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
    // Call the loop function to send/receive data
    loop(clientSocket, buffer);
    close(clientSocket); // Close the socket
    return 0;
}
