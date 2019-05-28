//
// Created by francisco on 28/05/19.
//

#include <stdint.h>
#include <stdio.h>
#include <lzma.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include "helper_types.h"
#include "InlineASM_thumb.h"
#include "helper_memAlloc.h"
#include "coreConfig.h"

#define THUMB_INTRUCTION_MOVS_REG_LO_I8 (regLoDestination, i8Source) (0x2000 | ((regLoDestination)<<8) | (i8Source))

#define THUMB_OP_MOVW (0xf240)
#define THUMB_OP_MOVT (0xf2c0)

//valid register r0-r7 to be loaded
#define THUMB_PUSH_OP_REGISTER_LIST (loRegisterList) (0xb400 | (loRegisterList))
#define THUMB_PUSH_OP_REGISTER_LIST_LR (loRegisterList) (0xb400 | 0x0100 | (loRegisterList))
#define THUMB_POP_OP_REGISTER_LIST (loRegisterList) (0xb400 | (loRegisterList))
#define THUMB_POP_OP_REGISTER_LIST_PC (loRegisterList) (0xb400 | 0X0100 | (loRegisterList))

#define THUMB_OP_ADD_SP(nWords) (0xb000 | (nWords))
#define THUMB_OP_SUB_SP(nWords) (0xb080 | (nWords))

#define THUMB_OP_REGLO_REGLO_I3(regLoDestination, regLoSource, i3Source) (0x1e00 | ((i3Source) << 6) | (regLoSource) << 3 | (regLoDestination))
#define THUMB_OP_CMP_REGLO_I8(regLo, i8) ( 0x2800 | ((regLo) << 8) | (i8))
#define THUMB_OP_B_N (byteOffset) (0xe000 | (((byteOffset) >> 1) & 0x07ff))


#define UNSIGNED_FIT8(x) (((x) & 0xffffff00) == 0)
#define UNSIGNED_FIT16(x) (((x) & 0xffff0000) == 0)
#define SIGNED_FIT8(x) (((x) & 0xffffff80) == 0) || (((x) & 0xffffff80) == 0xffffff80)
#define SIGNED_FIT9(x) (((x) & 0xffffff00) == 0) || (((x) & 0xffffff00) == 0xffffff00)
#define SIGNED_FIT12(x) (((x) & 0xfffff800) == 0) || (((x) & 0xfffff800) == 0xfffff800)

#define THUMB_OP_BEQ_N(byteOffset) (0xd000 | (((byteOffset) >> 1) & 0x00ff))
#define THUMB_OP_BNE_N(byteOffset) (0xd100 | (((byteOffset) >> 1) & 0x00ff))
#define THUMB_OP_BCS_N(byteOffset) (0xd200 | (((byteOffset) >> 1) & 0x00ff))
#define THUMB_OP_BCC_N(byteOffset) (0xd300 | (((byteOffset) >> 1) & 0x00ff))
#define THUMB_OP_BMI_N(byteOffset) (0xd400 | (((byteOffset) >> 1) & 0x00ff))
#define THUMB_OP_BPL_N(byteOffset) (0xd500 | (((byteOffset) >> 1) & 0x00ff))
#define THUMB_OP_BVS_N(byteOffset) (0xd600 | (((byteOffset) >> 1) & 0x00ff))
#define THUMB_OP_BVC_N(byteOffset) (0xd700 | (((byteOffset) >> 1) & 0x00ff))
#define THUMB_OP_BHI_N(byteOffset) (0xd800 | (((byteOffset) >> 1) & 0x00ff))
#define THUMB_OP_BLS_N(byteOffset) (0xd900 | (((byteOffset) >> 1) & 0x00ff))
#define THUMB_OP_BGE_N(byteOffset) (0xda00 | (((byteOffset) >> 1) & 0x00ff))
#define THUMB_OP_BLT_N(byteOffset) (0xdb00 | (((byteOffset) >> 1) & 0x00ff))
#define THUMB_OP_BGT_N(byteOffset) (0xdc00 | (((byteOffset) >> 1) & 0x00ff))

