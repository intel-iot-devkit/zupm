cmd_/home/intel_2/Zephyr_dev/hello_world/microkernel/src/main_apa102.o := /opt/zephyr-sdk//sysroots/i686-pokysdk-linux/usr/bin/iamcu-poky-elfiamcu/i586-poky-elfiamcu-gcc -Wp,-MD,/home/intel_2/Zephyr_dev/hello_world/microkernel/src/.main_apa102.o.d  -nostdinc -isystem /opt/zephyr-sdk/sysroots/i686-pokysdk-linux/usr/bin/iamcu-poky-elfiamcu/../../lib/iamcu-poky-elfiamcu/gcc/i586-poky-elfiamcu/5.2.1/include -isystem /opt/zephyr-sdk/sysroots/i686-pokysdk-linux/usr/bin/iamcu-poky-elfiamcu/../../lib/iamcu-poky-elfiamcu/gcc/i586-poky-elfiamcu/5.2.1/include-fixed -I/home/intel_2/zephyr/arch/x86/include -I/home/intel_2/zephyr/arch/x86/soc/quark_se -I/home/intel_2/zephyr/boards/arduino_101  -I/home/intel_2/zephyr/include -I/home/intel_2/zephyr/include -I/home/intel_2/Zephyr_dev/hello_world/microkernel/outdir/include/generated -I/home/intel_2/Zephyr_dev/hello_world/microkernel/outdir/misc/generated/sysgen -include /home/intel_2/Zephyr_dev/hello_world/microkernel/outdir/include/generated/autoconf.h  -I/opt/zephyr-sdk//sysroots/iamcu-poky-elfiamcu/usr/i586-default-elfiamcu/include -I/home/intel_2/zephyr/ext/lib/mraa/include -I/home/intel_2/zephyr/ext/lib/upm/include -I/home/intel_2/zephyr/ext/lib/upm/src/apa102 -I/home/intel_2/zephyr/ext/lib/upm/src/utilities   -I/home/intel_2/Zephyr_dev/hello_world/microkernel/src -DKERNEL -c -g -std=c99 -fno-asynchronous-unwind-tables -fno-omit-frame-pointer -Wall -Wno-format-zero-length -Wno-main -ffreestanding -Os -fno-stack-protector -ffunction-sections -fdata-sections -mpreferred-stack-boundary=2 -mno-sse -march=lakemont -mtune=lakemont -msoft-float -miamcu -Wno-unused-but-set-variable -fno-reorder-functions -fno-defer-pop -Wno-pointer-sign -fno-strict-overflow -Werror=implicit-int    -D"KBUILD_STR(s)=\#s" -D"KBUILD_BASENAME=KBUILD_STR(main_apa102)"  -D"KBUILD_MODNAME=KBUILD_STR(main_apa102)" -c -o /home/intel_2/Zephyr_dev/hello_world/microkernel/src/main_apa102.o /home/intel_2/Zephyr_dev/hello_world/microkernel/src/main_apa102.c

source_/home/intel_2/Zephyr_dev/hello_world/microkernel/src/main_apa102.o := /home/intel_2/Zephyr_dev/hello_world/microkernel/src/main_apa102.c

