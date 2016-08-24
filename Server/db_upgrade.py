from config_loader import config
from migrate.versioning import api
import config_loader


SQLALCHEMY_DATABASE_URI = config['database']['SQLALCHEMY_DATABASE_URI']
SQLALCHEMY_MIGRATE_REPO = config['database']['SQLALCHEMY_MIGRATE_REPO']

api.upgrade(SQLALCHEMY_DATABASE_URI, SQLALCHEMY_MIGRATE_REPO)
v = api.db_version(SQLALCHEMY_DATABASE_URI, SQLALCHEMY_MIGRATE_REPO)
print('Current database version: ' + str(v))