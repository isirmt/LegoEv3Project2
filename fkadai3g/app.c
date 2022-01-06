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
	if (bf == 0)
		centimeter *= -1;
	if (withPen == 1)
		MiddleMotorDown();
	ev3_motor_rotate(L_motor, (int)(centimeter / 17.5 * 360), power, false);
	ev3_motor_rotate(R_motor, (int)(centimeter / 17.5 * 360), power, true);
	KILL_MOTOR();
	if (withPen == 1)
		MiddleMotorUp();
}

inline void ForwardAndaWrite(float centimeter)
{
	int power = 40;
	MiddleMotorDown();
	// draw 1
	ev3_motor_rotate(L_motor, (int)(centimeter / 17.5 * 360), power, false);
	ev3_motor_rotate(R_motor, (int)(centimeter / 17.5 * 360), power, true);
	KILL_MOTOR();
	MiddleMotorUp();
}

inline void TurnRightWithGyro(int digree)
{
	ev3_gyro_sensor_reset(gyro_sensor);
	while (true)
	{
		ev3_motor_rotate(L_motor, 10, 30, false);
		ev3_motor_rotate(R_motor, -10, 30, true);
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
		ev3_motor_rotate(L_motor, -10, 30, false);
		ev3_motor_rotate(R_motor, 10, 30, true);
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
	float back = 5;
	int16_t distance;
	distance = (int)ev3_ultrasonic_sensor_get_distance(ultraSonic_sensor);
	tslp_tsk(3000); // 待機用
	distance = 20;
	int bDigree = back;

	while (true)
	{
		ev3_motor_rotate(L_motor, (int)(1 / 17.5 * 360), power, false);
		ev3_motor_rotate(R_motor, (int)(1 / 17.5 * 360), power, true);
		KILL_MOTOR();

		distance = (int)ev3_ultrasonic_sensor_get_distance(ultraSonic_sensor);
		tslp_tsk(1000);

		if (distance <= 10)
		{
			KILL_MOTOR();
			break;
		}
		tslp_tsk(WAIT_TIME_MS);
	}

	tslp_tsk(WAIT_TIME_MS);

	DrawStraight(0, 10, power, 1);
	DrawStraight(1, 5, power, 0);

	TurnLeftWithGyro(90);
	DrawStraight(0, bDigree, power, 0);

	DrawStraight(1, 10, power, 1);
	DrawStraight(1, 3, power, 0);

	TurnRightWithGyro(135);
	DrawStraight(0, bDigree, power, 0);

	DrawStraight(1, (int)(10.0f * 1.41f), power, 1);
}

void main_task(intptr_t unused)
{
	ev3_sensor_config(gyro_sensor, GYRO_SENSOR);
	ev3_motor_config(L_motor, LARGE_MOTOR);
	ev3_motor_config(R_motor, LARGE_MOTOR);
	ev3_motor_config(M_motor, MEDIUM_MOTOR);

	ev3_sensor_config(ultraSonic_sensor, ULTRASONIC_SENSOR);

	act_tsk(RUN_TASK);
}
