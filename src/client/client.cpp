#include <boost/asio.hpp>
#include <iostream>
#include <memory>
#include <thread>
#include <deque>

namespace io = boost::asio;
using tcp = io::ip::tcp;
using error_code = boost::system::error_code;

class async_tcp_client : public std::enable_shared_from_this<async_tcp_client> {
public:
    async_tcp_client(io::io_context& io_context, const std::string& server, const std::string& port)
        : socket_(io_context), resolver_(io_context) {}

    void start() {
        auto self = shared_from_this();

        resolver_.async_resolve("127.0.0.1", "15001",
            [self](error_code ec, tcp::resolver::results_type endpoints) {
                if (!ec) {
                    self->async_connect(endpoints);
                }
                else {
                    std::cerr << "Resolve error: " << ec.message() << std::endl;
                }
            });
    }

    void send_message(const std::string& message) {
        auto self = shared_from_this();

        io::post(socket_.get_executor(),
            [self, message]() {
                bool writing_in_progress = !self->outgoing_messages_.empty();
                self->outgoing_messages_.push_back(message + "\n");

                if (!writing_in_progress) {
                    self->async_write();
                }
            });
    }

private:
    void async_connect(const tcp::resolver::results_type& endpoints) {
        auto self = shared_from_this();

        io::async_connect(socket_, endpoints,
            [self](error_code ec, const tcp::endpoint&) {
                if (!ec) {
                    std::cout << "Connected to the server!" << std::endl;
                    self->async_read();
                }
                else {
                    std::cerr << "Connection error: " << ec.message() << std::endl;
                }
            });
    }

    void async_write() {
        auto self = shared_from_this();

        io::async_write(socket_, io::buffer(outgoing_messages_.front()),
            [self](error_code ec, std::size_t /*length*/) {
                if (!ec) {
                    self->outgoing_messages_.pop_front();
                    if (!self->outgoing_messages_.empty()) {
                        self->async_write();
                    }
                }
                else {
                    std::cerr << "Write error: " << ec.message() << std::endl;
                    self->socket_.close();
                }
            });
    }

    void async_read() {
        auto self = shared_from_this();

        io::async_read_until(socket_, buffer_, "\n",
            [self](error_code ec, std::size_t length) {
                if (!ec) {
                    std::istream is(&self->buffer_);
                    std::string received_message;
                    std::getline(is, received_message);

                    std::cout << "Received: " << received_message << std::endl;
                    self->async_read();
                }
                else {
                    std::cerr << "Read error: " << ec.message() << std::endl;
                    self->socket_.close();
                }
            });
    }

    tcp::socket socket_;
    tcp::resolver resolver_;
    boost::asio::streambuf buffer_;
    std::deque<std::string> outgoing_messages_;
};

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
