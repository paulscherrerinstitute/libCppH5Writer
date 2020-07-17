#!/bin/env python

from flask import Flask, request, jsonify
import requests
import json
import subprocess
import sys

tomcat_pco_writer = '/home/dbe/git/lib_cpp_h5_writer/tomcat/bin/tomcat_h5_writer'

default_args = ['connection_address', 'output_file', 'n_frames', 'user_id', 'n_modules', 'rest_api_port', 'dataset_name', 'max_frames_per_file', 'statistics_monitor_address']

app = Flask(__name__)

def validate_start_parameters(json_parameters):
    data = json.loads(json_parameters)
    for argument in default_args:
        if argument not in data:
            value = "Argument %s missing on the configuration file. Please, check the configuration template file." % argument
            return json.loads({'success':False, 'value':value})
    return {'success':True, 'value':"OK"}

def validate_response_from_writer(writer_response):
    if writer_response['status'] != "receiving":
        print("\nWriter is not receiving. Current status: %s.\n" % writer_response['status'])
    else:
        msg = "\nWriter is not running. Please start it first using:\n      $ pco_rclient start <path/to/config.pco>.\n"
        return msg

@app.route('/start_pco_writer', methods=['GET', 'POST'])
def start_pco_writer():
    if request.method == 'POST':
        request_json = request.data.decode()
        response = validate_start_parameters(request_json)
        if response["value"] == "OK":
            tomcat_args = [tomcat_pco_writer]
            args= json.loads(request_json)
            for key in default_args:
                tomcat_args.append(args[key])
            #p = subprocess.run(tomcat_args)
            print(tomcat_args)
            p = subprocess.Popen(tomcat_args,shell=False,stdin=None,stdout=None,stderr=None,close_fds=True)
    return response

@app.route('/status', methods=['GET', 'POST'])
def get_status():
    if request.method == 'GET':
        request_url = 'http://xbl-daq-32:9555/status'
        try:
            response = requests.get(request_url).json()
            return validate_response(response)
        except Exception as e:
            msg = "\nWriter is not running. Please start it first using:\n      $ pco_rclient start <path/to/config.pco>.\n"
            return {'success':True, 'value':msg}

if __name__ == '__main__':
    app.run(debug=True, host='0.0.0.0', port=9901)