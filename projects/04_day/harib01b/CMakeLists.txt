#----------------------------------------------------------
message(STATUS "Entering directory projects/04_day/harib01b/")

set(FONT ${root_BINARY_DIR}/src/makefont)
set(B2O  ${root_BINARY_DIR}/src/bin2obj)
set(CONV ${root_BINARY_DIR}/objconv/objconv)
set(04_day_harib01b_WILDOBJ ${root_BINARY_DIR}/projects/04_day/harib01b/*.o)

# Full OS build rules
set(04_day_harib01b_OS    ${root_BINARY_DIR}/projects/04_day/harib01b/os.img)
set(04_day_harib01b_SYS   ${root_BINARY_DIR}/projects/04_day/harib01b/os.sys)
set(04_day_harib01b_IPLB  ${root_BINARY_DIR}/projects/04_day/harib01b/ipl.bin)
set(04_day_harib01b_IPLS  ${root_SOURCE_DIR}/projects/04_day/harib01b/ipl10.nas)
set(04_day_harib01b_HEADB ${root_BINARY_DIR}/projects/04_day/harib01b/asmhead.bin)
set(04_day_harib01b_HEADS ${root_SOURCE_DIR}/projects/04_day/harib01b/asmhead.nas)
set(04_day_harib01b_BOOTB ${root_BINARY_DIR}/projects/04_day/harib01b/boot.bin)
set(04_day_harib01b_CCO   ${root_BINARY_DIR}/projects/04_day/harib01b/boot.o)
set(04_day_harib01b_CCS   ${root_SOURCE_DIR}/projects/04_day/harib01b/*.c)
set(04_day_harib01b_LDS   ${root_SOURCE_DIR}/projects/os.lds)
set(04_day_harib01b_FUNCS ${root_SOURCE_DIR}/projects/04_day/harib01b/naskfunc.nas)
set(04_day_harib01b_FUNCO ${root_BINARY_DIR}/projects/04_day/harib01b/naskfunc.o)

add_custom_target(04_day_harib01b_run
  COMMAND ${QEMU} ${QEMUOPT} ${04_day_harib01b_OS}
  DEPENDS 04_day_harib01b_img
)
add_custom_target(04_day_harib01b_debug
  COMMAND ${QEMU} -s -S ${QEMUOPT} ${04_day_harib01b_OS} ${QEMU_DEBUG_OPT}
  DEPENDS 04_day_harib01b_img
)
add_custom_target(04_day_harib01b_clean
  COMMAND rm -f ${04_day_harib01b_OS}
  COMMAND rm -f ${04_day_harib01b_SYS}
  COMMAND rm -f ${04_day_harib01b_IPLB}
  COMMAND rm -f ${04_day_harib01b_HEADB}
  COMMAND rm -f ${04_day_harib01b_BOOTB}
  COMMAND rm -f ${04_day_harib01b_WILDOBJ}
  COMMAND rm -f ${root_BINARY_DIR}/projects/04_day/harib01b/ipl10.bin
  COMMAND rm -f ${root_BINARY_DIR}/projects/04_day/harib01b/asmhead.bin
  COMMAND rm -f ${root_BINARY_DIR}/projects/04_day/harib01b/*_wine.bin
  COMMAND rm -f ${root_BINARY_DIR}/projects/04_day/harib01b/*.txt
)
add_custom_target(04_day_harib01b_ipl_build
  COMMAND ${RAKUSK_EXECUTABLE} ${04_day_harib01b_IPLS} ${04_day_harib01b_IPLB}
  DEPENDS ${04_day_harib01b_IPLS}
)
add_custom_command(
  OUTPUT ${04_day_harib01b_HEADB}
  COMMAND ${RAKUSK_EXECUTABLE} ${04_day_harib01b_HEADS} ${04_day_harib01b_HEADB}
  DEPENDS ${04_day_harib01b_HEADS}
  COMMENT "Generating asmhead.bin for 04_day_harib01b"
)

add_custom_command(
  OUTPUT ${04_day_harib01b_FUNCO}
  COMMAND ${RAKUSK_EXECUTABLE} ${04_day_harib01b_FUNCS} ${04_day_harib01b_FUNCO}
  DEPENDS ${04_day_harib01b_FUNCS}
  COMMENT "Generating naskfunc.o for 04_day_harib01b"
)
add_custom_target(04_day_harib01b_sys
  COMMAND gcc ${BINOPT} -T ${04_day_harib01b_LDS} ${04_day_harib01b_CCS} ${04_day_harib01b_WILDOBJ} -o ${04_day_harib01b_BOOTB}
  COMMAND cat ${04_day_harib01b_HEADB} ${04_day_harib01b_BOOTB} > ${04_day_harib01b_SYS}
  DEPENDS ${04_day_harib01b_HEADB} 04_day_harib01b_ipl_build
  DEPENDS ${04_day_harib01b_FUNCO}
)
add_custom_target(04_day_harib01b_img
  COMMAND mformat -f 1440 -l HARIBOTEOS -N 0xffffffff -C -B ${04_day_harib01b_IPLB} -i ${04_day_harib01b_OS}
  COMMAND mcopy -i ${04_day_harib01b_OS} ${04_day_harib01b_SYS} ::
  DEPENDS 04_day_harib01b_sys
)

########### next target ###############
set(04_day_harib01b_asmhead_SRCS ${root_SOURCE_DIR}/projects/04_day/harib01b/asmhead.nas)
set(04_day_harib01b_asmhead_OUTS ${root_BINARY_DIR}/projects/04_day/harib01b/asmhead.bin)

add_custom_target(04_day_harib01b_asmhead
  COMMAND ${RAKUSK_EXECUTABLE} ${04_day_harib01b_asmhead_SRCS} ${04_day_harib01b_asmhead_OUTS}
  DEPENDS ${04_day_harib01b_asmhead_SRCS}
)

set(04_day_harib01b_asmhead_WINE_OUTS ${root_BINARY_DIR}/projects/04_day/harib01b/asmhead_wine.bin)
add_custom_target(04_day_harib01b_asmhead_wine
  COMMAND ${WINE} ${WINE_NASK} ${04_day_harib01b_asmhead_SRCS} ${04_day_harib01b_asmhead_WINE_OUTS}
)
add_dependencies(wine 04_day_harib01b_asmhead_wine)

add_custom_target(04_day_harib01b_asmhead_od
  COMMAND ${OD} -t x1 ${04_day_harib01b_asmhead_OUTS}      > ${04_day_harib01b_asmhead_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${04_day_harib01b_asmhead_WINE_OUTS} > ${04_day_harib01b_asmhead_WINE_OUTS}_t.txt
  COMMAND diff -s ${04_day_harib01b_asmhead_OUTS}_f.txt ${04_day_harib01b_asmhead_WINE_OUTS}_t.txt
)

add_dependencies(04_day_harib01b_asmhead_od 04_day_harib01b_asmhead)
add_dependencies(04_day_harib01b_asmhead_od 04_day_harib01b_asmhead_wine)
add_dependencies(od 04_day_harib01b_asmhead_od)

#----------------------------------------------------------
########### next target ###############
set(04_day_harib01b_ipl10_SRCS ${root_SOURCE_DIR}/projects/04_day/harib01b/ipl10.nas)
set(04_day_harib01b_ipl10_OUTS ${root_BINARY_DIR}/projects/04_day/harib01b/ipl10.bin)

add_custom_target(04_day_harib01b_ipl10
  COMMAND ${RAKUSK_EXECUTABLE} ${04_day_harib01b_ipl10_SRCS} ${04_day_harib01b_ipl10_OUTS}
  DEPENDS ${04_day_harib01b_ipl10_SRCS}
)

set(04_day_harib01b_ipl10_WINE_OUTS ${root_BINARY_DIR}/projects/04_day/harib01b/ipl10_wine.bin)
add_custom_target(04_day_harib01b_ipl10_wine
  COMMAND ${WINE} ${WINE_NASK} ${04_day_harib01b_ipl10_SRCS} ${04_day_harib01b_ipl10_WINE_OUTS}
)
add_dependencies(wine 04_day_harib01b_ipl10_wine)

add_custom_target(04_day_harib01b_ipl10_od
  COMMAND ${OD} -t x1 ${04_day_harib01b_ipl10_OUTS}      > ${04_day_harib01b_ipl10_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${04_day_harib01b_ipl10_WINE_OUTS} > ${04_day_harib01b_ipl10_WINE_OUTS}_t.txt
  COMMAND diff -s ${04_day_harib01b_ipl10_OUTS}_f.txt ${04_day_harib01b_ipl10_WINE_OUTS}_t.txt
)

add_dependencies(04_day_harib01b_ipl10_od 04_day_harib01b_ipl10)
add_dependencies(04_day_harib01b_ipl10_od 04_day_harib01b_ipl10_wine)
add_dependencies(od 04_day_harib01b_ipl10_od)

#----------------------------------------------------------
########### next target ###############
set(04_day_harib01b_naskfunc_SRCS ${root_SOURCE_DIR}/projects/04_day/harib01b/naskfunc.nas)
set(04_day_harib01b_naskfunc_OUTS ${root_BINARY_DIR}/projects/04_day/harib01b/naskfunc.img)

add_custom_target(04_day_harib01b_naskfunc
  COMMAND ${RAKUSK_EXECUTABLE} ${04_day_harib01b_naskfunc_SRCS} ${04_day_harib01b_naskfunc_OUTS}
  DEPENDS ${04_day_harib01b_naskfunc_SRCS}
)

add_dependencies(images 04_day_harib01b_naskfunc)
set(04_day_harib01b_naskfunc_WINE_OUTS ${root_BINARY_DIR}/projects/04_day/harib01b/naskfunc_wine.img)
add_custom_target(04_day_harib01b_naskfunc_wine
  COMMAND ${WINE} ${WINE_NASK} ${04_day_harib01b_naskfunc_SRCS} ${04_day_harib01b_naskfunc_WINE_OUTS}
)
add_dependencies(wine 04_day_harib01b_naskfunc_wine)

add_custom_target(04_day_harib01b_naskfunc_od
  COMMAND ${OD} -t x1 ${04_day_harib01b_naskfunc_OUTS}      > ${04_day_harib01b_naskfunc_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${04_day_harib01b_naskfunc_WINE_OUTS} > ${04_day_harib01b_naskfunc_WINE_OUTS}_t.txt
  COMMAND diff -s ${04_day_harib01b_naskfunc_OUTS}_f.txt ${04_day_harib01b_naskfunc_WINE_OUTS}_t.txt
)

add_dependencies(04_day_harib01b_naskfunc_od 04_day_harib01b_naskfunc)
add_dependencies(04_day_harib01b_naskfunc_od 04_day_harib01b_naskfunc_wine)
add_dependencies(od 04_day_harib01b_naskfunc_od)

#----------------------------------------------------------
