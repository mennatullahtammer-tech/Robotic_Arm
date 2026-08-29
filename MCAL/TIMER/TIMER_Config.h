#ifndef MCAL_TIMER_TIMER_CONFIG_H_
#define MCAL_TIMER_TIMER_CONFIG_H_

/* ================= Timer1 Servo ================= */

#define TIMER1_SERVO_TOP       19999

#define TIMER1_SERVO_MIN       1000
#define TIMER1_SERVO_MAX       2000


/* ================= Timer2 Servo 3 ================= */

/*
 * Timer2 tick = 100 us
 *
 * 10 ticks  = 1 ms
 * 20 ticks  = 2 ms
 *
 * Period = 200 ticks = 20 ms
 */

#define TIMER2_SERVO3_MIN_TICKS    10
#define TIMER2_SERVO3_MAX_TICKS    20
#define TIMER2_SERVO3_PERIOD_TICKS 200

#endif
