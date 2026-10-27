/**
 * @file hazard_detector.cpp
 * @brief Hazard Detection & Safety Control Sub-System
 * @author Ekenedilichukwu Emmanuel Nsiadu
 * @details Real-time monitoring of environmental and hardware sensor thresholds
 *          with automated safety cut-off mechanisms.
 */

#include <iostream>
#include <vector>
#include <chrono>
#include <thread>

// Enum representing safety alarm levels
enum class AlertLevel {
    NORMAL,
    WARNING,
    CRITICAL
};

// Structure holding sensor telemetry data
struct SensorData {
    float temperature_celsius;
    float gas_ppm;
    bool flame_detected;
};

class HazardDetectionSystem {
private:
    float max_temp_threshold;
    float max_gas_threshold;
    bool safety_relay_active;

public:
    HazardDetectionSystem(float temp_limit = 65.0f, float gas_limit = 400.0f)
        : max_temp_threshold(temp_limit), max_gas_threshold(gas_limit), safety_relay_active(true) {}

    // Evaluates sensor inputs against preset hardware safety limits
    AlertLevel evaluateStatus(const SensorData& data) {
        if (data.flame_detected || data.temperature_celsius > max_temp_threshold || data.gas_ppm > max_gas_threshold) {
            triggerSafetyCutoff();
            return AlertLevel::CRITICAL;
        } else if (data.temperature_celsius > (max_temp_threshold * 0.85f)) {
            return AlertLevel::WARNING;
        }
        return AlertLevel::NORMAL;
    }

    void triggerSafetyCutoff() {
        safety_relay_active = false;
        std::cout << "[EMERGENCY]: Critical threshold exceeded! Safety relay TRIPPED.\n";
    }

    bool isRelayActive() const { return safety_relay_active; }
};

int main() {
    HazardDetectionSystem detector(60.0f, 350.0f);

    // Simulated real-time sensor telemetry stream
    std::vector<SensorData> telemetry_stream = {
        {25.5f, 120.0f, false},
        {42.0f, 180.0f, false},
        {54.0f, 310.0f, false},
        {63.5f, 380.0f, true}  // Trigger condition
    };

    std::cout << "Starting Hazard Detection System Monitoring...\n";

    for (size_t i = 0; i < telemetry_stream.size(); ++i) {
        AlertLevel status = detector.evaluateStatus(telemetry_stream[i]);
        
        std::cout << "Cycle " << i + 1 << " | Temp: " << telemetry_stream[i].temperature_celsius 
                  << "C | Gas: " << telemetry_stream[i].gas_ppm << " PPM | Status: ";

        switch (status) {
            case AlertLevel::NORMAL:
                std::cout << "OK\n";
                break;
            case AlertLevel::WARNING:
                std::cout << "WARNING (High Temp/Gas)\n";
                break;
            case AlertLevel::CRITICAL:
                std::cout << "CRITICAL (System Halted)\n";
                break;
        }

        if (!detector.isRelayActive()) {
            std::cout << "System safely locked out. Terminating telemetry loop.\n";
            break;
        }
    }

    return 0;
}
