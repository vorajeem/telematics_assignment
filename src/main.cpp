#include <iostream>
using namespace std;
#include "Vehicle.hpp"
// #include "simdjson.h"
// using namespace simdjson;

int main() {
  cout << "Hello world from Cmake\n\n" <<endl;

  Vehicle vehicle("veh-001",  "evt-1001", 1704100800000, 51.5074, -0.1278, 48.3, 12.0);
  vehicle.printInfo();

  return 0;
}
