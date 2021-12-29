#include "ev3api.h"
#include "app.h"

#if defined(BUILD_MODULE)
#include "module_cfg.h"
#else
#include "kernel_cfg.h"
#endif

#define R_motor EV3_PORT_D
#define L_motor EV3_PORT_A
#define M_motor EV3_PORT_B

#define gyro_sensor EV3_PORT_2

#define M_M_PORT EV3_PORT_B
#define R_M_PORT EV3_PORT_D
#define L_M_PORT EV3_PORT_A
#define G_S_PORT EV3_PORT_4
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

void run_task(intptr_t unused)
{
    float size = 22;
    char str[20];

    //draw 1
	float centimeter = 16;
	float npc = 5;
	float back = -6;
    int power = 40;
    ev3_motor_stop(M_M_PORT, true);
    int32_t startval_m = ev3_motor_get_counts(M_M_PORT);
    KILL_MOTOR();
    ev3_motor_rotate(M_M_PORT, -M_D_DEGREE, M_D_POWER, true);
    // draw 1
	int digree = (int)(centimeter / 17.5 * 360);
	int digb = (int)(back / 17.5 * 360);
	int din = (int)(npc / 17.5 * 360);
	ev3_motor_rotate(R_motor, digree, power, false);
    ev3_motor_rotate(L_motor, digree, power, true);
    
    Stop();
    MiddleMotorUp();
	
	ev3_motor_rotate(R_motor, din, power, false);
    ev3_motor_rotate(L_motor, din, power, true);
	Stop();
    // rotate
    TurnRightWithGyro(133);
	
	ev3_motor_rotate(R_motor, digb, power, false);
    ev3_motor_rotate(L_motor, digb, power, true);
    // draw 2
	MiddleMotorDown();
     ev3_motor_rotate(L_motor, digree, power, false);
    ev3_motor_rotate(R_motor, digree, power, true);
    Stop();
	MiddleMotorUp();
	
	ev3_motor_rotate(R_motor, din, power, false);
    ev3_motor_rotate(L_motor, din, power, true);
	Stop();
    // rotate
    TurnRightWithGyro(126);
	ev3_motor_rotate(R_motor, digb, power, false);
    ev3_motor_rotate(L_motor, digb, power, true);
    // draw 3
	MiddleMotorDown();
     ev3_motor_rotate(L_motor, digree, power, false);
    ev3_motor_rotate(R_motor, digree, power, true);
    Stop();
	MiddleMotorUp();
	
	ev3_motor_rotate(R_motor, din, power, false);
    ev3_motor_rotate(L_motor, din, power, true);
	Stop();
    // rotate
    TurnRightWithGyro(120);
	ev3_motor_rotate(R_motor, digb, power, false);
    ev3_motor_rotate(L_motor, digb, power, true);
    // draw 4
	MiddleMotorDown();
     ev3_motor_rotate(L_motor, digree, power, false);
    ev3_motor_rotate(R_motor, digree, power, true);
    Stop();
	MiddleMotorUp();
	
	ev3_motor_rotate(R_motor, din, power, false);
    ev3_motor_rotate(L_motor, din, power, true);
	Stop();
    // rotate
    TurnRightWithGyro(133);
	ev3_motor_rotate(R_motor, digb, power, false);
    ev3_motor_rotate(L_motor, digb, power, true);
    // draw 5
	MiddleMotorDown();
     ev3_motor_rotate(L_motor, digree, power, false);
    ev3_motor_rotate(R_motor, digree, power, true);
    Stop();
	MiddleMotorUp();
}

void main_task(intptr_t unused)
{
    ev3_sensor_config(gyro_sensor, GYRO_SENSOR);
    ev3_motor_config(L_motor, LARGE_MOTOR);
    ev3_motor_config(R_motor, LARGE_MOTOR);
    ev3_motor_config(M_motor, MEDIUM_MOTOR);

    act_tsk(RUN_TASK);
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

inline void Stop()
{
    ev3_motor_stop(L_motor, true);
    ev3_motor_stop(R_motor, true);
    tslp_tsk(WAIT_TIME_MS);
}

void ForwardAndaWrite(float centimeter)
{
    int power = 40;
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
	tslp_tsk(1000);
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
