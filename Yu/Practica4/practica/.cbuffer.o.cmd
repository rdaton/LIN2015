cmd_/home/barfelix/LIN2015/Yu/Practica4/practica/cbuffer.o :=  gcc-4.8 -Wp,-MD,/home/barfelix/LIN2015/Yu/Practica4/practica/.cbuffer.o.d  -nostdinc -isystem /usr/lib/gcc/x86_64-linux-gnu/4.8/include -I/usr/src/linux-headers-3.16.0-4-common/arch/x86/include -Iarch/x86/include/generated  -I/usr/src/linux-headers-3.16.0-4-common/include -Iinclude -I/usr/src/linux-headers-3.16.0-4-common/arch/x86/include/uapi -Iarch/x86/include/generated/uapi -I/usr/src/linux-headers-3.16.0-4-common/include/uapi -Iinclude/generated/uapi -include /usr/src/linux-headers-3.16.0-4-common/include/linux/kconfig.h   -I/home/barfelix/LIN2015/Yu/Practica4/practica -D__KERNEL__ -Wall -Wundef -Wstrict-prototypes -Wno-trigraphs -fno-strict-aliasing -fno-common -Werror-implicit-function-declaration -Wno-format-security -std=gnu89 -m64 -mno-mmx -mno-sse -mno-80387 -mno-fp-ret-in-387 -mpreferred-stack-boundary=3 -mtune=generic -mno-red-zone -mcmodel=kernel -funit-at-a-time -maccumulate-outgoing-args -DCONFIG_X86_X32_ABI -DCONFIG_AS_CFI=1 -DCONFIG_AS_CFI_SIGNAL_FRAME=1 -DCONFIG_AS_CFI_SECTIONS=1 -DCONFIG_AS_FXSAVEQ=1 -DCONFIG_AS_CRC32=1 -DCONFIG_AS_AVX=1 -DCONFIG_AS_AVX2=1 -pipe -Wno-sign-compare -fno-asynchronous-unwind-tables -mno-sse -mno-mmx -mno-sse2 -mno-3dnow -mno-avx -fno-delete-null-pointer-checks -O2 -Wframe-larger-than=2048 -fstack-protector -Wno-unused-but-set-variable -fno-var-tracking-assignments -g -pg -mfentry -DCC_USING_FENTRY -Wdeclaration-after-statement -Wno-pointer-sign -fno-strict-overflow -fconserve-stack -Werror=implicit-int -Werror=strict-prototypes -DCC_HAVE_ASM_GOTO  -DMODULE  -D"KBUILD_STR(s)=\#s" -D"KBUILD_BASENAME=KBUILD_STR(cbuffer)"  -D"KBUILD_MODNAME=KBUILD_STR(probar)" -c -o /home/barfelix/LIN2015/Yu/Practica4/practica/.tmp_cbuffer.o /home/barfelix/LIN2015/Yu/Practica4/practica/cbuffer.c

source_/home/barfelix/LIN2015/Yu/Practica4/practica/cbuffer.o := /home/barfelix/LIN2015/Yu/Practica4/practica/cbuffer.c

