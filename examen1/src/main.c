/**********************************************
 * Abraham P�rez A01633926

Activamos con pulsos el puente h
Motor derecho PORTC
7 adelante
15 atras

Motor izquierdo PORTC
14 adelante
17 atras

Potenciometro 1 ADC0
PTB13 canal (8)
1-127 adelante
128-255 atras

Poteciometro 2 ADC1
PTB14 canal (9)
1-127derecha
128-255 izquierda

Alto total si ambos estan en 0
ADC0->R[0]> == 0 & ADC1->R[0]> == 0 esto lo hacemos con la variable i


en caso de que existe movimiento en ambos potenciometros lo detectamos con la variable j
las variables t0 y t1 indican el periodo
t0 es utilizado para chanel 0
t1 es utiizado para chanel 1, en caso de que exista
un resultado en ambos pines, el tiempo de T0 m�s lento, provocando que tenga un PWM menor y pueda girar
a una velocidad menor, al mismo tiempo que vaya hacia adelante
*********************************************/



#include "S32K144.h" /* include peripheral declarations S32K144 */

#define adelante (1<<14)+(1<<7) //Seleccionamos pines 7 y 14;
#define atras (1<<17)+(1<<15) //Seleccionamos pines 15 y 17;
#define derecha (1<<17)+(1<<7) //Seleccionamos pines 7 y 17;
#define izquierda (1<<15)+(1<<14) //Seleccionamos pines 15 y 14;
#define alto (0<<17)+(0<<15)+(0<<14)+(0<<7)

unsigned char duty_cycle=100;
unsigned char espejo_pin;
unsigned char i=0;
unsigned char j=0;
unsigned char t0=4000000;
unsigned char t1=4000000;


void LPIT0_Ch0_IRQHandler (void)

{

	LPIT0->MSR=1;					//Borrar bandera
	LPIT0->TMR[0].TCTRL&=~(1<<0);  //Escribimos en TVAL estando detenido el timer
	ADC0->SC1[0]=(1<<6)+8;
	if (i == 2)LPIT0->TMR[0] = 0; //parar timer y se detiene el robot
	if (j == 2) t0 == 3000000; //parar timer y se detiene el robot
	if (espejo_pin==1)
	{
		espejo_pin=0;
		LPIT0->TMR[0].TVAL=(((100-(unsigned int)duty_cycle)*t0)/100);
	}
	else
	{
		espejo_pin=1;
		LPIT0->TMR[0].TVAL=((((unsigned int)duty_cycle)*t0)/100);
	}
	LPIT0->TMR[0].TCTRL|=(1<<0);		//Enable->Trigger

}

void LPIT0_Ch0_init(void)
{

	PCC->PCCn[PCC_LPIT_INDEX]=(2<<24);
    PCC->PCCn[PCC_LPIT_INDEX]|=(1<<30);
	SCG->SIRCDIV=(1<<8);  //8MHz/1

	LPIT0->MCR=1;		//M_CEN=1;
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	LPIT0->TMR[0].TCTRL=(0<<24)+(1<<23)+(1<<17)+(1<<18);
	LPIT0->TMR[0].TVAL=t0;
	LPIT0->MIER=1;
	LPIT0->TMR[0].TCTRL|=1;
	S32_NVIC->ISER[48/32]=(1<<(48%32));

}

void LPIT0_Ch1_IRQHandler (void)

{


	LPIT0->MSR=1;					//Borrar bandera
	LPIT0->TMR[1].TCTRL&=~(1<<0);  //Escribimos en TVAL estando detenido el timer
	ADC1->SC1[0]=(1<<6)+9;
	if (i == 2)LPIT0->TMR[1] = 0; //parar timer y se detiene el robot
	if (espejo_pin==1)
	{
		espejo_pin=0;
		LPIT0->TMR[1].TVAL=(((100-(unsigned int)duty_cycle)*t1)/100);
	}
	else
	{
		espejo_pin=1;
		LPIT0->TMR[1].TVAL=((((unsigned int)duty_cycle)*t1)/100);
	}
	LPIT0->TMR[1].TCTRL|=(1<<0);		//Enable->Trigger

}

