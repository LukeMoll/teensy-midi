from threading import Thread, Event
import curses, re, mido, sys
from queue import Queue

cc = 0
refresh = Event()
refresh.clear()

def main(stdscr, outport):
    global cc
    def draw(stdscr):
        stdscr.clear()
        stdscr.addstr(1,1, "CC #5:", curses.A_BOLD)
        stdscr.addstr(1,8, str(cc))
        stdscr.refresh()
    
    ir = InputReader(stdscr)
    ir.start()

    refresh.set()
    while True:
        refresh.wait(timeout=1)
        draw(stdscr)
        updateMidi(outport)
        refresh.clear()

class InputReader(Thread):
    def __init__(self, stdscr):
        Thread.__init__(self)
        self.stdscr = stdscr

    def run(self):
        global cc
        while True:
            # self.stdscr.addstr(3,1, "["+k.replace("\n",r"\n")+"]", curses.A_STANDOUT);
            k = self.stdscr.getkey()
            if k.lower() == "q": sys.exit()
            elif k == "KEY_UP":     cc = cc + 1 if cc < 127 else 127
            elif k == "KEY_DOWN":   cc = cc - 1 if cc > 0 else 0
            elif k == "KEY_NPAGE":  cc = max(cc - 16, 0)
            elif k == "KEY_PPAGE":  cc = min(cc + 16, 127)
            refresh.set()
        
def draw(stdscr):
    global cc
    stdscr.clear()
    stdscr.addstr(1,1, "CC #5:", curses.A_BOLD)
    stdscr.addstr(1,8, str(cc))
    stdscr.refresh()



def updateMidi(outport):
    msg = mido.Message('control_change')
    msg.control = 5
    msg.value = cc
    outport.send(msg)

def handle_message(msg):
    global cc
    if msg.type == 'control_change' and msg.control == 5:
        cc = msg.value
        refresh.set()

try:
    dev_name = next(filter(lambda s: s.lower().find("teensy") > -1,mido.get_output_names()))
    print("Using device {}".format(dev_name))
except StopIteration as e:
    print("No device matching \"teensy\" found, exiting")
    exit()

with mido.open_output(dev_name) as outport, mido.open_input(dev_name) as inport:
    inport.callback = handle_message
    curses.wrapper(main, outport)