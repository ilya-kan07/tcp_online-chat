#include "..\..\include\server.hpp"

session::session(tcp::socket&& socket) : socket(std::move(socket)) {};

void session::start(message_handler&& on_message, error_handler&& on_error) {
    this->on_message = std::move(on_message);
    this->on_error_handler = std::move(on_error);
    async_read();
}

void session::post(const std::string& message) {
    bool idle = outgoing.empty();
    outgoing.push(message);

    if (idle) {
        async_write();
    }
}

void session::async_read() {
    io::async_read_until(
        socket,
        streambuf,
        "\n",
        [self = shared_from_this()](error_code error, std::size_t bytes_transferred) {
            self->on_read(error, bytes_transferred);
        });
}

void session::on_read(error_code error, std::size_t bytes_transferred) {
    if (!error) {
        std::stringstream message;
        message << socket.remote_endpoint(error) << ": "
            << std::istream(&streambuf).rdbuf();
        streambuf.consume(bytes_transferred);
        on_message(message.str());
        async_read();
        }
    else {
        on_error(error);
    }
}

void session::async_write() {
    io::async_write(
        socket,
        io::buffer(outgoing.front()),
        [self = shared_from_this()](error_code error, std::size_t bytes_transferred) {
            self->on_write(error, bytes_transferred);
        });
}

void session::on_write(error_code error, std::size_t bytes_transferred) {
    if (!error) {
        outgoing.pop();

        if (!outgoing.empty()) {
            async_write();
        }
    }
    else {
        on_error(error);
    }
}

void session::on_error(const error_code& error) {
    std::cerr << "Error in session: " << error.message() << "(code "
        << error.value() << ")" << std::endl;

    if (socket.is_open()) {
        post("Error: " + error.message() + "\n");
    }

    socket.close();

    if (on_error_handler) {
        on_error_handler();
    }
}


server::server(io::io_context& io_context, std::uint16_t port) :
        io_context(io_context),
        acceptor(io_context, tcp::endpoint(tcp::v4(), port)) {}

void server::async_accept() {
    acceptor.async_accept([this](error_code error, tcp::socket new_socket) {
        if (!error) {
            auto client = std::make_shared<session>(std::move(new_socket));
            client->post("Welcome to chat\n\r");
            post("We have a newcomer\n\r");

            clients.insert(client);

            client->start(
                std::bind(&server::post, this, _1),
                [&, weak = std::weak_ptr(client)] {
                    if (auto shared = weak.lock();
                        shared && clients.erase(shared)) {
                        post("We are one less\n\r");
                    }
                });
        } else {
            std::cerr << "Accept error: " << error.message()
                << " (code " << error.value() << ")" << std::endl;
        }
        async_accept();
    });
}

void server::post(const std::string& message) {
    for (auto& client : clients) {
        client->post(message);
    }
}

int main() {
    setlocale(LC_ALL, "Rus");

    try {
        io::io_context io_context;
        server srv(io_context, 15001);
        srv.async_accept();
        io_context.run();
    } catch (const std::exception& e) {
        std::cerr << "Critical server error: " << e.what() << std::endl;
    }
    return 0;
}
