message(STATUS "Entering directory projects/02_day/helloos3/")

########### next target ###############
set(helloos.img_SRCS ${root_SOURCE_DIR}/projects/02_day/helloos3/helloos.nas)
set(helloos.img_OUTS ${root_BINARY_DIR}/projects/02_day/helloos3/helloos.nas)

add_custom_target(02_day_helloos3_helloos
  COMMAND ${root_BINARY_DIR}/src/opennask ${helloos.img_SRCS} ${PROJECT_BINARY_DIR}/helloos.img
)

add_dependencies(images 02_day_helloos3_helloos)
