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
#define G_S_PORT EV3_PORT_2
#define STRAIGHT_POWER 20
#define M_D_DEGREE 30
#define M_D_POWER 20
#define M_U_POWER 20
#define M_U_DEGREE -30
#define ECSGR(x) ev3_color_sensor_get_reflect(x)
#define STRAIGHT(x) ev3_motor_steer(L_M_PORT, R_M_PORT, x, 0)
#define KILL_MOTOR() (ev3_motor_stop(L_M_PORT, true), ev3_motor_stop(R_M_PORT, true))
#define OneSteppd (ev3_motor_get_counts(L_M_PORT) - startval_l > STRAIGHT_ONESTEP || ev3_motor_get_counts(R_M_PORT) - startval_r > STRAIGHT_ONESTEP)
const uint32_t WAIT_TIME_MS = 100;
inline void MiddleMotorDown(void)
{
    ev3_motor_stop(M_M_PORT, true);
    KILL_MOTOR();
    ev3_motor_rotate(M_M_PORT, -M_D_DEGREE, M_D_POWER, true);
}
inline void MiddleMotorUp(void)
{
    ev3_motor_stop(M_M_PORT, true);
    KILL_MOTOR();
    ev3_motor_rotate(M_M_PORT, -M_U_DEGREE, M_U_POWER, true);
}
void DrawStraight(int bf, int centimeter, int power, int withPen) // 1 : forward, 0 : back
{
    if (!bf)
        centimeter *= -1;
    if (withPen)
        MiddleMotorDown();
    ev3_motor_rotate(L_M_PORT, (int)(centimeter / 17.5 * 360), power, false);
    ev3_motor_rotate(R_M_PORT, (int)(centimeter / 17.5 * 360), power, true);
    KILL_MOTOR();
    if (withPen)
        MiddleMotorUp();
}
inline void ForwardAndaWrite(float centimeter)
{
    int power = 20;
    MiddleMotorDown();
    // draw 1
    ev3_motor_rotate(L_M_PORT, (int)(centimeter / 17.5 * 360), power, false);
    ev3_motor_rotate(R_M_PORT, (int)(centimeter / 17.5 * 360), power, true);
    KILL_MOTOR();
    MiddleMotorUp();
}
inline void TurnRightWithGyro(int digree)
{
    ev3_gyro_sensor_reset(gyro_sensor);
    while (true)
    {
        ev3_motor_rotate(L_M_PORT, 10, 30, false);
        ev3_motor_rotate(R_M_PORT, -10, 30, true);
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
    ev3_gyro_sensor_reset(gyro_sensor);
    while (true)
    {
        ev3_motor_rotate(L_M_PORT, -10, 30, false);
        ev3_motor_rotate(R_M_PORT, 10, 30, true);
        if (ev3_gyro_sensor_get_angle(gyro_sensor) <= -digree)
        {
            KILL_MOTOR();
            break;
        }
        tslp_tsk(50);
    }
}
void run_task(intptr_t unused)
{
    int power = 20;
    char str[20];
    int distance = (int)ev3_ultrasonic_sensor_get_distance(ultraSonic_sensor);
    tslp_tsk(2000);
    ev3_motor_rotate(L_M_PORT, (int)(0.5 / 17.5 * 360), power, false);
    ev3_motor_rotate(R_M_PORT, (int)(0.5 / 17.5 * 360), power, true);
    KILL_MOTOR();
    distance = (int)ev3_ultrasonic_sensor_get_distance(ultraSonic_sensor);
    distance /= 10;
    sprintf(str, "%4d", distance);
    ev3_lcd_draw_string(str, 0, false);
    int length = 8;
    int backLength = 5;
    int npc = 5;
    DrawStraight(true, 5, power, false);
    switch (distance)
    {
    case 1:
    case 2:
    case 3:
        DrawStraight(true, length, power, true);
        DrawStraight(true, npc, power, false);
        if (distance == 1)
            break;
        TurnRightWithGyro(90);
        DrawStraight(true, npc, power, false);
        TurnRightWithGyro(90);
        DrawStraight(false, backLength, power, false);
        DrawStraight(true, length, power, true);
        DrawStraight(true, npc, power, false);
        if (distance == 2)
            break;
        TurnLeftWithGyro(90);
        DrawStraight(true, npc, power, false);
        TurnLeftWithGyro(70);
        DrawStraight(false, backLength, power, false);
        DrawStraight(true, length, power, true);
        DrawStraight(true, npc, power, false);
        break;
    case 4:
        DrawStraight(true, length, power, true);
        DrawStraight(true, npc, power, false);
        TurnRightWithGyro(90);
        DrawStraight(true, npc, power, false);
        TurnRightWithGyro(60);
        DrawStraight(false, backLength, power, false);
        DrawStraight(true, 9, power, true);
        DrawStraight(true, npc, power, false);
        TurnLeftWithGyro(90);
        DrawStraight(false, backLength, power, false);
        DrawStraight(true, 9, power, true);
        break;
    case 5:
    case 6:
    case 7:
    case 8:
        DrawStraight(true, length, power, false);
        DrawStraight(true, npc, power, false);
        TurnRightWithGyro(90);
        // DrawStraight(true, npc, power, false);
        TurnRightWithGyro(60);
        DrawStraight(false, backLength, power, false);
        DrawStraight(true, 9, power, true);
        DrawStraight(true, npc, power, false);
        TurnLeftWithGyro(90);
        DrawStraight(false, backLength, power, false);
        DrawStraight(true, 9, power, true);
        DrawStraight(true, npc, power, false);
        if (distance == 5)
            break;
        TurnRightWithGyro(60);
        DrawStraight(true, npc, power, false);
        TurnRightWithGyro(90);
        DrawStraight(false, backLength, power, false);
        DrawStraight(true, length, power, true);
        DrawStraight(true, npc, power, false);
        if (distance == 6)
            break;
        TurnLeftWithGyro(90);
        DrawStraight(true, npc, power, false);
        TurnLeftWithGyro(90);
        DrawStraight(false, backLength, power, false);
        DrawStraight(true, length, power, true);
        DrawStraight(true, npc, power, false);
        if (distance == 7)
            break;
        TurnRightWithGyro(90);
        DrawStraight(true, npc, power, false);
        TurnRightWithGyro(90);
        DrawStraight(false, backLength, power, false);
        DrawStraight(true, length, power, true);
        break;
    case 9:
        DrawStraight(true, length, power, true);
        DrawStraight(true, npc, power, false);
        TurnRightWithGyro(90);
        TurnRightWithGyro(60);
        DrawStraight(false, backLength, power, false);
        DrawStraight(true, 9, power, true);
        DrawStraight(true, npc, power, false);
        TurnLeftWithGyro(120);
        DrawStraight(false, backLength, power, false);
        DrawStraight(true, length, power, false);
        DrawStraight(true, npc, power, false);
        TurnLeftWithGyro(120);
        DrawStraight(false, backLength, power, false);
        DrawStraight(true, 9, power, false);
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
