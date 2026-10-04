# SOLUTIONS

Describe the design and trade-offs made.

CMake:

Out-of-order: by design the table doesn't 
Duplicate logs: - Taken care of by Insert statement  ( `ON CONFLICT(event_id) DO NOTHING;`)
-

SQLite3:




## Task 2: Operational queries and reporting surface
### Last known location

Show last known location for each vehicle using this SQL statement:

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



