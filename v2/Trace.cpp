/*
 * Trace.cpp
 *
 *  Created on: 13.11.2017
 *      Author: stephan_wink
 */

#include <ESP8266WiFi.h>
#include "Trace.h"

Trace::Trace()
{
    this->isActive_bol = true;
    this->channel_u16 = 0;

}

Trace::Trace(bool isActive_bol)
{
    this->isActive_bol = isActive_bol;
    this->channel_u16 = 0;

}

Trace::Trace(bool isActive_bol, unsigned int channel_u16)
{
    this->isActive_bol = isActive_bol;
    this->channel_u16 = channel_u16;
}

Trace::~Trace()
{
    // TODO Auto-generated destructor stub
}

void Trace::Initialize()
{
    // init the serial
    Serial.begin(115200);
    Serial.println("");
}

void Trace::SetInfo(char * p_msg)
{
    if(true == isActive_bol)
    {
        Serial.println("");
        Serial.print("[info]");
        Serial.print(p_msg);
    }
}

void Trace::SetError(char * p_msg)
{
    if(true == isActive_bol)
    {
        Serial.println("");
        Serial.print("[err]");
        Serial.print(p_msg);
    }
}

void Trace::Append(String msg)
{
    if(true == isActive_bol)
    {
        Serial.print(msg);
    }
}
void Trace::EndMsg(String msg)
{
    if(true == isActive_bol)
    {
        Serial.println(msg);
    }
}

void Trace::print(int type_u16, String msg_str)
{
  prepareMsg(type_u16, msg_str);
  Serial.print(buffer_str);
  
}

void Trace::println(int type_u16, String msg_str)
{
  prepareMsg(type_u16, String(msg_str));
  Serial.println(buffer_str);
}

void Trace::print(int type_u16, char *msg_pc)
{
  prepareMsg(type_u16, String(msg_pc));
  Serial.print(buffer_str);
  
}

void Trace::println(int type_u16, char *msg_pc)
{
  prepareMsg(type_u16, String(msg_pc));
  Serial.println(buffer_str);
}

void Trace::print(int type_u16, uint8_t value_u8)
{
  prepareMsg(type_u16, String(value_u8));
  Serial.print(buffer_str);
}
void Trace::println(int type_u16, uint8_t value_u8)
{
  prepareMsg(type_u16, String(value_u8));
  Serial.println(buffer_str);
}

void Trace::prepareMsg(int type_u16, String msg_str)
{
  //String temp_str = String(p_pc);
  switch(type_u16)
  {
    case 0:
      buffer_str = String(msg_str);
      break;
    case 1:
      buffer_str = String("[INFO]" + msg_str);
      break;
    case 2:
      buffer_str = String("[WARN]" + msg_str);
      break;
    case 3:
    buffer_str = String("[ERROR]" + msg_str);
      break;
    default:
      buffer_str = String(msg_str);
      break;
  }
}

void Trace::PushToChannel()
{
    // not needed in this implementation
}

