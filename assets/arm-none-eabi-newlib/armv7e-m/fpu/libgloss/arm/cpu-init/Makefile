#
#
DESTDIR =
VPATH = ../../../../../.././libgloss/arm/cpu-init ../../../../../.././libgloss/arm/cpu-init/.. ../../../../../.././libgloss/arm/cpu-init/../..
srcdir = ../../../../../.././libgloss/arm/cpu-init
objdir = .
srcroot = $(srcdir)/../../..
objroot = $(objdir)/../../..

prefix = /usr/local
exec_prefix = ${prefix}

host_alias = arm-none-eabi
target_alias = arm-none-eabi

bindir = ${exec_prefix}/bin
libdir = ${exec_prefix}/lib
tooldir = $(exec_prefix)/$(target_alias)

objtype = elf-

INSTALL = /usr/bin/install -c
INSTALL_PROGRAM = /usr/bin/install -c
INSTALL_DATA = /usr/bin/install -c -m 644

# Multilib support variables.
# TOP is used instead of MULTI{BUILD,SRC}TOP.
MULTISRCTOP = 
MULTIBUILDTOP = ../../
MULTIDIRS = 
MULTISUBDIR = /armv7e-m/fpu
MULTIDO = true
MULTICLEAN = true

SHELL =	/bin/sh

CC = arm-none-eabi-gcc -B/home/vkottler/Documents/newlib-cygwin/arm-none-eabi/armv7e-m/fpu/newlib/ -isystem /home/vkottler/Documents/newlib-cygwin/arm-none-eabi/armv7e-m/fpu/newlib/targ-include -isystem /home/vkottler/Documents/newlib-cygwin/newlib/libc/include -B/home/vkottler/Documents/newlib-cygwin/arm-none-eabi/armv7e-m/fpu/libgloss/arm -L/home/vkottler/Documents/newlib-cygwin/arm-none-eabi/armv7e-m/fpu/libgloss/libnosys -L/home/vkottler/Documents/newlib-cygwin/libgloss/arm  -mthumb -march=armv7e-m -mfloat-abi=hard -mfpu=fpv4-sp-d16

AS = arm-none-eabi-as
AR = arm-none-eabi-ar
LD = arm-none-eabi-ld
RANLIB = arm-none-eabi-ranlib

CPU_INIT_OBJS = rdimon-aem.o
CPU_INIT_INSTALL = install-cpu-init

CFLAGS		= -g

# Host specific makefile fragment comes in here.
NEWLIB_CFLAGS = `if [ -d ${objroot}/newlib ]; then echo -I${objroot}/newlib/targ-include -I${srcroot}/newlib/libc/include; fi`
NEWLIB_LDFLAGS = `if [ -d ${objroot}/newlib ]; then echo -B${objroot}/newlib/ -L${objroot}/newlib/; fi`

INCLUDES = -I. -I$(srcdir)/..
# Note that when building the library, ${MULTILIB} is not the way multilib
# options are passed; they're passed in $(CFLAGS).
CFLAGS_FOR_TARGET = -O2 -g ${MULTILIB} ${INCLUDES} ${NEWLIB_CFLAGS}
LDFLAGS_FOR_TARGET = ${MULTILIB} ${NEWLIB_LDFLAGS}
AR_FLAGS = rc

.c.o:
	$(CC) $(CFLAGS_FOR_TARGET) -O2 $(INCLUDES) -c $(CFLAGS) $<

.C.o:
	$(CC) $(CFLAGS_FOR_TARGET) -O2 $(INCLUDES) -c $(CFLAGS) $<
.s.o:
	$(AS) $(ASFLAGS_FOR_TARGET) $(INCLUDES) $(ASFLAGS) -o $*.o $<

#
# GCC knows to run the preprocessor on .S files before it assembles them.
#
.S.o:
	$(CC) $(CFLAGS_FOR_TARGET) $(INCLUDES) $(CFLAGS) -c $<

#
# this is a bogus target that'll produce an assembler from the
# C source with the right compiler options. this is so we can
# track down code generation or debug symbol bugs.
#
.c.s:
	$(CC) $(CFLAGS_FOR_TARGET) -S $(INCLUDES) $(CFLAGS) $<

.PHONY: all
all: ${CPU_INIT_OBJS}

#
# here's where we build the test programs for each target
#
.PHONY: test
test:

# Static pattern rule for assembling cpu init files to object files.
${CPU_INIT_OBJS}: %.o: %.S
	$(CC) $(CFLAGS_FOR_TARGET) $(CFLAGS) $(INCLUDES) -DARM_RDI_MONITOR -o $@ -c $<

clean mostlyclean:
	rm -f a.out core *.i *.o *-test *.srec *.dis *.x

distclean maintainer-clean realclean: clean
	rm -f Makefile *~

.PHONY: install info install-info clean-info
install: ${CPU_INIT_INSTALL}

install-cpu-init:
	test -d $(DESTDIR)${tooldir}/lib${MULTISUBDIR}/cpu-init || mkdir $(DESTDIR)${tooldir}/lib${MULTISUBDIR}/cpu-init
	set -e; for x in ${CPU_INIT_OBJS}; do ${INSTALL_DATA} $$x $(DESTDIR)${tooldir}/lib${MULTISUBDIR}/cpu-init/$$x; done

doc:
info:
install-info:
clean-info:

Makefile: Makefile.in ../config.status /home/vkottler/Documents/newlib-cygwin/libgloss/config/default.mh
	$(SHELL) ../config.status --file cpu-init/Makefile

../config.status: ../configure
	$(SHELL) ../config.status --recheck
