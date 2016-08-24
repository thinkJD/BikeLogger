from migrate.versioning import api
from config_loader import config


SQLALCHEMY_DATABASE_URI = config['database']['SQLALCHEMY_DATABASE_URI']
SQLALCHEMY_MIGRATE_REPO = config['database']['SQLALCHEMY_MIGRATE_REPO']

v = api.db_version(SQLALCHEMY_DATABASE_URI, SQLALCHEMY_MIGRATE_REPO)
api.downgrade(SQLALCHEMY_DATABASE_URI, SQLALCHEMY_MIGRATE_REPO, v - 1)
v = api.db_version(SQLALCHEMY_DATABASE_URI, SQLALCHEMY_MIGRATE_REPO)
print('Current database version: ' + str(v))