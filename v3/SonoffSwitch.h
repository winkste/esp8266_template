/*****************************************************************************************
* FILENAME :        SonoffSwitch.h
*
* DESCRIPTION :
*       Class header for SonoffSwitch
*
* PUBLIC FUNCTIONS :
*       N/A
*
* NOTES :
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
*****************************************************************************************/
#ifndef SONOFFSWITCH_H_
#define SONOFFSWITCH_H_

/****************************************************************************************/
/* Imported header files: */

#include "MqttDevice.h"
#include "Trace.h"
#include "PubSubClient.h"

#include <ESP8266WiFi.h>         
#include <PubSubClient.h>

#include "prjsettings.h"

/****************************************************************************************/
/* Global constant defines: */

/****************************************************************************************/
/* Global function like macro defines (to be avoided): */

/****************************************************************************************/
/* Global type definitions (enum, struct, union): */

/****************************************************************************************/
/* Class definition: */
class SonoffSwitch : public MqttDevice
{
    public:
        /********************************************************************************/
        /* Public data definitions */

        /********************************************************************************/
        /* Public function definitions: */
        SonoffSwitch(Trace *p_trace, uint8_t simpleLightPin_u8, uint8_t ledPin_u8);
        // virtual functions, implementation in derived classes
        bool ProcessPublishRequests(PubSubClient *client);
        void CallbackMqtt(PubSubClient *client, char* p_topic, String p_payload);
        void Initialize();
        void Reconnect(PubSubClient *client_p, const char *dev_p);
        void ToggleSimpleLight(void);
        char* build_topic(const char *topic) ;
        virtual
        ~SonoffSwitch();
    private:
        /********************************************************************************/
        /* Private data definitions */
        boolean simpleLightState_bol  = false;
        boolean publishState_bol      = true;
        char buffer_ca[100];
        uint8_t simpleLightPin_u8     = 0;
        uint8_t ledPin_u8             = 0;
        /********************************************************************************/
        /* Private function definitions: */
        void TurnRelaisOff(void);
        void TurnRelaisOn(void);
        void setSimpleLight(void);
    protected:
        /********************************************************************************/
        /* Protected data definitions */

        /********************************************************************************/
        /* Protected function definitions: */

};

#endif /* SONOFFSWITCH_H_ */
