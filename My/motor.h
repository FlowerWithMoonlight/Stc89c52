#ifndef __MOTOR_H_
#define __MOTOR_H_

/*  - 定义针脚 - */



/*  - 关键词 - */
#define MotorPin P1


/*  - 方法  - */
/* 步进电机启动函数，angle-需转过的角度 */
void StartMotor(signed long angle);

/* 步进电机停止函数 */
void StopMotor();

/* 电机转动控制函数 */
void TurnMotor();





