#include "hall.h"



__IO uint16_t uSaHall;
__IO uint16_t vSaHall;
__IO uint16_t wSaHall; //电机 W相，采样电压值

__IO uint8_t step;




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
	
    
    PORT_PinPullConfig(HW_GPIOB, 18,  kPORT_PullUp);
    PORT_PinPullConfig(HW_GPIOC, 8,   kPORT_PullUp);
    PORT_PinPullConfig(HW_GPIOE, 16,  kPORT_PullUp);
        
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

 
  
  if(uSaHall > 8000)  //U
  {
    State |= 0x01U;
  }
  if(vSaHall > 8000)  //V
  {
    State |= 0x02U;
  }
  if(wSaHall > 8000)  //W
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

  static uint8_t noHallValue;

  noHallValue = NO_HallSensor_GetPinState();
 // PRINTF("d = %d \r\n",noHallValue);

  if(noHallValue == 0x4) //0x4 --hall  0x5 0x1
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

              uwStep = 0x5;
              step=5;
           
              return uwStep ;
          }
        
      
    }
   if((noHallValue == 0x0)||(noHallValue == 0x7)) //0x0 --hall 0x6 0x04
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
              uwStep = 0x6;
              step=6;
            
              return uwStep ;


          }

    }
   if(noHallValue == 0x3) //0x3 --hall 0x2,0x03
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
           uwStep = 0x2;
           step=2;
       
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
           uwStep = 0x3;
           step=3;
         
           return uwStep ;
          }
   }
   if((noHallValue == 0x5)||(noHallValue == 0x2)||(noHallValue == 0x6))
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


   }

  if(Dir == 0) //逆时针方向 -0
  {
    uwStep = (uint32_t)7 - uwStep;        // 逆时针 CW = 7 - CCW;
  }
  return uwStep;

}




