#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h> // inet_addr
#include <unistd.h>

using namespace std;

int main()
{
    // 1. Create Socket
    int clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == -1) {
        cerr << "❌ Error: Could not create socket." << endl;
        return 1;
    }

    // 2. Setup Address
    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(8080);
    serverAddress.sin_addr.s_addr = inet_addr("127.0.0.1"); // Localhost

    // 3. Connect
    cout << "⏳ Connecting to server..." << endl;
    if (connect(clientSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) == -1) {
        cerr << "❌ Error: Connection failed! (Is the server running?)" << endl;
        return 1;
    }
    cout << "✅ Connected successfully!" << endl;

    // 4. Send Data
    const char* message = "Hello from Client!";
    string input;
    cout << "Type your message: ";
    getline(cin, input);
    message = input.c_str();
    
    send(clientSocket, message, strlen(message), 0);
    cout << "📤 Message sent: " << message << endl;

    // 5. Close
    close(clientSocket);
    cout << "🔒 Connection closed." << endl;

    return 0;
}