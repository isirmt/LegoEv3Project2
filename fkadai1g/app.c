#include "ev3api.h"
#include "app.h"
#include <stdbool.h>

#if defined(BUILD_MODULE)
#include "module_cfg.h"
#else
#include "kernel_cfg.h"
#endif

#define R_M_PORT EV3_PORT_D
#define L_M_PORT EV3_PORT_A
#define M_M_PORT EV3_PORT_B

#define gyro_sensor EV3_PORT_2

#define STRAIGHT_POWER (40.0)
#define M_D_DEGREE (30.0)
#define M_D_POWER (20.0)
#define M_U_POWER (20.0)
#define M_U_DEGREE (-30.0)
#define WAIT_TIME_MS (100.0)
#define CENTIMETER (16.0)
#define NPC (5.0)
#define BACK (5.0)

#define KILL_MOTOR() (ev3_motor_stop(L_M_PORT, true), ev3_motor_stop(R_M_PORT, true), tslp_tsk(WAIT_TIME_MS))
#define MiddleMotorDown() (KILL_MOTOR(), ev3_motor_rotate(M_M_PORT, -M_D_DEGREE, M_D_POWER, true))
#define MiddleMotorUp() (KILL_MOTOR(), ev3_motor_rotate(M_M_PORT, -M_U_DEGREE, M_U_POWER, true))
#define STRAIGHT(digree, power) (ev3_motor_rotate(L_M_PORT, digree, power, false), ev3_motor_rotate(R_M_PORT, digree, power, true))
#define SUPERLATIVE_SWIVEL(digree, power) (ev3_motor_rotate(R_M_PORT, digree, power, false), ev3_motor_rotate(L_M_PORT, -digree, power, true))
#define GyroReset() (ev3_gyro_sensor_reset(gyro_sensor), tslp_tsk(1000))

