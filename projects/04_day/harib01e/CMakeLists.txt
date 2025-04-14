#----------------------------------------------------------
message(STATUS "Entering directory projects/04_day/harib01e/")

set(FONT ${root_BINARY_DIR}/src/makefont)
set(B2O  ${root_BINARY_DIR}/src/bin2obj)
set(CONV ${root_BINARY_DIR}/objconv/objconv)
set(04_day_harib01e_WILDOBJ ${root_BINARY_DIR}/projects/04_day/harib01e/*.o)

# Full OS build rules
set(04_day_harib01e_OS    ${root_BINARY_DIR}/projects/04_day/harib01e/os.img)
set(04_day_harib01e_SYS   ${root_BINARY_DIR}/projects/04_day/harib01e/os.sys)
set(04_day_harib01e_IPLB  ${root_BINARY_DIR}/projects/04_day/harib01e/ipl.bin)
set(04_day_harib01e_IPLS  ${root_SOURCE_DIR}/projects/04_day/harib01e/ipl10.nas)
set(04_day_harib01e_HEADB ${root_BINARY_DIR}/projects/04_day/harib01e/asmhead.bin)
set(04_day_harib01e_HEADS ${root_SOURCE_DIR}/projects/04_day/harib01e/asmhead.nas)
set(04_day_harib01e_BOOTB ${root_BINARY_DIR}/projects/04_day/harib01e/boot.bin)
set(04_day_harib01e_CCO   ${root_BINARY_DIR}/projects/04_day/harib01e/boot.o)
set(04_day_harib01e_CCS   ${root_SOURCE_DIR}/projects/04_day/harib01e/*.c)
set(04_day_harib01e_LDS   ${root_SOURCE_DIR}/projects/os.lds)
set(04_day_harib01e_FUNCS ${root_SOURCE_DIR}/projects/04_day/harib01e/naskfunc.nas)
set(04_day_harib01e_FUNCO ${root_BINARY_DIR}/projects/04_day/harib01e/naskfunc.o)

add_custom_target(04_day_harib01e_run
  COMMAND ${QEMU} ${QEMUOPT} ${04_day_harib01e_OS}
  DEPENDS 04_day_harib01e_img
)
add_custom_target(04_day_harib01e_debug
  COMMAND ${QEMU} -s -S ${QEMUOPT} ${04_day_harib01e_OS} ${QEMU_DEBUG_OPT}
  DEPENDS 04_day_harib01e_img
)
add_custom_target(04_day_harib01e_clean
  COMMAND rm -f ${04_day_harib01e_OS}
  COMMAND rm -f ${04_day_harib01e_SYS}
  COMMAND rm -f ${04_day_harib01e_IPLB}
  COMMAND rm -f ${04_day_harib01e_HEADB}
  COMMAND rm -f ${04_day_harib01e_BOOTB}
  COMMAND rm -f ${04_day_harib01e_WILDOBJ}
  COMMAND rm -f ${root_BINARY_DIR}/projects/04_day/harib01e/ipl10.bin
  COMMAND rm -f ${root_BINARY_DIR}/projects/04_day/harib01e/asmhead.bin
  COMMAND rm -f ${root_BINARY_DIR}/projects/04_day/harib01e/*_wine.bin
  COMMAND rm -f ${root_BINARY_DIR}/projects/04_day/harib01e/*.txt
)
add_custom_target(04_day_harib01e_ipl_build
  COMMAND ${GOSK_EXECUTABLE} ${04_day_harib01e_IPLS} ${04_day_harib01e_IPLB}
  DEPENDS ${04_day_harib01e_IPLS}
)
add_custom_command(
  OUTPUT ${04_day_harib01e_HEADB}
  COMMAND ${GOSK_EXECUTABLE} ${04_day_harib01e_HEADS} ${04_day_harib01e_HEADB}
  DEPENDS ${04_day_harib01e_HEADS}
  COMMENT "Generating asmhead.bin for 04_day_harib01e"
)

add_custom_target(04_day_harib01e_sys
  COMMAND ${GOSK_EXECUTABLE} ${04_day_harib01e_FUNCS} ${04_day_harib01e_FUNCO}
  COMMAND gcc ${BINOPT} -T ${04_day_harib01e_LDS} ${04_day_harib01e_CCS} ${04_day_harib01e_WILDOBJ} -o ${04_day_harib01e_BOOTB}
  COMMAND cat ${04_day_harib01e_HEADB} ${04_day_harib01e_BOOTB} > ${04_day_harib01e_SYS}
  DEPENDS ${04_day_harib01e_HEADB} 04_day_harib01e_ipl_build
  DEPENDS ${04_day_harib01e_FUNCO}
)
add_custom_target(04_day_harib01e_img
  COMMAND mformat -f 1440 -l HARIBOTEOS -N 0xffffffff -C -B ${04_day_harib01e_IPLB} -i ${04_day_harib01e_OS}
  COMMAND mcopy -i ${04_day_harib01e_OS} ${04_day_harib01e_SYS} ::
  DEPENDS 04_day_harib01e_sys
)

########### next target ###############
set(04_day_harib01e_asmhead_SRCS ${root_SOURCE_DIR}/projects/04_day/harib01e/asmhead.nas)
set(04_day_harib01e_asmhead_OUTS ${root_BINARY_DIR}/projects/04_day/harib01e/asmhead.bin)

add_custom_target(04_day_harib01e_asmhead
  COMMAND ${GOSK_EXECUTABLE} ${04_day_harib01e_asmhead_SRCS} ${04_day_harib01e_asmhead_OUTS}
  DEPENDS ${04_day_harib01e_asmhead_SRCS}
)

set(04_day_harib01e_asmhead_WINE_OUTS ${root_BINARY_DIR}/projects/04_day/harib01e/asmhead_wine.bin)
add_custom_target(04_day_harib01e_asmhead_wine
  COMMAND ${WINE} ${WINE_NASK} ${04_day_harib01e_asmhead_SRCS} ${04_day_harib01e_asmhead_WINE_OUTS}
)
add_dependencies(wine 04_day_harib01e_asmhead_wine)

add_custom_target(04_day_harib01e_asmhead_od
  COMMAND ${OD} -t x1 ${04_day_harib01e_asmhead_OUTS}      > ${04_day_harib01e_asmhead_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${04_day_harib01e_asmhead_WINE_OUTS} > ${04_day_harib01e_asmhead_WINE_OUTS}_t.txt
  COMMAND diff -s ${04_day_harib01e_asmhead_OUTS}_f.txt ${04_day_harib01e_asmhead_WINE_OUTS}_t.txt
)

add_dependencies(04_day_harib01e_asmhead_od 04_day_harib01e_asmhead)
add_dependencies(04_day_harib01e_asmhead_od 04_day_harib01e_asmhead_wine)
add_dependencies(od 04_day_harib01e_asmhead_od)

#----------------------------------------------------------
########### next target ###############
set(04_day_harib01e_ipl10_SRCS ${root_SOURCE_DIR}/projects/04_day/harib01e/ipl10.nas)
set(04_day_harib01e_ipl10_OUTS ${root_BINARY_DIR}/projects/04_day/harib01e/ipl10.bin)

add_custom_target(04_day_harib01e_ipl10
  COMMAND ${GOSK_EXECUTABLE} ${04_day_harib01e_ipl10_SRCS} ${04_day_harib01e_ipl10_OUTS}
  DEPENDS ${04_day_harib01e_ipl10_SRCS}
)

set(04_day_harib01e_ipl10_WINE_OUTS ${root_BINARY_DIR}/projects/04_day/harib01e/ipl10_wine.bin)
add_custom_target(04_day_harib01e_ipl10_wine
  COMMAND ${WINE} ${WINE_NASK} ${04_day_harib01e_ipl10_SRCS} ${04_day_harib01e_ipl10_WINE_OUTS}
)
add_dependencies(wine 04_day_harib01e_ipl10_wine)

add_custom_target(04_day_harib01e_ipl10_od
  COMMAND ${OD} -t x1 ${04_day_harib01e_ipl10_OUTS}      > ${04_day_harib01e_ipl10_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${04_day_harib01e_ipl10_WINE_OUTS} > ${04_day_harib01e_ipl10_WINE_OUTS}_t.txt
  COMMAND diff -s ${04_day_harib01e_ipl10_OUTS}_f.txt ${04_day_harib01e_ipl10_WINE_OUTS}_t.txt
)

add_dependencies(04_day_harib01e_ipl10_od 04_day_harib01e_ipl10)
add_dependencies(04_day_harib01e_ipl10_od 04_day_harib01e_ipl10_wine)
add_dependencies(od 04_day_harib01e_ipl10_od)

#----------------------------------------------------------
########### next target ###############
set(04_day_harib01e_naskfunc_SRCS ${root_SOURCE_DIR}/projects/04_day/harib01e/naskfunc.nas)
set(04_day_harib01e_naskfunc_OUTS ${root_BINARY_DIR}/projects/04_day/harib01e/naskfunc.img)

add_custom_target(04_day_harib01e_naskfunc
  COMMAND ${GOSK_EXECUTABLE} ${04_day_harib01e_naskfunc_SRCS} ${04_day_harib01e_naskfunc_OUTS}
  DEPENDS ${04_day_harib01e_naskfunc_SRCS}
)

add_dependencies(images 04_day_harib01e_naskfunc)
set(04_day_harib01e_naskfunc_WINE_OUTS ${root_BINARY_DIR}/projects/04_day/harib01e/naskfunc_wine.img)
add_custom_target(04_day_harib01e_naskfunc_wine
  COMMAND ${WINE} ${WINE_NASK} ${04_day_harib01e_naskfunc_SRCS} ${04_day_harib01e_naskfunc_WINE_OUTS}
)
add_dependencies(wine 04_day_harib01e_naskfunc_wine)

add_custom_target(04_day_harib01e_naskfunc_od
  COMMAND ${OD} -t x1 ${04_day_harib01e_naskfunc_OUTS}      > ${04_day_harib01e_naskfunc_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${04_day_harib01e_naskfunc_WINE_OUTS} > ${04_day_harib01e_naskfunc_WINE_OUTS}_t.txt
  COMMAND diff -s ${04_day_harib01e_naskfunc_OUTS}_f.txt ${04_day_harib01e_naskfunc_WINE_OUTS}_t.txt
)

add_dependencies(04_day_harib01e_naskfunc_od 04_day_harib01e_naskfunc)
add_dependencies(04_day_harib01e_naskfunc_od 04_day_harib01e_naskfunc_wine)
add_dependencies(od 04_day_harib01e_naskfunc_od)

#----------------------------------------------------------
