# Makefile for libjpeg-turbo

ifneq ($(TARGET_SIMULATOR),true)

##################################################
###                simd                        ###
##################################################
LOCAL_PATH := $(my-dir)
include $(CLEAR_VARS)

# From autoconf-generated Makefile
EXTRA_DIST = simd/nasm_lt.sh simd/jcclrmmx.asm simd/jcclrss2.asm simd/jdclrmmx.asm simd/jdclrss2.asm \
        simd/jdmrgmmx.asm simd/jdmrgss2.asm simd/jcclrss2-64.asm simd/jdclrss2-64.asm \
        simd/jdmrgss2-64.asm simd/CMakeLists.txt

libsimd_SOURCES_DIST = simd/jsimd_arm_neon.c \
                       simd/jdcolor-armv7.s \
                       simd/jdidct-armv7.s \
                       simd/armv6_idct.S


LOCAL_SRC_FILES := $(libsimd_SOURCES_DIST)

LOCAL_C_INCLUDES := $(LOCAL_PATH)/simd

LOCAL_CFLAGS := 
AM_CFLAGS := -march=armv7-a -mfpu=neon
AM_CCASFLAGS := -march=armv7-a -mfpu=neon

LOCAL_MODULE_TAGS := debug

LOCAL_MODULE := libsimd

include $(BUILD_STATIC_LIBRARY)

######################################################
###             libjpeg.so                         ###
######################################################

include $(CLEAR_VARS)

# From autoconf-generated Makefile
libjpeg_SOURCES_DIST = jcapimin.c jcapistd.c jccoefct.c \
        jccolor.c jcdctmgr.c jchuff.c jcinit.c jcmainct.c jcmarker.c \
        jcmaster.c jcomapi.c jcparam.c jcphuff.c jcprepct.c jcsample.c \
        jctrans.c jdapimin.c jdapistd.c jdatadst.c jdatasrc.c \
        jdcoefct.c jdcolor.c jddctmgr.c jdhuff.c jdinput.c jdmainct.c \
        jdmarker.c jdmaster.c jdmerge.c jdphuff.c jdpostct.c \
        jdsample.c jdtrans.c jerror.c jfdctflt.c jfdctfst.c jfdctint.c \
        jidctflt.c jidctfst.c jidctint.c jidctred.c jquant1.c \
        jquant2.c jutils.c jmemmgr.c jmem-ashmem.c jaricom.c jcarith.c \
	jdarith.c 
	#jmem-android.c jmemnobs.c

ifneq ($(WITHOUT_SIMD),true)
libjpeg_SOURCES_DIST += jsimd_none.c
endif 

LOCAL_SRC_FILES:= $(libjpeg_SOURCES_DIST)

LOCAL_SHARED_LIBRARIES := libcutils
LOCAL_STATIC_LIBRARIES := libsimd

LOCAL_C_INCLUDES := $(LOCAL_PATH) 

LOCAL_CFLAGS := -DAVOID_TABLES  -O3 -fstrict-aliasing -fprefetch-loop-arrays -DANDROID_TILE_BASED_DECODE -DANDROID_ARMV6_IDCT -DUSE_ANDROID_ASHMEM -DWITH_ARITH_ENC 

LOCAL_MODULE_PATH := $(TARGET_OUT_OPTIONAL_STATIC_LIBRARY)

LOCAL_MODULE_TAGS := debug

LOCAL_MODULE := libjpeg

include $(BUILD_SHARED_LIBRARY)

######################################################
###           libtrubojpeg.so                       ##
######################################################

include $(CLEAR_VARS)

# From autoconf-generated Makefile
libturbojpeg_SOURCES_DIST = jcapimin.c jcapistd.c jccoefct.c \
        jccolor.c jcdctmgr.c jchuff.c jcinit.c jcmainct.c jcmarker.c \
        jcmaster.c jcomapi.c jcparam.c jcphuff.c jcprepct.c jcsample.c \
        jctrans.c jdapimin.c jdapistd.c jdatadst.c jdatasrc.c \
        jdcoefct.c jdcolor.c jddctmgr.c jdhuff.c jdinput.c jdmainct.c \
        jdmarker.c jdmaster.c jdmerge.c jdphuff.c jdpostct.c \
        jdsample.c jdtrans.c jerror.c jfdctflt.c jfdctfst.c jfdctint.c \
        jidctflt.c jidctfst.c jidctint.c jidctred.c jquant1.c \
        jquant2.c jutils.c jmemmgr.c jmem-ashmem.c jaricom.c jcarith.c \
	jdarith.c turbojpegl.c turbojpeg-mapfile
	#jmem-android.c jmemnobs.c

ifneq ($(WITHOUT_SIMD),true)
libjpeg_SOURCES_DIST += jsimd_none.c
endif 

LOCAL_SRC_FILES:= $(libturbojpeg_SOURCES_DIST)

LOCAL_SHARED_LIBRARIES := libcutils
LOCAL_STATIC_LIBRARIES := libsimd

LOCAL_C_INCLUDES := $(LOCAL_PATH) 

LOCAL_CFLAGS := -DAVOID_TABLES  -O3 -fstrict-aliasing -fprefetch-loop-arrays -DANDROID_TILE_BASED_DECODE -DANDROID_ARMV6_IDCT -DUSE_ANDROID_ASHMEM

LOCAL_MODULE_PATH := $(TARGET_OUT_OPTIONAL_STATIC_LIBRARY)

LOCAL_MODULE_TAGS := debug

