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
#define color_sensor EV3_PORT_4
#define ultraSonic_sensor EV3_PORT_3

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

#define BLACK 6
#define WHITE 91

#define ROTATE_MODE 0

inline void DrawStraight(bool bf, float centimeter, int power, bool withPen)
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

// #define DrawStar() (DrawStraight(true, 25.7f, STRAIGHT_POWER / 2, true), DrawStraight(true, NPC, STRAIGHT_POWER / 2, 0), TurnRightWithGyro(145), DrawStraight(false, BACK, STRAIGHT_POWER / 2, 0), DrawStraight(true, 28.6f, STRAIGHT_POWER / 2, true), DrawStraight(true, NPC, STRAIGHT_POWER / 2, 0), TurnRightWithGyro(144), DrawStraight(false, BACK, STRAIGHT_POWER / 2, 0), DrawStraight(true, 29.3f, STRAIGHT_POWER / 2, true), DrawStraight(true, NPC, STRAIGHT_POWER / 2, 0), TurnRightWithGyro(139), DrawStraight(false, BACK, STRAIGHT_POWER / 2, 0), DrawStraight(true, 29.0f, STRAIGHT_POWER / 2, true), DrawStraight(true, NPC, STRAIGHT_POWER / 2, 0), TurnRightWithGyro(145), DrawStraight(false, BACK, STRAIGHT_POWER / 2, 0), DrawStraight(true, 27.3f, STRAIGHT_POWER / 2, true))
inline void DrawStar()
{
	DrawStraight(true, 25.7f, STRAIGHT_POWER / 2, true);
	DrawStraight(true, NPC, STRAIGHT_POWER / 2, 0);
	TurnRightWithGyro(145);
	DrawStraight(false, BACK, STRAIGHT_POWER / 2, 0);
	DrawStraight(true, 28.6f, STRAIGHT_POWER / 2, true);
	DrawStraight(true, NPC, STRAIGHT_POWER / 2, 0);
	TurnRightWithGyro(144);
	DrawStraight(false, BACK, STRAIGHT_POWER / 2, 0);
	DrawStraight(true, 29.3f, STRAIGHT_POWER / 2, true);
	DrawStraight(true, NPC, STRAIGHT_POWER / 2, 0);
	TurnRightWithGyro(139);
	DrawStraight(false, BACK, STRAIGHT_POWER / 2, 0);
	DrawStraight(true, 29.0f, STRAIGHT_POWER / 2, true);
	DrawStraight(true, NPC, STRAIGHT_POWER / 2, 0);
	TurnRightWithGyro(145);
	DrawStraight(false, BACK, STRAIGHT_POWER / 2, 0);
	DrawStraight(true, 27.3f, STRAIGHT_POWER / 2, true);
}
void DrawTriangle()
{
	// draw 1
	int power = 40;

	DrawStraight(true, 15.0f, power / 2, 1);
	DrawStraight(true, 5.0f, power / 2, 0);

	DrawStraight(true, 15.0f, STRAIGHT_POWER / 2, true);
	DrawStraight(true, 5.0f, STRAIGHT_POWER / 2, 0);

	TurnRightWithGyro(135);
	DrawStraight(false, BACK, STRAIGHT_POWER / 2, 0);

	DrawStraight(true, 21.2f, STRAIGHT_POWER / 2, true);
	DrawStraight(true, 5.0f, STRAIGHT_POWER / 2, 0);

	TurnRightWithGyro(135);
	DrawStraight(false, BACK, STRAIGHT_POWER / 2, 0);

	DrawStraight(true, 15.0f, STRAIGHT_POWER / 2, true);
}

void DrawSquare() // ok
{
	// draw 1
	int squareRotate = 80;

	for (int i = 0; i < 4; i++)
	{
		// if ( i == 2) DrawStraight(true, 22, STRAIGHT_POWER,true);
		DrawStraight(true, 15, STRAIGHT_POWER, true);
		DrawStraight(true, 5, STRAIGHT_POWER, 0);
		TurnRightWithGyro(squareRotate);
		// if ( i == 1) TurnLeftWithGyro(20);
		DrawStraight(false, BACK, STRAIGHT_POWER, 0); // back
	}
}

void DrawCircle() // ok
{
	// draw 1
	int fDigree = 6;

	DrawStraight(true, fDigree, STRAIGHT_POWER, 0);
	MiddleMotorDown();
	ev3_motor_rotate(L_M_PORT, 1600, 40, true);
	MiddleMotorUp();
	// DrawStraight(false,bDigree, STRAIGHT_POWER, 0), // back
}

void run_task(intptr_t unused)
{
	int bc = (int)((BLACK + WHITE) / 2);

	float tireCir = 17.5;
	int shape = 0; // 1:circle 2:tri 3:sq 4:star
	while (true)
	{
		bc = 0;
		ev3_motor_rotate(L_M_PORT, (int)(1 / tireCir * 360), STRAIGHT_POWER, false);
		ev3_motor_rotate(R_M_PORT, (int)(1 / tireCir * 360), STRAIGHT_POWER, true);
		tslp_tsk(50);

		colorid_t now_color = ev3_color_sensor_get_color(color_sensor);
		switch (now_color)
		{
#if ROTATE_MODE
		case COLOR_BLACK:
			ev3_motor_rotate(L_M_PORT, -(int)(3.14 * size * 90 / 360), STRAIGHT_POWER, false);
			ev3_motor_rotate(R_M_PORT, (int)(3.14 * size * 90 / 360), STRAIGHT_POWER, true);
			ev3_motor_rotate(L_M_PORT, (int)(3 / tireCir * 360), STRAIGHT_POWER, false);
			ev3_motor_rotate(R_M_PORT, (int)(3 / tireCir * 360), STRAIGHT_POWER, true);
			bc = 0;
			break;
#else
#endif

		case COLOR_RED:
			shape = 1;
			bc = 1;
			break;
		case COLOR_GREEN:
			shape = 2;
			bc = 1;
			break;
		case COLOR_YELLOW:
			shape = 3;
			bc = 1;
			break;
		case COLOR_BLUE:
			shape = 4;
			bc = 1;
			break;
		default:
			break;
		}
		if (bc)
			break;
		tslp_tsk(WAIT_TIME_MS);
	}

	switch (shape)
	{
	case 1: // cir

		DrawStraight(true, 2, 40, 0);
		DrawCircle();
		break;
	case 2: // tri

		KILL_MOTOR();
		DrawTriangle();
		break;

	case 3: // sq

		DrawStraight(false, 2, 20, 0);
		KILL_MOTOR();
		DrawSquare();
		break;

	case 4:

		DrawStraight(false, 12.1f, 20, 0);
		KILL_MOTOR();
		DrawStar();
		break;
	}
}

void main_task(intptr_t unused)
{
	ev3_sensor_config(gyro_sensor, GYRO_SENSOR);
	ev3_motor_config(L_M_PORT, LARGE_MOTOR);
	ev3_motor_config(R_M_PORT, LARGE_MOTOR);
	ev3_motor_config(M_M_PORT, MEDIUM_MOTOR);
	ev3_sensor_config(color_sensor, COLOR_SENSOR);

	act_tsk(RUN_TASK);
}
