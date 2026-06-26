#include <iostream>
#include <vector>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#include "signal_generator.hpp"

int main()
{
    // 建立 socket
    int sock_fd = socket(AF_INET, SOCK_STREAM, 0);

    if (sock_fd < 0) {
        std::cerr << "socket failed\n";
        return 1;
    }

    // 設定 receiver 位址
    sockaddr_in server_addr{};
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8080);

    // Receiver 在同一台電腦，所以用 localhost
    inet_pton(AF_INET, "127.0.0.1",
              &server_addr.sin_addr);

    // 連線到 receiver
    if (connect(sock_fd,
                (sockaddr*)&server_addr,
                sizeof(server_addr)) < 0) {

        std::cerr << "connect failed\n";
        close(sock_fd);
        return 1;
    }

    std::cout << "Connected to receiver\n";

    // 產生訊號
    SignalGenerator generator(
        1000.0,
        50.0,
        1.0);

    auto signal =
        generator.generateSineWave(100);

    // 傳送資料
    for (double sample : signal)
    {
        send(sock_fd,
             &sample,
             sizeof(sample),
             0);

        std::cout
            << "Sent: "
            << sample
            << "\n";

        usleep(1000); // 1 ms
    }

    close(sock_fd);

    return 0;
}