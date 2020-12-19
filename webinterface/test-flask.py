#!/usr/bin/python3
import flask
import requests
import argparse

app = flask.Flask("LED-Interface")
app.debug = True

@app.route("/")
def mainPage():
    return flask.render_template("main.html", example="halloo") 

@app.route("/API", methods=['GET', 'POST'])
def modePage():
    mode = "mode_1"
    if (flask.request.method=='POST'):
        mode = flask.request.form["mode"]
    else:
        mode = flask.request.args.get("mode")
    if (mode == "mode_1"):
        requests.post('http://192.168.178.199/', data="0r\n")
    elif (mode == "mode_2"):
        requests.post('http://192.168.178.199/', data="1g\n")
    else:
        mydata = mode+"\n"
        app.logger.info('my mydata is %s and mode is %s\n', mydata, mode)
        requests.post('http://192.168.178.199/', data=mydata)
    app.logger.info('the mode in gereral is %s\n', mode)
    return ("" ,204)


if __name__ == "__main__":
    app.run(host='0.0.0.0')    
