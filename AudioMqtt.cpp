#include "AudioMqtt.h"

WiFiClient espClient;
PubSubClient client(espClient);

void callback(char* topic, byte* payload, unsigned int length) {
//  Serial.print("Message arrived [");
//  Serial.print(topic);
//  Serial.print("] ");
//  for (int i = 0; i < length; i++) {
//    Serial.print((char)payload[i]);
//  }

  for (int i = 0; i < length; i++)//接收到信息后转换为16bit，补充左右声道，写入到I2S
  {
    recive_16bit[i] = (payload[i] - 128) << 5;
    output_16bit[i * 2 ] = recive_16bit[i];
    output_16bit[i * 2 + 1] = recive_16bit[i];
  }
  
  I2Swrite(output_16bit,length);
}
void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP32Client-";
    clientId += String(random(0xffff), HEX);
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      // ... and resubscribe
      client.subscribe(LOCALTOPIC,0);
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}
void sendData(const uint8_t  *data, uint16_t len)
{
  if(MQTT_CONNECTED==client.state())
  {
    if(false  == client.publish(PUBTOPIC, data,len,0))
    {
      Serial.println("sendfailed");
    }

  }
  else
  {
    Serial.println("not connect");
  }
    
}
