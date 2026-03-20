#----------------------------------------------------------
message(STATUS "Entering directory projects/03_day/harib00c/")

set(FONT ${root_BINARY_DIR}/src/makefont)
set(B2O  ${root_BINARY_DIR}/src/bin2obj)
set(CONV ${root_BINARY_DIR}/objconv/objconv)
set(03_day_harib00c_WILDOBJ ${root_BINARY_DIR}/projects/03_day/harib00c/*.o)

# IPL only build rules (ipl.nas)
set(03_day_harib00c_ipl_only_IPLS ${root_SOURCE_DIR}/projects/03_day/harib00c/ipl.nas)
set(03_day_harib00c_ipl_only_IPLB ${root_BINARY_DIR}/projects/03_day/harib00c/ipl.bin)

add_custom_target(03_day_harib00c_ipl_only
  COMMAND ${RAKUSK_EXECUTABLE} ${03_day_harib00c_ipl_only_IPLS} ${03_day_harib00c_ipl_only_IPLB}
  DEPENDS ${03_day_harib00c_ipl_only_IPLS}
)
add_custom_target(03_day_harib00c_run
  COMMAND ${QEMU} ${QEMUOPT} ${03_day_harib00c_ipl_only_IPLB}
  DEPENDS 03_day_harib00c_ipl_only
)
add_custom_target(03_day_harib00c_debug
  COMMAND ${QEMU} -s -S ${QEMUOPT} ${03_day_harib00c_ipl_only_IPLB} ${QEMU_DEBUG_OPT}
  DEPENDS 03_day_harib00c_ipl_only
)
add_custom_target(03_day_harib00c_clean
  COMMAND rm -f ${03_day_harib00c_ipl_only_IPLB}
  COMMAND rm -f ${root_BINARY_DIR}/projects/03_day/harib00c/*_wine.bin
  COMMAND rm -f ${root_BINARY_DIR}/projects/03_day/harib00c/*.txt
)
add_dependencies(images 03_day_harib00c_ipl_only)

########### next target ###############
set(03_day_harib00c_ipl_SRCS ${root_SOURCE_DIR}/projects/03_day/harib00c/ipl.nas)
set(03_day_harib00c_ipl_WINE_OUTS ${root_BINARY_DIR}/projects/03_day/harib00c/ipl_wine.bin)
add_custom_target(03_day_harib00c_ipl_wine
  COMMAND ${WINE} ${WINE_NASK} ${03_day_harib00c_ipl_SRCS} ${03_day_harib00c_ipl_WINE_OUTS}
)
add_dependencies(wine 03_day_harib00c_ipl_wine)

set(03_day_harib00c_ipl_OUTS ${root_BINARY_DIR}/projects/03_day/harib00c/ipl.bin)
add_custom_target(03_day_harib00c_ipl_od
  COMMAND ${OD} -t x1 ${03_day_harib00c_ipl_OUTS}      > ${03_day_harib00c_ipl_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${03_day_harib00c_ipl_WINE_OUTS} > ${03_day_harib00c_ipl_WINE_OUTS}_t.txt
  COMMAND diff -s ${03_day_harib00c_ipl_OUTS}_f.txt ${03_day_harib00c_ipl_WINE_OUTS}_t.txt
)

add_dependencies(03_day_harib00c_ipl_od 03_day_harib00c_ipl_only)
add_dependencies(03_day_harib00c_ipl_od 03_day_harib00c_ipl_wine)
add_dependencies(od 03_day_harib00c_ipl_od)

#----------------------------------------------------------
