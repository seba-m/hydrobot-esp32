#ifndef sensor_temperature_h
#define sensor_temperature_h

#include <OneWire.h>
#include <DallasTemperature.h>
#include <Arduino.h>
#include <settings.h>
#include <communication/mqtt_manager.h>
#include <sensors/sensor.h>

class TemperatureSensor : public MqttObserver, public Sensor
{
    private:
        OneWire ds;
        unsigned long timepoint = 0;
        MqttManager &mqttManager;
        OneWire ourWire;
        DallasTemperature sensors;

    public:
        TemperatureSensor(MqttManager &manager);
        void setup();
        void loop(unsigned int timeout = 500U);
        void readSensorValue() override;
        void publish() override;
        void update() override;
};

#endif // sensor_temperature_h