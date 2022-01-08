#include "ev3api.h"
#include "app.h"
#if defined(BUILD_MODULE)
#include "module_cfg.h"
#else
#include "kernel_cfg.h"
#endif
#include <stdbool.h>
#define gyro_sensor EV3_PORT_2
#define color_sensor EV3_PORT_4
#define ultraSonic_sensor EV3_PORT_3
#define R_motor EV3_PORT_D
#define L_motor EV3_PORT_A
#define M_motor EV3_PORT_B
#define M_M_PORT EV3_PORT_B
#define R_M_PORT EV3_PORT_D
#define L_M_PORT EV3_PORT_A
#define STRAIGHT_POWER (20.0)
#define M_D_DEGREE (30.0)
#define M_D_POWER (20.0)
#define M_U_POWER (20.0)
#define M_U_DEGREE (-30.0)
#define NPC (5.0)
#define BACK (5.0)
#define LENGTH (8.0)
#define KILL_MOTOR() (ev3_motor_stop(L_M_PORT, true), ev3_motor_stop(R_M_PORT, true), tslp_tsk(WAIT_TIME_MS))
#define MiddleMotorDown() (KILL_MOTOR(), ev3_motor_rotate(M_M_PORT, -M_D_DEGREE, M_D_POWER, true))
#define MiddleMotorUp() (KILL_MOTOR(), ev3_motor_rotate(M_M_PORT, -M_U_DEGREE, M_U_POWER, true))
#define STRAIGHT(digree, power) (ev3_motor_rotate(L_M_PORT, digree, power, false), ev3_motor_rotate(R_M_PORT, digree, power, true))
#define SUPERLATIVE_SWIVEL(digree, power) (ev3_motor_rotate(R_M_PORT, digree, power, false), ev3_motor_rotate(L_M_PORT, -digree, power, true))
#define WAIT_TIME_MS (100.0)
#define GyroReset() (ev3_gyro_sensor_reset(gyro_sensor), tslp_tsk(1000))
void DrawStraight(int bf, int centimeter, int power, int withPen)
{
    if (!bf)
        centimeter *= -1;
    if (withPen)
        MiddleMotorDown();
    STRAIGHT(360.0 * centimeter / 17.5, power);
    KILL_MOTOR();
    if (withPen)
        MiddleMotorUp();
}
inline void TurnRightWithGyro(int digree)
{
    GyroReset();
    while (true)
    {
        SUPERLATIVE_SWIVEL(-10.0, 30.0);
        if (ev3_gyro_sensor_get_angle(gyro_sensor) >= digree)
        {
            KILL_MOTOR();
            break;
        }
        tslp_tsk(50);
    }
}
inline void TurnLeftWithGyro(int digree)
{
    GyroReset();
    while (true)
    {
        SUPERLATIVE_SWIVEL(10.0, 30.0);
        if (ev3_gyro_sensor_get_angle(gyro_sensor) <= -digree)
        {
            KILL_MOTOR();
            break;
        }
        tslp_tsk(50);
    }
}
inline void ForwardAndaWrite(float centimeter)
{
    int power = 20;
    MiddleMotorDown();
    STRAIGHT(360.0 * centimeter / 17.5, power);
    KILL_MOTOR();
    MiddleMotorUp();
}

