message(STATUS "Entering directory projects/03_day/harib00f/")

########### next target ###############
set(ipl.img_SRCS ${root_SOURCE_DIR}/projects/03_day/harib00f/ipl.nas)
set(ipl.img_OUTS ${root_BINARY_DIR}/projects/03_day/harib00f/ipl.img)

add_custom_target(03_day_harib00f_ipl
  COMMAND ${root_BINARY_DIR}/src/opennask ${ipl.img_SRCS} ${ipl.img_OUTS}
)

add_dependencies(images 03_day_harib00f_ipl)

########### next target ###############
set(haribote.img_SRCS ${root_SOURCE_DIR}/projects/03_day/harib00f/haribote.nas)
set(haribote.img_OUTS ${root_BINARY_DIR}/projects/03_day/harib00f/haribote.img)

add_custom_target(03_day_harib00f_haribote
  COMMAND ${root_BINARY_DIR}/src/opennask ${haribote.img_SRCS} ${haribote.img_OUTS}
)

add_dependencies(images 03_day_harib00f_haribote)
