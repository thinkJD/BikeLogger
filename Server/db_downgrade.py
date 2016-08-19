from migrate.versioning import api
import load_configuration


cfg = load_configuration.get_config()
SQLALCHEMY_DATABASE_URI = cfg['database']['SQLALCHEMY_DATABASE_URI']
SQLALCHEMY_MIGRATE_REPO = cfg['database']['SQLALCHEMY_MIGRATE_REPO']

v = api.db_version(SQLALCHEMY_DATABASE_URI, SQLALCHEMY_MIGRATE_REPO)
api.downgrade(SQLALCHEMY_DATABASE_URI, SQLALCHEMY_MIGRATE_REPO, v - 1)
v = api.db_version(SQLALCHEMY_DATABASE_URI, SQLALCHEMY_MIGRATE_REPO)
print('Current database version: ' + str(v))