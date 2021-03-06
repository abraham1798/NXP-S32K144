
/*Jos� Abraham P�rez Mart�nez A01633926
You can change the velocity of the motor if you modify the value of the unsigned long i in delay function
fast = 50000
slow = 300000
Values of the module ULN2003
PTC17 = IN1
PTC14 = IN2
PTC15 = IN3
PTC16 = IN4

*/
#include "S32K144.h" /* include peripheral declarations S32K144 */

void delay (void)
{
	unsigned long i=50000;
	do{}while (i--);
}

void delay2 (unsigned int tiempo)
{

		LPTMR0->CMR=tiempo-1;
		LPTMR0->CSR=1;
		do{}while (((LPTMR0->CSR) & (1<<7))==0);
		LPTMR0->CSR=0;
}

void CW()
{

	  PTB->PDOR=(1<<17);
	  delay();
	  PTB->PDOR=(1<<14);
	  delay();
	  PTB->PDOR=(1<<15);
	  delay();
	  PTB->PDOR=(1<<16);
	  delay();
}

void CCW()
{
	PTB->PDOR=(1<<16);
	delay();
	PTB->PDOR=(1<<15);
	delay();
	PTB->PDOR=(1<<14);
	delay();
	PTB->PDOR=(1<<17);
	delay();
}
int main(void)
{
        //CGC Activate clock PCC
		PCC->PCCn[PCC_PORTB_INDEX]=0x40000000;

		//Select port (Mux)
		PORTB->PCR[17]=0x00000100;
		PORTB->PCR[16]=0x00000100;
		PORTB->PCR[15]=0x00000100;
		PORTB->PCR[14]=0x00000100;

		//Pin activate
		PTB->PDDR=0x0003C000;

		//CGC Activate clock PCC
		PCC->PCCn[PCC_PORTC_INDEX]=0x40000000;
		//Select port (Mux)
		PORTC->PCR[13]=0x00000100;

		PCC->PCCn[PCC_PORTD_INDEX]=0x40000000; //1<<30
			PORTD->PCR[0]=0x00000100;
			PORTD->PCR[15]=0x00000100;  //1<<8
			PORTD->PCR[16]=0x00000100;
			PTD->PDDR=0x00018001;

			PCC->PCCn[PCC_LPTMR0_INDEX]=0x40000000;
			LPTMR0->CMR=500-1;			// 500 cuentas de 1 ms
			LPTMR0->PSR=5;				//Bypass preescaler, clock source=1 KHz (pag 1406)
			LPTMR0->CSR=(1<<6)+1;		//Hab Intr LPTRM0, Enable
			S32_NVIC->ISER[58/32]=(1<<(58%32));

        for(;;) {
        	//Input
        	//if ((PTC->PDIR & (0x00002000))==0) CW();
        	//else CCW();


        	PTD->PDOR=0x00018000;
        	delay2(1000);
        	PTD->PDOR=0x00018001;
        	delay2(1000);
        }

	return 0;
}
