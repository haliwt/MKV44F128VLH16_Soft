.\mk60d10\flash\pdb.o: ..\..\..\..\Libraries\drivers\K\src\pdb.c
.\mk60d10\flash\pdb.o: ..\..\..\..\Libraries\drivers\K\inc\pdb.h
.\mk60d10\flash\pdb.o: ..\..\..\..\Libraries\drivers\K\inc\common.h
.\mk60d10\flash\pdb.o: C:\Keil_v5\ARM\ARMCC\bin\..\include\stdint.h
.\mk60d10\flash\pdb.o: C:\Keil_v5\ARM\ARMCC\bin\..\include\stdbool.h
.\mk60d10\flash\pdb.o: C:\Keil_v5\ARM\ARMCC\bin\..\include\stddef.h
.\mk60d10\flash\pdb.o: ..\..\..\..\Libraries\startup\DeviceSupport\MK60D10.h
.\mk60d10\flash\pdb.o: ..\..\..\..\Libraries\startup\CoreSupport\core_cm4.h
.\mk60d10\flash\pdb.o: ..\..\..\..\Libraries\startup\CoreSupport\core_cmInstr.h
.\mk60d10\flash\pdb.o: ..\..\..\..\Libraries\startup\CoreSupport\core_cmFunc.h
.\mk60d10\flash\pdb.o: ..\..\..\..\Libraries\startup\CoreSupport\core_cm4_simd.h
.\mk60d10\flash\pdb.o: ..\..\..\..\Libraries\startup\DeviceSupport\system_MK60D10.h
                                                                                                                                    ignment */
        *(sAlignmentFcn->pf16IdReq) = sAlignmentFcn->f16CurrentAlign;
    }
    else
    {
        /* after defined time period set required d-axis current to zero */
        *(sAlignmentFcn->pf16IdReq) = FRAC16(0.0);
        sAlignmentFcn->ui16Active = FALSE;
    }
}