import sys
import yaml
import socket


class TcpServer:
    def __init__(self):
        # create a new streaming socket
        self.sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

    def start_listen(self, host, port):
        try:
            self.sock.bind((host, port))
            print('socket bind complete')
        except:
            print('tcp socket bind failed')
            sys.exit()

        print('start listening')
        self.sock.listen(1)

        # wait for connection (blocking)
        conn, addr = self.sock.accept()
        print('connected with: ', addr[0], ':', addr[1])
        # Sending message to connected client
        conn.send('Welcome to the bike_logger server\n'.encode('utf8'))

        # infinite loop so that function do not terminate and thread do not end.
        while True:

            # Receiving from client
            data = conn.recv(1024)
            reply = 'OK...' + str(data, encoding='utf8')
            if not data:
                break
            conn.sendall(reply.encode('utf8'))

        # came out of loop
        conn.close()

    def __send_data(self, data):
        pass

    def __receive_data(self):
        pass


def main():
    with open('configuration.yaml', 'r') as config_file:
        cfg = yaml.load(config_file)
    print(cfg)
    server = TcpServer()
    server.start_listen(cfg['tcp_server']['host'], cfg['tcp_server']['port'])



if __name__ == '__main__':
    main()
