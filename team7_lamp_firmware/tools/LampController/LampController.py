import sys

from .LampCryptoModule import LampCryptoModule
import paho.mqtt.client as mqtt


class LampController:
    def __init__(self, cert, broker="host.lavafrai.ml", port=1883, login="smarthomerussia", password="smarthomerussia"):
        self.cert = cert
        self.crypto_module = LampCryptoModule(cert)

        self.broker = broker
        self.port = port
        self.login = login
        self.password = password

        self.mqtt_client = mqtt.Client(client_id="server",
                                       transport='tcp',
                                       protocol=mqtt.MQTTv311,
                                       clean_session=True,
                                       userdata=None)
        self.mqtt_client.username_pw_set(login, password)

        self.mqtt_client.on_connect = self.on_connect
        self.mqtt_client.on_message = self.on_message

        self.mqtt_client.connect(broker, port, 60)

    def on_connect(self, client, userdata, flags, rc):
        print("Connected with result code " + str(rc))
        self.mqtt_client.subscribe("outTopic")

    def on_message(self, client, userdata, msg):
        print(msg.topic + " " + str(msg.payload))

    def loop_forever(self):
        self.mqtt_client.publish("inTopic", "Hello world!")
        while True:
            self.mqtt_client.loop()


if __name__ == "__main__":
    controller = LampController("g5jsb8be6xbek066ertm76bw0xeztlja5b38hdke")
    controller.loop_forever()
