#include <iostream>
#include <iomanip>
#include <cmath>
#include <vector>
#include <functional>

const double MAX_CRITICAL_TEMPERATURE = 35.0;
const double MIN_CRITICAL_HUMIDITY = 40.0;

// Struct for storing data from sensors
struct SensorReading {
    int id;
    double temperature; // Celsius
    double humidity; // %
};

// Converts temperature from Celsius to Fahrenheit
double celsiusToFahrenheit(double c) {
    return c * 1.8 + 32.0;
}

// Calculates Vapor Pressure Deficit
double calculateVPD(double temp, double humidity) {
    return 0.6108 *  exp(17.27 * temp / (temp + 237.3)) * (1.0 - humidity / 100.0);
}

// Checks if the temperature in optimal range (Celsius)
bool isOptimalTemperature(double temp) {
    double minTemp = 18.0;
    double maxTemp = 26.0;
    return minTemp <= temp && temp <= maxTemp;
}


// Transforms a collection of sensor readings by applying a specific function to each element
std::vector<double> transformReadings(
const std::vector<double>& readings,
const std::function<double(double)>& func
) {
    std::vector<double> result;
    result.reserve(readings.size());
    for (double val : readings) {
        result.push_back(func(val));
    }
    return result;
}



int main() {
    // Test values
    std::vector<SensorReading> readings = {
        {1, 22.5, 68.0}, // norm
        {2, 37.2, 45.0}, // critical temperature
        {3, 24.1, 35.5}, // critical humidity
        {4, 19.8, 72.0}, // norm
        {5, 38.0, 38.0}, // both critical
        };
    // Stores sensor readings that exceed safe thresholds
    std::vector<SensorReading> criticalZones;
    // Checks if the sensor data indicates critical conditions for the plants

    std::copy_if(readings.begin(), readings.end(), std::back_inserter(criticalZones),
        [](const SensorReading& r) {
            return r.temperature > MAX_CRITICAL_TEMPERATURE || r.humidity < MIN_CRITICAL_HUMIDITY;
        });

    for (auto& zone : criticalZones) {
        std::cout << "[ALERT] Zone " << zone.id << ": Temperature " << std::fixed << std::setprecision(1)
        << zone.temperature << "C, Humidity " << zone.humidity << std::endl;
    }
}