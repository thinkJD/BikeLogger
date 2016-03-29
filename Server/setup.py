try:
    from setuptools import setup
except ImportError:
    from distutils.core import setup

config = {
    'description': 'LogHandler server app',
    'author': 'Jan-Daniel Georgens',
    'url': 'http://www.thinkjd.de',
    'download_url': 'nowhere',
    'author_email': 'jd.georgens@gmail.com',
    'version': '0.0.1',
    'install_requires': ['nose'],
    'packages': ['LogHandler'],
    'scripts': [],
    'name': 'BikeLoggerServer'
    }

setup(**config)
