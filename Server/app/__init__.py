import os
import yaml
from flask import Flask
from flask_sqlalchemy import SQLAlchemy
from flask_login import LoginManager
from flask_openid import OpenID

# read configuration from yaml file
# todo remove this section its already don in load_configuration
with open('configuration.yaml', 'r') as config_file:
    cfg = yaml.load(config_file)

# do some string replace stuff
for group in cfg.keys():
    for setting in cfg[group].keys():
        if '{base_path}' in str(cfg[group][setting]):
            cfg[group][setting] = cfg[group][setting].replace('{base_path}', os.path.dirname(__file__))

# configure application
app = Flask(__name__)
app.config.update(cfg['flask'])
app.config.update(cfg['database'])
db = SQLAlchemy(app)
lm = LoginManager()
lm.init_app(app)
lm.login_view = 'login'
oid = OpenID(app, app.config['TMP_PATH'])

from app import views, models