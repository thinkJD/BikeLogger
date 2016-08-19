from migrate.versioning import api
import load_configuration
import os
from app import db

cfg = load_configuration.get_config()
SQLALCHEMY_DATABASE_URI = cfg['database']['SQLALCHEMY_DATABASE_URI']
SQLALCHEMY_MIGRATE_REPO = cfg['database']['SQLALCHEMY_MIGRATE_REPO']

db.create_all()
if not os.path.exists(SQLALCHEMY_MIGRATE_REPO):
    api.create(SQLALCHEMY_MIGRATE_REPO, 'database repository')
    api.version_control(SQLALCHEMY_DATABASE_URI, SQLALCHEMY_MIGRATE_REPO)
else:
    api.version_control(SQLALCHEMY_DATABASE_URI, SQLALCHEMY_MIGRATE_REPO, api.version(SQLALCHEMY_MIGRATE_REPO))