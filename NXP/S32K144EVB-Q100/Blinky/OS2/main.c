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
*                                              uC/OS-II
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

static OS_TCB blinkRedTCB;
static CPU_STK blinkRedSTK[APP_CFG_STARTUP_TASK_STK_SIZE];

static OS_TCB blinkGreenTCB;
static CPU_STK blinkGreenSTK[APP_CFG_STARTUP_TASK_STK_SIZE];



#include  <cpu.h>
#include  <lib_mem.h>
#include  <os.h>
#include  <bsp_os.h>
#include  <bsp_clk.h>
#include  <bsp_int.h>
#include  <bsp_led.h>

#include  "../app_cfg.h"


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

static  OS_STK  StartupTaskStk[APP_CFG_STARTUP_TASK_STK_SIZE];


/*
*********************************************************************************************************
*                                         FUNCTION PROTOTYPES
*********************************************************************************************************
*/

static  void  StartupTask (void  *p_arg);
static void blinkRed(void *p_arg);
static void blinkGreen(void *p_arg);

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
#if OS_TASK_NAME_EN > 0u
    CPU_INT08U  os_err;
#endif

    BSP_ClkInit();                             /* Initialize the main clock.                           */
    BSP_IntInit();                             /* Initialize the main clock.                           */
    BSP_OS_TickInit();                         /* Initialize kernel tick timer.                        */

    Mem_Init();                                /* Initialize Memory Managment Module                   */
    CPU_IntDis();                              /* Disable all Interrupts.                              */
    CPU_Init();                                /* Initialize the uC/CPU services                       */

    OSInit();                                  /* Initialize uC/OS-II                                  */

    OSTaskCreateExt( StartupTask,              /* Create the startup task                              */
                     0,
                    &StartupTaskStk[APP_CFG_STARTUP_TASK_STK_SIZE - 1u],
                     APP_CFG_STARTUP_TASK_PRIO,
                     APP_CFG_STARTUP_TASK_PRIO,
                    &StartupTaskStk[0u],
                     APP_CFG_STARTUP_TASK_STK_SIZE,
                     0u,
                    (OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR));

#if OS_TASK_NAME_EN > 0u
    OSTaskNameSet(         APP_CFG_STARTUP_TASK_PRIO,
                  (INT8U *)"Startup Task",
                           &os_err);
#endif

    OS_ERR errRed;


    OSStart();                                 /* Start multitasking (i.e. give control to uC/OS-II).  */

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
    CPU_INT08U  rgb_led_color;


   (void)p_arg;

    OS_TRACE_INIT();                           /* Initialize Trace recorder                            */

    BSP_OS_TickEnable();                       /* Enable the tick timer and interrupt                  */

#if (OS_TASK_STAT_EN > 0u)
    OSStatInit();                              /* Determine CPU capacity                               */
#endif

#ifdef CPU_CFG_INT_DIS_MEAS_EN
    CPU_IntDisMeasMaxCurReset();
#endif

    BSP_LED_Init();                            /* Initialize RGB LED                                   */
    OSTaskCreate(&blinkRedTCB,
        			"BlinkRed",
    				blinkRed,
    				(void *) 0,
    				APP_CFG_STARTUP_TASK_PRIO,
    				blinkRedSTK,
    				APP_CFG_STARTUP_TASK_STK_SIZE/10,
    				APP_CFG_STARTUP_TASK_STK_SIZE,
    				(OS_MSG_QTY) 0,
    				(OS_TICK) 0,
    				(void *) 0,
    				OS_OPT_TASK_STK_CHK + OS_OPT_TASK_STK_CLR,
    				&errRed);


}

//Blink red and green

void blinkRed(void *p_arg)
{
	while(1)
	{
		BSP_LED_On(LED_RED);
		OSTimeDlyHMSM(0, 0, 0, 500u);
		BSP_LED_Off(LED_RED);
		OSTimeDlyHMSM(0, 0, 0, 500u);
	}
}

void blinkGreen(void *p_arg)
{
	OS_ERR err;
	while(DEF_TRUE)
	{
		BSP_LED_On(LED_GREEN);
		OSTimeDlyHMSM(0, 0, 0, 500u, OS_OPT_TIME_HMSM_STRICT,  &err);
		BSP_LED_Off(LED_GREEN);
		OSTimeDlyHMSM(0, 0, 0, 500u, OS_OPT_TIME_HMSM_STRICT, &err);
	}
}


