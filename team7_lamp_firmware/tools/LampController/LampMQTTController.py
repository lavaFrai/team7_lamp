import random

from .LampCrypto2 import LampCrypto2
from paho.mqtt import client as mqtt_client


class LampMQTTController:
    def __init__(self, cert, mqtt_config):
        self.crypto = LampCrypto2(cert)

        client_id = f'server-mqtt-{random.randint(0, 100000)}'
        self.mqtt_client = mqtt_client.Client(client_id)
        self.mqtt_client.username_pw_set(mqtt_config["mqtt_login"], mqtt_config["mqtt_password"])
        self.mqtt_client.connect(mqtt_config["mqtt_host"], mqtt_config["mqtt_port"])

    def set_color(self, uid, hex_color):
        self.mqtt_client.publish(f"topic.lamp.{uid}", self.crypto.lamp_color_package(hex_color, uid))

    def set_brightness(self, uid, hex_brightness):
        self.mqtt_client.publish(f"topic.lamp.{uid}", self.crypto.lamp_brightness_package(hex_brightness, uid))


if __name__ == "__main__":
    mqtt_config = {
        "mqtt_login": "admin",
        "mqtt_password": "#\"63Ug@~Rv'~9dL&",

        "mqtt_host": "lavafrai.ml",
        "mqtt_port": 1883,
    }
    controller = LampMQTTController("g5jsb8be6xbek066ertm76bw0xeztlja5b38hdke", mqtt_config)
    controller.set_color(228, "FF00FF")
    controller.set_brightness(228, "10")
    pass
