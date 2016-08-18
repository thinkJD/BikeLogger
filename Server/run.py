import sys
import yaml



from app import app





def main():
   # with open('configuration.yaml', 'r') as config_file:
    #    cfg = yaml.load(config_file)
    #print(cfg)
    #server = TcpServer()
    #server.start_listen(cfg['tcp_server']['host'], cfg['tcp_server']['port'])

    #particle = CloudHandler(cfg['particle_io']['access_token'])
    #particle.get_devices()

    app.run(debug=True)

if __name__ == '__main__':
    main()
