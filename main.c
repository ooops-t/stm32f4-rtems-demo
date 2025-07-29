#include <rtems.h>
#include <rtems/shell.h>
#include <rtems/console.h>

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
    .af = STM32F4_GPIO_AF_SYSTEM
  }
};

//STM32F4 Discovery Board, USART3: PD8(USART3_TX), PD9(USART3_RX)
stm32f4_gpio_config usart3_tx_config = STM32F4_PIN_USART3_TX_PD8;
stm32f4_gpio_config usart3_rx_config = STM32F4_PIN_USART3_RX_PD9;

#define LED_INIT() stm32f4_gpio_set_config(&led3config)
#define LED_ON() stm32f4_gpio_set_output(STM32F4_GPIO_PIN(3,12), 1)
#define LED_OFF() stm32f4_gpio_set_output(STM32F4_GPIO_PIN(3,12), 0)

#define USART3_INIT() \
  do { \
    stm32f4_gpio_set_config(&usart3_tx_config); \
    stm32f4_gpio_set_config(&usart3_rx_config); \
  } while (0)


rtems_task Init(rtems_task_argument argument)
{
  /* initialize application */
  rtems_status_code sc;

  /* Real application would call idle loop functionality */
  rtems_interval    one_second;

  one_second = 1 * rtems_clock_get_ticks_per_second();

  LED_INIT();

  USART3_INIT();

  sc = rtems_shell_init(
    "shell",
    RTEMS_MINIMUM_STACK_SIZE * 2,
    100,
    CONSOLE_DEVICE_NAME,
    false,
    true,
    rtems_shell_login_check
  );
  if (sc != RTEMS_SUCCESSFUL)
    printf ("error: starting shell: %s (%d)\n", rtems_status_text (sc), sc);

  while (1) {
    (void) rtems_task_wake_after( one_second );
    LED_OFF();
    (void) rtems_task_wake_after( one_second );
    LED_ON();
  }

  (void) rtems_task_delete( RTEMS_SELF );
}

#define CONFIGURE_MAXIMUM_POSIX_KEYS             16
#define CONFIGURE_MAXIMUM_POSIX_KEY_VALUE_PAIRS  16

#define CONFIGURE_SHELL_COMMANDS_INIT
#define CONFIGURE_SHELL_COMMANDS_ALL
#define CONFIGURE_SHELL_MOUNT_MSDOS
#include <rtems/shellconfig.h>

#define CONFIGURE_APPLICATION_NEEDS_LIBBLOCK
#define CONFIGURE_SWAPOUT_TASK_PRIORITY            10

#define CONFIGURE_APPLICATION_NEEDS_CONSOLE_DRIVER
#define CONFIGURE_APPLICATION_NEEDS_CLOCK_DRIVER
#define CONFIGURE_MAXIMUM_TASKS                    rtems_resource_unlimited (10)
#define CONFIGURE_MAXIMUM_SEMAPHORES               rtems_resource_unlimited (10)
#define CONFIGURE_MAXIMUM_MESSAGE_QUEUES           rtems_resource_unlimited (5)
#define CONFIGURE_MAXIMUM_PARTITIONS               rtems_resource_unlimited (2)
#define CONFIGURE_RTEMS_INIT_TASKS_TABLE

#define CONFIGURE_EXTRA_TASK_STACKS         (6 * RTEMS_MINIMUM_STACK_SIZE)

#define CONFIGURE_INIT
#include <rtems/confdefs.h>
