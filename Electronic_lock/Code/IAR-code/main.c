#include "msp430g2553.h"
#include "i2c_lcd.h"
//#include "flash.h"
#include "math.h"
void check();
void nhapmk();
char PadADC();
void buzzer();
void openlock();
void intro();
void led_red();
void led_green();
void resetpass();
void doimk();
void moinhapmk();
void canhbao();
char mk[]={'1','2','3','4'};
char mkkt[4];
int giatri;

char keypad(){
  ADC10CTL0 = ADC10SHT_2 + REFON  + ADC10ON + ADC10IE + SREF_0;
  __delay_cycles (30);
  ADC10CTL1 = INCH_0; 
  ADC10AE0 |= BIT0;  
  ADC10CTL0 |= ENC + ADC10SC;
  int value[16]={1760,2980,7400,8420,9040,9800,12660,13300,13590,14050,15820,16420,16700,17020,18400, 18800};
  char key[16]={'A'  ,'3' ,'1' ,'2' ,'B' ,'6' ,'4'  ,'5'  ,'C'  ,'9'  ,'7'  ,'8'  ,'D'  ,'#'  ,'*'  ,  '0' };
  int adc;
  adc = (ADC10MEM*33000ul)/1023;
  for(int i=0; i<16;i++){      
    if (abs((adc-value[i])<100)){
      return key[i];
      while(adc<30000){__delay_cycles(100000);}
    }
  }
  return 'x';  
};

void main(void)
{
  
  
  WDTCTL = WDTPW + WDTHOLD;
  
  //khai b�o OUTPUT: P1.1=BUZZER, P1.2=INRELAY
  P1DIR |= BIT1+BIT3;
  P1OUT &= ~BIT1+BIT3;
  
  P1DIR |=  BIT4+BIT5; // ngo ra led   
  P1OUT &= ~BIT4+BIT5;
  
  //khai bao ngat
  P1IE |= BIT2;
  P1IES |= BIT2;
  P1IFG &=~ BIT2;
  LCD_Init(0X27, 2, 16);
  LCD_backlightOff();
  LCD_displayOff();
  
 // _BIS_SR(GIE);
  while(1){ 
  if(!(P1IN&BIT2)){
    LCD_backlightOn();
    LCD_displayOn();
    intro();    
    if(!(P1IN&BIT2)){
      nhapmk();
      check();    
      LCD_Clear();
      LCD_SetCursor(6,1);
    }
  
  LCD_Clear();
  LCD_SetCursor(5,0);
  LCD_Print("MK MOI:");
  for(int i = 0; i < 4; i++){
    LCD_SetCursor(6+i,1);
    LCD_writeChar(mk[i]);}
  __delay_cycles(1000000);
  
  LCD_backlightOff();
  LCD_displayOff();
  }
  }   
}


void resetpass(){
  for(int i = 0; i <4; i++){
    mkkt[i]= '0';
  }
}
void led_red(){
  P1OUT |= BIT5;
  P1OUT |= BIT1;
  __delay_cycles(100000);
  P1OUT &= ~BIT1;
  P1OUT &= ~BIT5;
  __delay_cycles(100000);
}
void led_green(){
  P1OUT |= BIT4;
  P1OUT |=BIT1;
  __delay_cycles(100000);
  P1OUT &= ~BIT1;
  P1OUT &= ~BIT4;
  __delay_cycles(100000);
}
void intro(){
  LCD_SetCursor(0,0);
  LCD_Print(" Nguyen Thanh Dat ");
  __delay_cycles(500000);
  LCD_SetCursor(0,1);
  LCD_Print("    B1812195    ");
  __delay_cycles(500000);
  LCD_Clear();
}
void buzzer(){
  P1OUT |= BIT1;
  __delay_cycles(100000);
  P1OUT &= ~BIT1;
  __delay_cycles(100000);
}
void openlock(){
  P1OUT |=BIT3;
  __delay_cycles(3000000);
  P1OUT &= ~BIT3;
  __delay_cycles(100000);
}
void check(){
  if(mk[0] == mkkt[0] && mk[1] == mkkt[1]&& mk[2] == mkkt[2]&& mk[3] == mkkt[3]){
    led_green();
    LCD_Clear();
    LCD_SetCursor(0,0);
    LCD_Print("NHAP THANH CONG"); 
    LCD_SetCursor(3,1);
    LCD_Print("MOI VAO !!"); 
    openlock();
    resetpass();
  } 
  else{
    canhbao();
  }
  LCD_Clear();
}

