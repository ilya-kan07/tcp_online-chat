#pragma once
#ifndef CLIENT_H
#define CLIENT_H

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
    async_tcp_client(io::io_context& io_context, const std::string& server, const std::string& port);

    void start();

    void send_message(const std::string& message);

private:
    void async_connect(const tcp::resolver::results_type& endpoints);

    void async_write();

    void async_read();

    tcp::socket socket_;
    tcp::resolver resolver_;
    boost::asio::streambuf buffer_;
    std::deque<std::string> outgoing_messages_;
};

#endif
