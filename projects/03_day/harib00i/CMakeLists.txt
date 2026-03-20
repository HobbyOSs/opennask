#----------------------------------------------------------
message(STATUS "Entering directory projects/03_day/harib00i/")

set(FONT ${RAKUSK_EXECUTABLE} --makefont)
set(B2O  ${RAKUSK_EXECUTABLE} --bin2obj)
set(CONV ${root_BINARY_DIR}/objconv/objconv)
set(03_day_harib00i_WILDOBJ ${root_BINARY_DIR}/projects/03_day/harib00i/*.o)

# Full OS build rules
set(03_day_harib00i_OS    ${root_BINARY_DIR}/projects/03_day/harib00i/os.img)
set(03_day_harib00i_SYS   ${root_BINARY_DIR}/projects/03_day/harib00i/os.sys)
set(03_day_harib00i_IPLB  ${root_BINARY_DIR}/projects/03_day/harib00i/ipl.bin)
set(03_day_harib00i_IPLS  ${root_SOURCE_DIR}/projects/03_day/harib00i/ipl10.nas)
set(03_day_harib00i_HEADB ${root_BINARY_DIR}/projects/03_day/harib00i/asmhead.bin)
set(03_day_harib00i_HEADS ${root_SOURCE_DIR}/projects/03_day/harib00i/asmhead.nas)
set(03_day_harib00i_BOOTB ${root_BINARY_DIR}/projects/03_day/harib00i/boot.bin)
set(03_day_harib00i_CCO   ${root_BINARY_DIR}/projects/03_day/harib00i/boot.o)
set(03_day_harib00i_CCS   ${root_SOURCE_DIR}/projects/03_day/harib00i/*.c)
set(03_day_harib00i_LDS   ${root_SOURCE_DIR}/projects/os.lds)

add_custom_target(03_day_harib00i_run
  COMMAND ${QEMU} ${QEMUOPT} ${03_day_harib00i_OS}
  DEPENDS 03_day_harib00i_img
)
add_custom_target(03_day_harib00i_debug
  COMMAND ${QEMU} -s -S ${QEMUOPT} ${03_day_harib00i_OS} ${QEMU_DEBUG_OPT}
  DEPENDS 03_day_harib00i_img
)
add_custom_target(03_day_harib00i_clean
  COMMAND rm -f ${03_day_harib00i_OS}
  COMMAND rm -f ${03_day_harib00i_SYS}
  COMMAND rm -f ${03_day_harib00i_IPLB}
  COMMAND rm -f ${03_day_harib00i_HEADB}
  COMMAND rm -f ${03_day_harib00i_BOOTB}
  COMMAND rm -f ${03_day_harib00i_WILDOBJ}
  COMMAND rm -f ${root_BINARY_DIR}/projects/03_day/harib00i/ipl10.bin
  COMMAND rm -f ${root_BINARY_DIR}/projects/03_day/harib00i/asmhead.bin
  COMMAND rm -f ${root_BINARY_DIR}/projects/03_day/harib00i/*_wine.bin
  COMMAND rm -f ${root_BINARY_DIR}/projects/03_day/harib00i/*.txt
)
add_custom_target(03_day_harib00i_ipl_build
  COMMAND ${RAKUSK_EXECUTABLE} ${03_day_harib00i_IPLS} ${03_day_harib00i_IPLB}
  DEPENDS ${03_day_harib00i_IPLS}
)
add_custom_command(
  OUTPUT ${03_day_harib00i_HEADB}
  COMMAND ${RAKUSK_EXECUTABLE} ${03_day_harib00i_HEADS} ${03_day_harib00i_HEADB}
  DEPENDS ${03_day_harib00i_HEADS}
  COMMENT "Generating asmhead.bin for 03_day_harib00i"
)

add_custom_target(03_day_harib00i_sys
  COMMAND gcc ${BINOPT} -T ${03_day_harib00i_LDS} ${03_day_harib00i_CCS} -o ${03_day_harib00i_BOOTB}
  COMMAND cat ${03_day_harib00i_HEADB} ${03_day_harib00i_BOOTB} > ${03_day_harib00i_SYS}
  DEPENDS ${03_day_harib00i_HEADB} 03_day_harib00i_ipl_build
)
add_custom_target(03_day_harib00i_img
  COMMAND mformat -f 1440 -l HARIBOTEOS -N 0xffffffff -C -B ${03_day_harib00i_IPLB} -i ${03_day_harib00i_OS}
  COMMAND mcopy -i ${03_day_harib00i_OS} ${03_day_harib00i_SYS} ::
  DEPENDS 03_day_harib00i_sys
)

########### next target ###############
set(03_day_harib00i_asmhead_SRCS ${root_SOURCE_DIR}/projects/03_day/harib00i/asmhead.nas)
set(03_day_harib00i_asmhead_OUTS ${root_BINARY_DIR}/projects/03_day/harib00i/asmhead.bin)

add_custom_target(03_day_harib00i_asmhead
  COMMAND ${RAKUSK_EXECUTABLE} ${03_day_harib00i_asmhead_SRCS} ${03_day_harib00i_asmhead_OUTS}
  DEPENDS ${03_day_harib00i_asmhead_SRCS}
)

set(03_day_harib00i_asmhead_WINE_OUTS ${root_BINARY_DIR}/projects/03_day/harib00i/asmhead_wine.bin)
add_custom_target(03_day_harib00i_asmhead_wine
  COMMAND ${WINE} ${WINE_NASK} ${03_day_harib00i_asmhead_SRCS} ${03_day_harib00i_asmhead_WINE_OUTS}
)
add_dependencies(wine 03_day_harib00i_asmhead_wine)

add_custom_target(03_day_harib00i_asmhead_od
  COMMAND ${OD} -t x1 ${03_day_harib00i_asmhead_OUTS}      > ${03_day_harib00i_asmhead_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${03_day_harib00i_asmhead_WINE_OUTS} > ${03_day_harib00i_asmhead_WINE_OUTS}_t.txt
  COMMAND diff -s ${03_day_harib00i_asmhead_OUTS}_f.txt ${03_day_harib00i_asmhead_WINE_OUTS}_t.txt
)

add_dependencies(03_day_harib00i_asmhead_od 03_day_harib00i_asmhead)
add_dependencies(03_day_harib00i_asmhead_od 03_day_harib00i_asmhead_wine)
add_dependencies(od 03_day_harib00i_asmhead_od)

#----------------------------------------------------------
########### next target ###############
set(03_day_harib00i_ipl10_SRCS ${root_SOURCE_DIR}/projects/03_day/harib00i/ipl10.nas)
set(03_day_harib00i_ipl10_OUTS ${root_BINARY_DIR}/projects/03_day/harib00i/ipl10.bin)

add_custom_target(03_day_harib00i_ipl10
  COMMAND ${RAKUSK_EXECUTABLE} ${03_day_harib00i_ipl10_SRCS} ${03_day_harib00i_ipl10_OUTS}
  DEPENDS ${03_day_harib00i_ipl10_SRCS}
)

set(03_day_harib00i_ipl10_WINE_OUTS ${root_BINARY_DIR}/projects/03_day/harib00i/ipl10_wine.bin)
add_custom_target(03_day_harib00i_ipl10_wine
  COMMAND ${WINE} ${WINE_NASK} ${03_day_harib00i_ipl10_SRCS} ${03_day_harib00i_ipl10_WINE_OUTS}
)
add_dependencies(wine 03_day_harib00i_ipl10_wine)

add_custom_target(03_day_harib00i_ipl10_od
  COMMAND ${OD} -t x1 ${03_day_harib00i_ipl10_OUTS}      > ${03_day_harib00i_ipl10_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${03_day_harib00i_ipl10_WINE_OUTS} > ${03_day_harib00i_ipl10_WINE_OUTS}_t.txt
  COMMAND diff -s ${03_day_harib00i_ipl10_OUTS}_f.txt ${03_day_harib00i_ipl10_WINE_OUTS}_t.txt
)

add_dependencies(03_day_harib00i_ipl10_od 03_day_harib00i_ipl10)
add_dependencies(03_day_harib00i_ipl10_od 03_day_harib00i_ipl10_wine)
add_dependencies(od 03_day_harib00i_ipl10_od)

#----------------------------------------------------------
