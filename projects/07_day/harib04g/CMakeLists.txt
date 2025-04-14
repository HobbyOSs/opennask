#----------------------------------------------------------
message(STATUS "Entering directory projects/07_day/harib04g/")

set(FONT ${root_BINARY_DIR}/src/makefont)
set(B2O  ${root_BINARY_DIR}/src/bin2obj)
set(CONV ${root_BINARY_DIR}/objconv/objconv)
set(07_day_harib04g_WILDOBJ ${root_BINARY_DIR}/projects/07_day/harib04g/*.o)

# Full OS build rules
set(07_day_harib04g_OS    ${root_BINARY_DIR}/projects/07_day/harib04g/os.img)
set(07_day_harib04g_SYS   ${root_BINARY_DIR}/projects/07_day/harib04g/os.sys)
set(07_day_harib04g_IPLB  ${root_BINARY_DIR}/projects/07_day/harib04g/ipl.bin)
set(07_day_harib04g_IPLS  ${root_SOURCE_DIR}/projects/07_day/harib04g/ipl10.nas)
set(07_day_harib04g_HEADB ${root_BINARY_DIR}/projects/07_day/harib04g/asmhead.bin)
set(07_day_harib04g_HEADS ${root_SOURCE_DIR}/projects/07_day/harib04g/asmhead.nas)
set(07_day_harib04g_BOOTB ${root_BINARY_DIR}/projects/07_day/harib04g/boot.bin)
set(07_day_harib04g_CCO   ${root_BINARY_DIR}/projects/07_day/harib04g/boot.o)
set(07_day_harib04g_CCS   ${root_SOURCE_DIR}/projects/07_day/harib04g/*.c)
set(07_day_harib04g_LDS   ${root_SOURCE_DIR}/projects/os.lds)
set(07_day_harib04g_FUNCS ${root_SOURCE_DIR}/projects/07_day/harib04g/naskfunc.nas)
set(07_day_harib04g_FUNCO ${root_BINARY_DIR}/projects/07_day/harib04g/naskfunc.o)
set(07_day_harib04g_FONTS ${root_SOURCE_DIR}/projects/07_day/harib04g/hankaku.txt)
set(07_day_harib04g_FONTB ${root_BINARY_DIR}/projects/07_day/harib04g/hankaku.bin)
set(07_day_harib04g_FONTO ${root_BINARY_DIR}/projects/07_day/harib04g/hankaku.o)
set(07_day_harib04g_LIBGE ${root_BINARY_DIR}/golibc/libgo.a)
set(07_day_harib04g_LIBGC ${root_BINARY_DIR}/projects/07_day/harib04g/libgo.o)

add_custom_target(07_day_harib04g_run
  COMMAND ${QEMU} ${QEMUOPT} ${07_day_harib04g_OS}
  DEPENDS 07_day_harib04g_img
)
add_custom_target(07_day_harib04g_debug
  COMMAND ${QEMU} -s -S ${QEMUOPT} ${07_day_harib04g_OS} ${QEMU_DEBUG_OPT}
  DEPENDS 07_day_harib04g_img
)
add_custom_target(07_day_harib04g_clean
  COMMAND rm -f ${07_day_harib04g_OS}
  COMMAND rm -f ${07_day_harib04g_SYS}
  COMMAND rm -f ${07_day_harib04g_IPLB}
  COMMAND rm -f ${07_day_harib04g_HEADB}
  COMMAND rm -f ${07_day_harib04g_BOOTB}
  COMMAND rm -f ${07_day_harib04g_WILDOBJ}
  COMMAND rm -f ${root_BINARY_DIR}/projects/07_day/harib04g/ipl10.bin
  COMMAND rm -f ${root_BINARY_DIR}/projects/07_day/harib04g/asmhead.bin
  COMMAND rm -f ${root_BINARY_DIR}/projects/07_day/harib04g/*_wine.bin
  COMMAND rm -f ${root_BINARY_DIR}/projects/07_day/harib04g/*.txt
)
add_custom_target(07_day_harib04g_ipl_build
  COMMAND ${GOSK_EXECUTABLE} ${07_day_harib04g_IPLS} ${07_day_harib04g_IPLB}
  DEPENDS ${07_day_harib04g_IPLS}
)
add_custom_command(
  OUTPUT ${07_day_harib04g_HEADB}
  COMMAND ${GOSK_EXECUTABLE} ${07_day_harib04g_HEADS} ${07_day_harib04g_HEADB}
  DEPENDS ${07_day_harib04g_HEADS}
  COMMENT "Generating asmhead.bin for 07_day_harib04g"
)

add_custom_target(07_day_harib04g_sys
  COMMAND ${FONT} ${07_day_harib04g_FONTS} ${07_day_harib04g_FONTB}
  COMMAND ${B2O}  ${07_day_harib04g_FONTB} ${07_day_harib04g_FONTO} _hankaku
  COMMAND ${CONV} -fcoff32 -nu ${07_day_harib04g_LIBGE} ${07_day_harib04g_LIBGC}
  COMMAND ${GOSK_EXECUTABLE} ${07_day_harib04g_FUNCS} ${07_day_harib04g_FUNCO}
  COMMAND gcc ${BINOPT} -T ${07_day_harib04g_LDS} ${07_day_harib04g_CCS} ${07_day_harib04g_WILDOBJ} -o ${07_day_harib04g_BOOTB}
  COMMAND cat ${07_day_harib04g_HEADB} ${07_day_harib04g_BOOTB} > ${07_day_harib04g_SYS}
  DEPENDS ${07_day_harib04g_HEADB} 07_day_harib04g_ipl_build
  DEPENDS ${07_day_harib04g_FUNCO}
  DEPENDS ${07_day_harib04g_FONTO}
)
add_custom_target(07_day_harib04g_img
  COMMAND mformat -f 1440 -l HARIBOTEOS -N 0xffffffff -C -B ${07_day_harib04g_IPLB} -i ${07_day_harib04g_OS}
  COMMAND mcopy -i ${07_day_harib04g_OS} ${07_day_harib04g_SYS} ::
  DEPENDS 07_day_harib04g_sys
)

########### next target ###############
set(07_day_harib04g_asmhead_SRCS ${root_SOURCE_DIR}/projects/07_day/harib04g/asmhead.nas)
set(07_day_harib04g_asmhead_OUTS ${root_BINARY_DIR}/projects/07_day/harib04g/asmhead.bin)

add_custom_target(07_day_harib04g_asmhead
  COMMAND ${GOSK_EXECUTABLE} ${07_day_harib04g_asmhead_SRCS} ${07_day_harib04g_asmhead_OUTS}
  DEPENDS ${07_day_harib04g_asmhead_SRCS}
)

set(07_day_harib04g_asmhead_WINE_OUTS ${root_BINARY_DIR}/projects/07_day/harib04g/asmhead_wine.bin)
add_custom_target(07_day_harib04g_asmhead_wine
  COMMAND ${WINE} ${WINE_NASK} ${07_day_harib04g_asmhead_SRCS} ${07_day_harib04g_asmhead_WINE_OUTS}
)
add_dependencies(wine 07_day_harib04g_asmhead_wine)

add_custom_target(07_day_harib04g_asmhead_od
  COMMAND ${OD} -t x1 ${07_day_harib04g_asmhead_OUTS}      > ${07_day_harib04g_asmhead_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${07_day_harib04g_asmhead_WINE_OUTS} > ${07_day_harib04g_asmhead_WINE_OUTS}_t.txt
  COMMAND diff -s ${07_day_harib04g_asmhead_OUTS}_f.txt ${07_day_harib04g_asmhead_WINE_OUTS}_t.txt
)

add_dependencies(07_day_harib04g_asmhead_od 07_day_harib04g_asmhead)
add_dependencies(07_day_harib04g_asmhead_od 07_day_harib04g_asmhead_wine)
add_dependencies(od 07_day_harib04g_asmhead_od)

#----------------------------------------------------------
########### next target ###############
set(07_day_harib04g_ipl10_SRCS ${root_SOURCE_DIR}/projects/07_day/harib04g/ipl10.nas)
set(07_day_harib04g_ipl10_OUTS ${root_BINARY_DIR}/projects/07_day/harib04g/ipl10.bin)

add_custom_target(07_day_harib04g_ipl10
  COMMAND ${GOSK_EXECUTABLE} ${07_day_harib04g_ipl10_SRCS} ${07_day_harib04g_ipl10_OUTS}
  DEPENDS ${07_day_harib04g_ipl10_SRCS}
)

set(07_day_harib04g_ipl10_WINE_OUTS ${root_BINARY_DIR}/projects/07_day/harib04g/ipl10_wine.bin)
add_custom_target(07_day_harib04g_ipl10_wine
  COMMAND ${WINE} ${WINE_NASK} ${07_day_harib04g_ipl10_SRCS} ${07_day_harib04g_ipl10_WINE_OUTS}
)
add_dependencies(wine 07_day_harib04g_ipl10_wine)

add_custom_target(07_day_harib04g_ipl10_od
  COMMAND ${OD} -t x1 ${07_day_harib04g_ipl10_OUTS}      > ${07_day_harib04g_ipl10_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${07_day_harib04g_ipl10_WINE_OUTS} > ${07_day_harib04g_ipl10_WINE_OUTS}_t.txt
  COMMAND diff -s ${07_day_harib04g_ipl10_OUTS}_f.txt ${07_day_harib04g_ipl10_WINE_OUTS}_t.txt
)

add_dependencies(07_day_harib04g_ipl10_od 07_day_harib04g_ipl10)
add_dependencies(07_day_harib04g_ipl10_od 07_day_harib04g_ipl10_wine)
add_dependencies(od 07_day_harib04g_ipl10_od)

#----------------------------------------------------------
########### next target ###############
set(07_day_harib04g_naskfunc_SRCS ${root_SOURCE_DIR}/projects/07_day/harib04g/naskfunc.nas)
set(07_day_harib04g_naskfunc_OUTS ${root_BINARY_DIR}/projects/07_day/harib04g/naskfunc.img)

add_custom_target(07_day_harib04g_naskfunc
  COMMAND ${GOSK_EXECUTABLE} ${07_day_harib04g_naskfunc_SRCS} ${07_day_harib04g_naskfunc_OUTS}
  DEPENDS ${07_day_harib04g_naskfunc_SRCS}
)

add_dependencies(images 07_day_harib04g_naskfunc)
set(07_day_harib04g_naskfunc_WINE_OUTS ${root_BINARY_DIR}/projects/07_day/harib04g/naskfunc_wine.img)
add_custom_target(07_day_harib04g_naskfunc_wine
  COMMAND ${WINE} ${WINE_NASK} ${07_day_harib04g_naskfunc_SRCS} ${07_day_harib04g_naskfunc_WINE_OUTS}
)
add_dependencies(wine 07_day_harib04g_naskfunc_wine)

add_custom_target(07_day_harib04g_naskfunc_od
  COMMAND ${OD} -t x1 ${07_day_harib04g_naskfunc_OUTS}      > ${07_day_harib04g_naskfunc_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${07_day_harib04g_naskfunc_WINE_OUTS} > ${07_day_harib04g_naskfunc_WINE_OUTS}_t.txt
  COMMAND diff -s ${07_day_harib04g_naskfunc_OUTS}_f.txt ${07_day_harib04g_naskfunc_WINE_OUTS}_t.txt
)

add_dependencies(07_day_harib04g_naskfunc_od 07_day_harib04g_naskfunc)
add_dependencies(07_day_harib04g_naskfunc_od 07_day_harib04g_naskfunc_wine)
add_dependencies(od 07_day_harib04g_naskfunc_od)

#----------------------------------------------------------
