#----------------------------------------------------------
message(STATUS "Entering directory projects/08_day/harib05a/")

########### next target ###############
set(asmhead.img_SRCS ${root_SOURCE_DIR}/projects/08_day/harib05a/asmhead.nas)
set(asmhead.img_OUTS ${root_BINARY_DIR}/projects/08_day/harib05a/asmhead.img)
set(asmhead_wine.img_OUTS ${root_BINARY_DIR}/projects/08_day/harib05a/asmhead_wine.img)

add_custom_target(08_day_harib05a_asmhead
  COMMAND ${root_BINARY_DIR}/src/opennask ${asmhead.img_SRCS} ${asmhead.img_OUTS}
)
add_custom_target(08_day_harib05a_asmhead_wine
  COMMAND ${WINE} ${WINE_NASK} ${asmhead.img_SRCS} ${asmhead_wine.img_OUTS}
)
add_custom_target(08_day_harib05a_asmhead_od
  COMMAND ${OD} -t x1 ${asmhead.img_OUTS}      > ${asmhead.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${asmhead_wine.img_OUTS} > ${asmhead_wine.img_OUTS}_t.txt
)

add_dependencies(images 08_day_harib05a_asmhead)
add_dependencies(wine 08_day_harib05a_asmhead_wine)
add_dependencies(od 08_day_harib05a_asmhead_od)

#----------------------------------------------------------
set(08_day_harib05a_OS ${root_BINARY_DIR}/projects/08_day/harib05a/os.img)

add_custom_target(08_day_harib05a_run
  COMMAND ${QEMU} ${QEMUOPT} ${root_BINARY_DIR}/projects/08_day/harib05a/os.img
)

########### next target ###############
set(ipl10.img_SRCS ${root_SOURCE_DIR}/projects/08_day/harib05a/ipl10.nas)
set(ipl10.img_OUTS ${root_BINARY_DIR}/projects/08_day/harib05a/ipl10.img)
set(ipl10_wine.img_OUTS ${root_BINARY_DIR}/projects/08_day/harib05a/ipl10_wine.img)

add_custom_target(08_day_harib05a_ipl10
  COMMAND ${root_BINARY_DIR}/src/opennask ${ipl10.img_SRCS} ${ipl10.img_OUTS}
)
add_custom_target(08_day_harib05a_ipl10_wine
  COMMAND ${WINE} ${WINE_NASK} ${ipl10.img_SRCS} ${ipl10_wine.img_OUTS}
)
add_custom_target(08_day_harib05a_ipl10_od
  COMMAND ${OD} -t x1 ${ipl10.img_OUTS}      > ${ipl10.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${ipl10_wine.img_OUTS} > ${ipl10_wine.img_OUTS}_t.txt
)

add_dependencies(images 08_day_harib05a_ipl10)
add_dependencies(wine 08_day_harib05a_ipl10_wine)
add_dependencies(od 08_day_harib05a_ipl10_od)

########### next target ###############
set(naskfunc.img_SRCS ${root_SOURCE_DIR}/projects/08_day/harib05a/naskfunc.nas)
set(naskfunc.img_OUTS ${root_BINARY_DIR}/projects/08_day/harib05a.imgkfunc.img)
set(naskfunc_wine.img_OUTS ${root_BINARY_DIR}/projects/08_day/harib05a_wine.imgkfunc_wine.img)

add_custom_target(08_day_harib05akfunc
  COMMAND ${root_BINARY_DIR}/src/opennask ${naskfunc.img_SRCS} ${naskfunc.img_OUTS}
)
add_custom_target(08_day_harib05akfunc_wine
  COMMAND ${WINE} ${WINE_NASK} ${naskfunc.img_SRCS} ${naskfunc_wine.img_OUTS}
)
add_custom_target(08_day_harib05akfunc_od
  COMMAND ${OD} -t x1 ${naskfunc.img_OUTS}      > ${naskfunc.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${naskfunc_wine.img_OUTS} > ${naskfunc_wine.img_OUTS}_t.txt
)

add_dependencies(images 08_day_harib05akfunc)
add_dependencies(wine 08_day_harib05akfunc_wine)
add_dependencies(od 08_day_harib05akfunc_od)

