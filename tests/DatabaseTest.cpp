#include <cassert>
#include <cstdio>
#include <sqlite3.h>

int main()
{
    const char* database_path = "test_database.db";

    std::remove(database_path);

    sqlite3* database = nullptr;

    assert(sqlite3_open(database_path, &database) == SQLITE_OK);

    const char* create_table_sql = R"(
        CREATE TABLE telemetry_events (
            event_id TEXT PRIMARY KEY,
            device_id TEXT NOT NULL,
            timestamp INTEGER NOT NULL,
            latitude REAL NOT NULL,
            longitude REAL NOT NULL,
            speed_kph REAL NOT NULL,
            heading_degrees REAL NOT NULL
        );
    )";

    assert(sqlite3_exec(database, create_table_sql, nullptr, nullptr, nullptr)
           == SQLITE_OK);

    const char* insert_sql = R"(
        INSERT INTO telemetry_events (
            event_id,
            device_id,
            timestamp,
            latitude,
            longitude,
            speed_kph,
            heading_degrees
        )
        VALUES (?, ?, ?, ?, ?, ?, ?)
        ON CONFLICT(event_id) DO NOTHING;
    )";

    sqlite3_stmt* statement = nullptr;

    assert(sqlite3_prepare_v2(
        database,
        insert_sql,
        -1,
        &statement,
        nullptr
    ) == SQLITE_OK);

    // Newer event arrives first.
    sqlite3_bind_text(statement, 1, "evt-new", -1, SQLITE_STATIC);
    sqlite3_bind_text(statement, 2, "veh-test", -1, SQLITE_STATIC);
    sqlite3_bind_int64(statement, 3, 1704100860000);
    sqlite3_bind_double(statement, 4, 51.5079);
    sqlite3_bind_double(statement, 5, -0.1201);
    sqlite3_bind_double(statement, 6, 52.1);
    sqlite3_bind_double(statement, 7, 20.0);

    assert(sqlite3_step(statement) == SQLITE_DONE);

    sqlite3_reset(statement);
    sqlite3_clear_bindings(statement);

    // Older event arrives later.
    sqlite3_bind_text(statement, 1, "evt-old", -1, SQLITE_STATIC);
    sqlite3_bind_text(statement, 2, "veh-test", -1, SQLITE_STATIC);
    sqlite3_bind_int64(statement, 3, 1704100740000);
    sqlite3_bind_double(statement, 4, 51.5070);
    sqlite3_bind_double(statement, 5, -0.1300);
    sqlite3_bind_double(statement, 6, 40.0);
    sqlite3_bind_double(statement, 7, 8.5);

    assert(sqlite3_step(statement) == SQLITE_DONE);

    sqlite3_reset(statement);
    sqlite3_clear_bindings(statement);

    // Submit the newer event again.
    sqlite3_bind_text(statement, 1, "evt-new", -1, SQLITE_STATIC);
    sqlite3_bind_text(statement, 2, "veh-test", -1, SQLITE_STATIC);
    sqlite3_bind_int64(statement, 3, 1704100860000);
    sqlite3_bind_double(statement, 4, 51.5079);
    sqlite3_bind_double(statement, 5, -0.1201);
    sqlite3_bind_double(statement, 6, 52.1);
    sqlite3_bind_double(statement, 7, 20.0);

    assert(sqlite3_step(statement) == SQLITE_DONE);

    sqlite3_finalize(statement);

    // Check that only two unique events were stored.
    const char* count_sql =
        "SELECT COUNT(*) FROM telemetry_events;";

    assert(sqlite3_prepare_v2(
        database,
        count_sql,
        -1,
        &statement,
        nullptr
    ) == SQLITE_OK);

    assert(sqlite3_step(statement) == SQLITE_ROW);

    int event_count = sqlite3_column_int(statement, 0);

    assert(event_count == 2);

    sqlite3_finalize(statement);

    // Check that the newest timestamp is still the latest location.
    const char* latest_sql = R"(
        SELECT timestamp
        FROM telemetry_events
        WHERE device_id = 'veh-test'
        ORDER BY timestamp DESC
        LIMIT 1;
    )";

    assert(sqlite3_prepare_v2(
        database,
        latest_sql,
        -1,
        &statement,
        nullptr
    ) == SQLITE_OK);

    assert(sqlite3_step(statement) == SQLITE_ROW);

    long long latest_timestamp =
        sqlite3_column_int64(statement, 0);

    assert(latest_timestamp == 1704100860000);

    sqlite3_finalize(statement);

    sqlite3_close(database);

    std::remove(database_path);

    return 0;
}