#define THUMB_OP_STR_TO_SP_OFFSET(regLoDestination, wordOffset) (0x9000 | ((regLoDestination) << 8) | ((wordOffset) & 0x00ff))
#define THUMB_OP_LDR_FROM_SP_OFFSET(regLoDestination, wordOffset) (0x9800 | ((regLoDestination) << 8) | ((wordOffset) & 0x00ff))
#define THUMB_OP_ADD_REG_REG_REG(rlo_dest, rlo_src_a, rlo_src_b) (0x1800 | ((rlo_src_b) << 6) | ((rlo_src_a) << 3) | (rlo_dest))


// this could be wrong, because it should have a range of +/- 16MiB...
#define THUMB_OP_BW_HI(byte_offset) (0xf000 | (((byte_offset) >> 12) & 0x07ff))
#define THUMB_OP_BW_LO(byte_offset) (0xb800 | (((byte_offset) >> 1) & 0x07ff))

// all these bit arithmetics need coverage testing!
#define THUMB_OP_BEQ(byte_offset) (0xd000 | (((byte_offset) >> 1) & 0x00ff))
#define THUMB_OP_BEQW_HI(byte_offset) (0xf000 | (((byte_offset) >> 10) & 0x0400) | (((byte_offset) >> 14) & 0x003f))
#define THUMB_OP_BEQW_LO(byte_offset) (0x8000 | ((byte_offset) & 0x2000) | (((byte_offset) >> 1) & 0x0fff))

#define THUMB_OP_BLX(reg) (0x4780 | ((reg) << 3))
#define THUMB_OP_SVC(arg) (0xdf00 | (arg))
#define THUMB_OP_LDR_FROM_BASE_OFFSET(rlo_dest, rlo_base, word_offset) (0x6800 | (((word_offset) << 6) & 0x07c0) | ((rlo_base) << 3) | (rlo_dest))

#define THUMB_OP_CMP_REG_REG(regLo_a, regLo_b) (0x4280 | ((regLo_b) << 3) | (regLo_a))



struct _asmThumb_t {
    int codeThumbPass;
    unsig_int32 codeOffset;
    unsig_int32 codeSize;
    unsig_byte32 *codeBase; // pointer base code
    unsig_byte32 dataDump[8];
    int maxLabels;
    int *labelsOffset;
    int nlocals;
    unsig_int32 pushListReg;
    unsig_int32 adjStack;
};

void asmThumbRelease(asmThumb_t *Asm, bool freeCode) {
    if (freeCode) {
        releaseMem(Asm->codeBase);
    }

    releaseMem(Asm);
}

void asmThumbPassBegin (asmThumb_t *Asm, int codeThumbPass) {
    Asm->codeThumbPass = codeThumbPass;
    Asm->codeOffset = 0;
    if(codeThumbPass == ARM_ASM_THUMB_P2){
        memset(Asm->labelsOffset, -1, Asm->maxLabels * sizeof(int));
    }
}

void asmThumbPassEnd(asmThumb_t *Asm){
    if(Asm->codeThumbPass == ARM_ASM_THUMB_P2) {
        Asm->codeSize = Asm->codeOffset;
        Asm->codeBase = newMem(unsig_byte32, Asm->codeSize);
        printf("codeSize: %u\n", Asm->codeSize);
    }
}

static unsig_byte32 *asmThumbWriteBytes (asmThumb_t *Asm, int qtyWriteBytes) {
    if (Asm->codeThumbPass < ARM_ASM_THUMB_P3) {
        Asm->codeOffset += qtyWriteBytes;
        return Asm->dataDump;
    } else {
        assert(Asm->codeOffset + qtyWriteBytes <= Asm->codeSize);
        unsig_byte32 *c = Asm->codeBase + Asm->codeOffset;
        Asm->codeOffset += qtyWriteBytes;
        return c;
    }
}



void *asmThumbGetCode (asmThumb_t *Asm){
    return (void *) (((uintMachine_t)Asm->codeBase) | 1);
    // need to set low bit to indicate that it's thumb code

}

unsig_int32 asmThumbCodeSize(asmThumb_t *Asm) {
    return Asm->codeSize;
}

static void asmThumbWrite_OP16(asmThumb_t *Asm, unsig_int32 op) {
    unsig_byte32 *c = asmThumbWriteBytes(Asm, 2);
    c[0] = op;
    c[1] = op >> 8;
}

