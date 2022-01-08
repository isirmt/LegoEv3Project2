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
#define M_M_PORT EV3_PORT_B
#define R_M_PORT EV3_PORT_D
#define L_M_PORT EV3_PORT_A
#define STRAIGHT_POWER (20.0)
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
#define WAIT_TIME_MS (100.0)
void DrawStraight(int bf, int centimeter, int power, int withPen) // 1 : forward, 0 : BACK
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
void run_task(intptr_t unused)
{
	int16_t distance;
	char str[20];
	tslp_tsk(3000); // 待機用
	int bDigree = BACK;
	while (true)
	{
		STRAIGHT(360.0 / 17.5, STRAIGHT_POWER);
		KILL_MOTOR();
		distance = (int)ev3_ultrasonic_sensor_get_distance(ultraSonic_sensor);
		tslp_tsk(1000);
		sprintf(str, "%4d", distance);
		ev3_lcd_draw_string(str, 0, false);
		if (distance <= 7)
			break;
		KILL_MOTOR();
	}
	tslp_tsk(WAIT_TIME_MS);
	DrawStraight(false, 10, STRAIGHT_POWER, true);
	DrawStraight(true, 5, STRAIGHT_POWER, false);
	TurnLeftWithGyro(76);
	DrawStraight(false, BACK, STRAIGHT_POWER, false);
	DrawStraight(true, 10, STRAIGHT_POWER, true);
	DrawStraight(true, 3, STRAIGHT_POWER, false);
	TurnRightWithGyro(122);
	DrawStraight(false, BACK, STRAIGHT_POWER, false);
	DrawStraight(true, 14.1f, STRAIGHT_POWER, true);
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
