from datetime import datetime
import serial as ser
import time

dust = ser.Serial(
    port='COM6', # rasp 기준 /dev/ttyACM0 일듯.. dmesg/tty, dmesg/tail 로 확인
    baudrate=9600 )

dustFlag = True
data = open(datetime.now().strftime("%Y-%m-%d %H.%M.%S") + '.txt', 'a') 

def port_chk(port, flag):
    while(flag):
        try:
            port.readline().decode('utf-8')
            flag = False
            break
        except:
            continue
try:
    while True:
        lastStr = ''
        if dust.readable():
            if(dustFlag) : port_chk(dust, dustFlag)
            dustFlag = False
            dustRES = dust.readline().decode('utf-8').strip().split(',')
            dustRES = str(datetime.now()) + ',' + str(dustRES[0]) + ','  + str(dustRES[1])
            lastStr = dustRES
        # Format : datetime,out,in,r,Window State
        data.write(lastStr+'\n')
        print(lastStr)
        time.sleep(1)
                
except KeyboardInterrupt:
    print('The END')
    dust.close()
    data.close()
