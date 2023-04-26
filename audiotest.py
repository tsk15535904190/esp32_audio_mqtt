import paho.mqtt.client as mqtt
import time
import pyaudio
import sys

p = pyaudio.PyAudio()
stream = p.open(format=p.get_format_from_width(1), channels=1, rate=16000, output=True)
timeflag =0
HOST = "124.223.103.23"
PORT = 1883
def client_loop():
    client_id = time.strftime('%Y%m%d%H%M%S',time.localtime(time.time()))
    client = mqtt.Client(client_id)    # ClientId不能重复，所以使用当前时间
    client.on_connect = on_connect
    client.on_message = on_message
    client.connect(HOST, PORT, 15)
    client.loop_forever()
 
def on_connect(client, userdata, flags, rc):
    print("Connected with result code "+str(rc))
    client.subscribe("ESP32_SENDER")
    print("ready!")
 
def on_message(client, userdata, msg):
    global timeflag
    timeflag=timeflag+1 
    sys.stdout.write("\033[F")
    print("\r"+str(timeflag)+" "*10)
    stream.write(msg.payload)
    
 
if __name__ == '__main__':
    client_loop()