#----------------------------------------------------------
message(STATUS "Entering directory projects/02_day/helloos4/")

set(02_day_helloos4_SRC ${root_SOURCE_DIR}/projects/02_day/helloos4/ipl.nas)
set(02_day_helloos4_OS ${root_BINARY_DIR}/projects/02_day/helloos4/os.img)

add_custom_target(02_day_helloos4_ipl_run
  COMMAND ${QEMU} ${QEMUOPT} ${02_day_helloos4_OS}
  DEPENDS 02_day_helloos4_ipl_img
)
add_custom_target(02_day_helloos4_ipl_debug
  COMMAND ${QEMU} -s -S ${QEMUOPT} ${02_day_helloos4_OS} ${QEMU_DEBUG_OPT}
  DEPENDS 02_day_helloos4_ipl_img
)
add_custom_target(02_day_helloos4_ipl_clean
  COMMAND rm -f ${02_day_helloos4_OS}
)
add_custom_target(02_day_helloos4_ipl_img
  COMMAND ${GOSK_EXECUTABLE} ${02_day_helloos4_SRC} ${02_day_helloos4_OS}
  DEPENDS ${02_day_helloos4_SRC}
)
########### next target ###############
set(ipl.img_SRCS ${root_SOURCE_DIR}/projects/02_day/helloos4/ipl.nas)
set(ipl.img_OUTS ${root_BINARY_DIR}/projects/02_day/helloos4/ipl.img)
set(ipl_wine.img_OUTS ${root_BINARY_DIR}/projects/02_day/helloos4/ipl_wine.img)

add_custom_target(02_day_helloos4_ipl
  COMMAND ${GOSK_EXECUTABLE} ${ipl.img_SRCS} ${ipl.img_OUTS}
  DEPENDS ${ipl.img_SRCS}
)
add_custom_target(02_day_helloos4_ipl_wine
  COMMAND ${WINE} ${WINE_NASK} ${ipl.img_SRCS} ${ipl_wine.img_OUTS}
)
add_custom_target(02_day_helloos4_ipl_od
  COMMAND ${OD} -t x1 ${ipl.img_OUTS}      > ${ipl.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${ipl_wine.img_OUTS} > ${ipl_wine.img_OUTS}_t.txt
  COMMAND diff -s ${ipl.img_OUTS}_f.txt ${ipl_wine.img_OUTS}_t.txt
)

add_dependencies(02_day_helloos4_ipl_od 02_day_helloos4_ipl)
add_dependencies(02_day_helloos4_ipl_od 02_day_helloos4_ipl_wine)

add_dependencies(images 02_day_helloos4_ipl)
add_dependencies(wine 02_day_helloos4_ipl_wine)
add_dependencies(od 02_day_helloos4_ipl_od)

#----------------------------------------------------------
