AC_INIT([Command], [0.3], [bugz@dlugolecki.net.pl], [command], [http://dlugolecki.net.pl/software/command/])
AC_PREREQ([2.59])
AM_INIT_AUTOMAKE([1.10 -Wall])
AC_CONFIG_HEADERS([config.h])
AC_PROG_CXX
AC_CONFIG_FILES([
    include/Makefile
    Makefile
    tests/Makefile
    examples/Makefile
    docs/Makefile
])

AC_CHECK_PROGS([DOXYGEN], [doxygen])
if test -z "$DOXYGEN"; then
    AC_MSG_WARN([Doxygen not found - continuing without Doxygen support])
fi

AM_CONDITIONAL([HAVE_DOXYGEN], [test -n "$DOXYGEN"])
AM_COND_IF([HAVE_DOXYGEN], [AC_CONFIG_FILES([docs/Doxyfile])])

AC_OUTPUT

