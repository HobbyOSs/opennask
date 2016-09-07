message(STATUS "Entering directory projects/03_day/harib00a/")

########### next target ###############
set(ipl.img_SRCS ${root_SOURCE_DIR}/projects/03_day/harib00a/ipl.nas)
set(ipl.img_OUTS ${root_BINARY_DIR}/projects/03_day/harib00a/ipl.nas)

add_custom_target(03_day_harib00a_ipl
  COMMAND ${root_BINARY_DIR}/src/opennask ${ipl.img_SRCS} ${PROJECT_BINARY_DIR}/ipl.img
)

add_dependencies(images 03_day_harib00a_ipl)
