shConfig) �FNV_BACKKEY7 NV_BACKKEY7_REG(FTFE_FlashConfig) �FNV_BACKKEY6 NV_BACKKEY6_REG(FTFE_FlashConfig) �FNV_BACKKEY5 NV_BACKKEY5_REG(FTFE_FlashConfig) �FNV_BACKKEY4 NV_BACKKEY4_REG(FTFE_FlashConfig) �FNV_FPROT3 NV_FPROT3_REG(FTFE_FlashConfig) �FNV_FPROT2 NV_FPROT2_REG(FTFE_FlashConfig) �FNV_FPROT1 NV_FPROT1_REG(FTFE_FlashConfig) �FNV_FPROT0 NV_FPROT0_REG(FTFE_FlashConfig) �FNV_FSEC NV_FSEC_REG(FTFE_FlashConfig) �FNV_FOPT NV_FOPT_REG(FTFE_FlashConfig) �FNV_FEPROT NV_FEPROT_REG(FTFE_FlashConfig) �FNV_FDPROT NV_FDPROT_REG(FTFE_FlashConfig) �FOSC_CR_REG(base) ((base)->CR) �FOSC_CR_SC16P_MASK 0x1u �FOSC_CR_SC16P_SHIFT 0 �FOSC_CR_SC8P_MASK 0x2u �FOSC_CR_SC8P_SHIFT 1 �FOSC_CR_SC4P_MASK 0x4u �FOSC_CR_SC4P_SHIFT 2 �FOSC_CR_SC2P_MASK 0x8u �FOSC_CR_SC2P_SHIFT 3 �FOSC_CR_EREFSTEN_MASK 0x20u �FOSC_CR_EREFSTEN_SHIFT 5 �FOSC_CR_ERCLKEN_MASK 0x80u �FOSC_CR_ERCLKEN_SHIFT 7 �GOSC_BASE (0x40065000u) �GOSC ((OSC_Type *)OSC_BASE) �GOSC_BASE_PTR (OSC) �GOSC_BASES { OSC } �GOSC_CR OSC_CR_REG(OSC) �GPDB_SC_REG(base) ((base)->SC) �GPDB_MOD_REG(base) ((base)->MOD) �GPDB_CNT_REG(base) ((base)->CNT) �GPDB_IDLY_REG(base) ((base)->IDLY) �GPDB_C1_REG(base,index) ((base)->CH[index].C1) �GPDB_S_REG(base,index) ((base)->CH[index].S) �GPDB_DLY_REG(base,index,index2) ((base)->CH[index].DLY[index2]) �GPDB_INTC_REG(base,index) ((base)->DAC[index].INTC) �GPDB_INT_REG(base,index) ((base)->DAC[index].INT) �GPDB_POEN_REG(base) ((base)->POEN) �GPDB_PODLY_REG(base,index) ((base)->PODLY[index]) �GPDB_SC_LDOK_MASK 0x1u �GPDB_SC_LDOK_SHIFT 0 �GPDB_SC_CONT_MASK 0x2u �GPDB_SC_CONT_SHIFT 1 �GPDB_SC_MULT_MASK 0xCu �GPDB_SC_MULT_SHIFT 2 �GPDB_SC_MULT(x) (((uint32_t)(((uint32_t)(x))<<PDB_SC_MULT_SHIFT))&PDB_SC_MULT_MASK) �GPDB_SC_PDBIE_MASK 0x20u �GPDB_SC_PDBIE_SHIFT 5 �GPDB_SC_PDBIF_MASK 0x40u �GPDB_SC_PDBIF_SHIFT 6 �GPDB_SC_PDBEN_MASK 0x80u �GPDB_SC_PDBEN_SHIFT 7 �GPDB_SC_TRGSEL_MASK 0