AC_INIT([clockchain], [0.0.1])
AC_CONFIG_AUX_DIR([build-aux])
AM_INIT_AUTOMAKE([-Wall -Werror foreign no-define subdir-objects])
AC_PROG_CXX
AC_CONFIG_HEADER([config.h])
AC_CONFIG_FILES([Makefile src/Makefile tests/Makefile])
PKG_CHECK_MODULES(check, check >= 0.10.0)
PKG_CHECK_MODULES(libcrypto, libcrypto++ >= 5.0.0)
PKG_CHECK_MODULES(sqlite, sqlite3 >= 3.0.0)
AC_OUTPUT