deps_/home/intel_2/Zephyr_dev/hello_world/microkernel/src/main_apa102.o := \
    $(wildcard include/config/stdout/console.h) \
  /home/intel_2/zephyr/include/zephyr.h \
    $(wildcard include/config/microkernel.h) \
  /home/intel_2/zephyr/include/microkernel.h \
  /home/intel_2/zephyr/include/nanokernel.h \
    $(wildcard include/config/nano/timeouts.h) \
    $(wildcard include/config/debug/tracing/kernel/objects.h) \
    $(wildcard include/config/thread/custom/data.h) \
    $(wildcard include/config/cplusplus.h) \
  /opt/zephyr-sdk/sysroots/i686-pokysdk-linux/usr/lib/iamcu-poky-elfiamcu/gcc/i586-poky-elfiamcu/5.2.1/include/stddef.h \
  /opt/zephyr-sdk//sysroots/iamcu-poky-elfiamcu/usr/i586-default-elfiamcu/include/stdint.h \
  /opt/zephyr-sdk//sysroots/iamcu-poky-elfiamcu/usr/i586-default-elfiamcu/include/machine/_default_types.h \
  /opt/zephyr-sdk//sysroots/iamcu-poky-elfiamcu/usr/i586-default-elfiamcu/include/sys/features.h \
  /opt/zephyr-sdk//sysroots/iamcu-poky-elfiamcu/usr/i586-default-elfiamcu/include/sys/_intsup.h \
  /opt/zephyr-sdk//sysroots/iamcu-poky-elfiamcu/usr/i586-default-elfiamcu/include/sys/_stdint.h \
  /home/intel_2/zephyr/include/toolchain.h \
  /home/intel_2/zephyr/include/toolchain/gcc.h \
    $(wildcard include/config/arm.h) \
    $(wildcard include/config/isa/thumb.h) \
    $(wildcard include/config/isa/thumb2.h) \
    $(wildcard include/config/isa/arm.h) \
    $(wildcard include/config/arc.h) \
    $(wildcard include/config/x86.h) \
    $(wildcard include/config/nios2.h) \
  /home/intel_2/zephyr/include/toolchain/common.h \
  /home/intel_2/zephyr/include/kernel_version.h \
  /home/intel_2/zephyr/include/sys_clock.h \
    $(wildcard include/config/sys/clock/exists.h) \
    $(wildcard include/config/sys/clock/hw/cycles/per/sec.h) \
    $(wildcard include/config/sys/clock/ticks/per/sec.h) \
    $(wildcard include/config/timer/reads/its/frequency/at/runtime.h) \
  /home/intel_2/zephyr/include/drivers/rand32.h \
  /home/intel_2/zephyr/include/misc/dlist.h \
  /home/intel_2/zephyr/include/arch/cpu.h \
  /home/intel_2/zephyr/include/arch/x86/arch.h \
    $(wildcard include/config/num/dynamic/stubs.h) \
    $(wildcard include/config/max/num/task/irqs.h) \
    $(wildcard include/config/nanokernel.h) \
    $(wildcard include/config/num/dynamic/exc/stubs.h) \
    $(wildcard include/config/num/dynamic/exc/noerr/stubs.h) \
    $(wildcard include/config/x86/iamcu.h) \
    $(wildcard include/config/sse.h) \
    $(wildcard include/config/kernel/event/logger/interrupt.h) \
    $(wildcard include/config/mvic.h) \
    $(wildcard include/config/debug/info.h) \
    $(wildcard include/config/int/latency/benchmark.h) \
    $(wildcard include/config/fp/sharing.h) \
    $(wildcard include/config/debug/irqs.h) \
  /home/intel_2/zephyr/include/irq.h \
  /home/intel_2/zephyr/include/arch/x86/asm_inline.h \
  /home/intel_2/zephyr/include/arch/x86/asm_inline_gcc.h \
    $(wildcard include/config/cmov.h) \
  /home/intel_2/zephyr/include/sys_io.h \
  /home/intel_2/zephyr/include/arch/x86/addr_types.h \
  /home/intel_2/zephyr/include/arch/x86/segselect.h \
  /home/intel_2/zephyr/include/microkernel/base_api.h \
  /opt/zephyr-sdk/sysroots/i686-pokysdk-linux/usr/lib/iamcu-poky-elfiamcu/gcc/i586-poky-elfiamcu/5.2.1/include/stdbool.h \
  /home/intel_2/zephyr/include/microkernel/task.h \
  /home/intel_2/zephyr/include/sections.h \
    $(wildcard include/config/gdb/info.h) \
    $(wildcard include/config/sw/isr/table.h) \
  /home/intel_2/zephyr/include/section_tags.h \
  /home/intel_2/zephyr/include/microkernel/ticks.h \
  /home/intel_2/zephyr/include/microkernel/memory_map.h \
  /home/intel_2/zephyr/include/microkernel/mutex.h \
  /home/intel_2/zephyr/include/microkernel/mailbox.h \
  /home/intel_2/zephyr/include/microkernel/fifo.h \
  /home/intel_2/zephyr/include/microkernel/semaphore.h \
  /home/intel_2/zephyr/include/microkernel/event.h \
  /home/intel_2/zephyr/include/microkernel/memory_pool.h \
  /home/intel_2/zephyr/include/microkernel/pipe.h \
  /home/intel_2/zephyr/include/microkernel/task_irq.h \
  /home/intel_2/Zephyr_dev/hello_world/microkernel/outdir/misc/generated/sysgen/sysgen.h \
  /home/intel_2/zephyr/ext/lib/upm/src/apa102/apa102.h \
  /opt/zephyr-sdk//sysroots/iamcu-poky-elfiamcu/usr/i586-default-elfiamcu/include/stdlib.h \
  /opt/zephyr-sdk//sysroots/iamcu-poky-elfiamcu/usr/i586-default-elfiamcu/include/machine/ieeefp.h \
  /opt/zephyr-sdk//sysroots/iamcu-poky-elfiamcu/usr/i586-default-elfiamcu/include/_ansi.h \
  /opt/zephyr-sdk//sysroots/iamcu-poky-elfiamcu/usr/i586-default-elfiamcu/include/newlib.h \
  /opt/zephyr-sdk//sysroots/iamcu-poky-elfiamcu/usr/i586-default-elfiamcu/include/sys/config.h \
    $(wildcard include/config/h//.h) \
  /opt/zephyr-sdk//sysroots/iamcu-poky-elfiamcu/usr/i586-default-elfiamcu/include/sys/reent.h \
  /opt/zephyr-sdk//sysroots/iamcu-poky-elfiamcu/usr/i586-default-elfiamcu/include/_ansi.h \
  /opt/zephyr-sdk//sysroots/iamcu-poky-elfiamcu/usr/i586-default-elfiamcu/include/sys/_types.h \
  /opt/zephyr-sdk//sysroots/iamcu-poky-elfiamcu/usr/i586-default-elfiamcu/include/machine/_types.h \
  /opt/zephyr-sdk//sysroots/iamcu-poky-elfiamcu/usr/i586-default-elfiamcu/include/sys/lock.h \
  /opt/zephyr-sdk//sysroots/iamcu-poky-elfiamcu/usr/i586-default-elfiamcu/include/sys/cdefs.h \
  /opt/zephyr-sdk//sysroots/iamcu-poky-elfiamcu/usr/i586-default-elfiamcu/include/machine/stdlib.h \
  /opt/zephyr-sdk//sysroots/iamcu-poky-elfiamcu/usr/i586-default-elfiamcu/include/unistd.h \
  /opt/zephyr-sdk//sysroots/iamcu-poky-elfiamcu/usr/i586-default-elfiamcu/include/sys/unistd.h \
  /opt/zephyr-sdk//sysroots/iamcu-poky-elfiamcu/usr/i586-default-elfiamcu/include/sys/types.h \
  /opt/zephyr-sdk//sysroots/iamcu-poky-elfiamcu/usr/i586-default-elfiamcu/include/machine/types.h \
  /opt/zephyr-sdk//sysroots/iamcu-poky-elfiamcu/usr/i586-default-elfiamcu/include/string.h \
  /opt/zephyr-sdk//sysroots/iamcu-poky-elfiamcu/usr/i586-default-elfiamcu/include/sys/string.h \
  /home/intel_2/zephyr/ext/lib/upm/include/upm.h \
  /home/intel_2/zephyr/ext/lib/upm/include/upm_types.h \
  /home/intel_2/zephyr/ext/lib/upm/include/upm_math.h \
  /home/intel_2/zephyr/ext/lib/mraa/include/mraa/gpio.h \
  /home/intel_2/zephyr/ext/lib/mraa/include/mraa/common.h \
  /home/intel_2/zephyr/ext/lib/mraa/include/mraa/types.h \
  /home/intel_2/zephyr/ext/lib/mraa/include/mraa/spi.h \
  /opt/zephyr-sdk//sysroots/iamcu-poky-elfiamcu/usr/i586-default-elfiamcu/include/stdio.h \
  /opt/zephyr-sdk/sysroots/i686-pokysdk-linux/usr/lib/iamcu-poky-elfiamcu/gcc/i586-poky-elfiamcu/5.2.1/include/stdarg.h \
  /opt/zephyr-sdk//sysroots/iamcu-poky-elfiamcu/usr/i586-default-elfiamcu/include/sys/stdio.h \
  /home/intel_2/zephyr/include/spi.h \
  /home/intel_2/zephyr/include/device.h \
    $(wildcard include/config/kernel/init/priority/default.h) \
    $(wildcard include/config/device/power/management.h) \
  /opt/zephyr-sdk//sysroots/iamcu-poky-elfiamcu/usr/i586-default-elfiamcu/include/malloc.h \
  /opt/zephyr-sdk//sysroots/iamcu-poky-elfiamcu/usr/i586-default-elfiamcu/include/machine/malloc.h \
  /home/intel_2/zephyr/include/pinmux.h \
  /home/intel_2/zephyr/ext/lib/upm/src/utilities/upm_utilities.h \
    $(wildcard include/config/board/arduino/101.h) \
    $(wildcard include/config/board/arduino/101/sss.h) \
    $(wildcard include/config/board/quark/d2000/crb.h) \

/home/intel_2/Zephyr_dev/hello_world/microkernel/src/main_apa102.o: $(deps_/home/intel_2/Zephyr_dev/hello_world/microkernel/src/main_apa102.o)

$(deps_/home/intel_2/Zephyr_dev/hello_world/microkernel/src/main_apa102.o):
