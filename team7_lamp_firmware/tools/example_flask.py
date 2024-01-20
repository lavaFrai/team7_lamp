
import os
import re

import flask
from flask import Flask
from flask import request

from LampController.LampMQTTController import LampMQTTController

app = Flask(__name__)

mqtt_config = {
        "mqtt_login": "admin",
        "mqtt_password": "#\"63Ug@~Rv'~9dL&",

        "mqtt_host": "lavafrai.ml",
        "mqtt_port": 1883,
    }

lamp = LampMQTTController("g5jsb8be6xbek066ertm76bw0xeztlja5b38hdke", mqtt_config)


@app.route("/")
def main():
    with open("flask_example.html") as f:
        r = f.read()

    color = request.args.get('color')
    if color is None:
        color = "#FFFFFF"
    else:
        color = color[1:]
    uid = request.args.get('uid')
    if uid is None:
        uid = "0"
    brightness = request.args.get('brightness')
    if brightness is None:
        brightness = "100"

    # print(color, uid)
    if re.fullmatch(r"[\dA-Fa-f]{6}", color):
        r = r.replace("{{color}}", "#" + color)
    else:
        r = r.replace("{{color}}", "#FFFFFF")

    if re.fullmatch(r"\d+", uid):
        r = r.replace("{{uid}}", str(uid))
    else:
        r = r.replace("{{uid}}", "0")

    if re.fullmatch(r"(100|[1-9][0-9]|[0-9])", brightness):
        r = r.replace("{{brightness}}", str(brightness))
    else:
        r = r.replace("{{brightness}}", "100")

    return r


@app.route('/set-color')
def set_color():
    color = request.args.get('color')[1:]
    uid = request.args.get('uid')
    brightness = request.args.get('brightness')

    print(color, uid)
    if not re.fullmatch(r"[\dA-Fa-f]{6}", color):
        return flask.redirect(f"/?uid={uid}&color=%23{color}&brightness={brightness}", 302, flask.jsonify(status="error"))
    if not re.fullmatch(r"\d+", uid):
        return flask.redirect(f"/?uid={uid}&color=%23{color}&brightness={brightness}", 302, flask.jsonify(status="error"))
    if not re.fullmatch(r"(100|[1-9][0-9]|[0-9])", brightness):
        return flask.redirect(f"/?uid={uid}&color=%23{color}&brightness={brightness}", 302, flask.jsonify(status="error"))

    print(hex(int(int(brightness) / 100 * 255))[2:])
    lamp.set_color(int(uid), color.upper())
    lamp.set_brightness(int(uid), str(hex(int(int(brightness) / 100 * 255))[2:]).upper().zfill(2))
    return flask.redirect(f"/?uid={uid}&color=%23{color}&brightness={brightness}", 302, flask.jsonify(status="ok"))


if __name__ == "__main__":
    app.run(host="0.0.0.0", port=8080)
