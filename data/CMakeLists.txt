message(STATUS "Entering directory data/")

# 1 line comments and opecode 'DB'
# 2 line source and opecode 'DB'
# 3 RESB command
# 4 Assembly to create img, "Hello, World"
# 5 line comments and opecode 'DW'
# 6 Day1 assembly file
# 7 Day2 assembly file
# 8 Day3 assembly file + 'JC', 'JE'

foreach(loop_var RANGE 1 8)
  add_test(
    NAME opennask_${loop_var}
    COMMAND $<TARGET_FILE:opennask> ${root_SOURCE_DIR}/data/0${loop_var}_nasfile.nas ${root_BINARY_DIR}/data/0${loop_var}_nasfile.img)
endforeach()
