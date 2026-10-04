#include "Vehicle.hpp"

#include <iostream>

Vehicle::Vehicle(const std::string& device_id)
:device_id(device_id){

}

// Vehicle::Vehicle(const std::string& device_id, const std::string& event_id, int64_t timestamp, double latitude, double longitude, double speed, double heading_degrees)
// :device_id(device_id),event_id(event_id),timestamp(timestamp),latitude(latitude),longitude(longitude),speed(speed),heading_degrees(heading_degrees)
// {
// }


void Vehicle::printInfo() const
{
    cout << "Vehicle ID:" << device_id << endl;}


// void Vehicle::printInfo() const
// {
//     cout << "Vehicle ID:" << device_id << 
//     " Event ID:" << event_id <<
//     " Timestamp:" << timestamp <<
//     " Latitude" << latitude <<
//     " Longitude:" << longitude << 
//     " Speed: " << speed <<
//     " Heading: " << heading_degrees << endl;
// }