static void asmThumbWrite_OP32 (asmThumb_t *Asm, unsig_int32 op_1, unsig_int32 op_2) {
    unsig_byte32 *c = asmThumbWriteBytes(Asm, 4);

    c[0] = op_1;
    c[1] = op_1 >> 8;
    c[2] = op_2;
    c[3] = op_2 >> 8;
}


asmThumb_t *new_asmThumb(unsig_int32 maxLabels){
    asmThumb_t *Asm;
        Asm=newMem(asmThumb_t, 1);
        Asm->codeThumbPass = 0;
        Asm->codeOffset =0;
        Asm->codeSize =0;
        Asm->codeBase = NULL;
        Asm->maxLabels = maxLabels;
        Asm->labelsOffset = newMem(int, maxLabels);
        Asm-> nlocals=0;

    return Asm;

}

// Cuanta Memoria reserva en el Stack y que tenga suficiente disponible para todos los locales alineados en un 8bytes boundary
void asmThumbIntrEntry(asmThumb_t *Asm, int nLocals) {

    unsig_int32 loRegisterList;
    unsig_int32 adjStack;

    if(nLocals < 0){
        nLocals = 0;
    }

    switch (nLocals) {
        case 0:
            loRegisterList = 0xf2;
            adjStack = 0;
            break;

        case 1:
            loRegisterList = 0xf2;
            adjStack = 0;
            break;

        case 2:
            loRegisterList = 0xfe;
            adjStack = 0;
            break;

        case 3:
            loRegisterList = 0xfe;
            adjStack = 0;
            break;

        default:
            loRegisterList = 0xfe;
            adjStack = ((nLocals - 3) + 1) & (-1);
            break;
    }

    asmThumbWrite_OP16(Asm, THUMB_PUSH_OP_REGISTER_LIST_LR(loRegisterList));
    if (adjStack > 0) {
        asmThumbWrite_OP16(Asm, THUMB_OP_SUB_SP(adjStack));
    }
    Asm->pushListReg = loRegisterList;
    Asm->adjStack = adjStack;
    Asm->nlocals = nLocals;

}

void asmThumbExit(asmThumb_t *Asm) {
    if(Asm->adjStack > 0) {
        asmThumbWrite_OP16(Asm, THUMB_OP_ADD_SP(Asm->adjStack));

    }
    asmThumbWrite_OP16(Asm, THUMB_POP_OP_REGISTER_LIST_PC(Asm->pushListReg));
}

void asmThumbAssignLabel (asmThumb_t *Asm, int label) {
    assert(label < Asm->maxLabels);
    if(Asm->codeThumbPass == ARM_ASM_THUMB_P2) {
        assert(Asm->labelsOffset[label] == -1);
        Asm->labelsOffset[label] = Asm->codeOffset;
    } else if (Asm->codeThumbPass == ARM_ASM_THUMB_P3){
        assert(Asm->labelsOffset[label] == Asm->codeOffset);

    }
}

static int labelDestination (asmThumb_t *Asm, int label) {
    assert(label < Asm->maxLabels);
    return Asm->labelsOffset[label];
}

// the i8_src value will be zero extended into the r32 register!
void asmThumb_MovsRegLo_i8(asmThumb_t *Asm, unsig_int32 regLoDestination, int i8Source) {
    assert(regLoDestination < REGISTER_R8);
    asmThumbWrite_OP16(Asm, THUMB_INTRUCTION_MOVS_REG_LO_I8(regLoDestination, i8Source));
}

// if loading lo half with movw, the i16 value will be zero extended into the r32 register!
static void asmThumb_MovReg_i16 (asmThumb_t *Asm, unsig_int32 movUp, unsig_int32 regDest, int i16Source) {
    assert(regDest < REGISTER_R15);
    // mov[wt] reg_dest, #i16_src
    asmThumbWrite_OP32(Asm, movUp | ((i16Source >> 1) & 0x0400) | ((i16Source >> 12) & 0xf), ((i16Source << 4) & 0x7000) | (regDest << 8) | (i16Source & 0xff));

}

