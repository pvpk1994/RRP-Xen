CONFIG_STUBDOM=yes
prefix=/usr/local
bindir=${prefix}/bin
mandir=$(MANDIR)
datadir=$(SHAREDIR)/qemu
docdir=$(SHAREDIR)/doc/qemu
MAKE=make
INSTALL=install
CC=gcc
HOST_CC=gcc
AR=ar
STRIP=strip -s -R .comment -R .note
OS_CFLAGS=
OS_LDFLAGS=
ARCH_CFLAGS=-m32
ARCH_LDFLAGS=-m32
EXESUF=
AIOLIBS=
ARCH=i386
CONFIG_GDBSTUB=
CONFIG_OSS=
CONFIG_OSS_LIB=
VERSION=0.10.2
SRC_PATH=/root/xen/stubdom/ioemu
TARGET_DIRS=i386-stubdom
BUILD_DOCS=
CONFIG_AIO=
TOOLS=
QEMU_ROOT ?= /root/xen/stubdom/ioemu
XEN_ROOT=/root/xen/stubdom/..
include $(XEN_ROOT)/tools/Rules.mk
SUBDIR_RULES=subdir-$(TARGET_DIRS)
subdir-$(TARGET_DIRS): libqemu_common.a
include $(QEMU_ROOT)/xen-hooks.mak
HOST_USB=stub

HOST_LDFLAGS= $(LDFLAGS) -g -Wl,--warn-common
HOST_CFLAGS= $(CFLAGS)
CPPFLAGS += -isystem /root/xen/stubdom/../extras/mini-os/include -D__MINIOS__ -DHAVE_LIBC -isystem /root/xen/stubdom/../extras/mini-os/include/posix -isystem /root/xen/stubdom/../tools/xenstore/include  -isystem /root/xen/stubdom/../extras/mini-os/include/x86 -isystem /root/xen/stubdom/../extras/mini-os/include/x86/x86_64 -U __linux__ -U __FreeBSD__ -U __sun__ -nostdinc -isystem /root/xen/stubdom/../extras/mini-os/include/posix -isystem /root/xen/stubdom/cross-root-x86_64/x86_64-xen-elf/include -isystem /usr/lib/gcc/x86_64-linux-gnu/5/include -isystem /root/xen/stubdom/lwip-x86_64/src/include -isystem /root/xen/stubdom/lwip-x86_64/src/include/ipv4 -I/root/xen/stubdom/include -I/root/xen/stubdom/../xen/include -DCONFIG_PARAVIRT -DCONFIG_SPARSE_BSS -DCONFIG_QEMU_XS_ARGS -DCONFIG_PCIFRONT -DCONFIG_BLKFRONT -DCONFIG_NETFRONT -DCONFIG_KBDFRONT -DCONFIG_FBFRONT -DCONFIG_CONSFRONT -DCONFIG_XENBUS -D__XEN_INTERFACE_VERSION__=__XEN_LATEST_INTERFACE_VERSION__ -DCONFIG_STUBDOM -MF .$(@F).d 
CFLAGS= -mno-red-zone -O1 -fno-omit-frame-pointer -O1 -fno-omit-frame-pointer  -m64 -mno-red-zone -fno-reorder-blocks -fno-asynchronous-unwind-tables -m64 -DBUILD_ID -fno-strict-aliasing -std=gnu99 -Wall -Wstrict-prototypes -Wdeclaration-after-statement -Wno-unused-but-set-variable -Wno-unused-local-typedefs   -fno-stack-protector -fno-exceptions
LDFLAGS= -nostdlib -L/root/xen/stubdom/cross-root-x86_64/x86_64-xen-elf/lib

SUBDIR_RULES=subdir-$(TARGET_DIRS)
subdir-$(TARGET_DIRS): libqemu_common.a
include $(QEMU_ROOT)/xen-hooks.mak

