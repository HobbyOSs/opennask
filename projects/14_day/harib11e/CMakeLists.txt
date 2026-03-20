#----------------------------------------------------------
message(STATUS "Entering directory projects/14_day/harib11e/")

set(FONT ${RAKUSK_EXECUTABLE} --makefont)
set(B2O  ${RAKUSK_EXECUTABLE} --bin2obj)
set(CONV ${root_BINARY_DIR}/objconv/objconv)
set(14_day_harib11e_WILDOBJ ${root_BINARY_DIR}/projects/14_day/harib11e/*.o)

# Full OS build rules
set(14_day_harib11e_OS    ${root_BINARY_DIR}/projects/14_day/harib11e/os.img)
set(14_day_harib11e_SYS   ${root_BINARY_DIR}/projects/14_day/harib11e/os.sys)
set(14_day_harib11e_IPLB  ${root_BINARY_DIR}/projects/14_day/harib11e/ipl.bin)
set(14_day_harib11e_IPLS  ${root_SOURCE_DIR}/projects/14_day/harib11e/ipl10.nas)
set(14_day_harib11e_HEADB ${root_BINARY_DIR}/projects/14_day/harib11e/asmhead.bin)
set(14_day_harib11e_HEADS ${root_SOURCE_DIR}/projects/14_day/harib11e/asmhead.nas)
set(14_day_harib11e_BOOTB ${root_BINARY_DIR}/projects/14_day/harib11e/boot.bin)
set(14_day_harib11e_CCO   ${root_BINARY_DIR}/projects/14_day/harib11e/boot.o)
set(14_day_harib11e_CCS   ${root_SOURCE_DIR}/projects/14_day/harib11e/*.c)
set(14_day_harib11e_LDS   ${root_SOURCE_DIR}/projects/os.lds)
set(14_day_harib11e_FUNCS ${root_SOURCE_DIR}/projects/14_day/harib11e/naskfunc.nas)
set(14_day_harib11e_FUNCO ${root_BINARY_DIR}/projects/14_day/harib11e/naskfunc.o)
set(14_day_harib11e_FONTS ${root_SOURCE_DIR}/projects/14_day/harib11e/hankaku.txt)
set(14_day_harib11e_FONTB ${root_BINARY_DIR}/projects/14_day/harib11e/hankaku.bin)
set(14_day_harib11e_FONTO ${root_BINARY_DIR}/projects/14_day/harib11e/hankaku.o)
set(14_day_harib11e_LIBGE ${root_BINARY_DIR}/golibc/libgo.a)
set(14_day_harib11e_LIBGC ${root_BINARY_DIR}/projects/14_day/harib11e/libgo.o)

add_custom_target(14_day_harib11e_run
  COMMAND ${QEMU} ${QEMUOPT} ${14_day_harib11e_OS}
  DEPENDS 14_day_harib11e_img
)
add_custom_target(14_day_harib11e_debug
  COMMAND ${QEMU} -s -S ${QEMUOPT} ${14_day_harib11e_OS} ${QEMU_DEBUG_OPT}
  DEPENDS 14_day_harib11e_img
)
add_custom_target(14_day_harib11e_clean
  COMMAND rm -f ${14_day_harib11e_OS}
  COMMAND rm -f ${14_day_harib11e_SYS}
  COMMAND rm -f ${14_day_harib11e_IPLB}
  COMMAND rm -f ${14_day_harib11e_HEADB}
  COMMAND rm -f ${14_day_harib11e_BOOTB}
  COMMAND rm -f ${14_day_harib11e_WILDOBJ}
  COMMAND rm -f ${root_BINARY_DIR}/projects/14_day/harib11e/ipl10.bin
  COMMAND rm -f ${root_BINARY_DIR}/projects/14_day/harib11e/asmhead.bin
  COMMAND rm -f ${root_BINARY_DIR}/projects/14_day/harib11e/*_wine.bin
  COMMAND rm -f ${root_BINARY_DIR}/projects/14_day/harib11e/*.txt
)
add_custom_target(14_day_harib11e_ipl_build
  COMMAND ${RAKUSK_EXECUTABLE} ${14_day_harib11e_IPLS} ${14_day_harib11e_IPLB}
  DEPENDS ${14_day_harib11e_IPLS}
)
add_custom_command(
  OUTPUT ${14_day_harib11e_HEADB}
  COMMAND ${RAKUSK_EXECUTABLE} ${14_day_harib11e_HEADS} ${14_day_harib11e_HEADB}
  DEPENDS ${14_day_harib11e_HEADS}
  COMMENT "Generating asmhead.bin for 14_day_harib11e"
)

add_custom_target(14_day_harib11e_sys
  COMMAND ${FONT} ${14_day_harib11e_FONTS} ${14_day_harib11e_FONTB}
  COMMAND ${B2O}  ${14_day_harib11e_FONTB} ${14_day_harib11e_FONTO} _hankaku
  COMMAND ${CONV} -fcoff32 -nu ${14_day_harib11e_LIBGE} ${14_day_harib11e_LIBGC}
  COMMAND ${RAKUSK_EXECUTABLE} ${14_day_harib11e_FUNCS} ${14_day_harib11e_FUNCO}
  COMMAND gcc ${BINOPT} -T ${14_day_harib11e_LDS} ${14_day_harib11e_CCS} ${14_day_harib11e_WILDOBJ} -o ${14_day_harib11e_BOOTB}
  COMMAND cat ${14_day_harib11e_HEADB} ${14_day_harib11e_BOOTB} > ${14_day_harib11e_SYS}
  DEPENDS ${14_day_harib11e_HEADB} 14_day_harib11e_ipl_build
  DEPENDS ${14_day_harib11e_FUNCO}
  DEPENDS ${14_day_harib11e_FONTO}
)
add_custom_target(14_day_harib11e_img
  COMMAND mformat -f 1440 -l HARIBOTEOS -N 0xffffffff -C -B ${14_day_harib11e_IPLB} -i ${14_day_harib11e_OS}
  COMMAND mcopy -i ${14_day_harib11e_OS} ${14_day_harib11e_SYS} ::
  DEPENDS 14_day_harib11e_sys
)

########### next target ###############
set(14_day_harib11e_asmhead_SRCS ${root_SOURCE_DIR}/projects/14_day/harib11e/asmhead.nas)
set(14_day_harib11e_asmhead_OUTS ${root_BINARY_DIR}/projects/14_day/harib11e/asmhead.bin)

add_custom_target(14_day_harib11e_asmhead
  COMMAND ${RAKUSK_EXECUTABLE} ${14_day_harib11e_asmhead_SRCS} ${14_day_harib11e_asmhead_OUTS}
  DEPENDS ${14_day_harib11e_asmhead_SRCS}
)

set(14_day_harib11e_asmhead_WINE_OUTS ${root_BINARY_DIR}/projects/14_day/harib11e/asmhead_wine.bin)
add_custom_target(14_day_harib11e_asmhead_wine
  COMMAND ${WINE} ${WINE_NASK} ${14_day_harib11e_asmhead_SRCS} ${14_day_harib11e_asmhead_WINE_OUTS}
)
add_dependencies(wine 14_day_harib11e_asmhead_wine)

add_custom_target(14_day_harib11e_asmhead_od
  COMMAND ${OD} -t x1 ${14_day_harib11e_asmhead_OUTS}      > ${14_day_harib11e_asmhead_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${14_day_harib11e_asmhead_WINE_OUTS} > ${14_day_harib11e_asmhead_WINE_OUTS}_t.txt
  COMMAND diff -s ${14_day_harib11e_asmhead_OUTS}_f.txt ${14_day_harib11e_asmhead_WINE_OUTS}_t.txt
)

add_dependencies(14_day_harib11e_asmhead_od 14_day_harib11e_asmhead)
add_dependencies(14_day_harib11e_asmhead_od 14_day_harib11e_asmhead_wine)
add_dependencies(od 14_day_harib11e_asmhead_od)

#----------------------------------------------------------
########### next target ###############
set(14_day_harib11e_ipl10_SRCS ${root_SOURCE_DIR}/projects/14_day/harib11e/ipl10.nas)
set(14_day_harib11e_ipl10_OUTS ${root_BINARY_DIR}/projects/14_day/harib11e/ipl10.bin)

add_custom_target(14_day_harib11e_ipl10
  COMMAND ${RAKUSK_EXECUTABLE} ${14_day_harib11e_ipl10_SRCS} ${14_day_harib11e_ipl10_OUTS}
  DEPENDS ${14_day_harib11e_ipl10_SRCS}
)

set(14_day_harib11e_ipl10_WINE_OUTS ${root_BINARY_DIR}/projects/14_day/harib11e/ipl10_wine.bin)
add_custom_target(14_day_harib11e_ipl10_wine
  COMMAND ${WINE} ${WINE_NASK} ${14_day_harib11e_ipl10_SRCS} ${14_day_harib11e_ipl10_WINE_OUTS}
)
add_dependencies(wine 14_day_harib11e_ipl10_wine)

add_custom_target(14_day_harib11e_ipl10_od
  COMMAND ${OD} -t x1 ${14_day_harib11e_ipl10_OUTS}      > ${14_day_harib11e_ipl10_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${14_day_harib11e_ipl10_WINE_OUTS} > ${14_day_harib11e_ipl10_WINE_OUTS}_t.txt
  COMMAND diff -s ${14_day_harib11e_ipl10_OUTS}_f.txt ${14_day_harib11e_ipl10_WINE_OUTS}_t.txt
)

add_dependencies(14_day_harib11e_ipl10_od 14_day_harib11e_ipl10)
add_dependencies(14_day_harib11e_ipl10_od 14_day_harib11e_ipl10_wine)
add_dependencies(od 14_day_harib11e_ipl10_od)

#----------------------------------------------------------
########### next target ###############
set(14_day_harib11e_naskfunc_SRCS ${root_SOURCE_DIR}/projects/14_day/harib11e/naskfunc.nas)
set(14_day_harib11e_naskfunc_OUTS ${root_BINARY_DIR}/projects/14_day/harib11e/naskfunc.img)

add_custom_target(14_day_harib11e_naskfunc
  COMMAND ${RAKUSK_EXECUTABLE} ${14_day_harib11e_naskfunc_SRCS} ${14_day_harib11e_naskfunc_OUTS}
  DEPENDS ${14_day_harib11e_naskfunc_SRCS}
)

add_dependencies(images 14_day_harib11e_naskfunc)
set(14_day_harib11e_naskfunc_WINE_OUTS ${root_BINARY_DIR}/projects/14_day/harib11e/naskfunc_wine.img)
add_custom_target(14_day_harib11e_naskfunc_wine
  COMMAND ${WINE} ${WINE_NASK} ${14_day_harib11e_naskfunc_SRCS} ${14_day_harib11e_naskfunc_WINE_OUTS}
)
add_dependencies(wine 14_day_harib11e_naskfunc_wine)

add_custom_target(14_day_harib11e_naskfunc_od
  COMMAND ${OD} -t x1 ${14_day_harib11e_naskfunc_OUTS}      > ${14_day_harib11e_naskfunc_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${14_day_harib11e_naskfunc_WINE_OUTS} > ${14_day_harib11e_naskfunc_WINE_OUTS}_t.txt
  COMMAND diff -s ${14_day_harib11e_naskfunc_OUTS}_f.txt ${14_day_harib11e_naskfunc_WINE_OUTS}_t.txt
)

add_dependencies(14_day_harib11e_naskfunc_od 14_day_harib11e_naskfunc)
add_dependencies(14_day_harib11e_naskfunc_od 14_day_harib11e_naskfunc_wine)
add_dependencies(od 14_day_harib11e_naskfunc_od)

#----------------------------------------------------------
