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

#define BLACK 6
#define WHITE 91

#define ROTATE_MODE 0

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
    ev3_motor_rotate(L_motor, (int)((float)centimeter / 16.0f * 360.0f), power, false);
    ev3_motor_rotate(R_motor, (int)((float)centimeter / 16.0f * 360.0f), power, true);
    Stop();
    if (withPen == 1)
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

inline void TurnLeftWithGyro(int digree)
{
    ev3_gyro_sensor_reset(gyro_sensor);
	 tslp_tsk(1000);
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

void DrawStar()
{
    /*// draw 1
    float forward = 15;
    float back = -9;
    int power = 40;
    int starRotate = 130;
    int fDigree = (int)(forward / 17.5 * 360);
    int bDigree = (int)(back / 17.5 * 360);

    for (int i = 0; i < 5; i++)
    {
        DrawStraight(1, fDigree, power, 1);
        TurnRightWithGyro(starRotate);
        DrawStraight(0, bDigree, power, 0); // back
    }*/
    // draw sample
    /*MiddleMotorDown();
    ev3_motor_rotate(L_motor, fDigree, power, false);
    ev3_motor_rotate(R_motor, fDigree, power, true);
    Stop();
    MiddleMotorUp();
    // rotate
    TurnRightWithGyro(starRotate);
    ev3_motor_rotate(R_motor, bDigree, power, false);
    ev3_motor_rotate(L_motor, bDigree, power, true);*/
    	
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

void DrawTriangle()
{
    //draw 1
    float forward = 12;
    float back = 6;
    int power = 40;
    int triangleRotate = 120;
    int fDigree = forward;
    int bDigree = back;
	
	for (int i = 0; i < 3; i++)
    {
    	if ( i == 1) DrawStraight(1, 25, power, 1);
    	else DrawStraight(1, 15, power, 1);
	DrawStraight(1, 3, power, 0);
    TurnRightWithGyro(triangleRotate);
    	//if ( i == 1) TurnRightWithGyro(40);
    DrawStraight(0, bDigree, power, 0); // back
    }
}

void DrawSquare()
{
    // draw 1
    float forward = 7;
    float back = 6;
    int power = 40;
    int squareRotate = 80;
    int fDigree = forward;
    int bDigree = back;

	for (int i = 0; i < 4; i++)
    {
    	//if ( i == 2) DrawStraight(1, 22, power, 1);
    	  DrawStraight(1, 15, power, 1);
	DrawStraight(1, 3, power, 0);
    TurnRightWithGyro(squareRotate);
    	//if ( i == 1) TurnLeftWithGyro(20);
    DrawStraight(0, bDigree, power, 0); // back
    }
}

void DrawCircle()
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
    ev3_motor_rotate(L_motor, 1600, 40, true);
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
    while (1)
    {
        bc = 0;
        ev3_motor_rotate(L_motor, (int)(1 / tireCir * 360), power, false);
        ev3_motor_rotate(R_motor, (int)(1 / tireCir * 360), power, true);
    	tslp_tsk(50);

        colorid_t now_color = ev3_color_sensor_get_color(color_sensor);
        switch (now_color)
        {
#if ROTATE_MODE
        case COLOR_BLACK:
            ev3_motor_rotate(L_motor, -(int)(3.14 * size * 90 / 360), power, false);
            ev3_motor_rotate(R_motor, (int)(3.14 * size * 90 / 360), power, true);
            ev3_motor_rotate(L_motor, (int)(3 / tireCir * 360), power, false);
            ev3_motor_rotate(R_motor, (int)(3 / tireCir * 360), power, true);
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
        // forward
        /*ev3_motor_rotate(L_motor, (int)(5 / tireCir * 360), power, false);
        ev3_motor_rotate(R_motor, (int)(5 / tireCir * 360), power, true);
        Stop();

        // draw
        tslp_tsk(WAIT_TIME_MS);
        ev3_motor_rotate(L_motor, (int)(3.14 * size * 90 / 360), power, false);
        ev3_motor_rotate(R_motor, -(int)(3.14 * size * 90 / 360), power, true);
        Stop();
        tslp_tsk(WAIT_TIME_MS);
        MiddleMotorDown();
        ev3_motor_rotate(L_motor, -(int)(3.14 * (size + 15) * 360 / 360), power, false); // 直径15
        ev3_motor_rotate(R_motor, (int)(3.14 * 15 * 360 / 360), power, true);
        Stop();
        MiddleMotorUp();
        break;*/

        DrawStraight(1, 2, 40, 0);
        DrawCircle();
        break;
    case 2: // tri
        /*ev3_motor_rotate(L_motor, (int)(12.4 / tireCir * 360), power, false);
        ev3_motor_rotate(R_motor, (int)(12.4 / tireCir * 360), power, true);

        ev3_motor_rotate(L_motor, -(int)(3.14 * size * 135 / 360), power, false);
        ev3_motor_rotate(R_motor, (int)(3.14 * size * 135 / 360), power, true);

        // draw 1
        ForwardAndaWrite(23.46);

        TurnLeftWithGyro(135);

        // draw 2
        ForwardAndaWrite(33.18);

        TurnLeftWithGyro(135);

        // draw 3
        ForwardAndaWrite(23.46);
        break;*/

        //ev3_motor_rotate(R_motor, 120, power, true);
    	//DrawStraight(0, 5, 40, 0);
    	Stop();
        DrawTriangle();
        break;

    case 3: // sq
        /*ev3_motor_rotate(L_motor, (int)(16 / tireCir * 360), power, false);
        ev3_motor_rotate(R_motor, (int)(16 / tireCir * 360), power, true);

        TurnLeftWithGyro(135);
        tslp_tsk(WAIT_TIME_MS);

        for (int i = 0; i < 4; i++)
        {
            ForwardAndaWrite(19.6);

            if (i == 3)
                break;

            TurnLeftWithGyro(90);
        }
        MiddleMotorUp();
        break;*/

        
    	//ev3_motor_rotate(R_motor, 120, power, true);
    	//DrawStraight(0, 5, 40, 0);
    	Stop();
        //TurnLeftWithGyro(45);
        DrawSquare();
        break;

    case 4:

        /*MiddleMotorDown();
        // draw 1
        int digree = (int)(centi / 17.5 * 360);
        int digb = (int)(back / 17.5 * 360);
        ev3_motor_rotate(R_motor, digree, power, false);
        ev3_motor_rotate(L_motor, digree, power, true);

        Stop();
        MiddleMotorUp();
        // rotate
        TurnRightWithGyro(130);

        ev3_motor_rotate(R_motor, digb, power, false);
        ev3_motor_rotate(L_motor, digb, power, true);
        // draw 2
        MiddleMotorDown();
        ev3_motor_rotate(L_motor, digree, power, false);
        ev3_motor_rotate(R_motor, digree, power, true);
        Stop();
        MiddleMotorUp();
        // rotate
        TurnRightWithGyro(130);
        ev3_motor_rotate(R_motor, digb, power, false);
        ev3_motor_rotate(L_motor, digb, power, true);
        // draw 3
        MiddleMotorDown();
        ev3_motor_rotate(L_motor, digree, power, false);
        ev3_motor_rotate(R_motor, digree, power, true);
        Stop();
        MiddleMotorUp();
        // rotate
        TurnRightWithGyro(130);
        ev3_motor_rotate(R_motor, digb, power, false);
        ev3_motor_rotate(L_motor, digb, power, true);
        // draw 4
        MiddleMotorDown();
        ev3_motor_rotate(L_motor, digree, power, false);
        ev3_motor_rotate(R_motor, digree, power, true);
        Stop();
        MiddleMotorUp();
        // rotate
        TurnRightWithGyro(130);
        ev3_motor_rotate(R_motor, digb, power, false);
        ev3_motor_rotate(L_motor, digb, power, true);
        // draw 5
        MiddleMotorDown();
        ev3_motor_rotate(L_motor, digree, power, false);
        ev3_motor_rotate(R_motor, digree, power, true);
        Stop();
        MiddleMotorUp();

        break;*/

        //DrawStraight(0, 3, 40, 0);
        //TurnLeftWithGyro(40);
        DrawStar();
        break;
    }
}

void main_task(intptr_t unused)
{
    ev3_sensor_config(gyro_sensor, GYRO_SENSOR);
    ev3_motor_config(L_motor, LARGE_MOTOR);
    ev3_motor_config(R_motor, LARGE_MOTOR);
    ev3_motor_config(M_motor, MEDIUM_MOTOR);
    ev3_sensor_config(color_sensor, COLOR_SENSOR);

    act_tsk(RUN_TASK);
}
