from ParticleCloud import ParticleCloud


class EndpointSync(object):
    def __init__(self, access_token):
        proxy_dict = {
            "proxies": {
                "https": "https://192.168.150.153:8080"
            }
        }
        self.particle = ParticleCloud(username_or_access_token=access_token, **proxy_dict)

    def subscribe(self):
        pass

    def event_callback(self):
        pass

    def get_devices(self):
        devices = self.particle.devices
