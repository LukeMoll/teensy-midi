import mido

try:
    dev_name = next(filter(lambda s: s.lower().find("teensy") > -1,mido.get_output_names()))
    print("Using device {}".format(dev_name))
except StopIteration as e:
    print("No device matching \"teensy\" found, exiting")
    exit()

with mido.open_input(dev_name) as port:
    for message in port:
        print(message)