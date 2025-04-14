#----------------------------------------------------------
message(STATUS "Entering directory projects/04_day/harib01g/")

set(FONT ${root_BINARY_DIR}/src/makefont)
set(B2O  ${root_BINARY_DIR}/src/bin2obj)
set(CONV ${root_BINARY_DIR}/objconv/objconv)
set(04_day_harib01g_WILDOBJ ${root_BINARY_DIR}/projects/04_day/harib01g/*.o)

# Full OS build rules
set(04_day_harib01g_OS    ${root_BINARY_DIR}/projects/04_day/harib01g/os.img)
set(04_day_harib01g_SYS   ${root_BINARY_DIR}/projects/04_day/harib01g/os.sys)
set(04_day_harib01g_IPLB  ${root_BINARY_DIR}/projects/04_day/harib01g/ipl.bin)
set(04_day_harib01g_IPLS  ${root_SOURCE_DIR}/projects/04_day/harib01g/ipl10.nas)
set(04_day_harib01g_HEADB ${root_BINARY_DIR}/projects/04_day/harib01g/asmhead.bin)
set(04_day_harib01g_HEADS ${root_SOURCE_DIR}/projects/04_day/harib01g/asmhead.nas)
set(04_day_harib01g_BOOTB ${root_BINARY_DIR}/projects/04_day/harib01g/boot.bin)
set(04_day_harib01g_CCO   ${root_BINARY_DIR}/projects/04_day/harib01g/boot.o)
set(04_day_harib01g_CCS   ${root_SOURCE_DIR}/projects/04_day/harib01g/*.c)
set(04_day_harib01g_LDS   ${root_SOURCE_DIR}/projects/os.lds)
set(04_day_harib01g_FUNCS ${root_SOURCE_DIR}/projects/04_day/harib01g/naskfunc.nas)
set(04_day_harib01g_FUNCO ${root_BINARY_DIR}/projects/04_day/harib01g/naskfunc.o)

add_custom_target(04_day_harib01g_run
  COMMAND ${QEMU} ${QEMUOPT} ${04_day_harib01g_OS}
  DEPENDS 04_day_harib01g_img
)
add_custom_target(04_day_harib01g_debug
  COMMAND ${QEMU} -s -S ${QEMUOPT} ${04_day_harib01g_OS} ${QEMU_DEBUG_OPT}
  DEPENDS 04_day_harib01g_img
)
add_custom_target(04_day_harib01g_clean
  COMMAND rm -f ${04_day_harib01g_OS}
  COMMAND rm -f ${04_day_harib01g_SYS}
  COMMAND rm -f ${04_day_harib01g_IPLB}
  COMMAND rm -f ${04_day_harib01g_HEADB}
  COMMAND rm -f ${04_day_harib01g_BOOTB}
  COMMAND rm -f ${04_day_harib01g_WILDOBJ}
  COMMAND rm -f ${root_BINARY_DIR}/projects/04_day/harib01g/ipl10.bin
  COMMAND rm -f ${root_BINARY_DIR}/projects/04_day/harib01g/asmhead.bin
  COMMAND rm -f ${root_BINARY_DIR}/projects/04_day/harib01g/*_wine.bin
  COMMAND rm -f ${root_BINARY_DIR}/projects/04_day/harib01g/*.txt
)
add_custom_target(04_day_harib01g_ipl_build
  COMMAND ${GOSK_EXECUTABLE} ${04_day_harib01g_IPLS} ${04_day_harib01g_IPLB}
  DEPENDS ${04_day_harib01g_IPLS}
)
add_custom_command(
  OUTPUT ${04_day_harib01g_HEADB}
  COMMAND ${GOSK_EXECUTABLE} ${04_day_harib01g_HEADS} ${04_day_harib01g_HEADB}
  DEPENDS ${04_day_harib01g_HEADS}
  COMMENT "Generating asmhead.bin for 04_day_harib01g"
)

add_custom_target(04_day_harib01g_sys
  COMMAND ${GOSK_EXECUTABLE} ${04_day_harib01g_FUNCS} ${04_day_harib01g_FUNCO}
  COMMAND gcc ${BINOPT} -T ${04_day_harib01g_LDS} ${04_day_harib01g_CCS} ${04_day_harib01g_WILDOBJ} -o ${04_day_harib01g_BOOTB}
  COMMAND cat ${04_day_harib01g_HEADB} ${04_day_harib01g_BOOTB} > ${04_day_harib01g_SYS}
  DEPENDS ${04_day_harib01g_HEADB} 04_day_harib01g_ipl_build
  DEPENDS ${04_day_harib01g_FUNCO}
)
add_custom_target(04_day_harib01g_img
  COMMAND mformat -f 1440 -l HARIBOTEOS -N 0xffffffff -C -B ${04_day_harib01g_IPLB} -i ${04_day_harib01g_OS}
  COMMAND mcopy -i ${04_day_harib01g_OS} ${04_day_harib01g_SYS} ::
  DEPENDS 04_day_harib01g_sys
)

########### next target ###############
set(04_day_harib01g_asmhead_SRCS ${root_SOURCE_DIR}/projects/04_day/harib01g/asmhead.nas)
set(04_day_harib01g_asmhead_OUTS ${root_BINARY_DIR}/projects/04_day/harib01g/asmhead.bin)

add_custom_target(04_day_harib01g_asmhead
  COMMAND ${GOSK_EXECUTABLE} ${04_day_harib01g_asmhead_SRCS} ${04_day_harib01g_asmhead_OUTS}
  DEPENDS ${04_day_harib01g_asmhead_SRCS}
)

set(04_day_harib01g_asmhead_WINE_OUTS ${root_BINARY_DIR}/projects/04_day/harib01g/asmhead_wine.bin)
add_custom_target(04_day_harib01g_asmhead_wine
  COMMAND ${WINE} ${WINE_NASK} ${04_day_harib01g_asmhead_SRCS} ${04_day_harib01g_asmhead_WINE_OUTS}
)
add_dependencies(wine 04_day_harib01g_asmhead_wine)

add_custom_target(04_day_harib01g_asmhead_od
  COMMAND ${OD} -t x1 ${04_day_harib01g_asmhead_OUTS}      > ${04_day_harib01g_asmhead_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${04_day_harib01g_asmhead_WINE_OUTS} > ${04_day_harib01g_asmhead_WINE_OUTS}_t.txt
  COMMAND diff -s ${04_day_harib01g_asmhead_OUTS}_f.txt ${04_day_harib01g_asmhead_WINE_OUTS}_t.txt
)

add_dependencies(04_day_harib01g_asmhead_od 04_day_harib01g_asmhead)
add_dependencies(04_day_harib01g_asmhead_od 04_day_harib01g_asmhead_wine)
add_dependencies(od 04_day_harib01g_asmhead_od)

#----------------------------------------------------------
########### next target ###############
set(04_day_harib01g_ipl10_SRCS ${root_SOURCE_DIR}/projects/04_day/harib01g/ipl10.nas)
set(04_day_harib01g_ipl10_OUTS ${root_BINARY_DIR}/projects/04_day/harib01g/ipl10.bin)

add_custom_target(04_day_harib01g_ipl10
  COMMAND ${GOSK_EXECUTABLE} ${04_day_harib01g_ipl10_SRCS} ${04_day_harib01g_ipl10_OUTS}
  DEPENDS ${04_day_harib01g_ipl10_SRCS}
)

set(04_day_harib01g_ipl10_WINE_OUTS ${root_BINARY_DIR}/projects/04_day/harib01g/ipl10_wine.bin)
add_custom_target(04_day_harib01g_ipl10_wine
  COMMAND ${WINE} ${WINE_NASK} ${04_day_harib01g_ipl10_SRCS} ${04_day_harib01g_ipl10_WINE_OUTS}
)
add_dependencies(wine 04_day_harib01g_ipl10_wine)

add_custom_target(04_day_harib01g_ipl10_od
  COMMAND ${OD} -t x1 ${04_day_harib01g_ipl10_OUTS}      > ${04_day_harib01g_ipl10_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${04_day_harib01g_ipl10_WINE_OUTS} > ${04_day_harib01g_ipl10_WINE_OUTS}_t.txt
  COMMAND diff -s ${04_day_harib01g_ipl10_OUTS}_f.txt ${04_day_harib01g_ipl10_WINE_OUTS}_t.txt
)

add_dependencies(04_day_harib01g_ipl10_od 04_day_harib01g_ipl10)
add_dependencies(04_day_harib01g_ipl10_od 04_day_harib01g_ipl10_wine)
add_dependencies(od 04_day_harib01g_ipl10_od)

#----------------------------------------------------------
########### next target ###############
set(04_day_harib01g_naskfunc_SRCS ${root_SOURCE_DIR}/projects/04_day/harib01g/naskfunc.nas)
set(04_day_harib01g_naskfunc_OUTS ${root_BINARY_DIR}/projects/04_day/harib01g/naskfunc.img)

add_custom_target(04_day_harib01g_naskfunc
  COMMAND ${GOSK_EXECUTABLE} ${04_day_harib01g_naskfunc_SRCS} ${04_day_harib01g_naskfunc_OUTS}
  DEPENDS ${04_day_harib01g_naskfunc_SRCS}
)

add_dependencies(images 04_day_harib01g_naskfunc)
set(04_day_harib01g_naskfunc_WINE_OUTS ${root_BINARY_DIR}/projects/04_day/harib01g/naskfunc_wine.img)
add_custom_target(04_day_harib01g_naskfunc_wine
  COMMAND ${WINE} ${WINE_NASK} ${04_day_harib01g_naskfunc_SRCS} ${04_day_harib01g_naskfunc_WINE_OUTS}
)
add_dependencies(wine 04_day_harib01g_naskfunc_wine)

add_custom_target(04_day_harib01g_naskfunc_od
  COMMAND ${OD} -t x1 ${04_day_harib01g_naskfunc_OUTS}      > ${04_day_harib01g_naskfunc_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${04_day_harib01g_naskfunc_WINE_OUTS} > ${04_day_harib01g_naskfunc_WINE_OUTS}_t.txt
  COMMAND diff -s ${04_day_harib01g_naskfunc_OUTS}_f.txt ${04_day_harib01g_naskfunc_WINE_OUTS}_t.txt
)

add_dependencies(04_day_harib01g_naskfunc_od 04_day_harib01g_naskfunc)
add_dependencies(04_day_harib01g_naskfunc_od 04_day_harib01g_naskfunc_wine)
add_dependencies(od 04_day_harib01g_naskfunc_od)

#----------------------------------------------------------
