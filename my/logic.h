#ifndef _LOGIC_H_
#define _LOGIC_H_

typedef enum TASK_TOTAL
{
    Stop,

/*-------------------------------*///第一圈
    BackOut_1 = 1, //出库

    ScanCode_1, //扫码2
    
    RunToRawMaterial_1_Pick, //原料区，抓取装载物料，装车，转弯进入下一个直道

    RunToRoughArea_1,  //跑到粗加工域 放置，抓取，转弯进入下一个直道

    RunToFineArea_1,    //跑到精加工域 放置，，转弯进入下一个直道


/*-------------------------------*///第二圈
    RunToCircle2_Start, //跑到第二圈直道启点 

    RunToRawMaterial_2_Pick,//第二圈，原料区，抓取装载物料，装车，转弯进入下一个直道

    RunToRoughArea_2, //第二圈，跑到粗加工域 放置，抓取，转弯进入下一个直道

    RunToFineArea_2,   //第二圈 跑到精加工域 放置，，转弯进入下一个直道, 下一部就是入库结束


/*------------------------------ *///返回 结束
    RunToRest,     //从最后一个直道 回到 休息区，比赛任务结束|| 前一个结束的任务可以是 FineArea 类型任务



/*------------------------------*///其他杂项 调试项
    Base,

    Test,   //测试模式
    Speed,  //测速
    Debug, //debug模式
    task_max,
}task_enum;

typedef enum CAR_DO
{
    do_00=0,
    do_01,
    do_02,
    do_03,
    do_04,
    do_05,
    do_06,
    do_07,
    do_08,
    do_09,
    do_10,
    do_11,
    do_12,
    
    do_Arm,
    finish_Arm,

    do_ScanCode,
    finish_Scancode,

    do_max,
}do_enum;

 



#endif // _LOGIC_H_