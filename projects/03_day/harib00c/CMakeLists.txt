message(STATUS "Entering directory projects/03_day/harib00c/")

########### next target ###############
set(ipl.img_SRCS ${root_SOURCE_DIR}/projects/03_day/harib00c/ipl.nas)
set(ipl.img_OUTS ${root_BINARY_DIR}/projects/03_day/harib00c/ipl.img)

add_custom_target(03_day_harib00c_ipl
  COMMAND ${root_BINARY_DIR}/src/opennask ${ipl.img_SRCS} ${ipl.img_OUTS}
)

add_dependencies(images 03_day_harib00c_ipl)
