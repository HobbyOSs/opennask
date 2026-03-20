#----------------------------------------------------------
message(STATUS "Entering directory projects/07_day/harib04f/")

set(FONT ${root_BINARY_DIR}/src/makefont)
set(B2O  ${root_BINARY_DIR}/src/bin2obj)
set(CONV ${root_BINARY_DIR}/objconv/objconv)
set(07_day_harib04f_WILDOBJ ${root_BINARY_DIR}/projects/07_day/harib04f/*.o)

# Full OS build rules
set(07_day_harib04f_OS    ${root_BINARY_DIR}/projects/07_day/harib04f/os.img)
set(07_day_harib04f_SYS   ${root_BINARY_DIR}/projects/07_day/harib04f/os.sys)
set(07_day_harib04f_IPLB  ${root_BINARY_DIR}/projects/07_day/harib04f/ipl.bin)
set(07_day_harib04f_IPLS  ${root_SOURCE_DIR}/projects/07_day/harib04f/ipl10.nas)
set(07_day_harib04f_HEADB ${root_BINARY_DIR}/projects/07_day/harib04f/asmhead.bin)
set(07_day_harib04f_HEADS ${root_SOURCE_DIR}/projects/07_day/harib04f/asmhead.nas)
set(07_day_harib04f_BOOTB ${root_BINARY_DIR}/projects/07_day/harib04f/boot.bin)
set(07_day_harib04f_CCO   ${root_BINARY_DIR}/projects/07_day/harib04f/boot.o)
set(07_day_harib04f_CCS   ${root_SOURCE_DIR}/projects/07_day/harib04f/*.c)
set(07_day_harib04f_LDS   ${root_SOURCE_DIR}/projects/os.lds)
set(07_day_harib04f_FUNCS ${root_SOURCE_DIR}/projects/07_day/harib04f/naskfunc.nas)
set(07_day_harib04f_FUNCO ${root_BINARY_DIR}/projects/07_day/harib04f/naskfunc.o)
set(07_day_harib04f_FONTS ${root_SOURCE_DIR}/projects/07_day/harib04f/hankaku.txt)
set(07_day_harib04f_FONTB ${root_BINARY_DIR}/projects/07_day/harib04f/hankaku.bin)
set(07_day_harib04f_FONTO ${root_BINARY_DIR}/projects/07_day/harib04f/hankaku.o)
set(07_day_harib04f_LIBGE ${root_BINARY_DIR}/golibc/libgo.a)
set(07_day_harib04f_LIBGC ${root_BINARY_DIR}/projects/07_day/harib04f/libgo.o)

add_custom_target(07_day_harib04f_run
  COMMAND ${QEMU} ${QEMUOPT} ${07_day_harib04f_OS}
  DEPENDS 07_day_harib04f_img
)
add_custom_target(07_day_harib04f_debug
  COMMAND ${QEMU} -s -S ${QEMUOPT} ${07_day_harib04f_OS} ${QEMU_DEBUG_OPT}
  DEPENDS 07_day_harib04f_img
)
add_custom_target(07_day_harib04f_clean
  COMMAND rm -f ${07_day_harib04f_OS}
  COMMAND rm -f ${07_day_harib04f_SYS}
  COMMAND rm -f ${07_day_harib04f_IPLB}
  COMMAND rm -f ${07_day_harib04f_HEADB}
  COMMAND rm -f ${07_day_harib04f_BOOTB}
  COMMAND rm -f ${07_day_harib04f_WILDOBJ}
  COMMAND rm -f ${root_BINARY_DIR}/projects/07_day/harib04f/ipl10.bin
  COMMAND rm -f ${root_BINARY_DIR}/projects/07_day/harib04f/asmhead.bin
  COMMAND rm -f ${root_BINARY_DIR}/projects/07_day/harib04f/*_wine.bin
  COMMAND rm -f ${root_BINARY_DIR}/projects/07_day/harib04f/*.txt
)
add_custom_target(07_day_harib04f_ipl_build
  COMMAND ${RAKUSK_EXECUTABLE} ${07_day_harib04f_IPLS} ${07_day_harib04f_IPLB}
  DEPENDS ${07_day_harib04f_IPLS}
)
add_custom_command(
  OUTPUT ${07_day_harib04f_HEADB}
  COMMAND ${RAKUSK_EXECUTABLE} ${07_day_harib04f_HEADS} ${07_day_harib04f_HEADB}
  DEPENDS ${07_day_harib04f_HEADS}
  COMMENT "Generating asmhead.bin for 07_day_harib04f"
)

add_custom_command(
  OUTPUT ${07_day_harib04f_FUNCO}
  COMMAND ${RAKUSK_EXECUTABLE} ${07_day_harib04f_FUNCS} ${07_day_harib04f_FUNCO}
  DEPENDS ${07_day_harib04f_FUNCS}
  COMMENT "Generating naskfunc.o for 07_day_harib04f"
)
add_custom_command(
  OUTPUT ${07_day_harib04f_FONTO}
  COMMAND ${FONT} ${07_day_harib04f_FONTS} ${07_day_harib04f_FONTB}
  COMMAND ${B2O}  ${07_day_harib04f_FONTB} ${07_day_harib04f_FONTO} _hankaku
  DEPENDS ${07_day_harib04f_FONTS}
  COMMENT "Generating hankaku.o for 07_day_harib04f"
)
add_custom_command(
  OUTPUT ${07_day_harib04f_LIBGC}
  COMMAND ${CONV} -fcoff32 -nu ${07_day_harib04f_LIBGE} ${07_day_harib04f_LIBGC}
  DEPENDS ${07_day_harib04f_LIBGE}
  COMMENT "Converting golibc for 07_day_harib04f"
)
add_custom_target(07_day_harib04f_sys
  COMMAND gcc ${BINOPT} -T ${07_day_harib04f_LDS} ${07_day_harib04f_CCS} ${07_day_harib04f_WILDOBJ} -o ${07_day_harib04f_BOOTB}
  COMMAND cat ${07_day_harib04f_HEADB} ${07_day_harib04f_BOOTB} > ${07_day_harib04f_SYS}
  DEPENDS ${07_day_harib04f_HEADB} 07_day_harib04f_ipl_build
  DEPENDS ${07_day_harib04f_FUNCO}
  DEPENDS ${07_day_harib04f_FONTO} ${07_day_harib04f_LIBGC}
)
add_custom_target(07_day_harib04f_img
  COMMAND mformat -f 1440 -l HARIBOTEOS -N 0xffffffff -C -B ${07_day_harib04f_IPLB} -i ${07_day_harib04f_OS}
  COMMAND mcopy -i ${07_day_harib04f_OS} ${07_day_harib04f_SYS} ::
  DEPENDS 07_day_harib04f_sys
)

########### next target ###############
set(07_day_harib04f_asmhead_SRCS ${root_SOURCE_DIR}/projects/07_day/harib04f/asmhead.nas)
set(07_day_harib04f_asmhead_OUTS ${root_BINARY_DIR}/projects/07_day/harib04f/asmhead.bin)

add_custom_target(07_day_harib04f_asmhead
  COMMAND ${RAKUSK_EXECUTABLE} ${07_day_harib04f_asmhead_SRCS} ${07_day_harib04f_asmhead_OUTS}
  DEPENDS ${07_day_harib04f_asmhead_SRCS}
)

set(07_day_harib04f_asmhead_WINE_OUTS ${root_BINARY_DIR}/projects/07_day/harib04f/asmhead_wine.bin)
add_custom_target(07_day_harib04f_asmhead_wine
  COMMAND ${WINE} ${WINE_NASK} ${07_day_harib04f_asmhead_SRCS} ${07_day_harib04f_asmhead_WINE_OUTS}
)
add_dependencies(wine 07_day_harib04f_asmhead_wine)

add_custom_target(07_day_harib04f_asmhead_od
  COMMAND ${OD} -t x1 ${07_day_harib04f_asmhead_OUTS}      > ${07_day_harib04f_asmhead_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${07_day_harib04f_asmhead_WINE_OUTS} > ${07_day_harib04f_asmhead_WINE_OUTS}_t.txt
  COMMAND diff -s ${07_day_harib04f_asmhead_OUTS}_f.txt ${07_day_harib04f_asmhead_WINE_OUTS}_t.txt
)

add_dependencies(07_day_harib04f_asmhead_od 07_day_harib04f_asmhead)
add_dependencies(07_day_harib04f_asmhead_od 07_day_harib04f_asmhead_wine)
add_dependencies(od 07_day_harib04f_asmhead_od)

#----------------------------------------------------------
########### next target ###############
set(07_day_harib04f_ipl10_SRCS ${root_SOURCE_DIR}/projects/07_day/harib04f/ipl10.nas)
set(07_day_harib04f_ipl10_OUTS ${root_BINARY_DIR}/projects/07_day/harib04f/ipl10.bin)

add_custom_target(07_day_harib04f_ipl10
  COMMAND ${RAKUSK_EXECUTABLE} ${07_day_harib04f_ipl10_SRCS} ${07_day_harib04f_ipl10_OUTS}
  DEPENDS ${07_day_harib04f_ipl10_SRCS}
)

set(07_day_harib04f_ipl10_WINE_OUTS ${root_BINARY_DIR}/projects/07_day/harib04f/ipl10_wine.bin)
add_custom_target(07_day_harib04f_ipl10_wine
  COMMAND ${WINE} ${WINE_NASK} ${07_day_harib04f_ipl10_SRCS} ${07_day_harib04f_ipl10_WINE_OUTS}
)
add_dependencies(wine 07_day_harib04f_ipl10_wine)

add_custom_target(07_day_harib04f_ipl10_od
  COMMAND ${OD} -t x1 ${07_day_harib04f_ipl10_OUTS}      > ${07_day_harib04f_ipl10_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${07_day_harib04f_ipl10_WINE_OUTS} > ${07_day_harib04f_ipl10_WINE_OUTS}_t.txt
  COMMAND diff -s ${07_day_harib04f_ipl10_OUTS}_f.txt ${07_day_harib04f_ipl10_WINE_OUTS}_t.txt
)

add_dependencies(07_day_harib04f_ipl10_od 07_day_harib04f_ipl10)
add_dependencies(07_day_harib04f_ipl10_od 07_day_harib04f_ipl10_wine)
add_dependencies(od 07_day_harib04f_ipl10_od)

#----------------------------------------------------------
########### next target ###############
set(07_day_harib04f_naskfunc_SRCS ${root_SOURCE_DIR}/projects/07_day/harib04f/naskfunc.nas)
set(07_day_harib04f_naskfunc_OUTS ${root_BINARY_DIR}/projects/07_day/harib04f/naskfunc.img)

add_custom_target(07_day_harib04f_naskfunc
  COMMAND ${RAKUSK_EXECUTABLE} ${07_day_harib04f_naskfunc_SRCS} ${07_day_harib04f_naskfunc_OUTS}
  DEPENDS ${07_day_harib04f_naskfunc_SRCS}
)

add_dependencies(images 07_day_harib04f_naskfunc)
set(07_day_harib04f_naskfunc_WINE_OUTS ${root_BINARY_DIR}/projects/07_day/harib04f/naskfunc_wine.img)
add_custom_target(07_day_harib04f_naskfunc_wine
  COMMAND ${WINE} ${WINE_NASK} ${07_day_harib04f_naskfunc_SRCS} ${07_day_harib04f_naskfunc_WINE_OUTS}
)
add_dependencies(wine 07_day_harib04f_naskfunc_wine)

add_custom_target(07_day_harib04f_naskfunc_od
  COMMAND ${OD} -t x1 ${07_day_harib04f_naskfunc_OUTS}      > ${07_day_harib04f_naskfunc_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${07_day_harib04f_naskfunc_WINE_OUTS} > ${07_day_harib04f_naskfunc_WINE_OUTS}_t.txt
  COMMAND diff -s ${07_day_harib04f_naskfunc_OUTS}_f.txt ${07_day_harib04f_naskfunc_WINE_OUTS}_t.txt
)

add_dependencies(07_day_harib04f_naskfunc_od 07_day_harib04f_naskfunc)
add_dependencies(07_day_harib04f_naskfunc_od 07_day_harib04f_naskfunc_wine)
add_dependencies(od 07_day_harib04f_naskfunc_od)

#----------------------------------------------------------
