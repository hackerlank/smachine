#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#define error() do { \
                    printf("error: [%s][%d]\n", __func__, __LINE__);  \
                    exit(-1); \
                } while(0)

#define inst_illegal(pinst)  do { \
                                    printf("[%s][%d] illegal instruction [%x]\n", __func__, __LINE__, *((u32*)pinst));  \
                                    exit(-1); \
                                } while(0)


#define MEM_SIZE (10*1024)

#define get_bit(x, bit_index) ((x >> bit_index) & 0x1)

#define R(i)  (cpu.r[i])

#define R0 cpu.r[0]
#define R1 cpu.r[1]
#define SP cpu.r[2]
#define PC cpu.r[3]
#define FLAG (cpu.flag)
#define RINDEX(rx) ((&rx - &cpu.r[0]) / 4)

typedef  unsigned char u8;
typedef  signed   char s8;
typedef  unsigned int  u32;
typedef  signed   int  s32;

u32 cpu_read_mem(u32 addr);
void cpu_write_mem(u32 addr, u32 data);

enum ADDRESS_MODE_E {
    AM_IMM          = 0,
    AM_REG_DIRECT   = 1,
    AM_REG_INDIRECT = 2,
    AM_MAX,
};

enum FLAG_E {
    FG_NEG  = 0,
    FG_ZERO = 1,
    FG_OVFW = 2,
};

enum OP_TYPE_E {
    OP_DATA_TRANSFER = 0,
    OP_STACK         = 1,
    OP_FUNC_CALL     = 2,
    OP_ARITHMETIC    = 3,
    OP_JMP           = 4,
};

enum VIC_TABLE_E {
    RESET_HANDLER = 0x0,
    IRQ_HANDLER   = 0x4,
    EXC_HANDLER   = 0x8,
};

enum SUB_TYPE_E {

    MOV = OP_DATA_TRANSFER << 8 |  0,
    LDR = OP_DATA_TRANSFER << 8 |  1,
    STR = OP_DATA_TRANSFER << 8 |  2,

    PUSH = OP_STACK << 8 | 3,
    POP  = OP_STACK << 8 | 4,

    CALL = OP_FUNC_CALL << 8 |  5,
    RET  = OP_FUNC_CALL << 8 |  6,

    ADD = OP_ARITHMETIC << 8 |  7,
    SUB = OP_ARITHMETIC << 8 |  9,
    DIV = OP_ARITHMETIC << 8 |  8,
    MUL = OP_ARITHMETIC << 8 | 10,
    AND = OP_ARITHMETIC << 8 | 11,
    OR  = OP_ARITHMETIC << 8 | 12,
    XOR = OP_ARITHMETIC << 8 | 13,

    JMP   = OP_JMP << 8 | 14,
    JMPN  = OP_JMP << 8 | 15,
    JMPZ  = OP_JMP << 8 | 16,
    JMPO  = OP_JMP << 8 | 17,
    JMPNN = OP_JMP << 8 | 18,
    JMPNZ = OP_JMP << 8 | 19,
    JMPNO = OP_JMP << 8 | 20,
    
};

struct __instruction__ {
    u32 op_type:  16;
    u32 reserved:  4;
    u32 src2:      2;
    u32 am_src2:   2;
    u32 src1:      2;
    u32 am_src1:   2;
    u32 dst:       2;
    u32 am_dst:    2;
};

struct __cpu__ {
    u32 r[4];
    u32 flag;
};

typedef void (*op_handler)(struct __instruction__ *inst);

struct __instruction_set__ {
    char *desc;
    u32 op_type;
    op_handler hander;
};
