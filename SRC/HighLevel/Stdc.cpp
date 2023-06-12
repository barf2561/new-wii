// Std C runtime
#include "pch.h"

#define PARAM(n)    Core->regs.gpr[3+n]
#define RET_VAL     Core->regs.gpr[3]
#define SWAP        _BYTESWAP_UINT32
#define FPRD(n)     Core->regs.fpr[n].dbl

// fast longlong swap, invented by org
static void swap_double(void* srcPtr)
{
    uint8_t* src = (uint8_t*)srcPtr;
    uint8_t t;

    for (int i = 0; i < 4; i++)
    {
        t = src[7 - i];
        src[7 - i] = src[i];
        src[i] = t;
    }
}

/* ---------------------------------------------------------------------------
    Memory operations
--------------------------------------------------------------------------- */

// void *memcpy( void *dest, const void *src, size_t count�);
void HLE_memcpy()
{
    int WIMG;
    uint32_t eaDest = PARAM(0), eaSrc = PARAM(1), cnt = PARAM(2);
    uint32_t paDest = Core->EffectiveToPhysical(eaDest, Gekko::MmuAccess::Read, WIMG);
    uint32_t paSrc = Core->EffectiveToPhysical(eaSrc, Gekko::MmuAccess::Read, WIMG);

    assert( paDest != Gekko::BadAddress);
    assert( paSrc != Gekko::BadAddress);
    assert( (paDest + cnt) < RAMSIZE);
    assert( (paSrc + cnt) < RAMSIZE);

//  DBReport( GREEN "memcpy(0x%08X, 0x%08X, %i(%s))\n", 
//            eaDest, eaSrc, cnt, FileSmartSize(cnt) );

    memcpy(&mi.ram[paDest], &mi.ram[paSrc], cnt);
}

// void *memset( void *dest, int c, size_t count�);
void HLE_memset()
{
    int WIMG;
    uint32_t eaDest = PARAM(0), c = PARAM(1), cnt = PARAM(2);
    uint32_t paDest = Core->EffectiveToPhysical(eaDest, Gekko::MmuAccess::Read, WIMG);

    assert(paDest != Gekko::BadAddress);
    assert( (paDest + cnt) < RAMSIZE);

//  DBReport( GREEN "memcpy(0x%08X, %i(%c), %i(%s))\n", 
//            eaDest, c, cnt, FileSmartSize(cnt) );

    memset(&mi.ram[paDest], c, cnt);
}

/* ---------------------------------------------------------------------------
    String operations
--------------------------------------------------------------------------- */


/* ---------------------------------------------------------------------------
    FP Math
--------------------------------------------------------------------------- */

// double sin(double x)
void HLE_sin()
{
    FPRD(1) = sin(FPRD(1));
}

// double cos(double x)
void HLE_cos()
{
    FPRD(1) = cos(FPRD(1));
}

// double modf(double x, double * intptr)
void HLE_modf()
{
    int WIMG;
    double * intptr = (double *)(&mi.ram[Core->EffectiveToPhysical(PARAM(0), Gekko::MmuAccess::Read, WIMG)]);
    
    FPRD(1) = modf(FPRD(1), intptr);
    swap_double(intptr);
}

// double frexp(double x, int * expptr)
void HLE_frexp()
{
    int WIMG;
    uint32_t * expptr = (uint32_t *)(&mi.ram[Core->EffectiveToPhysical(PARAM(0), Gekko::MmuAccess::Read, WIMG)]);
    
    FPRD(1) = frexp(FPRD(1), (int *)expptr);
    *expptr = SWAP(*expptr);
}

// double ldexp(double x, int exp)
void HLE_ldexp()
{
    FPRD(1) = ldexp(FPRD(1), (int)PARAM(0));
}

// double floor(double x)
void HLE_floor()
{
    FPRD(1) = floor(FPRD(1));
}

// double ceil(double x)
void HLE_ceil()
{
    FPRD(1) = ceil(FPRD(1));
}
