


ARM Macro Assembler    Page 1 


    1 00000000         ;/******************************************************
                       ***********************
    2 00000000         ; * @file:    startup_MK64F12.s
    3 00000000         ; * @purpose: CMSIS Cortex-M4 Core Device Startup File f
                       or the
    4 00000000         ; *           MK64F12
    5 00000000         ; * @version: 1.0
    6 00000000         ; * @date:    2013-8-12
    7 00000000         ; *
    8 00000000         ; * Copyright: 1997 - 2013 Freescale Semiconductor, Inc.
                        All Rights Reserved.
    9 00000000         ;*
   10 00000000         ; *------- <<< Use Configuration Wizard in Context Menu 
                       >>> ------------------
   11 00000000         ; *
   12 00000000         ; ******************************************************
                       ***********************/
   13 00000000         
   14 00000000         
   15 00000000         ; <h> Stack Configuration
   16 00000000         ;   <o> Stack Size (in Bytes) <0x0-0xFFFFFFFF:8>
   17 00000000         ; </h>
   18 00000000         
   19 00000000 00000800 
                       Stack_Size
                               EQU              0x00000800
   20 00000000         
   21 00000000                 AREA             STACK, NOINIT, READWRITE, ALIGN
=3
   22 00000000         Stack_Mem
                               SPACE            Stack_Size
   23 00000800         __initial_sp
   24 00000800         
   25 00000800         
   26 00000800         ; <h> Heap Configuration
   27 00000800         ;   <o>  Heap Size (in Bytes) <0x0-0xFFFFFFFF:8>
   28 00000800         ; </h>
   29 00000800         
   30 00000800 00000400 
                       Heap_Size
                               EQU              0x00000400
   31 00000800         
   32 00000800                 AREA             HEAP, NOINIT, READWRITE, ALIGN=
3
   33 00000000         __heap_base
   34 00000000         Heap_Mem
                               SPACE            Heap_Size
   35 00000400         __heap_limit
   36 00000400         
   37 00000400         
   38 00000400                 PRESERVE8
   39 00000400            