deps_/home/barfelix/LIN2015/Yu/Practica4/practica/cbuffer.o := \
  /home/barfelix/LIN2015/Yu/Practica4/practica/cbuffer.h \
  /usr/src/linux-headers-3.16.0-4-common/include/linux/vmalloc.h \
    $(wildcard include/config/mmu.h) \
    $(wildcard include/config/smp.h) \
  /usr/src/linux-headers-3.16.0-4-common/include/linux/spinlock.h \
    $(wildcard include/config/debug/spinlock.h) \
    $(wildcard include/config/generic/lockbreak.h) \
    $(wildcard include/config/preempt.h) \
    $(wildcard include/config/debug/lock/alloc.h) \
  /usr/src/linux-headers-3.16.0-4-common/include/linux/typecheck.h \
  /usr/src/linux-headers-3.16.0-4-common/include/linux/preempt.h \
    $(wildcard include/config/debug/preempt.h) \
    $(wildcard include/config/preempt/tracer.h) \
    $(wildcard include/config/preempt/count.h) \
    $(wildcard include/config/context/tracking.h) \
    $(wildcard include/config/preempt/notifiers.h) \
  /usr/src/linux-headers-3.16.0-4-common/include/linux/linkage.h \
  /usr/src/linux-headers-3.16.0-4-common/include/linux/compiler.h \
    $(wildcard include/config/sparse/rcu/pointer.h) \
    $(wildcard include/config/trace/branch/profiling.h) \
    $(wildcard include/config/profile/all/branches.h) \
    $(wildcard include/config/64bit.h) \
    $(wildcard include/config/enable/must/check.h) \
    $(wildcard include/config/enable/warn/deprecated.h) \
    $(wildcard include/config/kprobes.h) \
  /usr/src/linux-headers-3.16.0-4-common/include/linux/compiler-gcc.h \
    $(wildcard include/config/arch/supports/optimized/inlining.h) \
    $(wildcard include/config/optimize/inlining.h) \
    $(wildcard include/config/gcov/kernel.h) \
    $(wildcard include/config/arch/use/builtin/bswap.h) \
  /usr/src/linux-headers-3.16.0-4-common/include/uapi/linux/types.h \
  /usr/src/linux-headers-3.16.0-4-common/arch/x86/include/uapi/asm/types.h \
  /usr/src/linux-headers-3.16.0-4-common/include/uapi/asm-generic/types.h \
  /usr/src/linux-headers-3.16.0-4-common/include/asm-generic/int-ll64.h \
  /usr/src/linux-headers-3.16.0-4-common/include/uapi/asm-generic/int-ll64.h \
  /usr/src/linux-headers-3.16.0-4-common/arch/x86/include/uapi/asm/bitsperlong.h \
  /usr/src/linux-headers-3.16.0-4-common/include/asm-generic/bitsperlong.h \
  /usr/src/linux-headers-3.16.0-4-common/include/uapi/asm-generic/bitsperlong.h \
  /usr/src/linux-headers-3.16.0-4-common/include/uapi/linux/posix_types.h \
  /usr/src/linux-headers-3.16.0-4-common/include/linux/stddef.h \
  /usr/src/linux-headers-3.16.0-4-common/include/uapi/linux/stddef.h \
  /usr/src/linux-headers-3.16.0-4-common/arch/x86/include/asm/posix_types.h \
    $(wildcard include/config/x86/32.h) \
  /usr/src/linux-headers-3.16.0-4-common/arch/x86/include/uapi/asm/posix_types_64.h \
  /usr/src/linux-headers-3.16.0-4-common/include/uapi/asm-generic/posix_types.h \
  /usr/src/linux-headers-3.16.0-4-common/include/linux/stringify.h \
  /usr/src/linux-headers-3.16.0-4-common/include/linux/export.h \
    $(wildcard include/config/have/underscore/symbol/prefix.h) \
    $(wildcard include/config/modules.h) \
    $(wildcard include/config/modversions.h) \
    $(wildcard include/config/unused/symbols.h) \
  /usr/src/linux-headers-3.16.0-4-common/arch/x86/include/asm/linkage.h \
    $(wildcard include/config/x86/64.h) \
    $(wildcard include/config/x86/alignment/16.h) \
  /usr/src/linux-headers-3.16.0-4-common/include/linux/list.h \
    $(wildcard include/config/debug/list.h) \
  /usr/src/linux-headers-3.16.0-4-common/include/linux/types.h \
    $(wildcard include/config/have/uid16.h) \
    $(wildcard include/config/uid16.h) \
    $(wildcard include/config/lbdaf.h) \
    $(wildcard include/config/arch/dma/addr/t/64bit.h) \
    $(wildcard include/config/phys/addr/t/64bit.h) \
  /usr/src/linux-headers-3.16.0-4-common/include/linux/poison.h \
    $(wildcard include/config/illegal/pointer/value.h) \
  /usr/src/linux-headers-3.16.0-4-common/include/uapi/linux/const.h \
  /usr/src/linux-headers-3.16.0-4-common/arch/x86/include/asm/preempt.h \
  /usr/src/linux-headers-3.16.0-4-common/arch/x86/include/asm/rmwcc.h \
  /usr/src/linux-headers-3.16.0-4-common/arch/x86/include/asm/percpu.h \
    $(wildcard include/config/x86/64/smp.h) \
    $(wildcard include/config/x86/cmpxchg64.h) \
  /usr/src/linux-headers-3.16.0-4-common/include/linux/kernel.h \
    $(wildcard include/config/preempt/voluntary.h) \
    $(wildcard include/config/debug/atomic/sleep.h) \
    $(wildcard include/config/prove/locking.h) \
    $(wildcard include/config/panic/timeout.h) \
    $(wildcard include/config/ring/buffer.h) \
    $(wildcard include/config/tracing.h) \
    $(wildcard include/config/ftrace/mcount/record.h) \
  /usr/lib/gcc/x86_64-linux-gnu/4.8/include/stdarg.h \
  /usr/src/linux-headers-3.16.0-4-common/include/linux/bitops.h \
  /usr/src/linux-headers-3.16.0-4-common/arch/x86/include/asm/bitops.h \
    $(wildcard include/config/x86/cmov.h) \
  /usr/src/linux-headers-3.16.0-4-common/arch/x86/include/asm/alternative.h \
    $(wildcard include/config/paravirt.h) \
  /usr/src/linux-headers-3.16.0-4-common/arch/x86/include/asm/asm.h \
  /usr/src/linux-headers-3.16.0-4-common/arch/x86/include/asm/ptrace.h \
    $(wildcard include/config/x86/debugctlmsr.h) \
  /usr/src/linux-headers-3.16.0-4-common/arch/x86/include/asm/segment.h \
    $(wildcard include/config/cc/stackprotector.h) \
    $(wildcard include/config/x86/32/lazy/gs.h) \
  /usr/src/linux-headers-3.16.0-4-common/arch/x86/include/asm/cache.h \
    $(wildcard include/config/x86/l1/cache/shift.h) \
    $(wildcard include/config/x86/internode/cache/shift.h) \
    $(wildcard include/config/x86/vsmp.h) \
  /usr/src/linux-headers-3.16.0-4-common/arch/x86/include/asm/page_types.h \
    $(wildcard include/config/physical/start.h) \
    $(wildcard include/config/physical/align.h) \
  /usr/src/linux-headers-3.16.0-4-common/arch/x86/include/asm/page_64_types.h \
    $(wildcard include/config/randomize/base.h) \
    $(wildcard include/config/randomize/base/max/offset.h) \
  /usr/src/linux-headers-3.16.0-4-common/arch/x86/include/uapi/asm/ptrace.h \
  /usr/src/linux-headers-3.16.0-4-common/arch/x86/include/uapi/asm/ptrace-abi.h \
  /usr/src/linux-headers-3.16.0-4-common/arch/x86/include/asm/processor-flags.h \
    $(wildcard include/config/vm86.h) \
  /usr/src/linux-headers-3.16.0-4-common/arch/x86/include/uapi/asm/processor-flags.h \
  /usr/src/linux-headers-3.16.0-4-common/arch/x86/include/asm/paravirt_types.h \
    $(wildcard include/config/x86/local/apic.h) \
    $(wildcard include/config/x86/pae.h) \
    $(wildcard include/config/paravirt/debug.h) \
  /usr/src/linux-headers-3.16.0-4-common/arch/x86/include/asm/desc_defs.h \
  /usr/src/linux-headers-3.16.0-4-common/arch/x86/include/asm/kmap_types.h \
    $(wildcard include/config/debug/highmem.h) \
  /usr/src/linux-headers-3.16.0-4-common/include/asm-generic/kmap_types.h \
  /usr/src/linux-headers-3.16.0-4-common/arch/x86/include/asm/pgtable_types.h \
    $(wildcard include/config/kmemcheck.h) \
    $(wildcard include/config/mem/soft/dirty.h) \
    $(wildcard include/config/numa/balancing.h) \
    $(wildcard include/config/proc/fs.h) \
  /usr/src/linux-headers-3.16.0-4-common/arch/x86/include/asm/pgtable_64_types.h \
  /usr/src/linux-headers-3.16.0-4-common/arch/x86/include/asm/sparsemem.h \
    $(wildcard include/config/sparsemem.h) \
  /usr/src/linux-headers-3.16.0-4-common/arch/x86/include/asm/spinlock_types.h \
    $(wildcard include/config/paravirt/spinlocks.h) \
    $(wildcard include/config/nr/cpus.h) \
    $(wildcard include/config/queue/rwlock.h) \
  /usr/src/linux-headers-3.16.0-4-common/include/asm-generic/qrwlock_types.h \
  /usr/src/linux-headers-3.16.0-4-common/include/asm-generic/ptrace.h \
  /usr/src/linux-headers-3.16.0-4-common/arch/x86/include/asm/cpufeature.h \
    $(wildcard include/config/x86/debug/static/cpu/has.h) \
  /usr/src/linux-headers-3.16.0-4-common/arch/x86/include/asm/required-features.h \
    $(wildcard include/config/x86/minimum/cpu/family.h) \
    $(wildcard include/config/math/emulation.h) \
    $(wildcard include/config/x86/use/3dnow.h) \
    $(wildcard include/config/x86/p6/nop.h) \
    $(wildcard include/config/matom.h) \
  /usr/src/linux-headers-3.16.0-4-common/arch/x86/include/asm/barrier.h \
    $(wildcard include/config/x86/ppro/fence.h) \
  /usr/src/linux-headers-3.16.0-4-common/arch/x86/include/asm/nops.h \
    $(wildcard include/config/mk7.h) \
  /usr/src/linux-headers-3.16.0-4-common/include/asm-generic/bitops/find.h \
    $(wildcard include/config/generic/find/first/bit.h) \
  /usr/src/linux-headers-3.16.0-4-common/include/asm-generic/bitops/sched.h \
  /usr/src/linux-headers-3.16.0-4-common/arch/x86/include/asm/arch_hweight.h \
  /usr/src/linux-headers-3.16.0-4-common/include/asm-generic/bitops/const_hweight.h \
  /usr/src/linux-headers-3.16.0-4-common/include/asm-generic/bitops/le.h \
  /usr/src/linux-headers-3.16.0-4-common/arch/x86/include/uapi/asm/byteorder.h \
  /usr/src/linux-headers-3.16.0-4-common/include/linux/byteorder/little_endian.h \
  /usr/src/linux-headers-3.16.0-4-common/include/uapi/linux/byteorder/little_endian.h \
  /usr/src/linux-headers-3.16.0-4-common/include/linux/swab.h \
  /usr/src/linux-headers-3.16.0-4-common/include/uapi/linux/swab.h \
  /usr/src/linux-headers-3.16.0-4-common/arch/x86/include/uapi/asm/swab.h \
  /usr/src/linux-headers-3.16.0-4-common/include/linux/byteorder/generic.h \
  /usr/src/linux-headers-3.16.0-4-common/include/asm-generic/bitops/ext2-atomic-setbit.h \
  /usr/src/linux-headers-3.16.0-4-common/include/linux/log2.h \
    $(wildcard include/config/arch/has/ilog2/u32.h) \
    $(wildcard include/config/arch/has/ilog2/u64.h) \
  /usr/src/linux-headers-3.16.0-4-common/include/linux/printk.h \
    $(wildcard include/config/default/message/loglevel.h) \
    $(wildcard include/config/early/printk.h) \
    $(wildcard include/config/printk.h) \
    $(wildcard include/config/dynamic/debug.h) \
  /usr/src/linux-headers-3.16.0-4-common/include/linux/init.h \
    $(wildcard include/config/broken/rodata.h) \
    $(wildcard include/config/lto.h) \
  /usr/src/linux-headers-3.16.0-4-common/include/linux/kern_levels.h \
  /usr/src/linux-headers-3.16.0-4-common/include/linux/cache.h \
    $(wildcard include/config/arch/has/cache/line/size.h) \
  /usr/src/linux-headers-3.16.0-4-common/include/uapi/linux/kernel.h \
  /usr/src/linux-headers-3.16.0-4-common/include/uapi/linux/sysinfo.h \
  /usr/src/linux-headers-3.16.0-4-common/include/linux/dynamic_debug.h \
  /usr/src/linux-headers-3.16.0-4-common/include/asm-generic/percpu.h \
    $(wildcard include/config/have/setup/per/cpu/area.h) \
  /usr/src/linux-headers-3.16.0-4-common/include/linux/threads.h \
    $(wildcard include/config/base/small.h) \
  /usr/src/linux-headers-3.16.0-4-common/include/linux/percpu-defs.h \
    $(wildcard include/config/debug/force/weak/per/cpu.h) \
  /usr/src/linux-headers-3.16.0-4-common/include/linux/thread_info.h \
    $(wildcard include/config/compat.h) \
    $(wildcard include/config/debug/stack/usage.h) \
  /usr/src/linux-headers-3.16.0-4-common/include/linux/bug.h \
    $(wildcard include/config/generic/bug.h) \
  /usr/src/linux-headers-3.16.0-4-common/arch/x86/include/asm/bug.h \
    $(wildcard include/config/debug/bugverbose.h) \
  /usr/src/linux-headers-3.16.0-4-common/include/asm-generic/bug.h \
    $(wildcard include/config/bug.h) \
    $(wildcard include/config/generic/bug/relative/pointers.h) \
  /usr/src/linux-headers-3.16.0-4-common/arch/x86/include/asm/thread_info.h \
    $(wildcard include/config/ia32/emulation.h) \
  /usr/src/linux-headers-3.16.0-4-common/arch/x86/include/asm/page.h \
  /usr/src/linux-headers-3.16.0-4-common/arch/x86/include/asm/page_64.h \
    $(wildcard include/config/debug/virtual.h) \
    $(wildcard include/config/flatmem.h) \
  /usr/src/linux-headers-3.16.0-4-common/include/linux/range.h \
  /usr/src/linux-headers-3.16.0-4-common/include/asm-generic/memory_model.h \
    $(wildcard include/config/discontigmem.h) \
    $(wildcard include/config/sparsemem/vmemmap.h) \
  /usr/src/linux-headers-3.16.0-4-common/include/asm-generic/getorder.h \
  /usr/src/linux-headers-3.16.0-4-common/arch/x86/include/asm/processor.h \
    $(wildcard include/config/m486.h) \
    $(wildcard include/config/xen.h) \
  /usr/src/linux-headers-3.16.0-4-common/arch/x86/include/asm/vm86.h \
  /usr/src/linux-headers-3.16.0-4-common/arch/x86/include/uapi/asm/vm86.h \
  /usr/src/linux-headers-3.16.0-4-common/arch/x86/include/asm/math_emu.h \
  /usr/src/linux-headers-3.16.0-4-common/arch/x86/include/asm/sigcontext.h \
  /usr/src/linux-headers-3.16.0-4-common/arch/x86/include/uapi/asm/sigcontext.h \
  /usr/src/linux-headers-3.16.0-4-common/arch/x86/include/asm/current.h \
  /usr/src/linux-headers-3.16.0-4-common/arch/x86/include/asm/msr.h \
  /usr/src/linux-headers-3.16.0-4-common/arch/x86/include/uapi/asm/msr.h \
  /usr/src/linux-headers-3.16.0-4-common/arch/x86/include/uapi/asm/msr-index.h \
  /usr/src/linux-headers-3.16.0-4-common/include/uapi/linux/ioctl.h \
  /usr/src/linux-headers-3.16.0-4-common/arch/x86/include/uapi/asm/ioctl.h \
  /usr/src/linux-headers-3.16.0-4-common/include/asm-generic/ioctl.h \
  /usr/src/linux-headers-3.16.0-4-common/include/uapi/asm-generic/ioctl.h \
  /usr/src/linux-headers-3.16.0-4-common/arch/x86/include/uapi/asm/errno.h \
  /usr/src/linux-headers-3.16.0-4-common/include/uapi/asm-generic/errno.h \
  /usr/src/linux-headers-3.16.0-4-common/include/uapi/asm-generic/errno-base.h \
  /usr/src/linux-headers-3.16.0-4-common/arch/x86/include/asm/cpumask.h \
  /usr/src/linux-headers-3.16.0-4-common/include/linux/cpumask.h \
    $(wildcard include/config/cpumask/offstack.h) \
    $(wildcard include/config/hotplug/cpu.h) \
    $(wildcard include/config/debug/per/cpu/maps.h) \
    $(wildcard include/config/disable/obsolete/cpumask/functions.h) \
  /usr/src/linux-headers-3.16.0-4-common/include/linux/bitmap.h \
  /usr/src/linux-headers-3.16.0-4-common/include/linux/string.h \
    $(wildcard include/config/binary/printf.h) \
  /usr/src/linux-headers-3.16.0-4-common/include/uapi/linux/string.h \
  /usr/src/linux-headers-3.16.0-4-common/arch/x86/include/asm/string.h \
  /usr/src/linux-headers-3.16.0-4-common/arch/x86/include/asm/string_64.h \
  /usr/src/linux-headers-3.16.0-4-common/arch/x86/include/asm/paravirt.h \
  /usr/src/linux-headers-3.16.0-4-common/arch/x86/include/asm/special_insns.h \
  /usr/src/linux-headers-3.16.0-4-common/include/linux/personality.h \
  /usr/src/linux-headers-3.16.0-4-common/include/uapi/linux/personality.h \
  /usr/src/linux-headers-3.16.0-4-common/include/linux/math64.h \
    $(wildcard include/config/arch/supports/int128.h) \
  /usr/src/linux-headers-3.16.0-4-common/arch/x86/include/asm/div64.h \
  /usr/src/linux-headers-3.16.0-4-common/include/asm-generic/div64.h \
  /usr/src/linux-headers-3.16.0-4-common/include/linux/err.h \
  /usr/src/linux-headers-3.16.0-4-common/include/linux/irqflags.h \
    $(wildcard include/config/trace/irqflags.h) \
    $(wildcard include/config/irqsoff/tracer.h) \
    $(wildcard include/config/trace/irqflags/support.h) \
  /usr/src/linux-headers-3.16.0-4-common/arch/x86/include/asm/irqflags.h \
  /usr/src/linux-headers-3.16.0-4-common/include/linux/atomic.h \
    $(wildcard include/config/arch/has/atomic/or.h) \
    $(wildcard include/config/generic/atomic64.h) \
  /usr/src/linux-headers-3.16.0-4-common/arch/x86/include/asm/atomic.h \
  /usr/src/linux-headers-3.16.0-4-common/arch/x86/include/asm/cmpxchg.h \
  /usr/src/linux-headers-3.16.0-4-common/arch/x86/include/asm/cmpxchg_64.h \
  /usr/src/linux-headers-3.16.0-4-common/arch/x86/include/asm/atomic64_64.h \
  /usr/src/linux-headers-3.16.0-4-common/include/asm-generic/atomic-long.h \
  /usr/src/linux-headers-3.16.0-4-common/include/linux/bottom_half.h \
  /usr/src/linux-headers-3.16.0-4-common/include/linux/preempt_mask.h \
  /usr/src/linux-headers-3.16.0-4-common/include/linux/spinlock_types.h \
  /usr/src/linux-headers-3.16.0-4-common/include/linux/lockdep.h \
    $(wildcard include/config/lockdep.h) \
    $(wildcard include/config/lock/stat.h) \
    $(wildcard include/config/prove/rcu.h) \
  /usr/src/linux-headers-3.16.0-4-common/include/linux/rwlock_types.h \
  /usr/src/linux-headers-3.16.0-4-common/arch/x86/include/asm/spinlock.h \
  /usr/src/linux-headers-3.16.0-4-common/include/linux/jump_label.h \
    $(wildcard include/config/jump/label.h) \
  /usr/src/linux-headers-3.16.0-4-common/arch/x86/include/asm/jump_label.h \
  /usr/src/linux-headers-3.16.0-4-common/arch/x86/include/asm/qrwlock.h \
    $(wildcard include/config/x86/oostore.h) \
  /usr/src/linux-headers-3.16.0-4-common/include/asm-generic/qrwlock.h \
  /usr/src/linux-headers-3.16.0-4-common/include/linux/rwlock.h \
  /usr/src/linux-headers-3.16.0-4-common/include/linux/spinlock_api_smp.h \
    $(wildcard include/config/inline/spin/lock.h) \
    $(wildcard include/config/inline/spin/lock/bh.h) \
    $(wildcard include/config/inline/spin/lock/irq.h) \
    $(wildcard include/config/inline/spin/lock/irqsave.h) \
    $(wildcard include/config/inline/spin/trylock.h) \
    $(wildcard include/config/inline/spin/trylock/bh.h) \
    $(wildcard include/config/uninline/spin/unlock.h) \
    $(wildcard include/config/inline/spin/unlock/bh.h) \
    $(wildcard include/config/inline/spin/unlock/irq.h) \
    $(wildcard include/config/inline/spin/unlock/irqrestore.h) \
  /usr/src/linux-headers-3.16.0-4-common/include/linux/rwlock_api_smp.h \
    $(wildcard include/config/inline/read/lock.h) \
    $(wildcard include/config/inline/write/lock.h) \
    $(wildcard include/config/inline/read/lock/bh.h) \
    $(wildcard include/config/inline/write/lock/bh.h) \
    $(wildcard include/config/inline/read/lock/irq.h) \
    $(wildcard include/config/inline/write/lock/irq.h) \
    $(wildcard include/config/inline/read/lock/irqsave.h) \
    $(wildcard include/config/inline/write/lock/irqsave.h) \
    $(wildcard include/config/inline/read/trylock.h) \
    $(wildcard include/config/inline/write/trylock.h) \
    $(wildcard include/config/inline/read/unlock.h) \
    $(wildcard include/config/inline/write/unlock.h) \
    $(wildcard include/config/inline/read/unlock/bh.h) \
    $(wildcard include/config/inline/write/unlock/bh.h) \
    $(wildcard include/config/inline/read/unlock/irq.h) \
    $(wildcard include/config/inline/write/unlock/irq.h) \
    $(wildcard include/config/inline/read/unlock/irqrestore.h) \
    $(wildcard include/config/inline/write/unlock/irqrestore.h) \
  /usr/src/linux-headers-3.16.0-4-common/include/linux/rbtree.h \

/home/barfelix/LIN2015/Yu/Practica4/practica/cbuffer.o: $(deps_/home/barfelix/LIN2015/Yu/Practica4/practica/cbuffer.o)

$(deps_/home/barfelix/LIN2015/Yu/Practica4/practica/cbuffer.o):
