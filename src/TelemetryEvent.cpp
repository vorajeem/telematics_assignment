#include <TelemetryEvent.hpp>


#include <iostream>
#include <cstdint>

TelemetryEvent::TelemetryEvent(
    const std::string& event_id, 
    const std::string& device_id, 
    int64_t timestamp, 
    double latitude, 
    double longitude, 
    double speed, 
    double heading_degrees)
:event_id(event_id),
device_id(device_id),
timestamp(timestamp),
latitude(latitude),
longitude(longitude),
speed(speed),
heading_degrees(heading_degrees)
{
}

void TelemetryEvent::printInfo() const
{
    std::cout << "Event ID: " << event_id <<
    "\nVehicle ID: " << device_id << 
    "\nTimestamp: " << timestamp <<
    "\nLatitude: " << latitude <<
    "\nLongitude: " << longitude << 
    "\nSpeed: " << speed <<
    "\nHeading: " << heading_degrees << 
    "\n" << std::endl;
}