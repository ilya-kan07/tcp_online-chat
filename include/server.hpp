#pragma once
#ifndef SERVER_H
#define SERVER_H

#include <boost/asio.hpp>
#include <optional>
#include <queue>
#include <unordered_set>
#include <iostream>

namespace io = boost::asio;
using tcp = io::ip::tcp;
using error_code = boost::system::error_code;
using namespace std::placeholders;

using message_handler = std::function<void(std::string)>;
using error_handler = std::function<void()>;

class session : public std::enable_shared_from_this<session> {
public:
    session(tcp::socket&& socket);

    void start(message_handler&& on_message, error_handler&& on_error);

    void post(const std::string& message);

private:
    void async_read();

    void on_read(error_code error, std::size_t bytes_transferred);

    void async_write();

    void on_write(error_code error, std::size_t bytes_transferred);

    void on_error(const error_code& error);

    tcp::socket socket;
    io::streambuf streambuf;
    std::queue<std::string> outgoing;
    message_handler on_message;
    error_handler on_error_handler;
};

class server {
public:
    server(io::io_context& io_context, std::uint16_t port);

    void async_accept();

    void post(const std::string& message);

private:
    io::io_context& io_context;
    tcp::acceptor acceptor;
    std::unordered_set<std::shared_ptr<session>> clients;
};

#endif
