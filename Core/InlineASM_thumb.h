//
// Created by francisco on 28/05/19.
//


#include "coreConfig.h"

#define REGISTER_R0 (0) // Low registros r0 to r7
#define REGISTER_R1 (1)      // r0-r3 registros de argumento y scratch;
#define REGISTER_R2 (2)      // r0-r1 son tambien registros de resultado
#define REGISTER_R3 (3)
#define REGISTER_R4 (4)      // r4-r8 son registros de guardado de llamada
#define REGISTER_R5 (5)
#define REGISTER_R6 (6)
#define REGISTER_R7 (7)
#define REGISTER_R8 (8) // High Register r8-r12
#define REGISTER_R9 (9) // r9 puede ser registro para salvar llamadas o no (variante del AAPCS o registro especial)
#define REGISTER_R10 (10) // r10-r11 son registros de guardado de llamadas
#define REGISTER_R11 (11)
#define REGISTER_R12 (12) //r12 - r15 son registros especiales
#define REGISTER_R13 (13) // Process stack pointer
#define REGISTER_R14 (14)  // Link register (REGISTER_LR)
#define REGISTER_R15 (15) // Program Counter //pc
#define REGISTER_LR (REGISTER_R14)

// r0-r3 registros de argumentos
#define REGISTER_ARGUMENT_R0 REGISTER_R0
#define REGISTER_ARGUMENT_R1 REGISTER_R1
#define REGISTER_ARGUMENT_R2 REGISTER_R2
#define REGISTER_ARGUMENT_R3 REGISTER_R3

//RET {Xn} REGISTRO http://infocenter.arm.com/help/index.jsp?topic=/com.arm.doc.dui0802a/RET.html
#define REGISTER_RET REGISTER_R0

//Assembler http://www.keil.com/support/man/docs/armasm/armasm_dom1359731122720.htm
#define ARM_ASM_THUMB_P1 (1) //ARM assembler pass check y determina : sintaxis, tamano, offsets, simbolos
#define ARM_ASM_THUMB_P2 (2) //Pass Fallas, codifica offsets encontrados en el paso 1(donde esten disponibles)
#define ARM_ASM_THUMB_P3 (3)

typedef struct _asmThumb_t asmThumb_t;
asmThumb_t *new_asmThumb (unsig_int32 maxLabels);

void asmThumbRelease(asmThumb_t *Asm, bool freeCode);
void asmThumbPassBegin (asmThumb_t *Asm, int codeThumbPass)
void asmThumbPassEnd(asmThumb_t *Asm);
unsig_int32 asmThumbCodeSize(asmThumb_t *Asm);
void *asmThumbGetCode (asmThumb_t *Asm);
void asmThumbIntrEntry(asmThumb_t *Asm, int nLocals);
void asmThumbExit(asmThumb_t *Asm);
void asmThumbAssignLabel(asmThumb_t *Asm, int label);


// argument order follows ARM, in general dest is first
// note there is a difference between movw and mov.w, and many others!

void asmThumb_MovsRegLo_i8(asmThumb_t *Asm, unsig_int32 regLoDestination, int i8Source);
void asmThumb_MovWreg_i16(asmThumb_t *Asm, unsig_int32 regDest, int i16Source);
void asmThumb_MovTreg_i16 (asmThumb_t *Asm, unsig_int32 regDest, int i16Source);
void asmThumb_MovReg_reg (asmThumb_t *Asm, unsig_int32 regDest, unsig_int32 regSource);
void asmThumbRegLo_rlo_i3(asmThumb_t *Asm, unsig_int32 regLoDestination, unsig_int32 regLoSource, int i3Source);
void asmThumb_cmpRegLo_i8 (asmThumb_t *Asm, unsig_int32 regLo, int i8);
void asmThumb_Bn (asmThumb_t *Asm, int label);
void asmThumb_bgt_n (asmThumb_t *Asm, int label);

void asmThumb_MovReg_i32(asmThumb_t *Asm, unsig_int32 regDest, uintMachine_t i32Source);
void asmThumb_MovReg_i32Optimized(asmThumb_t *Asm, unsig_int32 regDest, int i32);
void asmThumb_Mov_localReg(asmThumb_t *Asm, int nLocal, unsig_int32 regLoSource);
void asmThumb_Mov_localReg(asmThumb_t *Asm, int nLocal, unsig_int32 regLoSource);
void asmThumb_MovReg_LocalAddr (asmThumb_t *Asm, unsig_int32 regDest, int nLocal);


void asmThumb_AddReg_RegReg (asmThumb_t *Asm, unsig_int32 regLoDest, unsig_int32 regLoSource_a, unsig_int32 regLoSource_b);
void asmThumb_cmpRegReg(asmThumb_t *Asm, unsig_int32 regLo_a, unsig_int32 regLo_b);
void asmThumb_iteGe(asmThumb_t *Asm);

void asmThumb_Blabel (asmThumb_t *Asm, int label);
void asmThumb_CMPReg_bzLabel (asmThumb_t *Asm, unsig_int32 regLo, int label);
void asmThumb_blInd (asmThumb_t *Asm, void *funPtr, unsig_int32 funId, unsig_int32 regTemp);
