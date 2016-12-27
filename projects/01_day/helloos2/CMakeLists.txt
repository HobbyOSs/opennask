#----------------------------------------------------------
message(STATUS "Entering directory projects/01_day/helloos2/")

set(NASK ${root_BINARY_DIR}/src/opennask)
set(01_day_helloos2_SRC ${root_SOURCE_DIR}/projects/01_day/helloos2/helloos.nas)
set(01_day_helloos2_OS ${root_BINARY_DIR}/projects/01_day/helloos2/os.img)

add_custom_target(01_day_helloos2_helloos_run
  COMMAND ${QEMU} ${QEMUOPT} ${01_day_helloos2_OS}
  DEPENDS 01_day_helloos2_helloos_img
)
add_custom_target(01_day_helloos2_helloos_debug
  COMMAND ${QEMU} -s -S ${QEMUOPT} ${01_day_helloos2_OS} ${QEMU_DEBUG_OPT}
  DEPENDS 01_day_helloos2_helloos_img
)
add_custom_target(01_day_helloos2_helloos_clean
  COMMAND rm -f ${01_day_helloos2_OS}
)
add_custom_target(01_day_helloos2_helloos_img
  ${root_BINARY_DIR}/src/opennask ${01_day_helloos2_SRC} ${01_day_helloos2_OS}
)
########### next target ###############
set(helloos.img_SRCS ${root_SOURCE_DIR}/projects/01_day/helloos2/helloos.nas)
set(helloos.img_OUTS ${root_BINARY_DIR}/projects/01_day/helloos2/helloos.img)
set(helloos_wine.img_OUTS ${root_BINARY_DIR}/projects/01_day/helloos2/helloos_wine.img)

add_custom_target(01_day_helloos2_helloos
  COMMAND ${root_BINARY_DIR}/src/opennask ${helloos.img_SRCS} ${helloos.img_OUTS}
)
add_custom_target(01_day_helloos2_helloos_wine
  COMMAND ${WINE} ${WINE_NASK} ${helloos.img_SRCS} ${helloos_wine.img_OUTS}
)
add_custom_target(01_day_helloos2_helloos_od
  COMMAND ${OD} -t x1 ${helloos.img_OUTS}      > ${helloos.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${helloos_wine.img_OUTS} > ${helloos_wine.img_OUTS}_t.txt
  COMMAND diff -s ${helloos.img_OUTS}_f.txt ${helloos_wine.img_OUTS}_t.txt
)

add_dependencies(01_day_helloos2_helloos_od 01_day_helloos2_helloos)
add_dependencies(01_day_helloos2_helloos_od 01_day_helloos2_helloos_wine)

add_dependencies(images 01_day_helloos2_helloos)
add_dependencies(wine 01_day_helloos2_helloos_wine)
add_dependencies(od 01_day_helloos2_helloos_od)

#----------------------------------------------------------
