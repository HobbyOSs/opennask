#----------------------------------------------------------
message(STATUS "Entering directory projects/02_day/helloos5/")

set(02_day_helloos5_SRC ${root_SOURCE_DIR}/projects/02_day/helloos5/ipl.nas)
set(02_day_helloos5_OS ${root_BINARY_DIR}/projects/02_day/helloos5/os.img)
set(FONT ${root_BINARY_DIR}/src/makefont)
set(B2O  ${root_BINARY_DIR}/src/bin2obj)

add_custom_target(02_day_helloos5_ipl_run
  COMMAND ${QEMU} ${QEMUOPT} ${02_day_helloos5_OS}
  DEPENDS 02_day_helloos5_ipl_img
)
add_custom_target(02_day_helloos5_ipl_debug
  COMMAND ${QEMU} -s -S ${QEMUOPT} ${02_day_helloos5_OS} ${QEMU_DEBUG_OPT}
  DEPENDS 02_day_helloos5_ipl_img
)
add_custom_target(02_day_helloos5_ipl_clean
  COMMAND rm -f ${02_day_helloos5_OS}
)
add_custom_target(02_day_helloos5_ipl_img
  COMMAND ${RAKUSK_EXECUTABLE} ${02_day_helloos5_SRC} ${02_day_helloos5_OS}
  DEPENDS ${02_day_helloos5_SRC}
)
########### next target ###############
set(ipl.img_SRCS ${root_SOURCE_DIR}/projects/02_day/helloos5/ipl.nas)
set(ipl.img_OUTS ${root_BINARY_DIR}/projects/02_day/helloos5/ipl.img)
set(ipl_wine.img_OUTS ${root_BINARY_DIR}/projects/02_day/helloos5/ipl_wine.img)

add_custom_target(02_day_helloos5_ipl
  COMMAND ${RAKUSK_EXECUTABLE} ${ipl.img_SRCS} ${ipl.img_OUTS}
  DEPENDS ${ipl.img_SRCS}
)
add_custom_target(02_day_helloos5_ipl_wine
  COMMAND ${WINE} ${WINE_NASK} ${ipl.img_SRCS} ${ipl_wine.img_OUTS}
)
add_custom_target(02_day_helloos5_ipl_od
  COMMAND ${OD} -t x1 ${ipl.img_OUTS}      > ${ipl.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${ipl_wine.img_OUTS} > ${ipl_wine.img_OUTS}_t.txt
  COMMAND diff -s ${ipl.img_OUTS}_f.txt ${ipl_wine.img_OUTS}_t.txt
)

add_dependencies(02_day_helloos5_ipl_od 02_day_helloos5_ipl)
add_dependencies(02_day_helloos5_ipl_od 02_day_helloos5_ipl_wine)

add_dependencies(images 02_day_helloos5_ipl)
add_dependencies(wine 02_day_helloos5_ipl_wine)
add_dependencies(od 02_day_helloos5_ipl_od)

#----------------------------------------------------------
