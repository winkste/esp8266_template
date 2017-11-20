/*****************************************************************************************
* FILENAME :        SonoffSwitch.cpp
*
* DESCRIPTION :
*       Implementation of the Sensor interface functions for the
*       Sonoff switch device.
*
* PUBLIC FUNCTIONS :
*
*
* NOTES :
*
*
* Copyright (c) [2017] [Stephan Wink]
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in all
* copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
vAUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*
* AUTHOR :    Stephan Wink        START DATE :    01.10.2017
*
*****************************************************************************************/

/****************************************************************************************/
/* Include Interfaces */
#include "SonoffSwitch.h"

#include "MqttDevice.h"
#include "Trace.h"
#include "PubSubClient.h"

/****************************************************************************************/
/* Local constant defines */

/****************************************************************************************/
/* Local function like makros */

/****************************************************************************************/
/* Local type definitions (enum, struct, union) */

/****************************************************************************************/
/* Public functions (unlimited visibility) */

/**---------------------------------------------------------------------------------------
 * @brief     Constructor for SonoffSwitch
 * @author    winkste
 * @date      20 Okt. 2017
 * @param     p_trace     trace object for info and error messages
 * @return    n/a
*//*-----------------------------------------------------------------------------------*/
SonoffSwitch::SonoffSwitch(Trace *p_trace, uint8_t simpleLightPin_u8, 
                              uint8_t ledPin_u8) : MqttDevice(p_trace)
{
    this->prevTime_u32 = 0;
    this->publications_u16 = 0;
    this->simpleLightPin_u8 = simpleLightPin_u8;
    this->ledPin_u8 = this->ledPin_u8;
    this->simpleLightState_bol  = false;
    this->publishState_bol      = true;
}

/**---------------------------------------------------------------------------------------
 * @brief     Default destructor
 * @author    winkste
 * @date      20 Okt. 2017
 * @return    n/a
*//*-----------------------------------------------------------------------------------*/
SonoffSwitch::~SonoffSwitch()
{
    // TODO Auto-generated destructor stub
}

/**---------------------------------------------------------------------------------------
 * @brief     Initialization of the Sonoff Switch object
 * @author    winkste
 * @date      20 Okt. 2017
 * @return    n/a
*//*-----------------------------------------------------------------------------------*/
void SonoffSwitch::Initialize()
{
    p_trace->println(trace_INFO_MSG, "Sonoff switch initialized");
    pinMode(this->simpleLightPin_u8, OUTPUT);
    this->setSimpleLight();
    pinMode(this->ledPin_u8, OUTPUT);
    this->isInitialized_bol = true;
}

/**---------------------------------------------------------------------------------------
 * @brief     Function call to initialize the MQTT interface for this switch
 * @author    winkste
 * @date      20 Okt. 2017
 * @param     client     mqtt client object
 * @param     dev_p      client device id for building the mqtt topics
 * @return    n/a
*//*-----------------------------------------------------------------------------------*/
void SonoffSwitch::Reconnect(PubSubClient *client_p, const char *dev_p)
{
    if(NULL != client_p)
    {
        this->dev_p = dev_p;
        this->isConnected_bol = true;
        p_trace->println(trace_INFO_MSG, "Sonoff switch reconnected");
        // ... and resubscribe
        // toggle sonoff light
        client_p->subscribe(build_topic(MQTT_SUB_TOGGLE));  
        client_p->loop();
        p_trace->print(trace_INFO_MSG, "[mqtt] subscribed 1: ");
        p_trace->println(trace_PURE_MSG, MQTT_SUB_TOGGLE);
        // change light state with payload
        client_p->subscribe(build_topic(MQTT_SUB_BUTTON));  
        client_p->loop();
        p_trace->print(trace_INFO_MSG, "[mqtt] subscribed 2: ");
        p_trace->println(trace_PURE_MSG, MQTT_SUB_BUTTON);
        client_p->loop();
    }
    else
    {
        // failure, not connected
        p_trace->println(trace_ERROR_MSG, 
                                "uninizialized MQTT client in sonoff switch detected");
        this->isConnected_bol = false;
    }
}

/**---------------------------------------------------------------------------------------
 * @brief     Callback function to process subscribed MQTT publication
 * @author    winkste
 * @date      20 Okt. 2017
 * @param     client     mqtt client object
 * @param     p_topic    received topic
 * @param     p_payload  attached payload message
 * @return    n/a
*//*-----------------------------------------------------------------------------------*/
void SonoffSwitch::CallbackMqtt(PubSubClient *client, char* p_topic, String p_payload)
{
    if(true == this->isConnected_bol)
    {
        // received toggle light mqtt topic
        if (String(build_topic(MQTT_SUB_TOGGLE)).equals(p_topic)) 
        {
            p_trace->println(trace_INFO_MSG, "Sonoff switch mqtt callback");
            p_trace->println(trace_INFO_MSG, p_topic);
            p_trace->println(trace_INFO_MSG, p_payload);
            this->ToggleSimpleLight();
        }
        // execute command to switch on/off the light
        else if (String(build_topic(MQTT_SUB_BUTTON)).equals(p_topic)) 
        {
            p_trace->println(trace_INFO_MSG, "Sonoff switch mqtt callback");
            p_trace->println(trace_INFO_MSG, p_topic);
            p_trace->println(trace_INFO_MSG, p_payload);
            // test if the payload is equal to "ON" or "OFF"
            if(0 == p_payload.indexOf(String(MQTT_PAYLOAD_CMD_ON))) 
            {
                this->simpleLightState_bol = true;
                this->setSimpleLight();  
            }
            else if(0 == p_payload.indexOf(String(MQTT_PAYLOAD_CMD_OFF)))
            {
                this->simpleLightState_bol = false;
                this->setSimpleLight();
            }
            else
            {
                p_trace->print(trace_ERROR_MSG, "[mqtt] unexpected payload: "); 
                p_trace->println(trace_PURE_MSG, p_payload);
            }   
        } 
    }
    else
    {
        p_trace->println(trace_ERROR_MSG, "connection failure in sonoff CallbackMqtt "); 
    }
}

