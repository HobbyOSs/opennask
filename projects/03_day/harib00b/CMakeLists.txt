message(STATUS "Entering directory projects/03_day/harib00b/")

########### next target ###############
set(ipl.img_SRCS ${root_SOURCE_DIR}/projects/03_day/harib00b/ipl.nas)
set(ipl.img_OUTS ${root_BINARY_DIR}/projects/03_day/harib00b/ipl.img)

add_custom_target(03_day_harib00b_ipl
  COMMAND ${root_BINARY_DIR}/src/opennask ${ipl.img_SRCS} ${ipl.img_OUTS}
)

add_dependencies(images 03_day_harib00b_ipl)
