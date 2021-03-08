# Distance Checker
# When something is received on the given channel, the radio strength is checked as value for distance
def on_received_number(receivedNumber):
    if radio.received_packet(RadioPacketProperty.SIGNAL_STRENGTH) > -70:
        basic.show_icon(IconNames.NO)
        basic.pause(100)
        basic.clear_screen()

# Forever loop
def on_forever():
    radio.send_number(0)
    basic.pause(50)

# Radio set-up
radio.set_transmit_power(2)
radio.set_group(1)

radio.on_received_number(on_received_number)
basic.forever(on_forever)
