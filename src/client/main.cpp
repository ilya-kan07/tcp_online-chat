#include "..\..\include\client\client.hpp"

int main() {
    try {
        io::io_context io_context;

        auto client = std::make_shared<async_tcp_client>(io_context, "127.0.0.1", "15001");
        client->start();

        std::thread io_thread([&io_context]() { io_context.run(); });

        std::string message;
        while (std::getline(std::cin, message)) {
            client->send_message(message);
        }

        io_thread.join();
    }
    catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << "\n";
    }

    return 0;
}
