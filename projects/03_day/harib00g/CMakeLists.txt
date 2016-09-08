#----------------------------------------------------------
message(STATUS "Entering directory projects/03_day/harib00g/")

########### next target ###############
set(haribote.img_SRCS ${root_SOURCE_DIR}/projects/03_day/harib00g/haribote.nas)
set(haribote.img_OUTS ${root_BINARY_DIR}/projects/03_day/harib00g/haribote.img)

add_custom_target(03_day_harib00g_haribote
  COMMAND ${root_BINARY_DIR}/src/opennask ${haribote.img_SRCS} ${haribote.img_OUTS}
)

add_dependencies(images 03_day_harib00g_haribote)

########### next target ###############
set(ipl10.img_SRCS ${root_SOURCE_DIR}/projects/03_day/harib00g/ipl10.nas)
set(ipl10.img_OUTS ${root_BINARY_DIR}/projects/03_day/harib00g/ipl10.img)

add_custom_target(03_day_harib00g_ipl10
  COMMAND ${root_BINARY_DIR}/src/opennask ${ipl10.img_SRCS} ${ipl10.img_OUTS}
)

add_dependencies(images 03_day_harib00g_ipl10)

