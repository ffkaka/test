#!/usr/bin/python2.7
import serial
import Tkinter
import thread
from time import sleep
import string
import termios, fcntl, sys, os
from datetime import datetime
import signal

if len(sys.argv) > 1:
    key = sys.argv[1].split('|')
    if len(sys.argv) > 2:
        fkey = sys.argv[2].split('|')
    else:
        fkey=''
else:
    key=''

buff=''

def exit_handler(signal, frame):
    print('You Pressed Ctrl+C')
    termios.tcsetattr(fd, termios.TCSANOW, attrs_save)
    sys.exit(0)


class bcolors:
    HEADER = '\033[95m'
    OKBLUE = '\033[94m'
    OKGREEN = '\033[92m'
    YELLOW = '\033[93m'
    RED_FAIL = '\033[1;91m'
    CYAN ='\033[96m'
    ENDC = '\033[0m'

def set_termios(ffd):
    global isTermSet
    attrs = list(attrs_save) # copy the stored version to update attrs[0] |= termios.ICRNL attrs[3] &= ~(termios.ECHO  | termios.ECHONL | termios.ICANON | termios.ISIG)
    #attrs[3] &= ~(termios.ECHONL | termios.ECHO | termios.ICANON
    attrs[3] &= ~(termios.ECHO  | termios.ECHONL | termios.ICANON | termios.ISIG)

    termios.tcsetattr(ffd, termios.TCSANOW, attrs)
    # turn off non-blocking
    fcntl.fcntl(ffd, fcntl.F_SETFL, flags_save & ~os.O_NONBLOCK)
    isTermSet = True


def read_single_keypress(ffd):
    global isTermSet
    if isTermSet == False :
        set_termios(ffd)
    # read a single keystroke
    ret = sys.stdin.read(1) # returns a single character
    #print ret.encode('hex')
    return ret

def writeport(port):
    s_tx=''
    s_tx = read_single_keypress(fd)
    global prev
    if prev == '\x01':
        if s_tx == 'c':
            global isThreadWorking
            isThreadWorking = False
            termios.tcsetattr(fd, termios.TCSANOW, attrs_save)
            print("Exit~~!! Bye")
            return
            #sys.exit(0)
        else:
            prev = ''
    if s_tx == '\x01':
        prev = s_tx

    try:
        #print bcolors.FAIL+s_tx+bcolors.ENDC
        port.write(s_tx)
    except:
        print bcolors.RED_FAIL + "Write Port Error!!!!!!!" + bcolors.ENDC
        isThreadWorking = False


def readport(port):
    #print 'read child process '
    pflag = False
    global isThreadWorking
    try: 
        ch=port.read(1)
        if ch != '\n':
            sys.stdout.write(ch)
            sys.stdout.flush()
        #if ch=='\r' or ch == '\n':
        if ch == '\n':
            sys.stdout.write(bcolors.ENDC + ch + bcolors.CYAN + datetime.now().strftime("[%H:%M:%S.%f]") + bcolors.ENDC + bcolors.YELLOW)

    except:
        print bcolors.RED_FAIL + "Read Port Error!!!!!!!" + bcolors.ENDC
        isThreadWorking = False

def serialTRX(tname, port):
    rv=""
    global isThreadWorking
    global rxExit
    global txExit
    
    while True:
        if tname == "rx":
            readport(port)
        if tname == "tx" :
            writeport(port)
        if isThreadWorking == False:
            break
        else :
            pass
    
    if tname == "rx":
        print bcolors.OKGREEN + "Stop Rx Thread" + bcolors.ENDC
        rxExit = True
    if tname == "tx":
        print bcolors.OKGREEN + "Stop Tx Thread" + bcolors.ENDC
        txExit = True
'''
    except KeyboardInterrupt:
        print("[readlineCR]Closing Attributei: 0x", format(attrs_save[0], '04x'),format(attrs_save[1], '04x'),format(attrs_save[2], '04x') 
'''        


#global variables
isTermSet=False
prev = ''
isThreadWorking = True
rxExit = False
txExit = False
fd = sys.stdin.fileno()
# save old state
flags_save = fcntl.fcntl(fd, fcntl.F_GETFL)
attrs_save = termios.tcgetattr(fd)

# set a exception handler
signal.signal(signal.SIGINT, exit_handler)

#open a serial port
port = serial.Serial('/dev/ttyUSB0', 115200, timeout = 1, writeTimeout = 1, xonxoff=False)
print("Current Attributei: 0x", format(attrs_save[0], '04x'),format(attrs_save[1], '04x'),format(attrs_save[2], '04x') )

try:
    thread.start_new_thread(serialTRX, ("rx", port,))
    thread.start_new_thread(serialTRX, ("tx", port,))
except:
    print "Error: can't make thread"
    exit()

while True:
    if rxExit == True and txExit == True:
        print "Exiting Main!!!!!!"
        break
    else:
        sleep(0.1)
        pass

port.close()


