/*
 * TEEvent.h
 *
 *  Created on: Dec 29, 2020
 *      Author: TwoEars
 */

#ifndef TEMAINTHREAD_H_
#define TEMAINTHREAD_H_

#include <stdint.h>
#include <stdio.h>

//事件类型
typedef enum
{
	STOP_Event,
	SINGLE_Event,
	LOOP_Event
}TEEvent_Type;

//事件类
typedef volatile struct
{
	TEEvent_Type type;				//类型
	uint8_t inQueue;			//是否在队列中
	uint32_t cycle;				//触发周期
	uint32_t count;				//计时器
	void(*func)(void);			//handle函数指针
	volatile void *next;		//nextָ指针
}TEEvent;

/*
 * 创建事件
 */
#define	TEEvent_New(n)		void n##_Handle(void);\
							TEEvent n = {STOP_Event,0,0,0,n##_Handle,NULL};\
							void n##_Handle(void)


/*
 * （延时）开启事件
 * time_ms:延时时间（单位：毫秒），为0时立即开启事件
 */
void TEEvent_Start(TEEvent *event,TEEvent_Type type,uint32_t time_ms);

/*
 * ֹͣ停止事件
 */
void TEEvent_Stop(TEEvent *event);

/*
 * 1ms触发器
 * 用来计时，需要放在1ms定时器中断中
 */
void TEEvent_Trigger1MS(void);

/*
 * 主线程循环
 * 用来执行事件，需要放在main函数主循环中
 */
void TEMainThread_Event(void);


#endif /* TEEVENT_H_ */
