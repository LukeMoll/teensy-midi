import mido

try:
    dev_name = next(filter(lambda s: s.lower().find("teensy") > -1,mido.get_output_names()))
    print("Using device {}".format(dev_name))
except StopIteration as e:
    print("No device matching \"teensy\" found, exiting")
    exit()

from threading import Thread
class CCSetter(Thread):

    def __init__(self, output): 
        Thread.__init__(self)
        self.running = True
        self.output = output
    def stop(self): self.running = False

    def run(self):
        print("CCSetter running!")
        while self.running:
            input()
            msg = mido.Message('control_change')
            msg.control = 5
            msg.value = 64
            self.output.send(msg)

ccs = CCSetter(mido.open_output(dev_name))
ccs.start()

with mido.open_input(dev_name) as port:
    for message in port:
        print(message)