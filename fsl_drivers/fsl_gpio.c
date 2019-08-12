/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2018 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_gpio.h"
#include "fsl_common.h"
#include "fsl_port.h"

/* Component ID definition, used by tools. */
#ifndef FSL_COMPONENT_ID
#define FSL_COMPONENT_ID "platform.drivers.gpio"
#endif

/*******************************************************************************
 * Variables
 ******************************************************************************/
static GPIO_Type * const GPIO_InstanceTable[] = GPIO_BASES;
static PORT_Type * const PORT_InstanceTable[] = PORT_BASES;
static const uint32_t SIM_GPIOClockGateTable[] =
{
    SIM_SCGC5_PORTA_MASK,
    SIM_SCGC5_PORTB_MASK,
    SIM_SCGC5_PORTC_MASK,
    SIM_SCGC5_PORTD_MASK,
    SIM_SCGC5_PORTE_MASK,
};
#if !(defined(FSL_FEATURE_PORT_HAS_NO_INTERRUPT) && FSL_FEATURE_PORT_HAS_NO_INTERRUPT)
static PORT_Type *const s_portBases[] = PORT_BASE_PTRS;
static GPIO_Type *const s_gpioBases[] = GPIO_BASE_PTRS;
#endif

#if defined(FSL_FEATURE_SOC_FGPIO_COUNT) && FSL_FEATURE_SOC_FGPIO_COUNT

#if defined(FSL_FEATURE_PCC_HAS_FGPIO_CLOCK_GATE_CONTROL) && FSL_FEATURE_PCC_HAS_FGPIO_CLOCK_GATE_CONTROL

#if !(defined(FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL) && FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL)
/*! @brief Array to map FGPIO instance number to clock name. */
static const clock_ip_name_t s_fgpioClockName[] = FGPIO_CLOCKS;
#endif /* FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL */

#endif /* FSL_FEATURE_PCC_HAS_FGPIO_CLOCK_GATE_CONTROL */

#endif /* FSL_FEATURE_SOC_FGPIO_COUNT */

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
#if !(defined(FSL_FEATURE_PORT_HAS_NO_INTERRUPT) && FSL_FEATURE_PORT_HAS_NO_INTERRUPT)
/*!
 * @brief Gets the GPIO instance according to the GPIO base
 *
 * @param base    GPIO peripheral base pointer(PTA, PTB, PTC, etc.)
 * @retval GPIO instance
 */
static uint32_t GPIO_GetInstance(GPIO_Type *base);
#endif
/*******************************************************************************
 * Code
 ******************************************************************************/
#if !(defined(FSL_FEATURE_PORT_HAS_NO_INTERRUPT) && FSL_FEATURE_PORT_HAS_NO_INTERRUPT)
static uint32_t GPIO_GetInstance(GPIO_Type *base)
{
    uint32_t instance;

    /* Find the instance index from base address mappings. */
    for (instance = 0; instance < ARRAY_SIZE(s_gpioBases); instance++)
    {
        if (s_gpioBases[instance] == base)
        {
            break;
        }
    }

    assert(instance < ARRAY_SIZE(s_gpioBases));

    return instance;
}
#endif
/*!
 * brief Initializes a GPIO pin used by the board.
 *
 * To initialize the GPIO, define a pin configuration, as either input or output, in the user file.
 * Then, call the GPIO_PinInit() function.
 *
 * This is an example to define an input pin or an output pin configuration.
 * code
 * Define a digital input pin configuration,
 * gpio_pin_config_t config =
 * {
 *   kGPIO_DigitalInput,
 *   0,
 * }
 * Define a digital output pin configuration,
 * gpio_pin_config_t config =
 * {
 *   kGPIO_DigitalOutput,
 *   0,
 * }
 * endcode
 *
 * param base   GPIO peripheral base pointer (GPIOA, GPIOB, GPIOC, and so on.)
 * param pin    GPIO port pin number
 * param config GPIO pin configuration pointer
 */
