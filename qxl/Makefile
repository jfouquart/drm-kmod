# $FreeBSD$

SRCDIR=	${.CURDIR:H}/drivers/gpu/drm/qxl

.PATH:	${SRCDIR}

.include "../kconfig.mk"

KMOD=	qxlkms

SRCS=	\
	qxl_cmd.c	\
	qxl_debugfs.c	\
	qxl_display.c	\
	qxl_draw.c	\
	qxl_drv.c	\
	qxl_dumb.c	\
	qxl_fb.c	\
	qxl_gem.c	\
	qxl_image.c	\
	qxl_ioctl.c	\
	qxl_irq.c	\
	qxl_kms.c	\
	qxl_object.c	\
	qxl_prime.c	\
	qxl_release.c	\
	qxl_ttm.c

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
CFLAGS+= ${KCONFIG:C/(.*)/-DCONFIG_\1/}

SRCS	+=			\
	opt_acpi.h		\
	opt_compat.h		\
	opt_drm.h		\
	opt_syscons.h		\
	acpi_if.h		\
	bus_if.h		\
	fb_if.h			\
	device_if.h		\
	pci_if.h		\
	vnode_if.h

CWARNFLAGS+= -Wno-expansion-to-defined -Wno-pointer-arith -Wno-format
# CWARNFLAGS+=	-Wno-pointer-arith -Wno-format -Wno-cast-qual
CWARNFLAGS+=	-Wno-pointer-sign

# CWARNFLAGS.qxl_drv.c=	-Wno-incompatible-pointer-types
CWARNFLAGS.qxl_ttm.c=	-Wno-incompatible-pointer-types

.include <bsd.kmod.mk>
