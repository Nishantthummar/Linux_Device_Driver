# This file was created by Nishant

DESCRIPTION = "Simple helloworld "
SECTION = "new"

SRC_URI = "file://new.c"

S = "${WORKDIR}"

do_compile() {
	${CC} new.c -o new
}

do_install() {
	install -d ${D}${bindir}
	install -m 0755 new ${D}${bindir}
}
