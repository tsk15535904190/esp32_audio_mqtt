#ifndef _AUDIOMQTT_
#define _AUDIOMQTT_

#include <PubSubClient.h>
#include <WiFi.h>
#include "IISAudio.h"
extern PubSubClient client;

#if 0

#define LOCALTOPIC "ESP32_SENDER"
#define PUBTOPIC   "ESP32_RECVER"

#else

#define LOCALTOPIC "ESP32_RECVER"
#define PUBTOPIC   "ESP32_SENDER"

#endif

void callback(char* topic, byte* payload, unsigned int length);
void reconnect() ;
void sendData(const uint8_t  *data, uint16_t len);

#endif
