#----------------------------------------------------------
message(STATUS "Entering directory projects/03_day/harib00j/")

set(FONT ${root_BINARY_DIR}/src/makefont)
set(B2O  ${root_BINARY_DIR}/src/bin2obj)
set(CONV ${root_BINARY_DIR}/objconv/objconv)
set(03_day_harib00j_WILDOBJ ${root_BINARY_DIR}/projects/03_day/harib00j/*.o)

# Full OS build rules
set(03_day_harib00j_OS    ${root_BINARY_DIR}/projects/03_day/harib00j/os.img)
set(03_day_harib00j_SYS   ${root_BINARY_DIR}/projects/03_day/harib00j/os.sys)
set(03_day_harib00j_IPLB  ${root_BINARY_DIR}/projects/03_day/harib00j/ipl.bin)
set(03_day_harib00j_IPLS  ${root_SOURCE_DIR}/projects/03_day/harib00j/ipl10.nas)
set(03_day_harib00j_HEADB ${root_BINARY_DIR}/projects/03_day/harib00j/asmhead.bin)
set(03_day_harib00j_HEADS ${root_SOURCE_DIR}/projects/03_day/harib00j/asmhead.nas)
set(03_day_harib00j_BOOTB ${root_BINARY_DIR}/projects/03_day/harib00j/boot.bin)
set(03_day_harib00j_CCO   ${root_BINARY_DIR}/projects/03_day/harib00j/boot.o)
set(03_day_harib00j_CCS   ${root_SOURCE_DIR}/projects/03_day/harib00j/*.c)
set(03_day_harib00j_LDS   ${root_SOURCE_DIR}/projects/os.lds)
set(03_day_harib00j_FUNCS ${root_SOURCE_DIR}/projects/03_day/harib00j/naskfunc.nas)
set(03_day_harib00j_FUNCO ${root_BINARY_DIR}/projects/03_day/harib00j/naskfunc.o)

add_custom_target(03_day_harib00j_run
  COMMAND ${QEMU} ${QEMUOPT} ${03_day_harib00j_OS}
  DEPENDS 03_day_harib00j_img
)
add_custom_target(03_day_harib00j_debug
  COMMAND ${QEMU} -s -S ${QEMUOPT} ${03_day_harib00j_OS} ${QEMU_DEBUG_OPT}
  DEPENDS 03_day_harib00j_img
)
add_custom_target(03_day_harib00j_clean
  COMMAND rm -f ${03_day_harib00j_OS}
  COMMAND rm -f ${03_day_harib00j_SYS}
  COMMAND rm -f ${03_day_harib00j_IPLB}
  COMMAND rm -f ${03_day_harib00j_HEADB}
  COMMAND rm -f ${03_day_harib00j_BOOTB}
  COMMAND rm -f ${03_day_harib00j_WILDOBJ}
  COMMAND rm -f ${root_BINARY_DIR}/projects/03_day/harib00j/ipl10.bin
  COMMAND rm -f ${root_BINARY_DIR}/projects/03_day/harib00j/asmhead.bin
  COMMAND rm -f ${root_BINARY_DIR}/projects/03_day/harib00j/*_wine.bin
  COMMAND rm -f ${root_BINARY_DIR}/projects/03_day/harib00j/*.txt
)
add_custom_target(03_day_harib00j_ipl_build
  COMMAND ${RAKUSK_EXECUTABLE} ${03_day_harib00j_IPLS} ${03_day_harib00j_IPLB}
  DEPENDS ${03_day_harib00j_IPLS}
)
add_custom_command(
  OUTPUT ${03_day_harib00j_HEADB}
  COMMAND ${RAKUSK_EXECUTABLE} ${03_day_harib00j_HEADS} ${03_day_harib00j_HEADB}
  DEPENDS ${03_day_harib00j_HEADS}
  COMMENT "Generating asmhead.bin for 03_day_harib00j"
)

add_custom_command(
  OUTPUT ${03_day_harib00j_FUNCO}
  COMMAND ${RAKUSK_EXECUTABLE} ${03_day_harib00j_FUNCS} ${03_day_harib00j_FUNCO}
  DEPENDS ${03_day_harib00j_FUNCS}
  COMMENT "Generating naskfunc.o for 03_day_harib00j"
)
add_custom_target(03_day_harib00j_sys
  COMMAND gcc ${BINOPT} -T ${03_day_harib00j_LDS} ${03_day_harib00j_CCS} ${03_day_harib00j_WILDOBJ} -o ${03_day_harib00j_BOOTB}
  COMMAND cat ${03_day_harib00j_HEADB} ${03_day_harib00j_BOOTB} > ${03_day_harib00j_SYS}
  DEPENDS ${03_day_harib00j_HEADB} 03_day_harib00j_ipl_build
  DEPENDS ${03_day_harib00j_FUNCO}
)
add_custom_target(03_day_harib00j_img
  COMMAND mformat -f 1440 -l HARIBOTEOS -N 0xffffffff -C -B ${03_day_harib00j_IPLB} -i ${03_day_harib00j_OS}
  COMMAND mcopy -i ${03_day_harib00j_OS} ${03_day_harib00j_SYS} ::
  DEPENDS 03_day_harib00j_sys
)

########### next target ###############
set(03_day_harib00j_asmhead_SRCS ${root_SOURCE_DIR}/projects/03_day/harib00j/asmhead.nas)
set(03_day_harib00j_asmhead_OUTS ${root_BINARY_DIR}/projects/03_day/harib00j/asmhead.bin)

add_custom_target(03_day_harib00j_asmhead
  COMMAND ${RAKUSK_EXECUTABLE} ${03_day_harib00j_asmhead_SRCS} ${03_day_harib00j_asmhead_OUTS}
  DEPENDS ${03_day_harib00j_asmhead_SRCS}
)

set(03_day_harib00j_asmhead_WINE_OUTS ${root_BINARY_DIR}/projects/03_day/harib00j/asmhead_wine.bin)
add_custom_target(03_day_harib00j_asmhead_wine
  COMMAND ${WINE} ${WINE_NASK} ${03_day_harib00j_asmhead_SRCS} ${03_day_harib00j_asmhead_WINE_OUTS}
)
add_dependencies(wine 03_day_harib00j_asmhead_wine)

add_custom_target(03_day_harib00j_asmhead_od
  COMMAND ${OD} -t x1 ${03_day_harib00j_asmhead_OUTS}      > ${03_day_harib00j_asmhead_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${03_day_harib00j_asmhead_WINE_OUTS} > ${03_day_harib00j_asmhead_WINE_OUTS}_t.txt
  COMMAND diff -s ${03_day_harib00j_asmhead_OUTS}_f.txt ${03_day_harib00j_asmhead_WINE_OUTS}_t.txt
)

add_dependencies(03_day_harib00j_asmhead_od 03_day_harib00j_asmhead)
add_dependencies(03_day_harib00j_asmhead_od 03_day_harib00j_asmhead_wine)
add_dependencies(od 03_day_harib00j_asmhead_od)

#----------------------------------------------------------
########### next target ###############
set(03_day_harib00j_ipl10_SRCS ${root_SOURCE_DIR}/projects/03_day/harib00j/ipl10.nas)
set(03_day_harib00j_ipl10_OUTS ${root_BINARY_DIR}/projects/03_day/harib00j/ipl10.bin)

add_custom_target(03_day_harib00j_ipl10
  COMMAND ${RAKUSK_EXECUTABLE} ${03_day_harib00j_ipl10_SRCS} ${03_day_harib00j_ipl10_OUTS}
  DEPENDS ${03_day_harib00j_ipl10_SRCS}
)

set(03_day_harib00j_ipl10_WINE_OUTS ${root_BINARY_DIR}/projects/03_day/harib00j/ipl10_wine.bin)
add_custom_target(03_day_harib00j_ipl10_wine
  COMMAND ${WINE} ${WINE_NASK} ${03_day_harib00j_ipl10_SRCS} ${03_day_harib00j_ipl10_WINE_OUTS}
)
add_dependencies(wine 03_day_harib00j_ipl10_wine)

add_custom_target(03_day_harib00j_ipl10_od
  COMMAND ${OD} -t x1 ${03_day_harib00j_ipl10_OUTS}      > ${03_day_harib00j_ipl10_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${03_day_harib00j_ipl10_WINE_OUTS} > ${03_day_harib00j_ipl10_WINE_OUTS}_t.txt
  COMMAND diff -s ${03_day_harib00j_ipl10_OUTS}_f.txt ${03_day_harib00j_ipl10_WINE_OUTS}_t.txt
)

add_dependencies(03_day_harib00j_ipl10_od 03_day_harib00j_ipl10)
add_dependencies(03_day_harib00j_ipl10_od 03_day_harib00j_ipl10_wine)
add_dependencies(od 03_day_harib00j_ipl10_od)

#----------------------------------------------------------
########### next target ###############
set(03_day_harib00j_naskfunc_SRCS ${root_SOURCE_DIR}/projects/03_day/harib00j/naskfunc.nas)
set(03_day_harib00j_naskfunc_OUTS ${root_BINARY_DIR}/projects/03_day/harib00j/naskfunc.img)

add_custom_target(03_day_harib00j_naskfunc
  COMMAND ${RAKUSK_EXECUTABLE} ${03_day_harib00j_naskfunc_SRCS} ${03_day_harib00j_naskfunc_OUTS}
  DEPENDS ${03_day_harib00j_naskfunc_SRCS}
)

add_dependencies(images 03_day_harib00j_naskfunc)
set(03_day_harib00j_naskfunc_WINE_OUTS ${root_BINARY_DIR}/projects/03_day/harib00j/naskfunc_wine.img)
add_custom_target(03_day_harib00j_naskfunc_wine
  COMMAND ${WINE} ${WINE_NASK} ${03_day_harib00j_naskfunc_SRCS} ${03_day_harib00j_naskfunc_WINE_OUTS}
)
add_dependencies(wine 03_day_harib00j_naskfunc_wine)

add_custom_target(03_day_harib00j_naskfunc_od
  COMMAND ${OD} -t x1 ${03_day_harib00j_naskfunc_OUTS}      > ${03_day_harib00j_naskfunc_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${03_day_harib00j_naskfunc_WINE_OUTS} > ${03_day_harib00j_naskfunc_WINE_OUTS}_t.txt
  COMMAND diff -s ${03_day_harib00j_naskfunc_OUTS}_f.txt ${03_day_harib00j_naskfunc_WINE_OUTS}_t.txt
)

add_dependencies(03_day_harib00j_naskfunc_od 03_day_harib00j_naskfunc)
add_dependencies(03_day_harib00j_naskfunc_od 03_day_harib00j_naskfunc_wine)
add_dependencies(od 03_day_harib00j_naskfunc_od)

#----------------------------------------------------------
