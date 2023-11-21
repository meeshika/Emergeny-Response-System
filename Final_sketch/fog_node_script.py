import socket

HOST = "0.0.0.0"  # Bind to all interfaces
PORT = 8080

global data_entry # Declare data_entry as a global variable

# Create a TCP socket
with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
    # Bind the socket to the HOST and PORT
    s.bind((HOST, PORT))

    # Listen for incoming connections
    s.listen(5)
    print("Started Listening")
    # Accept incoming connections
    while True:
        # Accept a connection and return a new socket object
        client_socket, address = s.accept()

        # Receive data from the client
        data = client_socket.recv(1024).decode('utf-8')
        print(f"Received data from client: {data}")

        client_socket.close()