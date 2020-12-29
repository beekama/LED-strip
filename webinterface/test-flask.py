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
    return flask.render_template("main.html", example="halloo", buttons=buttons, mButtons=modiButtons)

def getMode(intMode):
    return ("000" + str(intMode))[-3]

## color and delay modis: ##
modis = ["einmal", "loop", "fading", "bla"]
buttons = [{'name': modis[k],'mode':'('+ getMode(k) + ')' } for k in range(len(modis))]
# add dicts until numOfDicts%4=0 - for bootstrap-formatting
while (len(buttons)%4 != 0):
    buttons.append({'name': "-",'mode' : '("001")' })

## modes ##
pureModis = ["nature","test1", "test2", "test3", "test4", "test5","test3","test1", "test2", "test3", "test4", "test5","test3", "test1", "test2", "test3", "test4", "test5","test3", "test4", "test5", "test6", "test7", "test8"]
modiButtons = [{'name': pureModis[k],'mode':'('+ getMode(k) + ')' } for k in range(len(pureModis))]
# add dicts until numOfDicts%4=0 - for bootstrap-formatting
while (len(modiButtons)%5 != 0):
    modiButtons.append({'name': "-",'mode' : '("001")' })



@app.route("/API")
def modePage():
    message = flask.request.args.get("mode")
    app.logger.info("message: >%s<\n", message)
    requests.post('http://192.168.178.199/', data=message + "\n")
    return ("" ,204)


if __name__ == "__main__":
    app.run(host='0.0.0.0')
