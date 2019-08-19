C_FEVT_AC12EBE_MASK 0x8u �8SDHC_FEVT_AC12EBE_SHIFT 3 �8SDHC_FEVT_AC12IE_MASK 0x10u �8SDHC_FEVT_AC12IE_SHIFT 4 �8SDHC_FEVT_CNIBAC12E_MASK 0x80u �8SDHC_FEVT_CNIBAC12E_SHIFT 7 �8SDHC_FEVT_CTOE_MASK 0x10000u �8SDHC_FEVT_CTOE_SHIFT 16 �8SDHC_FEVT_CCE_MASK 0x20000u �8SDHC_FEVT_CCE_SHIFT 17 �8SDHC_FEVT_CEBE_MASK 0x40000u �8SDHC_FEVT_CEBE_SHIFT 18 �8SDHC_FEVT_CIE_MASK 0x80000u �8SDHC_FEVT_CIE_SHIFT 19 �8SDHC_FEVT_DTOE_MASK 0x100000u �8SDHC_FEVT_DTOE_SHIFT 20 �8SDHC_FEVT_DCE_MASK 0x200000u �8SDHC_FEVT_DCE_SHIFT 21 �8SDHC_FEVT_DEBE_MASK 0x400000u �8SDHC_FEVT_DEBE_SHIFT 22 �8SDHC_FEVT_AC12E_MASK 0x1000000u �8SDHC_FEVT_AC12E_SHIFT 24 �8SDHC_FEVT_DMAE_MASK 0x10000000u �8SDHC_FEVT_DMAE_SHIFT 28 �8SDHC_FEVT_CINT_MASK 0x80000000u �8SDHC_FEVT_CINT_SHIFT 31 �8SDHC_ADMAES_ADMAES_MASK 0x3u �8SDHC_ADMAES_ADMAES_SHIFT 0 �8SDHC_ADMAES_ADMAES(x) (((uint32_t)(((uint32_t)(x))<<SDHC_ADMAES_ADMAES_SHIFT))&SDHC_ADMAES_ADMAES_MASK) �8SDHC_ADMAES_ADMALME_MASK 0x4u �8SDHC_ADMAES_ADMALME_SHIFT 2 �8SDHC_ADMAES_ADMADCE_MASK 0x8u �8SDHC_ADMAES_ADMADCE_SHIFT 3 �8SDHC_ADSADDR_ADSADDR_MASK 0xFFFFFFFCu �8SDHC_ADSADDR_ADSADDR_SHIFT 2 �8SDHC_ADSADDR_ADSADDR(x) (((uint32_t)(((uint32_t)(x))<<SDHC_ADSADDR_ADSADDR_SHIFT))&SDHC_ADSADDR_ADSADDR_MASK) �8SDHC_VENDOR_EXTDMAEN_MASK 0x1u �8SDHC_VENDOR_EXTDMAEN_SHIFT 0 �8SDHC_VENDOR_EXBLKNU_MASK 0x2u �8SDHC_VENDOR_EXBLKNU_SHIFT 1 �8SDHC_VENDOR_INTSTVAL_MASK 0xFF0000u �8SDHC_VENDOR_INTSTVAL_SHIFT 16 �8SDHC_VENDOR_INTSTVAL(x) (((uint32_t)(((uint32_t)(x))<<SDHC_VENDOR_INTSTVAL_SHIFT))&SDHC_VENDOR_INTSTVAL_MASK) �8SDHC_MMCBOOT_DTOCVACK_MASK 0xFu �8SDHC_MMCBOOT_DTOCVACK_SHIFT 0 �8SDHC_MMCBOOT_DTOCVACK(x) (((uint32_t)(((uint32_t)(x))<<SDHC_MMCBOOT_DTOCVACK_SHIFT))&SDHC_MMCBOOT_DTOCVACK_MASK) �8SDHC_MMCBOOT_BOOTACK_MASK 0x10u �8SDHC_MMCBOOT_BOOTACK_SHIFT 4 �8SDHC_MMCBOOT_BOOTMODE_MASK 0x20u �8SDHC_MMCBOOT_BOOTMODE_SHIFT 5 �8SDHC_MMCBOOT_BOOTEN_MASK 0x40u �8SDHC_MMCBOOT_BOOTEN_SHIFT 6 �8SDHC_MMCBOOT_AUTOSABGEN_MASK 0x80u �8SDHC_MMCBOOT_AUTOSABGEN_SHIFT 7 �8SDHC_MMCBOOT_BOOTBLKCNT_MASK 0xFFFF0000u �8SDHC_MMCBOOT_BOOTBLKCNT_SHIFT 16 �8SDHC_MMCBOOT_BOOTBLKCNT(x) (((uint32_t)(((uint32_t)(x))<<SDHC_MMCBOOT_BOOTBLKCNT_SHIFT))&SDHC_MMCBOOT_BOOTBLKCNT_MASK) �8SDHC_HOSTVER_SVN_MASK 0xFFu �8SDHC_HOSTVER_SVN_SHIFT 0 �8SDHC_HOSTVER_SVN(x) (((uint32_t)(((uint32_t)(x))<<SDHC_HOSTVER_SVN_SHIFT))&SDHC_HOSTVER_SVN_MASK) �9SDHC_HOSTVER_VVN_MASK 0xFF00u �9SDHC_HOSTVER_VVN_SHIFT 8 �9SDHC_HOSTVER_VVN(x) (((uint32_t)(((uint32_t)(x))<<SDHC_HOSTVER_VVN_SHIFT))&SDHC_HOSTVER_VVN_MASK) �9SDHC_BASE (0x400B1000u) �9SDHC ((SDHC_Type *)SDHC_BASE) �9SDHC_BASES { SDHC } �9SIM_SOPT1_RAMSIZE_MASK 0xF000u �9SIM_SOPT1_RAMSIZE_SHIFT 12 �9SIM_SOPT1_RAMSIZE(x) (((uint32_t)(((uint32_t)(x))<<SIM_SOPT1_RAMSIZE_SHIFT))&SIM_SOPT1_RAMSIZE_MASK) �9SIM_SOPT1_OSC32KSEL_MASK 0xC0000u �9SIM_SOPT1_OSC32KSEL_SHIFT 18 �9SIM_SOPT1_OSC32KSEL(x) (((uint32_t)(((uint32_t)(x))<<SIM_SOPT1_OSC32KSEL_SHIFT))&SIM_SOPT1_OSC32KSEL_MASK) �9SIM_SOPT1_USBVSTBY_MASK 0x20000000u �9SIM_SOPT1_USBVSTBY_SHIFT 29 �9SIM_SOPT1_USBSSTBY_MASK 0x40000000u �9SIM_SOPT1_USBSSTBY_SHIFT 30 �9SIM_SOPT1_USBREGEN_MASK 0x80000000u �9SIM_SOPT1_USBREGEN_SHIFT 31 �9SIM_SOPT1CFG_URWE_MASK 0x1000000u �9SIM_SOPT1CFG_URWE_SHIFT 24 �9SIM_SOPT1CFG_UVSWE_MASK 0x2000000u �9SIM_SOPT1CFG_UVSWE_SHIFT 25 �9SIM_SOPT1CFG_USSWE_MASK 0x4000000u �9SIM_SOPT1CFG_USSWE_SHIFT 26 �9SIM_SOPT2_RTCCLKOUTSEL_MASK 0x10u �9SIM_SOPT2_RTCCLKOUTSEL_SHIFT 4 �9SIM_SOPT2_CLKOUTSEL_MASK 0xE0u �9SIM_SOPT2_CLKOUTSEL_SHIFT 5 �9SIM_SOPT2_CLKOUTSEL(x) (((uint32_t)(((uint32_t)(x))<<SIM_SOPT2_CLKOUTSEL_SHIFT))&SIM_SOPT2_CLKOUTSEL_MASK) �9SIM_SOPT2_FBSL_MASK 0x300u �9SIM_SOPT2_FBSL_SHIFT 8 �9SIM_SOPT2_FBSL(x) (((uint32_t)(((uint32_t)(x))<<SIM_SOPT2_FBSL_SHIFT))&SIM_SOPT2_FBSL_MASK) �9SIM_SOPT2_PTD7PAD_MASK 0x800u �9SIM_SOPT2_PTD7PAD_SHIFT 11 �9SIM_SOPT2_TRACECLKSEL_MASK 0x1000u �9SIM_SOPT2_TRACECLKSEL_SHIFT 12 �9SIM_SOPT2_PLLFLLSEL_MASK 0x10000u �9SIM_SOPT2_PLLFLLSEL_SHIFT 16 �9SIM_SOPT2_USBSRC_MASK 0x40000u �9SIM_SOPT2_USBSRC_SHIFT 18 �9SIM_SOPT2_RMIISRC_MASK 0x80000u �9SIM_SOPT2_RMIISRC_SHIFT 19 �9SIM_SOPT2_TIMESRC_MASK 0x300000u �9SIM_SOPT2_TIMESRC_SHIFT 20 �9SIM_SOPT2_TIMESRC(x) (((uint32_t)(((uint32_t)(x))<<SIM_SOPT2_TIMESRC_SHIFT))&SIM_SOPT2_TIMESRC_MASK) �9SIM_SOPT2_SDHCSRC_MASK 0x30000000u �9SIM_SOPT2_SDHCSRC_SHIFT 28 �9SIM_SOPT2_SDHCSRC(x) (((uint32_t)(((uint32_t)(x))<<SIM_SOPT2_SDHCSRC_SHIFT))&SIM_SOPT2_SDHCSRC_MASK) �9SIM_SOPT4_FTM0FLT0_MASK 0x1u �9SIM_SOPT4_FTM0FLT0_SHIFT 0 �9SIM_SOPT4_FTM0FLT1_MASK 0x2u �9SIM_SOPT4_FTM0FLT1_SHIFT 1 �9SIM_SOPT4_FTM0FLT2_MASK 0x4u �9SIM_SOPT4_FTM0FLT2_SHIFT 2 �9SIM_SOPT4_FTM1FLT0_MASK 0x10u �9SIM_SOPT4_FTM1FLT0_SHIFT 4 �9SIM_SOPT4_FTM2FLT0_MASK 0x100u �9SIM_SOPT4_FTM2FLT0_SHIFT 8 �9SIM_SOPT4_FTM1CH0SRC_MASK 0xC0000u �9SIM_SOPT4_FTM1CH0SRC_SHIFT 18 �:SIM_SOPT4_FTM1CH0SRC(x) (((uint32_t)(((uint32_t)(x))<<SIM_SOPT4_FTM1CH0SRC_SHIFT))&SIM_SOPT4_FTM1CH0SRC_MASK) �:SIM_SOPT4_FTM2CH0SRC_MASK 0x300000u �:SIM_SOPT4_FTM2CH0SRC_SHIFT 20 �:SIM_SOPT4_FTM2CH0SRC(x) (((uint32_t)(((uint32_t)(x))<<SIM_SOPT4_FTM2CH0SRC_SHIFT))&SIM_SOPT4_FTM2CH0SRC_MASK) �:SIM_SOPT4_FTM0CLKSEL_MASK 0x1000000u �:SIM_SOPT4_FTM0CLKSEL_SHIFT 24 �:SIM_SOPT4_FTM1CLKSEL_MASK 0x2000000u �:SIM_SOPT4_FTM1CLKSEL_SHIFT 25 �:SIM_SOPT4_FTM2CLKSEL_MASK 0x4000000u �:SIM_SOPT4_FTM2CLKSEL_SHIFT 26 �:SIM_SOPT4_FTM0TRG0SRC_MASK 0x10000000u �:SIM_SOPT4_FTM0TRG0SRC_SHIFT 28 �:SIM_SOPT4_FTM0TRG1SRC_MASK 0x20000000u �:SIM_SOPT4_FTM0TRG1SRC_SHIFT 29 �:SIM_SOPT5_UART0TXSRC_MASK 0x3u �:SIM_SOPT5_UART0TXSRC_SHIFT 0 �:SIM_SOPT5_UART0TXSRC(x) (((uint32_t)(((uint32_t)(x))<<SIM_SOPT5_UART0TXSRC_SHIFT))&SIM_SOPT5_UART0TXSRC_MASK) �:SIM_SOPT5_UART0RXSRC_MASK 0xCu �:SIM_SOPT5_UART0RXSRC_SHIFT 2 �:SIM_SOPT5_UART0RXSRC(x) (((uint32_t)(((uint32_t)(x))<<SIM_SOPT5_UART0RXSRC_SHIFT))&SIM_SOPT5_UART0RXSRC_MASK) �:SIM_SOPT5_UART1TXSRC_MASK 0x30u �:SIM_SOPT5_UART1TXSRC_SHIFT 4 �:SIM_SOPT5_UART1TXSRC(x) (((uint32_t)(((uint32_t)(x))<<SIM_SOPT5_UART1TXSRC_SHIFT))&SIM_SOPT5_UART1TXSRC_MASK) �:SIM_SOPT5_UART1RXSRC_MASK 0xC0u �:SIM_SOPT5_UART1RXSRC_SHIFT 6 �:SIM_SOPT5_UART1RXSRC(x) (((uint32_t)(((uint32_t)(x))<<SIM_SOPT5_UART1RXSRC_SHIFT))&SIM_SOPT5_UART1RXSRC_MASK) �:SIM_SOPT7_ADC0TRGSEL_MASK 0xFu �:SIM_SOPT7_ADC0TRGSEL_SHIFT 0 �:SIM_SOPT7_ADC0TRGSEL(x) (((uint32_t)(((uint32_t)(x))<<SIM_SOPT7_ADC0TRGSEL_SHIFT))&SIM_SOPT7_ADC0TRGSEL_MASK) �:SIM_SOPT7_ADC0PRETRGSEL_MASK 0x10u �:SIM_SOPT7_ADC0PRETRGSEL_SHIFT 4 �:SIM_SOPT7_ADC0ALTTRGEN_MASK 0x80u �:SIM_SOPT7_ADC0ALTTRGEN_SHIFT 7 �:SIM_SOPT7_ADC1TRGSEL_MASK 0xF00u �:SIM_SOPT7_ADC1TRGSEL_SHIFT 8 �:SIM_SOPT7_ADC1TRGSEL(x) (((uint32_t)(((uint32_t)(x))<<SIM_SOPT7_ADC1TRGSEL_SHIFT))&SIM_SOPT7_ADC1TRGSEL_MASK) �:SIM_SOPT7_ADC1PRETRGSEL_MASK 0x1000u �:SIM_SOPT7_ADC1PRETRGSEL_SHIFT 12 �:SIM_SOPT7_ADC1ALTTRGEN_MASK 0x8000u �:SIM_SOPT7_ADC1ALTTRGEN_SHIFT 15 �:SIM_SDID_PINID_MASK 0xFu �:SIM_SDID_PINID_SHIFT 0 �:SIM_SDID_PINID(x) (((uint32_t)(((uint32_t)(x))<<SIM_SDID_PINID_SHIFT))&SIM_SDID_PINID_MASK) �:SIM_SDID_FAMID_MASK 0x70u �:SIM_SDID_FAMID_SHIFT 4 �:SIM_SDID_FAMID(x) (((uint32_t)(((uint32_t)(x))<<SIM_SDID_FAMID_SHIFT))&SIM_SDID_FAMID_MASK) �:SIM_SDID_REVID_MASK 0xF000u �:SIM_SDID_REVID_SHIFT 12 �:SIM_SDID_REVID(x) (((uint32_t)(((uint32_t)(x))<<SIM_SDID_REVID_SHIFT))&SIM_SDID_REVID_MASK) �:SIM_SCGC1_UART4_MASK 0x400u �:SIM_SCGC1_UART4_SHIFT 10 �:SIM_SCGC1_UART5_MASK 0x800u �:SIM_SCGC1_UART5_SHIFT 11 �:SIM_SCGC2_ENET_MASK 0x1u �:SIM_SCGC2_ENET_SHIFT 0 �:SIM_SCGC2_DAC0_MASK 0x1000u �:SIM_SCGC2_DAC0_SHIFT 12 �:SIM_SCGC2_DAC1_MASK 0x2000u �:SIM_SCGC2_DAC1_SHIFT 13 �:SIM_SCGC3_RNGA_MASK 0x1u �:SIM_SCGC3_RNGA_SHIFT 0 �:SIM_SCGC3_FLEXCAN1_MASK 0x10u �:SIM_SCGC3_FLEXCAN1_SHIFT 4 �:SIM_SCGC3_SPI2_MASK 0x1000u �:SIM_SCGC3_SPI2_SHIFT 12 �:SIM_SCGC3_SDHC_MASK 0x20000u �:SIM_SCGC3_SDHC_SHIFT 17 �:SIM_SCGC3_FTM2_MASK 0x1000000u �:SIM_SCGC3_FTM2_SHIFT 24 �:SIM_SCGC3_ADC1_MASK 0x8000000u �:SIM_SCGC3_ADC1_SHIFT 27 �:SIM_SCGC4_EWM_MASK 0x2u �:SIM_SCGC4_EWM_SHIFT 1 �:SIM_SCGC4_CMT_MASK 0x4u �:SIM_SCGC4_CMT_SHIFT 2 �:SIM_SCGC4_I2C0_MASK 0x40u �:SIM_SCGC4_I2C0_SHIFT 6 �:SIM_SCGC4_I2C1_MASK 0x80u �:SIM_SCGC4_I2C1_SHIFT 7 �:SIM_SCGC4_UART0_MASK 0x400u �:SIM_SCGC4_UART0_SHIFT 10 �:SIM_SCGC4_UART1_MASK 0x800u �:SIM_SCGC4_UART1_SHIFT 11 �:SIM_SCGC4_UART2_MASK 0x1000u �:SIM_SCGC4_UART2_SHIFT 12 �:SIM_SCGC4_UART3_MASK 0x2000u �:SIM_SCGC4_UART3_SHIFT 13 �:SIM_SCGC4_USBOTG_MASK 0x40000u �:SIM_SCGC4_USBOTG_SHIFT 18 �:SIM_SCGC4_CMP_MASK 0x80000u �:SIM_SCGC4_CMP_SHIFT 19 �:SIM_SCGC4_VREF_MASK 0x100000u �:SIM_SCGC4_VREF_SHIFT 20 �:SIM_SCGC4_LLWU_MASK 0x10000000u �:SIM_SCGC4_LLWU_SHIFT 28 �:SIM_SCGC5_LPTIMER_MASK 0x1u �:SIM_SCGC5_LPTIMER_SHIFT 0 �:SIM_SCGC5_TSI_MASK 0x20u �:SIM_SCGC5_TSI_SHIFT 5 �:SIM_SCGC5_PORTA_MASK 0x200u �:SIM_SCGC5_PORTA_SHIFT 9 �:SIM_SCGC5_PORTB_MASK 0x400u �:SIM_SCGC5_PORTB_SHIFT 10 �:SIM_SCGC5_PORTC_MASK 0x800u �:SIM_SCGC5_PORTC_SHIFT 11 �:SIM_SCGC5_PORTD_MASK 0x1000u �:SIM_SCGC5_PORTD_SHIFT 12 �:SIM_SCGC5_PORTE_MASK 0x2000u �:SIM_SCGC5_PORTE_SHIFT 13 �:SIM_SCGC6_FTFL_MASK 0x1u �:SIM_SCGC6_FTFL_SHIFT 0 �:SIM_SCGC6_DMAMUX_MASK 0x2u �:SIM_SCGC6_DMAMUX_SHIFT 1 �:SIM_SCGC6_FLEXCAN0_MASK 0x10u �:SIM_SCGC6_FLEXCAN0_SHIFT 4 �:SIM_SCGC6_SPI0_MASK 0x1000u �:SIM_SCGC6_SPI0_SHIFT 12 �:SIM_SCGC6_SPI1_MASK 0x2000u �:SIM_SCGC6_SPI1_SHIFT 13 �;SIM_SCGC6_I2S_MASK 0x8000u �;SIM_SCGC6_I2S_SHIFT 15 �;SIM_SCGC6_CRC_MASK 0x40000u �;SIM_SCGC6_CRC_SHIFT 18 �;SIM_SCGC6_USBDCD_MASK 0x200000u �;SIM_SCGC6_USBDCD_SHIFT 21 �;SIM_SCGC6_PDB_MASK 0x400000u �;SIM_SCGC6_PDB_SHIFT 22 �;SIM_SCGC6_PIT_MASK 0x800000u �;SIM_SCGC6_PIT_SHIFT 23 �;SIM_SCGC6_FTM0_MASK 0x1000000u �;SIM_SCGC6_FTM0_SHIFT 24 �;SIM_SCGC6_FTM1_MASK 0x2000000u �;SIM_SCGC6_FTM1_SHIFT 25 �;SIM_SCGC6_ADC0_MASK 0x8000000u �;SIM_SCGC6_ADC0_SHIFT 27 �;SIM_SCGC6_RTC_MASK 0x20000000u �;SIM_SCGC6_RTC_SHIFT 29 �;SIM_SCGC7_FLEXBUS_MASK 0x1u �;SIM_SCGC7_FLEXBUS_SHIFT 0 �;SIM_SCGC7_DMA_MASK 0x2u �;SIM_SCGC7_DMA_SHIFT 1 �;SIM_SCGC7_MPU_MASK 0x4u �;SIM_SCGC7_MPU_SHIFT 2 �;SIM_CLKDIV1_OUTDIV4_MASK 0xF0000u �;SIM_CLKDIV1_OUTDIV4_SHIFT 16 �;SIM_CLKDIV1_OUTDIV4(x) (((uint32_t)(((uint32_t)(x))<<SIM_CLKDIV1_OUTDIV4_SHIFT))&SIM_CLKDIV1_OUTDIV4_MASK) �;SIM_CLKDIV1_OUTDIV3_MASK 0xF00000u �;SIM_CLKDIV1_OUTDIV3_SHIFT 20 �;SIM_CLKDIV1_OUTDIV3(x) (((uint32_t)(((uint32_t)(x))<<SIM_CLKDIV1_OUTDIV3_SHIFT))&SIM_CLKDIV1_OUTDIV3_MASK) �;SIM_CLKDIV1_OUTDIV2_MASK 0xF000000u �;SIM_CLKDIV1_OUTDIV2_SHIFT 24 �;SIM_CLKDIV1_OUTDIV2(x) (((uint32_t)(((uint32_t)(x))<<SIM_CLKDIV1_OUTDIV2_SHIFT))&SIM_CLKDIV1_OUTDIV2_MASK) �;SIM_CLKDIV1_OUTDIV1_MASK 0xF0000000u �;SIM_CLKDIV1_OUTDIV1_SHIFT 28 �;SIM_CLKDIV1_OUTDIV1(x) (((uint32_t)(((uint32_t)(x))<<SIM_CLKDIV1_OUTDIV1_SHIFT))&SIM_CLKDIV1_OUTDIV1_MASK) �;SIM_CLKDIV2_USBFRAC_MASK 0x1u �;SIM_CLKDIV2_USBFRAC_SHIFT 0 �;SIM_CLKDIV2_USBDIV_MASK 0xEu �;SIM_CLKDIV2_USBDIV_SHIFT 1 �;SIM_CLKDIV2_USBDIV(x) (((uint32_t)(((uint32_t)(x))<<SIM_CLKDIV2_USBDIV_SHIFT))&SIM_CLKDIV2_USBDIV_MASK) �;SIM_FCFG1_FLASHDIS_MASK 0x1u �;SIM_FCFG1_FLASHDIS_SHIFT 0 �;SIM_FCFG1_FLASHDOZE_MASK 0x2u �;SIM_FCFG1_FLASHDOZE_SHIFT 1 �;SIM_FCFG1_DEPART_MASK 0xF00u �;SIM_FCFG1_DEPART_SHIFT 8 �;SIM_FCFG1_DEPART(x) (((uint32_t)(((uint32_t)(x))<<SIM_FCFG1_DEPART_SHIFT))&SIM_FCFG1_DEPART_MASK) �;SIM_FCFG1_EESIZE_MASK 0xF0000u �;SIM_FCFG1_EESIZE_SHIFT 16 �;SIM_FCFG1_EESIZE(x) (((uint32_t)(((uint32_t)(x))<<SIM_FCFG1_EESIZE_SHIFT))&SIM_FCFG1_EESIZE_MASK) �;SIM_FCFG1_PFSIZE_MASK 0xF000000u �;SIM_FCFG1_PFSIZE_SHIFT 24 �;SIM_FCFG1_PFSIZE(x) (((uint32_t)(((uint32_t)(x))<<SIM_FCFG1_PFSIZE_SHIFT))&SIM_FCFG1_PFSIZE_MASK) �;SIM_FCFG1_NVMSIZE_MASK 0xF0000000u �;SIM_FCFG1_NVMSIZE_SHIFT 28 �;SIM_FCFG1_NVMSIZE(x) (((uint32_t)(((uint32_t)(x))<<SIM_FCFG1_NVMSIZE_SHIFT))&SIM_FCFG1_NVMSIZE_MASK) �;SIM_FCFG2_MAXADDR1_MASK 0x7F0000u �;SIM_FCFG2_MAXADDR1_SHIFT 16 �;SIM_FCFG2_MAXADDR1(x) (((uint32_t)(((uint32_t)(x))<<SIM_FCFG2_MAXADDR1_SHIFT))&SIM_FCFG2_MAXADDR1_MASK) �;SIM_FCFG2_PFLSH_MASK 0x800000u �;SIM_FCFG2_PFLSH_SHIFT 23 �;SIM_FCFG2_MAXADDR0_MASK 0x7F000000u �;SIM_FCFG2_MAXADDR0_SHIFT 24 �;SIM_FCFG2_MAXADDR0(x) (((uint32_t)(((uint32_t)(x))<<SIM_FCFG2_MAXADDR0_SHIFT))&SIM_FCFG2_MAXADDR0_MASK) �;SIM_FCFG2_SWAPPFLSH_MASK 0x80000000u �;SIM_FCFG2_SWAPPFLSH_SHIFT 31 �;SIM_UIDH_UID_MASK 0xFFFFFFFFu �;SIM_UIDH_UID_SHIFT 0 �;SIM_UIDH_UID(x) (((uint32_t)(((uint32_t)(x))<<SIM_UIDH_UID_SHIFT))&SIM_UIDH_UID_MASK) �;SIM_UIDMH_UID_MASK 0xFFFFFFFFu �;SIM_UIDMH_UID_SHIFT 0 �;SIM_UIDMH_UID(x) (((uint32_t)(((uint32_t)(x))<<SIM_UIDMH_UID_SHIFT))&SIM_UIDMH_UID_MASK) �;SIM_UIDML_UID_MASK 0xFFFFFFFFu �;SIM_UIDML_UID_SHIFT 0 �;SIM_UIDML_UID(x) (((uint32_t)(((uint32_t)(x))<<SIM_UIDML_UID_SHIFT))&SIM_UIDML_UID_MASK) �;SIM_UIDL_UID_MASK 0xFFFFFFFFu �;SIM_UIDL_UID_SHIFT 0 �;SIM_UIDL_UID(x) (((uint32_t)(((uint32_t)(x))<<SIM_UIDL_UID_SHIFT))&SIM_UIDL_UID_MASK) �;SIM_BASE (0x40047000u) �;SIM ((SIM_Type *)SIM_BASE) �;SIM_BASES { SIM } �<SMC_PMPROT_AVLLS_MASK 0x2u �<SMC_PMPROT_AVLLS_SHIFT 1 �<SMC_PMPROT_ALLS_MASK 0x8u �<SMC_PMPROT_ALLS_SHIFT 3 �<SMC_PMPROT_AVLP_MASK 0x20u �<SMC_PMPROT_AVLP_SHIFT 5 �<SMC_PMCTRL_STOPM_MASK 0x7u �<SMC_PMCTRL_STOPM_SHIFT 0 �<SMC_PMCTRL_STOPM(x) (((uint8_t)(((uint8_t)(x))<<SMC_PMCTRL_STOPM_SHIFT))&SMC_PMCTRL_STOPM_MASK) �<SMC_PMCTRL_STOPA_MASK 0x8u �<SMC_PMCTRL_STOPA_SHIFT 3 �<SMC_PMCTRL_RUNM_MASK 0x60u �<SMC_PMCTRL_RUNM_SHIFT 5 �<SMC_PMCTRL_RUNM(x) (((uint8_t)(((uint8_t)(x))<<SMC_PMCTRL_RUNM_SHIFT))&SMC_PMCTRL_RUNM_MASK) �<SMC_PMCTRL_LPWUI_MASK 0x80u �<SMC_PMCTRL_LPWUI_SHIFT 7 �<SMC_VLLSCTRL_VLLSM_MASK 0x7u �<SMC_VLLSCTRL_VLLSM_SHIFT 0 �<SMC_VLLSCTRL_VLLSM(x) (((uint8_t)(((uint8_t)(x))<<SMC_VLLSCTRL_VLLSM_SHIFT))&SMC_VLLSCTRL_VLLSM_MASK) �<SMC_VLLSCTRL_RAM2PO_MASK 0x10u �<SMC_VLLSCTRL_RAM2PO_SHIFT 4 �<SMC_PMSTAT_PMSTAT_MASK 0x7Fu �<SMC_PMSTAT_PMSTAT_SHIFT 0 �<SMC_PMSTAT_PMSTAT(x) (((uint8_t)(((uint8_t)(x))<<SMC_PMSTAT_PMSTAT_SHIFT))&SMC_PMSTAT_PMSTAT_MASK) �<SMC_BASE (0x4007E000u) �<SMC ((SMC_Type *)SMC_BASE) �<SMC_BASES { SMC } �<SPI_MCR_HALT_MASK 0x1u �<SPI_MCR_HALT_SHIFT 0 �<SPI_MCR_SMPL_PT_MASK 0x300u �<SPI_MCR_SMPL_PT_SHIFT 8 �<SPI_MCR_SMPL_PT(x) (((uint32_t)(((uint32_t)(x))<<SPI_MCR_SMPL_PT_SHIFT))&SPI_MCR_SMPL_PT_MASK) �<SPI_MCR_CLR_RXF_MASK 0x400u �<SPI_MCR_CLR_RXF_SHIFT 10 �<SPI_MCR_CLR_TXF_MASK 0x800u �<SPI_MCR_CLR_TXF_SHIFT 11 �<SPI_MCR_DIS_RXF_MASK 0x1000u �<SPI_MCR_DIS_RXF_SHIFT 12 �<SPI_MCR_DIS_TXF_MASK 0x2000u �<SPI_MCR_DIS_TXF_SHIFT 13 �<SPI_MCR_MDIS_MASK 0x4000u �<SPI_MCR_MDIS_SHIFT 14 �<SPI_MCR_DOZE_MASK 0x8000u �<SPI_MCR_DOZE_SHIFT 15 �<SPI_MCR_PCSIS_MASK 0x1F0000u �<SPI_MCR_PCSIS_SHIFT 16 �<SPI_MCR_PCSIS(x) (((uint32_t)(((uint32_t)(x))<<SPI_MCR_PCSIS_SHIFT))&SPI_MCR_PCSIS_MASK) �<SPI_MCR_ROOE_MASK 0x1000000u �<SPI_MCR_ROOE_SHIFT 24 �<SPI_MCR_PCSSE_MASK 0x2000000u �<SPI_MCR_PCSSE_SHIFT 25 �<SPI_MCR_MTFE_MASK 0x4000000u �<SPI_MCR_MTFE_SHIFT 26 �<SPI_MCR_FRZ_MASK 0x8000000u �<SPI_MCR_FRZ_SHIFT 27 �<SPI_MCR_DCONF_MASK 0x30000000u �=SPI_MCR_DCONF_SHIFT 28 �=SPI_MCR_DCONF(x) (((uint32_t)(((uint32_t)(x))<<SPI_MCR_DCONF_SHIFT))&SPI_MCR_DCONF_MASK) �=SPI_MCR_CONT_SCKE_MASK 0x40000000u �=SPI_MCR_CONT_SCKE_SHIFT 30 �=SPI_MCR_MSTR_MASK 0x80000000u �=SPI_MCR_MSTR_SHIFT 31 �=SPI_TCR_SPI_TCNT_MASK 0xFFFF0000u �=SPI_TCR_SPI_TCNT_SHIFT 16 �=SPI_TCR_SPI_TCNT(x) (((uint32_t)(((uint32_t)(x))<<SPI_TCR_SPI_TCNT_SHIFT))&SPI_TCR_SPI_TCNT_MASK) �=SPI_CTAR_BR_MASK 0xFu �=SPI_CTAR_BR_SHIFT 0 �=SPI_CTAR_BR(x) (((uint32_t)(((uint32_t)(x))<<SPI_CTAR_BR_SHIFT))&SPI_CTAR_BR_MASK) �=SPI_CTAR_DT_MASK 0xF0u �=SPI_CTAR_DT_SHIFT 4 �=SPI_CTAR_DT(x) (((uint32_t)(((uint32_t)(x))<<SPI_CTAR_DT_SHIFT))&SPI_CTAR_DT_MASK) �=SPI_CTAR_ASC_MASK 0xF00u �=SPI_CTAR_ASC_SHIFT 8 �=SPI_CTAR_ASC(x) (((uint32_t)(((uint32_t)(x))<<SPI_CTAR_ASC_SHIFT))&SPI_CTAR_ASC_MASK) �=SPI_CTAR_CSSCK_MASK 0xF000u �=SPI_CTAR_CSSCK_SHIFT 12 �=SPI_CTAR_CSSCK(x) (((uint32_t)(((uint32_t)(x))<<SPI_CTAR_CSSCK_SHIFT))&SPI_CTAR_CSSCK_MASK) �=SPI_CTAR_PBR_MASK 0x30000u �=SPI_CTAR_PBR_SHIFT 16 �=SPI_CTAR_PBR(x) (((uint32_t)(((uint32_t)(x))<<SPI_CTAR_PBR_SHIFT))&SPI_CTAR_PBR_MASK) �=SPI_CTAR_PDT_MASK 0xC0000u �=SPI_CTAR_PDT_SHIFT 18 �=SPI_CTAR_PDT(x) (((uint32_t)(((uint32_t)(x))<<SPI_CTAR_PDT_SHIFT))&SPI_CTAR_PDT_MASK) �=SPI_CTAR_PASC_MASK 0x300000u �=SPI_CTAR_PASC_SHIFT 20 �=SPI_CTAR_PASC(x) (((uint32_t)(((uint32_t)(x))<<SPI_CTAR_PASC_SHIFT))&SPI_CTAR_PASC_MASK) �=SPI_CTAR_PCSSCK_MASK 0xC00000u �=SPI_CTAR_PCSSCK_SHIFT 22 �=SPI_CTAR_PCSSCK(x) (((uint32_t)(((uint32_t)(x))<<SPI_CTAR_PCSSCK_SHIFT))&SPI_CTAR_PCSSCK_MASK) �=SPI_CTAR_LSBFE_MASK 0x1000000u �=SPI_CTAR_LSBFE_SHIFT 24 �=SPI_CTAR_CPHA_MASK 0x2000000u �=SPI_CTAR_CPHA_SHIFT 25 �=SPI_CTAR_CPOL_MASK 0x4000000u �=SPI_CTAR_CPOL_SHIFT 26 �=SPI_CTAR_FMSZ_MASK 0x78000000u �=SPI_CTAR_FMSZ_SHIFT 27 �=SPI_CTAR_FMSZ(x) (((uint32_t)(((uint32_t)(x))<<SPI_CTAR_FMSZ_SHIFT))&SPI_CTAR_FMSZ_MASK) �=SPI_CTAR_DBR_MASK 0x80000000u �=SPI_CTAR_DBR_SHIFT 31 �=SPI_CTAR_SLAVE_CPHA_MASK 0x2000000u �=SPI_CTAR_SLAVE_CPHA_SHIFT 25 �=SPI_CTAR_SLAVE_CPOL_MASK 0x4000000u �=SPI_CTAR_SLAVE_CPOL_SHIFT 26 �=SPI_CTAR_SLAVE_FMSZ_MASK 0xF8000000u �=SPI_CTAR_SLAVE_FMSZ_SHIFT 27 �=SPI_CTAR_SLAVE_FMSZ(x) (((uint32_t)(((uint32_t)(x))<<SPI_CTAR_SLAVE_FMSZ_SHIFT))&SPI_CTAR_SLAVE_FMSZ_MASK) �=SPI_SR_POPNXTPTR_MASK 0xFu �=SPI_SR_POPNXTPTR_SHIFT 0 �=SPI_SR_POPNXTPTR(x) (((uint32_t)(((uint32_t)(x))<<SPI_SR_POPNXTPTR_SHIFT))&SPI_SR_POPNXTPTR_MASK) �=SPI_SR_RXCTR_MASK 0xF0u �=SPI_SR_RXCTR_SHIFT 4 �=SPI_SR_RXCTR(x) (((uint32_t)(((uint32_t)(x))<<SPI_SR_RXCTR_SHIFT))&SPI_SR_RXCTR_MASK) �=SPI_SR_TXNXTPTR_MASK 0xF00u �=SPI_SR_TXNXTPTR_SHIFT 8 �=SPI_SR_TXNXTPTR(x) (((uint32_t)(((uint32_t)(x))<<SPI_SR_TXNXTPTR_SHIFT))&SPI_SR_TXNXTPTR_MASK) �=SPI_SR_TXCTR_MASK 0xF000u �=SPI_SR_TXCTR_SHIFT 12 �=SPI_SR_TXCTR(x) (((uint32_t)(((uint32_t)(x))<<SPI_SR_TXCTR_SHIFT))&SPI_SR_TXCTR_MASK) �=SPI_SR_RFDF_MASK 0x20000u �=SPI_SR_RFDF_SHIFT 17 �=SPI_SR_RFOF_MASK 0x80000u �=SPI_SR_RFOF_SHIFT 19 �=SPI_SR_TFFF_MASK 0x2000000u �=SPI_SR_TFFF_SHIFT 25 �=SPI_SR_TFUF_MASK 0x8000000u �=SPI_SR_TFUF_SHIFT 27 �=SPI_SR_EOQF_MASK 0x10000000u �=SPI_SR_EOQF_SHIFT 28 �=SPI_SR_TXRXS_MASK 0x40000000u �=SPI_SR_TXRXS_SHIFT 30 �=SPI_SR_TCF_MASK 0x80000000u �=SPI_SR_TCF_SHIFT 31 �=SPI_RSER_RFDF_DIRS_MASK 0x10000u �=SPI_RSER_RFDF_DIRS_SHIFT 16 �=SPI_RSER_RFDF_RE_MASK 0x20000u �=SPI_RSER_RFDF_RE_SHIFT 17 �=SPI_RSER_RFOF_RE_MASK 0x80000u �=SPI_RSER_RFOF_RE_SHIFT 19 �=SPI_RSER_TFFF_DIRS_MASK 0x1000000u �=SPI_RSER_TFFF_DIRS_SHIFT 24 �=SPI_RSER_TFFF_RE_MASK 0x2000000u �=SPI_RSER_TFFF_RE_SHIFT 25 �=SPI_RSER_TFUF_RE_MASK 0x8000000u �=SPI_RSER_TFUF_RE_SHIFT 27 �=SPI_RSER_EOQF_RE_MASK 0x10000000u �=SPI_RSER_EOQF_RE_SHIFT 28 �=SPI_RSER_TCF_RE_MASK 0x80000000u �=SPI_RSER_TCF_RE_SHIFT 31 �=SPI_PUSHR_TXDATA_MASK 0xFFFFu �=SPI_PUSHR_TXDATA_SHIFT 0 �=SPI_PUSHR_TXDATA(x) (((uint32_t)(((uint32_t)(x))<<SPI_PUSHR_TXDATA_SHIFT))&SPI_PUSHR_TXDATA_MASK) �=SPI_PUSHR_PCS_MASK 0x3F0000u �=SPI_PUSHR_PCS_SHIFT 16 �=SPI_PUSHR_PCS(x) (((uint32_t)(((uint32_t)(x))<<SPI_PUSHR_PCS_SHIFT))&SPI_PUSHR_PCS_MASK) �=SPI_PUSHR_CTCNT_MASK 0x4000000u �=SPI_PUSHR_CTCNT_SHIFT 26 �=SPI_PUSHR_EOQ_MASK 0x8000000u �=SPI_PUSHR_EOQ_SHIFT 27 �=SPI_PUSHR_CTAS_MASK 0x70000000u �=SPI_PUSHR_CTAS_SHIFT 28 �=SPI_PUSHR_CTAS(x) (((uint32_t)(((uint32_t)(x))<<SPI_PUSHR_CTAS_SHIFT))&SPI_PUSHR_CTAS_MASK) �=SPI_PUSHR_CONT_MASK 0x80000000u �=SPI_PUSHR_CONT_SHIFT 31 �=SPI_PUSHR_SLAVE_TXDATA_MASK 0xFFFFu �=SPI_PUSHR_SLAVE_TXDATA_SHIFT 0 �=SPI_PUSHR_SLAVE_TXDATA(x) (((uint32_t)(((uint32_t)(x))<<SPI_PUSHR_SLAVE_TXDATA_SHIFT))&SPI_PUSHR_SLAVE_TXDATA_MASK) �=SPI_POPR_RXDATA_MASK 0xFFFFFFFFu �=SPI_POPR_RXDATA_SHIFT 0 �=SPI_POPR_RXDATA(x) (((uint32_t)(((uint32_t)(x))<<SPI_POPR_RXDATA_SHIFT))&SPI_POPR_RXDATA_MASK) �=SPI_TXFR0_TXDATA_MASK 0xFFFFu �=SPI_TXFR0_TXDATA_SHIFT 0 �=SPI_TXFR0_TXDATA(x) (((uint32_t)(((uint32_t)(x))<<SPI_TXFR0_TXDATA_SHIFT))&SPI_TXFR0_TXDATA_MASK) �=SPI_TXFR0_TXCMD_TXDATA_MASK 0xFFFF0000u �=SPI_TXFR0_TXCMD_TXDATA_SHIFT 16 �>SPI_TXFR0_TXCMD_TXDATA(x) (((uint32_t)(((uint32_t)(x))<<SPI_TXFR0_TXCMD_TXDATA_SHIFT))&SPI_TXFR0_TXCMD_TXDATA_MASK) �>SPI_TXFR1_TXDATA_MASK 0xFFFFu �>SPI_TXFR1_TXDATA_SHIFT 0 �>SPI_TXFR1_TXDATA(x) (((uint32_t)(((uint32_t)(x))<<SPI_TXFR1_TXDATA_SHIFT))&SPI_TXFR1_TXDATA_MASK) �>SPI_TXFR1_TXCMD_TXDATA_MASK 0xFFFF0000u �>SPI_TXFR1_TXCMD_TXDATA_SHIFT 16 �>SPI_TXFR1_TXCMD_TXDATA(x) (((uint32_t)(((uint32_t)(x))<<SPI_TXFR1_TXCMD_TXDATA_SHIFT))&SPI_TXFR1_TXCMD_TXDATA_MASK) �>SPI_TXFR2_TXDATA_MASK 0xFFFFu �>SPI_TXFR2_TXDATA_SHIFT 0 �>SPI_TXFR2_TXDATA(x) (((uint32_t)(((uint32_t)(x))<<SPI_TXFR2_TXDATA_SHIFT))&SPI_TXFR2_TXDATA_MASK) �>SPI_TXFR2_TXCMD_TXDATA_MASK 0xFFFF0000u �>SPI_TXFR2_TXCMD_TXDATA_SHIFT 16 �>SPI_TXFR2_TXCMD_TXDATA(x) (((uint32_t)(((uint32_t)(x))<<SPI_TXFR2_TXCMD_TXDATA_SHIFT))&SPI_TXFR2_TXCMD_TXDATA_MASK) �>SPI_TXFR3_TXDATA_MASK 0xFFFFu �>SPI_TXFR3_TXDATA_SHIFT 0 �>SPI_TXFR3_TXDATA(x) (((uint32_t)(((uint32_t)(x))<<SPI_TXFR3_TXDATA_SHIFT))&SPI_TXFR3_TXDATA_MASK) �>SPI_TXFR3_TXCMD_TXDATA_MASK 0xFFFF0000u �>SPI_TXFR3_TXCMD_TXDATA_SHIFT 16 �>SPI_TXFR3_TXCMD_TXDATA(x) (((uint32_t)(((uint32_t)(x))<<SPI_TXFR3_TXCMD_TXDATA_SHIFT))&SPI_TXFR3_TXCMD_TXDATA_MASK) �>SPI_RXFR0_RXDATA_MASK 0xFFFFFFFFu �>SPI_RXFR0_RXDATA_SHIFT 0 �>SPI_RXFR0_RXDATA(x) (((uint32_t)(((uint32_t)(x))<<SPI_RXFR0_RXDATA_SHIFT))&SPI_RXFR0_RXDATA_MASK) �>SPI_RXFR1_RXDATA_MASK 0xFFFFFFFFu �>SPI_RXFR1_RXDATA_SHIFT 0 �>SPI_RXFR1_RXDATA(x) (((uint32_t)(((uint32_t)(x))<<SPI_RXFR1_RXDATA_SHIFT))&SPI_RXFR1_RXDATA_MASK) �>SPI_RXFR2_RXDATA_MASK 0xFFFFFFFFu �>SPI_RXFR2_RXDATA_SHIFT 0 �>SPI_RXFR2_RXDATA(x) (((uint32_t)(((uint32_t)(x))<<SPI_RXFR2_RXDATA_SHIFT))&SPI_RXFR2_RXDATA_MASK) �>SPI_RXFR3_RXDATA_MASK 0xFFFFFFFFu �>SPI_RXFR3_RXDATA_SHIFT 0 �>SPI_RXFR3_RXDATA(x) (((uint32_t)(((uint32_t)(x))<<SPI_RXFR3_RXDATA_SHIFT))&SPI_RXFR3_RXDATA_MASK) �>SPI0_BASE (0x4002C000u) �>SPI0 ((SPI_Type *)SPI0_BASE) �>SPI1_BASE (0x4002D000u) �>SPI1 ((SPI_Type *)SPI1_BASE) �>SPI2_BASE (0x400AC000u) �>SPI2 ((SPI_Type *)SPI2_BASE) �>SPI_BASES { SPI0, SPI1, SPI2 } �>TSI_GENCS_STPE_MASK 0x1u �>TSI_GENCS_STPE_SHIFT 0 �>TSI_GENCS_STM_MASK 0x2u �>TSI_GENCS_STM_SHIFT 1 �>TSI_GENCS_ESOR_MASK 0x10u �>TSI_GENCS_ESOR_SHIFT 4 �>TSI_GENCS_ERIE_MASK 0x20u �>TSI_GENCS_ERIE_SHIFT 5 �>TSI_GENCS_TSIIE_MASK 0x40u �>TSI_GENCS_TSIIE_SHIFT 6 �>TSI_GENCS_TSIEN_MASK 0x80u �>TSI_GENCS_TSIEN_SHIFT 7 �>TSI_GENCS_SWTS_MASK 0x100u �>TSI_GENCS_SWTS_SHIFT 8 �>TSI_GENCS_SCNIP_MASK 0x200u �>TSI_GENCS_SCNIP_SHIFT 9 �>TSI_GENCS_OVRF_MASK 0x1000u �>TSI_GENCS_OVRF_SHIFT 12 �>TSI_GENCS_EXTERF_MASK 0x2000u �>TSI_GENCS_EXTERF_SHIFT 13 �>TSI_GENCS_OUTRGF_MASK 0x4000u �>TSI_GENCS_OUTRGF_SHIFT 14 �>TSI_GENCS_EOSF_MASK 0x8000u �>TSI_GENCS_EOSF_SHIFT 15 �>TSI_GENCS_PS_MASK 0x70000u �>TSI_GENCS_PS_SHIFT 16 �?TSI_GENCS_PS(x) (((uint32_t)(((uint32_t)(x))<<TSI_GENCS_PS_SHIFT))&TSI_GENCS_PS_MASK) �?TSI_GENCS_NSCN_MASK 0xF80000u �?TSI_GENCS_NSCN_SHIFT 19 �?TSI_GENCS_NSCN(x) (((uint32_t)(((uint32_t)(x))<<TSI_GENCS_NSCN_SHIFT))&TSI_GENCS_NSCN_MASK) �?TSI_GENCS_LPSCNITV_MASK 0xF000000u �?TSI_GENCS_LPSCNITV_SHIFT 24 �?TSI_GENCS_LPSCNITV(x) (((uint32_t)(((uint32_t)(x))<<TSI_GENCS_LPSCNITV_SHIFT))&TSI_GENCS_LPSCNITV_MASK) �?TSI_GENCS_LPCLKS_MASK 0x10000000u �?TSI_GENCS_LPCLKS_SHIFT 28 �?TSI_SCANC_AMPSC_MASK 0x7u �?TSI_SCANC_AMPSC_SHIFT 0 �?TSI_SCANC_AMPSC(x) (((uint32_t)(((uint32_t)(x))<<TSI_SCANC_AMPSC_SHIFT))&TSI_SCANC_AMPSC_MASK) �?TSI_SCANC_AMCLKS_MASK 0x18u �?TSI_SCANC_AMCLKS_SHIFT 3 �?TSI_SCANC_AMCLKS(x) (((uint32_t)(((uint32_t)(x))<<TSI_SCANC_AMCLKS_SHIFT))&TSI_SCANC_AMCLKS_MASK) �?TSI_SCANC_SMOD_MASK 0xFF00u �?TSI_SCANC_SMOD_SHIFT 8 �?TSI_SCANC_SMOD(x) (((uint32_t)(((uint32_t)(x))<<TSI_SCANC_SMOD_SHIFT))&TSI_SCANC_SMOD_MASK) �?TSI_SCANC_EXTCHRG_MASK 0xF0000u �?TSI_SCANC_EXTCHRG_SHIFT 16 �?TSI_SCANC_EXTCHRG(x) (((uint32_t)(((uint32_t)(x))<<TSI_SCANC_EXTCHRG_SHIFT))&TSI_SCANC_EXTCHRG_MASK) �?TSI_SCANC_REFCHRG_MASK 0xF000000u �?TSI_SCANC_REFCHRG_SHIFT 24 �?TSI_SCANC_REFCHRG(x) (((uint32_t)(((uint32_t)(x))<<TSI_SCANC_REFCHRG_SHIFT))&TSI_SCANC_REFCHRG_MASK) �?TSI_PEN_PEN0_MASK 0x1u �?TSI_PEN_PEN0_SHIFT 0 �?TSI_PEN_PEN1_MASK 0x2u �?TSI_PEN_PEN1_SHIFT 1 �?TSI_PEN_PEN2_MASK 0x4u �?TSI_PEN_PEN2_SHIFT 2 �?TSI_PEN_PEN3_MASK 0x8u �?TSI_PEN_PEN3_SHIFT 3 �?TSI_PEN_PEN4_MASK 0x10u �?TSI_PEN_PEN4_SHIFT 4 �?TSI_PEN_PEN5_MASK 0x20u �?TSI_PEN_PEN5_SHIFT 5 �?TSI_PEN_PEN6_MASK 0x40u �?TSI_PEN_PEN6_SHIFT 6 �?TSI_PEN_PEN7_MASK 0x80u �?TSI_PEN_PEN7_SHIFT 7 �?TSI_PEN_PEN8_MASK 0x100u �?TSI_PEN_PEN8_SHIFT 8 �?TSI_PEN_PEN9_MASK 0x200u �?TSI_PEN_PEN9_SHIFT 9 �?TSI_PEN_PEN10_MASK 0x400u �?TSI_PEN_PEN10_SHIFT 10 �?TSI_PEN_PEN11_MASK 0x800u �?TSI_PEN_PEN11_SHIFT 11 �?TSI_PEN_PEN12_MASK 0x1000u �?TSI_PEN_PEN12_SHIFT 12 �?TSI_PEN_PEN13_MASK 0x2000u �?TSI_PEN_PEN13_SHIFT 13 �?TSI_PEN_PEN14_MASK 0x4000u �?TSI_PEN_PEN14_SHIFT 14 �?TSI_PEN_PEN15_MASK 0x8000u �?TSI_PEN_PEN15_SHIFT 15 �?TSI_PEN_LPSP_MASK 0xF0000u �?TSI_PEN_LPSP_SHIFT 16 �?TSI_PEN_LPSP(x) (((uint32_t)(((uint32_t)(x))<<TSI_PEN_LPSP_SHIFT))&TSI_PEN_LPSP_MASK) �?TSI_WUCNTR_WUCNT_MASK 0xFFFFu �?TSI_WUCNTR_WUCNT_SHIFT 0 �?TSI_WUCNTR_WUCNT(x) (((uint32_t)(((uint32_t)(x))<<TSI_WUCNTR_WUCNT_SHIFT))&TSI_WUCNTR_WUCNT_MASK) �?TSI_CNTR1_CTN1_MASK 0xFFFFu �?TSI_CNTR1_CTN1_SHIFT 0 �?TSI_CNTR1_CTN1(x) (((uint32_t)(((uint32_t)(x))<<TSI_CNTR1_CTN1_SHIFT))&TSI_CNTR1_CTN1_MASK) �?TSI_CNTR1_CTN_MASK 0xFFFF0000u �?TSI_CNTR1_CTN_SHIFT 16 �?TSI_CNTR1_CTN(x) (((uint32_t)(((uint32_t)(x))<<TSI_CNTR1_CTN_SHIFT))&TSI_CNTR1_CTN_MASK) �?TSI_CNTR3_CTN1_MASK 0xFFFFu �?TSI_CNTR3_CTN1_SHIFT 0 �?TSI_CNTR3_CTN1(x) (((uint32_t)(((uint32_t)(x))<<TSI_CNTR3_CTN1_SHIFT))&TSI_CNTR3_CTN1_MASK) �?TSI_CNTR3_CTN_MASK 0xFFFF0000u �?TSI_CNTR3_CTN_SHIFT 16 �?TSI_CNTR3_CTN(x) (((uint32_t)(((uint32_t)(x))<<TSI_CNTR3_CTN_SHIFT))&TSI_CNTR3_CTN_MASK) �?TSI_CNTR5_CTN1_MASK 0xFFFFu �?TSI_CNTR5_CTN1_SHIFT 0 �?TSI_CNTR5_CTN1(x) (((uint32_t)(((uint32_t)(x))<<TSI_CNTR5_CTN1_SHIFT))&TSI_CNTR5_CTN1_MASK) �?TSI_CNTR5_CTN_MASK 0xFFFF0000u �?TSI_CNTR5_CTN_SHIFT 16 �?TSI_CNTR5_CTN(x) (((uint32_t)(((uint32_t)(x))<<TSI_CNTR5_CTN_SHIFT))&TSI_CNTR5_CTN_MASK) �?TSI_CNTR7_CTN1_MASK 0xFFFFu �?TSI_CNTR7_CTN1_SHIFT 0 �?TSI_CNTR7_CTN1(x) (((uint32_t)(((uint32_t)(x))<<TSI_CNTR7_CTN1_SHIFT))&TSI_CNTR7_CTN1_MASK) �?TSI_CNTR7_CTN_MASK 0xFFFF0000u �?TSI_CNTR7_CTN_SHIFT 16 �?TSI_CNTR7_CTN(x) (((uint32_t)(((uint32_t)(x))<<TSI_CNTR7_CTN_SHIFT))&TSI_CNTR7_CTN_MASK) �?TSI_CNTR9_CTN1_MASK 0xFFFFu �?TSI_CNTR9_CTN1_SHIFT 0 �?TSI_CNTR9_CTN1(x) (((uint32_t)(((uint32_t)(x))<<TSI_CNTR9_CTN1_SHIFT))&TSI_CNTR9_CTN1_MASK) �?TSI_CNTR9_CTN_MASK 0xFFFF0000u �?TSI_CNTR9_CTN_SHIFT 16 �?TSI_CNTR9_CTN(x) (((uint32_t)(((uint32_t)(x))<<TSI_CNTR9_CTN_SHIFT))&TSI_CNTR9_CTN_MASK) �?TSI_CNTR11_CTN1_MASK 0xFFFFu �?TSI_CNTR11_CTN1_SHIFT 0 �?TSI_CNTR11_CTN1(x) (((uint32_t)(((uint32_t)(x))<<TSI_CNTR11_CTN1_SHIFT))&TSI_CNTR11_CTN1_MASK) �?TSI_CNTR11_CTN_MASK 0xFFFF0000u �?TSI_CNTR11_CTN_SHIFT 16 �?TSI_CNTR11_CTN(x) (((uint32_t)(((uint32_t)(x))<<TSI_CNTR11_CTN_SHIFT))&TSI_CNTR11_CTN_MASK) �?TSI_CNTR13_CTN1_MASK 0xFFFFu �?TSI_CNTR13_CTN1_SHIFT 0 �?TSI_CNTR13_CTN1(x) (((uint32_t)(((uint32_t)(x))<<TSI_CNTR13_CTN1_SHIFT))&TSI_CNTR13_CTN1_MASK) �?TSI_CNTR13_CTN_MASK 0xFFFF0000u �?TSI_CNTR13_CTN_SHIFT 16 �?TSI_CNTR13_CTN(x) (((uint32_t)(((uint32_t)(x))<<TSI_CNTR13_CTN_SHIFT))&TSI_CNTR13_CTN_MASK) �?TSI_CNTR15_CTN1_MASK 0xFFFFu �?TSI_CNTR15_CTN1_SHIFT 0 �?TSI_CNTR15_CTN1(x) (((uint32_t)(((uint32_t)(x))<<TSI_CNTR15_CTN1_SHIFT))&TSI_CNTR15_CTN1_MASK) �?TSI_CNTR15_CTN_MASK 0xFFFF0000u �?TSI_CNTR15_CTN_SHIFT 16 �?TSI_CNTR15_CTN(x) (((uint32_t)(((uint32_t)(x))<<TSI_CNTR15_CTN_SHIFT))&TSI_CNTR15_CTN_MASK) �?TSI_THRESHOLD_HTHH_MASK 0xFFFFu �?TSI_THRESHOLD_HTHH_SHIFT 0 �?TSI_THRESHOLD_HTHH(x) (((uint32_t)(((uint32_t)(x))<<TSI_THRESHOLD_HTHH_SHIFT))&TSI_THRESHOLD_HTHH_MASK) �?TSI_THRESHOLD_LTHH_MASK 0xFFFF0000u �?TSI_THRESHOLD_LTHH_SHIFT 16 �?TSI_THRESHOLD_LTHH(x) (((uint32_t)(((uint32_t)(x))<<TSI_THRESHOLD_LTHH_SHIFT))&TSI_THRESHOLD_LTHH_MASK) �@TSI0_BASE (0x40045000u) �@TSI0 ((TSI_Type *)TSI0_BASE) �@TSI_BASES { TSI0 } �@UART_BDH_SBR_MASK 0x1Fu �@UART_BDH_SBR_SHIFT 0 �@UART_BDH_SBR(x) (((uint8_t)(((uint8_t)(x))<<UART_BDH_SBR_SHIFT))&UART_BDH_SBR_MASK) �@UART_BDH_RXEDGIE_MASK 0x40u �@UART_BDH_RXEDGIE_SHIFT 6 �@UART_BDH_LBKDIE_MASK 0x80u �@UART_BDH_LBKDIE_SHIFT 7 �@UART_BDL_SBR_MASK 0xFFu �@UART_BDL_SBR_SHIFT 0 �@UART_BDL_SBR(x) (((uint8_t)(((uint8_t)(x))<<UART_BDL_SBR_SHIFT))&UART_BDL_SBR_MASK) �@UART_C1_PT_MASK 0x1u �@UART_C1_PT_SHIFT 0 �@UART_C1_PE_MASK 0x2u �@UART_C1_PE_SHIFT 1 �@UART_C1_ILT_MASK 0x4u �@UART_C1_ILT_SHIFT 2 �@UART_C1_WAKE_MASK 0x8u �@UART_C1_WAKE_SHIFT 3 �@UART_C1_M_MASK 0x10u �@UART_C1_M_SHIFT 4 �@UART_C1_RSRC_MASK 0x20u �@UART_C1_RSRC_SHIFT 5 �@UART_C1_UARTSWAI_MASK 0x40u �@UART_C1_UARTSWAI_SHIFT 6 �@UART_C1_LOOPS_MASK 0x80u �@UART_C1_LOOPS_SHIFT 7 �@UART_C2_SBK_MASK 0x1u �@UART_C2_SBK_SHIFT 0 �@UART_C2_RWU_MASK 0x2u �@UART_C2_RWU_SHIFT 1 �AUART_C2_RE_MASK 0x4u �AUART_C2_RE_SHIFT 2 �AUART_C2_TE_MASK 0x8u �AUART_C2_TE_SHIFT 3 �AUART_C2_ILIE_MASK 0x10u �AUART_C2_ILIE_SHIFT 4 �AUART_C2_RIE_MASK 0x20u �AUART_C2_RIE_SHIFT 5 �AUART_C2_TCIE_MASK 0x40u �AUART_C2_TCIE_SHIFT 6 �AUART_C2_TIE_MASK 0x80u �AUART_C2_TIE_SHIFT 7 �AUART_S1_PF_MASK 0x1u �AUART_S1_PF_SHIFT 0 �AUART_S1_FE_MASK 0x2u �AUART_S1_FE_SHIFT 1 �AUART_S1_NF_MASK 0x4u �AUART_S1_NF_SHIFT 2 �AUART_S1_OR_MASK 0x8u �AUART_S1_OR_SHIFT 3 �AUART_S1_IDLE_MASK 0x10u �AUART_S1_IDLE_SHIFT 4 �AUART_S1_RDRF_MASK 0x20u �AUART_S1_RDRF_SHIFT 5 �AUART_S1_TC_MASK 0x40u �AUART_S1_TC_SHIFT 6 �AUART_S1_TDRE_MASK 0x80u �AUART_S1_TDRE_SHIFT 7 �AUART_S2_RAF_MASK 0x1u �AUART_S2_RAF_SHIFT 0 �AUART_S2_LBKDE_MASK 0x2u �AUART_S2_LBKDE_SHIFT 1 �AUART_S2_BRK13_MASK 0x4u �AUART_S2_BRK13_SHIFT 2 �AUART_S2_RWUID_MASK 0x8u �AUART_S2_RWUID_SHIFT 3 �AUART_S2_RXINV_MASK 0x10u �AUART_S2_RXINV_SHIFT 4 �AUART_S2_MSBF_MASK 0x20u �AUART_S2_MSBF_SHIFT 5 �AUART_S2_RXEDGIF_MASK 0x40u �AUART_S2_RXEDGIF_SHIFT 6 �AUART_S2_LBKDIF_MASK 0x80