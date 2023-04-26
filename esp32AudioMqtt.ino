#include <WiFi.h>
#include <PubSubClient.h>
#include "AudioMqtt.h"
#include "IISAudio.h"
const char* ssid     = "BeeBest";//修改为你的WIFI账号与密码
const char* password = "fengyu0715";
const char* mqtt_server = "124.223.103.23";//这是树莓的MQTT服务器地址

void wifiInit(void)//连接WIFI
{
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
    }
    Serial.println(WiFi.localIP());
}
bool  BtnisPressed(void)//按键是否按下
{
  bool key=digitalRead(BTN);
  if(1==key)
  {
    return 0;
  }
  else
  {
    return 1 ;
  }
}
void setup(void)
{
  Serial.begin(115200);
  wifiInit();

  I2SInit();//I2S初始化 
  Serial.println("I2SInit over!");

  pinMode(BTN, INPUT_PULLUP);//按键
  pinMode(LED, OUTPUT);//指示灯
  digitalWrite(LED,HIGH);
  
  client.setServer(mqtt_server, 1883);//mqtt配置
  client.setCallback(callback);//绑定回调函数
}


bool sendOver=1;//发送完成标志位
bool recOver=0;//接受完成标志位
bool speakOut;//0代表对外讲话，1代表收听

void loop(void)
{
  if (!client.connected()) {//判断是否连接
    reconnect();
  }
  client.loop();
  if(BtnisPressed())//按下按键发射数据
  {
    speakOut=0;
    digitalWrite(LED,HIGH);//发射时开灯
    int samples_read = I2Sread(samples_16bit,128);//读取数据
    covert_bit(samples_16bit,samples_8bit,samples_read);//发送时转换为8位
    sendData(samples_8bit,samples_read);//发射数据
  }
  else
  {
    delay(28);//经过一段延时再判断，接收数据并且播放也需要时间
    speakOut=1;
    if(recOver)
    {
      recOver=0;
      if(digitalRead(LED))//接受数据时闪烁LED
      {
          digitalWrite(LED,LOW);
      }
      else
      {
          digitalWrite(LED,HIGH);
      }
    }
    else
    {
      digitalWrite(LED,LOW);//没有接收到消息，也没有发射，关灯
      i2s_zero_dma_buffer(SPK_I2S_PORT);//清空DMA中缓存的数据，你可以尝试一下不清空（注释这行）是什么效果
    }
  }  
}
