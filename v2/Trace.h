/*
 * Trace.h
 *
 *  Created on: 13.11.2017
 *      Author: stephan_wink
 */

#ifndef TRACE_H_
#define TRACE_H_


#include <ESP8266WiFi.h>

class Trace
{
    public:
        const int PURE_MSG  = 0;
        const int INFO_MSG  = 1;
        const int WARN_MSG  = 2;
        const int ERROR_MSG = 3;

        

        Trace();
        Trace(bool isActive_bol);
        Trace(bool isActive_bol, unsigned int channel_u16);
        void SetInfo(char * p_msg);
        void SetError(char * p_msg);
        void Append(String msg);
        void EndMsg(String msg);
        void PushToChannel();
        void Initialize();
        virtual ~Trace();

        void print(int type_u16, char * msg_pc);
        void println(int type_u16, char * msg_pc);
        void print(int type_u16, String msg_str);
        void println(int type_u16, String msg_str);
        void print(int type_u16, uint8_t value_u8);
        void println(int type_u16, uint8_t value_u8);

        /*void print(int type_u16, unsigned long data_u32);
        void println(int type_16, unsigned long data_u32);

        void print(int type_u16, float32_t data_f32);
        void println(int type_16, float32_t data_f32);*/

    private:
        bool isActive_bol;
        unsigned int channel_u16;
        String buffer_str;
        
        void prepareMsg(int type_u16, String msg_str);
};

#endif /* TRACE_H_ */
