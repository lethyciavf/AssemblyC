/*
Lethycia Venturini Ferreira

FLASH 711
RAM 34   

PTA5 LED VERDE
PTA4 NC
PTA3 LED VERMELHO
PTA2 PEDESTRE
PTA1 CH1 
PTA0 CH0

*/

#include "derivative.h" /* include peripheral declarations */

 //definições

#define VERDE 0x20 
#define AMARELO 0x28 
#define VERMELHO 0x08
#define APAGADO 0X00
//#define D1SEG 10000 //3,2MHz

//variaveis

unsigned char TEMPO;
unsigned char STATUSTEMPO;
  
//funcoes 
void interrupt PISCA (void);
void interrupt PEDESTRE (void);
void interrupt ROTTIMER (void);

void main(void)
{//CONFIGURA

CONFIG2= 0x40;    //3,2MHz COM IRQ/PULLUP E SEM RESET
CONFIG1= 0x19;    //+5V SEM COP SEM LVI STOP ILEGAL
DDRA=0x28;          //PTA5 E PTA3 SAIDAS
PTAPUE=0x03;      //PULLUP NO PTA1 E NO PTA0
TSC=0x30;             //PARAR E RESETAR
TSC=0x46;             // /64 E HABILITA ROTTIMER
_asm {
  LDHX #$100
  TXS                      //PASSA A PILHA PRO FINAL DA RAM
  LDHX #50000
  STHX TMODH 
  CLI                      //HABILITA TODAS AS INTERRUPCOES 
}
for(;;)

{
   if (!PTA_PTA1&&!PTA_PTA0){
    
  //TEMP0
   PTA=VERDE;
   TEMPO=5;
    while(TEMPO>0){
     //5 BILHOES DE NANO SEG    
    }
   
  PTA=AMARELO;
  TEMPO=3;
   while(TEMPO>0){

   }
   
  PTA=VERMELHO;
  // TEMPO=2; BUG
 _asm {
 LDA #2
 STA TEMPO
 }
  while(TEMPO>0){  

  } 
   }
  if (!PTA_PTA1&&PTA_PTA0) { 
  //TEMP1

  PTA=VERDE;
  TEMPO=3;
  while(TEMPO>0){ 
  
  }
   
PTA=AMARELO;

  /*TEMPO=1 NAO ERA ACEITO PELO COMPILADOR
    PARA SOLUCIONAR ESSE BUG, USOU-SE PARTE
   DO CODIGO EM ASSEMBLY*/
 
_asm {
      LDA #$01 
      STA TEMPO
     } 
 
  while(TEMPO>0){

    }

   
  PTA=VERMELHO;
  TEMPO=4;
  while(TEMPO>0){
   
  }
  }

  if (PTA_PTA1&&!PTA_PTA0){
  //PISCAAMARELO
 
 _asm SWI  // INSTRUCAO
  
  
  } else{
  //FIXAVERDE
  PTA=VERDE;
  
  }
}  
} 


void interrupt PEDESTRE (void) {
_asm {
  BSET 1, INTSCR
  CLI    
}

 if (PTA_PTA5&&!PTA_PTA3){ 
  STATUSTEMPO=TEMPO;
  PTA=AMARELO;
  TEMPO= 1; 
   while(TEMPO>0){
    
    }
  TEMPO=4;
  PTA=VERMELHO;
   while (TEMPO>0){

   }
  PTA=VERDE;
  TEMPO=STATUSTEMPO;
  INTSCR=0x04;//ACK=1 E IMASK=0
 }
}

//CORRIGIDO PARA QUE O TEMPO PASSASSE DE FORMA CORRETA
//ANTES ROTTIMER ESTAVA INCOMPLETO

void interrupt ROTTIMER(void)
{
 while (TEMPO>0)  {     //DECREMENTA ATE QUE TEMPO=0
 _asm {
      LDA TEMPO 
      DECA 
      STA TEMPO
      }
  }
_asm  BCLR 7, TSC //TIRAR PENDENCIA DO TIMER   
}

void interrupt PISCA (void){
//ENQUANTO CH1CH0=10 PISCA
_asm {
  BSET 1, INTSCR //DESABILITA O PEDESTRE
  CLI
}
while (PTA_PTA1&&!PTA_PTA0){
  
PTA=AMARELO;

_asm {                                //MAIS UMA VEZ O CODEWARRIOR “BUGA” 
      LDA #$01                     //AO COLOCAR TEMPO=1, SOLUCIONADO
      STA TEMPO                //USANDO CODIGO EM ASM
     }       
     
while(TEMPO>0){
    
  }
 
PTA=APAGADO;
_asm {                              //ERRO USANDO TEMPO=1
      LDA #$01                   //SOLUCIONADO COM ASSEMBLY
      STA TEMPO
     }           

TEMPO= 1; 

while(TEMPO>0){
    
  }
}
INTSCR=0x04; //TIRA A PENDENCIA E HABILITA O PEDESTRE
}




/*
VECTOR 1 PISCA
VECTOR 2 PEDESTRE
VECTOR 6 ROTTIMER
*/