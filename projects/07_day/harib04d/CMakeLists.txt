#----------------------------------------------------------
message(STATUS "Entering directory projects/07_day/harib04d/")

set(FONT ${root_BINARY_DIR}/src/makefont)
set(B2O  ${root_BINARY_DIR}/src/bin2obj)
set(CONV ${root_BINARY_DIR}/objconv/objconv)
set(07_day_harib04d_WILDOBJ ${root_BINARY_DIR}/projects/07_day/harib04d/*.o)

# Full OS build rules
set(07_day_harib04d_OS    ${root_BINARY_DIR}/projects/07_day/harib04d/os.img)
set(07_day_harib04d_SYS   ${root_BINARY_DIR}/projects/07_day/harib04d/os.sys)
set(07_day_harib04d_IPLB  ${root_BINARY_DIR}/projects/07_day/harib04d/ipl.bin)
set(07_day_harib04d_IPLS  ${root_SOURCE_DIR}/projects/07_day/harib04d/ipl10.nas)
set(07_day_harib04d_HEADB ${root_BINARY_DIR}/projects/07_day/harib04d/asmhead.bin)
set(07_day_harib04d_HEADS ${root_SOURCE_DIR}/projects/07_day/harib04d/asmhead.nas)
set(07_day_harib04d_BOOTB ${root_BINARY_DIR}/projects/07_day/harib04d/boot.bin)
set(07_day_harib04d_CCO   ${root_BINARY_DIR}/projects/07_day/harib04d/boot.o)
set(07_day_harib04d_CCS   ${root_SOURCE_DIR}/projects/07_day/harib04d/*.c)
set(07_day_harib04d_LDS   ${root_SOURCE_DIR}/projects/os.lds)
set(07_day_harib04d_FUNCS ${root_SOURCE_DIR}/projects/07_day/harib04d/naskfunc.nas)
set(07_day_harib04d_FUNCO ${root_BINARY_DIR}/projects/07_day/harib04d/naskfunc.o)
set(07_day_harib04d_FONTS ${root_SOURCE_DIR}/projects/07_day/harib04d/hankaku.txt)
set(07_day_harib04d_FONTB ${root_BINARY_DIR}/projects/07_day/harib04d/hankaku.bin)
set(07_day_harib04d_FONTO ${root_BINARY_DIR}/projects/07_day/harib04d/hankaku.o)
set(07_day_harib04d_LIBGE ${root_BINARY_DIR}/golibc/libgo.a)
set(07_day_harib04d_LIBGC ${root_BINARY_DIR}/projects/07_day/harib04d/libgo.o)

add_custom_target(07_day_harib04d_run
  COMMAND ${QEMU} ${QEMUOPT} ${07_day_harib04d_OS}
  DEPENDS 07_day_harib04d_img
)
add_custom_target(07_day_harib04d_debug
  COMMAND ${QEMU} -s -S ${QEMUOPT} ${07_day_harib04d_OS} ${QEMU_DEBUG_OPT}
  DEPENDS 07_day_harib04d_img
)
add_custom_target(07_day_harib04d_clean
  COMMAND rm -f ${07_day_harib04d_OS}
  COMMAND rm -f ${07_day_harib04d_SYS}
  COMMAND rm -f ${07_day_harib04d_IPLB}
  COMMAND rm -f ${07_day_harib04d_HEADB}
  COMMAND rm -f ${07_day_harib04d_BOOTB}
  COMMAND rm -f ${07_day_harib04d_WILDOBJ}
  COMMAND rm -f ${root_BINARY_DIR}/projects/07_day/harib04d/ipl10.bin
  COMMAND rm -f ${root_BINARY_DIR}/projects/07_day/harib04d/asmhead.bin
  COMMAND rm -f ${root_BINARY_DIR}/projects/07_day/harib04d/*_wine.bin
  COMMAND rm -f ${root_BINARY_DIR}/projects/07_day/harib04d/*.txt
)
add_custom_target(07_day_harib04d_ipl_build
  COMMAND ${RAKUSK_EXECUTABLE} ${07_day_harib04d_IPLS} ${07_day_harib04d_IPLB}
  DEPENDS ${07_day_harib04d_IPLS}
)
add_custom_command(
  OUTPUT ${07_day_harib04d_HEADB}
  COMMAND ${RAKUSK_EXECUTABLE} ${07_day_harib04d_HEADS} ${07_day_harib04d_HEADB}
  DEPENDS ${07_day_harib04d_HEADS}
  COMMENT "Generating asmhead.bin for 07_day_harib04d"
)

add_custom_command(
  OUTPUT ${07_day_harib04d_FUNCO}
  COMMAND ${RAKUSK_EXECUTABLE} ${07_day_harib04d_FUNCS} ${07_day_harib04d_FUNCO}
  DEPENDS ${07_day_harib04d_FUNCS}
  COMMENT "Generating naskfunc.o for 07_day_harib04d"
)
add_custom_command(
  OUTPUT ${07_day_harib04d_FONTO}
  COMMAND ${FONT} ${07_day_harib04d_FONTS} ${07_day_harib04d_FONTB}
  COMMAND ${B2O}  ${07_day_harib04d_FONTB} ${07_day_harib04d_FONTO} _hankaku
  DEPENDS ${07_day_harib04d_FONTS}
  COMMENT "Generating hankaku.o for 07_day_harib04d"
)
add_custom_command(
  OUTPUT ${07_day_harib04d_LIBGC}
  COMMAND ${CONV} -fcoff32 -nu ${07_day_harib04d_LIBGE} ${07_day_harib04d_LIBGC}
  DEPENDS ${07_day_harib04d_LIBGE}
  COMMENT "Converting golibc for 07_day_harib04d"
)
add_custom_target(07_day_harib04d_sys
  COMMAND gcc ${BINOPT} -T ${07_day_harib04d_LDS} ${07_day_harib04d_CCS} ${07_day_harib04d_WILDOBJ} -o ${07_day_harib04d_BOOTB}
  COMMAND cat ${07_day_harib04d_HEADB} ${07_day_harib04d_BOOTB} > ${07_day_harib04d_SYS}
  DEPENDS ${07_day_harib04d_HEADB} 07_day_harib04d_ipl_build
  DEPENDS ${07_day_harib04d_FUNCO}
  DEPENDS ${07_day_harib04d_FONTO} ${07_day_harib04d_LIBGC}
)
add_custom_target(07_day_harib04d_img
  COMMAND mformat -f 1440 -l HARIBOTEOS -N 0xffffffff -C -B ${07_day_harib04d_IPLB} -i ${07_day_harib04d_OS}
  COMMAND mcopy -i ${07_day_harib04d_OS} ${07_day_harib04d_SYS} ::
  DEPENDS 07_day_harib04d_sys
)

########### next target ###############
set(07_day_harib04d_asmhead_SRCS ${root_SOURCE_DIR}/projects/07_day/harib04d/asmhead.nas)
set(07_day_harib04d_asmhead_OUTS ${root_BINARY_DIR}/projects/07_day/harib04d/asmhead.bin)

add_custom_target(07_day_harib04d_asmhead
  COMMAND ${RAKUSK_EXECUTABLE} ${07_day_harib04d_asmhead_SRCS} ${07_day_harib04d_asmhead_OUTS}
  DEPENDS ${07_day_harib04d_asmhead_SRCS}
)

set(07_day_harib04d_asmhead_WINE_OUTS ${root_BINARY_DIR}/projects/07_day/harib04d/asmhead_wine.bin)
add_custom_target(07_day_harib04d_asmhead_wine
  COMMAND ${WINE} ${WINE_NASK} ${07_day_harib04d_asmhead_SRCS} ${07_day_harib04d_asmhead_WINE_OUTS}
)
add_dependencies(wine 07_day_harib04d_asmhead_wine)

add_custom_target(07_day_harib04d_asmhead_od
  COMMAND ${OD} -t x1 ${07_day_harib04d_asmhead_OUTS}      > ${07_day_harib04d_asmhead_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${07_day_harib04d_asmhead_WINE_OUTS} > ${07_day_harib04d_asmhead_WINE_OUTS}_t.txt
  COMMAND diff -s ${07_day_harib04d_asmhead_OUTS}_f.txt ${07_day_harib04d_asmhead_WINE_OUTS}_t.txt
)

add_dependencies(07_day_harib04d_asmhead_od 07_day_harib04d_asmhead)
add_dependencies(07_day_harib04d_asmhead_od 07_day_harib04d_asmhead_wine)
add_dependencies(od 07_day_harib04d_asmhead_od)

#----------------------------------------------------------
########### next target ###############
set(07_day_harib04d_ipl10_SRCS ${root_SOURCE_DIR}/projects/07_day/harib04d/ipl10.nas)
set(07_day_harib04d_ipl10_OUTS ${root_BINARY_DIR}/projects/07_day/harib04d/ipl10.bin)

add_custom_target(07_day_harib04d_ipl10
  COMMAND ${RAKUSK_EXECUTABLE} ${07_day_harib04d_ipl10_SRCS} ${07_day_harib04d_ipl10_OUTS}
  DEPENDS ${07_day_harib04d_ipl10_SRCS}
)

set(07_day_harib04d_ipl10_WINE_OUTS ${root_BINARY_DIR}/projects/07_day/harib04d/ipl10_wine.bin)
add_custom_target(07_day_harib04d_ipl10_wine
  COMMAND ${WINE} ${WINE_NASK} ${07_day_harib04d_ipl10_SRCS} ${07_day_harib04d_ipl10_WINE_OUTS}
)
add_dependencies(wine 07_day_harib04d_ipl10_wine)

add_custom_target(07_day_harib04d_ipl10_od
  COMMAND ${OD} -t x1 ${07_day_harib04d_ipl10_OUTS}      > ${07_day_harib04d_ipl10_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${07_day_harib04d_ipl10_WINE_OUTS} > ${07_day_harib04d_ipl10_WINE_OUTS}_t.txt
  COMMAND diff -s ${07_day_harib04d_ipl10_OUTS}_f.txt ${07_day_harib04d_ipl10_WINE_OUTS}_t.txt
)

add_dependencies(07_day_harib04d_ipl10_od 07_day_harib04d_ipl10)
add_dependencies(07_day_harib04d_ipl10_od 07_day_harib04d_ipl10_wine)
add_dependencies(od 07_day_harib04d_ipl10_od)

#----------------------------------------------------------
########### next target ###############
set(07_day_harib04d_naskfunc_SRCS ${root_SOURCE_DIR}/projects/07_day/harib04d/naskfunc.nas)
set(07_day_harib04d_naskfunc_OUTS ${root_BINARY_DIR}/projects/07_day/harib04d/naskfunc.img)

add_custom_target(07_day_harib04d_naskfunc
  COMMAND ${RAKUSK_EXECUTABLE} ${07_day_harib04d_naskfunc_SRCS} ${07_day_harib04d_naskfunc_OUTS}
  DEPENDS ${07_day_harib04d_naskfunc_SRCS}
)

add_dependencies(images 07_day_harib04d_naskfunc)
set(07_day_harib04d_naskfunc_WINE_OUTS ${root_BINARY_DIR}/projects/07_day/harib04d/naskfunc_wine.img)
add_custom_target(07_day_harib04d_naskfunc_wine
  COMMAND ${WINE} ${WINE_NASK} ${07_day_harib04d_naskfunc_SRCS} ${07_day_harib04d_naskfunc_WINE_OUTS}
)
add_dependencies(wine 07_day_harib04d_naskfunc_wine)

add_custom_target(07_day_harib04d_naskfunc_od
  COMMAND ${OD} -t x1 ${07_day_harib04d_naskfunc_OUTS}      > ${07_day_harib04d_naskfunc_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${07_day_harib04d_naskfunc_WINE_OUTS} > ${07_day_harib04d_naskfunc_WINE_OUTS}_t.txt
  COMMAND diff -s ${07_day_harib04d_naskfunc_OUTS}_f.txt ${07_day_harib04d_naskfunc_WINE_OUTS}_t.txt
)

add_dependencies(07_day_harib04d_naskfunc_od 07_day_harib04d_naskfunc)
add_dependencies(07_day_harib04d_naskfunc_od 07_day_harib04d_naskfunc_wine)
add_dependencies(od 07_day_harib04d_naskfunc_od)

#----------------------------------------------------------
