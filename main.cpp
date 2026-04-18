#include <iostream>
#include <iomanip>
#include <cmath>
#include <vector>
#include <functional>
#include <numeric>
#include <algorithm>

// =========================================================================
// GLOBAL CONSTANTS
// =========================================================================
const double MAX_CRITICAL_TEMPERATURE = 35.0;
const double MIN_CRITICAL_HUMIDITY = 40.0;

// =========================================================================
// DATA STRUCTURES
// =========================================================================

// Struct for storing data from sensors
struct SensorReading {
    int id;
    double temperature; // Celsius
    double humidity; // %
};

// =========================================================================
// PURE FUNCTIONS
// =========================================================================

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

// Normalizes a value to a scale of [0.0, 1.0]
double normalize(double x, double min, double max) {
    return (x - min) / (max - min);
}

// Returns 1.0 if the value exceeds the threshold, otherwise returns the value itself
double applyThreshold(double x, double threshold) {
    return (x > threshold) ? 1.0 : x;
}

// =========================================================================
// HIGHER-ORDER FUNCTIONS
// =========================================================================

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

// Combines two mathematical functions f and g into a single pipeline f(g(x))
std::function <double(double)> compose(
    const std::function<double(double)> &f,
    const std::function<double(double)> &g) {
    return [f,g](double x) {return f(g(x));};
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

    // Filter stress zones (Temperature > 28 or < 16)
    std::vector<SensorReading> stressZones;
    std::copy_if(readings.begin(), readings.end(),
    std::back_inserter(stressZones),
    [](const SensorReading& r) {
    return r.temperature > 28.0 || r.temperature < 16.0;
    });

    // Average temperature in stress zones
    double avgStressTemp = 0.0;
    if (!stressZones.empty()) {
        avgStressTemp = std::accumulate(
    stressZones.begin(), stressZones.end(), 0.0,
[](double sum, const SensorReading& r) {
        return sum + r.temperature;
        }) / stressZones.size();
        std::cout << "Average stress temperature: "
        <<  std::fixed << std::setprecision(1) << avgStressTemp << std::endl;
    }
    else {
        std::cout << "No stress temperature found!" << std::endl;
    }


    // Calculates the number of hours the greenhouse was in a critical state.
    // Assuming each record in the 'readings' vector represents exactly one hour.
    // Condition for critical state: Temperature > 35 or Humidity < 40.
    int criticalHoursCount = std::count_if(
        readings.begin(), readings.end(),
        [](const SensorReading& r) {
            return r.temperature > MAX_CRITICAL_TEMPERATURE || r.humidity < MIN_CRITICAL_HUMIDITY;
        }
    );

    std::cout << "Critical conditions lasted for: " << criticalHoursCount << " hours" << std::endl;


    // Average VPD across all readings
    double avgVPD = 0.0;
    if (!readings.empty()) {
        avgVPD = std::accumulate(
            readings.begin(), readings.end(), 0.0,
            [](double sum, const SensorReading& r) {
                return sum + calculateVPD(r.temperature, r.humidity);
            }
        ) / readings.size();
        std::cout << "Average VPD: "
        << std::fixed << std::setprecision(3) << avgVPD << std::endl;
    }

    // VPD Recommendations
    std::cout << "Recommendations:" << std::endl;
    if (avgVPD < 0.4) {
        std::cout << "Average VPD less than 0.4 - too humid, open ventilation" << std::endl;
    }
    else if (avgVPD >= 0.4 && avgVPD <= 1.2) {
        std::cout << "Average VPD between 0.4 and 1.2 - optimal, don't do anything" << std::endl;
    }
    else if (avgVPD > 1.6) {
        std::cout << "Average VPD more than 1.6 - too dry, turn on humidification system" << std::endl;
    }
    else {
        std::cout << "Keep monitoring VPD data" <<std::endl;
    }
}


