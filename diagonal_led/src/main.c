


#include "S32K144.h" /* include peripheral declarations S32K144 */
void delay (void)
{
unsigned long i=10000000;
do{}while (--i);

}
int main(void)
{
#define COUNTER_LIMIT 100
    /*Activate Clock*/
	PCC->PCCn[PCC_PORTB_INDEX]=0X40000000;
	/*Select port (Mux) PORT_PCR*/ /*Chapter 12  PORT_PCRn page 198*/
		PORTB->PCR[14]=0X00000100;
		PORTB->PCR[15]=0X00000100;
		PORTB->PCR[16]=0X00000100;
		PORTB->PCR[17]=0X00000100;
		/*Port Data direction PDDR Activar todos*/ /*Chapter 13 page 218*/
		PTD->PDDR=0X0003C000;



		        for(;;) {
		        	/*Port Data direction 14*/
		        	PTD->PDOR=0X00004000;
		        	delay();
		        	/*Port Data direction 15*/
		        	PTD->PDOR=0X00002000;
		        	delay();
		        	/*Port Data direction 16*/
		        	PTD->PDOR=0X00010000;
		        	delay();
		        	/*Port Data direction 17*/
		        	PTD->PDOR=0X00020000;
		        	delay();


		        }

	return 0;
}