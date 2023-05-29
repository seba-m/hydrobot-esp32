#ifndef mqtt_manager_h
#define mqtt_manager_h

#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>

#include <settings.h>

#include <observer/subject.h>
#include <device.h>
#include <actuators/actuator.h>
#include <sensors/sensor.h>

class MqttManager : public Subject
{
    private:
        WiFiClient wifiClient;
        PubSubClient client;
        const char *espId;
        std::vector<Device*> observers;

    public:
        MqttManager();

        void setup();
        void loop();
        void publish(const char *topic, StaticJsonDocument<200> & message);

        void attach(Device *observer) override
        {
            observers.push_back(observer);
        }

        void detach(Device *observer) override
        {
            // Buscar y eliminar el observador de la lista
            auto it = std::find(observers.begin(), observers.end(), observer);
            if (it != observers.end())
            {
                observers.erase(it);
            }
        }

        void notify(char *topic, byte *payload, unsigned int length) override
        {
            if (!isValidMqtt())
            {
                return;
            }

            String message = "";

            for (int i = 0; i < length; i++)
            {
                message.concat(payload[i]);
            }

            if (log_enabled)
            {
                Serial.print("Message arrived in topic: ");
                Serial.println(topic);

                Serial.print("Message:");
                Serial.println(message);

                Serial.println("-----------------------");
            }

            StaticJsonDocument<200> doc;
            DeserializationError error = deserializeJson(doc, message);
            if (error) return;

            String owner = doc["owner"];
            String type = doc["type"];
            String name = doc["name"];

            if (owner != espId) return; //check if message is for this device

            Device *device = getDevice(name);

            if (device == nullptr) return;

            if (type == "sensor")
                handleSensorAction(device, doc);
            else if (type == "actuator")
                handleActuatorAction(device, doc);
        }

        Device *getDevice(String deviceName)
        {
            auto it = std::find_if(observers.begin(), observers.end(), [deviceName](Device *observer) {
                return observer->getDeviceName() == deviceName;
            });

            return *it;
        }

    private:
        static bool isValidWifi();
        static bool isValidMqtt();
        const char *getEspId();
        void connectWiFi();
        void connectMQTT();
        void reconnect();
        void handleSensorAction(Device *device, StaticJsonDocument<200> value);
        void handleActuatorAction(Device *device, StaticJsonDocument<200> value);
};

#endif // mqtt_manager_h