void GPIO_PinInit(GPIO_Type *base, uint32_t pin, const gpio_pin_config_t *config)
{
    assert(NULL != config);

    uint32_t u32flag = 1;

    if (config->pinDirection == kGPIO_DigitalInput)
    {
        base->PDDR &= ~(u32flag << pin);
    }
    else
    {
        GPIO_PinWrite(base, pin, config->outputLogic);
        base->PDDR |= (u32flag << pin);
    }
}

#if !(defined(FSL_FEATURE_PORT_HAS_NO_INTERRUPT) && FSL_FEATURE_PORT_HAS_NO_INTERRUPT)
/*!
 * brief Reads the GPIO port interrupt status flag.
 *
 * If a pin is configured to generate the DMA request, the corresponding flag
 * is cleared automatically at the completion of the requested DMA transfer.
 * Otherwise, the flag remains set until a logic one is written to that flag.
 * If configured for a level sensitive interrupt that remains asserted, the flag
 * is set again immediately.
 *
 * param base GPIO peripheral base pointer (GPIOA, GPIOB, GPIOC, and so on.)
 * retval The current GPIO port interrupt status flag, for example, 0x00010001 means the
 *         pin 0 and 17 have the interrupt.
 */
uint32_t GPIO_PortGetInterruptFlags(GPIO_Type *base)
{
    uint8_t instance;
    PORT_Type *portBase;
    instance = (uint8_t)GPIO_GetInstance(base);
    portBase = s_portBases[instance];
    return portBase->ISFR;
}

/*!
 * brief Clears multiple GPIO pin interrupt status flags.
 *
 * param base GPIO peripheral base pointer (GPIOA, GPIOB, GPIOC, and so on.)
 * param mask GPIO pin number macro
 */
void GPIO_PortClearInterruptFlags(GPIO_Type *base, uint32_t mask)
{
    uint8_t instance;
    PORT_Type *portBase;
    instance       = (uint8_t)GPIO_GetInstance(base);
    portBase       = s_portBases[instance];
    portBase->ISFR = mask;
}
#endif

#if defined(FSL_FEATURE_GPIO_HAS_ATTRIBUTE_CHECKER) && FSL_FEATURE_GPIO_HAS_ATTRIBUTE_CHECKER
/*!
 * brief The GPIO module supports a device-specific number of data ports, organized as 32-bit
 * words. Each 32-bit data port includes a GACR register, which defines the byte-level
 * attributes required for a successful access to the GPIO programming model. The attribute controls for the 4 data
 * bytes in the GACR follow a standard little endian
 * data convention.
 *
 * param base GPIO peripheral base pointer (GPIOA, GPIOB, GPIOC, and so on.)
 * param mask GPIO pin number macro
 */
void GPIO_CheckAttributeBytes(GPIO_Type *base, gpio_checker_attribute_t attribute)
{
    base->GACR = ((uint32_t)attribute << GPIO_GACR_ACB0_SHIFT) | ((uint32_t)attribute << GPIO_GACR_ACB1_SHIFT) |
                 ((uint32_t)attribute << GPIO_GACR_ACB2_SHIFT) | ((uint32_t)attribute << GPIO_GACR_ACB3_SHIFT);
}
#endif

#if defined(FSL_FEATURE_SOC_FGPIO_COUNT) && FSL_FEATURE_SOC_FGPIO_COUNT

/*******************************************************************************
 * Variables
 ******************************************************************************/
#if !(defined(FSL_FEATURE_PORT_HAS_NO_INTERRUPT) && FSL_FEATURE_PORT_HAS_NO_INTERRUPT)
static FGPIO_Type *const s_fgpioBases[] = FGPIO_BASE_PTRS;
#endif
/*******************************************************************************
 * Prototypes
 ******************************************************************************/
#if !(defined(FSL_FEATURE_PORT_HAS_NO_INTERRUPT) && FSL_FEATURE_PORT_HAS_NO_INTERRUPT)
/*!
 * @brief Gets the FGPIO instance according to the GPIO base
 *
 * @param base    FGPIO peripheral base pointer(PTA, PTB, PTC, etc.)
 * @retval FGPIO instance
 */
static uint32_t FGPIO_GetInstance(FGPIO_Type *base);
#endif
/*******************************************************************************
 * Code
 ******************************************************************************/
