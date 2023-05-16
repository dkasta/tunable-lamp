/*
*********************************************************************************************************
*                                            EXAMPLE CODE
*
*               This file is provided as an example on how to use Micrium products.
*
*               Please feel free to use any application code labeled as 'EXAMPLE CODE' in
*               your application products.  Example code may be used as is, in whole or in
*               part, or may be used as a reference only. This file can be modified as
*               required to meet the end-product requirements.
*
*               Please help us continue to provide the Embedded community with the finest
*               software available.  Your honesty is greatly appreciated.
*
*               You can find our product's user manual, API reference, release notes and
*               more information at https://doc.micrium.com.
*               You can contact us at www.micrium.com.
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*
*                                             uC/OS-III
*                                            EXAMPLE CODE
*
* Filename : main.c
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                            INCLUDE FILES
*********************************************************************************************************
*/

#include  <cpu.h>
#include  <lib_mem.h>
#include  <os.h>
#include  <bsp_os.h>
#include  <bsp_clk.h>
#include  <bsp_int.h>
#include  <bsp_led.h>
#include <bsp_switch.h>
#include <bsp_timer.h>
#include <bsp_adc.h>

#include  "os_app_hooks.h"
#include  "../app_cfg.h"

extern CPU_INT08U SW3_status;
extern int ADC0_adc_chx;
extern OS_SEM ADC0sem;

/*
*********************************************************************************************************
*                                            LOCAL DEFINES
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                       LOCAL GLOBAL VARIABLES
*********************************************************************************************************
*/

static  OS_TCB   StartupTaskTCB;
static  CPU_STK  StartupTaskStk[APP_CFG_STARTUP_TASK_STK_SIZE];

void blinkRed(void * p_arg);
void blinkGreen(void * p_arg);

/*
*********************************************************************************************************
*                                         FUNCTION PROTOTYPES
*********************************************************************************************************
*/

static  void  StartupTask (void  *p_arg);


/*
*********************************************************************************************************
*                                                main()
*
* Description : This is the standard entry point for C code.  It is assumed that your code will call
*               main() once you have performed all necessary initialization.
*
* Arguments   : none
*
* Returns     : none
*
* Notes       : none
*********************************************************************************************************
*/

int  main (void)
{
    OS_ERR  os_err;


    BSP_ClkInit();                             /* Initialize the main clock.                           */
    BSP_IntInit();
    BSP_OS_TickInit();                         /* Initialize kernel tick timer                         */

    Mem_Init();                                /* Initialize Memory Managment Module                   */
    CPU_IntDis();                              /* Disable all Interrupts.                              */
    CPU_Init();                                /* Initialize the uC/CPU services                       */

    OSInit(&os_err);                           /* Initialize uC/OS-III                                 */
    if (os_err != OS_ERR_NONE) {
        while (1);
    }

    App_OS_SetAllHooks();                      /* Set all applications hooks                           */

    OSTaskCreate(&StartupTaskTCB,              /* Create the startup task                              */
                 "Startup Task",
                  StartupTask,
                  0u,
                  APP_CFG_STARTUP_TASK_PRIO,
                 &StartupTaskStk[0u],
                  StartupTaskStk[APP_CFG_STARTUP_TASK_STK_SIZE / 10u],
                  APP_CFG_STARTUP_TASK_STK_SIZE,
                  0u,
                  0u,
                  0u,
                 (OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
                 &os_err);
    if (os_err != OS_ERR_NONE) {
        while (1);
    }


    OSStart(&os_err);                          /* Start multitasking (i.e. give control to uC/OS-III)  */

    while (DEF_ON) {                           /* Should Never Get Here.                               */
        ;
    }
}


/*
*********************************************************************************************************
*                                            STARTUP TASK
*
* Description : This is an example of a startup task.  As mentioned in the book's text, you MUST
*               initialize the ticker only once multitasking has started.
*
* Arguments   : p_arg   is the argument passed to 'StartupTask()' by 'OSTaskCreate()'.
*
* Returns     : none
*
* Notes       : 1) The first line of code is used to prevent a compiler warning because 'p_arg' is not
*                  used.  The compiler should not generate any code for this statement.
*********************************************************************************************************
*/

static  void  StartupTask (void *p_arg)
{
    OS_ERR      os_err;

   (void)p_arg;

    OS_TRACE_INIT();                           /* Initialize Trace recorder                            */

    BSP_OS_TickEnable();

    /* Enable the tick timer and interrupt                  */


#if OS_CFG_STAT_TASK_EN > 0u
    OSStatTaskCPUUsageInit(&os_err);           /* Compute CPU capacity with no task running            */
#endif

#ifdef CPU_CFG_INT_DIS_MEAS_EN
    CPU_IntDisMeasMaxCurReset();
#endif

    OSSemCreate(&ADC0sem, "ADC0 Sempahore", 0u, &os_err);
    BSP_Switch_Init();

    BSP_ADC0_init_interrupt();
    BSP_PWM_LED_INIT();
    while(1)
    {
    	//Start conversion
		BSP_ADC0_convertAdcChan_interrupt(12);
		//Wait for conversion to be completed
		OSSemPend(&ADC0sem, 0u, OS_OPT_PEND_BLOCKING, 0u, &os_err);
		//Update duty cycle
		BSP_PWM_CnV(ADC0_adc_chx);
    }

    while(1)
    	;
}
