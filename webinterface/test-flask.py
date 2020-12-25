#!/usr/bin/python3
import flask
import requests
import argparse

app = flask.Flask("LED-Interface")
app.debug = True

#default -> mode0:= AUS
#message = "m000"

@app.route("/")
def mainPage():
    return flask.render_template("main.html", example="halloo")

@app.route("/API")
def modePage():
    message = flask.request.args.get("mode")
    app.logger.info("message: >%s<\n", message)
    requests.post('http://192.168.178.199/', data=message + "\n")
    return ("" ,204)



if __name__ == "__main__":
    app.run(host='0.0.0.0')
