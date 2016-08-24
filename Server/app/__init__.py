from config_loader import config
from flask import Flask
from flask_sqlalchemy import SQLAlchemy
from flask_login import LoginManager


# configure application
app = Flask(__name__)
app.config.update(config['flask'])
app.config.update(config['database'])

db = SQLAlchemy(app)
lm = LoginManager()
lm.init_app(app)
lm.login_view = 'login'

from app import views, models
