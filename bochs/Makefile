# $FreeBSD$

SRCDIR=	${.CURDIR:H}/drivers/gpu/drm/bochs

.PATH:	${SRCDIR} ${SRCDIR}/../

.include "../kconfig.mk"

KMOD= 	bochs
SRCS= 	\
	bochs_drv.c \
	bochs_hw.c \
	bochs_kms.c \
	bochs_mm.c \
	drm_simple_kms_helper.c \
	drm_gem_vram_helper.c \
	drm_vram_mm_helper.c

CLEANFILES+= ${KMOD}.ko.full ${KMOD}.ko.debug

CFLAGS+= -I${.CURDIR:H}/linuxkpi/gplv2/include
CFLAGS+= -I${SYSDIR}/compat/linuxkpi/common/include
CFLAGS+= -I${.CURDIR:H}/linuxkpi/dummy/include

CFLAGS+= -I${.CURDIR:H}/include
CFLAGS+= -I${.CURDIR:H}/include/drm
CFLAGS+= -I${.CURDIR:H}/include/uapi
CFLAGS+= -I${SRCDIR:H:H}

CFLAGS+= '-DKBUILD_MODNAME="${KMOD}"'
CFLAGS+= '-DLINUXKPI_PARAM_PREFIX=${KMOD}_' -DDRM_SYSCTL_PARAM_PREFIX=_${KMOD} -DLINUXKPI_VERSION=50000
CFLAGS+= -DCONFIG_PM_SLEEP
CFLAGS+= ${KCONFIG:C/(.*)/-DCONFIG_\1/}

SRCS	+=			\
	opt_acpi.h		\
	opt_compat.h		\
	opt_drm.h		\
	opt_platform.h		\
	opt_splash.h		\
	opt_syscons.h		\
	opt_teken.h		\
	acpi_if.h		\
	bus_if.h		\
	fb_if.h			\
	device_if.h		\
	pci_if.h		\
	pci_iov_if.h		\
	vnode_if.h

CWARNFLAGS.bochs_hw.c= -Wno-pointer-arith -Wno-format

.include <bsd.kmod.mk>
