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

void DrawStar()
{
	float size = 22;
	char str[20];

	// draw 1
	float centimeter = 16;
	float npc = 5;
	float back = 5;
	int power = 40;

	DrawStraight(1, 25.7f, power / 2, 1);
	DrawStraight(1, npc, power / 2, 0);

	TurnRightWithGyro(145);
	DrawStraight(0, back, power / 2, 0);

	DrawStraight(1, 28.6f, power / 2, 1);
	DrawStraight(1, npc, power / 2, 0);

	TurnRightWithGyro(144);
	DrawStraight(0, back, power / 2, 0);

	DrawStraight(1, 29.3f, power / 2, 1);
	DrawStraight(1, npc, power / 2, 0);

	TurnRightWithGyro(139);
	DrawStraight(0, back, power / 2, 0);

	DrawStraight(1, 29.0f, power / 2, 1);
	DrawStraight(1, npc, power / 2, 0);

	TurnRightWithGyro(145);
	DrawStraight(0, back, power / 2, 0);

	DrawStraight(1, 27.3f, power / 2, 1);
}

void DrawTriangle()
{
	// draw 1
	float forward = 12;
	float back = 5;
	int power = 40;
	int triangleRotate = 120;
	int fDigree = forward;
	int bDigree = back;

	DrawStraight(1, 15.0f, power / 2, 1);
	DrawStraight(1, 5.0f, power / 2, 0);

	TurnRightWithGyro(135);
	DrawStraight(0, back, power / 2, 0);

	DrawStraight(1, 21.2f, power / 2, 1);
	DrawStraight(1, 5.0f, power / 2, 0);

	TurnRightWithGyro(135);
	DrawStraight(0, back, power / 2, 0);

	DrawStraight(1, 15.0f, power / 2, 1);

	// for (int i = 0; i < 3; i++)
	// {
	//     if (i == 1)
	//         DrawStraight(1, 25, power, 1);
	//     else
	//         DrawStraight(1, 15, power, 1);
	//     DrawStraight(1, 5, power, 0);
	//     TurnRightWithGyro(triangleRotate);
	//     // if ( i == 1) TurnRightWithGyro(40);
	//     DrawStraight(0, bDigree, power, 0); // back
	// }
}

void DrawSquare() // ok
{
	// draw 1
	float forward = 7;
	float back = 5;
	int power = 20;
	int squareRotate = 80;
	int fDigree = forward;
	int bDigree = back;

	for (int i = 0; i < 4; i++)
	{
		// if ( i == 2) DrawStraight(1, 22, power, 1);
		DrawStraight(1, 15, power, 1);
		DrawStraight(1, 5, power, 0);
		TurnRightWithGyro(squareRotate);
		// if ( i == 1) TurnLeftWithGyro(20);
		DrawStraight(0, bDigree, power, 0); // back
	}
}

void DrawCircle() // ok
{
	// draw 1
	float forward = 6;
	float back = -9;
	int power = 40;
	int circleRotate = 360;
	int fDigree = forward;
	int bDigree = back;

	DrawStraight(1, fDigree, power, 0);
	MiddleMotorDown();
	ev3_motor_rotate(L_M_PORT, 1600, 40, true);
	MiddleMotorUp();
	// DrawStraight(0, bDigree, power, 0); // back
}

void run_task(intptr_t unused)
{
	float centi = 15.0;
	float back = -9.0;
	int bc = (int)((BLACK + WHITE) / 2);
	int power = 40;
	float size = 22;
	float tireCir = 17.5;
	int shape = 0; // 1:circle 2:tri 3:sq 4:star
	while (true)
	{
		bc = 0;
		ev3_motor_rotate(L_M_PORT, (int)(1 / tireCir * 360), power, false);
		ev3_motor_rotate(R_M_PORT, (int)(1 / tireCir * 360), power, true);
		tslp_tsk(50);

		colorid_t now_color = ev3_color_sensor_get_color(color_sensor);
		switch (now_color)
		{
#if ROTATE_MODE
		case COLOR_BLACK:
			ev3_motor_rotate(L_M_PORT, -(int)(3.14 * size * 90 / 360), power, false);
			ev3_motor_rotate(R_M_PORT, (int)(3.14 * size * 90 / 360), power, true);
			ev3_motor_rotate(L_M_PORT, (int)(3 / tireCir * 360), power, false);
			ev3_motor_rotate(R_M_PORT, (int)(3 / tireCir * 360), power, true);
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
		if (bc == 1)
			break;
		tslp_tsk(WAIT_TIME_MS);
	}

	switch (shape)
	{
	case 1: // cir

		DrawStraight(1, 2, 40, 0);
		DrawCircle();
		break;
	case 2: // tri

		KILL_MOTOR();
		DrawTriangle();
		break;

	case 3: // sq

		DrawStraight(0, 2, 20, 0);
		KILL_MOTOR();
		DrawSquare();
		break;

	case 4:

		DrawStraight(0, 12.1f, 20, 0);
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
