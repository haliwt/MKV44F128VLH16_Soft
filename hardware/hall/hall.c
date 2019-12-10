#include "hall.h"



__IO uint16_t uSaHall;
__IO uint16_t vSaHall;
__IO uint16_t wSaHall; //电机 W相，采样电压值

__IO uint8_t step;

__IO uint8_t Phase ;
__IO uint8_t SENSE_H;

#define NO_HALL  0 
void NO_HALL_GPIO_Init(void)
{
      gpio_pin_config_t key_config = {
	  kGPIO_DigitalInput ,
	  0,
    };

   /* Port D Clock Gate Control: Clock enabled */
    CLOCK_EnableClock(kCLOCK_PortE);
    
    CLOCK_EnableClock(kCLOCK_PortB);
	CLOCK_EnableClock(kCLOCK_PortC);

	//PORT_SetMultiplePinsConfig();

    /* PORTD0 (pin 93) is configured as PTD0, */
   PORT_SetPinMux(PORTB, 18U, kPORT_MuxAsGpio);
	PORT_SetPinMux(PORTC, 8U, kPORT_MuxAsGpio);
	PORT_SetPinMux(PORTE, 16U, kPORT_MuxAsGpio);
		
    GPIO_PinInit(NO_HALL_A_GPIO, NO_HALL_A_GPIO_PIN, &key_config);
    GPIO_PinInit(NO_HALL_B_GPIO, NO_HALL_B_GPIO_PIN, &key_config);
	 GPIO_PinInit(NO_HALL_C_GPIO, NO_HALL_C_GPIO_PIN, &key_config);
	
    
     const port_pin_config_t  porte6_pin7_config = {/* Internal pull-up resistor is enabled */
                                                  kPORT_PullUp,
                                                  /* Fast slew rate is configured */
                                                  kPORT_FastSlewRate,
                                                  /* Passive filter is disabled */
                                                  kPORT_PassiveFilterDisable,
                                                  /* Open drain is disabled */
                                                  kPORT_OpenDrainDisable,
                                                  /* Low drive strength is configured */
                                                  kPORT_LowDriveStrength,
                                                  /* Pin is configured as PTE6 */
                                                  kPORT_MuxAsGpio,
                                                  /* Pin Control Register fields [15:0] are not locked */
                                                  kPORT_UnlockRegister};
    /* PORTE6 (pin 7) is configured as PTE6 */
    PORT_SetPinConfig(PORTE, 16U, &porte6_pin7_config);
	PORT_SetPinConfig(PORTB, 18U, &porte6_pin7_config);
	PORT_SetPinConfig(PORTC, 8U, &porte6_pin7_config);
#if 0
  /* NO_HALL_A Init input interrupt switch GPIO. */
  #if (defined(FSL_FEATURE_PORT_HAS_NO_INTERRUPT) && FSL_FEATURE_PORT_HAS_NO_INTERRUPT)
   GPIO_SetPinInterruptConfig(NO_HALL_A_GPIO , NO_HALL_A_GPIO_PIN , kPORT_InterruptRisingEdge);
   GPIO_SetPinInterruptConfig(NO_HALL_B_GPIO , NO_HALL_B_GPIO_PIN , kPORT_InterruptFallingEdge);
   GPIO_SetPinInterruptConfig(NO_HALL_C_GPIO , NO_HALL_C_GPIO_PIN , kPORT_InterruptRisingEdge);
 
  #else
    PORT_SetPinInterruptConfig(NO_HALL_A_PORT, NO_HALL_A_GPIO_PIN,  kPORT_InterruptRisingEdge);
    PORT_SetPinInterruptConfig(NO_HALL_B_PORT, NO_HALL_B_GPIO_PIN,  kPORT_InterruptFallingEdge);
    PORT_SetPinInterruptConfig(NO_HALL_C_PORT, NO_HALL_C_GPIO_PIN,  kPORT_InterruptRisingEdge);

  #endif
  //  EnableIRQ(NO_HALL_A_IRQ);
  //  EnableIRQ(NO_HALL_B_IRQ);
  //  EnableIRQ(NO_HALL_C_IRQ);
    
#endif 
        
}

