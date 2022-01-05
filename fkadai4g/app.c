#include "ev3api.h"
#include "app.h"

#if defined(BUILD_MODULE)
#include "module_cfg.h"
#else
#include "kernel_cfg.h"
#endif

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

inline void Stop()
{
    ev3_motor_stop(L_motor, true);
    ev3_motor_stop(R_motor, true);
    tslp_tsk(WAIT_TIME_MS);
}

inline void MiddleMotorDown(void)
{
    ev3_motor_stop(M_M_PORT, true);
    int32_t startval_m = ev3_motor_get_counts(M_M_PORT);
    KILL_MOTOR();
    ev3_motor_rotate(M_M_PORT, -M_D_DEGREE, M_D_POWER, true);
}
inline void MiddleMotorUp(void)
{
    ev3_motor_stop(M_M_PORT, true);
    int32_t startval_m = ev3_motor_get_counts(M_M_PORT);
    KILL_MOTOR();
    ev3_motor_rotate(M_M_PORT, -M_U_DEGREE, M_U_POWER, true);
}

void DrawStraight(int bf, int centimeter, int power, int withPen) // 1 : forward, 0 : back
{
    if (bf == 0)
        centimeter *= -1;
    if (withPen == 1)
        MiddleMotorDown();
    ev3_motor_rotate(L_motor, (int)(centimeter / 17.5 * 360), power, false);
    ev3_motor_rotate(R_motor, (int)(centimeter / 17.5 * 360), power, true);
    Stop();
    if (withPen == 1)
        MiddleMotorUp();
}

inline void ForwardAndaWrite(float centimeter)
{
    int power = 20;
    float tireCir = 17.5;
    MiddleMotorDown();
    // draw 1
    ev3_motor_rotate(L_motor, (int)(centimeter / 17.5 * 360), power, false);
    ev3_motor_rotate(R_motor, (int)(centimeter / 17.5 * 360), power, true);
    Stop();
    MiddleMotorUp();
}

inline void TurnRightWithGyro(int digree)
{
    ev3_gyro_sensor_reset(gyro_sensor);
    while (1)
    {
        ev3_motor_rotate(L_motor, 10, 30, false);
        ev3_motor_rotate(R_motor, -10, 30, true);
        if (ev3_gyro_sensor_get_angle(gyro_sensor) >= digree)
        {
            Stop();
            break;
        }
        tslp_tsk(50);
    }
}

inline void TurnLeftWithGyro(int digree)
{
    ev3_gyro_sensor_reset(gyro_sensor);
    while (1)
    {
        ev3_motor_rotate(L_motor, -10, 30, false);
        ev3_motor_rotate(R_motor, 10, 30, true);
        if (ev3_gyro_sensor_get_angle(gyro_sensor) <= -digree)
        {
            Stop();
            break;
        }
        tslp_tsk(50);
    }
}

void run_task(intptr_t unused)
{
    int power = 20;
    float size = 22;
    int shape = 0; // 1:circle 2:tri 3:sq 4:star
    int distance = (int)ev3_ultrasonic_sensor_get_distance(ultraSonic_sensor);
    tslp_tsk(2000);
    ev3_motor_rotate(L_motor, (int)(0.5 / 17.5 * 360), power, false);
    ev3_motor_rotate(R_motor, (int)(0.5 / 17.5 * 360), power, true);
    Stop();

    distance = (int)ev3_ultrasonic_sensor_get_distance(ultraSonic_sensor);

    distance /= 10;

    int length = 8;
    int backLength = 5;
    int npc = 5;

    DrawStraight(1, 5, power, 0);

    switch (distance)
    {
    case 1:
    case 2:
    case 3:
        DrawStraight(1, length, power, 1);
        DrawStraight(1, npc, power, 0);

        if (distance == 1)
            break;
        TurnRightWithGyro(90);
        DrawStraight(1, npc, power, 0);

        TurnRightWithGyro(90);
        DrawStraight(0, backLength, power, 0);

        DrawStraight(1, length, power, 1);
        DrawStraight(1, npc, power, 0);

        if (distance == 2)
            break;
        TurnLeftWithGyro(90);
        DrawStraight(1, npc, power, 0);

        TurnLeftWithGyro(90);
        DrawStraight(0, backLength, power, 0);

        DrawStraight(1, length, power, 1);
        DrawStraight(1, npc, power, 0);

        break;
    case 4:
        DrawStraight(1, length, power, 1);
        DrawStraight(1, npc, power, 0);

        TurnRightWithGyro(90);
        DrawStraight(1, npc, power, 0);

        TurnRightWithGyro(60);
        DrawStraight(0, backLength, power, 0);

        DrawStraight(1, 9, power, 1);
        DrawStraight(1, npc, power, 0);

        TurnLeftWithGyro(90);
        DrawStraight(0, backLength, power, 0);

        DrawStraight(1, 9, power, 1);
        break;
    case 5:
    case 6:
    case 7:
    case 8:
        DrawStraight(1, length, power, 0);
        DrawStraight(1, npc, power, 0);

        TurnRightWithGyro(90);
        //DrawStraight(1, npc, power, 0);

        TurnRightWithGyro(60);
        DrawStraight(0, backLength, power, 0);

        DrawStraight(1, 9, power, 1);
        DrawStraight(1, npc, power, 0);

        TurnLeftWithGyro(90);
        DrawStraight(0, backLength, power, 0);

        DrawStraight(1, 9, power, 1);
        DrawStraight(1, npc, power, 0);

        if (distance == 5)
            break;
        TurnRightWithGyro(60);
        DrawStraight(1, npc, power, 0);

        TurnRightWithGyro(90);
        DrawStraight(0, backLength, power, 0);

        DrawStraight(1, length, power, 1);
        DrawStraight(1, npc, power, 0);

        if (distance == 6)
            break;

        TurnLeftWithGyro(90);
        DrawStraight(1, npc, power, 0);

        TurnLeftWithGyro(90);
        DrawStraight(0, backLength, power, 0);

        DrawStraight(1, length, power, 1);
        DrawStraight(1, npc, power, 0);

        if (distance == 7)
            break;

        TurnRightWithGyro(90);
        DrawStraight(1, npc, power, 0);

        TurnRightWithGyro(90);
        DrawStraight(0, backLength, power, 0);

        DrawStraight(1, length, power, 1);

        break;
    case 9:
        DrawStraight(1, length, power, 1);
        DrawStraight(1, npc, power, 0);

        TurnRightWithGyro(90);

        TurnRightWithGyro(60);
        DrawStraight(0, backLength, power, 0);

        DrawStraight(1, 9, power, 1);
        DrawStraight(1, npc, power, 0);

        TurnLeftWithGyro(120);
        DrawStraight(0, backLength, power, 0);

        DrawStraight(1, length, power, 0);
        DrawStraight(1, npc, power, 0);

        TurnLeftWithGyro(120);
        DrawStraight(0, backLength, power, 0);

        DrawStraight(1, 9, power, 0);
        break;
    default:
        break;
    }
    tslp_tsk(WAIT_TIME_MS);
}

void main_task(intptr_t unused)
{
    // ev3_sensor_config(gyro_sensor,GYRO_SENSOR);
    ev3_motor_config(L_motor, LARGE_MOTOR);
    ev3_motor_config(R_motor, LARGE_MOTOR);
    ev3_motor_config(M_motor, MEDIUM_MOTOR);

    ev3_sensor_config(ultraSonic_sensor, ULTRASONIC_SENSOR);

    act_tsk(RUN_TASK);
}
