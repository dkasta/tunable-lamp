#ifndef BSP_TIMER_PRESENT
#define BSP_TIMER_PRESENT

#define COLOR_RED 0
#define COLOR_GREEN 1
#define COLOR_BLUE 2

#ifdef __cplusplus
extern "C"
{
#endif
	extern volatile CPU_INT32S FTM0_CH0_period;
	void BSP_PWM_LED_INIT( void );
	void BSP_PWM_CnV(int module);
	void BSP_PWM_COLOR(int color);

#ifdef __cplusplus
}
#endif

#endif

