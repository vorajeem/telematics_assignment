#ifndef VEHICLE_HPP
#define VEHICLE_HPP

using namespace std;
#include <string>

class Vehicle {
    public: 
        Vehicle(const string& device_id, const string& event_id, float timestamp, float latitude, float longitude, float speed, float heading_degrees);
        void printInfo() const;

    private:
    string device_id;
    string event_id;
    float latitude;
    float longitude;
    float speed;
    float heading_degrees;
    float timestamp;

};


#endif