# tcp_online-chat
Client-server online chat application on boost asio

### Requirements
- CMake version 3.10 or higher
- C++ compiler with support for C++17 (for example, GCC, Clang or MSVC)
- Boost library version 1.66 or higher

### Steps to launch the project
1. **Clone the repository:**

   ```bash
   git clone https://github.com/ilya-kan07/tcp_online-chat.git
   cd tcp_online_chat
2. **Create a folder for the build:**
   ```bash
   mkdir build
   cd build

3. **Run CMake to generate build files:**
    ```bash
    cmake ..

If errors occur, specify the path to Boost in CMakeLists.txt: set(BOOST_ROOT "C:/[path_to_boost]")

4. **Build the project:**
    ```bash
    cmake --build .

5.  **Run server:**
Start the server in one console

    ```bash
    ./server

6.  **Run client:**
In another console, launch the client

    ```bash
    ./client

7.  **Start chatting:**
Type the messages in the client and press Enter to send them to the server. The server will send messages to all connected clients.
