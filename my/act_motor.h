#ifndef _ACT_MOTOR_H_
#define _ACT_MOTOR_H_

//typedef struct{
//    char V1;
//    char V2;
//    int  L1;
//    int  L2;
//}Exe;

// Exe exe={0};

/**
 * @param pre_target_val p
 * @param target_val t
 * @param actual_val a
 */
typedef enum
{
    enum__local=0,
    Go_local,
    Wait ,  // a=a;  t=t_in;  // clear actual location to 0,then run to 0 pos
    Zero_GO,  // a=0;  t=t_in //zero to start
    Abs_GO,
    Accum_GO, // a=a;  t=a+t_inter;//accumulate previous history  distance
    Correct_GO, //
    Turn_Left,   // a=0,  t=t_in
    Turn_Right,  // a=0,  t=t_in
    // Wait_currentPos=0, // a=a;  t=p;  // don't clear actual location now,then run to 0
    enum_local_end, //location control

    enum_speed,
    Go_speed,
    speed_debug,
    enum_speed_end,    //speed control

    enum_angle,    
    Go_angle,
    Turn_Left_Angle,   // a=0,  t=t_in
    Turn_Right_Angle,  // a=0,  t=t_in
    enum_angle_end, 

    ActNum,
} Action_Mode;

 
void MotorDrive(float tarV1,float tarL1,float tarV2,float tarL2, float tarAngle);
void CAR_RUN_Local(Action_Mode action,float tarV, float tarL1,float tarL2,float angle);

void CAR_TURN_Left();
void CAR_RUN(Action_Mode action,float tarV1, float tarL1, float tarV2, float tarL2,float target_angle);
#endif // _ACT_MOTOR_H_

