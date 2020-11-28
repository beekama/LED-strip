#!/usr/bin/python3
import flask
import requests
import argparse

app = flask.Flask("LOLOL")


@app.route("/")
def mainPage():
    return flask.render_template("main.html", example="halloo") 

@app.route("/API")
def modePage():
    mode = flask.request.args.get("mode")
    if (mode == "mode_1"):
        requests.post('http://192.168.178.199/', data="Ar\n")
    else:
        requests.post('http://192.168.178.199/', data="Bg\n")
    return (204, "")

if __name__ == "__main__":
    app.run(host='0.0.0.0')    
