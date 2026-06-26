#include <iostream>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

int main()
{
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);

    if (server_fd < 0) {
        std::cerr << "socket failed\n";
        return 1;
    }

    sockaddr_in address{};
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(8080);

    if (bind(server_fd, (sockaddr*)&address, sizeof(address)) < 0) {
        std::cerr << "bind failed\n";
        close(server_fd);
        return 1;
    }

    if (listen(server_fd, 1) < 0) {
        std::cerr << "listen failed\n";
        close(server_fd);
        return 1;
    }

    std::cout << "Receiver waiting on port 8080...\n";

    int client_fd = accept(server_fd, nullptr, nullptr);

    if (client_fd < 0) {
        std::cerr << "accept failed\n";
        close(server_fd);
        return 1;
    }

    std::cout << "Sender connected.\n";

    double sample = 0.0;

    while (true) {
        ssize_t bytes = recv(client_fd, &sample, sizeof(sample), 0);

        if (bytes <= 0) {
            break;
        }

        std::cout << "Received sample: " << sample << "\n";
    }

    close(client_fd);
    close(server_fd);

    return 0;
}