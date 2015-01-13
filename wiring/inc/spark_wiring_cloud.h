/**
 ******************************************************************************
 * @file    spark_wiring_cloud.h
 * @author  Satish Nair, Zachary Crockett, Matthew McGowan
 ******************************************************************************
  Copyright (c) 2013-2015 Spark Labs, Inc.  All rights reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation, either
  version 3 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, see <http://www.gnu.org/licenses/>.
  ******************************************************************************
 */

#include "spark_wiring_string.h"
#include "events.h"
#include "system_cloud.h"
#include "spark_protocol_functions.h"

typedef enum
{
    SLEEP_MODE_WLAN = 0, SLEEP_MODE_DEEP = 1
} Spark_Sleep_TypeDef;

typedef enum
{
  MY_DEVICES
} Spark_Subscription_Scope_TypeDef;


class SparkClass {
public:
    static void variable(const char *varKey, void *userVar, Spark_Data_TypeDef userVarType) 
    {
        spark_variable(varKey, userVar, userVarType);
    }
    static void function(const char *funcKey, int (*pFunc)(String paramString)) 
    {
        spark_function(funcKey, pFunc);
    }

    void publish(const char *eventName)
    {
        spark_protocol_send_event(sp(), eventName, NULL, 60, EventType::PUBLIC);
    }

    void publish(const char *eventName, const char *eventData)
    {
        spark_protocol_send_event(sp(), eventName, eventData, 60, EventType::PUBLIC);
    }

    void publish(const char *eventName, const char *eventData, int ttl)
    {
        spark_protocol_send_event(sp(), eventName, eventData, ttl, EventType::PUBLIC);
    }

    void publish(const char *eventName, const char *eventData, int ttl, Spark_Event_TypeDef eventType)
    {
        spark_protocol_send_event(sp(), eventName, eventData, ttl, (eventType ? EventType::PRIVATE : EventType::PUBLIC));
    }

    void publish(String eventName)
    {
        publish(eventName.c_str());
    }

    void publish(String eventName, String eventData)
    {
        publish(eventName.c_str(), eventData.c_str());
    }

    void publish(String eventName, String eventData, int ttl)
    {
        publish(eventName.c_str(), eventData.c_str(), ttl);
    }

    void publish(String eventName, String eventData, int ttl, Spark_Event_TypeDef eventType)
    {
        publish(eventName.c_str(), eventData.c_str(), ttl, eventType);
    }

    bool subscribe(const char *eventName, EventHandler handler)
    {
        bool success = spark_protocol_add_event_handler(sp(), eventName, handler);
        if (success)
        {
          success = spark_protocol_send_subscription_scope(sp(), eventName, SubscriptionScope::FIREHOSE);
        }
        return success;
    }

    bool subscribe(const char *eventName, EventHandler handler, Spark_Subscription_Scope_TypeDef scope)
    {
        bool success = spark_protocol_add_event_handler(sp(), eventName, handler);
        if (success)
        {
            success = spark_protocol_send_subscription_scope(sp(), eventName, SubscriptionScope::MY_DEVICES);
        }
        return success;
    }

    bool subscribe(const char *eventName, EventHandler handler, const char *deviceID)
    {
        bool success = spark_protocol_add_event_handler(sp(), eventName, handler);
        if (success)
        {
            success = spark_protocol_send_subscription_device(sp(), eventName, deviceID);
        }
        return success;
    }

    bool subscribe(String eventName, EventHandler handler)
    {
        return subscribe(eventName.c_str(), handler);
    }

    bool subscribe(String eventName, EventHandler handler, Spark_Subscription_Scope_TypeDef scope)
    {
        return subscribe(eventName.c_str(), handler, scope);
    }

    bool subscribe(String eventName, EventHandler handler, String deviceID)
    {
        return subscribe(eventName.c_str(), handler, deviceID.c_str());
    }

    void syncTime(void)
    {
        spark_protocol_send_time_request(sp());
    }


    static void sleep(Spark_Sleep_TypeDef sleepMode, long seconds);
    static void sleep(long seconds);
    static void sleep(uint16_t wakeUpPin, uint16_t edgeTriggerMode);
    static void sleep(uint16_t wakeUpPin, uint16_t edgeTriggerMode, long seconds);
    static bool connected(void);
    static void connect(void);
    static void disconnect(void);
    static void process(void);
    static String deviceID(void);
    
private:

    SparkProtocol* sp() { return spark_protocol_instance(); }
};


extern SparkClass Spark;