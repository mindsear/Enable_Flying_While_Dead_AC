AC_ADD_SCRIPT("${CMAKE_CURRENT_LIST_DIR}/src/enable_flying_while_dead.cpp")
AC_ADD_SCRIPT_LOADER("AddSC_enable_flying_while_dead" "${CMAKE_CURRENT_LIST_DIR}/src/enable_flying_while_dead_loader.h")

AC_ADD_CONFIG_FILE("${CMAKE_CURRENT_LIST_DIR}/conf/EnableFlyingWhileDead.conf.dist")
