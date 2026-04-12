#include <iostream>
#include <cmath>
#include <vector>
#include <functional>


// Converts temperature from Celsius to Fahrenheit
double celsiusToFahrenheit(double c) {
    return c * 1.8 + 32.0;
}

// Calculates Vapor Pressure Deficit
double calculateVDP(double temp, double humidity) {
    return 0.6108 *  exp(17.27 * temp / (temp + 237.3)) * (1.0 - humidity / 100.0);
}

// Chicks if the temperature in optimal range (Celsius)
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

}