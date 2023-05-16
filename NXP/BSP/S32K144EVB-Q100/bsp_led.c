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
* Filename : bsp_led.c
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                            INCLUDE FILES
*********************************************************************************************************
*/

#include  <lib_def.h>
#include  "bsp_led.h"
#include  "S32K144.h"


/*
*********************************************************************************************************
*                                               DEFINES
*********************************************************************************************************
*/

#define  BSP_LED_RGB_RED_PIN      DEF_BIT_15
#define  BSP_LED_RGB_GREEN_PIN    DEF_BIT_16
#define  BSP_LED_RGB_BLUE_PIN     DEF_BIT_00

#define  BSP_LED_RGB_ALL_PINS    (BSP_LED_RGB_RED_PIN   | \
	                              BSP_LED_RGB_GREEN_PIN | \
								  BSP_LED_RGB_BLUE_PIN)


/*
*********************************************************************************************************
*                                      LOCAL FUNCTION PROTOTYPES
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*********************************************************************************************************
**                                         GLOBAL FUNCTIONS
*********************************************************************************************************
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                           BSP_LED_Init()
*
* Description : Initializes the required pins that control the RGB LED.
*
* Argument(s) : none.
*
* Return(s)   : none.
*
* Note(s)     : none.
*********************************************************************************************************
*/


void BSP_LED_Init(void)
{
	PCC->PCCn[PCC_PORTD_INDEX] = PCC_PCCn_CGC_MASK; /* Enable clock to PORT D */
	PTD->PDDR |= (BSP_LED_RGB_RED_PIN |
	/* Set PD15 (RGB_RED) as an output pin */
	BSP_LED_RGB_GREEN_PIN |
	/* Set PD16 (RGB_GREEN) as an output pin*/
	BSP_LED_RGB_BLUE_PIN);
	/* Set PD0 (RGB_BLUE) as an output pin */

	/* Configure PD15 (RGB_RED) as a GPIO function */
	PORTD->PCR[15] = 0x100u;
	/* Configure PD16 (RGB_GREEN) as a GPIO function*/
	PORTD->PCR[16] = 0x100u;
	/* Configure PD0 (RGB_BLUE) as a GPIO function */
	PORTD->PCR[0] = 0x100u;

	BSP_LED_Off(LED_ALL);
}


/*
*********************************************************************************************************
*                                            BSP_LED_On()
*
* Description : Turn ON the RGB LED in a specific color.
*
* Argument(s) : color    The color you want the RGB LED to show.
*
*                  LED_RED       Turns on RGB_LED in RED color
*                  LED_GREEN     Turns on RGB_LED in GREEN color
*                  LED_BLUE      Turns on RGB_LED in BLUE color
*                  LED_YELLOW    Turns on RGB_LED in YELLOW color
*                  LED_PURPLE    Turns on RGB_LED in PURPLE color
*                  LED_AQUA      Turns on RGB_LED in AQUA color
*                  LED_WHITE     Turns on RGB_LED in WHITE color
*                  LED_ALL       Turns on RGB_LED in WHITE color
*
* Return(s)   : none.
*
* Note(s)     : none.
*********************************************************************************************************
*/

void BSP_LED_On(BSP_LED color)
{
//BSP_LED_Off();
	switch (color)
	{
	case LED_RED:
		PTD->PCOR |= BSP_LED_RGB_RED_PIN;
		break;
	case LED_GREEN:
		PTD->PCOR |= BSP_LED_RGB_GREEN_PIN;
		break;
	case LED_BLUE:
		PTD->PCOR |= BSP_LED_RGB_BLUE_PIN;
		break;
	case LED_YELLOW:
		PTD->PCOR |= BSP_LED_RGB_RED_PIN | BSP_LED_RGB_GREEN_PIN;
		break;
	case LED_PURPLE:
		PTD->PCOR |= BSP_LED_RGB_RED_PIN | BSP_LED_RGB_BLUE_PIN;
		break;
	case LED_AQUA:
		PTD->PCOR |= BSP_LED_RGB_GREEN_PIN | BSP_LED_RGB_BLUE_PIN;
		break;
	case LED_WHITE:
	default:
		PTD->PCOR |= BSP_LED_RGB_ALL_PINS;
		break;
	}
}



