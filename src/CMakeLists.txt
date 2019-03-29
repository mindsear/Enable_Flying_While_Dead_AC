CU_SET_PATH("CMAKE_TEMPLATE_SRC_DIR" "${CMAKE_CURRENT_LIST_DIR}")

AC_ADD_SCRIPT("${CMAKE_TEMPLATE_SRC_DIR}/enable_flying_while_dead.cpp")

AC_ADD_SCRIPT_LOADER("AddSC_enable_flying_while_dead" "${CMAKE_TEMPLATE_SRC_DIR}/enable_flying_while_dead_loader.h")