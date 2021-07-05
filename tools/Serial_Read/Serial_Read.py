import serial #导入模块
import time
import signal
import sys


def signal_handler(signal,frame):
    Whole_recordFile.close()
    print('You pressed Ctrl+C!')
    sys.exit(0)



Whole_recordFile = open("WholeData.txt","w+")
Whole_recordFile.truncate(0)

try:

  #端口，GNU / Linux上的/ dev / ttyUSB0 等 或 Windows上的 COM3 等
  portx="COM4"
  #波特率，标准值之一：50,75,110,134,150,200,300,600,1200,1800,2400,4800,9600,19200,38400,57600,115200
  bps=230400
  #超时设置,None：永远等待操作，0为立即返回请求结果，其他值为等待超时时间(单位为秒）
  timex=5
  # 打开串口，并得到串口对象
  ser=serial.Serial(portx,bps,timeout=timex)
  print("串口详情参数：", ser)

  print(ser.port)#获取到当前打开的串口名
  print(ser.baudrate)#获取波特率


  rx_count = 0
  signal.signal(signal.SIGINT,signal_handler)
  while True:
         if ser.in_waiting:
             line=ser.read(ser.in_waiting ).decode('utf-8')
             if(line==""):#退出标志
                 break
             else:
                print(line)
                if "AcX" in line:
                    Whole_recordFile.write(line)

                rx_count += 1
                print("rx_count is:")
                print(rx_count)
                print("---------------")
  print("---------------")
  time.sleep()
  ser.close()#关闭串口

except Exception as e:
    print("---异常---：",e)