// the i16_src value will be zero extended into the r32 register!
void asmThumb_MovWreg_i16(asmThumb_t *Asm, unsig_int32 regDest, int i16Source) {
    asmThumb_MovReg_i16(Asm, THUMB_OP_MOVW, regDest, i16Source);
}


// the i16_src value will be zero extended into the r32 register!
void asmThumb_MovTreg_i16 (asmThumb_t *Asm, unsig_int32 regDest, int i16Source) {
    asmThumb_MovReg_i16(Asm, THUMB_OP_MOVT, regDest, i16Source);
}

void asmThumb_MovReg_reg (asmThumb_t *Asm, unsig_int32 regDest, unsig_int32 regSource) {
    unsig_int32 opLo;

    if (regSource < 8) {
        opLo |= regSource << 3;
    } else {
        opLo = 0x40 | ((regSource-8) << 3);
    }
    if (regDest < 8) {
        opLo |= regDest;
    } else {
        opLo |= 0x80 | (regDest - 8);
    }

    //mov regDest, regSource
    asmThumbWrite_OP16(Asm, 0x4600 | opLo);
}

void asmThumbRegLo_rlo_i3(asmThumb_t *Asm, unsig_int32 regLoDestination, unsig_int32 regLoSource, int i3Source) {
    assert(regLoDestination < REGISTER_R8);
    assert(regLoSource < REGISTER_R8);

    asmThumbWrite_OP16(Asm, THUMB_OP_REGLO_REGLO_I3(regLoDestination, regLoSource, i3Source));
}

void asmThumb_cmpRegLo_i8 (asmThumb_t *Asm, unsig_int32 regLo, int i8) {
    asmThumbWrite_OP16(Asm, THUMB_OP_CMP_REGLO_I8(regLo, i8));
}

void asmThumb_Bn (asmThumb_t *Asm, int label) {
    int dest = labelDestination(Asm, label);
    int rel = dest - Asm->codeOffset;
    rel -= 4;

    if(SIGNED_FIT12(rel)) {
        asmThumbWrite_OP16(Asm, THUMB_OP_B_N(rel));
    } else {
        printf("asmThumb_B_n : branch does not fit in 12bits\n");
    }
}

void asmThumb_bgt_n (asmThumb_t *Asm, int label) {
    int dest = labelDestination(Asm, label);
    int rel = dest - Asm->codeOffset;
    rel -= 4; //account for instruction prefectch, PC is 4 bytes ahead of this instruction;
    if(SIGNED_FIT9(rel)) {
        asmThumbWrite_OP16(Asm, THUMB_OP_BGT_N(rel));
    } else {
        printf("asm thumb bgt: branch dont fit in 9bits");
    }
}

void asmThumb_MovReg_i32(asmThumb_t *Asm, unsig_int32 regDest, uintMachine_t i32Source){
    // movw, movt does it in 8 bytes
    // ldr [pc, #], dw does it in 6 bytes, but we might not reach to end of code for dw

    asmThumb_MovReg_i16(Asm, THUMB_OP_MOVW, regDest, i32Source);
    asmThumb_MovReg_i16(Asm, THUMB_OP_MOVT, regDest, i32Source >> 16);

}

void asmThumb_MovReg_i32Optimized(asmThumb_t *Asm, unsig_int32 regDest, int i32){
    if(regDest < 8 && UNSIGNED_FIT8(i32)) {
        asmThumb_MovsRegLo_i8(Asm, regDest, i32);
    } else if(UNSIGNED_FIT16(i32)) {
        asmThumb_MovReg_i16(Asm, THUMB_OP_MOVW, regDest, i32);
    } else {
        asmThumb_MovReg_i32(Asm, regDest, i32);
    }
}


void asmThumb_Mov_localReg(asmThumb_t *Asm, int nLocal, unsig_int32 regLoSource) {
    assert(regLoSource < REGISTER_R8);
    int wordOffset = Asm->nlocals - nLocal -1; //nlocal = number of local
    assert(Asm->codeThumbPass < ARM_ASM_THUMB_P3 || wordOffset >= 0);
    asmThumbWrite_OP16(Asm, THUMB_OP_STR_TO_SP_OFFSET(regLoSource, wordOffset));
}

