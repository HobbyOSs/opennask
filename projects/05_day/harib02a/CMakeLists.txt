#----------------------------------------------------------
message(STATUS "Entering directory projects/05_day/harib02a/")

set(FONT ${RAKUSK_EXECUTABLE} --makefont)
set(B2O  ${RAKUSK_EXECUTABLE} --bin2obj)
set(CONV ${root_BINARY_DIR}/objconv/objconv)
set(05_day_harib02a_WILDOBJ ${root_BINARY_DIR}/projects/05_day/harib02a/*.o)

# Full OS build rules
set(05_day_harib02a_OS    ${root_BINARY_DIR}/projects/05_day/harib02a/os.img)
set(05_day_harib02a_SYS   ${root_BINARY_DIR}/projects/05_day/harib02a/os.sys)
set(05_day_harib02a_IPLB  ${root_BINARY_DIR}/projects/05_day/harib02a/ipl.bin)
set(05_day_harib02a_IPLS  ${root_SOURCE_DIR}/projects/05_day/harib02a/ipl10.nas)
set(05_day_harib02a_HEADB ${root_BINARY_DIR}/projects/05_day/harib02a/asmhead.bin)
set(05_day_harib02a_HEADS ${root_SOURCE_DIR}/projects/05_day/harib02a/asmhead.nas)
set(05_day_harib02a_BOOTB ${root_BINARY_DIR}/projects/05_day/harib02a/boot.bin)
set(05_day_harib02a_CCO   ${root_BINARY_DIR}/projects/05_day/harib02a/boot.o)
set(05_day_harib02a_CCS   ${root_SOURCE_DIR}/projects/05_day/harib02a/*.c)
set(05_day_harib02a_LDS   ${root_SOURCE_DIR}/projects/os.lds)
set(05_day_harib02a_FUNCS ${root_SOURCE_DIR}/projects/05_day/harib02a/naskfunc.nas)
set(05_day_harib02a_FUNCO ${root_BINARY_DIR}/projects/05_day/harib02a/naskfunc.o)

add_custom_target(05_day_harib02a_run
  COMMAND ${QEMU} ${QEMUOPT} ${05_day_harib02a_OS}
  DEPENDS 05_day_harib02a_img
)
add_custom_target(05_day_harib02a_debug
  COMMAND ${QEMU} -s -S ${QEMUOPT} ${05_day_harib02a_OS} ${QEMU_DEBUG_OPT}
  DEPENDS 05_day_harib02a_img
)
add_custom_target(05_day_harib02a_clean
  COMMAND rm -f ${05_day_harib02a_OS}
  COMMAND rm -f ${05_day_harib02a_SYS}
  COMMAND rm -f ${05_day_harib02a_IPLB}
  COMMAND rm -f ${05_day_harib02a_HEADB}
  COMMAND rm -f ${05_day_harib02a_BOOTB}
  COMMAND rm -f ${05_day_harib02a_WILDOBJ}
  COMMAND rm -f ${root_BINARY_DIR}/projects/05_day/harib02a/ipl10.bin
  COMMAND rm -f ${root_BINARY_DIR}/projects/05_day/harib02a/asmhead.bin
  COMMAND rm -f ${root_BINARY_DIR}/projects/05_day/harib02a/*_wine.bin
  COMMAND rm -f ${root_BINARY_DIR}/projects/05_day/harib02a/*.txt
)
add_custom_target(05_day_harib02a_ipl_build
  COMMAND ${RAKUSK_EXECUTABLE} ${05_day_harib02a_IPLS} ${05_day_harib02a_IPLB}
  DEPENDS ${05_day_harib02a_IPLS}
)
add_custom_command(
  OUTPUT ${05_day_harib02a_HEADB}
  COMMAND ${RAKUSK_EXECUTABLE} ${05_day_harib02a_HEADS} ${05_day_harib02a_HEADB}
  DEPENDS ${05_day_harib02a_HEADS}
  COMMENT "Generating asmhead.bin for 05_day_harib02a"
)

add_custom_target(05_day_harib02a_sys
  COMMAND ${RAKUSK_EXECUTABLE} ${05_day_harib02a_FUNCS} ${05_day_harib02a_FUNCO}
  COMMAND gcc ${BINOPT} -T ${05_day_harib02a_LDS} ${05_day_harib02a_CCS} ${05_day_harib02a_WILDOBJ} -o ${05_day_harib02a_BOOTB}
  COMMAND cat ${05_day_harib02a_HEADB} ${05_day_harib02a_BOOTB} > ${05_day_harib02a_SYS}
  DEPENDS ${05_day_harib02a_HEADB} 05_day_harib02a_ipl_build
  DEPENDS ${05_day_harib02a_FUNCO}
)
add_custom_target(05_day_harib02a_img
  COMMAND mformat -f 1440 -l HARIBOTEOS -N 0xffffffff -C -B ${05_day_harib02a_IPLB} -i ${05_day_harib02a_OS}
  COMMAND mcopy -i ${05_day_harib02a_OS} ${05_day_harib02a_SYS} ::
  DEPENDS 05_day_harib02a_sys
)

########### next target ###############
set(05_day_harib02a_asmhead_SRCS ${root_SOURCE_DIR}/projects/05_day/harib02a/asmhead.nas)
set(05_day_harib02a_asmhead_OUTS ${root_BINARY_DIR}/projects/05_day/harib02a/asmhead.bin)

add_custom_target(05_day_harib02a_asmhead
  COMMAND ${RAKUSK_EXECUTABLE} ${05_day_harib02a_asmhead_SRCS} ${05_day_harib02a_asmhead_OUTS}
  DEPENDS ${05_day_harib02a_asmhead_SRCS}
)

set(05_day_harib02a_asmhead_WINE_OUTS ${root_BINARY_DIR}/projects/05_day/harib02a/asmhead_wine.bin)
add_custom_target(05_day_harib02a_asmhead_wine
  COMMAND ${WINE} ${WINE_NASK} ${05_day_harib02a_asmhead_SRCS} ${05_day_harib02a_asmhead_WINE_OUTS}
)
add_dependencies(wine 05_day_harib02a_asmhead_wine)

add_custom_target(05_day_harib02a_asmhead_od
  COMMAND ${OD} -t x1 ${05_day_harib02a_asmhead_OUTS}      > ${05_day_harib02a_asmhead_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${05_day_harib02a_asmhead_WINE_OUTS} > ${05_day_harib02a_asmhead_WINE_OUTS}_t.txt
  COMMAND diff -s ${05_day_harib02a_asmhead_OUTS}_f.txt ${05_day_harib02a_asmhead_WINE_OUTS}_t.txt
)

add_dependencies(05_day_harib02a_asmhead_od 05_day_harib02a_asmhead)
add_dependencies(05_day_harib02a_asmhead_od 05_day_harib02a_asmhead_wine)
add_dependencies(od 05_day_harib02a_asmhead_od)

#----------------------------------------------------------
########### next target ###############
set(05_day_harib02a_ipl10_SRCS ${root_SOURCE_DIR}/projects/05_day/harib02a/ipl10.nas)
set(05_day_harib02a_ipl10_OUTS ${root_BINARY_DIR}/projects/05_day/harib02a/ipl10.bin)

add_custom_target(05_day_harib02a_ipl10
  COMMAND ${RAKUSK_EXECUTABLE} ${05_day_harib02a_ipl10_SRCS} ${05_day_harib02a_ipl10_OUTS}
  DEPENDS ${05_day_harib02a_ipl10_SRCS}
)

set(05_day_harib02a_ipl10_WINE_OUTS ${root_BINARY_DIR}/projects/05_day/harib02a/ipl10_wine.bin)
add_custom_target(05_day_harib02a_ipl10_wine
  COMMAND ${WINE} ${WINE_NASK} ${05_day_harib02a_ipl10_SRCS} ${05_day_harib02a_ipl10_WINE_OUTS}
)
add_dependencies(wine 05_day_harib02a_ipl10_wine)

add_custom_target(05_day_harib02a_ipl10_od
  COMMAND ${OD} -t x1 ${05_day_harib02a_ipl10_OUTS}      > ${05_day_harib02a_ipl10_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${05_day_harib02a_ipl10_WINE_OUTS} > ${05_day_harib02a_ipl10_WINE_OUTS}_t.txt
  COMMAND diff -s ${05_day_harib02a_ipl10_OUTS}_f.txt ${05_day_harib02a_ipl10_WINE_OUTS}_t.txt
)

add_dependencies(05_day_harib02a_ipl10_od 05_day_harib02a_ipl10)
add_dependencies(05_day_harib02a_ipl10_od 05_day_harib02a_ipl10_wine)
add_dependencies(od 05_day_harib02a_ipl10_od)

#----------------------------------------------------------
########### next target ###############
set(05_day_harib02a_naskfunc_SRCS ${root_SOURCE_DIR}/projects/05_day/harib02a/naskfunc.nas)
set(05_day_harib02a_naskfunc_OUTS ${root_BINARY_DIR}/projects/05_day/harib02a/naskfunc.img)

add_custom_target(05_day_harib02a_naskfunc
  COMMAND ${RAKUSK_EXECUTABLE} ${05_day_harib02a_naskfunc_SRCS} ${05_day_harib02a_naskfunc_OUTS}
  DEPENDS ${05_day_harib02a_naskfunc_SRCS}
)

add_dependencies(images 05_day_harib02a_naskfunc)
set(05_day_harib02a_naskfunc_WINE_OUTS ${root_BINARY_DIR}/projects/05_day/harib02a/naskfunc_wine.img)
add_custom_target(05_day_harib02a_naskfunc_wine
  COMMAND ${WINE} ${WINE_NASK} ${05_day_harib02a_naskfunc_SRCS} ${05_day_harib02a_naskfunc_WINE_OUTS}
)
add_dependencies(wine 05_day_harib02a_naskfunc_wine)

add_custom_target(05_day_harib02a_naskfunc_od
  COMMAND ${OD} -t x1 ${05_day_harib02a_naskfunc_OUTS}      > ${05_day_harib02a_naskfunc_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${05_day_harib02a_naskfunc_WINE_OUTS} > ${05_day_harib02a_naskfunc_WINE_OUTS}_t.txt
  COMMAND diff -s ${05_day_harib02a_naskfunc_OUTS}_f.txt ${05_day_harib02a_naskfunc_WINE_OUTS}_t.txt
)

add_dependencies(05_day_harib02a_naskfunc_od 05_day_harib02a_naskfunc)
add_dependencies(05_day_harib02a_naskfunc_od 05_day_harib02a_naskfunc_wine)
add_dependencies(od 05_day_harib02a_naskfunc_od)

#----------------------------------------------------------
