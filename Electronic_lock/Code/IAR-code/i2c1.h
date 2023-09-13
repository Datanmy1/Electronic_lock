//I2C PIN CONFIG:
#define SCL       BIT1
#define SDA       BIT0 //
#define P2        P2IN //
#define SCL_1     P2OUT |= SCL //
#define SCL_0     P2OUT &= ~SCL //
#define SCLdir_1  P2DIR |= SCL //
#define SCLdir_0  P2DIR &= ~SCL //
#define SDA_1     P2OUT |= SDA //
#define SDA_0     P2OUT &= ~SDA //
#define SDAdir_1  P2DIR |=  SDA //
#define SDAdir_0  P2DIR &= ~SDA //
//==============================================================================
//CHUONG TRINH GIAO TIEP I2C:
//START CONDITTION:
void I2C_Start(void)// start communication
{ 
  SCLdir_1;   //SCL
  SDAdir_1;   //SDA
  SCL_1; 
  SDA_1;
  __no_operation();
  SDA_0;
  __no_operation();
  SCL_0; 
}
//==============================================================================
//STOP CONDITTION:
void I2C_Stop(void)// stop communication
{     
  SDAdir_1;// SDA out
  SDA_0;   //LOW SDA
  SCL_1;    //HIGH SCL
  __no_operation();   //no delay();
  SDA_1;     //HIGH SDA
}
//==============================================================================
//==============================================================================
//I2C WRITER:
void I2C_Write(unsigned char datawr){

  SDAdir_1;     //chan SDA la chan out, de ghi data len DS1307

  for(int i=0;i<8;i++){
  
      SCL_0;        //Muc thap SCL
      //--------------------------
      if((datawr & 0x80) && 1){
      
        SDA_1;
        __no_operation();
      }
      else{
      
        SDA_0;
        __no_operation();
      }
      datawr <<= 1;
      //----------------------------
      __no_operation();
      SCL_1; 
      __no_operation();
      SCL_0;
  }
    __no_operation();
    SCL_1; 
    __no_operation();
    SCL_0;
    __no_operation();
}
//==============================================================================
//==============================================================================
//Ham doc data i2c
char I2C_Read(void)// read 1 byte data
{
  SDAdir_0;// sda
 
  char i, dat;
  dat = 0x00;
  //--------------------
  for(i=0;i<8;i++) // read  1 byte  data
  { 
    __no_operation();
    SCL_1;
    dat <<= 1;	// Set SCL 
    if(SDA & P2) dat++;
    SCL_0; 	// Clear SCL 
  };
  //---------------------
  SDA_1 ; 
  SCL_1;
  __no_operation();
  SCL_0;
  __no_operation();
  return dat;
}
//==============================================================================