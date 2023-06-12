// Flipper Processor Interface (for Gekko)

#pragma once

// Address spaces
#define PI_MEMSPACE_MAINMEM     0x0000'0000         // 1T-SRAM main memory
#define PI_MEMSPACE_EFB         0x0800'0000         // GX eFB base address
#define PI_REGSPACE_CP          0x0C00'0000
#define PI_REGSPACE_PE          0x0C00'1000         // GX Pixel Engine regs mapped to physical memory
#define PI_REGSPACE_VI          0x0C00'2000
#define PI_REGSPACE_PI          0x0C00'3000
#define PI_REGSPACE_MEM         0x0C00'4000
#define PI_REGSPACE_DSP         0x0C00'5000
#define PI_REGSPACE_DI          0x0C00'6000
#define PI_REGSPACE_SI          0x0C00'6400
#define PI_REGSPACE_EXI         0x0C00'6800
#define PI_REGSPACE_AIS         0x0C00'6C00
#define PI_REGSPACE_GX_FIFO     0x0C00'8000         // GX streaming fifo
#define PI_MEMSPACE_BOOTROM     0xFFF0'0000

#define PI_REG8_TO_SPACE(space, id)     (space | ((uint32_t)(id)))
#define PI_REG16_TO_SPACE(space, id)    (space | ((uint32_t)(id) << 1))
#define PI_REG32_TO_SPACE(space, id)    (space | ((uint32_t)(id) << 2))

// Efb Z-plane select
#define PI_EFB_ZPLANE       0x0040'0000 

// Mask EFB address
#define PI_EFB_ADDRESS_MASK 0xFF80'0000

#define PI_INTSR            0x0C003000      // master interrupt reg
#define PI_INTMR            0x0C003004      // master interrupt mask
#define PI_BASE             0x0C00300C      // PI CP fifo base
#define PI_TOP              0x0C003010      // PI CP fifo top
#define PI_WRPTR            0x0C003014      // PI CP fifo write pointer
#define PI_CPABT            0x0C003018      // Abort PI CP FIFO?
#define PI_PIESR            0x0C00301C
#define PI_PIEAR            0x0C003020
#define PI_CONFIG           0x0C003024      // PI CFG + reset bits
#define PI_DURAR            0x0C003028
#define PI_CHIPID           0x0C00302C      // Flipper ID (console revision)
#define PI_STRGTH           0x0C003030
#define PI_CPUDBB           0x0C003034

// PI interrupt regs mask
#define PI_INTERRUPT_HSP        0x2000      // high-speed port
#define PI_INTERRUPT_DEBUG      0x1000      // debug hardware
#define PI_INTERRUPT_CP         0x0800      // command fifo
#define PI_INTERRUPT_PE_FINISH  0x0400      // PE finish command (draw done)
#define PI_INTERRUPT_PE_TOKEN   0x0200      // PE token parsed (draw sync)
#define PI_INTERRUPT_VI         0x0100      // 4 VI line ints
#define PI_INTERRUPT_MEM        0x0080      // memory protection failed
#define PI_INTERRUPT_DSP        0x0040      // various DSP (ARAM, AI FIFO, DSP)
#define PI_INTERRUPT_AI         0x0020      // DVD streaming trigger interrupt
#define PI_INTERRUPT_EXI        0x0010      // EXI transfer complete
#define PI_INTERRUPT_SI         0x0008      // serial interrupts
#define PI_INTERRUPT_DI         0x0004      // DVD cover, break, transfer complete
#define PI_INTERRUPT_RSW        0x0002      // reset "switch"
#define PI_INTERRUPT_PI         0x0001      // Generated when something goes wrong inside Flipper

// PI CONFIG Reset control bits
#define PI_CONFIG_SYSRSTB 0x00000001
#define PI_CONFIG_MEMRSTB 0x00000002
#define PI_CONFIG_DIRSTB 0x00000004

enum class PIInterruptSource
{
    PI,
    RSW,
    DI,
    SI,
    EXI,
    AI,
    DSP,
    MEM,
    VI,
    PE_TOKEN,
    PE_FINISH,
    CP,
    DEBUG,
    HSP,

    Max,
};

// hardware registers base (physical address)
#define HW_BASE         0x0C000000

// max known GC HW address is 0x0C008004 (fifo), so 0x8010 will be enough.
// note : it must not be greater 0xffff, unless you need to change code.
#define HW_MAX_KNOWN    0x8010

void PIReadBurst(uint32_t phys_addr, uint8_t burstData[32]);
void PIWriteBurst(uint32_t phys_addr, uint8_t burstData[32]);

// ---------------------------------------------------------------------------
// hardware API

// PI state (registers and other data)
struct PIControl
{
    volatile uint32_t    intsr;          // interrupt cause
    volatile uint32_t    intmr;          // interrupt mask
    bool        rswhack;        // reset "switch" hack
    bool        log;            // log interrupts
    uint32_t    consoleVer;     // console version
    int64_t     intCounters[(size_t)PIInterruptSource::Max];      // interrupt counters
};

extern  PIControl pi;

void PIAssertInt(uint32_t mask);  // set interrupt(s)
void PIClearInt(uint32_t mask);   // clear interrupt(s)
void PIOpen(HWConfig * config);
void PIClose();

void PISetTrap(
    uint32_t type,                                       // 8, 16 or 32
    uint32_t addr,                                       // physical address of trap
    void (*rdTrap)(uint32_t, uint32_t*) = NULL,  // register read trap
    void (*wrTrap)(uint32_t, uint32_t) = NULL);  // register write trap