/*
*********************************************************************************************************
*                                            BSP_LED_Off()
*
* Description : Turns OFF the RGB LED.
*
* Argument(s) : none.
*
* Return(s)   : none.
*
* Note(s)     : none.
*********************************************************************************************************
*/

void BSP_LED_Off(BSP_LED color)
{
	switch (color)
	{
	case LED_RED:
		PTD->PSOR |= BSP_LED_RGB_RED_PIN;
		break;
	case LED_GREEN:
		PTD->PSOR |= BSP_LED_RGB_GREEN_PIN;
		break;
	case LED_BLUE:
		PTD->PSOR |= BSP_LED_RGB_BLUE_PIN;
		break;
	case LED_ALL:
	default:
		PTD->PSOR = BSP_LED_RGB_ALL_PINS;
		break;
	}
}

/*
*********************************************************************************************************
*                                          BSP_LED_Toggle()
*
* Description : Toggles the RGB LED in a specific color.
*
* Argument(s) : color    The color you want the RGB LED to show.
*
*                  LED_RED       Toggle RGB_LED in RED color
*                  LED_GREEN     Toggle RGB_LED in GREEN color
*                  LED_BLUE      Toggle RGB_LED in BLUE color
*                  LED_YELLOW    Toggle RGB_LED in YELLOW color
*                  LED_PURPLE    Toggle RGB_LED in PURPLE color
*                  LED_AQUA      Toggle RGB_LED in AQUA color
*                  LED_WHITE     Toggle RGB_LED in WHITE color
*                  LED_ALL       Toggle RGB_LED in WHITE color
*
* Return(s)   : none.
*
* Note(s)     : none.
*********************************************************************************************************
*/

void  BSP_LED_Toggle (BSP_LED  color)
{
	switch (color) {
	    case LED_RED:
	    	 PTD->PSOR |= BSP_LED_RGB_GREEN_PIN | BSP_LED_RGB_BLUE_PIN;
             PTD->PTOR |= BSP_LED_RGB_RED_PIN;
	    	 break;


	    case LED_GREEN:
	    	 PTD->PSOR |= BSP_LED_RGB_RED_PIN | BSP_LED_RGB_BLUE_PIN;
	    	 PTD->PTOR |= BSP_LED_RGB_GREEN_PIN;
	    	 break;


	    case LED_BLUE:
	    	 PTD->PSOR |= BSP_LED_RGB_RED_PIN | BSP_LED_RGB_GREEN_PIN;
	    	 PTD->PTOR |= BSP_LED_RGB_BLUE_PIN;
	    	 break;


	    case LED_YELLOW:
	    	 PTD->PSOR |= BSP_LED_RGB_BLUE_PIN;
	    	 PTD->PTOR |= BSP_LED_RGB_RED_PIN | BSP_LED_RGB_GREEN_PIN;
	    	 break;


	    case LED_PURPLE:
	    	 PTD->PSOR |= BSP_LED_RGB_GREEN_PIN;
	    	 PTD->PTOR |= BSP_LED_RGB_RED_PIN | BSP_LED_RGB_BLUE_PIN;
	    	 break;


	    case LED_AQUA:
	    	 PTD->PSOR |= BSP_LED_RGB_RED_PIN;
	    	 PTD->PTOR |= BSP_LED_RGB_GREEN_PIN | BSP_LED_RGB_BLUE_PIN;
	    	 break;


	    case LED_WHITE:
	    default:
	    	 PTD->PTOR |= BSP_LED_RGB_ALL_PINS;
	    	 break;
	}
}