void asmThumb_MovReg_LocalAddr (asmThumb_t *Asm, unsig_int32 regDest, int nLocal) {
    assert(0);
    //see format 12, load address
    asmThumbWrite_OP16(Asm, 0x0000);
}

void asmThumb_AddReg_RegReg (asmThumb_t *Asm, unsig_int32 regLoDest, unsig_int32 regLoSource_a, unsig_int32 regLoSource_b) {
    asmThumbWrite_OP16(Asm, THUMB_OP_ADD_REG_REG_REG(regLoDest, regLoSource_a, regLoSource_b);
}

void asmThumb_cmpRegReg(asmThumb_t *Asm, unsig_int32 regLo_a, unsig_int32 regLo_b) {
    asmThumbWrite_OP16(Asm, THUMB_OP_CMP_REG_REG(regLo_a, regLo_b));
}

void asmThumb_iteGe (asmThumb_t *Asm) {
    asmThumbWrite_OP16(Asm, 0xbfac);
}


void asmThumb_Blabel (asmThumb_t *Asm, int label) {
    int dest = labelDestination(Asm, label);
    int rel = dest - Asm->codeOffset;
    rel -= 4; // account for instruction prefetch, PC is 4 bytes ahead of this instruction
    if(dest >= 0 && rel <= -4) {
        // is a backwards jump, so we know the size of the jump on the first pass
        // calculate rel assuming 12 bit relative jump
        if(SIGNED_FIT12(rel)) {
            asmThumbWrite_OP16(Asm, THUMB_OP_B_N(rel));

        }else {
            goto lJump; // large Jump
        }
    } else {
        // is a forwards jump, so need to assume it's large
        lJump:
        asmThumbWrite_OP32(Asm, THUMB_OP_BW_HI(rel), THUMB_OP_BW_LO(rel));
    }

}

void asmThumb_CMPReg_bzLabel (asmThumb_t *Asm, unsig_int32 regLo, int label) {
    assert(regLo < REGISTER_R8);

    //Compare reg with 0
    asmThumbWrite_OP16(Asm, THUMB_OP_CMP_REGLO_I8(regLo, 0));

    //branch if equal
    int dest = labelDestination(Asm, label);
    int rel = dest - Asm->codeOffset;
    rel -= 4; // account for instruction prefetch, PC is 4 bytes ahead of this instruction

    // is a backwards jump, so we know the size of the jump on the first pass
    // calculate rel assuming 9 bit relative jump
    if (SIGNED_FIT9(rel)) {
        asmThumbWrite_OP16(Asm, THUMB_OP_BEQ(rel));
    } else {
        goto lJump;

    } else {
        // is a forwards jump, so need to assume it's large
        lJump:
        asmThumbWrite_OP32(Asm, THUMB_OP_BEQW_HI(rel), THUMB_OP_BEQW_LO(rel));
    }

}

void asmThumb_blInd (asmThumb_t *Asm, void *funPtr, unsig_int32 funId, unsig_int32 regTemp) {
    /* TODO make this use less bytes
    uint rlo_base = REG_R3;
    uint rlo_dest = REG_R7;
    uint word_offset = 4;
    asm_thumb_write_op16(as, 0x0000);
    asm_thumb_write_op16(as, 0x6800 | (word_offset << 6) | (rlo_base << 3) | rlo_dest); // ldr rlo_dest, [rlo_base, #offset]
    asm_thumb_write_op16(as, 0x4780 | (REG_R9 << 3)); // blx reg
    */

    if (0) {
        // load ptr to function into register using immediate, then branch
        // not relocatable
        asmThumb_MovReg_i32(Asm, regTemp, (uintMachine_t)funPtr);
        asmThumbWrite_OP16(Asm, THUMB_OP_BLX(regTemp));
    } else if (1) {
        asmThumbWrite_OP16(Asm, THUMB_OP_LDR_FROM_BASE_OFFSET(regTemp, REGISTER_R7, funId));
        asmThumbWrite_OP16(Asm, THUMB_OP_BLX(regTemp));
    } else {
        //use SVC
        asmThumbWrite_OP16(Asm, THUMB_OP_SVC(funId));
    }
}







