/*
 *********************************************************************************************************
 *
 *
 MICRIUM BOARD SUPPORT PACKAGE
 *
 * Filename : bsp_switch.c
 *********************************************************************************************************
 */
/*
 *********************************************************************************************************
 *
 INCLUDE FILES
 *********************************************************************************************************
 */
#include <lib_def.h>
#include <cpu.h>

#include <bsp_int.h>
#include <os.h>
#include "bsp_switch.h"
#include "S32K144.h"
#include "bsp_timer.h"
/*
 *********************************************************************************************************
 *
 DEFINES
 *********************************************************************************************************
 */
volatile CPU_INT08U SW3_status = 0;
volatile CPU_INT08U SW2_status = 0;

extern int current_color;

/*
 *********************************************************************************************************
 *
 LOCAL FUNCTION PROTOTYPES
 *********************************************************************************************************
 */
static void SW_int_hdlr(void);
/*
 *********************************************************************************************************
 **
 GLOBAL FUNCTIONS
 *********************************************************************************************************
 */
/*
 *********************************************************************************************************
 *
 BSP_Switch_Init()
 *
 * Description : Initializes the required pins that control the switches.
 *
 * Argument(s) : none.
 *
 * Return(s)
 : none.
 *
 * Note(s)
 : none.
 *********************************************************************************************************
 */
void BSP_Switch_Init(void)
{
	PCC->PCCn[PCC_PORTC_INDEX] = PCC_PCCn_CGC_MASK;
	PTC->PDDR &= ~(DEF_BIT_12 | DEF_BIT_13);
	PORTC->PCR[12] = 0x100u;
	PORTC->PCR[13] = 0x100u;
	PORTC->PCR[13] |= 0x000b0000u;
	PORTC->PCR[12] |= 0x000b0000u;
	BSP_IntVectSet(PORTC_IRQn, 0, 0, SW_int_hdlr);
	BSP_IntEnable( PORTC_IRQn);
}
/*
 *********************************************************************************************************
 *
 BSP_Switch_Read()
 *
 * Description : Read the required pin that control the switches.
 *
 * Argument(s) : switch to be read.
 *
 * Return(s)
 : valiue of the switch (0 or 1).
 *
 * Note(s)
 : none.
 *********************************************************************************************************
 */
CPU_INT08U BSP_Switch_Read(BSP_SWITCH sw)
{
	switch (sw)
	{
	case SW2:
		return (((PTC->PDIR) >> 12) & 0x01u);
		break;
	case SW3:
		return (((PTC->PDIR) >> 13) & 0x01u);
		break;
	}
	return (0x00u);
}

static void SW_int_hdlr(void)
{
	//CPU_SR_ALLOC();
	//CPU_CRITICAL_ENTER();  //Used to protect current_color variable
	SW2_status = BSP_Switch_Read(SW2);
	SW3_status = BSP_Switch_Read(SW3);
	OSIntEnter();
	if ((PORTC->PCR[13]) & 0x01000000)
	{
		if(SW3_status == 1 && SW2_status == 0)
			BSP_PWM_COLOR((current_color+2)%3);
		PORTC->PCR[13] |= 0x01000000;
	}
	else if ((PORTC->PCR[12]) & 0x01000000)
	{
		if(SW3_status == 0 && SW2_status == 1)
			BSP_PWM_COLOR((current_color+1)%3);
		PORTC->PCR[12] |= 0x01000000;
	}
	//CPU_CRITICAL_EXIT();
	OSIntExit();
}