#if !(defined(FSL_FEATURE_PORT_HAS_NO_INTERRUPT) && FSL_FEATURE_PORT_HAS_NO_INTERRUPT)
static uint32_t FGPIO_GetInstance(FGPIO_Type *base)
{
    uint32_t instance;

    /* Find the instance index from base address mappings. */
    for (instance = 0; instance < ARRAY_SIZE(s_fgpioBases); instance++)
    {
        if (s_fgpioBases[instance] == base)
        {
            break;
        }
    }

    assert(instance < ARRAY_SIZE(s_fgpioBases));

    return instance;
}
#endif
#if defined(FSL_FEATURE_PCC_HAS_FGPIO_CLOCK_GATE_CONTROL) && FSL_FEATURE_PCC_HAS_FGPIO_CLOCK_GATE_CONTROL
/*!
 * brief Initializes the FGPIO peripheral.
 *
 * This function ungates the FGPIO clock.
 *
 * param base   FGPIO peripheral base pointer (FGPIOA, FGPIOB, FGPIOC, and so on.)
 */
void FGPIO_PortInit(FGPIO_Type *base)
{
#if !(defined(FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL) && FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL)
    /* Ungate FGPIO periphral clock */
    CLOCK_EnableClock(s_fgpioClockName[FGPIO_GetInstance(base)]);
#endif /* FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL */
}
#endif /* FSL_FEATURE_PCC_HAS_FGPIO_CLOCK_GATE_CONTROL */

/*!
 * brief Initializes a FGPIO pin used by the board.
 *
 * To initialize the FGPIO driver, define a pin configuration, as either input or output, in the user file.
 * Then, call the FGPIO_PinInit() function.
 *
 * This is an example to define an input pin or an output pin configuration:
 * code
 * Define a digital input pin configuration,
 * gpio_pin_config_t config =
 * {
 *   kGPIO_DigitalInput,
 *   0,
 * }
 * Define a digital output pin configuration,
 * gpio_pin_config_t config =
 * {
 *   kGPIO_DigitalOutput,
 *   0,
 * }
 * endcode
 *
 * param base   FGPIO peripheral base pointer (FGPIOA, FGPIOB, FGPIOC, and so on.)
 * param pin    FGPIO port pin number
 * param config FGPIO pin configuration pointer
 */
void FGPIO_PinInit(FGPIO_Type *base, uint32_t pin, const gpio_pin_config_t *config)
{
    assert(NULL != config);

    uint32_t u32flag = 1;

    if (config->pinDirection == kGPIO_DigitalInput)
    {
        base->PDDR &= ~(u32flag << pin);
    }
    else
    {
        FGPIO_PinWrite(base, pin, config->outputLogic);
        base->PDDR |= (u32flag << pin);
    }
}
#if !(defined(FSL_FEATURE_PORT_HAS_NO_INTERRUPT) && FSL_FEATURE_PORT_HAS_NO_INTERRUPT)
/*!
 * brief Reads the FGPIO port interrupt status flag.
 *
 * If a pin is configured to generate the DMA request, the corresponding flag
 * is cleared automatically at the completion of the requested DMA transfer.
 * Otherwise, the flag remains set until a logic one is written to that flag.
 * If configured for a level-sensitive interrupt that remains asserted, the flag
 * is set again immediately.
 *
 * param base FGPIO peripheral base pointer (FGPIOA, FGPIOB, FGPIOC, and so on.)
 * retval The current FGPIO port interrupt status flags, for example, 0x00010001 means the
 *         pin 0 and 17 have the interrupt.
 */
uint32_t FGPIO_PortGetInterruptFlags(FGPIO_Type *base)
{
    uint8_t instance;
    instance = (uint8_t)FGPIO_GetInstance(base);
    PORT_Type *portBase;
    portBase = s_portBases[instance];
    return portBase->ISFR;
}

/*!
 * brief Clears the multiple FGPIO pin interrupt status flag.
 *
 * param base FGPIO peripheral base pointer (FGPIOA, FGPIOB, FGPIOC, and so on.)
 * param mask FGPIO pin number macro
 */
