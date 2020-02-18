#!/usr/bin/env python3

import socket
import json

HOST = '127.0.0.1'  # Standard loopback interface address (localhost)
PORT = 11111        # Port to listen on (non-privileged ports are > 1023)

rokoko_frame_json = {}


with open("rokoko_input.txt", "r") as inp:
    rokoko_frame_json = json.loads(inp.read())


def update_frame_data():
    rokoko_frame_json['props'][0]['position']['x'] = rokoko_frame_json['props'][0]['position']['x'] + 0.0001
    rokoko_frame_json['props'][0]['position']['y'] = rokoko_frame_json['props'][0]['position']['y'] + 0.0001
    rokoko_frame_json['props'][0]['position']['z'] = rokoko_frame_json['props'][0]['position']['z'] + 0.0001


with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
    print('BIND')
    s.bind((HOST, PORT))
    print('LISTEN')
    s.listen()
    print('ACCEPT')
    conn, addr = s.accept()
    print((conn, addr))
    print('SEND DATA')
    with conn:
        print('Connected by', addr)
        while True:
            print('sendall')

            update_frame_data()

            serialized_data = json.dumps(rokoko_frame_json)
            send_res = conn.sendall(serialized_data.encode())

            if send_res is None:
                print('succ')

