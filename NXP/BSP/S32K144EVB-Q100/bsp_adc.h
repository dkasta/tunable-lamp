#ifndef BSP_ADC_PRESENT
#define BSP_ADC_PRESENT


void BSP_ADC0_init_interrupt(void);
void ADC0_IRQHandler(void);
void BSP_ADC0_convertAdcChan_interrupt(CPU_INT16U adcChan);

#endif
