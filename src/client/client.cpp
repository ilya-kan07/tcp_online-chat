#include "..\..\include\client.hpp"

async_tcp_client::async_tcp_client(io::io_context& io_context, const std::string& server, const std::string& port)
    : socket_(io_context), resolver_(io_context) {};

void async_tcp_client::start() {
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

void async_tcp_client::send_message(const std::string& message) {
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

void async_tcp_client::async_connect(const tcp::resolver::results_type& endpoints) {
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

void async_tcp_client::async_write() {
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

void async_tcp_client::async_read() {
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
