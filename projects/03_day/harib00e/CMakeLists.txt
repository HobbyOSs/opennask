message(STATUS "Entering directory projects/03_day/harib00e/")

########### next target ###############
set(ipl.img_SRCS ${root_SOURCE_DIR}/projects/03_day/harib00e/ipl.nas)
set(ipl.img_OUTS ${root_BINARY_DIR}/projects/03_day/harib00e/ipl.nas)

add_custom_target(03_day_harib00e_ipl
  COMMAND ${root_BINARY_DIR}/src/opennask ${ipl.img_SRCS} ${PROJECT_BINARY_DIR}/ipl.img
)

add_dependencies(images 03_day_harib00e_ipl)
