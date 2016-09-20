#----------------------------------------------------------
message(STATUS "Entering directory projects/05_day/harib02b/")

########### next target ###############
set(asmhead.img_SRCS ${root_SOURCE_DIR}/projects/05_day/harib02b/asmhead.nas)
set(asmhead.img_OUTS ${root_BINARY_DIR}/projects/05_day/harib02b/asmhead.img)
set(asmhead_wine.img_OUTS ${root_BINARY_DIR}/projects/05_day/harib02b/asmhead_wine.img)

add_custom_target(05_day_harib02b_asmhead
  COMMAND ${root_BINARY_DIR}/src/opennask ${asmhead.img_SRCS} ${asmhead.img_OUTS}
)
add_custom_target(05_day_harib02b_asmhead_wine
  COMMAND ${WINE} ${WINE_NASK} ${asmhead.img_SRCS} ${asmhead_wine.img_OUTS}
)
add_custom_target(05_day_harib02b_asmhead_od
  COMMAND ${OD} -t x1 ${asmhead.img_OUTS}      > ${asmhead.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${asmhead_wine.img_OUTS} > ${asmhead_wine.img_OUTS}_t.txt
)

add_dependencies(images 05_day_harib02b_asmhead)
add_dependencies(wine 05_day_harib02b_asmhead_wine)
add_dependencies(od 05_day_harib02b_asmhead_od)

########### next target ###############
set(ipl10.img_SRCS ${root_SOURCE_DIR}/projects/05_day/harib02b/ipl10.nas)
set(ipl10.img_OUTS ${root_BINARY_DIR}/projects/05_day/harib02b/ipl10.img)
set(ipl10_wine.img_OUTS ${root_BINARY_DIR}/projects/05_day/harib02b/ipl10_wine.img)

add_custom_target(05_day_harib02b_ipl10
  COMMAND ${root_BINARY_DIR}/src/opennask ${ipl10.img_SRCS} ${ipl10.img_OUTS}
)
add_custom_target(05_day_harib02b_ipl10_wine
  COMMAND ${WINE} ${WINE_NASK} ${ipl10.img_SRCS} ${ipl10_wine.img_OUTS}
)
add_custom_target(05_day_harib02b_ipl10_od
  COMMAND ${OD} -t x1 ${ipl10.img_OUTS}      > ${ipl10.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${ipl10_wine.img_OUTS} > ${ipl10_wine.img_OUTS}_t.txt
)

add_dependencies(images 05_day_harib02b_ipl10)
add_dependencies(wine 05_day_harib02b_ipl10_wine)
add_dependencies(od 05_day_harib02b_ipl10_od)

########### next target ###############
set(naskfunc.img_SRCS ${root_SOURCE_DIR}/projects/05_day/harib02b/naskfunc.nas)
set(naskfunc.img_OUTS ${root_BINARY_DIR}/projects/05_day/harib02b.imgkfunc.img)
set(naskfunc_wine.img_OUTS ${root_BINARY_DIR}/projects/05_day/harib02b_wine.imgkfunc_wine.img)

add_custom_target(05_day_harib02bkfunc
  COMMAND ${root_BINARY_DIR}/src/opennask ${naskfunc.img_SRCS} ${naskfunc.img_OUTS}
)
add_custom_target(05_day_harib02bkfunc_wine
  COMMAND ${WINE} ${WINE_NASK} ${naskfunc.img_SRCS} ${naskfunc_wine.img_OUTS}
)
add_custom_target(05_day_harib02bkfunc_od
  COMMAND ${OD} -t x1 ${naskfunc.img_OUTS}      > ${naskfunc.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${naskfunc_wine.img_OUTS} > ${naskfunc_wine.img_OUTS}_t.txt
)

add_dependencies(images 05_day_harib02bkfunc)
add_dependencies(wine 05_day_harib02bkfunc_wine)
add_dependencies(od 05_day_harib02bkfunc_od)
