#----------------------------------------------------------
message(STATUS "Entering directory projects/05_day/harib02c/")

set(FONT ${RAKUSK_EXECUTABLE} --makefont)
set(B2O  ${RAKUSK_EXECUTABLE} --bin2obj)
set(CONV ${root_BINARY_DIR}/objconv/objconv)
set(05_day_harib02c_WILDOBJ ${root_BINARY_DIR}/projects/05_day/harib02c/*.o)

# Full OS build rules
set(05_day_harib02c_OS    ${root_BINARY_DIR}/projects/05_day/harib02c/os.img)
set(05_day_harib02c_SYS   ${root_BINARY_DIR}/projects/05_day/harib02c/os.sys)
set(05_day_harib02c_IPLB  ${root_BINARY_DIR}/projects/05_day/harib02c/ipl.bin)
set(05_day_harib02c_IPLS  ${root_SOURCE_DIR}/projects/05_day/harib02c/ipl10.nas)
set(05_day_harib02c_HEADB ${root_BINARY_DIR}/projects/05_day/harib02c/asmhead.bin)
set(05_day_harib02c_HEADS ${root_SOURCE_DIR}/projects/05_day/harib02c/asmhead.nas)
set(05_day_harib02c_BOOTB ${root_BINARY_DIR}/projects/05_day/harib02c/boot.bin)
set(05_day_harib02c_CCO   ${root_BINARY_DIR}/projects/05_day/harib02c/boot.o)
set(05_day_harib02c_CCS   ${root_SOURCE_DIR}/projects/05_day/harib02c/*.c)
set(05_day_harib02c_LDS   ${root_SOURCE_DIR}/projects/os.lds)
set(05_day_harib02c_FUNCS ${root_SOURCE_DIR}/projects/05_day/harib02c/naskfunc.nas)
set(05_day_harib02c_FUNCO ${root_BINARY_DIR}/projects/05_day/harib02c/naskfunc.o)

add_custom_target(05_day_harib02c_run
  COMMAND ${QEMU} ${QEMUOPT} ${05_day_harib02c_OS}
  DEPENDS 05_day_harib02c_img
)
add_custom_target(05_day_harib02c_debug
  COMMAND ${QEMU} -s -S ${QEMUOPT} ${05_day_harib02c_OS} ${QEMU_DEBUG_OPT}
  DEPENDS 05_day_harib02c_img
)
add_custom_target(05_day_harib02c_clean
  COMMAND rm -f ${05_day_harib02c_OS}
  COMMAND rm -f ${05_day_harib02c_SYS}
  COMMAND rm -f ${05_day_harib02c_IPLB}
  COMMAND rm -f ${05_day_harib02c_HEADB}
  COMMAND rm -f ${05_day_harib02c_BOOTB}
  COMMAND rm -f ${05_day_harib02c_WILDOBJ}
  COMMAND rm -f ${root_BINARY_DIR}/projects/05_day/harib02c/ipl10.bin
  COMMAND rm -f ${root_BINARY_DIR}/projects/05_day/harib02c/asmhead.bin
  COMMAND rm -f ${root_BINARY_DIR}/projects/05_day/harib02c/*_wine.bin
  COMMAND rm -f ${root_BINARY_DIR}/projects/05_day/harib02c/*.txt
)
add_custom_target(05_day_harib02c_ipl_build
  COMMAND ${RAKUSK_EXECUTABLE} ${05_day_harib02c_IPLS} ${05_day_harib02c_IPLB}
  DEPENDS ${05_day_harib02c_IPLS}
)
add_custom_command(
  OUTPUT ${05_day_harib02c_HEADB}
  COMMAND ${RAKUSK_EXECUTABLE} ${05_day_harib02c_HEADS} ${05_day_harib02c_HEADB}
  DEPENDS ${05_day_harib02c_HEADS}
  COMMENT "Generating asmhead.bin for 05_day_harib02c"
)

add_custom_target(05_day_harib02c_sys
  COMMAND ${RAKUSK_EXECUTABLE} ${05_day_harib02c_FUNCS} ${05_day_harib02c_FUNCO}
  COMMAND gcc ${BINOPT} -T ${05_day_harib02c_LDS} ${05_day_harib02c_CCS} ${05_day_harib02c_WILDOBJ} -o ${05_day_harib02c_BOOTB}
  COMMAND cat ${05_day_harib02c_HEADB} ${05_day_harib02c_BOOTB} > ${05_day_harib02c_SYS}
  DEPENDS ${05_day_harib02c_HEADB} 05_day_harib02c_ipl_build
  DEPENDS ${05_day_harib02c_FUNCO}
)
add_custom_target(05_day_harib02c_img
  COMMAND mformat -f 1440 -l HARIBOTEOS -N 0xffffffff -C -B ${05_day_harib02c_IPLB} -i ${05_day_harib02c_OS}
  COMMAND mcopy -i ${05_day_harib02c_OS} ${05_day_harib02c_SYS} ::
  DEPENDS 05_day_harib02c_sys
)

########### next target ###############
set(05_day_harib02c_asmhead_SRCS ${root_SOURCE_DIR}/projects/05_day/harib02c/asmhead.nas)
set(05_day_harib02c_asmhead_OUTS ${root_BINARY_DIR}/projects/05_day/harib02c/asmhead.bin)

add_custom_target(05_day_harib02c_asmhead
  COMMAND ${RAKUSK_EXECUTABLE} ${05_day_harib02c_asmhead_SRCS} ${05_day_harib02c_asmhead_OUTS}
  DEPENDS ${05_day_harib02c_asmhead_SRCS}
)

set(05_day_harib02c_asmhead_WINE_OUTS ${root_BINARY_DIR}/projects/05_day/harib02c/asmhead_wine.bin)
add_custom_target(05_day_harib02c_asmhead_wine
  COMMAND ${WINE} ${WINE_NASK} ${05_day_harib02c_asmhead_SRCS} ${05_day_harib02c_asmhead_WINE_OUTS}
)
add_dependencies(wine 05_day_harib02c_asmhead_wine)

add_custom_target(05_day_harib02c_asmhead_od
  COMMAND ${OD} -t x1 ${05_day_harib02c_asmhead_OUTS}      > ${05_day_harib02c_asmhead_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${05_day_harib02c_asmhead_WINE_OUTS} > ${05_day_harib02c_asmhead_WINE_OUTS}_t.txt
  COMMAND diff -s ${05_day_harib02c_asmhead_OUTS}_f.txt ${05_day_harib02c_asmhead_WINE_OUTS}_t.txt
)

add_dependencies(05_day_harib02c_asmhead_od 05_day_harib02c_asmhead)
add_dependencies(05_day_harib02c_asmhead_od 05_day_harib02c_asmhead_wine)
add_dependencies(od 05_day_harib02c_asmhead_od)

#----------------------------------------------------------
########### next target ###############
set(05_day_harib02c_ipl10_SRCS ${root_SOURCE_DIR}/projects/05_day/harib02c/ipl10.nas)
set(05_day_harib02c_ipl10_OUTS ${root_BINARY_DIR}/projects/05_day/harib02c/ipl10.bin)

add_custom_target(05_day_harib02c_ipl10
  COMMAND ${RAKUSK_EXECUTABLE} ${05_day_harib02c_ipl10_SRCS} ${05_day_harib02c_ipl10_OUTS}
  DEPENDS ${05_day_harib02c_ipl10_SRCS}
)

set(05_day_harib02c_ipl10_WINE_OUTS ${root_BINARY_DIR}/projects/05_day/harib02c/ipl10_wine.bin)
add_custom_target(05_day_harib02c_ipl10_wine
  COMMAND ${WINE} ${WINE_NASK} ${05_day_harib02c_ipl10_SRCS} ${05_day_harib02c_ipl10_WINE_OUTS}
)
add_dependencies(wine 05_day_harib02c_ipl10_wine)

add_custom_target(05_day_harib02c_ipl10_od
  COMMAND ${OD} -t x1 ${05_day_harib02c_ipl10_OUTS}      > ${05_day_harib02c_ipl10_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${05_day_harib02c_ipl10_WINE_OUTS} > ${05_day_harib02c_ipl10_WINE_OUTS}_t.txt
  COMMAND diff -s ${05_day_harib02c_ipl10_OUTS}_f.txt ${05_day_harib02c_ipl10_WINE_OUTS}_t.txt
)

add_dependencies(05_day_harib02c_ipl10_od 05_day_harib02c_ipl10)
add_dependencies(05_day_harib02c_ipl10_od 05_day_harib02c_ipl10_wine)
add_dependencies(od 05_day_harib02c_ipl10_od)

#----------------------------------------------------------
########### next target ###############
set(05_day_harib02c_naskfunc_SRCS ${root_SOURCE_DIR}/projects/05_day/harib02c/naskfunc.nas)
set(05_day_harib02c_naskfunc_OUTS ${root_BINARY_DIR}/projects/05_day/harib02c/naskfunc.img)

add_custom_target(05_day_harib02c_naskfunc
  COMMAND ${RAKUSK_EXECUTABLE} ${05_day_harib02c_naskfunc_SRCS} ${05_day_harib02c_naskfunc_OUTS}
  DEPENDS ${05_day_harib02c_naskfunc_SRCS}
)

add_dependencies(images 05_day_harib02c_naskfunc)
set(05_day_harib02c_naskfunc_WINE_OUTS ${root_BINARY_DIR}/projects/05_day/harib02c/naskfunc_wine.img)
add_custom_target(05_day_harib02c_naskfunc_wine
  COMMAND ${WINE} ${WINE_NASK} ${05_day_harib02c_naskfunc_SRCS} ${05_day_harib02c_naskfunc_WINE_OUTS}
)
add_dependencies(wine 05_day_harib02c_naskfunc_wine)

add_custom_target(05_day_harib02c_naskfunc_od
  COMMAND ${OD} -t x1 ${05_day_harib02c_naskfunc_OUTS}      > ${05_day_harib02c_naskfunc_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${05_day_harib02c_naskfunc_WINE_OUTS} > ${05_day_harib02c_naskfunc_WINE_OUTS}_t.txt
  COMMAND diff -s ${05_day_harib02c_naskfunc_OUTS}_f.txt ${05_day_harib02c_naskfunc_WINE_OUTS}_t.txt
)

add_dependencies(05_day_harib02c_naskfunc_od 05_day_harib02c_naskfunc)
add_dependencies(05_day_harib02c_naskfunc_od 05_day_harib02c_naskfunc_wine)
add_dependencies(od 05_day_harib02c_naskfunc_od)

#----------------------------------------------------------