void LPIT0_Ch1_init(void)
{

	PCC->PCCn[PCC_LPIT_INDEX]=(2<<24);
    PCC->PCCn[PCC_LPIT_INDEX]|=(1<<30);
	SCG->SIRCDIV=(1<<8);  //8MHz/1

	LPIT0->MCR=1;		//M_CEN=1;
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	LPIT0->TMR[1].TCTRL=(0<<24)+(1<<23)+(1<<17)+(1<<18);
	LPIT0->TMR[0].TVAL=t1;
	LPIT0->MIER=2;
	LPIT0->TMR[0].TCTRL|=1;
	S32_NVIC->ISER[49/32]=(1<<(49%32));
}
void PORTC_init(void)
{
	PCC->PCCn[PCC_PORTC_INDEX]=(1<<30);
	PORTC->PCR[7]=(1<<8);
	PORTC->PCR[15]=(1<<8);
	PORTC->PCR[17]=(1<<8);
	PORTC->PCR[14]=(1<<8);
	PTC->PDDR=(1<<7)+(1<<15)+(1<<17)+(1<<14);
	PTC->PDOR=0;
}
void PORTB_init(void)
{
	PCC->PCCn[PCC_PORTC_INDEX]=(1<<30);
	PORTB->PCR[13]=(1<<8);
	PORTB->PCR[14]=(1<<8);
	PTB->PDDR=(1<<4)+(1<<13);

}
void ADC0_IRQHandler (void)
{

	if (ADC0->R[0]>1 & ADC0->R[0] < 127) PTC->PDOR=adelante;
    if (ADC0->R[0]>=127)PTC->PDOR=atras;
	if (ADC0->R[0]== 0) i=i++;
	else i= 0;
	return i;
	if (ADC0->R[0]!= 0) j=j++;
	else j= 0;
	return j;

}

void ADC1_IRQHandler (void)
{

	if (ADC1->R[0]>1 & ADC0->R[0] < 127) PTC->PDOR=derecha;
    if (ADC1->R[0]>=127)PTC->PDOR=izquierda;
    if (ADC1->R[0]== 0) i=i++;
    else i= 0;
    return i;
    if (ADC1->R[0]!= 0) j=j++;
    else j= 0;
    return j;

}

void ADC0_init(void)

{
	PCC->PCCn[PCC_ADC1_INDEX]=(2<<24);
	PCC->PCCn[PCC_ADC1_INDEX]|=(1<<30);


	ADC1->CFG1=(2<<5);      //DIVCLK=4, Mode: 8 bits, ALTCLK1 proviene PCC
	//ADC0->SC1[0]=(1<<6)+12;	//hab Intr ADC, seleccionar canal 12 (pot)

	S32_NVIC->ISER[39/32]=(1<<(39%32));
}

void ADC1_init(void)

{
	PCC->PCCn[PCC_ADC1_INDEX]=(2<<24);
	PCC->PCCn[PCC_ADC1_INDEX]|=(1<<30);


	ADC1->CFG1=(2<<5);      //DIVCLK=4, Mode: 8 bits, ALTCLK1 proviene PCC
	//ADC0->SC1[0]=(1<<6)+12;	//hab Intr ADC, seleccionar canal 12 (pot)

	S32_NVIC->ISER[40/32]=(1<<(40%32));
}

int main(void)
{
	SCG->SIRCDIV=(1<<8);	//SIRCDIV2=8MHz/1=8MHz
	//PORTC_init();
	LPIT0_Ch0_init();
	LPIT0_Ch1_init();
	ADC0_init();
	ADC1_init();
	PORTC_init();
	PORTB_init();
	espejo_pin=0;



	while (1);

	return 0;
}
