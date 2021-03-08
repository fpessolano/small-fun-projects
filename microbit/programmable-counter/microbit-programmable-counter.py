# Programmable Stop Counter

mode = 0
startStop = 0
step = 0
currentTime = 0
currentTime = 30
step = 5
startStop = 0
mode = 0

# increase step and counter
def on_button_pressed_a():
    global currentTime, step, startStop
    if mode == 1:
        if startStop == 0:
            currentTime += step
    elif mode == 0:
        step += 1
    else:
        if startStop == 0:
            startStop = 1
        basic.pause(500)
    basic.pause(500)

# decrease step and counter
def on_button_pressed_b():
    global currentTime, step, startStop
    if mode == 1:
        if startStop == 0:
            currentTime += -1 * step
    elif mode == 0:
        step += -1
    else:
        if startStop == 1:
            startStop = 0
        basic.pause(500)
    basic.pause(500)

# move to the next step (step to counter, from counter to start
def on_button_pressed_ab():
    global mode
    if mode < 2:
        mode += 1
        basic.pause(500)


# reset the counter on shake
def on_gesture_shake():
    global mode, startStop, currentTime
    basic.clear_screen()
    mode = 0
    startStop = 0
    currentTime = 30
    basic.pause(500)

# forever cycle
def on_forever():
    global startStop, currentTime
    if mode == 1:
        for index in range(9):
            Kitronik_VIEWTEXT32.show_string("Timer is " + str(currentTime) + "s")
            basic.pause(100)
    elif mode == 0:
        for index2 in range(9):
            Kitronik_VIEWTEXT32.show_string("Step is " + str(step) + "s")
            basic.pause(100)
    else:
        for index3 in range(9):
            if currentTime == 0:
                Kitronik_VIEWTEXT32.show_string("Time is up!!")
                basic.show_icon(IconNames.HAPPY)
            else:
                Kitronik_VIEWTEXT32.show_string("Still " + str(currentTime) + "s")
            basic.pause(100)
        if currentTime == 0 and startStop != 0:
            startStop = 0
        else:
            if startStop > 0:
                currentTime += -1 * startStop

input.on_button_pressed(Button.A, on_button_pressed_a)
input.on_button_pressed(Button.AB, on_button_pressed_ab)
input.on_button_pressed(Button.B, on_button_pressed_b)
input.on_gesture(Gesture.SHAKE, on_gesture_shake)

basic.forever(on_forever)