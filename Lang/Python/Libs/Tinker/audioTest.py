from ast import Try
from importlib.resources import read_text
from msilib.schema import ComboBox
from multiprocessing.sharedctypes import Value
from pydoc import text
from tkinter import *
from tkinter import ttk
import threading
import time
import serial
import serial.tools.list_ports
import re

# pyinstaller -F audioTest.py --noconsole


class LineHzItem(Frame):
    funcGetDateLp = None
    def __init__(self, master=None, Hz=None): 
        super().__init__(master)
        self.master = master
        self.Hz = Hz
        self.pack()
        self.createWidget()
    
    def createWidget(self):
        Button(self, text=str(self.Hz) + " Hz", width=8, command=self.btnClickCb).pack(side="left", padx="3")

        self.e1 = Entry(self, width=6)
        self.e1.pack(side="left", padx="3")

        self.e2 = Entry(self, width=6)
        self.e2.pack(side="left", padx="3")

        self.e3 = Entry(self, width=6)
        self.e3.pack(side="left", padx="3")

    def entryReplace(self, entry, value):
        entry.delete(0, END)
        entry.insert(0, value)

    def btnClickCb(self):
        if self.funcGetDateLp != None:
            lp = self.funcGetDateLp()
            if lp != None and len(lp) == 3:
                self.entryReplace(self.e1, lp[0])
                self.entryReplace(self.e2, lp[1])
                self.entryReplace(self.e3, lp[2])
        print("%.1f Hz" % self.Hz)

class UI(Frame):
    hzTable = [500, 630, 800, 1000, 1250, 1600, 2000, 2500, 3150, 4000, 5000, 6300, 8000, 10000, 12500];
    hzitems = []
    text = None
    thread = None
    isexit = False
    isPortOpened = False
    ser = None
    lp = []

    def __init__(self, master=None): 
        super().__init__(master)
        self.master = master
        self.pack()
        self.bind("<Destroy>", self.on_destroy)
        self.createWidget()
        self.ser = serial.Serial()
        self.ser.baudrate = 115200
        self.ser.timeout = 0.5
        self.port_readlock = threading.Lock()
        self.thread = threading.Thread(target=self.scan_task)
        self.thread2 = threading.Thread(target=self.portGetValuetask)
        self.thread.start()
        self.thread2.start()
        
    def createWidget(self):
        f1 = Frame(self)
        f1.pack(side="left")
        f2 = Frame(self)
        f2.pack(side="right")
        
        self.label = Label(f2, text="")
        self.label.pack(side="top")
        self.text = Text(f2)
        self.text.pack()

        for i in range(0, len(self.hzTable)):
            hzitem = LineHzItem(f1, self.hzTable[i])
            hzitem.funcGetDateLp = self.getLp
            self.hzitems.append(hzitem)

        f3 = Frame(f1)
        f3.pack(side="bottom")

        Button(f3, text="清空", command=self.button_clear).grid(row=0, column=0, padx="3", pady="5")
        Button(f3, text="生成", command=self.button_ok).grid(row=0, column=1, padx="3", pady="5")
        
        self.combobox = ttk.Combobox(f3)
        self.combobox.grid(row=1, column=0, padx="3", pady="5")

        self.btnport = Button(f3, text="打开", command=self.open_port)
        self.btnport.grid(row=1, column=1)
        
        # self.label = Label(f1).pack()

    def getLp(self):
        return self.lp

    def button_ok(self):
        f = None
        try:
            f = open('2.xlsx', 'w')
        except:
            f = None
        
        self.text.delete("1.0", END)
        for i in range(len(self.hzitems)):
            h = self.hzitems[i]
            
            data = (h.e1.get()  + "\t"   +
                    h.e2.get()  + "\t"   +
                    h.e3.get()  + "\n")
            if f:
                f.write(str(h.Hz) + "Hz\t" + data)
            self.text.insert(CURRENT, data)
        
        if f:
            f.close()
    
    def open_port(self):
        self.port_readlock.acquire()
        if self.btnport['text'] == "打开":
            self.ser.port = self.combobox.get()
            try:
                self.ser.open()
            except:
                print("open ser failed")

            if self.ser.isOpen():
                self.isPortOpened = True
                self.port_readlock.release()
                self.btnport['text'] = "关闭"
                self.btnport['bg'] = "green"
                cmdstart = "ana -d\r\nlog -l 0\r\n";
                self.ser.write(cmdstart.encode())
                print('open port ' + self.ser.port + ' ok')
                return

        
        if self.ser.isOpen():
            try:
                self.ser.close()
            except:
                print("close ser failed")
    
        self.isPortOpened = False
        self.port_readlock.release()

        self.btnport['text'] = "打开"
        self.btnport['bg'] = "white"

    def button_clear(self):
        self.text.delete("1.0", END)
        for i in range(len(self.hzitems)):
            h = self.hzitems[i]
            h.e1.delete(0, END)
            h.e2.delete(0, END)
            h.e3.delete(0, END)
    
    def portGetValuetask(self):
        while self.isexit == False:
            self.port_readlock.acquire()
            if self.isPortOpened:
                recv = ''
                try:
                    recv = self.ser.readline()
                except:
                    recv = ''
                    if self.btnport['text'] == "关闭":
                        self.btnport['text'] = "打开"
                        self.btnport['bg'] = "white"
                        self.isPortOpened = False
                        try:
                            self.ser.close()
                        except:
                            print("close ser failed")

                self.port_readlock.release()
                if len(recv) > 0 :
                    data = str(recv,'utf-8')
                    # print(data)
                    # self.text.insert(CURRENT, data)
                    # LAFp    38.2     LCFp    37.4    LZFp   44.4
                    regular = re.findall(r"LAFp[^0-9.]*([0-9.]*).*LCFp[^0-9.]*([0-9.]*).*LZFp[^0-9.]*([0-9.]*).*", data)
                    if len(regular) > 0 and len(regular[0]) == 3:
                        print(data)
                        self.label['text'] = data
                        self.lp = regular[0]
            else:
                self.port_readlock.release()
                time.sleep(1)

    def scan_task(self):
        while self.isexit == False:
            # 扫描串口
            port_list = list(serial.tools.list_ports.comports())
            ports = []
            for port in port_list:
                ports.append(port[0])
            self.combobox['values'] = ports
            time.sleep(1)
    
    def on_destroy(self, event):
        if event.widget != self:
            return
        print("just closed")
        self.isexit = True

def main():
    root = Tk()
    root.geometry("720x620")
    ui = UI(root)
    root.mainloop()


if __name__ == '__main__':
    main()

