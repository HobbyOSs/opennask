#----------------------------------------------------------
message(STATUS "Entering directory projects/05_day/harib02b/")

set(FONT ${RAKUSK_EXECUTABLE} --makefont)
set(B2O  ${RAKUSK_EXECUTABLE} --bin2obj)
set(CONV ${root_BINARY_DIR}/objconv/objconv)
set(05_day_harib02b_WILDOBJ ${root_BINARY_DIR}/projects/05_day/harib02b/*.o)

# Full OS build rules
set(05_day_harib02b_OS    ${root_BINARY_DIR}/projects/05_day/harib02b/os.img)
set(05_day_harib02b_SYS   ${root_BINARY_DIR}/projects/05_day/harib02b/os.sys)
set(05_day_harib02b_IPLB  ${root_BINARY_DIR}/projects/05_day/harib02b/ipl.bin)
set(05_day_harib02b_IPLS  ${root_SOURCE_DIR}/projects/05_day/harib02b/ipl10.nas)
set(05_day_harib02b_HEADB ${root_BINARY_DIR}/projects/05_day/harib02b/asmhead.bin)
set(05_day_harib02b_HEADS ${root_SOURCE_DIR}/projects/05_day/harib02b/asmhead.nas)
set(05_day_harib02b_BOOTB ${root_BINARY_DIR}/projects/05_day/harib02b/boot.bin)
set(05_day_harib02b_CCO   ${root_BINARY_DIR}/projects/05_day/harib02b/boot.o)
set(05_day_harib02b_CCS   ${root_SOURCE_DIR}/projects/05_day/harib02b/*.c)
set(05_day_harib02b_LDS   ${root_SOURCE_DIR}/projects/os.lds)
set(05_day_harib02b_FUNCS ${root_SOURCE_DIR}/projects/05_day/harib02b/naskfunc.nas)
set(05_day_harib02b_FUNCO ${root_BINARY_DIR}/projects/05_day/harib02b/naskfunc.o)

add_custom_target(05_day_harib02b_run
  COMMAND ${QEMU} ${QEMUOPT} ${05_day_harib02b_OS}
  DEPENDS 05_day_harib02b_img
)
add_custom_target(05_day_harib02b_debug
  COMMAND ${QEMU} -s -S ${QEMUOPT} ${05_day_harib02b_OS} ${QEMU_DEBUG_OPT}
  DEPENDS 05_day_harib02b_img
)
add_custom_target(05_day_harib02b_clean
  COMMAND rm -f ${05_day_harib02b_OS}
  COMMAND rm -f ${05_day_harib02b_SYS}
  COMMAND rm -f ${05_day_harib02b_IPLB}
  COMMAND rm -f ${05_day_harib02b_HEADB}
  COMMAND rm -f ${05_day_harib02b_BOOTB}
  COMMAND rm -f ${05_day_harib02b_WILDOBJ}
  COMMAND rm -f ${root_BINARY_DIR}/projects/05_day/harib02b/ipl10.bin
  COMMAND rm -f ${root_BINARY_DIR}/projects/05_day/harib02b/asmhead.bin
  COMMAND rm -f ${root_BINARY_DIR}/projects/05_day/harib02b/*_wine.bin
  COMMAND rm -f ${root_BINARY_DIR}/projects/05_day/harib02b/*.txt
)
add_custom_target(05_day_harib02b_ipl_build
  COMMAND ${RAKUSK_EXECUTABLE} ${05_day_harib02b_IPLS} ${05_day_harib02b_IPLB}
  DEPENDS ${05_day_harib02b_IPLS}
)
add_custom_command(
  OUTPUT ${05_day_harib02b_HEADB}
  COMMAND ${RAKUSK_EXECUTABLE} ${05_day_harib02b_HEADS} ${05_day_harib02b_HEADB}
  DEPENDS ${05_day_harib02b_HEADS}
  COMMENT "Generating asmhead.bin for 05_day_harib02b"
)

add_custom_target(05_day_harib02b_sys
  COMMAND ${RAKUSK_EXECUTABLE} ${05_day_harib02b_FUNCS} ${05_day_harib02b_FUNCO}
  COMMAND gcc ${BINOPT} -T ${05_day_harib02b_LDS} ${05_day_harib02b_CCS} ${05_day_harib02b_WILDOBJ} -o ${05_day_harib02b_BOOTB}
  COMMAND cat ${05_day_harib02b_HEADB} ${05_day_harib02b_BOOTB} > ${05_day_harib02b_SYS}
  DEPENDS ${05_day_harib02b_HEADB} 05_day_harib02b_ipl_build
  DEPENDS ${05_day_harib02b_FUNCO}
)
add_custom_target(05_day_harib02b_img
  COMMAND mformat -f 1440 -l HARIBOTEOS -N 0xffffffff -C -B ${05_day_harib02b_IPLB} -i ${05_day_harib02b_OS}
  COMMAND mcopy -i ${05_day_harib02b_OS} ${05_day_harib02b_SYS} ::
  DEPENDS 05_day_harib02b_sys
)

########### next target ###############
set(05_day_harib02b_asmhead_SRCS ${root_SOURCE_DIR}/projects/05_day/harib02b/asmhead.nas)
set(05_day_harib02b_asmhead_OUTS ${root_BINARY_DIR}/projects/05_day/harib02b/asmhead.bin)

add_custom_target(05_day_harib02b_asmhead
  COMMAND ${RAKUSK_EXECUTABLE} ${05_day_harib02b_asmhead_SRCS} ${05_day_harib02b_asmhead_OUTS}
  DEPENDS ${05_day_harib02b_asmhead_SRCS}
)

set(05_day_harib02b_asmhead_WINE_OUTS ${root_BINARY_DIR}/projects/05_day/harib02b/asmhead_wine.bin)
add_custom_target(05_day_harib02b_asmhead_wine
  COMMAND ${WINE} ${WINE_NASK} ${05_day_harib02b_asmhead_SRCS} ${05_day_harib02b_asmhead_WINE_OUTS}
)
add_dependencies(wine 05_day_harib02b_asmhead_wine)

add_custom_target(05_day_harib02b_asmhead_od
  COMMAND ${OD} -t x1 ${05_day_harib02b_asmhead_OUTS}      > ${05_day_harib02b_asmhead_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${05_day_harib02b_asmhead_WINE_OUTS} > ${05_day_harib02b_asmhead_WINE_OUTS}_t.txt
  COMMAND diff -s ${05_day_harib02b_asmhead_OUTS}_f.txt ${05_day_harib02b_asmhead_WINE_OUTS}_t.txt
)

add_dependencies(05_day_harib02b_asmhead_od 05_day_harib02b_asmhead)
add_dependencies(05_day_harib02b_asmhead_od 05_day_harib02b_asmhead_wine)
add_dependencies(od 05_day_harib02b_asmhead_od)

#----------------------------------------------------------
########### next target ###############
set(05_day_harib02b_ipl10_SRCS ${root_SOURCE_DIR}/projects/05_day/harib02b/ipl10.nas)
set(05_day_harib02b_ipl10_OUTS ${root_BINARY_DIR}/projects/05_day/harib02b/ipl10.bin)

add_custom_target(05_day_harib02b_ipl10
  COMMAND ${RAKUSK_EXECUTABLE} ${05_day_harib02b_ipl10_SRCS} ${05_day_harib02b_ipl10_OUTS}
  DEPENDS ${05_day_harib02b_ipl10_SRCS}
)

set(05_day_harib02b_ipl10_WINE_OUTS ${root_BINARY_DIR}/projects/05_day/harib02b/ipl10_wine.bin)
add_custom_target(05_day_harib02b_ipl10_wine
  COMMAND ${WINE} ${WINE_NASK} ${05_day_harib02b_ipl10_SRCS} ${05_day_harib02b_ipl10_WINE_OUTS}
)
add_dependencies(wine 05_day_harib02b_ipl10_wine)

add_custom_target(05_day_harib02b_ipl10_od
  COMMAND ${OD} -t x1 ${05_day_harib02b_ipl10_OUTS}      > ${05_day_harib02b_ipl10_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${05_day_harib02b_ipl10_WINE_OUTS} > ${05_day_harib02b_ipl10_WINE_OUTS}_t.txt
  COMMAND diff -s ${05_day_harib02b_ipl10_OUTS}_f.txt ${05_day_harib02b_ipl10_WINE_OUTS}_t.txt
)

add_dependencies(05_day_harib02b_ipl10_od 05_day_harib02b_ipl10)
add_dependencies(05_day_harib02b_ipl10_od 05_day_harib02b_ipl10_wine)
add_dependencies(od 05_day_harib02b_ipl10_od)

#----------------------------------------------------------
########### next target ###############
set(05_day_harib02b_naskfunc_SRCS ${root_SOURCE_DIR}/projects/05_day/harib02b/naskfunc.nas)
set(05_day_harib02b_naskfunc_OUTS ${root_BINARY_DIR}/projects/05_day/harib02b/naskfunc.img)

add_custom_target(05_day_harib02b_naskfunc
  COMMAND ${RAKUSK_EXECUTABLE} ${05_day_harib02b_naskfunc_SRCS} ${05_day_harib02b_naskfunc_OUTS}
  DEPENDS ${05_day_harib02b_naskfunc_SRCS}
)

add_dependencies(images 05_day_harib02b_naskfunc)
set(05_day_harib02b_naskfunc_WINE_OUTS ${root_BINARY_DIR}/projects/05_day/harib02b/naskfunc_wine.img)
add_custom_target(05_day_harib02b_naskfunc_wine
  COMMAND ${WINE} ${WINE_NASK} ${05_day_harib02b_naskfunc_SRCS} ${05_day_harib02b_naskfunc_WINE_OUTS}
)
add_dependencies(wine 05_day_harib02b_naskfunc_wine)

add_custom_target(05_day_harib02b_naskfunc_od
  COMMAND ${OD} -t x1 ${05_day_harib02b_naskfunc_OUTS}      > ${05_day_harib02b_naskfunc_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${05_day_harib02b_naskfunc_WINE_OUTS} > ${05_day_harib02b_naskfunc_WINE_OUTS}_t.txt
  COMMAND diff -s ${05_day_harib02b_naskfunc_OUTS}_f.txt ${05_day_harib02b_naskfunc_WINE_OUTS}_t.txt
)

add_dependencies(05_day_harib02b_naskfunc_od 05_day_harib02b_naskfunc)
add_dependencies(05_day_harib02b_naskfunc_od 05_day_harib02b_naskfunc_wine)
add_dependencies(od 05_day_harib02b_naskfunc_od)

#----------------------------------------------------------
