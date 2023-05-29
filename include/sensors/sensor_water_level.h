#ifndef sensor_water_level_h
#define sensor_water_level_h

#include <Arduino.h>
#include <settings.h>
#include <communication/mqtt_manager.h>
#include <sensors/sensor.h>

class WaterLevelSensor : public MqttObserver, public Sensor
{
    private:
        unsigned long timepoint = 0;
        MqttManager &mqttManager;

    public:
        WaterLevelSensor(MqttManager &manager);
        void setup();
        void loop(unsigned int timeout = 500U);
        void update() override;
        void readSensorValue() override;

    private:
        void publish() override;
};

#endif // sensor_water_level_h