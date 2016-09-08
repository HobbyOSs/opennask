#----------------------------------------------------------
message(STATUS "Entering directory projects/03_day/harib00j/")

########### next target ###############
set(asmhead.img_SRCS ${root_SOURCE_DIR}/projects/03_day/harib00j/asmhead.nas)
set(asmhead.img_OUTS ${root_BINARY_DIR}/projects/03_day/harib00j/asmhead.img)

add_custom_target(03_day_harib00j_asmhead
  COMMAND ${root_BINARY_DIR}/src/opennask ${asmhead.img_SRCS} ${asmhead.img_OUTS}
)

add_dependencies(images 03_day_harib00j_asmhead)

########### next target ###############
set(ipl10.img_SRCS ${root_SOURCE_DIR}/projects/03_day/harib00j/ipl10.nas)
set(ipl10.img_OUTS ${root_BINARY_DIR}/projects/03_day/harib00j/ipl10.img)

add_custom_target(03_day_harib00j_ipl10
  COMMAND ${root_BINARY_DIR}/src/opennask ${ipl10.img_SRCS} ${ipl10.img_OUTS}
)

add_dependencies(images 03_day_harib00j_ipl10)

########### next target ###############
set(naskfunc.img_SRCS ${root_SOURCE_DIR}/projects/03_day/harib00j/naskfunc.nas)
set(naskfunc.img_OUTS ${root_BINARY_DIR}/projects/03_day/harib00j.imgkfunc.img)

add_custom_target(03_day_harib00jkfunc
  COMMAND ${root_BINARY_DIR}/src/opennask ${naskfunc.img_SRCS} ${naskfunc.img_OUTS}
)

add_dependencies(images 03_day_harib00jkfunc)

