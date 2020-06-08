SUMMARY = "Example of how to build an external Linux kernel module"
LICENSE = "GPLv2"
LIC_FILES_CHKSUM = "file://${COREBASE}/meta-kmodule/COPYING.MIT;md5=3da9cfbcb788c80a0384361b4de20420"

inherit module

SRC_URI = "file://Makefile \
           file://kmodule.c \
          "

S = "${WORKDIR}"

# The inherit of module.bbclass will automatically name module packages with
# "kernel-module-" prefix as required by the oe-core build environment.

RPROVIDES_${PN} += "kernel-module-kmodule"
