/*****************************************************************************************
* FILENAME :        version.h          
*
* DESCRIPTION :
*       Header file to define project specific settings
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
* 
* AUTHOR :    Stephan Wink        START DATE :    01.10.2017
*
*
* REF NO  VERSION DATE    WHO     DETAIL
* 000       16.10         SWI     branched from template
* 001       21.11         SWI     first version with single relay object
*****************************************************************************************/
#ifndef PRJSETTINGS_H
#define PRJSETTINGS_H

/****************************************************************************************/
/* Imported header files: */

/****************************************************************************************/
/* Global constant defines: */
#define FW_IDENTIFIER             "00000FW" // Firmware identification
#define FW_VERSION                "002"     // Firmware Version
#define FW_DESCRIPTION            "template firmware"

#define MQTT_SUB_TOGGLE           "/simple_light/toggle" // command message for toggle command
#define MQTT_SUB_BUTTON           "/simple_light/switch" // command message for button commands
#define MQTT_PUB_LIGHT_STATE      "/simple_light/status" //state of relais
#define MQTT_PAYLOAD_CMD_ON       "ON"
#define MQTT_PAYLOAD_CMD_OFF      "OFF"

#define SERIAL_DEBUG

/****************************************************************************************/
/* Global function like macro defines (to be avoided): */

/****************************************************************************************/
/* Global type definitions (enum, struct, union): */

/****************************************************************************************/
/* Global data allusions (allows type checking, definition in c file): */

/****************************************************************************************/
/* Global function prototypes: */

#endif /* PRJSETTINGS_H */
/****************************************************************************************/
