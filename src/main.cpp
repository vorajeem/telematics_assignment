#include "Vehicle.hpp"
#include "TelemetryEvent.hpp"

#include <vector>
#include "simdjson.h"
// using namespace simdjson;

#include <fstream>

simdjson::dom::parser parser;

int main() {

  std::ifstream input_file("../data/input_data.json");

if (!input_file) {
  std::cout << "Could not open input data json." << std::endl;
  return 1;
}

  std::string json(
    (std::istreambuf_iterator<char>(input_file)),
    std::istreambuf_iterator<char>()
  );
  
  std::vector<TelemetryEvent> events;
  simdjson::dom::element document = parser.parse(json);
  simdjson::dom::array my_events = document.get_array();

  for (simdjson::dom::element my_event : my_events) {
    try {
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
    catch (const simdjson::simdjson_error& error) {
      std::cout << "Error parsing telemetry event: " << error.what() << std::endl;
    }
  }

  for (const TelemetryEvent& event: events) {
    event.printInfo();
  }

  return 0;
}
