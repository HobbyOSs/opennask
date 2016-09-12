#----------------------------------------------------------
message(STATUS "Entering directory projects/03_day/harib00j/")

########### next target ###############
set(asmhead.img_SRCS ${root_SOURCE_DIR}/projects/03_day/harib00j/asmhead.nas)
set(asmhead.img_OUTS ${root_BINARY_DIR}/projects/03_day/harib00j/asmhead.img)
set(asmhead_wine.img_OUTS ${root_BINARY_DIR}/projects/03_day/harib00j/asmhead_wine.img)

add_custom_target(03_day_harib00j_asmhead
  COMMAND ${root_BINARY_DIR}/src/opennask ${asmhead.img_SRCS} ${asmhead.img_OUTS}
)
add_custom_target(03_day_harib00j_asmhead_wine
  COMMAND ${WINE} ${WINE_NASK} ${asmhead.img_SRCS} ${asmhead_wine.img_OUTS}
)

add_dependencies(images 03_day_harib00j_asmhead)
add_dependencies(wine 03_day_harib00j_asmhead_wine)

########### next target ###############
set(ipl10.img_SRCS ${root_SOURCE_DIR}/projects/03_day/harib00j/ipl10.nas)
set(ipl10.img_OUTS ${root_BINARY_DIR}/projects/03_day/harib00j/ipl10.img)
set(ipl10_wine.img_OUTS ${root_BINARY_DIR}/projects/03_day/harib00j/ipl10_wine.img)

add_custom_target(03_day_harib00j_ipl10
  COMMAND ${root_BINARY_DIR}/src/opennask ${ipl10.img_SRCS} ${ipl10.img_OUTS}
)
add_custom_target(03_day_harib00j_ipl10_wine
  COMMAND ${WINE} ${WINE_NASK} ${ipl10.img_SRCS} ${ipl10_wine.img_OUTS}
)

add_dependencies(images 03_day_harib00j_ipl10)
add_dependencies(wine 03_day_harib00j_ipl10_wine)

########### next target ###############
set(naskfunc.img_SRCS ${root_SOURCE_DIR}/projects/03_day/harib00j/naskfunc.nas)
set(naskfunc.img_OUTS ${root_BINARY_DIR}/projects/03_day/harib00j.imgkfunc.img)
set(naskfunc_wine.img_OUTS ${root_BINARY_DIR}/projects/03_day/harib00j_wine.imgkfunc_wine.img)

add_custom_target(03_day_harib00jkfunc
  COMMAND ${root_BINARY_DIR}/src/opennask ${naskfunc.img_SRCS} ${naskfunc.img_OUTS}
)
add_custom_target(03_day_harib00jkfunc_wine
  COMMAND ${WINE} ${WINE_NASK} ${naskfunc.img_SRCS} ${naskfunc_wine.img_OUTS}
)

add_dependencies(images 03_day_harib00jkfunc)
add_dependencies(wine 03_day_harib00jkfunc_wine)