void FGPIO_PortClearInterruptFlags(FGPIO_Type *base, uint32_t mask)
{
    uint8_t instance;
    instance = (uint8_t)FGPIO_GetInstance(base);
    PORT_Type *portBase;
    portBase       = s_portBases[instance];
    portBase->ISFR = mask;
}
#endif
#if defined(FSL_FEATURE_FGPIO_HAS_ATTRIBUTE_CHECKER) && FSL_FEATURE_FGPIO_HAS_ATTRIBUTE_CHECKER
/*!
 * brief The FGPIO module supports a device-specific number of data ports, organized as 32-bit
 * words. Each 32-bit data port includes a GACR register, which defines the byte-level
 * attributes required for a successful access to the GPIO programming model. The attribute controls for the 4 data
 * bytes in the GACR follow a standard little endian
 * data convention.
 *
 * param base FGPIO peripheral base pointer (FGPIOA, FGPIOB, FGPIOC, and so on.)
 * param mask FGPIO pin number macro
 */
void FGPIO_CheckAttributeBytes(FGPIO_Type *base, gpio_checker_attribute_t attribute)
{
    base->GACR = (attribute << FGPIO_GACR_ACB0_SHIFT) | (attribute << FGPIO_GACR_ACB1_SHIFT) |
                 (attribute << FGPIO_GACR_ACB2_SHIFT) | (attribute << FGPIO_GACR_ACB3_SHIFT);
}
#endif

#endif /* FSL_FEATURE_SOC_FGPIO_COUNT */

/******************************************************************/
 /**
 * @brief  è®¾ç½®å¼•è„šä¸ºè¾“å…¥è¿˜æ˜¯è¾“å‡ºåŠŸèƒ½  ç”¨æˆ·ä¸€èˆ¬ä¸å¿…è°ƒç”¨
 * @note   åªæœ‰å½“å¼•è„šä½œä¸ºGPIOæ—¶æ‰æœ‰æ„ä¹‰
 * @code
 *      // å°†PORTBç«¯å£çš„3å¼•è„šè®¾ç½®è¾“å…¥å¼•è„š
 *      GPIO_PinConfig(HW_GPIOB, 3, kInpput);
 * @endcode
 * @param[in]  instance GPIOæ¨¡å—å·
 *              @arg HW_GPIOA èŠ¯ç‰‡çš„PORTAç«¯å£
 *              @arg HW_GPIOB èŠ¯ç‰‡çš„PORTBç«¯å£
 *              @arg HW_GPIOC èŠ¯ç‰‡çš„PORTCç«¯å£
 *              @arg HW_GPIOD èŠ¯ç‰‡çš„PORTDç«¯å£
 *              @arg HW_GPIOE èŠ¯ç‰‡çš„PORTEç«¯å£
 * @param[in]  pin  ç«¯å£ä¸Šçš„å¼•è„šå· 0~31
 * @param[in]  mode è¾“å…¥æˆ–è€…è¾“å‡ºè®¾ç½®
 *              @arg kInpput  è¾“å…¥åŠŸèƒ½é€‰æ‹©
 *              @arg kOutput  è¾“å‡ºåŠŸèƒ½é€‰æ‹©
 * @retval None
 */
void GPIO_PinConfig(uint32_t instance, uint8_t pin, gpio_pin_direction_t mode)
{
    SIM->SCGC5 |= SIM_GPIOClockGateTable[instance];
    (mode == kGPIO_DigitalOutput) ? (GPIO_InstanceTable[instance]->PDDR |= (1 << pin)):(GPIO_InstanceTable[instance]->PDDR &= ~(1 << pin));
}

void PORT_PinMuxConfig(uint32_t instance, uint8_t pin, port_mux_t pinMux)
{
    SIM->SCGC5 |= SIM_GPIOClockGateTable[instance];
    PORT_InstanceTable[instance]->PCR[pin] &= ~(PORT_PCR_MUX_MASK);
    PORT_InstanceTable[instance]->PCR[pin] |=  PORT_PCR_MUX(pinMux);
}

