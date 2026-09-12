#ifndef _DEO_TIMER_HWIO_H_
#define _DEO_TIMER_HWIO_H_

#define GE_GEN_EMU_BASE                                             0xf8000000
#define GE_GEN_EMU_BASE_SIZE                                        0x100000000
#define GE_GEN_EMU_BASE_PHYS                                        0xf8000000

#define GE_GEN_EMU_REGS_REG_BASE                                           (GE_GEN_EMU_BASE      + 0x001e0000)
#define GE_GEN_EMU_REGS_REG_BASE_PHYS                                      (GE_GEN_EMU_BASE_PHYS + 0x001e0000)
#define GE_GEN_EMU_REGS_REG_BASE_OFFS                                      0x001e0000

#define HWIO_GE_GEN_EMU_BTIMER_LSB_REG_ADDR                                (GE_GEN_EMU_REGS_REG_BASE      + 0x00000058)
#define HWIO_GE_GEN_EMU_BTIMER_LSB_REG_PHYS                                (GE_GEN_EMU_REGS_REG_BASE_PHYS + 0x00000058)
#define HWIO_GE_GEN_EMU_BTIMER_LSB_REG_OFFS                                (GE_GEN_EMU_REGS_REG_BASE_OFFS + 0x00000058)
#define HWIO_GE_GEN_EMU_BTIMER_LSB_REG_RMSK                                0xffffffff
#define HWIO_GE_GEN_EMU_BTIMER_LSB_REG_POR                                 0x00000000
#define HWIO_GE_GEN_EMU_BTIMER_LSB_REG_POR_RMSK                            0xffffffff
#define HWIO_GE_GEN_EMU_BTIMER_LSB_REG_ATTR                                       0x1
#define HWIO_GE_GEN_EMU_BTIMER_LSB_REG_IN          \
        in_dword_masked(HWIO_GE_GEN_EMU_BTIMER_LSB_REG_ADDR, HWIO_GE_GEN_EMU_BTIMER_LSB_REG_RMSK)
#define HWIO_GE_GEN_EMU_BTIMER_LSB_REG_INM(m)      \
        in_dword_masked(HWIO_GE_GEN_EMU_BTIMER_LSB_REG_ADDR, m)
#define HWIO_GE_GEN_EMU_BTIMER_LSB_REG_BINARY_LSB_BMSK                     0xffffffff
#define HWIO_GE_GEN_EMU_BTIMER_LSB_REG_BINARY_LSB_SHFT                            0x0

#define HWIO_GE_GEN_EMU_BTIMER_MSB_REG_ADDR                                (GE_GEN_EMU_REGS_REG_BASE      + 0x0000005c)
#define HWIO_GE_GEN_EMU_BTIMER_MSB_REG_PHYS                                (GE_GEN_EMU_REGS_REG_BASE_PHYS + 0x0000005c)
#define HWIO_GE_GEN_EMU_BTIMER_MSB_REG_OFFS                                (GE_GEN_EMU_REGS_REG_BASE_OFFS + 0x0000005c)
#define HWIO_GE_GEN_EMU_BTIMER_MSB_REG_RMSK                                  0xffffff
#define HWIO_GE_GEN_EMU_BTIMER_MSB_REG_POR                                 0x00000000
#define HWIO_GE_GEN_EMU_BTIMER_MSB_REG_POR_RMSK                            0xffffffff
#define HWIO_GE_GEN_EMU_BTIMER_MSB_REG_ATTR                                       0x1
#define HWIO_GE_GEN_EMU_BTIMER_MSB_REG_IN          \
        in_dword_masked(HWIO_GE_GEN_EMU_BTIMER_MSB_REG_ADDR, HWIO_GE_GEN_EMU_BTIMER_MSB_REG_RMSK)
#define HWIO_GE_GEN_EMU_BTIMER_MSB_REG_INM(m)      \
        in_dword_masked(HWIO_GE_GEN_EMU_BTIMER_MSB_REG_ADDR, m)
#define HWIO_GE_GEN_EMU_BTIMER_MSB_REG_BINARY_MSB_BMSK                       0xffffff
#define HWIO_GE_GEN_EMU_BTIMER_MSB_REG_BINARY_MSB_SHFT                            0x0

#endif /* _DEO_TIMER_HWIO_H_ */
