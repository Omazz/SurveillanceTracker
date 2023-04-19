#ifndef RADARTARGET_H
#define RADARTARGET_H

#include <cstdint>

enum class TargetDetection : uint8_t
{
    Psr,
    Ssr,
    PsrPlusSsr,
};

struct RadarTarget
{
    float rho_km;
    float theta_rad;
    float time_of_day_sec;
    const char *squawk;
    uint32_t address;
    const char *ident;
    float ground_speed_kmh;
    float heading_rad;
    uint32_t altitude_m;
    float lat;
    float lon;
    TargetDetection detection;
    int32_t track_number;
    float track_pos_x_km;
    float track_pos_y_km;
    float r_max;
    float r_min;
    float delta_theta;
};

#endif // RADARTARGET_H
