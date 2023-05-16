import RPi.GPIO as GPIO
from time import sleep
import serial 
import time

ser = serial.Serial("/dev/ttyS0", 9600)
GPIO.setwarnings(False)
GPIO.setmode(GPIO.BCM)
M1 = 27
M2 = 22	
M3 = 23	
M4 = 24	
pumpa = 10

GPIO.setup(M1, GPIO.OUT, initial=GPIO.HIGH)
GPIO.setup(M2, GPIO.OUT, initial=GPIO.LOW)
GPIO.setup(M3, GPIO.OUT, initial=GPIO.LOW)
GPIO.setup(M4, GPIO.OUT, initial=GPIO.HIGH)
GPIO.setup(pumpa, GPIO.OUT, initial=GPIO.HIGH)

ENa = 12
ENb = 13 
GPIO.setup(ENa, GPIO.OUT)
ENa_pwm = GPIO.PWM(ENa, 50)
ENa_pwm.start(10)
GPIO.setup(ENb, GPIO.OUT)
ENb_pwm = GPIO.PWM(ENb, 50)
ENb_pwm.start(10)

ir1 = 5 
ir2 = 6 
ir3 = 26 
GPIO.setup(ir1, GPIO.IN)
GPIO.setup(ir2, GPIO.IN)
GPIO.setup(ir3, GPIO.IN)

#Funkcije za praćenje linije i zaustavljanje robota
def stopMoving():
    GPIO.output(M1, GPIO.LOW)
    GPIO.output(M2, GPIO.LOW)
    GPIO.output(M3, GPIO.LOW)
    GPIO.output(M4, GPIO.LOW)
    
def followLine(vr_ir1, vr_ir2, vr_ir3):
    #naprijed
    if vr_ir1 == 0 and vr_ir2 == 0 and vr_ir3 == 0:
        GPIO.output(M1, GPIO.HIGH)
        GPIO.output(M2, GPIO.LOW)
        GPIO.output(M3, GPIO.LOW)
        GPIO.output(M4, GPIO.HIGH)
        ENa_pwm.ChangeDutyCycle(35)
        ENb_pwm.ChangeDutyCycle(35)
elif vr_ir1 == 0 and vr_ir2 == 1 and vr_ir3 == 1:
        GPIO.output(M1, GPIO.LOW)
        GPIO.output(M2, GPIO.HIGH)
        GPIO.output(M3, GPIO.LOW)
        GPIO.output(M4, GPIO.HIGH)
        ENa_pwm.ChangeDutyCycle(40)
        ENb_pwm.ChangeDutyCycle(35)

    elif vr_ir1 == 0 and vr_ir2 == 0 and vr_ir3 == 1:
        GPIO.output(M1, GPIO.LOW)
        GPIO.output(M2, GPIO.HIGH)
        GPIO.output(M3, GPIO.LOW)
        GPIO.output(M4, GPIO.HIGH)
        ENa_pwm.ChangeDutyCycle(35)
        ENb_pwm.ChangeDutyCycle(35)

    elif vr_ir1 == 1 and vr_ir2 == 1 and vr_ir3 == 0:
        print("ide jako lijevo")
        GPIO.output(M1, GPIO.HIGH)
        GPIO.output(M2, GPIO.LOW)
        GPIO.output(M3, GPIO.HIGH)
        GPIO.output(M4, GPIO.LOW)
        ENa_pwm.ChangeDutyCycle(35)
        ENb_pwm.ChangeDutyCycle(40)

    elif vr_ir1 == 1 and vr_ir2 == 0 and vr_ir3 == 0:
        GPIO.output(M1, GPIO.HIGH)
        GPIO.output(M2, GPIO.LOW)
        GPIO.output(M3, GPIO.HIGH)
        GPIO.output(M4, GPIO.LOW)
        ENa_pwm.ChangeDutyCycle(35)
        ENb_pwm.ChangeDutyCycle(35)

    elif vr_ir1 == 1 and vr_ir2 == 0 and vr_ir3 == 1:
        GPIO.output(M1, GPIO.HIGH)
        GPIO.output(M2, GPIO.LOW)
        GPIO.output(M3, GPIO.LOW)
        GPIO.output(M4, GPIO.HIGH)
        ENa_pwm.ChangeDutyCycle(35)
        ENb_pwm.ChangeDutyCycle(35)

    elif vr_ir1 == 0 and vr_ir2 == 1 and vr_ir3 == 0:
        GPIO.output(M1, GPIO.LOW)
        GPIO.output(M2, GPIO.LOW)
        GPIO.output(M3, GPIO.LOW)
        GPIO.output(M4, GPIO.LOW)

sleep(1)
while True:
    vr_ir1 = GPIO.input(ir1)
    vr_ir2 = GPIO.input(ir2)
    vr_ir3 = GPIO.input(ir3)
    List = []

    while len(List)<2 :
        List.append(ser.read())

    #Prvi slucaj, ne treba zaljevati nista   
    if List[0]==b'\x13' and List[1]==b'\x15':
        stopMoving()
    #Drugi slucaj, treba zaliti prvu saksiju, drugu ne treba
    elif List[0]==b'\x14'and List[1]==b'\x15':
        if ser.read()==b'\x06':
            followLine(vr_ir1,vr_ir2,vr_ir3)
        elif ser.read()==b'\x05':
            #ZALJEVANJE
            stopMoving()
            time.sleep(4)
            GPIO.output(pumpa, GPIO.LOW)
            time.sleep(4)
            GPIO.output(pumpa, GPIO.HIGH)
            time.sleep()
            followLine(vr_ir1,vr_ir2,vr_ir3)
            time.sleep(2)
            stopMoving()
            List.clear()
        else: 
            followLine(vr_ir1,vr_ir2,vr_ir3)

    #Treci slucaj, treba zaliti drugu saksiju, prvu ne treba
    elif List[0]==b'\x13'and List[1]==b'\x16':
        if ser.read()==b'\x05':
            followLine(vr_ir1,vr_ir2,vr_ir3)
        elif ser.read()==b'\x06':
            #ZALJEVANJE
            stopMoving()
            time.sleep(4)
            GPIO.output(pumpa, GPIO.LOW)
            time.sleep(4)
            GPIO.output(pumpa, GPIO.HIGH)
            time.sleep()
            followLine(vr_ir1,vr_ir2,vr_ir3)
            time.sleep(2)
            stopMoving()
            List.clear()
        else: 
            followLine(vr_ir1,vr_ir2,vr_ir3)
    
    #Cetvrti slucaj, treba zaliti obje saksije
    elif List[0]==b'\x14'and List[1]==b'\x16':
        followLine(vr_ir1,vr_ir2,vr_ir3)
        if ser.read()==b'\x06' or ser.read()==b'\x05':
stopMoving()
            time.sleep(4)
            GPIO.output(pumpa, GPIO.LOW)
            time.sleep(4)
            GPIO.output(pumpa, GPIO.HIGH)
            time.sleep()
            followLine(vr_ir1,vr_ir2,vr_ir3)
            time.sleep(2)
            stopMoving()
            List.clear()
else: 
            followLine(vr_ir1,vr_ir2,vr_ir3)