/*********************************************************
 *
 *Funtion Name: NO_Sensorless_GetPinState
 *
 *
 *
*********************************************************/
uint8_t NO_HallSensor_GetPinState(void)
{
  __IO  static uint16_t State ;
  State  = 0;

  ADC_UVW_Sample_HALL_Value();

 
  
  if(uSaHall > 6000)  //U
  {
    State |= 0x01U;
  }
  if(vSaHall > 6000)  //V
  {
    State |= 0x02U;
  }
  if(wSaHall > 6000)  //W
  {
    State |= 0x04U;
  }
   return State;

}
/*********************************************************
 *
 *Funtion Name: NO_HallSensor_Hex(void)
 *Function :Hex number to has hall of hex number
 *
 *
 *
*********************************************************/

uint8_t NO_HallSensor_Hex(void)
{

  static uint8_t noHallValue=0x4;

 // noHallValue = NO_HallSensor_GetPinState();
 // PRINTF("d = %d \r\n",noHallValue);
    DelayMs(4);
     
  
  if(noHallValue == 0x4) //0x4 --hall  0x5 0x1
   {

          
         if(uwStep == 5)
           {
              uwStep = 0x4;
              step=4;
              return uwStep ;
           }
         if(uwStep == 4)
           {
              uwStep = 0x6;
              step=6;
              return uwStep ;
           }
         if(uwStep == 6)
           {
              uwStep = 0x2;
              step=2;
              return uwStep ;
           }
         if(uwStep == 2)
           {
              uwStep = 0x3;
              step=3;
              return uwStep ;
           }
         if(uwStep == 3)
           {
              uwStep = 0x1;
              step=1;
              return uwStep ;
           }
         if(uwStep == 1)
           {
              uwStep = 0x5;
              step=5;
              return uwStep ;
           }
          else  
          {

              uwStep = 0x4;
              step=4;
           
              return uwStep ;
          }
        
      
    }
   if((noHallValue == 0x0)||(noHallValue == 0x7)) //0x0 --hall 0x6 0x04
   {
          
            if(uwStep == 5)
           {
              uwStep = 0x4;
              step=4;
              return uwStep ;
           }
         if(uwStep == 4)
           {
              uwStep = 0x6;
              step=6;
              return uwStep ;
           }
         if(uwStep == 6)
           {
              uwStep = 0x2;
              step=2;
              return uwStep ;
           }
         if(uwStep == 2)
           {
              uwStep = 0x3;
              step=3;
              return uwStep ;
           }
         if(uwStep == 3)
           {
              uwStep = 0x1;
              step=1;
              return uwStep ;
           }
         if(uwStep == 1)
           {
              uwStep = 0x5;
              step=5;
              return uwStep ;
           }
         else
         {
              uwStep = 0x5;
              step=5;
           
              return uwStep ;

         }
          
    }
   if(noHallValue == 0x3) //0x3 --hall 0x2,0x03
   {

         
         if(uwStep == 5)
           {
              uwStep = 0x4;
              step=4;
              return uwStep ;
           }
         if(uwStep == 4)
           {
              uwStep = 0x6;
              step=6;
              return uwStep ;
           }
         if(uwStep == 6)
           {
              uwStep = 0x2;
              step=2;
              return uwStep ;
           }
         if(uwStep == 2)
           {
              uwStep = 0x3;
              step=3;
              return uwStep ;
           }
         if(uwStep == 3)
           {
              uwStep = 0x1;
              step=1;
              return uwStep ;
           }
         if(uwStep == 1)
           {
              uwStep = 0x5;
              step=5;
              return uwStep ;
           }
        else
          {
           uwStep = 0x3;
           step=3;
       
           return uwStep ;
          }
     
   }
   if(noHallValue == 0x1) 
   {
         
        if(step == 5)
           {
              uwStep = 0x4;
              step=4;
              return uwStep ;
           }
         if(step == 4)
           {
              uwStep = 0x6;
              step=6;
              return uwStep ;
           }
         if(step == 6)
           {
              uwStep = 0x2;
              step=2;
              return uwStep ;
           }
         if(step == 2)
           {
              uwStep = 0x3;
              step=3;
              return uwStep ;
           }
         if(step == 3)
           {
              uwStep = 0x1;
              step=1;
              return uwStep ;
           }
         if(step == 1)
           {
              uwStep = 0x5;
              step=5;
              return uwStep ;
           }
          else
          {
           uwStep = 0x1;
           step=1;
         
           return uwStep ;
          }
   }
   if(noHallValue == 0x5)
   {
      if(uwStep == 5)
        {
           uwStep = 0x4;
           step=4;
           return uwStep ;
        }
      if(uwStep == 4)
        {
           uwStep = 0x6;
           step=6;
           return uwStep ;
        }
      if(uwStep == 6)
        {
           uwStep = 0x2;
           step=2;
           return uwStep ;
        }
      if(uwStep == 2)
        {
           uwStep = 0x3;
           step=3;
           return uwStep ;
        }
      if(uwStep == 3)
        {
           uwStep = 0x1;
           step=1;
           return uwStep ;
        }
      if(uwStep == 1)
        {
           uwStep = 0x5;
           step=5;
           return uwStep ;
        }
      else
      {

         uwStep = 0x5;
         step=5;
          return uwStep ;


      }
   }
      if(noHallValue == 0x2)
         {
            if(uwStep == 5)
              {
                 uwStep = 0x4;
                 step=4;
                 return uwStep ;
              }
            if(uwStep == 4)
              {
                 uwStep = 0x6;
                 step=6;
                 return uwStep ;
              }
            if(uwStep == 6)
              {
                 uwStep = 0x2;
                 step=2;
                 return uwStep ;
              }
            if(uwStep == 2)
              {
                 uwStep = 0x3;
                 step=3;
                 return uwStep ;
              }
            if(uwStep == 3)
              {
                 uwStep = 0x1;
                 step=1;
                 return uwStep ;
              }
            if(uwStep == 1)
              {
                 uwStep = 0x5;
                 step=5;
                 return uwStep ;
              }
            else
            {
      
               uwStep = 0x2;
               step=2;
                return uwStep ;
      
      
            }
      }
      
      if(noHallValue == 0x6)
         {
            if(uwStep == 5)
              {
                 uwStep = 0x4;
                 step=4;
                 return uwStep ;
              }
            if(uwStep == 4)
              {
                 uwStep = 0x6;
                 step=6;
                 return uwStep ;
              }
            if(uwStep == 6)
              {
                 uwStep = 0x2;
                 step=2;
                 return uwStep ;
              }
            if(uwStep == 2)
              {
                 uwStep = 0x3;
                 step=3;
                 return uwStep ;
              }
            if(uwStep == 3)
              {
                 uwStep = 0x1;
                 step=1;
                 return uwStep ;
              }
            if(uwStep == 1)
              {
                 uwStep = 0x5;
                 step=5;
                 return uwStep ;
              }
            else
            {
      
               uwStep = 0x6;
               step=6;
                return uwStep ;
      
      
            }
      }
  
   return uwStep;
}
//############################################################################
// + Interruptroutine
// + Wird durch den Analogkomperator ausgelöst
// + Dadurch wird das Kommutieren erzeugt
////{AB,AC,BC,BA,CA,CB} Phase =0 =AB通电
//############################################################################
void Dector_Phase(uint8_t duty)
{
     
    
     uint8_t hex=0;
     switch(Phase)  //{AB,AC,BC,BA,CA,CB} Phase =0 =AB通电
         {
            case 0:  //A+ B- = "2" 换相到 A+ C- = '6'
               
               PWMA_Open_A_Plus_Channel(duty);
           
                  
				  #if (defined(FSL_FEATURE_PORT_HAS_NO_INTERRUPT) && FSL_FEATURE_PORT_HAS_NO_INTERRUPT)
                  GPIO_SetPinInterruptConfig(NO_HALL_C_GPIO , NO_HALL_C_GPIO_PIN , kPORT_InterruptRisingEdge);
                  #else
                      PORT_SetPinInterruptConfig(NO_HALL_C_PORT, NO_HALL_C_GPIO_PIN,  kPORT_InterruptRisingEdge);
                  #endif
                     EnableIRQ(NO_HALL_C_IRQ);

                    if(Phase == 1) 
                    {
                     // DWT_DelayUs(100);
                      PWMA_Close_ABC_Channel(1) ;//关闭B相
                     PWMA_Open_C_Negative_Channel(duty); //打开C- 下半周
                    }
                  // #if NOPRINT  
                   //  PRINTF("P = %d\r \n",Phase);  
                  // #endif
                  
               else
               {
                   PWMA_Close_ABC_Channel(2); //关闭C相
                   PWMA_Open_B_Negative_Channel(duty); //打开B- 下半周
                   #if NOPRINT
                   hex = 0;
                      PRINTF("hex = %d\r \n",hex); 
                   #endif
               }
                        
            break;
            case 1:   /*  A+ C- = "6"  换成 B+ C-= '4'*/   
                  
                  PWMA_Open_C_Negative_Channel(duty); //打开C- 下半周 
                 // if(wSaHall > 2000) // C相反向电动势，工作电压.
                 //  {
                        
                     #if (defined(FSL_FEATURE_PORT_HAS_NO_INTERRUPT) && FSL_FEATURE_PORT_HAS_NO_INTERRUPT)
                        GPIO_SetPinInterruptConfig(NO_HALL_B_GPIO , NO_HALL_B_GPIO_PIN , kPORT_InterruptFallingEdge);
                     #else
                        PORT_SetPinInterruptConfig(NO_HALL_B_PORT, NO_HALL_B_GPIO_PIN,  kPORT_InterruptFallingEdge);
                     #endif
                        EnableIRQ(NO_HALL_B_IRQ);
                           
                        if(Phase ==2)
                        {
                        //  DWT_DelayUs(100);
                          PWMA_Close_ABC_Channel(0) ;//关闭A相
                          PWMA_Open_B_Plus_Channel(duty);// 换向,打开B+ 上半周，关闭C相
                         // #if NOPRINT
                          // PRINTF("P = %d\r \n",Phase); 
                          // #endif  
                        }
                        
                   // }
                     else
                     {
                        PWMA_Close_ABC_Channel(1) ;////关闭B相
                        PWMA_Open_A_Plus_Channel(duty);
                        #if NOPRINT
                          hex = 1;
                          PRINTF("hex = %d\r \n",hex); 
                        #endif
                     }

            break;
            case 2:  // B+ C- ='4'换成 B + A- '5'
                  
                  PWMA_Open_B_Plus_Channel(duty);  //B+ 上半周打开 
                 // if(vSaHall < 500) //B相 工作反向电动势，工作电压
                 // {
                    
                    
                     #if (defined(FSL_FEATURE_PORT_HAS_NO_INTERRUPT) && FSL_FEATURE_PORT_HAS_NO_INTERRUPT)
                        GPIO_SetPinInterruptConfig(NO_HALL_A_GPIO , NO_HALL_A_GPIO_PIN , kPORT_InterruptRisingEdge);
                     #else
                        PORT_SetPinInterruptConfig(NO_HALL_A_PORT, NO_HALL_A_GPIO_PIN,  kPORT_InterruptRisingEdge);
                     #endif
                        EnableIRQ(NO_HALL_A_IRQ);
                        
                      if(Phase == 3)  
                      {
                       // DWT_DelayUs(100);
                        PWMA_Close_ABC_Channel(2) ;//关闭C相
                        PWMA_Open_A_Negative_Channel(duty); //打开A- 下半周，关闭C相。
                      //  #if NOPRINT
                     //   PRINTF("P = %d\r \n",Phase);  
                     //   #endif 
                      }
                   
                 // }
                  else
                  {
                     PWMA_Close_ABC_Channel(0); //关闭A相
                     PWMA_Open_C_Negative_Channel(duty); //打开C- 下半周
                     #if NOPRINT
                       hex = 2;
                      PRINTF("hex = %d\r \n",hex);  
                      #endif 
                  }
                     
            break;
            case 3:  //B+ A- ='5' 换成 C+ A- = '1'
                  
                  PWMA_Open_A_Negative_Channel(duty); //A- 下半周打开
                 // if(uSaHall > 2000)// A相BEMF 电压,工作的电压，
                 // {
                
                    
                  
                     
                   #if (defined(FSL_FEATURE_PORT_HAS_NO_INTERRUPT) && FSL_FEATURE_PORT_HAS_NO_INTERRUPT)
                     GPIO_SetPinInterruptConfig(NO_HALL_C_GPIO , NO_HALL_C_GPIO_PIN , kPORT_InterruptFallingEdge);
                  #else
                      PORT_SetPinInterruptConfig(NO_HALL_C_PORT, NO_HALL_C_GPIO_PIN,  kPORT_InterruptFallingEdge);
                  #endif
                     EnableIRQ(NO_HALL_C_IRQ);
                       
                       if(Phase ==4)
                       {
                       //   DWT_DelayUs(100);
                         PWMA_Close_ABC_Channel(1); //关闭B相
                        PWMA_Open_C_Plus_Channel(duty); //打开C相上半周，关闭B相。
                       // #if NOPRINT
                       // PRINTF("P = %d\r \n",Phase); 
                      //  #endif  
                       }
                     
                     
                 // }
                  else
                  {
                      PWMA_Close_ABC_Channel(2) ;//关闭c相
                      PWMA_Open_B_Plus_Channel(duty);
                      #if NOPRINT
                        hex = 3;
                      PRINTF("hex = %d\r \n",hex); 
                      #endif 
                  }
				break;
            case 4:  //C+ A- ='1' 换成 C+ B- = '3'
                  
                  PWMA_Open_C_Plus_Channel(duty); //C+ 上半周打开
               
                     
                     #if (defined(FSL_FEATURE_PORT_HAS_NO_INTERRUPT) && FSL_FEATURE_PORT_HAS_NO_INTERRUPT)
                        GPIO_SetPinInterruptConfig(NO_HALL_B_GPIO , NO_HALL_B_GPIO_PIN , kPORT_InterruptRisingEdge);
                     #else
                        PORT_SetPinInterruptConfig(NO_HALL_B_PORT, NO_HALL_B_GPIO_PIN,  kPORT_InterruptRisingEdge);
                     #endif
                        EnableIRQ(NO_HALL_B_IRQ);
                       if(Phase == 5)
                       {
                       //  DWT_DelayUs(100);
                         PWMA_Close_ABC_Channel(0); //关闭A相
                        PWMA_Open_B_Negative_Channel(duty); //打开B- 下半周
                       // #if NOPRINT
                       // PRINTF("P = %d\r \n",Phase);  
                       // #endif
                       }
                     
                
                  else
                  {
                     PWMA_Close_ABC_Channel(1) ;//关闭B相
                     PWMA_Open_A_Negative_Channel(duty); //A-
                     #if NOPRINT
                       hex = 4;
                      PRINTF("hex = %d\r \n",hex); 
                      #endif 
                  }
                     
            break;
            case 5:  //C+ B- ='3' 换成 A+ B-  ='2'
                  
                  PWMA_Open_B_Negative_Channel(duty);
              
                     #if (defined(FSL_FEATURE_PORT_HAS_NO_INTERRUPT) && FSL_FEATURE_PORT_HAS_NO_INTERRUPT)
                        GPIO_SetPinInterruptConfig(NO_HALL_A_GPIO , NO_HALL_A_GPIO_PIN , kPORT_InterruptFallingEdge);
                     #else
                        PORT_SetPinInterruptConfig(NO_HALL_A_PORT, NO_HALL_A_GPIO_PIN,  kPORT_InterruptFallingEdge);
                     #endif
                        EnableIRQ(NO_HALL_A_IRQ);
                        
                        if(Phase == 0)
                        {
                       // DWT_DelayUs(100);
                          PWMA_Close_ABC_Channel(2) ;//关闭C相
                        PWMA_Open_A_Plus_Channel(duty);// 打开A+
                        
                       // #if NOPRINT
                       // PRINTF("P = %d\r \n",Phase);  
                       // #endif 
                        }
                     
                     
               
                  else
                  {
                     PWMA_Close_ABC_Channel(0) ;//关闭A相
                     PWMA_Open_C_Plus_Channel(duty);
                     #if NOPRINT
                       hex = 5;
                      PRINTF("hex = %d\r \n",hex); 
                      #endif 
                  }
                     
            break;
         }
     }
   
     






