# SOLUTIONS

- Purpose of document: Describe the design and trade-offs made.


This project is C++17 telematics event ingestion and storage application.
The application reads newline-delimited JSON objects from an input file as per the format given below:

```
{"event_id":"evt-1001","device_id":"veh-001","ts_ms":1704100800000,"lat":51.5074,"lon":-0.1278,"speed_kph":48.3,"heading_deg":12.0}
```
The application is built using CMake (portable, lightweight C++ building tool).


## Technologies used
- C++17
- SQLite3
- Git
- Azure DevOps YAML pipeline
- Cmake
- Libraries used: https://github.com/simdjson/simdjson
- SQLite3
- Cmake 

# Platforms
- Cross-platform (Linux, Windows)

## Duplicate logs:
The application handles duplicates events. This is taken care off by the insert statement with the phrase ( `ON CONFLICT(event_id) DO NOTHING;`) appended to the end of the insert statement.

## Out-of-order events

The application caters for 'out-of-order' events submissions.
By design, the table is not ordered by timestamp. So out-of-order events will still be added to the table.

## Choices of technologies
- JSON parser: `Simdjson` - lightweight, allows for fast processing of incoming data. This choice allows the application to be scalable (5k-10k events/day).

- SQLite3: Database is just a local file (.db), relatively simple to use, allows for portability between different platforms


## Table schemas considerations
- Schema allows for long term storage similar to the format how it was received (without lossing accuracy)
- Event_ID is selected as the 'primary key' to differentiate different events. (This is enforced in the database)


## Task 2: Operational queries and reporting surface
### Last known location (Mandatory query)

Show last known location for each vehicle using this SQL statement:

To get the latest known location for each device, we 

```
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
```

## Other queries

Show event, device and timestamp for all vehicles (in order of timestamp with latest timestamp first and oldest timestamp last)

```
SELECT event_id, device_id, timestamp
FROM telemetry_events
ORDER BY timestamp DESC;

```
### AI Chat prompts used in the development of this application:

1. 
```add getters for all fields in the TelemetryEvent object. I need those fields available so that the objects can be persisted into the SQLite3 telemetry_events table```


2. 
```continue adding the sqlite3_bind_text(insert_statement, X, event_parsed.getX().c_str(), -1, SQLITE_TRANSIENT); 

for all the other fields which I need to add in the telemetry_events table. Be careful not to break the implementation. Make sure that there no type conflicts```

3. I also used ChatGPT to understand the problem, compare technologies choice (e.g. JSON parser, database . 

