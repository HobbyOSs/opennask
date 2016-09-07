message(STATUS "Entering directory projects/01_day/helloos1/")

########### next target ###############
set(helloos.img_SRCS ${root_SOURCE_DIR}/projects/01_day/helloos1/helloos.nas)
set(helloos.img_OUTS ${root_BINARY_DIR}/projects/01_day/helloos1/helloos.img)

add_custom_target(01_day_helloos1_helloos
  COMMAND ${root_BINARY_DIR}/src/opennask ${helloos.img_SRCS} ${helloos.img_OUTS}
)

add_dependencies(images 01_day_helloos1_helloos)
