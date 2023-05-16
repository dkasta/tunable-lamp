#include <lib_def.h>
#include <bsp_int.h>
#include <os.h>
#include "bsp_timer.h"
#include "S32K144.h"
void FTM0_IRQHandler_Single_Edge(void);
volatile CPU_INT32S FTM0_CH0_period;

int current_color = COLOR_RED;
int current_CnV = 0;


void BSP_PWM_LED_INIT( void )
{
	PCC->PCCn[PCC_PORTD_INDEX] = PCC_PCCn_CGC_MASK;
	PCC->PCCn[PCC_FTM0_INDEX] = PCC_PCCn_PCS(6) | PCC_PCCn_CGC_MASK;

	PORTD->PCR[15] = PORT_PCR_MUX(2); //RED PWM
	PORTD->PCR[16] = PORT_PCR_MUX(2); //GREEN PWM
	PORTD->PCR[0] = PORT_PCR_MUX(2); //BLU PWM

	//Enable FTMEN mode in order to set default values to registers
	FTM0->MODE = FTM_MODE_FTMEN_MASK;
	//Set pulse frequency to 10 kHz  (fck = 80 MHz)
	FTM0->MOD = FTM_MOD_MOD(8000-1);
	//Enable syncronization - needed for duty cycle update
	FTM0->COMBINE = FTM_COMBINE_SYNCEN0_MASK | FTM_COMBINE_SYNCEN1_MASK | FTM_COMBINE_SYNCEN2_MASK;

	//Set the lowest value of the counter to all zeros
	FTM0->CNTIN = FTM_CNTIN_INIT(0);

	//Enable up-counting mode and output = 1 when CNT > CnV (ELSA set to 1)
	FTM0->CONTROLS[0].CnSC = FTM_CnSC_MSB_MASK | FTM_CnSC_ELSA_MASK;
	FTM0->CONTROLS[1].CnSC = FTM_CnSC_MSB_MASK | FTM_CnSC_ELSA_MASK;
	FTM0->CONTROLS[2].CnSC = FTM_CnSC_MSB_MASK | FTM_CnSC_ELSA_MASK;

	//Set initial duty cycle to 0%
	FTM0->CONTROLS[0].CnV = FTM_CnV_VAL(0);
	FTM0->CONTROLS[1].CnV = FTM_CnV_VAL(0);
	FTM0->CONTROLS[2].CnV = FTM_CnV_VAL(0);

	FTM0->CNT = 0;
	FTM0->SYNC = FTM_SYNC_CNTMAX_MASK;
	FTM0->SYNCONF = FTM_SYNCONF_SWWRBUF_MASK | FTM_SYNCONF_SYNCMODE_MASK;
	FTM0->SC = FTM_SC_CLKS(1) | FTM_SC_PWMEN0_MASK | FTM_SC_PWMEN1_MASK | FTM_SC_PWMEN2_MASK;

}

void BSP_PWM_CnV(int CnV)
{
	FTM0->CONTROLS[current_color].CnV = FTM_CnV_VAL(CnV);
	FTM0->SYNC |= FTM_SYNC_SWSYNC_MASK;
}

void BSP_PWM_COLOR(int color)
{
	current_color = color;
	FTM0->CONTROLS[color].CnV = FTM_CnV_VAL(current_CnV);
	FTM0->CONTROLS[(color+1)%3].CnV = FTM_CnV_VAL(0);
	FTM0->CONTROLS[(color+2)%3].CnV = FTM_CnV_VAL(0);
	FTM0->SYNC |= FTM_SYNC_SWSYNC_MASK;
}

