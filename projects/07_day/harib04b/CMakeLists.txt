#----------------------------------------------------------
message(STATUS "Entering directory projects/07_day/harib04b/")

set(FONT ${root_BINARY_DIR}/src/makefont)
set(B2O  ${root_BINARY_DIR}/src/bin2obj)
set(CONV ${root_BINARY_DIR}/objconv/objconv)
set(07_day_harib04b_WILDOBJ ${root_BINARY_DIR}/projects/07_day/harib04b/*.o)

# Full OS build rules
set(07_day_harib04b_OS    ${root_BINARY_DIR}/projects/07_day/harib04b/os.img)
set(07_day_harib04b_SYS   ${root_BINARY_DIR}/projects/07_day/harib04b/os.sys)
set(07_day_harib04b_IPLB  ${root_BINARY_DIR}/projects/07_day/harib04b/ipl.bin)
set(07_day_harib04b_IPLS  ${root_SOURCE_DIR}/projects/07_day/harib04b/ipl10.nas)
set(07_day_harib04b_HEADB ${root_BINARY_DIR}/projects/07_day/harib04b/asmhead.bin)
set(07_day_harib04b_HEADS ${root_SOURCE_DIR}/projects/07_day/harib04b/asmhead.nas)
set(07_day_harib04b_BOOTB ${root_BINARY_DIR}/projects/07_day/harib04b/boot.bin)
set(07_day_harib04b_CCO   ${root_BINARY_DIR}/projects/07_day/harib04b/boot.o)
set(07_day_harib04b_CCS   ${root_SOURCE_DIR}/projects/07_day/harib04b/*.c)
set(07_day_harib04b_LDS   ${root_SOURCE_DIR}/projects/os.lds)
set(07_day_harib04b_FUNCS ${root_SOURCE_DIR}/projects/07_day/harib04b/naskfunc.nas)
set(07_day_harib04b_FUNCO ${root_BINARY_DIR}/projects/07_day/harib04b/naskfunc.o)
set(07_day_harib04b_FONTS ${root_SOURCE_DIR}/projects/07_day/harib04b/hankaku.txt)
set(07_day_harib04b_FONTB ${root_BINARY_DIR}/projects/07_day/harib04b/hankaku.bin)
set(07_day_harib04b_FONTO ${root_BINARY_DIR}/projects/07_day/harib04b/hankaku.o)
set(07_day_harib04b_LIBGE ${root_BINARY_DIR}/golibc/libgo.a)
set(07_day_harib04b_LIBGC ${root_BINARY_DIR}/projects/07_day/harib04b/libgo.o)

add_custom_target(07_day_harib04b_run
  COMMAND ${QEMU} ${QEMUOPT} ${07_day_harib04b_OS}
  DEPENDS 07_day_harib04b_img
)
add_custom_target(07_day_harib04b_debug
  COMMAND ${QEMU} -s -S ${QEMUOPT} ${07_day_harib04b_OS} ${QEMU_DEBUG_OPT}
  DEPENDS 07_day_harib04b_img
)
add_custom_target(07_day_harib04b_clean
  COMMAND rm -f ${07_day_harib04b_OS}
  COMMAND rm -f ${07_day_harib04b_SYS}
  COMMAND rm -f ${07_day_harib04b_IPLB}
  COMMAND rm -f ${07_day_harib04b_HEADB}
  COMMAND rm -f ${07_day_harib04b_BOOTB}
  COMMAND rm -f ${07_day_harib04b_WILDOBJ}
  COMMAND rm -f ${root_BINARY_DIR}/projects/07_day/harib04b/ipl10.bin
  COMMAND rm -f ${root_BINARY_DIR}/projects/07_day/harib04b/asmhead.bin
  COMMAND rm -f ${root_BINARY_DIR}/projects/07_day/harib04b/*_wine.bin
  COMMAND rm -f ${root_BINARY_DIR}/projects/07_day/harib04b/*.txt
)
add_custom_target(07_day_harib04b_ipl_build
  COMMAND ${GOSK_EXECUTABLE} ${07_day_harib04b_IPLS} ${07_day_harib04b_IPLB}
  DEPENDS ${07_day_harib04b_IPLS}
)
add_custom_command(
  OUTPUT ${07_day_harib04b_HEADB}
  COMMAND ${GOSK_EXECUTABLE} ${07_day_harib04b_HEADS} ${07_day_harib04b_HEADB}
  DEPENDS ${07_day_harib04b_HEADS}
  COMMENT "Generating asmhead.bin for 07_day_harib04b"
)

add_custom_target(07_day_harib04b_sys
  COMMAND ${FONT} ${07_day_harib04b_FONTS} ${07_day_harib04b_FONTB}
  COMMAND ${B2O}  ${07_day_harib04b_FONTB} ${07_day_harib04b_FONTO} _hankaku
  COMMAND ${CONV} -fcoff32 -nu ${07_day_harib04b_LIBGE} ${07_day_harib04b_LIBGC}
  COMMAND ${GOSK_EXECUTABLE} ${07_day_harib04b_FUNCS} ${07_day_harib04b_FUNCO}
  COMMAND gcc ${BINOPT} -T ${07_day_harib04b_LDS} ${07_day_harib04b_CCS} ${07_day_harib04b_WILDOBJ} -o ${07_day_harib04b_BOOTB}
  COMMAND cat ${07_day_harib04b_HEADB} ${07_day_harib04b_BOOTB} > ${07_day_harib04b_SYS}
  DEPENDS ${07_day_harib04b_HEADB} 07_day_harib04b_ipl_build
  DEPENDS ${07_day_harib04b_FUNCO}
  DEPENDS ${07_day_harib04b_FONTO}
)
add_custom_target(07_day_harib04b_img
  COMMAND mformat -f 1440 -l HARIBOTEOS -N 0xffffffff -C -B ${07_day_harib04b_IPLB} -i ${07_day_harib04b_OS}
  COMMAND mcopy -i ${07_day_harib04b_OS} ${07_day_harib04b_SYS} ::
  DEPENDS 07_day_harib04b_sys
)

########### next target ###############
set(07_day_harib04b_asmhead_SRCS ${root_SOURCE_DIR}/projects/07_day/harib04b/asmhead.nas)
set(07_day_harib04b_asmhead_OUTS ${root_BINARY_DIR}/projects/07_day/harib04b/asmhead.bin)

add_custom_target(07_day_harib04b_asmhead
  COMMAND ${GOSK_EXECUTABLE} ${07_day_harib04b_asmhead_SRCS} ${07_day_harib04b_asmhead_OUTS}
  DEPENDS ${07_day_harib04b_asmhead_SRCS}
)

set(07_day_harib04b_asmhead_WINE_OUTS ${root_BINARY_DIR}/projects/07_day/harib04b/asmhead_wine.bin)
add_custom_target(07_day_harib04b_asmhead_wine
  COMMAND ${WINE} ${WINE_NASK} ${07_day_harib04b_asmhead_SRCS} ${07_day_harib04b_asmhead_WINE_OUTS}
)
add_dependencies(wine 07_day_harib04b_asmhead_wine)

add_custom_target(07_day_harib04b_asmhead_od
  COMMAND ${OD} -t x1 ${07_day_harib04b_asmhead_OUTS}      > ${07_day_harib04b_asmhead_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${07_day_harib04b_asmhead_WINE_OUTS} > ${07_day_harib04b_asmhead_WINE_OUTS}_t.txt
  COMMAND diff -s ${07_day_harib04b_asmhead_OUTS}_f.txt ${07_day_harib04b_asmhead_WINE_OUTS}_t.txt
)

add_dependencies(07_day_harib04b_asmhead_od 07_day_harib04b_asmhead)
add_dependencies(07_day_harib04b_asmhead_od 07_day_harib04b_asmhead_wine)
add_dependencies(od 07_day_harib04b_asmhead_od)

#----------------------------------------------------------
########### next target ###############
set(07_day_harib04b_ipl10_SRCS ${root_SOURCE_DIR}/projects/07_day/harib04b/ipl10.nas)
set(07_day_harib04b_ipl10_OUTS ${root_BINARY_DIR}/projects/07_day/harib04b/ipl10.bin)

add_custom_target(07_day_harib04b_ipl10
  COMMAND ${GOSK_EXECUTABLE} ${07_day_harib04b_ipl10_SRCS} ${07_day_harib04b_ipl10_OUTS}
  DEPENDS ${07_day_harib04b_ipl10_SRCS}
)

set(07_day_harib04b_ipl10_WINE_OUTS ${root_BINARY_DIR}/projects/07_day/harib04b/ipl10_wine.bin)
add_custom_target(07_day_harib04b_ipl10_wine
  COMMAND ${WINE} ${WINE_NASK} ${07_day_harib04b_ipl10_SRCS} ${07_day_harib04b_ipl10_WINE_OUTS}
)
add_dependencies(wine 07_day_harib04b_ipl10_wine)

add_custom_target(07_day_harib04b_ipl10_od
  COMMAND ${OD} -t x1 ${07_day_harib04b_ipl10_OUTS}      > ${07_day_harib04b_ipl10_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${07_day_harib04b_ipl10_WINE_OUTS} > ${07_day_harib04b_ipl10_WINE_OUTS}_t.txt
  COMMAND diff -s ${07_day_harib04b_ipl10_OUTS}_f.txt ${07_day_harib04b_ipl10_WINE_OUTS}_t.txt
)

add_dependencies(07_day_harib04b_ipl10_od 07_day_harib04b_ipl10)
add_dependencies(07_day_harib04b_ipl10_od 07_day_harib04b_ipl10_wine)
add_dependencies(od 07_day_harib04b_ipl10_od)

#----------------------------------------------------------
########### next target ###############
set(07_day_harib04b_naskfunc_SRCS ${root_SOURCE_DIR}/projects/07_day/harib04b/naskfunc.nas)
set(07_day_harib04b_naskfunc_OUTS ${root_BINARY_DIR}/projects/07_day/harib04b/naskfunc.img)

add_custom_target(07_day_harib04b_naskfunc
  COMMAND ${GOSK_EXECUTABLE} ${07_day_harib04b_naskfunc_SRCS} ${07_day_harib04b_naskfunc_OUTS}
  DEPENDS ${07_day_harib04b_naskfunc_SRCS}
)

add_dependencies(images 07_day_harib04b_naskfunc)
set(07_day_harib04b_naskfunc_WINE_OUTS ${root_BINARY_DIR}/projects/07_day/harib04b/naskfunc_wine.img)
add_custom_target(07_day_harib04b_naskfunc_wine
  COMMAND ${WINE} ${WINE_NASK} ${07_day_harib04b_naskfunc_SRCS} ${07_day_harib04b_naskfunc_WINE_OUTS}
)
add_dependencies(wine 07_day_harib04b_naskfunc_wine)

add_custom_target(07_day_harib04b_naskfunc_od
  COMMAND ${OD} -t x1 ${07_day_harib04b_naskfunc_OUTS}      > ${07_day_harib04b_naskfunc_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${07_day_harib04b_naskfunc_WINE_OUTS} > ${07_day_harib04b_naskfunc_WINE_OUTS}_t.txt
  COMMAND diff -s ${07_day_harib04b_naskfunc_OUTS}_f.txt ${07_day_harib04b_naskfunc_WINE_OUTS}_t.txt
)

add_dependencies(07_day_harib04b_naskfunc_od 07_day_harib04b_naskfunc)
add_dependencies(07_day_harib04b_naskfunc_od 07_day_harib04b_naskfunc_wine)
add_dependencies(od 07_day_harib04b_naskfunc_od)

#----------------------------------------------------------
