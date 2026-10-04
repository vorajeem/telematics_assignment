#ifndef VEHICLE_HPP
#define VEHICLE_HPP

using namespace std;
#include <string>

class Vehicle {
    public:    
        Vehicle(const std::string& device_id);
        // Vehicle(const std::string& device_id, const std::string& event_id, int64_t timestamp, double latitude, double longitude, double speed, double heading_degrees);
        void printInfo() const;
    private:
    string device_id;
};


#endif