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
*                                    MICRIUM BOARD SUPPORT PACKAGE
*
* Filename : bsp_led.h
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                               MODULE
*
* Note(s) : (1) This header file is protected from multiple pre-processor inclusion through use of the
*               BSP present pre-processor macro definition.
*********************************************************************************************************
*/

#ifndef  BSP_LED_PRESENT
#define  BSP_LED_PRESENT


/*
*********************************************************************************************************
*                                            INCLUDE FILES
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                     EXTERNAL C LANGUAGE LINKAGE
*
* Note(s) : (1) C++ compilers MUST 'extern'ally declare ALL C function prototypes & variable/object
*               declarations for correct C language linkage.
*********************************************************************************************************
*/

#ifdef __cplusplus
extern  "C" {                                                   /* See Note #1.                        */
#endif


/*
*********************************************************************************************************
*                                               DEFINES
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                              DATA TYPES
*********************************************************************************************************
*/

typedef  enum  bsp_led {
    LED_RED,                                                    /* RED: 1, GREEN: 0, BLUE: 0           */
    LED_GREEN,                                                  /* RED: 0, GREEN: 1, BLUE: 0           */
    LED_BLUE,                                                   /* RED: 0, GREEN: 0, BLUE: 1           */
	LED_YELLOW,                                                 /* RED: 1, GREEN: 1, BLUE: 0           */
	LED_PURPLE,                                                 /* RED: 1, GREEN: 0, BLUE: 1           */
	LED_AQUA,                                                   /* RED: 0, GREEN: 1, BLUE: 1           */
	LED_WHITE,                                                  /* RED: 1, GREEN: 1, BLUE: 1           */
	LED_ALL
} BSP_LED;


/*
*********************************************************************************************************
*                                         FUNCTION PROTOTYPES
*********************************************************************************************************
*/

void  BSP_LED_Init  (void);

void  BSP_LED_On    (BSP_LED  color);

void BSP_LED_Off(BSP_LED color);


void  BSP_LED_Toggle(BSP_LED  color);


/*
*********************************************************************************************************
*                                   EXTERNAL C LANGUAGE LINKAGE END
*********************************************************************************************************
*/

#ifdef __cplusplus
}                                                               /* End of 'extern'al C lang linkage.   */
#endif


/*
*********************************************************************************************************
*                                              MODULE END
*********************************************************************************************************
*/

#endif                                                          /* End of module include.              */
