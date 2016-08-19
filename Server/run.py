import os
import yaml
from app import app


def main():
    with open('configuration.yaml', 'r') as config_file:
        cfg = yaml.load(config_file)

    for group in cfg.keys():
        for setting in cfg[group].keys():
            if '{base_path}' in str(cfg[group][setting]):
                cfg[group][setting] = cfg[group][setting].replace('{base_path}', os.path.dirname(__file__))

    print(cfg)


    #server = TcpServer()
    #server.start_listen(cfg['tcp_server']['host'], cfg['tcp_server']['port'])
    app.config.update(cfg['flask'])
    app.run(debug=True)


if __name__ == '__main__':
    main()
