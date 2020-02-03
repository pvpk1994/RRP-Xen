QEMU_ROOT ?= ..
XEN_ROOT ?= $(QEMU_ROOT)/../xen-unstable.hg

include ../config-host.mak

TARGET_ARCH=i386
TARGET_PATH:=$(SRC_PATH)/$(TARGET_DIRS)
CONFIG_SOFTMMU=yes

TARGET_CPPFLAGS += -isystem /root/xen/stubdom/../extras/mini-os/include -D__MINIOS__ -DHAVE_LIBC -isystem /root/xen/stubdom/../extras/mini-os/include/posix -isystem /root/xen/stubdom/../tools/xenstore/include  -isystem /root/xen/stubdom/../extras/mini-os/include/x86 -isystem /root/xen/stubdom/../extras/mini-os/include/x86/x86_64 -U __linux__ -U __FreeBSD__ -U __sun__ -nostdinc -isystem /root/xen/stubdom/../extras/mini-os/include/posix -isystem /root/xen/stubdom/cross-root-x86_64/x86_64-xen-elf/include -isystem /usr/lib/gcc/x86_64-linux-gnu/5/include -isystem /root/xen/stubdom/lwip-x86_64/src/include -isystem /root/xen/stubdom/lwip-x86_64/src/include/ipv4 -I/root/xen/stubdom/include -I/root/xen/stubdom/../xen/include -DCONFIG_PARAVIRT -DCONFIG_SPARSE_BSS -DCONFIG_QEMU_XS_ARGS -DCONFIG_PCIFRONT -DCONFIG_BLKFRONT -DCONFIG_NETFRONT -DCONFIG_KBDFRONT -DCONFIG_FBFRONT -DCONFIG_CONSFRONT -DCONFIG_XENBUS -D__XEN_INTERFACE_VERSION__=__XEN_LATEST_INTERFACE_VERSION__ -DCONFIG_STUBDOM -D__XEN_TOOLS__
TARGET_CFLAGS= -mno-red-zone -O1 -fno-omit-frame-pointer -O1 -fno-omit-frame-pointer  -m64 -mno-red-zone -fno-reorder-blocks -fno-asynchronous-unwind-tables -m64 -DBUILD_ID -fno-strict-aliasing -std=gnu99 -Wall -Wstrict-prototypes -Wdeclaration-after-statement -Wno-unused-but-set-variable -Wno-unused-local-typedefs   -fno-stack-protector -fno-exceptions
TARGET_LDFLAGS= -nostdlib -L/root/xen/stubdom/cross-root-x86_64/x86_64-xen-elf/lib

bindir = ${LIBEXEC_BIN}
