#ifndef _TIMER_H_
#define _TIMER_H_


extern void delay(int num);
extern void mdelay(int num);
extern void vic_init(void);
extern int timer_init(void);
extern void stop_timer(void);
extern void start_timer(void);
extern void pwm3_out_init(void);
extern void pwm_duty_add(void);
extern void pwm_duty_sub(void);


#endif /*_TIMER_H_*/

