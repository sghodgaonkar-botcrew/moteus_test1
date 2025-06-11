#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <limits>              // For std::numeric_limits
#include "moteus.h"            

// Define the mathematical constant π
const double PI = 3.14159265358979323846;

/**
 * @brief Convert an angle from radians to revolutions.
 * 
 * @param rad Angle in radians.
 * @return Angle in revolutions (1 rev = 2π rad).
 */
inline float radtorev(float rad) {
    return rad / (2 * PI);
}

int main(int argc, char** argv) {
    using namespace mjbots;

    // Process command-line arguments for transport configuration.
    moteus::Controller::DefaultArgProcess(argc, argv);

    // Create configuration options for the controller.
    moteus::Controller::Options options;

    // Instantiate the controller with the specified options.
    moteus::Controller controller(options);

    // Clear all past states of the motor and return it to default.
    controller.SetStop();

    // Prepare a position mode command structure.
    moteus::PositionMode::Command cmd;

    // Set desired position and velocity for the motor.
    cmd.position = radtorev(0.5);   // Move to 0.5 radians converted to revolutions
    cmd.velocity = 1.0;             // Target velocity in revolutions/sec

    int missed_replies = 0;         // Track missed replies for safety timeout

    // Main control loop
    while (true) {
        ::usleep(10);               // Sleep for 10 microseconds

        // Send the command and wait for a response
        const auto maybe_result = controller.SetPosition(cmd);

        // Handle missed replies
        if (!maybe_result) {
            missed_replies++;
            if (missed_replies > 3) {
                printf("\n\nmotor timeout!\n");
                break;
            }
            continue;
        } else {
            missed_replies = 0;
        }

        // Extract servo response values
        const auto r = maybe_result->values;

        // Print torque value returned by servo
        printf("Torque is %6.3f\n", r.torque);

        // Stop commanding position once the servo reaches the target
        if (r.position == cmd.position) {
            cmd.position = std::numeric_limits<float>::quiet_NaN(); // Stop position command
            cmd.velocity = 0.0;                                      // Zero velocity
        }
    }

    return 0;
}
