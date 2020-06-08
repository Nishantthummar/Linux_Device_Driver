# Recipe created by recipetool
# This is the basis of a recipe and may need further editing in order to be fully functional.
# (Feel free to remove these comments when editing.)

# Unable to find any files that looked like license statements. Check the accompanying
# documentation and source headers and set LICENSE and LIC_FILES_CHKSUM accordingly.
#
# NOTE: LICENSE is being set to "CLOSED" to allow you to at least start building - if
# this is not accurate with respect to the licensing of the software being built (it
# will not be in most cases) you must specify the correct value before using this
# recipe for anything other than initial testing/development!
LICENSE = "GPLv2"
LIC_FILES_CHKSUM = "file://${COREBASE}/meta-self/COPYING.MIT;md5=3da9cfbcb788c80a0384361b4de20420"

inherit module

SRC_URI = "file://self.c \
           file://Makefile \
           file://self.ko \
          "

S = "${WORKDIR}"

# NOTE: no Makefile found, unable to determine what needs to be done
RPROVIDES_${PN} += "kernel-module-self"
IMAGE_INSTALL += "vncserver"
