
#include <bsp.h>
#include <bsp/io.h> //Everything we need is in io.h

//STM32F4 Discovery Board, LED4: PORTD, 12
stm32f4_gpio_config led3config =
{
  .fields={
    .pin_first = STM32F4_GPIO_PIN(3, 12),
    .pin_last = STM32F4_GPIO_PIN(3, 12),
    .mode = STM32F4_GPIO_MODE_OUTPUT,
    .otype = STM32F4_GPIO_OTYPE_PUSH_PULL,
    .ospeed = STM32F4_GPIO_OSPEED_2_MHZ,
    .pupd = STM32F4_GPIO_NO_PULL,
    .output = 1,
    .af = 0
  }
};

#define LED_INIT() stm32f4_gpio_set_config(&led3config)
#define LED_ON() stm32f4_gpio_set_output(STM32F4_GPIO_PIN(3,12), 1)
#define LED_OFF() stm32f4_gpio_set_output(STM32F4_GPIO_PIN(3,12), 0)

rtems_task Init(rtems_task_argument argument)
{
  /* initialize application */

  /* Real application would call idle loop functionality */
  rtems_interval    one_second;

  one_second = 1 * rtems_clock_get_ticks_per_second();

  LED_INIT();

  while (1) {
    (void) rtems_task_wake_after( one_second );
    LED_OFF();
    (void) rtems_task_wake_after( one_second );
    LED_ON();
  }

  (void) rtems_task_delete( RTEMS_SELF );
}

#define CONFIGURE_APPLICATION_NEEDS_CONSOLE_DRIVER
#define CONFIGURE_APPLICATION_NEEDS_CLOCK_DRIVER
#define CONFIGURE_MAXIMUM_TASKS             1
#define CONFIGURE_RTEMS_INIT_TASKS_TABLE
#define CONFIGURE_INIT
#include <rtems/confdefs.h>
