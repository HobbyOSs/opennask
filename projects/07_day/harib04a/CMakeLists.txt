#----------------------------------------------------------
message(STATUS "Entering directory projects/07_day/harib04a/")

set(FONT ${root_BINARY_DIR}/src/makefont)
set(B2O  ${root_BINARY_DIR}/src/bin2obj)
set(CONV ${root_BINARY_DIR}/objconv/objconv)
set(07_day_harib04a_WILDOBJ ${root_BINARY_DIR}/projects/07_day/harib04a/*.o)

# Full OS build rules
set(07_day_harib04a_OS    ${root_BINARY_DIR}/projects/07_day/harib04a/os.img)
set(07_day_harib04a_SYS   ${root_BINARY_DIR}/projects/07_day/harib04a/os.sys)
set(07_day_harib04a_IPLB  ${root_BINARY_DIR}/projects/07_day/harib04a/ipl.bin)
set(07_day_harib04a_IPLS  ${root_SOURCE_DIR}/projects/07_day/harib04a/ipl10.nas)
set(07_day_harib04a_HEADB ${root_BINARY_DIR}/projects/07_day/harib04a/asmhead.bin)
set(07_day_harib04a_HEADS ${root_SOURCE_DIR}/projects/07_day/harib04a/asmhead.nas)
set(07_day_harib04a_BOOTB ${root_BINARY_DIR}/projects/07_day/harib04a/boot.bin)
set(07_day_harib04a_CCO   ${root_BINARY_DIR}/projects/07_day/harib04a/boot.o)
set(07_day_harib04a_CCS   ${root_SOURCE_DIR}/projects/07_day/harib04a/*.c)
set(07_day_harib04a_LDS   ${root_SOURCE_DIR}/projects/os.lds)
set(07_day_harib04a_FUNCS ${root_SOURCE_DIR}/projects/07_day/harib04a/naskfunc.nas)
set(07_day_harib04a_FUNCO ${root_BINARY_DIR}/projects/07_day/harib04a/naskfunc.o)
set(07_day_harib04a_FONTS ${root_SOURCE_DIR}/projects/07_day/harib04a/hankaku.txt)
set(07_day_harib04a_FONTB ${root_BINARY_DIR}/projects/07_day/harib04a/hankaku.bin)
set(07_day_harib04a_FONTO ${root_BINARY_DIR}/projects/07_day/harib04a/hankaku.o)
set(07_day_harib04a_LIBGE ${root_BINARY_DIR}/golibc/libgo.a)
set(07_day_harib04a_LIBGC ${root_BINARY_DIR}/projects/07_day/harib04a/libgo.o)

add_custom_target(07_day_harib04a_run
  COMMAND ${QEMU} ${QEMUOPT} ${07_day_harib04a_OS}
  DEPENDS 07_day_harib04a_img
)
add_custom_target(07_day_harib04a_debug
  COMMAND ${QEMU} -s -S ${QEMUOPT} ${07_day_harib04a_OS} ${QEMU_DEBUG_OPT}
  DEPENDS 07_day_harib04a_img
)
add_custom_target(07_day_harib04a_clean
  COMMAND rm -f ${07_day_harib04a_OS}
  COMMAND rm -f ${07_day_harib04a_SYS}
  COMMAND rm -f ${07_day_harib04a_IPLB}
  COMMAND rm -f ${07_day_harib04a_HEADB}
  COMMAND rm -f ${07_day_harib04a_BOOTB}
  COMMAND rm -f ${07_day_harib04a_WILDOBJ}
  COMMAND rm -f ${root_BINARY_DIR}/projects/07_day/harib04a/ipl10.bin
  COMMAND rm -f ${root_BINARY_DIR}/projects/07_day/harib04a/asmhead.bin
  COMMAND rm -f ${root_BINARY_DIR}/projects/07_day/harib04a/*_wine.bin
  COMMAND rm -f ${root_BINARY_DIR}/projects/07_day/harib04a/*.txt
)
add_custom_target(07_day_harib04a_ipl_build
  COMMAND ${GOSK_EXECUTABLE} ${07_day_harib04a_IPLS} ${07_day_harib04a_IPLB}
  DEPENDS ${07_day_harib04a_IPLS}
)
add_custom_command(
  OUTPUT ${07_day_harib04a_HEADB}
  COMMAND ${GOSK_EXECUTABLE} ${07_day_harib04a_HEADS} ${07_day_harib04a_HEADB}
  DEPENDS ${07_day_harib04a_HEADS}
  COMMENT "Generating asmhead.bin for 07_day_harib04a"
)

add_custom_target(07_day_harib04a_sys
  COMMAND ${FONT} ${07_day_harib04a_FONTS} ${07_day_harib04a_FONTB}
  COMMAND ${B2O}  ${07_day_harib04a_FONTB} ${07_day_harib04a_FONTO} _hankaku
  COMMAND ${CONV} -fcoff32 -nu ${07_day_harib04a_LIBGE} ${07_day_harib04a_LIBGC}
  COMMAND ${GOSK_EXECUTABLE} ${07_day_harib04a_FUNCS} ${07_day_harib04a_FUNCO}
  COMMAND gcc ${BINOPT} -T ${07_day_harib04a_LDS} ${07_day_harib04a_CCS} ${07_day_harib04a_WILDOBJ} -o ${07_day_harib04a_BOOTB}
  COMMAND cat ${07_day_harib04a_HEADB} ${07_day_harib04a_BOOTB} > ${07_day_harib04a_SYS}
  DEPENDS ${07_day_harib04a_HEADB} 07_day_harib04a_ipl_build
  DEPENDS ${07_day_harib04a_FUNCO}
  DEPENDS ${07_day_harib04a_FONTO}
)
add_custom_target(07_day_harib04a_img
  COMMAND mformat -f 1440 -l HARIBOTEOS -N 0xffffffff -C -B ${07_day_harib04a_IPLB} -i ${07_day_harib04a_OS}
  COMMAND mcopy -i ${07_day_harib04a_OS} ${07_day_harib04a_SYS} ::
  DEPENDS 07_day_harib04a_sys
)

########### next target ###############
set(07_day_harib04a_asmhead_SRCS ${root_SOURCE_DIR}/projects/07_day/harib04a/asmhead.nas)
set(07_day_harib04a_asmhead_OUTS ${root_BINARY_DIR}/projects/07_day/harib04a/asmhead.bin)

add_custom_target(07_day_harib04a_asmhead
  COMMAND ${GOSK_EXECUTABLE} ${07_day_harib04a_asmhead_SRCS} ${07_day_harib04a_asmhead_OUTS}
  DEPENDS ${07_day_harib04a_asmhead_SRCS}
)

set(07_day_harib04a_asmhead_WINE_OUTS ${root_BINARY_DIR}/projects/07_day/harib04a/asmhead_wine.bin)
add_custom_target(07_day_harib04a_asmhead_wine
  COMMAND ${WINE} ${WINE_NASK} ${07_day_harib04a_asmhead_SRCS} ${07_day_harib04a_asmhead_WINE_OUTS}
)
add_dependencies(wine 07_day_harib04a_asmhead_wine)

add_custom_target(07_day_harib04a_asmhead_od
  COMMAND ${OD} -t x1 ${07_day_harib04a_asmhead_OUTS}      > ${07_day_harib04a_asmhead_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${07_day_harib04a_asmhead_WINE_OUTS} > ${07_day_harib04a_asmhead_WINE_OUTS}_t.txt
  COMMAND diff -s ${07_day_harib04a_asmhead_OUTS}_f.txt ${07_day_harib04a_asmhead_WINE_OUTS}_t.txt
)

add_dependencies(07_day_harib04a_asmhead_od 07_day_harib04a_asmhead)
add_dependencies(07_day_harib04a_asmhead_od 07_day_harib04a_asmhead_wine)
add_dependencies(od 07_day_harib04a_asmhead_od)

#----------------------------------------------------------
########### next target ###############
set(07_day_harib04a_ipl10_SRCS ${root_SOURCE_DIR}/projects/07_day/harib04a/ipl10.nas)
set(07_day_harib04a_ipl10_OUTS ${root_BINARY_DIR}/projects/07_day/harib04a/ipl10.bin)

add_custom_target(07_day_harib04a_ipl10
  COMMAND ${GOSK_EXECUTABLE} ${07_day_harib04a_ipl10_SRCS} ${07_day_harib04a_ipl10_OUTS}
  DEPENDS ${07_day_harib04a_ipl10_SRCS}
)

set(07_day_harib04a_ipl10_WINE_OUTS ${root_BINARY_DIR}/projects/07_day/harib04a/ipl10_wine.bin)
add_custom_target(07_day_harib04a_ipl10_wine
  COMMAND ${WINE} ${WINE_NASK} ${07_day_harib04a_ipl10_SRCS} ${07_day_harib04a_ipl10_WINE_OUTS}
)
add_dependencies(wine 07_day_harib04a_ipl10_wine)

add_custom_target(07_day_harib04a_ipl10_od
  COMMAND ${OD} -t x1 ${07_day_harib04a_ipl10_OUTS}      > ${07_day_harib04a_ipl10_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${07_day_harib04a_ipl10_WINE_OUTS} > ${07_day_harib04a_ipl10_WINE_OUTS}_t.txt
  COMMAND diff -s ${07_day_harib04a_ipl10_OUTS}_f.txt ${07_day_harib04a_ipl10_WINE_OUTS}_t.txt
)

add_dependencies(07_day_harib04a_ipl10_od 07_day_harib04a_ipl10)
add_dependencies(07_day_harib04a_ipl10_od 07_day_harib04a_ipl10_wine)
add_dependencies(od 07_day_harib04a_ipl10_od)

#----------------------------------------------------------
########### next target ###############
set(07_day_harib04a_naskfunc_SRCS ${root_SOURCE_DIR}/projects/07_day/harib04a/naskfunc.nas)
set(07_day_harib04a_naskfunc_OUTS ${root_BINARY_DIR}/projects/07_day/harib04a/naskfunc.img)

add_custom_target(07_day_harib04a_naskfunc
  COMMAND ${GOSK_EXECUTABLE} ${07_day_harib04a_naskfunc_SRCS} ${07_day_harib04a_naskfunc_OUTS}
  DEPENDS ${07_day_harib04a_naskfunc_SRCS}
)

add_dependencies(images 07_day_harib04a_naskfunc)
set(07_day_harib04a_naskfunc_WINE_OUTS ${root_BINARY_DIR}/projects/07_day/harib04a/naskfunc_wine.img)
add_custom_target(07_day_harib04a_naskfunc_wine
  COMMAND ${WINE} ${WINE_NASK} ${07_day_harib04a_naskfunc_SRCS} ${07_day_harib04a_naskfunc_WINE_OUTS}
)
add_dependencies(wine 07_day_harib04a_naskfunc_wine)

add_custom_target(07_day_harib04a_naskfunc_od
  COMMAND ${OD} -t x1 ${07_day_harib04a_naskfunc_OUTS}      > ${07_day_harib04a_naskfunc_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${07_day_harib04a_naskfunc_WINE_OUTS} > ${07_day_harib04a_naskfunc_WINE_OUTS}_t.txt
  COMMAND diff -s ${07_day_harib04a_naskfunc_OUTS}_f.txt ${07_day_harib04a_naskfunc_WINE_OUTS}_t.txt
)

add_dependencies(07_day_harib04a_naskfunc_od 07_day_harib04a_naskfunc)
add_dependencies(07_day_harib04a_naskfunc_od 07_day_harib04a_naskfunc_wine)
add_dependencies(od 07_day_harib04a_naskfunc_od)

#----------------------------------------------------------
