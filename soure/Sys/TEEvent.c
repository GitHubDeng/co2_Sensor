/*
 * TEEvent.c
 *
 *  Created on: Dec 29, 2020
 *      Author: TwoEars
 */
#include "TEEvent.h"

TEEvent *PQueueHead = NULL;		//队列头指针
TEEvent *PQueueTail = NULL;		//队列尾指针

/*
 * 事件挂钩（入列）
 */
static void Hook(TEEvent *event)
{
	//事件部分成员初始化
	event->next = NULL;
	if(PQueueHead == NULL)//队列空
	{
		//队列加入第一个事件
		PQueueHead = event;		
		PQueueTail = event;
	}
	else
	{
		PQueueTail->next = event;	//队尾next指针指向事件
		PQueueTail = event;			//队尾指针指向事件
	}
	event->inQueue = 1;
}

void TEEvent_Start(TEEvent *event,TEEvent_Type type,uint32_t time_ms)
{
	event->type = type;
	event->cycle = time_ms+1;
	event->count = time_ms+1;			//设置事件延时触发时间
	if(event->inQueue == 0)				//时间不在队列中
	{
		Hook(event);				//挂钩
	}
}

void TEEvent_Stop(TEEvent *event)
{
	event->type = STOP_Event;
	event->count = 1;
}

void TEEvent_Trigger1MS(void)
{	
	TEEvent *temp_p;
	for(temp_p= PQueueHead;temp_p!=NULL;temp_p=temp_p->next)//扫描整个队列
	{
		if(temp_p->count > 1)	//计时没结束
		{
			temp_p->count--;	//计时器计时
		}
	}
}

void TEMainThread_Event(void)
{
	TEEvent *temp_p;
	for(temp_p= PQueueHead;temp_p!=NULL;temp_p=temp_p->next)//遍历队列
	{
		if(temp_p->type == STOP_Event)
		{
			continue;
		}
		/*
		 * 执行
		 */
		if(temp_p->count <= 1)
		{
			if(temp_p->type == SINGLE_Event)
			{
				temp_p->type = STOP_Event;
			}
			else if(temp_p->type == LOOP_Event)
			{
				temp_p->count = temp_p->cycle;
			}
			temp_p->func();			//调用钩子
		}
	}
}

