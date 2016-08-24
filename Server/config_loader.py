import os
import yaml

with open('configuration.yaml', 'r') as config_file:
    config = yaml.load(config_file)

for group in config.keys():
    for setting in config[group].keys():
        if '{base_path}' in str(config[group][setting]):
            config[group][setting] = config[group][setting].replace('{base_path}', os.path.dirname(__file__) + '/')