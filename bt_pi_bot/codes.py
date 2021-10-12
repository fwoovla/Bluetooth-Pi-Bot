
#creates object 'gamepad' to store the data you can call it whatever you like

from evdev import InputDevice, categorize, ecodes
from smbus2 import SMBus, i2c_msg

#creates object 'gamepad' to store the data
#you can call it whatever you like
gamepad = InputDevice('/dev/input/event1')

#define gamepad codes
analog_y = 1
analog_x = 0
dpad_y = 17
dpad_x = 16
but_y = 308
but_x = 307
but_a = 304
but_b = 305
#prints out device info at start

print(gamepad)

data = 0
button = 0

#evdev takes care of polling the controller in a loop
for event in gamepad.read_loop():
    if event.type == ecodes.EV_KEY or event.type == ecodes.EV_ABS:
        if event.code == analog_y:
            print("a_y")
            print(event.value)
            button = 0
        if event.code == analog_x:
            print("a_x")
            print(event.value)
	    button = 1
        if event.code == dpad_y:
            print("d_y")
	    print(event.value)
	    button = 2
        if event.code == dpad_x:
            print("d_x")
	    print(event.value)
	    button = 3
        if event.code == but_y:
            print("y")
            print(event.value)
	    button = 4
        if event.code == but_x:
            print("x")
            print(event.value)
	    button = 5
        if event.code == but_a:
            print("a")
            print(event.value)
	    button = 6
        if event.code == but_b:
            print("b")
	    print(event.value)
	    button = 7

        data = int(event.value)

        with SMBus(1) as bus:
            msg=i2c_msg.write(8,[button,data])
            bus.i2c_rdwr(msg)

