#ifndef sensor_dht_h
#define sensor_dht_h

#include <Arduino.h>
#include <settings.h>
#include <DHT.h>
#include <communication/mqtt_manager.h>
#include <sensors/sensor.h>

class DHTSensor : public Sensor
{
    private:
        unsigned long timepoint = 0;
        DHT *dht;
        MqttManager &mqttManager;

    public:
        DHTSensor(MqttManager &manager);
        ~DHTSensor();

        void setup(int pin = -1, int sensorType = DHT11, String name = "dht");
        void loop(unsigned int timeout = 2000U);
        void update(StaticJsonDocument<200> doc) override;
        void readSensorValue() override;

    private:
        void publish() override;
};

#endif // sensor_dht_h