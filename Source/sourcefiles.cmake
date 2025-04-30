set(SRCDIR ${CMAKE_CURRENT_LIST_DIR})

set(CPP_SOURCES
    ${SRCDIR}/Startup/startup_stm32f030x6.s
    ${SRCDIR}/Startup/syscalls_stub.c
    ${SRCDIR}/Startup/system_stm32f0xx.c

    ${SRCDIR}/Core/Src/detector.c
    ${SRCDIR}/Core/Src/gpio.c
    ${SRCDIR}/Core/Src/main.c
    ${SRCDIR}/Core/Src/remote.c
    ${SRCDIR}/Core/Src/router.c
    ${SRCDIR}/Core/Src/stm32f0xx_it.c
    ${SRCDIR}/Core/Src/tim.c
    ${SRCDIR}/Core/Src/usart.c
    # ${SRCDIR}/Drivers/STM32F0xx_HAL_Driver/Src/Legacy/stm32f0xx_hal_can.c
    # ${SRCDIR}/Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal.c
    # ${SRCDIR}/Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_adc.c
    # ${SRCDIR}/Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_adc_ex.c
    # ${SRCDIR}/Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_can.c
    # ${SRCDIR}/Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_cec.c
    # ${SRCDIR}/Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_comp.c
    # ${SRCDIR}/Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_cortex.c
    # ${SRCDIR}/Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_crc.c
    # ${SRCDIR}/Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_crc_ex.c
    # ${SRCDIR}/Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_dac.c
    # ${SRCDIR}/Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_dac_ex.c
    # ${SRCDIR}/Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_dma.c
    # ${SRCDIR}/Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_exti.c
    # ${SRCDIR}/Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_flash.c
    # ${SRCDIR}/Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_flash_ex.c
    # ${SRCDIR}/Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_gpio.c
    # ${SRCDIR}/Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_i2c.c
    # ${SRCDIR}/Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_i2c_ex.c
    # ${SRCDIR}/Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_i2s.c
    # ${SRCDIR}/Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_irda.c
    # ${SRCDIR}/Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_iwdg.c
    # ${SRCDIR}/Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_msp_template.c
    # ${SRCDIR}/Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_pcd.c
    # ${SRCDIR}/Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_pcd_ex.c
    # ${SRCDIR}/Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_pwr.c
    # ${SRCDIR}/Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_pwr_ex.c
    # ${SRCDIR}/Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_rcc.c
    # ${SRCDIR}/Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_rcc_ex.c
    # ${SRCDIR}/Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_rtc.c
    # ${SRCDIR}/Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_rtc_ex.c
    # ${SRCDIR}/Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_smartcard.c
    # ${SRCDIR}/Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_smartcard_ex.c
    # ${SRCDIR}/Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_smbus.c
    # ${SRCDIR}/Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_spi.c
    # ${SRCDIR}/Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_spi_ex.c
    # ${SRCDIR}/Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_tim.c
    # ${SRCDIR}/Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_tim_ex.c
    # ${SRCDIR}/Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_timebase_rtc_alarm_template.c
    # ${SRCDIR}/Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_timebase_rtc_wakeup_template.c
    # ${SRCDIR}/Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_timebase_tim_template.c
    # ${SRCDIR}/Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_tsc.c
    # ${SRCDIR}/Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_uart.c
    # ${SRCDIR}/Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_uart_ex.c
    # ${SRCDIR}/Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_usart.c
    # ${SRCDIR}/Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_usart_ex.c
    # ${SRCDIR}/Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_wwdg.c
    ${SRCDIR}/Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_ll_adc.c
    ${SRCDIR}/Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_ll_comp.c
    ${SRCDIR}/Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_ll_crc.c
    ${SRCDIR}/Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_ll_crs.c
    ${SRCDIR}/Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_ll_dac.c
    ${SRCDIR}/Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_ll_dma.c
    ${SRCDIR}/Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_ll_exti.c
    ${SRCDIR}/Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_ll_gpio.c
    ${SRCDIR}/Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_ll_i2c.c
    ${SRCDIR}/Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_ll_pwr.c
    ${SRCDIR}/Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_ll_rcc.c
    ${SRCDIR}/Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_ll_rtc.c
    ${SRCDIR}/Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_ll_spi.c
    ${SRCDIR}/Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_ll_tim.c
    ${SRCDIR}/Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_ll_usart.c
    # ${SRCDIR}/Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_ll_usb.c
    ${SRCDIR}/Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_ll_utils.c
)