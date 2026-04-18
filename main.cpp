#include <iostream>
#include <iomanip>
#include <cmath>
#include <vector>
#include <functional>

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
    std::vector<double> temps = {18.5, 22.0, 31.7, 15.2, 25.8};

    auto normalizeTemp = [](double t) {return normalize(t, 0.0, 50.0);};
    // Set threshold to 0.7
    auto applyAlertThreshold = [](double t) {return applyThreshold(t, 0.7);};

    auto tempPipeline = compose(applyAlertThreshold, normalizeTemp);

    auto results = transformReadings(temps, tempPipeline);

    for (double val : results) {
        std::cout << std::fixed << std::setprecision(2) << val << " " ;
        if (val == 1.0) {
            std::cout << "(alert!) ";
        }
    }
}
