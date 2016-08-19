import os
import yaml


def get_config():
    with open('configuration.yaml', 'r') as config_file:
        cfg = yaml.load(config_file)

    for group in cfg.keys():
        for setting in cfg[group].keys():
            if '{base_path}' in str(cfg[group][setting]):
                cfg[group][setting] = cfg[group][setting].replace('{base_path}', os.path.dirname(__file__) + '/')

    return cfg
