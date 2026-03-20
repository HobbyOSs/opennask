#----------------------------------------------------------
message(STATUS "Entering directory projects/02_day/helloos3/")

set(02_day_helloos3_SRC ${root_SOURCE_DIR}/projects/02_day/helloos3/helloos.nas)
set(02_day_helloos3_OS ${root_BINARY_DIR}/projects/02_day/helloos3/os.img)
set(FONT ${root_BINARY_DIR}/src/makefont)
set(B2O  ${root_BINARY_DIR}/src/bin2obj)

add_custom_target(02_day_helloos3_helloos_run
  COMMAND ${QEMU} ${QEMUOPT} ${02_day_helloos3_OS}
  DEPENDS 02_day_helloos3_helloos_img
)
add_custom_target(02_day_helloos3_helloos_debug
  COMMAND ${QEMU} -s -S ${QEMUOPT} ${02_day_helloos3_OS} ${QEMU_DEBUG_OPT}
  DEPENDS 02_day_helloos3_helloos_img
)
add_custom_target(02_day_helloos3_helloos_clean
  COMMAND rm -f ${02_day_helloos3_OS}
)
add_custom_target(02_day_helloos3_helloos_img
  COMMAND ${RAKUSK_EXECUTABLE} ${02_day_helloos3_SRC} ${02_day_helloos3_OS}
  DEPENDS ${02_day_helloos3_SRC}
)
########### next target ###############
set(helloos.img_SRCS ${root_SOURCE_DIR}/projects/02_day/helloos3/helloos.nas)
set(helloos.img_OUTS ${root_BINARY_DIR}/projects/02_day/helloos3/helloos.img)
set(helloos_wine.img_OUTS ${root_BINARY_DIR}/projects/02_day/helloos3/helloos_wine.img)

add_custom_target(02_day_helloos3_helloos
  COMMAND ${RAKUSK_EXECUTABLE} ${helloos.img_SRCS} ${helloos.img_OUTS}
  DEPENDS ${helloos.img_SRCS}
)
add_custom_target(02_day_helloos3_helloos_wine
  COMMAND ${WINE} ${WINE_NASK} ${helloos.img_SRCS} ${helloos_wine.img_OUTS}
)
add_custom_target(02_day_helloos3_helloos_od
  COMMAND ${OD} -t x1 ${helloos.img_OUTS}      > ${helloos.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${helloos_wine.img_OUTS} > ${helloos_wine.img_OUTS}_t.txt
  COMMAND diff -s ${helloos.img_OUTS}_f.txt ${helloos_wine.img_OUTS}_t.txt
)

add_dependencies(02_day_helloos3_helloos_od 02_day_helloos3_helloos)
add_dependencies(02_day_helloos3_helloos_od 02_day_helloos3_helloos_wine)

add_dependencies(images 02_day_helloos3_helloos)
add_dependencies(wine 02_day_helloos3_helloos_wine)
add_dependencies(od 02_day_helloos3_helloos_od)

#----------------------------------------------------------
