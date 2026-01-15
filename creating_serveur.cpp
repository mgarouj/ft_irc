// #include <iostream>
// #include <sys/socket.h>
// #include <netinet/in.h>
// #include <unistd.h>
// #include <cstdio> // for perror

// using namespace std;

// int main() {
//     int server_fd = socket(AF_INET, SOCK_STREAM, 0);
//     cout << "server_fd: " << server_fd << endl;
//     write(server_fd, "server_fd test\n", 15); // Test write function
//     if (server_fd == -1) { perror("Socket failed"); return 1; }

//     // هاد اللعيبة (SO_REUSEADDR) مهمة بزاف في 1337
//     // باش إلا طفيتي السيرفر وشعلتيه ميبقاش يقول ليك "Address already in use"
//     int opt = 1;
//     setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

//     struct sockaddr_in address;
//     address.sin_family = AF_INET;
//     address.sin_port = htons(8080);
//     address.sin_addr.s_addr = INADDR_ANY;

//     if (::bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
//         perror("❌ Bind failed"); // هنا غيقول ليك السبب
//         return 1;
//     }

//     if (listen(server_fd, 5) < 0) {
//         perror("Listen failed");
//         return 1;
//     }

//     cout << "✅ Server is LISTENING on port 8080... (waiting)" << endl;

//     // ... Accept code here
//     int client_fd = accept(server_fd, nullptr, nullptr);
//     std::cout << "client_fd: " << client_fd << endl;
//     if (client_fd < 0) {
//         perror("Accept failed");
//         return 1;
//     }
    
//     cout << "✅ Client connected!" << endl;
    
//     char buffer[1024] = {0};
//     recv(client_fd, buffer, 1024, 0);
//     cout << "Message: " << buffer << endl;
    
//     close(client_fd);
//     close(server_fd);
//     return 0;
// }








#include <iostream>
#include <vector>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <fcntl.h>
#include <poll.h>      // مكتبة poll الضرورية
#include <cstdio>

using namespace std;

int main() {
    // 1. Setup Server
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1) { perror("Socket failed"); return 1; }

    // Non-blocking mode ضروري للسوجي
    fcntl(server_fd, F_SETFL, O_NONBLOCK); 

    int opt = 1;
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0){perror("setsockopt failed"); return 1;}

    struct sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_port = htons(8080);
    address.sin_addr.s_addr = INADDR_ANY;

    if (::bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) { // bind the socket to the port 8080 
        perror("Bind failed"); return 1;
    }

    if (listen(server_fd, 10) < 0) { //this function make the socket listen to the port for incoming connections
        perror("Listen failed"); return 1;
    }

    // 2. Poll Preparation
    // غنصاوبو لائحة فيها كاع الناس اللي حاضينهم (السيرفر + الكليان)
    std::vector<struct pollfd> fds; // لائحة ديال pollfd pullfd هي structure فيها جوج ديال الحوايج: fd و events
    
    // أول واحد نحضيوه هو السيرفر براسو (باش نعرفو واش شي حد جديد جا)
    struct pollfd server_poll;
    server_poll.fd = server_fd;
    server_poll.events = POLLIN; // كنقولو للبول: علمنا إلا شي حد بغا يدخل (Input)
    fds.push_back(server_poll);

    cout << "✅ Server listening on 8080 using POLL..." << endl;

    // 3. The Main Loop (قلب السيرفر)
    while (true) {
        // poll كتحبس هنا وتتسنا شي حاجة طرا فواحد من fds
        int ret = poll(fds.data(), fds.size(), -1);
        if (ret < 0) { perror("poll error"); break; }

        // كندورو على اللائحة نشوفو شكون تحرك
        for (size_t i = 0; i < fds.size(); i++) {
            
            // واش هاد fd عندو داتا للقراءة؟
            if (fds[i].revents & POLLIN) {
                
                // الحالة 1: السيرفر هو اللي تحرك (يعني كليان جديد)
                if (fds[i].fd == server_fd) {
                    int new_client = accept(server_fd, nullptr, nullptr); // نقبلو الكليان الجديد
                    if (new_client >= 0) {
                        fcntl(new_client, F_SETFL, O_NONBLOCK); // حتى هو نردوه non-blocking
                        
                        // نزيدوه للائحة باش poll يولي يحضيه
                        struct pollfd client_poll;
                        client_poll.fd = new_client;
                        client_poll.events = POLLIN;
                        fds.push_back(client_poll);
                        
                        cout << "New client connected! number : " << new_client - 3 << endl;
                    }
                }
                // الحالة 2: كليان قديم هو اللي تحرك (يعني صيفط ميساج)
                else {
                    char buffer[1024] = {0};
                    long ss = LONG_MAX;
                    unsigned int ii = -1;
                    int bytes = recv(fds[i].fd, buffer, 1024, 0);
                    if (bytes <= 0) {
                        // الكليان مشى أو وقع مشكل
                        close(fds[i].fd);
                        fds.erase(fds.begin() + i); // نحيدوه من اللائحة
                        i--; // نرجعو خطوة باش منزقلوش اللي تابعو
                        cout << "Client disconnected number new_client - 3" << endl;
                    } else {
                        cout << "Message from FD " << fds[i].fd << ": " << buffer << endl;
                    }
                }
            }
        }
    }

    close(server_fd);
    return 0;
}









