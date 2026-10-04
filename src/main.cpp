#include "Vehicle.hpp"
#include "TelemetryEvent.hpp"
#include <sqlite3.h>
#include <vector>
#include "simdjson.h"
// using namespace simdjson;

#include <fstream>
#include <iostream>

simdjson::dom::parser parser;

int main(int argc, char* argv[]) {

  // Database opening , error-handling
  sqlite3* database = nullptr;
  int result = sqlite3_open("../data/telematics.db", &database);
  if (result != SQLITE_OK) {
    std::cerr << "Could not open database: " << sqlite3_errmsg(database) << std::endl;
    sqlite3_close(database);
    return 1;
  }

  std::string line;
  std::vector<TelemetryEvent> events;
  if (argc < 2) {
    std::cerr << "Usage: ./telematics <input-files>" << std::endl;
    return 1;
  }

  std::ifstream input_file(argv[1]);
  // std::ifstream input_file("../data/input_data.json");

  if (!input_file) {
    std::cout << "Could not open input data json." << std::endl;
    return 1;
  }

  std::cout << "Database opened successfully." << std::endl;

  // create table if required
  const char* create_table_sql = R"(
  CREATE TABLE IF NOT EXISTS telemetry_events (
  event_id TEXT PRIMARY KEY,
  device_id TEXT NOT NULL,
  timestamp INTEGER NOT NULL,
  latitude REAL NOT NULL,
  longitude REAL NOT NULL,
  speed_kph REAL NOT NULL,
  heading_degrees REAL NOT NULL
  );
  )";
  char* error_message = nullptr;
  result = sqlite3_exec(
    database,
    create_table_sql,
    nullptr,
    nullptr,
    &error_message
  );
  if (result != SQLITE_OK) {
      std:cerr << "Could not create events table" << error_message << std::endl;
      sqlite3_free(error_message);
      sqlite3_close(database);
      return 1;
    }
    std::cout << "Table is ready" << std::endl;
  
  
    // prepare insert statement - 
    // this handles duplicates by ignoring duplicates.
    const char* insert_sql = R"(
    INSERT INTO telemetry_events (
    event_id,
    device_id,
    timestamp,
    latitude,
    longitude,
    speed_kph,
    heading_degrees)
    VALUES (?, ?, ?, ?, ?, ?, ?)
    ON CONFLICT(event_id) DO NOTHING;
  )";





  sqlite3_stmt* insert_statement = nullptr;
  result = sqlite3_prepare_v2(
    database, 
    insert_sql,
    -1,
    &insert_statement,
    nullptr
  );

  if (result != SQLITE_OK) {
    std::cerr << "Could not prepare insert statement:" <<sqlite3_errmsg(database) << std::endl;
    sqlite3_close(database);
    return 1;
  }


  while (std::getline(input_file,line)) {
    if (line.empty()) {
      continue;
    }

    try {

      // parse one line
      simdjson::dom::element my_event = parser.parse(line);
      //extract the event fields

      std::string_view event_id_sv = my_event["event_id"];
      std::string event_id_str{event_id_sv};
      std::string_view device_id_sv = my_event["device_id"];
      std::string device_id_str{device_id_sv};

      int64_t timestamp = my_event["ts_ms"];
      double latitude = my_event["lat"];
      double longitude = my_event["lon"];
      double speed = my_event["speed_kph"];
      double heading_degrees = my_event["heading_deg"];
      // construct a TelemetryEvent
      TelemetryEvent event_parsed(event_id_str,device_id_str,timestamp, latitude, longitude,speed, heading_degrees);    
      // add it to events
      events.push_back(event_parsed);

      int bind_result = sqlite3_bind_text(
        insert_statement,
        1,
        event_parsed.getEventId().c_str(),
        -1,
        SQLITE_TRANSIENT
      );
      if (bind_result == SQLITE_OK) {
        bind_result = sqlite3_bind_text(
          insert_statement,
          2,
          event_parsed.getDeviceId().c_str(),
          -1,
          SQLITE_TRANSIENT
        );
      }
      if (bind_result == SQLITE_OK) {
        bind_result = sqlite3_bind_int64(
          insert_statement,
          3,
          event_parsed.getTimestamp()
        );
      }
      if (bind_result == SQLITE_OK) {
        bind_result = sqlite3_bind_double(
          insert_statement,
          4,
          event_parsed.getLatitude()
        );
      }
      if (bind_result == SQLITE_OK) {
        bind_result = sqlite3_bind_double(
          insert_statement,
          5,
          event_parsed.getLongitude()
        );
      }
      if (bind_result == SQLITE_OK) {
        bind_result = sqlite3_bind_double(
          insert_statement,
          6,
          event_parsed.getSpeed()
        );
      }
      if (bind_result == SQLITE_OK) {
        bind_result = sqlite3_bind_double(
          insert_statement,
          7,
          event_parsed.getHeadingDegrees()
        );
      }

      if (bind_result != SQLITE_OK) {
        std::cerr << "Could not bind telemetry event: "
                  << sqlite3_errmsg(database) << std::endl;
        sqlite3_reset(insert_statement);
        sqlite3_clear_bindings(insert_statement);
        continue;
      }

      result = sqlite3_step(insert_statement);
      if (result != SQLITE_DONE) {
        std::cerr << "Could not insert telemetry event: "
                  << sqlite3_errmsg(database) << std::endl;
      }
      
      // execute the prepare statement
      result = sqlite3_step(insert_statement);
      if (result != SQLITE_DONE) {
        std::cerr << "Could not insert telemetry event: " << sqlite3_errmsg(database) << std::endl;
      }

      sqlite3_reset(insert_statement);
      
    }
    catch (const simdjson::simdjson_error& error) {
      std::cout << "Error parsing telemetry event: " << error.what() << std::endl;
    }

    for (const TelemetryEvent& event: events) {
      // event.printInfo();
    }
  }

    // mandatory - latest known location
   const char* latest_location_sql = R"(
     SELECT device_id, latitude, longitude, timestamp
    FROM (
        SELECT
            device_id,
            latitude,
            longitude,
            timestamp,
            ROW_NUMBER() OVER (
                PARTITION BY device_id
                ORDER BY timestamp DESC, event_id DESC
            ) AS row_number
        FROM telemetry_events
    )
    WHERE row_number = 1;
   )";

   sqlite3_stmt* latest_location_statement = nullptr;

   result = sqlite3_prepare_v2(
    database,
    latest_location_sql,
    -1,
    &latest_location_statement,
    nullptr
   );

   if (result != SQLITE_OK) {
    std::cerr << "Could not prepare latest-location query: " << sqlite3_errmsg(database) << std::endl;
    sqlite3_finalize(insert_statement);
    sqlite3_close(database);
    return 1;
   }

   //execute statement and display Load

   std::cout << "\nLatest location for each device:\n";

   while ( sqlite3_step(latest_location_statement) == SQLITE_ROW) {
    const unsigned char* device_id =
    sqlite3_column_text(latest_location_statement,0);

    double latitude = 
    sqlite3_column_double(latest_location_statement, 1);

    double longitude =
    sqlite3_column_double(latest_location_statement, 2);

    std::int64_t timestamp = 
    sqlite3_column_int64(latest_location_statement, 3);


    std::cout << "Device: " << device_id
        << ", Latitude: " << latitude
        << ", Longitude: " << longitude
        << ", Timestamp: " << timestamp
        << '\n';

        // corresponding to the SELECT statement
        //

   }


    // release the prepared statement 
    sqlite3_finalize(insert_statement);
    sqlite3_finalize(latest_location_statement);
    // close database safely
    sqlite3_close(database);
    return 0;
  }
  
