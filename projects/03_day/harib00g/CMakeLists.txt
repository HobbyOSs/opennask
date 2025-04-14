#----------------------------------------------------------
message(STATUS "Entering directory projects/03_day/harib00g/")

set(FONT ${root_BINARY_DIR}/src/makefont)
set(B2O  ${root_BINARY_DIR}/src/bin2obj)
set(CONV ${root_BINARY_DIR}/objconv/objconv)
set(03_day_harib00g_WILDOBJ ${root_BINARY_DIR}/projects/03_day/harib00g/*.o)

# IPL only build rules (ipl10.nas)
set(03_day_harib00g_ipl10_only_IPLS ${root_SOURCE_DIR}/projects/03_day/harib00g/ipl10.nas)
set(03_day_harib00g_ipl10_only_IPLB ${root_BINARY_DIR}/projects/03_day/harib00g/ipl10.bin)

add_custom_target(03_day_harib00g_ipl10_only
  COMMAND ${GOSK_EXECUTABLE} ${03_day_harib00g_ipl10_only_IPLS} ${03_day_harib00g_ipl10_only_IPLB}
  DEPENDS ${03_day_harib00g_ipl10_only_IPLS}
)
add_custom_target(03_day_harib00g_run
  COMMAND ${QEMU} ${QEMUOPT} ${03_day_harib00g_ipl10_only_IPLB}
  DEPENDS 03_day_harib00g_ipl10_only
)
add_custom_target(03_day_harib00g_debug
  COMMAND ${QEMU} -s -S ${QEMUOPT} ${03_day_harib00g_ipl10_only_IPLB} ${QEMU_DEBUG_OPT}
  DEPENDS 03_day_harib00g_ipl10_only
)
add_custom_target(03_day_harib00g_clean
  COMMAND rm -f ${03_day_harib00g_ipl10_only_IPLB}
  COMMAND rm -f ${root_BINARY_DIR}/projects/03_day/harib00g/*_wine.bin
  COMMAND rm -f ${root_BINARY_DIR}/projects/03_day/harib00g/*.txt
)
add_dependencies(images 03_day_harib00g_ipl10_only)

########### next target ###############
set(03_day_harib00g_haribote_SRCS ${root_SOURCE_DIR}/projects/03_day/harib00g/haribote.nas)
set(03_day_harib00g_haribote_OUTS ${root_BINARY_DIR}/projects/03_day/harib00g/haribote.img)

add_custom_target(03_day_harib00g_haribote
  COMMAND ${GOSK_EXECUTABLE} ${03_day_harib00g_haribote_SRCS} ${03_day_harib00g_haribote_OUTS}
  DEPENDS ${03_day_harib00g_haribote_SRCS}
)

add_dependencies(images 03_day_harib00g_haribote)
set(03_day_harib00g_haribote_WINE_OUTS ${root_BINARY_DIR}/projects/03_day/harib00g/haribote_wine.img)
add_custom_target(03_day_harib00g_haribote_wine
  COMMAND ${WINE} ${WINE_NASK} ${03_day_harib00g_haribote_SRCS} ${03_day_harib00g_haribote_WINE_OUTS}
)
add_dependencies(wine 03_day_harib00g_haribote_wine)

add_custom_target(03_day_harib00g_haribote_od
  COMMAND ${OD} -t x1 ${03_day_harib00g_haribote_OUTS}      > ${03_day_harib00g_haribote_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${03_day_harib00g_haribote_WINE_OUTS} > ${03_day_harib00g_haribote_WINE_OUTS}_t.txt
  COMMAND diff -s ${03_day_harib00g_haribote_OUTS}_f.txt ${03_day_harib00g_haribote_WINE_OUTS}_t.txt
)

add_dependencies(03_day_harib00g_haribote_od 03_day_harib00g_haribote)
add_dependencies(03_day_harib00g_haribote_od 03_day_harib00g_haribote_wine)
add_dependencies(od 03_day_harib00g_haribote_od)

#----------------------------------------------------------
########### next target ###############
set(03_day_harib00g_ipl10_SRCS ${root_SOURCE_DIR}/projects/03_day/harib00g/ipl10.nas)
set(03_day_harib00g_ipl10_WINE_OUTS ${root_BINARY_DIR}/projects/03_day/harib00g/ipl10_wine.bin)
add_custom_target(03_day_harib00g_ipl10_wine
  COMMAND ${WINE} ${WINE_NASK} ${03_day_harib00g_ipl10_SRCS} ${03_day_harib00g_ipl10_WINE_OUTS}
)
add_dependencies(wine 03_day_harib00g_ipl10_wine)

set(03_day_harib00g_ipl10_OUTS ${root_BINARY_DIR}/projects/03_day/harib00g/ipl10.bin)
add_custom_target(03_day_harib00g_ipl10_od
  COMMAND ${OD} -t x1 ${03_day_harib00g_ipl10_OUTS}      > ${03_day_harib00g_ipl10_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${03_day_harib00g_ipl10_WINE_OUTS} > ${03_day_harib00g_ipl10_WINE_OUTS}_t.txt
  COMMAND diff -s ${03_day_harib00g_ipl10_OUTS}_f.txt ${03_day_harib00g_ipl10_WINE_OUTS}_t.txt
)

add_dependencies(03_day_harib00g_ipl10_od 03_day_harib00g_ipl10_only)
add_dependencies(03_day_harib00g_ipl10_od 03_day_harib00g_ipl10_wine)
add_dependencies(od 03_day_harib00g_ipl10_od)

#----------------------------------------------------------
