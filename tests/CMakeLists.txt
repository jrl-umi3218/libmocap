# LIBMOCAP_TEST(NAME)
# -------------------
#
# Define a test named `NAME'.
#
# This macro will create a binary from `NAME.cc', link it against
# the library and add it to the testsuite.
#
MACRO(LIBMOCAP_TEST NAME)
  ADD_EXECUTABLE(${NAME} ${NAME}.cc)
  ADD_TEST(${NAME} ${CHECK_PREFIX} ${CMAKE_CURRENT_BINARY_DIR}/${NAME})

  TARGET_LINK_LIBRARIES(${NAME} mocap)

  SET_PROPERTY(
    TEST ${NAME} PROPERTY ENVIRONMENT
    "LIBMOCAP_DATA_PATH=${CMAKE_SOURCE_DIR}/tests/data/")
ENDMACRO(LIBMOCAP_TEST)

LIBMOCAP_TEST(marker-set-factory)
