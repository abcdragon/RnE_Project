# print data each seconds to rasp
# 관리자로 켜기!
# 공기청정기는 파랑색 LED 로 표현

from datetime import datetime
import serial as ser
import time

rain = ser.Serial(
    port='COM5',
    baudrate=9600 )
dust = ser.Serial(
    port='COM6', # rasp 기준 /dev/ttyACM0 일듯.. dmesg/tty, dmesg/tail 로 확인
    baudrate=9600 )

rainFlag, dustFlag = True, True
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
        if rain.readable():
            if(rainFlag) : port_chk(rain, rainFlag)
            rainFlag = False
            rainRES = rain.readline().decode('utf-8').strip()
            lastStr += ',' + rainRES
        # Format : datetime,out,in,r,Raining State,Moisture Level,Window State
        data.write(lastStr+'\n')
        print(lastStr)
        time.sleep(1)
                
except KeyboardInterrupt:
    print('The END')
    rain.close()
    dust.close()
    data.close()
'''except UnicodeDecodeError:
    print('UnicodeDecodeError')
    rain.close()
    dust.close()
    data.close()
    
'''