void run_task(intptr_t unused)
{
    char str[20];
    tslp_tsk(2000);
    STRAIGHT(360.0 * 0.5 / 17.5, STRAIGHT_POWER);
    KILL_MOTOR();
    int distance = (int)ev3_ultrasonic_sensor_get_distance(ultraSonic_sensor);
    distance /= 10;
    sprintf(str, "%4d", distance);
    ev3_lcd_draw_string(str, 0, false);
    DrawStraight(true, 5, STRAIGHT_POWER, false);
    switch (distance)
    {
    case 1:
    case 2:
    case 3:
        DrawStraight(true, LENGTH, STRAIGHT_POWER, true);
        DrawStraight(true, NPC, STRAIGHT_POWER, false);
        if (distance == 1)
            break;
        TurnRightWithGyro(90);
        DrawStraight(true, NPC, STRAIGHT_POWER, false);
        TurnRightWithGyro(90);
        DrawStraight(false, BACK, STRAIGHT_POWER, false);
        DrawStraight(true, LENGTH, STRAIGHT_POWER, true);
        DrawStraight(true, NPC, STRAIGHT_POWER, false);
        if (distance == 2)
            break;
        TurnLeftWithGyro(90);
        DrawStraight(true, NPC, STRAIGHT_POWER, false);
        TurnLeftWithGyro(70);
        DrawStraight(false, BACK, STRAIGHT_POWER, false);
        DrawStraight(true, LENGTH, STRAIGHT_POWER, true);
        DrawStraight(true, NPC, STRAIGHT_POWER, false);
        break;
    case 4:
        DrawStraight(true, LENGTH, STRAIGHT_POWER, true);
        DrawStraight(true, NPC, STRAIGHT_POWER, false);
        TurnRightWithGyro(90);
        DrawStraight(true, NPC, STRAIGHT_POWER, false);
        TurnRightWithGyro(60);
        DrawStraight(false, BACK, STRAIGHT_POWER, false);
        DrawStraight(true, 9, STRAIGHT_POWER, true);
        DrawStraight(true, NPC, STRAIGHT_POWER, false);
        TurnLeftWithGyro(90);
        DrawStraight(false, BACK, STRAIGHT_POWER, false);
        DrawStraight(true, 9, STRAIGHT_POWER, true);
        break;
    case 5:
    case 6:
    case 7:
    case 8:
        DrawStraight(true, LENGTH, STRAIGHT_POWER, false);
        DrawStraight(true, NPC, STRAIGHT_POWER, false);
        TurnRightWithGyro(90);
        TurnRightWithGyro(60);
        DrawStraight(false, BACK, STRAIGHT_POWER, false);
        DrawStraight(true, 9, STRAIGHT_POWER, true);
        DrawStraight(true, NPC, STRAIGHT_POWER, false);
        TurnLeftWithGyro(90);
        DrawStraight(false, BACK, STRAIGHT_POWER, false);
        DrawStraight(true, 9, STRAIGHT_POWER, true);
        DrawStraight(true, NPC, STRAIGHT_POWER, false);
        if (distance == 5)
            break;
        TurnRightWithGyro(60);
        DrawStraight(true, NPC, STRAIGHT_POWER, false);
        TurnRightWithGyro(90);
        DrawStraight(false, BACK, STRAIGHT_POWER, false);
        DrawStraight(true, LENGTH, STRAIGHT_POWER, true);
        DrawStraight(true, NPC, STRAIGHT_POWER, false);
        if (distance == 6)
            break;
        TurnLeftWithGyro(90);
        DrawStraight(true, NPC, STRAIGHT_POWER, false);
        TurnLeftWithGyro(90);
        DrawStraight(false, BACK, STRAIGHT_POWER, false);
        DrawStraight(true, LENGTH, STRAIGHT_POWER, true);
        DrawStraight(true, NPC, STRAIGHT_POWER, false);
        if (distance == 7)
            break;
        TurnRightWithGyro(90);
        DrawStraight(true, NPC, STRAIGHT_POWER, false);
        TurnRightWithGyro(90);
        DrawStraight(false, BACK, STRAIGHT_POWER, false);
        DrawStraight(true, LENGTH, STRAIGHT_POWER, true);
        break;
    case 9:
        DrawStraight(true, LENGTH, STRAIGHT_POWER, true);
        DrawStraight(true, NPC, STRAIGHT_POWER, false);
        TurnRightWithGyro(90);
        TurnRightWithGyro(60);
        DrawStraight(false, BACK, STRAIGHT_POWER, false);
        DrawStraight(true, 9, STRAIGHT_POWER, true);
        DrawStraight(true, NPC, STRAIGHT_POWER, false);
        TurnLeftWithGyro(120);
        DrawStraight(false, BACK, STRAIGHT_POWER, false);
        DrawStraight(true, LENGTH, STRAIGHT_POWER, false);
        DrawStraight(true, NPC, STRAIGHT_POWER, false);
        TurnLeftWithGyro(120);
        DrawStraight(false, BACK, STRAIGHT_POWER, false);
        DrawStraight(true, 9, STRAIGHT_POWER, false);
        break;
    default:
        break;
    }
    tslp_tsk(WAIT_TIME_MS);
}
void main_task(intptr_t unused)
{
    ev3_sensor_config(gyro_sensor, GYRO_SENSOR);
    ev3_motor_config(L_M_PORT, LARGE_MOTOR);
    ev3_motor_config(R_M_PORT, LARGE_MOTOR);
    ev3_motor_config(M_M_PORT, MEDIUM_MOTOR);
    ev3_sensor_config(ultraSonic_sensor, ULTRASONIC_SENSOR);
    act_tsk(RUN_TASK);
}
