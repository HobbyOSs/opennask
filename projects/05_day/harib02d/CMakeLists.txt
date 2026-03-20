#----------------------------------------------------------
message(STATUS "Entering directory projects/05_day/harib02d/")

set(FONT ${root_BINARY_DIR}/src/makefont)
set(B2O  ${root_BINARY_DIR}/src/bin2obj)
set(CONV ${root_BINARY_DIR}/objconv/objconv)
set(05_day_harib02d_WILDOBJ ${root_BINARY_DIR}/projects/05_day/harib02d/*.o)

# Full OS build rules
set(05_day_harib02d_OS    ${root_BINARY_DIR}/projects/05_day/harib02d/os.img)
set(05_day_harib02d_SYS   ${root_BINARY_DIR}/projects/05_day/harib02d/os.sys)
set(05_day_harib02d_IPLB  ${root_BINARY_DIR}/projects/05_day/harib02d/ipl.bin)
set(05_day_harib02d_IPLS  ${root_SOURCE_DIR}/projects/05_day/harib02d/ipl10.nas)
set(05_day_harib02d_HEADB ${root_BINARY_DIR}/projects/05_day/harib02d/asmhead.bin)
set(05_day_harib02d_HEADS ${root_SOURCE_DIR}/projects/05_day/harib02d/asmhead.nas)
set(05_day_harib02d_BOOTB ${root_BINARY_DIR}/projects/05_day/harib02d/boot.bin)
set(05_day_harib02d_CCO   ${root_BINARY_DIR}/projects/05_day/harib02d/boot.o)
set(05_day_harib02d_CCS   ${root_SOURCE_DIR}/projects/05_day/harib02d/*.c)
set(05_day_harib02d_LDS   ${root_SOURCE_DIR}/projects/os.lds)
set(05_day_harib02d_FUNCS ${root_SOURCE_DIR}/projects/05_day/harib02d/naskfunc.nas)
set(05_day_harib02d_FUNCO ${root_BINARY_DIR}/projects/05_day/harib02d/naskfunc.o)

add_custom_target(05_day_harib02d_run
  COMMAND ${QEMU} ${QEMUOPT} ${05_day_harib02d_OS}
  DEPENDS 05_day_harib02d_img
)
add_custom_target(05_day_harib02d_debug
  COMMAND ${QEMU} -s -S ${QEMUOPT} ${05_day_harib02d_OS} ${QEMU_DEBUG_OPT}
  DEPENDS 05_day_harib02d_img
)
add_custom_target(05_day_harib02d_clean
  COMMAND rm -f ${05_day_harib02d_OS}
  COMMAND rm -f ${05_day_harib02d_SYS}
  COMMAND rm -f ${05_day_harib02d_IPLB}
  COMMAND rm -f ${05_day_harib02d_HEADB}
  COMMAND rm -f ${05_day_harib02d_BOOTB}
  COMMAND rm -f ${05_day_harib02d_WILDOBJ}
  COMMAND rm -f ${root_BINARY_DIR}/projects/05_day/harib02d/ipl10.bin
  COMMAND rm -f ${root_BINARY_DIR}/projects/05_day/harib02d/asmhead.bin
  COMMAND rm -f ${root_BINARY_DIR}/projects/05_day/harib02d/*_wine.bin
  COMMAND rm -f ${root_BINARY_DIR}/projects/05_day/harib02d/*.txt
)
add_custom_target(05_day_harib02d_ipl_build
  COMMAND ${RAKUSK_EXECUTABLE} ${05_day_harib02d_IPLS} ${05_day_harib02d_IPLB}
  DEPENDS ${05_day_harib02d_IPLS}
)
add_custom_command(
  OUTPUT ${05_day_harib02d_HEADB}
  COMMAND ${RAKUSK_EXECUTABLE} ${05_day_harib02d_HEADS} ${05_day_harib02d_HEADB}
  DEPENDS ${05_day_harib02d_HEADS}
  COMMENT "Generating asmhead.bin for 05_day_harib02d"
)

add_custom_command(
  OUTPUT ${05_day_harib02d_FUNCO}
  COMMAND ${RAKUSK_EXECUTABLE} ${05_day_harib02d_FUNCS} ${05_day_harib02d_FUNCO}
  DEPENDS ${05_day_harib02d_FUNCS}
  COMMENT "Generating naskfunc.o for 05_day_harib02d"
)
add_custom_target(05_day_harib02d_sys
  COMMAND gcc ${BINOPT} -T ${05_day_harib02d_LDS} ${05_day_harib02d_CCS} ${05_day_harib02d_WILDOBJ} -o ${05_day_harib02d_BOOTB}
  COMMAND cat ${05_day_harib02d_HEADB} ${05_day_harib02d_BOOTB} > ${05_day_harib02d_SYS}
  DEPENDS ${05_day_harib02d_HEADB} 05_day_harib02d_ipl_build
  DEPENDS ${05_day_harib02d_FUNCO}
)
add_custom_target(05_day_harib02d_img
  COMMAND mformat -f 1440 -l HARIBOTEOS -N 0xffffffff -C -B ${05_day_harib02d_IPLB} -i ${05_day_harib02d_OS}
  COMMAND mcopy -i ${05_day_harib02d_OS} ${05_day_harib02d_SYS} ::
  DEPENDS 05_day_harib02d_sys
)

########### next target ###############
set(05_day_harib02d_asmhead_SRCS ${root_SOURCE_DIR}/projects/05_day/harib02d/asmhead.nas)
set(05_day_harib02d_asmhead_OUTS ${root_BINARY_DIR}/projects/05_day/harib02d/asmhead.bin)

add_custom_target(05_day_harib02d_asmhead
  COMMAND ${RAKUSK_EXECUTABLE} ${05_day_harib02d_asmhead_SRCS} ${05_day_harib02d_asmhead_OUTS}
  DEPENDS ${05_day_harib02d_asmhead_SRCS}
)

set(05_day_harib02d_asmhead_WINE_OUTS ${root_BINARY_DIR}/projects/05_day/harib02d/asmhead_wine.bin)
add_custom_target(05_day_harib02d_asmhead_wine
  COMMAND ${WINE} ${WINE_NASK} ${05_day_harib02d_asmhead_SRCS} ${05_day_harib02d_asmhead_WINE_OUTS}
)
add_dependencies(wine 05_day_harib02d_asmhead_wine)

add_custom_target(05_day_harib02d_asmhead_od
  COMMAND ${OD} -t x1 ${05_day_harib02d_asmhead_OUTS}      > ${05_day_harib02d_asmhead_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${05_day_harib02d_asmhead_WINE_OUTS} > ${05_day_harib02d_asmhead_WINE_OUTS}_t.txt
  COMMAND diff -s ${05_day_harib02d_asmhead_OUTS}_f.txt ${05_day_harib02d_asmhead_WINE_OUTS}_t.txt
)

add_dependencies(05_day_harib02d_asmhead_od 05_day_harib02d_asmhead)
add_dependencies(05_day_harib02d_asmhead_od 05_day_harib02d_asmhead_wine)
add_dependencies(od 05_day_harib02d_asmhead_od)

#----------------------------------------------------------
########### next target ###############
set(05_day_harib02d_ipl10_SRCS ${root_SOURCE_DIR}/projects/05_day/harib02d/ipl10.nas)
set(05_day_harib02d_ipl10_OUTS ${root_BINARY_DIR}/projects/05_day/harib02d/ipl10.bin)

add_custom_target(05_day_harib02d_ipl10
  COMMAND ${RAKUSK_EXECUTABLE} ${05_day_harib02d_ipl10_SRCS} ${05_day_harib02d_ipl10_OUTS}
  DEPENDS ${05_day_harib02d_ipl10_SRCS}
)

set(05_day_harib02d_ipl10_WINE_OUTS ${root_BINARY_DIR}/projects/05_day/harib02d/ipl10_wine.bin)
add_custom_target(05_day_harib02d_ipl10_wine
  COMMAND ${WINE} ${WINE_NASK} ${05_day_harib02d_ipl10_SRCS} ${05_day_harib02d_ipl10_WINE_OUTS}
)
add_dependencies(wine 05_day_harib02d_ipl10_wine)

add_custom_target(05_day_harib02d_ipl10_od
  COMMAND ${OD} -t x1 ${05_day_harib02d_ipl10_OUTS}      > ${05_day_harib02d_ipl10_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${05_day_harib02d_ipl10_WINE_OUTS} > ${05_day_harib02d_ipl10_WINE_OUTS}_t.txt
  COMMAND diff -s ${05_day_harib02d_ipl10_OUTS}_f.txt ${05_day_harib02d_ipl10_WINE_OUTS}_t.txt
)

add_dependencies(05_day_harib02d_ipl10_od 05_day_harib02d_ipl10)
add_dependencies(05_day_harib02d_ipl10_od 05_day_harib02d_ipl10_wine)
add_dependencies(od 05_day_harib02d_ipl10_od)

#----------------------------------------------------------
########### next target ###############
set(05_day_harib02d_naskfunc_SRCS ${root_SOURCE_DIR}/projects/05_day/harib02d/naskfunc.nas)
set(05_day_harib02d_naskfunc_OUTS ${root_BINARY_DIR}/projects/05_day/harib02d/naskfunc.img)

add_custom_target(05_day_harib02d_naskfunc
  COMMAND ${RAKUSK_EXECUTABLE} ${05_day_harib02d_naskfunc_SRCS} ${05_day_harib02d_naskfunc_OUTS}
  DEPENDS ${05_day_harib02d_naskfunc_SRCS}
)

add_dependencies(images 05_day_harib02d_naskfunc)
set(05_day_harib02d_naskfunc_WINE_OUTS ${root_BINARY_DIR}/projects/05_day/harib02d/naskfunc_wine.img)
add_custom_target(05_day_harib02d_naskfunc_wine
  COMMAND ${WINE} ${WINE_NASK} ${05_day_harib02d_naskfunc_SRCS} ${05_day_harib02d_naskfunc_WINE_OUTS}
)
add_dependencies(wine 05_day_harib02d_naskfunc_wine)

add_custom_target(05_day_harib02d_naskfunc_od
  COMMAND ${OD} -t x1 ${05_day_harib02d_naskfunc_OUTS}      > ${05_day_harib02d_naskfunc_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${05_day_harib02d_naskfunc_WINE_OUTS} > ${05_day_harib02d_naskfunc_WINE_OUTS}_t.txt
  COMMAND diff -s ${05_day_harib02d_naskfunc_OUTS}_f.txt ${05_day_harib02d_naskfunc_WINE_OUTS}_t.txt
)

add_dependencies(05_day_harib02d_naskfunc_od 05_day_harib02d_naskfunc)
add_dependencies(05_day_harib02d_naskfunc_od 05_day_harib02d_naskfunc_wine)
add_dependencies(od 05_day_harib02d_naskfunc_od)

#----------------------------------------------------------