/**---------------------------------------------------------------------------------------
 * @brief     Sending generated publications
 * @author    winkste
 * @date      20 Okt. 2017
 * @param     client     mqtt client object
 * @return    n/a
*//*-----------------------------------------------------------------------------------*/
bool SonoffSwitch::ProcessPublishRequests(PubSubClient *client)
{
    String tPayload;
    boolean ret = false;
    
    if(true == this->isConnected_bol)
    {
        // check if state has changed, than publish this state
        if(true == publishState_bol)
        {
            p_trace->print(trace_INFO_MSG, "[mqtt] publish requested state: ");
            p_trace->print(trace_PURE_MSG, MQTT_PUB_LIGHT_STATE);
            p_trace->print(trace_PURE_MSG, "  :  ");
            if(true == simpleLightState_bol)
            {
              ret = client->publish(build_topic(MQTT_PUB_LIGHT_STATE), 
                                        MQTT_PAYLOAD_CMD_ON, true);
              p_trace->println(trace_PURE_MSG, MQTT_PAYLOAD_CMD_ON);
            }
            else
            {
                ret = client->publish(build_topic(MQTT_PUB_LIGHT_STATE), 
                                          MQTT_PAYLOAD_CMD_OFF, true); 
                p_trace->println(trace_PURE_MSG, MQTT_PAYLOAD_CMD_OFF); 
            } 
            if(ret)
            {
                publishState_bol = false;     
            }
        }
    }
    else
    {
        p_trace->println(trace_ERROR_MSG, 
                              "connection failure in sonoff ProcessPublishRequests "); 
    }
    return ret; 
};

/**---------------------------------------------------------------------------------------
 * @brief     This function toggles the relais
 * @author    winkste
 * @date      20 Okt. 2017
 * @return    void
*//*-----------------------------------------------------------------------------------*/
void SonoffSwitch::ToggleSimpleLight(void)
{
  if(true == this->simpleLightState_bol)
  {   
    this->TurnRelaisOff();  
  }
  else
  {   
    this->TurnRelaisOn();
  }
}

/****************************************************************************************/
/* Private functions: */
/**---------------------------------------------------------------------------------------
 * @brief     This function turns the relais off
 * @author    winkste
 * @date      20 Okt. 2017
 * @return    void
*//*-----------------------------------------------------------------------------------*/
void SonoffSwitch::TurnRelaisOff(void)
{
  if(true == this->isInitialized_bol)
  {
      this->simpleLightState_bol = false;
      digitalWrite(this->simpleLightPin_u8, LOW);
      digitalWrite(this->ledPin_u8, HIGH);
      Serial.println("relais state: OFF");
      Serial.println("request publish");
      this->publishState_bol = true;
  }
}

/**---------------------------------------------------------------------------------------
 * @brief     This function turns the relais on
 * @author    winkste
 * @date      20 Okt. 2017
 * @return    void
*//*-----------------------------------------------------------------------------------*/
void SonoffSwitch::TurnRelaisOn(void)
{
  if(true == this->isInitialized_bol)
  {
      this->simpleLightState_bol = true;
      digitalWrite(this->simpleLightPin_u8, HIGH);
      digitalWrite(this->ledPin_u8, LOW);
      Serial.println("Button state: ON");
      Serial.println("request publish");
      this->publishState_bol = true;
  }
}

/**---------------------------------------------------------------------------------------
 * @brief     This function sets the relay based on the state of the global variable
 *              simpleLightState_bolst
 * @author    winkste
 * @date      20 Okt. 2017
 * @return    n/a
*//*-----------------------------------------------------------------------------------*/
void SonoffSwitch::setSimpleLight(void)
{
  if(true == this->simpleLightState_bol)
  {
    TurnRelaisOn(); 
  }
  else
  {
    TurnRelaisOff(); 
  }
}

/**---------------------------------------------------------------------------------------
 * @brief     This function helps to build the complete topic including the 
 *              custom device.
 * @author    winkste
 * @date      20 Okt. 2017
 * @param     topic       pointer to topic string
 * @return    combined topic as char pointer, it uses buffer_stca to store the topic
*//*-----------------------------------------------------------------------------------*/
char* SonoffSwitch::build_topic(const char *topic) 
{
  sprintf(buffer_ca, "%s%s", this->dev_p, topic);
  return buffer_ca;
}
