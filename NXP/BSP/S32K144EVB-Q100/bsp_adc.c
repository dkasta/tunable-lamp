#include <lib_def.h>
#include <os.h>
#include <bsp_adc.h>
#include <S32K144.h>
#include <bsp_timer.h>
#include  <bsp_int.h>

int ADC0_adc_chx;
OS_SEM ADC0sem;

void BSP_ADC0_init_interrupt(void)
{
	PCC->PCCn[PCC_ADC0_INDEX] &= ~PCC_PCCn_CGC_MASK;
	PCC->PCCn[PCC_ADC0_INDEX] |= PCC_PCCn_PCS(1);
	PCC->PCCn[PCC_ADC0_INDEX] |= PCC_PCCn_CGC_MASK;
	ADC0->SC1[0] = 0x00007F;
	ADC0->CFG1 = 0x000000004;
	ADC0->CFG2 = 0x00000000C;
	ADC0->SC2 = 0x00000000;
	ADC0->SC3 = 0x00000000;
	BSP_IntVectSet(ADC0_IRQn, 0, 0, ADC0_IRQHandler);
	BSP_IntEnable(ADC0_IRQn);
}

void BSP_ADC0_convertAdcChan_interrupt(CPU_INT16U adcChan)
{ /* For SW trigger mode, SC1[0] isused*/
	ADC0->SC1[0] &= ~ADC_SC1_ADCH_MASK; /* Clear priorADCH bits */
	ADC0->SC1[0] = ADC_SC1_ADCH(adcChan) | 0x40; /* InitiateConversion and AIEN=1*/
}


void ADC0_IRQHandler(void)
{
	CPU_INT16U adc_result;
	OS_ERR os_err;
	OSIntEnter();
	adc_result = ADC0->R[0];
	//set adc range to the one of the PWM counter
	ADC0_adc_chx = (CPU_INT16U) (((8000) * adc_result) / 0xFFF);
	ADC0->SC1[0] = 0x00007F;
	OSSemPost(&ADC0sem, OS_OPT_POST_1 | OS_OPT_POST_NO_SCHED, &os_err);
	OSIntExit();
}
