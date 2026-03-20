#----------------------------------------------------------
message(STATUS "Entering directory projects/04_day/harib01h/")

set(FONT ${RAKUSK_EXECUTABLE} --makefont)
set(B2O  ${RAKUSK_EXECUTABLE} --bin2obj)
set(CONV ${root_BINARY_DIR}/objconv/objconv)
set(04_day_harib01h_WILDOBJ ${root_BINARY_DIR}/projects/04_day/harib01h/*.o)

# Full OS build rules
set(04_day_harib01h_OS    ${root_BINARY_DIR}/projects/04_day/harib01h/os.img)
set(04_day_harib01h_SYS   ${root_BINARY_DIR}/projects/04_day/harib01h/os.sys)
set(04_day_harib01h_IPLB  ${root_BINARY_DIR}/projects/04_day/harib01h/ipl.bin)
set(04_day_harib01h_IPLS  ${root_SOURCE_DIR}/projects/04_day/harib01h/ipl10.nas)
set(04_day_harib01h_HEADB ${root_BINARY_DIR}/projects/04_day/harib01h/asmhead.bin)
set(04_day_harib01h_HEADS ${root_SOURCE_DIR}/projects/04_day/harib01h/asmhead.nas)
set(04_day_harib01h_BOOTB ${root_BINARY_DIR}/projects/04_day/harib01h/boot.bin)
set(04_day_harib01h_CCO   ${root_BINARY_DIR}/projects/04_day/harib01h/boot.o)
set(04_day_harib01h_CCS   ${root_SOURCE_DIR}/projects/04_day/harib01h/*.c)
set(04_day_harib01h_LDS   ${root_SOURCE_DIR}/projects/os.lds)
set(04_day_harib01h_FUNCS ${root_SOURCE_DIR}/projects/04_day/harib01h/naskfunc.nas)
set(04_day_harib01h_FUNCO ${root_BINARY_DIR}/projects/04_day/harib01h/naskfunc.o)

add_custom_target(04_day_harib01h_run
  COMMAND ${QEMU} ${QEMUOPT} ${04_day_harib01h_OS}
  DEPENDS 04_day_harib01h_img
)
add_custom_target(04_day_harib01h_debug
  COMMAND ${QEMU} -s -S ${QEMUOPT} ${04_day_harib01h_OS} ${QEMU_DEBUG_OPT}
  DEPENDS 04_day_harib01h_img
)
add_custom_target(04_day_harib01h_clean
  COMMAND rm -f ${04_day_harib01h_OS}
  COMMAND rm -f ${04_day_harib01h_SYS}
  COMMAND rm -f ${04_day_harib01h_IPLB}
  COMMAND rm -f ${04_day_harib01h_HEADB}
  COMMAND rm -f ${04_day_harib01h_BOOTB}
  COMMAND rm -f ${04_day_harib01h_WILDOBJ}
  COMMAND rm -f ${root_BINARY_DIR}/projects/04_day/harib01h/ipl10.bin
  COMMAND rm -f ${root_BINARY_DIR}/projects/04_day/harib01h/asmhead.bin
  COMMAND rm -f ${root_BINARY_DIR}/projects/04_day/harib01h/*_wine.bin
  COMMAND rm -f ${root_BINARY_DIR}/projects/04_day/harib01h/*.txt
)
add_custom_target(04_day_harib01h_ipl_build
  COMMAND ${RAKUSK_EXECUTABLE} ${04_day_harib01h_IPLS} ${04_day_harib01h_IPLB}
  DEPENDS ${04_day_harib01h_IPLS}
)
add_custom_command(
  OUTPUT ${04_day_harib01h_HEADB}
  COMMAND ${RAKUSK_EXECUTABLE} ${04_day_harib01h_HEADS} ${04_day_harib01h_HEADB}
  DEPENDS ${04_day_harib01h_HEADS}
  COMMENT "Generating asmhead.bin for 04_day_harib01h"
)

add_custom_target(04_day_harib01h_sys
  COMMAND ${RAKUSK_EXECUTABLE} ${04_day_harib01h_FUNCS} ${04_day_harib01h_FUNCO}
  COMMAND gcc ${BINOPT} -T ${04_day_harib01h_LDS} ${04_day_harib01h_CCS} ${04_day_harib01h_WILDOBJ} -o ${04_day_harib01h_BOOTB}
  COMMAND cat ${04_day_harib01h_HEADB} ${04_day_harib01h_BOOTB} > ${04_day_harib01h_SYS}
  DEPENDS ${04_day_harib01h_HEADB} 04_day_harib01h_ipl_build
  DEPENDS ${04_day_harib01h_FUNCO}
)
add_custom_target(04_day_harib01h_img
  COMMAND mformat -f 1440 -l HARIBOTEOS -N 0xffffffff -C -B ${04_day_harib01h_IPLB} -i ${04_day_harib01h_OS}
  COMMAND mcopy -i ${04_day_harib01h_OS} ${04_day_harib01h_SYS} ::
  DEPENDS 04_day_harib01h_sys
)

########### next target ###############
set(04_day_harib01h_asmhead_SRCS ${root_SOURCE_DIR}/projects/04_day/harib01h/asmhead.nas)
set(04_day_harib01h_asmhead_OUTS ${root_BINARY_DIR}/projects/04_day/harib01h/asmhead.bin)

add_custom_target(04_day_harib01h_asmhead
  COMMAND ${RAKUSK_EXECUTABLE} ${04_day_harib01h_asmhead_SRCS} ${04_day_harib01h_asmhead_OUTS}
  DEPENDS ${04_day_harib01h_asmhead_SRCS}
)

set(04_day_harib01h_asmhead_WINE_OUTS ${root_BINARY_DIR}/projects/04_day/harib01h/asmhead_wine.bin)
add_custom_target(04_day_harib01h_asmhead_wine
  COMMAND ${WINE} ${WINE_NASK} ${04_day_harib01h_asmhead_SRCS} ${04_day_harib01h_asmhead_WINE_OUTS}
)
add_dependencies(wine 04_day_harib01h_asmhead_wine)

add_custom_target(04_day_harib01h_asmhead_od
  COMMAND ${OD} -t x1 ${04_day_harib01h_asmhead_OUTS}      > ${04_day_harib01h_asmhead_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${04_day_harib01h_asmhead_WINE_OUTS} > ${04_day_harib01h_asmhead_WINE_OUTS}_t.txt
  COMMAND diff -s ${04_day_harib01h_asmhead_OUTS}_f.txt ${04_day_harib01h_asmhead_WINE_OUTS}_t.txt
)

add_dependencies(04_day_harib01h_asmhead_od 04_day_harib01h_asmhead)
add_dependencies(04_day_harib01h_asmhead_od 04_day_harib01h_asmhead_wine)
add_dependencies(od 04_day_harib01h_asmhead_od)

#----------------------------------------------------------
########### next target ###############
set(04_day_harib01h_ipl10_SRCS ${root_SOURCE_DIR}/projects/04_day/harib01h/ipl10.nas)
set(04_day_harib01h_ipl10_OUTS ${root_BINARY_DIR}/projects/04_day/harib01h/ipl10.bin)

add_custom_target(04_day_harib01h_ipl10
  COMMAND ${RAKUSK_EXECUTABLE} ${04_day_harib01h_ipl10_SRCS} ${04_day_harib01h_ipl10_OUTS}
  DEPENDS ${04_day_harib01h_ipl10_SRCS}
)

set(04_day_harib01h_ipl10_WINE_OUTS ${root_BINARY_DIR}/projects/04_day/harib01h/ipl10_wine.bin)
add_custom_target(04_day_harib01h_ipl10_wine
  COMMAND ${WINE} ${WINE_NASK} ${04_day_harib01h_ipl10_SRCS} ${04_day_harib01h_ipl10_WINE_OUTS}
)
add_dependencies(wine 04_day_harib01h_ipl10_wine)

add_custom_target(04_day_harib01h_ipl10_od
  COMMAND ${OD} -t x1 ${04_day_harib01h_ipl10_OUTS}      > ${04_day_harib01h_ipl10_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${04_day_harib01h_ipl10_WINE_OUTS} > ${04_day_harib01h_ipl10_WINE_OUTS}_t.txt
  COMMAND diff -s ${04_day_harib01h_ipl10_OUTS}_f.txt ${04_day_harib01h_ipl10_WINE_OUTS}_t.txt
)

add_dependencies(04_day_harib01h_ipl10_od 04_day_harib01h_ipl10)
add_dependencies(04_day_harib01h_ipl10_od 04_day_harib01h_ipl10_wine)
add_dependencies(od 04_day_harib01h_ipl10_od)

#----------------------------------------------------------
########### next target ###############
set(04_day_harib01h_naskfunc_SRCS ${root_SOURCE_DIR}/projects/04_day/harib01h/naskfunc.nas)
set(04_day_harib01h_naskfunc_OUTS ${root_BINARY_DIR}/projects/04_day/harib01h/naskfunc.img)

add_custom_target(04_day_harib01h_naskfunc
  COMMAND ${RAKUSK_EXECUTABLE} ${04_day_harib01h_naskfunc_SRCS} ${04_day_harib01h_naskfunc_OUTS}
  DEPENDS ${04_day_harib01h_naskfunc_SRCS}
)

add_dependencies(images 04_day_harib01h_naskfunc)
set(04_day_harib01h_naskfunc_WINE_OUTS ${root_BINARY_DIR}/projects/04_day/harib01h/naskfunc_wine.img)
add_custom_target(04_day_harib01h_naskfunc_wine
  COMMAND ${WINE} ${WINE_NASK} ${04_day_harib01h_naskfunc_SRCS} ${04_day_harib01h_naskfunc_WINE_OUTS}
)
add_dependencies(wine 04_day_harib01h_naskfunc_wine)

add_custom_target(04_day_harib01h_naskfunc_od
  COMMAND ${OD} -t x1 ${04_day_harib01h_naskfunc_OUTS}      > ${04_day_harib01h_naskfunc_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${04_day_harib01h_naskfunc_WINE_OUTS} > ${04_day_harib01h_naskfunc_WINE_OUTS}_t.txt
  COMMAND diff -s ${04_day_harib01h_naskfunc_OUTS}_f.txt ${04_day_harib01h_naskfunc_WINE_OUTS}_t.txt
)

add_dependencies(04_day_harib01h_naskfunc_od 04_day_harib01h_naskfunc)
add_dependencies(04_day_harib01h_naskfunc_od 04_day_harib01h_naskfunc_wine)
add_dependencies(od 04_day_harib01h_naskfunc_od)

#----------------------------------------------------------
