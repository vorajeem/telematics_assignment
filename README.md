# telematics_assignment
Telematics ingestion and reporting (C++/SQL/Cross-platform)

## Instructions to run 
- Libraries used: https://github.com/simdjson/simdjson

./telematics [location_of_json_file]
i.e.

./telematics ../data/input_data.json


Testing (Manually)
`./telematics ../data/duplicate_out-of-order_data.json`

## Technologies tested
- C++17
- SQL (suggested SQLite)
- Git
- Azure DevOps YAML pipeline


# Platforms
- Cross-platform (Linux, Windows)

## Given Assumptions

- Event flows,...
- See assignments


## My assumptions:



# Deliverables
- Git 
- Video



# Codex prompts

```add getters for all fields in the TelemetryEvent object. I need those fields available so that the objects can be persisted into the SQLite3 telemetry_events table```

---------------

```continue adding the sqlite3_bind_text(insert_statement, X, event_parsed.getX().c_str(), -1, SQLITE_TRANSIENT); 

for all the other fields which I need to add in the telemetry_events table. Be careful not to break the implementation. Make sure that there no type conflicts```

-------------
