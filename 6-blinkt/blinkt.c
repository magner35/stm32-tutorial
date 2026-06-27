// target bluepillplus STM32F103CBT6 weactstudio

#include <stdint.h>

static void enable_port_clock(void);
static void configure_pin(void);
static void configure_sys_tick_timer(void);

void start(void)
{
    enable_port_clock();
    configure_pin();
    configure_sys_tick_timer();

    for (;;)
    {
    }
}

static void enable_port_clock(void)
{
    // enable I/O port B clock
    uint32_t rcc_base_address = 0x40021000;
    uint32_t rcc_apb2enr_address = rcc_base_address + 0x18;
    uint32_t rcc_apb2enr_iopben = 1 << 3;
    volatile uint32_t *rcc_apb2enr_pointer = (uint32_t *)rcc_apb2enr_address;
    uint32_t rcc_apb2enr_value = *rcc_apb2enr_pointer;
    rcc_apb2enr_value |= rcc_apb2enr_iopben;
    *rcc_apb2enr_pointer = rcc_apb2enr_value;
}

static void configure_pin(void)
{
    // configure PB2 as push-pull output with 2 MHz speed
    uint32_t gpiob_base_address = 0x40010C00;
    uint32_t gpiob_crl_address = gpiob_base_address + 0x00;
    uint32_t gpiox_crl_mode2_0 = 1 << 8;
    uint32_t gpiox_crl_mode2_1 = 1 << 9;
    uint32_t gpiox_crl_cnf2_0 = 1 << 10;
    uint32_t gpiox_crl_cnf2_1 = 1 << 11;

    volatile uint32_t *gpiob_crl_pointer = (uint32_t *)gpiob_crl_address;
    uint32_t gpiob_crl_value = *gpiob_crl_pointer;
    gpiob_crl_value &= ~gpiox_crl_mode2_0;
    gpiob_crl_value |= gpiox_crl_mode2_1;
    gpiob_crl_value &= ~gpiox_crl_cnf2_0;
    gpiob_crl_value &= ~gpiox_crl_cnf2_1;
    *gpiob_crl_pointer = gpiob_crl_value;
}

static void configure_sys_tick_timer(void)
{
    uint32_t stk_base_address = 0xE000E010;
    uint32_t stk_ctrl_address = stk_base_address + 0x00;
    uint32_t stk_ctrl_enable = 1 << 0;
    uint32_t stk_ctrl_tickint = 1 << 1;

    uint32_t stk_load_address = stk_base_address + 0x04;

    // enable systick exception, enable systick counter
    volatile uint32_t *stk_ctrl_pointer = (uint32_t *)stk_ctrl_address;
    uint32_t stk_ctrl_value = *stk_ctrl_pointer;
    stk_ctrl_value |= stk_ctrl_enable;
    stk_ctrl_value |= stk_ctrl_tickint;
    *stk_ctrl_pointer = stk_ctrl_value;

    // set reload value to 500 ms
    volatile uint32_t *stk_load_pointer = (uint32_t *)stk_load_address;
    uint32_t stk_load_value;
    stk_load_value = 8000000 / 8 / 2;
    *stk_load_pointer = stk_load_value;
}

static void toggle_pin(void);

void sys_tick_exception_handler(void)
{
    toggle_pin();
}

static void toggle_pin(void)
{
    // toggle PB2
    uint32_t gpiob_base_address = 0x40010C00;
    uint32_t gpiob_odr_address = gpiob_base_address + 0x0c;
    uint32_t gpiox_odr_odr2 = 1 << 2;

    volatile uint32_t *gpioc_odr_pointer = (uint32_t *)gpiob_odr_address;
    uint32_t gpioc_odr_value = *gpioc_odr_pointer;
    gpioc_odr_value ^= gpiox_odr_odr2;
    *gpioc_odr_pointer = gpioc_odr_value;
}