LOCAL_MODULE := libturbojpeg

include $(BUILD_SHARED_LIBRARY)

######################################################
###         cjpeg                                  ###
######################################################

include $(CLEAR_VARS)

# From autoconf-generated Makefile
cjpeg_SOURCES = cdjpeg.c cjpeg.c rdbmp.c rdgif.c \
	rdppm.c rdswitch.c rdtarga.c

LOCAL_SRC_FILES:= $(cjpeg_SOURCES)

LOCAL_SHARED_LIBRARIES := libjpeg

LOCAL_C_INCLUDES := $(LOCAL_PATH) 

LOCAL_CFLAGS := -DBMP_SUPPORTED -DGIF_SUPPORTED -DPPM_SUPPORTED -DTARGA_SUPPORTED 

LOCAL_MODULE_PATH := $(TARGET_OUT_OPTIONAL_EXECUTABLE)

LOCAL_MODULE_TAGS := debug

LOCAL_MODULE := cjpeg

include $(BUILD_EXECUTABLE)

######################################################
###            djpeg                               ###
######################################################

include $(CLEAR_VARS)

# From autoconf-generated Makefile
djpeg_SOURCES = cdjpeg.c djpeg.c rdcolmap.c rdswitch.c \
	wrbmp.c wrgif.c wrppm.c wrtarga.c

LOCAL_SRC_FILES:= $(djpeg_SOURCES)

LOCAL_SHARED_LIBRARIES := libjpeg

LOCAL_C_INCLUDES := $(LOCAL_PATH) 

LOCAL_CFLAGS := -DBMP_SUPPORTED -DGIF_SUPPORTED -DPPM_SUPPORTED -DTARGA_SUPPORTED

LOCAL_MODULE_PATH := $(TARGET_OUT_OPTIONAL_EXECUTABLE)

LOCAL_MODULE_TAGS := debug

LOCAL_MODULE := djpeg

include $(BUILD_EXECUTABLE)

######################################################
###            jpegtran                            ###
######################################################

include $(CLEAR_VARS)

# From autoconf-generated Makefile
jpegtran_SOURCES = jpegtran.c rdswitch.c cdjpeg.c transupp.c

LOCAL_SRC_FILES:= $(jpegtran_SOURCES)

LOCAL_SHARED_LIBRARIES := libjpeg

LOCAL_C_INCLUDES := $(LOCAL_PATH) 

LOCAL_CFLAGS := 

LOCAL_MODULE_PATH := $(TARGET_OUT_OPTIONAL_EXECUTABLE)

LOCAL_MODULE_TAGS := debug

LOCAL_MODULE := jpegtran

include $(BUILD_EXECUTABLE)

######################################################
###              jpegut                            ###
######################################################

include $(CLEAR_VARS)

# From autoconf-generated Makefile
jpegut_SOURCES = jpegut.c bmp.c

LOCAL_SRC_FILES:= $(jpegut_SOURCES)

LOCAL_SHARED_LIBRARIES := libturbojpeg

LOCAL_C_INCLUDES := $(LOCAL_PATH) 

LOCAL_CFLAGS := 

LOCAL_MODULE_PATH := $(TARGET_OUT_OPTIONAL_EXECUTABLE)

LOCAL_MODULE_TAGS := debug

LOCAL_MODULE := jpegut

include $(BUILD_EXECUTABLE)

######################################################
###              jpgtest                           ###
######################################################

include $(CLEAR_VARS)

# From autoconf-generated Makefile
jpgtest_SOURCES = jpgtest.c bmp.c

LOCAL_SRC_FILES:= $(jpgtest_SOURCES)

LOCAL_SHARED_LIBRARIES := libturbojpeg

LOCAL_C_INCLUDES := $(LOCAL_PATH) 

LOCAL_CFLAGS := 

LOCAL_MODULE_PATH := $(TARGET_OUT_OPTIONAL_EXECUTABLE)

LOCAL_MODULE_TAGS := debug

LOCAL_MODULE := jpgtest

include $(BUILD_EXECUTABLE)

######################################################
###             rdjpgcom                           ###
######################################################

include $(CLEAR_VARS)

# From autoconf-generated Makefile
rdjpgcom_SOURCES = rdjpgcom.c

LOCAL_SRC_FILES:= $(rdjpgcom_SOURCES)

LOCAL_SHARED_LIBRARIES := libjpeg

LOCAL_C_INCLUDES := $(LOCAL_PATH) 

LOCAL_CFLAGS := 

LOCAL_MODULE_PATH := $(TARGET_OUT_OPTIONAL_EXECUTABLE)

LOCAL_MODULE_TAGS := debug

LOCAL_MODULE := rdjpgcom

include $(BUILD_EXECUTABLE)

######################################################
###           wrjpgcom                            ###
######################################################

include $(CLEAR_VARS)

# From autoconf-generated Makefile
wrjpgcom_SOURCES = wrjpgcom.c

LOCAL_SRC_FILES:= $(wrjpgcom_SOURCES)

LOCAL_SHARED_LIBRARIES := libjpeg

LOCAL_C_INCLUDES := $(LOCAL_PATH) 

LOCAL_CFLAGS := 

LOCAL_MODULE_PATH := $(TARGET_OUT_OPTIONAL_EXECUTABLE)

LOCAL_MODULE_TAGS := debug

LOCAL_MODULE := wrjpgcom

include $(BUILD_EXECUTABLE)

endif  # TARGET_SIMULATOR != true
