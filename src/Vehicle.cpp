#include "Vehicle.hpp"

#include <iostream>

Vehicle::Vehicle(const std::string& device_id, const std::string& event_id, float timestamp, float latitude, float longitude, float speed, float heading_degrees)
:device_id(device_id),event_id(event_id),timestamp(timestamp),latitude(latitude),longitude(longitude),speed(speed),heading_degrees(heading_degrees)
{
}

void Vehicle::printInfo() const
{
    cout << "Vehicle ID:" << device_id << 
    " Event ID:" << event_id <<
    " Timestamp:" << timestamp <<
    " Latitude" << latitude <<
    " Longitude:" << longitude << 
    " Speed: " << speed <<
    " Heading: " << heading_degrees << endl;
}