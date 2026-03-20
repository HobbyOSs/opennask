#----------------------------------------------------------
message(STATUS "Entering directory projects/07_day/harib04e/")

set(FONT ${RAKUSK_EXECUTABLE} --makefont)
set(B2O  ${RAKUSK_EXECUTABLE} --bin2obj)
set(CONV ${root_BINARY_DIR}/objconv/objconv)
set(07_day_harib04e_WILDOBJ ${root_BINARY_DIR}/projects/07_day/harib04e/*.o)

# Full OS build rules
set(07_day_harib04e_OS    ${root_BINARY_DIR}/projects/07_day/harib04e/os.img)
set(07_day_harib04e_SYS   ${root_BINARY_DIR}/projects/07_day/harib04e/os.sys)
set(07_day_harib04e_IPLB  ${root_BINARY_DIR}/projects/07_day/harib04e/ipl.bin)
set(07_day_harib04e_IPLS  ${root_SOURCE_DIR}/projects/07_day/harib04e/ipl10.nas)
set(07_day_harib04e_HEADB ${root_BINARY_DIR}/projects/07_day/harib04e/asmhead.bin)
set(07_day_harib04e_HEADS ${root_SOURCE_DIR}/projects/07_day/harib04e/asmhead.nas)
set(07_day_harib04e_BOOTB ${root_BINARY_DIR}/projects/07_day/harib04e/boot.bin)
set(07_day_harib04e_CCO   ${root_BINARY_DIR}/projects/07_day/harib04e/boot.o)
set(07_day_harib04e_CCS   ${root_SOURCE_DIR}/projects/07_day/harib04e/*.c)
set(07_day_harib04e_LDS   ${root_SOURCE_DIR}/projects/os.lds)
set(07_day_harib04e_FUNCS ${root_SOURCE_DIR}/projects/07_day/harib04e/naskfunc.nas)
set(07_day_harib04e_FUNCO ${root_BINARY_DIR}/projects/07_day/harib04e/naskfunc.o)
set(07_day_harib04e_FONTS ${root_SOURCE_DIR}/projects/07_day/harib04e/hankaku.txt)
set(07_day_harib04e_FONTB ${root_BINARY_DIR}/projects/07_day/harib04e/hankaku.bin)
set(07_day_harib04e_FONTO ${root_BINARY_DIR}/projects/07_day/harib04e/hankaku.o)
set(07_day_harib04e_LIBGE ${root_BINARY_DIR}/golibc/libgo.a)
set(07_day_harib04e_LIBGC ${root_BINARY_DIR}/projects/07_day/harib04e/libgo.o)

add_custom_target(07_day_harib04e_run
  COMMAND ${QEMU} ${QEMUOPT} ${07_day_harib04e_OS}
  DEPENDS 07_day_harib04e_img
)
add_custom_target(07_day_harib04e_debug
  COMMAND ${QEMU} -s -S ${QEMUOPT} ${07_day_harib04e_OS} ${QEMU_DEBUG_OPT}
  DEPENDS 07_day_harib04e_img
)
add_custom_target(07_day_harib04e_clean
  COMMAND rm -f ${07_day_harib04e_OS}
  COMMAND rm -f ${07_day_harib04e_SYS}
  COMMAND rm -f ${07_day_harib04e_IPLB}
  COMMAND rm -f ${07_day_harib04e_HEADB}
  COMMAND rm -f ${07_day_harib04e_BOOTB}
  COMMAND rm -f ${07_day_harib04e_WILDOBJ}
  COMMAND rm -f ${root_BINARY_DIR}/projects/07_day/harib04e/ipl10.bin
  COMMAND rm -f ${root_BINARY_DIR}/projects/07_day/harib04e/asmhead.bin
  COMMAND rm -f ${root_BINARY_DIR}/projects/07_day/harib04e/*_wine.bin
  COMMAND rm -f ${root_BINARY_DIR}/projects/07_day/harib04e/*.txt
)
add_custom_target(07_day_harib04e_ipl_build
  COMMAND ${RAKUSK_EXECUTABLE} ${07_day_harib04e_IPLS} ${07_day_harib04e_IPLB}
  DEPENDS ${07_day_harib04e_IPLS}
)
add_custom_command(
  OUTPUT ${07_day_harib04e_HEADB}
  COMMAND ${RAKUSK_EXECUTABLE} ${07_day_harib04e_HEADS} ${07_day_harib04e_HEADB}
  DEPENDS ${07_day_harib04e_HEADS}
  COMMENT "Generating asmhead.bin for 07_day_harib04e"
)

add_custom_target(07_day_harib04e_sys
  COMMAND ${FONT} ${07_day_harib04e_FONTS} ${07_day_harib04e_FONTB}
  COMMAND ${B2O}  ${07_day_harib04e_FONTB} ${07_day_harib04e_FONTO} _hankaku
  COMMAND ${CONV} -fcoff32 -nu ${07_day_harib04e_LIBGE} ${07_day_harib04e_LIBGC}
  COMMAND ${RAKUSK_EXECUTABLE} ${07_day_harib04e_FUNCS} ${07_day_harib04e_FUNCO}
  COMMAND gcc ${BINOPT} -T ${07_day_harib04e_LDS} ${07_day_harib04e_CCS} ${07_day_harib04e_WILDOBJ} -o ${07_day_harib04e_BOOTB}
  COMMAND cat ${07_day_harib04e_HEADB} ${07_day_harib04e_BOOTB} > ${07_day_harib04e_SYS}
  DEPENDS ${07_day_harib04e_HEADB} 07_day_harib04e_ipl_build
  DEPENDS ${07_day_harib04e_FUNCO}
  DEPENDS ${07_day_harib04e_FONTO}
)
add_custom_target(07_day_harib04e_img
  COMMAND mformat -f 1440 -l HARIBOTEOS -N 0xffffffff -C -B ${07_day_harib04e_IPLB} -i ${07_day_harib04e_OS}
  COMMAND mcopy -i ${07_day_harib04e_OS} ${07_day_harib04e_SYS} ::
  DEPENDS 07_day_harib04e_sys
)

########### next target ###############
set(07_day_harib04e_asmhead_SRCS ${root_SOURCE_DIR}/projects/07_day/harib04e/asmhead.nas)
set(07_day_harib04e_asmhead_OUTS ${root_BINARY_DIR}/projects/07_day/harib04e/asmhead.bin)

add_custom_target(07_day_harib04e_asmhead
  COMMAND ${RAKUSK_EXECUTABLE} ${07_day_harib04e_asmhead_SRCS} ${07_day_harib04e_asmhead_OUTS}
  DEPENDS ${07_day_harib04e_asmhead_SRCS}
)

set(07_day_harib04e_asmhead_WINE_OUTS ${root_BINARY_DIR}/projects/07_day/harib04e/asmhead_wine.bin)
add_custom_target(07_day_harib04e_asmhead_wine
  COMMAND ${WINE} ${WINE_NASK} ${07_day_harib04e_asmhead_SRCS} ${07_day_harib04e_asmhead_WINE_OUTS}
)
add_dependencies(wine 07_day_harib04e_asmhead_wine)

add_custom_target(07_day_harib04e_asmhead_od
  COMMAND ${OD} -t x1 ${07_day_harib04e_asmhead_OUTS}      > ${07_day_harib04e_asmhead_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${07_day_harib04e_asmhead_WINE_OUTS} > ${07_day_harib04e_asmhead_WINE_OUTS}_t.txt
  COMMAND diff -s ${07_day_harib04e_asmhead_OUTS}_f.txt ${07_day_harib04e_asmhead_WINE_OUTS}_t.txt
)

add_dependencies(07_day_harib04e_asmhead_od 07_day_harib04e_asmhead)
add_dependencies(07_day_harib04e_asmhead_od 07_day_harib04e_asmhead_wine)
add_dependencies(od 07_day_harib04e_asmhead_od)

#----------------------------------------------------------
########### next target ###############
set(07_day_harib04e_ipl10_SRCS ${root_SOURCE_DIR}/projects/07_day/harib04e/ipl10.nas)
set(07_day_harib04e_ipl10_OUTS ${root_BINARY_DIR}/projects/07_day/harib04e/ipl10.bin)

add_custom_target(07_day_harib04e_ipl10
  COMMAND ${RAKUSK_EXECUTABLE} ${07_day_harib04e_ipl10_SRCS} ${07_day_harib04e_ipl10_OUTS}
  DEPENDS ${07_day_harib04e_ipl10_SRCS}
)

set(07_day_harib04e_ipl10_WINE_OUTS ${root_BINARY_DIR}/projects/07_day/harib04e/ipl10_wine.bin)
add_custom_target(07_day_harib04e_ipl10_wine
  COMMAND ${WINE} ${WINE_NASK} ${07_day_harib04e_ipl10_SRCS} ${07_day_harib04e_ipl10_WINE_OUTS}
)
add_dependencies(wine 07_day_harib04e_ipl10_wine)

add_custom_target(07_day_harib04e_ipl10_od
  COMMAND ${OD} -t x1 ${07_day_harib04e_ipl10_OUTS}      > ${07_day_harib04e_ipl10_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${07_day_harib04e_ipl10_WINE_OUTS} > ${07_day_harib04e_ipl10_WINE_OUTS}_t.txt
  COMMAND diff -s ${07_day_harib04e_ipl10_OUTS}_f.txt ${07_day_harib04e_ipl10_WINE_OUTS}_t.txt
)

add_dependencies(07_day_harib04e_ipl10_od 07_day_harib04e_ipl10)
add_dependencies(07_day_harib04e_ipl10_od 07_day_harib04e_ipl10_wine)
add_dependencies(od 07_day_harib04e_ipl10_od)

#----------------------------------------------------------
########### next target ###############
set(07_day_harib04e_naskfunc_SRCS ${root_SOURCE_DIR}/projects/07_day/harib04e/naskfunc.nas)
set(07_day_harib04e_naskfunc_OUTS ${root_BINARY_DIR}/projects/07_day/harib04e/naskfunc.img)

add_custom_target(07_day_harib04e_naskfunc
  COMMAND ${RAKUSK_EXECUTABLE} ${07_day_harib04e_naskfunc_SRCS} ${07_day_harib04e_naskfunc_OUTS}
  DEPENDS ${07_day_harib04e_naskfunc_SRCS}
)

add_dependencies(images 07_day_harib04e_naskfunc)
set(07_day_harib04e_naskfunc_WINE_OUTS ${root_BINARY_DIR}/projects/07_day/harib04e/naskfunc_wine.img)
add_custom_target(07_day_harib04e_naskfunc_wine
  COMMAND ${WINE} ${WINE_NASK} ${07_day_harib04e_naskfunc_SRCS} ${07_day_harib04e_naskfunc_WINE_OUTS}
)
add_dependencies(wine 07_day_harib04e_naskfunc_wine)

add_custom_target(07_day_harib04e_naskfunc_od
  COMMAND ${OD} -t x1 ${07_day_harib04e_naskfunc_OUTS}      > ${07_day_harib04e_naskfunc_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${07_day_harib04e_naskfunc_WINE_OUTS} > ${07_day_harib04e_naskfunc_WINE_OUTS}_t.txt
  COMMAND diff -s ${07_day_harib04e_naskfunc_OUTS}_f.txt ${07_day_harib04e_naskfunc_WINE_OUTS}_t.txt
)

add_dependencies(07_day_harib04e_naskfunc_od 07_day_harib04e_naskfunc)
add_dependencies(07_day_harib04e_naskfunc_od 07_day_harib04e_naskfunc_wine)
add_dependencies(od 07_day_harib04e_naskfunc_od)

#----------------------------------------------------------