void doimk(){
  LCD_Clear();
  LCD_SetCursor(5,0);
  LCD_Print("DOI MK:");
  LCD_SetCursor(6,1);
  LCD_Print("####");
  int dem=0;
  int check=0;
  while(check<1){
    char a;
    a = keypad();
    while(dem<4){
      __delay_cycles(100000);
      a = keypad();
      if(a!='x'&&a!='A'&&a!='B'&&a!='C'&&a!='D'&&a!='*'&&a!='#'){
        __delay_cycles(100000);
        led_green();
        mk[dem]=a;
        LCD_SetCursor(6+dem,1);
        LCD_writeChar(a); 
        __delay_cycles(200000);
        LCD_SetCursor(6+dem,1);
        LCD_Print("*"); 
        dem++;
      }
      if(a=='C'){
        if(dem!=0){
          led_red();
          dem--;
          LCD_SetCursor(6+dem,1);
          LCD_Print("#"); 
        }        
      }
      if(a=='B'){
        dem=0;
        LCD_SetCursor(6,1);
        LCD_Print("####");
        led_red();
        for(int i = 0; i <4; i++){
          mk[i]= '0';
        }
      }      
      if(a=='D'){
        LCD_Clear();
        LCD_SetCursor(5,0);
        LCD_Print("MK MOI:");
        for(int i = 0; i < 4; i++){      
          LCD_SetCursor(6 + i,1);
          LCD_writeChar(mk[i]);}
        __delay_cycles(1500000);
        dem=4;
        check=1;
      }
    }
    if (a=='B'){
      dem=0;
      LCD_SetCursor(6,1);
      LCD_Print("####");
      led_red();
    }
    if(a=='D'){
      LCD_Clear();
      LCD_SetCursor(5,0);
      LCD_Print("MK MOI:");
      for(int i = 0; i < 4; i++){
        LCD_SetCursor(6+i,1);
        LCD_writeChar(mk[i]);}
      __delay_cycles(2000000);
      check=1;
    }
  }
  
};

void moinhapmk(){
  LCD_Clear();
  LCD_SetCursor(2,0);
  LCD_Print("MOI NHAP MK: ");
  LCD_SetCursor(6,1);
  LCD_Print("####");
};
void canhbao(){
  LCD_Clear();
  LCD_SetCursor(3,0);
  LCD_Print("CANH BAO!!"); 
  LCD_SetCursor(4,1);
  LCD_Print("NHAP SAI"); 
  for(int i=0; i<10;i++){
    led_red();
  }
  LCD_Clear();
}
void nhapmk(){
  int check=0;
  int dem=0;
  moinhapmk();
  while(check<1){
    char a;
    a = keypad();
    while(dem<4){
      __delay_cycles(100000);
      a = keypad();
      if(a!='x'&&a!='A'&&a!='B'&&a!='C'&&a!='D'&&a!='*'&&a!='#'){
        __delay_cycles(100000);
        led_green();
        mkkt[dem]=a;
        LCD_SetCursor(6+dem,1);
        LCD_writeChar(a); 
        __delay_cycles(200000);
        LCD_SetCursor(6+dem,1);
        LCD_Print("*"); 
        dem++;
      }
      if(a=='C'){
        if(dem!=0){
          led_red();
          dem--;
          LCD_SetCursor(6+dem,1);
          LCD_Print("#"); 
        }        
      }
      if(a=='B'){
        dem=0;
        LCD_SetCursor(6,1);
        LCD_Print("####");
        led_red();
        resetpass();
      }      
      if(a=='D'){
        dem=4;
        check=1;
      }
    }
    if (a=='B'){
      dem=0;
      LCD_SetCursor(6,1);
      LCD_Print("####");
      led_red();
    }
    if(a=='D'){
      check=1;
    }
    if(a=='A'){
      if(mk[0] == mkkt[0] && mk[1] == mkkt[1]&& mk[2] == mkkt[2]&& mk[3] == mkkt[3]){
        doimk();
        moinhapmk();
        dem=0;
        check=0;}
      
      else{ 
        canhbao();
        moinhapmk(); 
        dem=0;
        check=0;
      }
      
    }
  }
}
#pragma vector = PORT1_VECTOR
__interrupt void Port_1(void){
  
  if(!(P1IN&BIT2)){
    LCD_backlightOn();
    LCD_displayOn();
    intro();    
    if(!(P1IN&BIT2)){
      nhapmk();
      check();    
      LCD_Clear();
      LCD_SetCursor(6,1);
    }
  }
  LCD_Clear();
  LCD_SetCursor(5,0);
  LCD_Print("MK MOI:");
  for(int i = 0; i < 4; i++){
    LCD_SetCursor(6+i,1);
    LCD_writeChar(mk[i]);}
  __delay_cycles(2000000);
  
  LCD_backlightOff();
  LCD_displayOff();
  P1IFG &= ~BIT2;  
}

