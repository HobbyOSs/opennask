#----------------------------------------------------------
message(STATUS "Entering directory projects/04_day/harib01d/")

set(FONT ${RAKUSK_EXECUTABLE} --makefont)
set(B2O  ${RAKUSK_EXECUTABLE} --bin2obj)
set(CONV ${root_BINARY_DIR}/objconv/objconv)
set(04_day_harib01d_WILDOBJ ${root_BINARY_DIR}/projects/04_day/harib01d/*.o)

# Full OS build rules
set(04_day_harib01d_OS    ${root_BINARY_DIR}/projects/04_day/harib01d/os.img)
set(04_day_harib01d_SYS   ${root_BINARY_DIR}/projects/04_day/harib01d/os.sys)
set(04_day_harib01d_IPLB  ${root_BINARY_DIR}/projects/04_day/harib01d/ipl.bin)
set(04_day_harib01d_IPLS  ${root_SOURCE_DIR}/projects/04_day/harib01d/ipl10.nas)
set(04_day_harib01d_HEADB ${root_BINARY_DIR}/projects/04_day/harib01d/asmhead.bin)
set(04_day_harib01d_HEADS ${root_SOURCE_DIR}/projects/04_day/harib01d/asmhead.nas)
set(04_day_harib01d_BOOTB ${root_BINARY_DIR}/projects/04_day/harib01d/boot.bin)
set(04_day_harib01d_CCO   ${root_BINARY_DIR}/projects/04_day/harib01d/boot.o)
set(04_day_harib01d_CCS   ${root_SOURCE_DIR}/projects/04_day/harib01d/*.c)
set(04_day_harib01d_LDS   ${root_SOURCE_DIR}/projects/os.lds)
set(04_day_harib01d_FUNCS ${root_SOURCE_DIR}/projects/04_day/harib01d/naskfunc.nas)
set(04_day_harib01d_FUNCO ${root_BINARY_DIR}/projects/04_day/harib01d/naskfunc.o)

add_custom_target(04_day_harib01d_run
  COMMAND ${QEMU} ${QEMUOPT} ${04_day_harib01d_OS}
  DEPENDS 04_day_harib01d_img
)
add_custom_target(04_day_harib01d_debug
  COMMAND ${QEMU} -s -S ${QEMUOPT} ${04_day_harib01d_OS} ${QEMU_DEBUG_OPT}
  DEPENDS 04_day_harib01d_img
)
add_custom_target(04_day_harib01d_clean
  COMMAND rm -f ${04_day_harib01d_OS}
  COMMAND rm -f ${04_day_harib01d_SYS}
  COMMAND rm -f ${04_day_harib01d_IPLB}
  COMMAND rm -f ${04_day_harib01d_HEADB}
  COMMAND rm -f ${04_day_harib01d_BOOTB}
  COMMAND rm -f ${04_day_harib01d_WILDOBJ}
  COMMAND rm -f ${root_BINARY_DIR}/projects/04_day/harib01d/ipl10.bin
  COMMAND rm -f ${root_BINARY_DIR}/projects/04_day/harib01d/asmhead.bin
  COMMAND rm -f ${root_BINARY_DIR}/projects/04_day/harib01d/*_wine.bin
  COMMAND rm -f ${root_BINARY_DIR}/projects/04_day/harib01d/*.txt
)
add_custom_target(04_day_harib01d_ipl_build
  COMMAND ${RAKUSK_EXECUTABLE} ${04_day_harib01d_IPLS} ${04_day_harib01d_IPLB}
  DEPENDS ${04_day_harib01d_IPLS}
)
add_custom_command(
  OUTPUT ${04_day_harib01d_HEADB}
  COMMAND ${RAKUSK_EXECUTABLE} ${04_day_harib01d_HEADS} ${04_day_harib01d_HEADB}
  DEPENDS ${04_day_harib01d_HEADS}
  COMMENT "Generating asmhead.bin for 04_day_harib01d"
)

add_custom_target(04_day_harib01d_sys
  COMMAND ${RAKUSK_EXECUTABLE} ${04_day_harib01d_FUNCS} ${04_day_harib01d_FUNCO}
  COMMAND gcc ${BINOPT} -T ${04_day_harib01d_LDS} ${04_day_harib01d_CCS} ${04_day_harib01d_WILDOBJ} -o ${04_day_harib01d_BOOTB}
  COMMAND cat ${04_day_harib01d_HEADB} ${04_day_harib01d_BOOTB} > ${04_day_harib01d_SYS}
  DEPENDS ${04_day_harib01d_HEADB} 04_day_harib01d_ipl_build
  DEPENDS ${04_day_harib01d_FUNCO}
)
add_custom_target(04_day_harib01d_img
  COMMAND mformat -f 1440 -l HARIBOTEOS -N 0xffffffff -C -B ${04_day_harib01d_IPLB} -i ${04_day_harib01d_OS}
  COMMAND mcopy -i ${04_day_harib01d_OS} ${04_day_harib01d_SYS} ::
  DEPENDS 04_day_harib01d_sys
)

########### next target ###############
set(04_day_harib01d_asmhead_SRCS ${root_SOURCE_DIR}/projects/04_day/harib01d/asmhead.nas)
set(04_day_harib01d_asmhead_OUTS ${root_BINARY_DIR}/projects/04_day/harib01d/asmhead.bin)

add_custom_target(04_day_harib01d_asmhead
  COMMAND ${RAKUSK_EXECUTABLE} ${04_day_harib01d_asmhead_SRCS} ${04_day_harib01d_asmhead_OUTS}
  DEPENDS ${04_day_harib01d_asmhead_SRCS}
)

set(04_day_harib01d_asmhead_WINE_OUTS ${root_BINARY_DIR}/projects/04_day/harib01d/asmhead_wine.bin)
add_custom_target(04_day_harib01d_asmhead_wine
  COMMAND ${WINE} ${WINE_NASK} ${04_day_harib01d_asmhead_SRCS} ${04_day_harib01d_asmhead_WINE_OUTS}
)
add_dependencies(wine 04_day_harib01d_asmhead_wine)

add_custom_target(04_day_harib01d_asmhead_od
  COMMAND ${OD} -t x1 ${04_day_harib01d_asmhead_OUTS}      > ${04_day_harib01d_asmhead_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${04_day_harib01d_asmhead_WINE_OUTS} > ${04_day_harib01d_asmhead_WINE_OUTS}_t.txt
  COMMAND diff -s ${04_day_harib01d_asmhead_OUTS}_f.txt ${04_day_harib01d_asmhead_WINE_OUTS}_t.txt
)

add_dependencies(04_day_harib01d_asmhead_od 04_day_harib01d_asmhead)
add_dependencies(04_day_harib01d_asmhead_od 04_day_harib01d_asmhead_wine)
add_dependencies(od 04_day_harib01d_asmhead_od)

#----------------------------------------------------------
########### next target ###############
set(04_day_harib01d_ipl10_SRCS ${root_SOURCE_DIR}/projects/04_day/harib01d/ipl10.nas)
set(04_day_harib01d_ipl10_OUTS ${root_BINARY_DIR}/projects/04_day/harib01d/ipl10.bin)

add_custom_target(04_day_harib01d_ipl10
  COMMAND ${RAKUSK_EXECUTABLE} ${04_day_harib01d_ipl10_SRCS} ${04_day_harib01d_ipl10_OUTS}
  DEPENDS ${04_day_harib01d_ipl10_SRCS}
)

set(04_day_harib01d_ipl10_WINE_OUTS ${root_BINARY_DIR}/projects/04_day/harib01d/ipl10_wine.bin)
add_custom_target(04_day_harib01d_ipl10_wine
  COMMAND ${WINE} ${WINE_NASK} ${04_day_harib01d_ipl10_SRCS} ${04_day_harib01d_ipl10_WINE_OUTS}
)
add_dependencies(wine 04_day_harib01d_ipl10_wine)

add_custom_target(04_day_harib01d_ipl10_od
  COMMAND ${OD} -t x1 ${04_day_harib01d_ipl10_OUTS}      > ${04_day_harib01d_ipl10_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${04_day_harib01d_ipl10_WINE_OUTS} > ${04_day_harib01d_ipl10_WINE_OUTS}_t.txt
  COMMAND diff -s ${04_day_harib01d_ipl10_OUTS}_f.txt ${04_day_harib01d_ipl10_WINE_OUTS}_t.txt
)

add_dependencies(04_day_harib01d_ipl10_od 04_day_harib01d_ipl10)
add_dependencies(04_day_harib01d_ipl10_od 04_day_harib01d_ipl10_wine)
add_dependencies(od 04_day_harib01d_ipl10_od)

#----------------------------------------------------------
########### next target ###############
set(04_day_harib01d_naskfunc_SRCS ${root_SOURCE_DIR}/projects/04_day/harib01d/naskfunc.nas)
set(04_day_harib01d_naskfunc_OUTS ${root_BINARY_DIR}/projects/04_day/harib01d/naskfunc.img)

add_custom_target(04_day_harib01d_naskfunc
  COMMAND ${RAKUSK_EXECUTABLE} ${04_day_harib01d_naskfunc_SRCS} ${04_day_harib01d_naskfunc_OUTS}
  DEPENDS ${04_day_harib01d_naskfunc_SRCS}
)

add_dependencies(images 04_day_harib01d_naskfunc)
set(04_day_harib01d_naskfunc_WINE_OUTS ${root_BINARY_DIR}/projects/04_day/harib01d/naskfunc_wine.img)
add_custom_target(04_day_harib01d_naskfunc_wine
  COMMAND ${WINE} ${WINE_NASK} ${04_day_harib01d_naskfunc_SRCS} ${04_day_harib01d_naskfunc_WINE_OUTS}
)
add_dependencies(wine 04_day_harib01d_naskfunc_wine)

add_custom_target(04_day_harib01d_naskfunc_od
  COMMAND ${OD} -t x1 ${04_day_harib01d_naskfunc_OUTS}      > ${04_day_harib01d_naskfunc_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${04_day_harib01d_naskfunc_WINE_OUTS} > ${04_day_harib01d_naskfunc_WINE_OUTS}_t.txt
  COMMAND diff -s ${04_day_harib01d_naskfunc_OUTS}_f.txt ${04_day_harib01d_naskfunc_WINE_OUTS}_t.txt
)

add_dependencies(04_day_harib01d_naskfunc_od 04_day_harib01d_naskfunc)
add_dependencies(04_day_harib01d_naskfunc_od 04_day_harib01d_naskfunc_wine)
add_dependencies(od 04_day_harib01d_naskfunc_od)

#----------------------------------------------------------