void PORT_PinOpenDrainConfig(uint32_t instance, uint8_t pin, bool status)
{
    SIM->SCGC5 |= SIM_GPIOClockGateTable[instance];
    (status) ? (PORT_InstanceTable[instance]->PCR[pin] |= PORT_PCR_ODE_MASK):(PORT_InstanceTable[instance]->PCR[pin] &= ~PORT_PCR_ODE_MASK);
}

/**
 * @brief  set pin internal pullup/down resistors
 * @note   pull resistor value is about 20K
 * @param[in]  instance GPIOæ¨¡å—å·
 *              @arg HW_GPIOx  GPIOx moudle
 * @param[in]  pin  pin index number 0-31
 * @param[in]  pull pull select
 *              @arg kPullDisabled  disable pull resistor
 *              @arg kPullUp        pull up
 *              @arg kPullDown      pull down
 * @retval None
 */
void PORT_PinPullConfig(uint32_t instance, uint8_t pin, port_pull pull)
{
    SIM->SCGC5 |= SIM_GPIOClockGateTable[instance];
    switch(pull)
    {
        case kPORT_PullDisable:
            PORT_InstanceTable[instance]->PCR[pin] &= ~PORT_PCR_PE_MASK;
            break;
        case kPORT_PullUp:
            PORT_InstanceTable[instance]->PCR[pin] |= PORT_PCR_PE_MASK;
            PORT_InstanceTable[instance]->PCR[pin] |= PORT_PCR_PS_MASK;
            break;
        case kPORT_PullDown :
            PORT_InstanceTable[instance]->PCR[pin] |= PORT_PCR_PE_MASK;
            PORT_InstanceTable[instance]->PCR[pin] &= ~PORT_PCR_PS_MASK;
            break;
        default:
            break;
    }
}

/**
 * @brief  GPIO³õÊ¼»¯ÅäÖÃ
 * @code
 *    //³õÊ¼»¯ÅäÖÃPORTB¶Ë¿ÚµÄ10Òý½ÅÎªÍÆÍìÊä³öÒý½Å
 *    GPIO_InitTypeDef GPIO_InitStruct1;      //ÉêÇëÒ»¸ö½á¹¹±äÁ¿
 *    GPIO_InitStruct1.instance = HW_GPIOB;   //Ñ¡ÔñPORTB¶Ë¿Ú
 *    GPIO_InitStruct1.mode = kGPIO_Mode_OPP; //ÍÆÍìÊä³ö
 *    GPIO_InitStruct1.pinx = 10;             //Ñ¡Ôñ10Òý½Å
 *    //µ÷ÓÃ³õÊ¼»¯GPIOº¯Êý 
 *    GPIO_Init(&GPIO_InitStruct1);
 * @endcode
 * @param[in]  GPIO_InitStruct GPIO³õÊ¼»¯½á¹¹Ìå£¬°üº¬ÁËÒý½Å×´Ì¬²ÎÊý  
 * @see GPIO³õÊ¼»¯ÅäÖÃÀý³Ì
 * @retval None
 */
