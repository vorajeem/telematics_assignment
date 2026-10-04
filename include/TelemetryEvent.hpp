#ifndef TELEMETRY_EVENT_HPP
#define TELEMETRY_EVENT_HPP

#include <string>
#include <cstdint>

class TelemetryEvent  {

public:
 TelemetryEvent(const std::string& event_id,const std::string& device_id, int64_t timestamp, double latitude, double longitude, double speed, double heading_degrees);
void printInfo() const;
const std::string& getEventId() const;
const std::string& getDeviceId() const;
std::int64_t getTimestamp() const;
double getLatitude() const;
double getLongitude() const;
double getSpeed() const;
double getHeadingDegrees() const;

private:
    std::string event_id;
    std::string device_id;
    std::int64_t timestamp;
    double latitude;
    double longitude;
    double speed;
    double heading_degrees;
};

#endif