#include "Vehicle.hpp"
#include "TelemetryEvent.hpp"

#include <vector>
#include "simdjson.h"
// using namespace simdjson;


simdjson::dom::parser parser;

int main() {


  std::string json =R"([
{"event_id":"evt-1001","device_id":"veh-001","ts_ms":1704100800000,"lat":51.5074,"lon":-0.1278,"speed_kph":48.3,"heading_deg":12.0},
{"event_id":"evt-1002","device_id":"veh-001","ts_ms":1704100860000,"lat":51.5079,"lon":-0.1201,"speed_kph":52.1,"heading_deg":20.0},
{"event_id":"evt-2001","device_id":"veh-002","ts_ms":1704100800000,"lat":53.4808,"lon":-2.2426,"speed_kph":35.0,"heading_deg":180.0},
{"event_id":"evt-1002","device_id":"veh-001","ts_ms":1704100860000,"lat":51.5079,"lon":-0.1201,"speed_kph":52.1,"heading_deg":20.0},
{"event_id":"evt-1000","device_id":"veh-001","ts_ms":1704100740000,"lat":51.5070,"lon":-0.1300,"speed_kph":40.0,"heading_deg":8.5}
])";
  // cout << "Hello world from Cmake\n\n" <<endl;

std::vector<TelemetryEvent> events;
  simdjson::dom::element document = parser.parse(json);

  simdjson::dom::array my_events = document.get_array();

  for (simdjson::dom::element my_event : my_events) {
    std::string_view event_id_sv = my_event["event_id"];
    std::string event_id_str{event_id_sv};

    std::string_view device_id_sv = my_event["device_id"];
    std::string device_id_str{device_id_sv};

    int64_t timestamp = my_event["ts_ms"];
    double latitude = my_event["lat"];
    double longitude = my_event["lon"];
    double speed = my_event["speed_kph"];
    double heading_degrees = my_event["heading_deg"];
    TelemetryEvent event_parsed(event_id_str,device_id_str,timestamp, latitude, longitude,speed, heading_degrees);    
    events.push_back(event_parsed);
  }

  for (const TelemetryEvent& event: events) {
    event.printInfo();
  }

  return 0;
}
