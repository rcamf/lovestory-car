from inputs import devices
import math

def main(gamepad=None):
    if not gamepad:
        gamepad = devices.gamepads[0]
    while True:
        for event in gamepad.read():
            if event.code == 'ABS_X':
                steer(event.state)
            elif event.code == 'ABS_RZ':
                acceleration(event.state)

def steer(value=0):
    angle = 90
    if value >= 0:
        angle += math.ceil(30*value/32512)
    else:
        angle += math.floor(30*value/32768)
    print('Steering:' ,angle)


def acceleration(value=0):
    print('Gas:', math.ceil(965*value/255))


if __name__ == "__main__":
    main()
