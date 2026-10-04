#include "Vehicle.hpp"
#include "TelemetryEvent.hpp"

#include <vector>
#include "simdjson.h"
// using namespace simdjson;


simdjson::dom::parser parser;

int main() {


  std::string json =R"({"event_id":"evt-1000","device_id":"veh-001","ts_ms":1704100740000,"lat":51.5070,"lon":-0.1300,"speed_kph":40.0,"heading_deg":8.5})";
  // cout << "Hello world from Cmake\n\n" <<endl;


  simdjson::dom::element document = parser.parse(json);

  std::string_view event_id_sv = document["event_id"];
  std::string event_id_str{event_id_sv};

  std::string_view device_id_sv = document["device_id"];
  std::string device_id_str{device_id_sv};

  int64_t timestamp = document["ts_ms"];
  double latitude = document["lat"];
  double longitude = document["lon"];
  double speed = document["speed_kph"];
  double heading = document["heading_deg"];


  std::cout << "Event Id: " << event_id_str << '\n';
  std::cout << "device_id: " << device_id_str << '\n';
  std::cout << "timestamp: " << timestamp << '\n';
  std::cout << "latitude: " << latitude << '\n';
  std::cout << "longitude: " << longitude << '\n';
  std::cout << "speed: " << speed << '\n';
  std::cout << "heading: " << heading << '\n';

  TelemetryEvent event_parsed = TelemetryEvent(event_id_str,device_id_str,timestamp, latitude, longitude,speed, heading);
  event_parsed.printInfo();
  std::cout << "XXXXXXXXXXXXXXXX"<< std::endl;
// {"event_id":"evt-1001","device_id":"veh-001","ts_ms":1704100800000,"lat":51.5074,"lon":-0.1278,"speed_kph":48.3,"heading_deg":12.0},
// {"event_id":"evt-1002","device_id":"veh-001","ts_ms":1704100860000,"lat":51.5079,"lon":-0.1201,"speed_kph":52.1,"heading_deg":20.0},
// {"event_id":"evt-2001","device_id":"veh-002","ts_ms":1704100800000,"lat":53.4808,"lon":-2.2426,"speed_kph":35.0,"heading_deg":180.0},
// {"event_id":"evt-1002","device_id":"veh-001","ts_ms":1704100860000,"lat":51.5079,"lon":-0.1201,"speed_kph":52.1,"heading_deg":20.0},
// {"event_id":"evt-1000","device_id":"veh-001","ts_ms":1704100740000,"lat":51.5070,"lon":-0.1300,"speed_kph":40.0,"heading_deg":8.5}
  std::vector<TelemetryEvent> events;
  Vehicle vehicle1 = Vehicle("veh-001");

  // {"event_id":"evt-1001","device_id":"veh-001","ts_ms":1704100800000,"lat":51.5074,"lon":-0.1278,"speed_kph":48.3,"heading_deg":12.0},
  TelemetryEvent event = TelemetryEvent("evt-1001", "veh-001", 1704100800000, 51.5074, -0.1278, 48.3, 12.0);
  event.printInfo();
  events.push_back(event);

  // {"event_id":"evt-1002","device_id":"veh-001","ts_ms":1704100860000,"lat":51.5079,"lon":-0.1201,"speed_kph":52.1,"heading_deg":20.0}
  event = TelemetryEvent("evt-1002", "veh-001", 1704100860000, 51.5079, -0.1201, 52.1, 20.0);
  events.push_back(event);

  // {"event_id":"evt-2001","device_id":"veh-002","ts_ms":1704100800000,"lat":53.4808,"lon":-2.2426,"speed_kph":35.0,"heading_deg":180.0},

  event = TelemetryEvent("evt-2001", "veh-002", 1704100800000, 53.4808, -2.2426, 35.0, 180.0);
  events.push_back(event);

  // {"event_id":"evt-1002","device_id":"veh-001","ts_ms":1704100860000,"lat":51.5079,"lon":-0.1201,"speed_kph":52.1,"heading_deg":20.0},  
  event = TelemetryEvent("evt-1002", "veh-001", 1704100860000, 51.5079, -0.1201, 52.1, 20.0);
  events.push_back(event);


// {"event_id":"evt-1000","device_id":"veh-001","ts_ms":1704100740000,"lat":51.5070,"lon":-0.1300,"speed_kph":40.0,"heading_deg":8.5}
  event = TelemetryEvent("evt-1000", "veh-001", 1704100740000, 51.5070, -0.1300, 40.0, 8.5);
  events.push_back(event);

  for (const TelemetryEvent& event: events) {
    event.printInfo();
  }

  return 0;
}