void GPIO_Init(GPIO_InitTypeDef * GPIO_InitStruct)
{
    /* config state */
    switch(GPIO_InitStruct->mode)
    {
        case kGPIO_Mode_IFT:
            PORT_PinPullConfig(GPIO_InitStruct->instance, GPIO_InitStruct->pinx, kPORT_PullDisable);
            PORT_PinOpenDrainConfig(GPIO_InitStruct->instance, GPIO_InitStruct->pinx, DISABLE);
            GPIO_PinConfig(GPIO_InitStruct->instance, GPIO_InitStruct->pinx,  kGPIO_DigitalInput);
            break;
        case kGPIO_Mode_IPD:
            PORT_PinPullConfig(GPIO_InitStruct->instance, GPIO_InitStruct->pinx,  kPORT_PullDown);
            PORT_PinOpenDrainConfig(GPIO_InitStruct->instance, GPIO_InitStruct->pinx, DISABLE);
            GPIO_PinConfig(GPIO_InitStruct->instance, GPIO_InitStruct->pinx,  kGPIO_DigitalInput);
            break;
        case kGPIO_Mode_IPU:
            PORT_PinPullConfig(GPIO_InitStruct->instance, GPIO_InitStruct->pinx, kPORT_PullUp);
            PORT_PinOpenDrainConfig(GPIO_InitStruct->instance, GPIO_InitStruct->pinx, DISABLE);
            GPIO_PinConfig(GPIO_InitStruct->instance, GPIO_InitStruct->pinx,  kGPIO_DigitalInput);
            break;
        case kGPIO_Mode_OOD:
            PORT_PinPullConfig(GPIO_InitStruct->instance, GPIO_InitStruct->pinx, kPORT_PullUp);
            PORT_PinOpenDrainConfig(GPIO_InitStruct->instance, GPIO_InitStruct->pinx, ENABLE);
            GPIO_PinConfig(GPIO_InitStruct->instance, GPIO_InitStruct->pinx, kGPIO_DigitalOutput);
            break;
        case kGPIO_Mode_OPP:
            PORT_PinPullConfig(GPIO_InitStruct->instance, GPIO_InitStruct->pinx, kPORT_PullDisable);
            PORT_PinOpenDrainConfig(GPIO_InitStruct->instance, GPIO_InitStruct->pinx, DISABLE);
            GPIO_PinConfig(GPIO_InitStruct->instance, GPIO_InitStruct->pinx, kGPIO_DigitalOutput);
            break;
        default:
            break;					
    }
    /* config pinMux */
    PORT_PinMuxConfig(GPIO_InitStruct->instance, GPIO_InitStruct->pinx, kPORT_MuxAsGpio );
  //  PORT_SetPinMux(GPIO_InitStruct->instance, GPIO_InitStruct->pinx, kPORT_MuxAsGpio); //WT.EDIT
}
/**
 * @brief  ¿ìËÙ³õÊ¼»¯Ò»¸öGPIOÒý½Å Êµ¼ÊÉÏÊÇGPIO_InitµÄ×î¼òµ¥ÅäÖÃ
 * @code
 *      //³õÊ¼»¯ÅäÖÃPORTB¶Ë¿ÚµÄ10Òý½ÅÎªÍÆÍìÊä³öÒý½Å
 *      GPIO_QuickInit(HW_GPIOB, 10, kGPIO_Mode_OPP);
 * @endcode
 * @param[in]  instance GPIOÄ£¿éºÅ
 *              @arg HW_GPIOA Ð¾Æ¬µÄPORTA¶Ë¿Ú
 *              @arg HW_GPIOB Ð¾Æ¬µÄPORTB¶Ë¿Ú
 *              @arg HW_GPIOC Ð¾Æ¬µÄPORTC¶Ë¿Ú
 *              @arg HW_GPIOD Ð¾Æ¬µÄPORTD¶Ë¿Ú
 *              @arg HW_GPIOE Ð¾Æ¬µÄPORTE¶Ë¿Ú
 * @param[in]  pinx ¶Ë¿ÚÉÏµÄÒý½ÅºÅ 0~31
 * @param[in]  mode Òý½Å¹¤×÷Ä£Ê½
 *              @arg kGPIO_Mode_IFT Ðü¿ÕÊäÈë
 *              @arg kGPIO_Mode_IPD ÏÂÀ­ÊäÈë
 *              @arg kGPIO_Mode_IPU ÉÏÀ­ÊäÈë
 *              @arg kGPIO_Mode_OOD ¿ªÂ©Êä³ö
 *              @arg kGPIO_Mode_OPP ÍÆÍìÊä³ö
 * @retval instance GPIOÄ£¿éºÅ
 */
uint8_t GPIO_QuickInit(uint32_t instance, uint32_t pinx, GPIO_Mode_Type mode)
{
    GPIO_InitTypeDef GPIO_InitStruct1;
    GPIO_InitStruct1.instance = instance;
    GPIO_InitStruct1.mode = mode;
    GPIO_InitStruct1.pinx = pinx;
    GPIO_Init(&GPIO_InitStruct1);
    return  instance;
}

