noinst_LTLIBRARIES = libsimd.la

BUILT_SOURCES = jsimdcfg.inc

EXTRA_DIST = nasm_lt.sh jcclrmmx.asm jcclrss2.asm jdclrmmx.asm jdclrss2.asm \
	jdmrgmmx.asm jdmrgss2.asm jcclrss2-64.asm jdclrss2-64.asm \
	jdmrgss2-64.asm CMakeLists.txt

if SIMD_ARM_NEON

AM_CFLAGS = -march=armv7-a -mfpu=neon
AM_CCASFLAGS = -march=armv7-a -mfpu=neon

libsimd_la_SOURCES = jsimd_arm_neon.c \
                     jdcolor-armv7.s \
                     jdidct-armv7.s \
                     armv6_idct.S

jdcolor-armv7.lo: jdcolor-armv7.s
jdidct-armv7.lo: jdidct-armv7.s

endif

if SIMD_X86_64

libsimd_la_SOURCES = jsimd_x86_64.c \
	jsimd.h jsimdcfg.inc.h \
	jsimdext.inc jcolsamp.inc jdct.inc \
	jfsseflt-64.asm \
	jccolss2-64.asm jdcolss2-64.asm \
	jcsamss2-64.asm jdsamss2-64.asm jdmerss2-64.asm \
	jcqnts2i-64.asm jfss2fst-64.asm jfss2int-64.asm \
	jiss2red-64.asm jiss2int-64.asm jiss2fst-64.asm \
	jcqnts2f-64.asm jiss2flt-64.asm

jccolss2-64.lo: jcclrss2-64.asm
jdcolss2-64.lo: jdclrss2-64.asm
jdmerss2-64.lo: jdmrgss2-64.asm

.asm.lo:
	$(LIBTOOL) --mode=compile --tag NASM $(srcdir)/nasm_lt.sh $(NASM) $(NAFLAGS) -I$(srcdir) $< -o $@

endif

if SIMD_I386

libsimd_la_SOURCES = jsimd_i386.c \
	jsimd.h jsimdcfg.inc.h \
	jsimdext.inc jcolsamp.inc jdct.inc \
	jsimdcpu.asm \
	jccolmmx.asm jdcolmmx.asm \
	jcsammmx.asm jdsammmx.asm jdmermmx.asm \
	jcqntmmx.asm jfmmxfst.asm jfmmxint.asm \
	jimmxred.asm jimmxint.asm jimmxfst.asm \
	jcqnt3dn.asm jf3dnflt.asm ji3dnflt.asm \
	jcqntsse.asm jfsseflt.asm jisseflt.asm \
	jccolss2.asm jdcolss2.asm \
	jcsamss2.asm jdsamss2.asm jdmerss2.asm \
	jcqnts2i.asm jfss2fst.asm jfss2int.asm \
	jiss2red.asm jiss2int.asm jiss2fst.asm \
	jcqnts2f.asm jiss2flt.asm

jccolmmx.lo: jcclrmmx.asm
jccolss2.lo: jcclrss2.asm
jdcolmmx.lo: jdclrmmx.asm
jdcolss2.lo: jdclrss2.asm
jdmermmx.lo: jdmrgmmx.asm
jdmerss2.lo: jdmrgss2.asm

.asm.lo:
	$(LIBTOOL) --mode=compile --tag NASM $(srcdir)/nasm_lt.sh $(NASM) $(NAFLAGS) -I$(srcdir) $< -o $@

endif

AM_CPPFLAGS = -I$(top_srcdir) 


jsimdcfg.inc: $(srcdir)/jsimdcfg.inc.h ../jpeglib.h ../jconfig.h ../jmorecfg.h
	$(CPP) -I$(top_builddir) -I$(top_builddir)/simd $(srcdir)/jsimdcfg.inc.h | $(EGREP) "^[\;%]|^\ %" | sed 's%_cpp_protection_%%' | sed 's@% define@%define@g' > $@