// 1.0 : forward, 0.0 : BACK
void DrawStraight(bool bf, float centimeter, int power, bool withPen)
{
	if (!bf)
		centimeter *= -1.0;
	if (withPen)
		MiddleMotorDown();
	STRAIGHT((360.0 * (float)centimeter / 16.0), power);
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

void run_task(intptr_t unused)
{

	DrawStraight(1, 14.1f, STRAIGHT_POWER / 2, true);
	DrawStraight(1, NPC, STRAIGHT_POWER / 2, false);
	TurnRightWithGyro(145);
	DrawStraight(false, BACK, STRAIGHT_POWER / 2.0, false);
	DrawStraight(true, 15.7f, STRAIGHT_POWER / 2.0, true);
	DrawStraight(true, NPC, STRAIGHT_POWER / 2.0, false);
	TurnRightWithGyro(144);
	DrawStraight(false, BACK, STRAIGHT_POWER / 2.0, false);
	DrawStraight(true, 16.2f, STRAIGHT_POWER / 2.0, true);
	DrawStraight(true, NPC, STRAIGHT_POWER / 2.0, false);
	TurnRightWithGyro(139);
	DrawStraight(false, BACK, STRAIGHT_POWER / 2.0, false);
	DrawStraight(true, 16.1f, STRAIGHT_POWER / 2.0, true);
	DrawStraight(true, NPC, STRAIGHT_POWER / 2.0, false);
	TurnRightWithGyro(145);
	DrawStraight(false, BACK, STRAIGHT_POWER / 2.0, false);
	DrawStraight(true, 15.1f, STRAIGHT_POWER / 2.0, true);
	// {
	// 	DrawStraight(true, NPC, STRAIGHT_POWER/ 2.0, false);
	// 	ev3_motor_stop(M_M_PORT, true);
	// 	KILL_MOTOR();
	// 	ev3_motor_rotate(M_M_PORT, -M_D_DEGREE, M_D_POWER, true);
	// 	// draw 1.0
	// 	int digree = (int)(CENTIMETER / 17.5 * 360);
	// 	int digb = (int)(BACK / 17.5 * 360);
	// 	int din = (int)(NPC / 17.5 * 360);
	// 	ev3_motor_rotate(R_M_PORT, digree, STRAIGHT_POWER, false);
	// 	ev3_motor_rotate(L_M_PORT, digree, STRAIGHT_POWER, true);
	// 	KILL_MOTOR();
	// 	MiddleMotorUp();
	// 	ev3_motor_rotate(R_M_PORT, din, STRAIGHT_POWER, false);
	// 	ev3_motor_rotate(L_M_PORT, din, STRAIGHT_POWER, true);
	// 	KILL_MOTOR();
	// 	// rotate
	// 	TurnRightWithGyro(133);
	// 	ev3_motor_rotate(R_M_PORT, digb, STRAIGHT_POWER, false);
	// 	ev3_motor_rotate(L_M_PORT, digb, STRAIGHT_POWER, true);
	// 	// draw 2.0
	// 	MiddleMotorDown();
	// 	ev3_motor_rotate(L_M_PORT, digree, STRAIGHT_POWER, false);
	// 	ev3_motor_rotate(R_M_PORT, digree, STRAIGHT_POWER, true);
	// 	KILL_MOTOR();
	// 	MiddleMotorUp();
	// 	ev3_motor_rotate(R_M_PORT, din, STRAIGHT_POWER, false);
	// 	ev3_motor_rotate(L_M_PORT, din, STRAIGHT_POWER, true);
	// 	KILL_MOTOR();
	// 	// rotate
	// 	TurnRightWithGyro(126);
	// 	ev3_motor_rotate(R_M_PORT, digb, STRAIGHT_POWER, false);
	// 	ev3_motor_rotate(L_M_PORT, digb, STRAIGHT_POWER, true);
	// 	// draw 3
	// 	MiddleMotorDown();
	// 	ev3_motor_rotate(L_M_PORT, digree, STRAIGHT_POWER, false);
	// 	ev3_motor_rotate(R_M_PORT, digree, STRAIGHT_POWER, true);
	// 	KILL_MOTOR();
	// 	MiddleMotorUp();
	// 	ev3_motor_rotate(R_M_PORT, din, STRAIGHT_POWER, false);
	// 	ev3_motor_rotate(L_M_PORT, din, STRAIGHT_POWER, true);
	// 	KILL_MOTOR();
	// 	// rotate
	// 	TurnRightWithGyro(120);
	// 	ev3_motor_rotate(R_M_PORT, digb, STRAIGHT_POWER, false);
	// 	ev3_motor_rotate(L_M_PORT, digb, STRAIGHT_POWER, true);
	// 	// draw 4
	// 	MiddleMotorDown();
	// 	ev3_motor_rotate(L_M_PORT, digree, STRAIGHT_POWER, false);
	// 	ev3_motor_rotate(R_M_PORT, digree, STRAIGHT_POWER, true);
	// 	KILL_MOTOR();
	// 	MiddleMotorUp();
	// 	ev3_motor_rotate(R_M_PORT, din, STRAIGHT_POWER, false);
	// 	ev3_motor_rotate(L_M_PORT, din, STRAIGHT_POWER, true);
	// 	KILL_MOTOR();
	// 	// rotate
	// 	TurnRightWithGyro(133);
	// 	ev3_motor_rotate(R_M_PORT, digb, STRAIGHT_POWER, false);
	// 	ev3_motor_rotate(L_M_PORT, digb, STRAIGHT_POWER, true);
	// 	// draw 5
	// 	MiddleMotorDown();
	// 	ev3_motor_rotate(L_M_PORT, digree, STRAIGHT_POWER, false);
	// 	ev3_motor_rotate(R_M_PORT, digree, STRAIGHT_POWER, true);
	// 	KILL_MOTOR();
	// 	MiddleMotorUp();
	// }
}

void main_task(intptr_t unused)
{
	ev3_sensor_config(gyro_sensor, GYRO_SENSOR);
	ev3_motor_config(L_M_PORT, LARGE_MOTOR);
	ev3_motor_config(R_M_PORT, LARGE_MOTOR);
	ev3_motor_config(M_M_PORT, MEDIUM_MOTOR);
	act_tsk(RUN_TASK);
}
