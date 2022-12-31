#include <Arduino.h>
#include "madgwick_6dof.h"

// calculations from Sebastian Madgwick at https://x-io.co.uk/open-source-imu-and-ahrs-algorithms/
// originally implemented from Madgwick's 2010 paper at https://x-io.co.uk/downloads/madgwick_internal_report.pdf
// more thorough/new/official implementation at https://github.com/xioTechnologies/Fusion/blob/main/Fusion/FusionAhrs.c

#define gyroMeasError 3.14159265358979f * (5.0f / 180.0f) // gyroscope measurement error in rad/s (shown as 5 deg/s)
#define beta sqrt(3.0f / 4.0f) * gyroMeasError // compute beta

static Quaternion q;                                                // live fused orientation
static VectorFloat aa, gv;                                          // math-friendly containers for raw IMU outputs
static Quaternion q_dir, q_gerr, q_half, q_two;                     // precalculated elements
static float q_objf[3];                                             // objective function elements
static float J_11or24, J_12or23, J_13or22, J_14or21, J_32, J_33;    // objective function Jacobian elements

Quaternion& update_madgwick_6dof_filter(int16_t ax, int16_t ay, int16_t az, int16_t gx, int16_t gy, int16_t gz, float dT, float accelScale, float gyroScale) {
    // make sure we have a sane starting point
    if (q.w == 0.0f && q.x == 0.0f && q.y == 0.0f && q.z == 0.0f) q.w = 1.0f;

    // copy raw values into VectorFloat containers (gyro is converted to rad/sec and scaled further)
    aa.x = ax * accelScale;
    aa.y = ay * accelScale;
    aa.z = az * accelScale;
    gv.x = gx * gyroScale;
    gv.y = gy * gyroScale;
    gv.z = gz * gyroScale;

    // note:
    //  - aa is linear acceleration from accelerometer
    //  - gv is angular velocity from gyroscope

    // auxiliary variables to avoid reapeated calculations
    q_half.w = 0.5f * q.w;
    q_half.x = 0.5f * q.x;
    q_half.y = 0.5f * q.y;
    q_half.z = 0.5f * q.z;
    q_two.w = 2.0f * q.w;
    q_two.x = 2.0f * q.x;
    q_two.y = 2.0f * q.y;

    // normalize the accelerometer measurement
    aa.normalize();

    // compute the objective function and Jacobian
    q_objf[0] = q_two.x * q.z - q_two.w * q.y - aa.x;
    q_objf[1] = q_two.w * q.x + q_two.y * q.z - aa.y;
    q_objf[2] = 1.0f - q_two.x * q.x - q_two.y * q.y - aa.z;
    J_11or24 = q_two.y; // J_11 negated in matrix multiplication
    J_12or23 = 2.0f * q.z;
    J_13or22 = q_two.w; // J_12 negated in matrix multiplication
    J_14or21 = q_two.x;
    J_32 = 2.0f * J_14or21; // negated in matrix multiplication
    J_33 = 2.0f * J_11or24; // negated in matrix multiplication

    // compute the gradient (matrix multiplication)
    q_gerr.w = J_14or21 * q_objf[1] - J_11or24 * q_objf[0];
    q_gerr.x = J_12or23 * q_objf[0] + J_13or22 * q_objf[1] - J_32 * q_objf[2];
    q_gerr.y = J_12or23 * q_objf[1] - J_33 * q_objf[2] - J_13or22 * q_objf[0];
    q_gerr.z = J_14or21 * q_objf[0] + J_11or24 * q_objf[1];

    // normalize the gradient
    q_gerr.normalize();

    // compute the quaternion derivative measured by gyroscopes
    q_dir.w = -q_half.x * gv.x - q_half.y * gv.y - q_half.z * gv.z;
    q_dir.x = q_half.w * gv.x + q_half.y * gv.z - q_half.z * gv.y;
    q_dir.y = q_half.w * gv.y - q_half.x * gv.z + q_half.z * gv.x;
    q_dir.z = q_half.w * gv.z + q_half.x * gv.y - q_half.y * gv.x;

    // compute, then integrate the estimated quaternion derivative
    q.w += (q_dir.w - (beta * q_gerr.w)) * dT;
    q.x += (q_dir.x - (beta * q_gerr.x)) * dT;
    q.y += (q_dir.y - (beta * q_gerr.y)) * dT;
    q.z += (q_dir.z - (beta * q_gerr.z)) * dT;

    // normalize quaternion
    q.normalize();

    // send back the current quaternion
    return q;
}
