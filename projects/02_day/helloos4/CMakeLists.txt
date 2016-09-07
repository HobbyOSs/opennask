message(STATUS "Entering directory projects/02_day/helloos4/")

########### next target ###############
set(ipl.img_SRCS ${root_SOURCE_DIR}/projects/02_day/helloos4/ipl.nas)
set(ipl.img_OUTS ${root_BINARY_DIR}/projects/02_day/helloos4/ipl.nas)

add_custom_target(02_day_helloos4_helloos
  COMMAND ${root_BINARY_DIR}/src/opennask ${ipl.img_SRCS} ${PROJECT_BINARY_DIR}/ipl.img
)

add_dependencies(images 02_day_helloos4_helloos)
