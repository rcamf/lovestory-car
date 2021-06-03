from glob import glob
import math
import socket
import struct
from threading import Thread
from time import sleep

from inputs import devices

IP = '192.168.4.1'
PORT = 8000
DELAY_MS = 30

stop = False

# A lock should not be needed as there are no apparent race conditions.
forwards = 0
backwards = 0
steering = 0

def handle_events(gamepad=None):
    global stop
    global forwards
    global backwards
    global steering
    if gamepad is None:
        gamepad = devices.gamepads[0]
    while True:
        for event in gamepad.read():
            if event.code == 'BTN_SELECT':
                stop = True
                return
            elif event.code == 'ABS_X':
                steering = event.state // 256
            elif event.code == 'ABS_RZ':
                forwards = event.state // 2
            elif event.code == 'ABS_Z':
                backwards = event.state // 2

def stream_data():
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
        s.connect((IP, PORT))
        while not stop:
            print(steering, forwards - backwards)
            msg = struct.pack('bb', steering, forwards - backwards)
            s.sendall(msg)
            sleep(DELAY_MS / 1000)

if __name__ == "__main__":
    Thread(target=handle_events).start()
    stream_data()
    