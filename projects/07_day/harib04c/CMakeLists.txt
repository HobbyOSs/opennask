#----------------------------------------------------------
message(STATUS "Entering directory projects/07_day/harib04c/")

set(FONT ${root_BINARY_DIR}/src/makefont)
set(B2O  ${root_BINARY_DIR}/src/bin2obj)
set(CONV ${root_BINARY_DIR}/objconv/objconv)
set(07_day_harib04c_WILDOBJ ${root_BINARY_DIR}/projects/07_day/harib04c/*.o)

# Full OS build rules
set(07_day_harib04c_OS    ${root_BINARY_DIR}/projects/07_day/harib04c/os.img)
set(07_day_harib04c_SYS   ${root_BINARY_DIR}/projects/07_day/harib04c/os.sys)
set(07_day_harib04c_IPLB  ${root_BINARY_DIR}/projects/07_day/harib04c/ipl.bin)
set(07_day_harib04c_IPLS  ${root_SOURCE_DIR}/projects/07_day/harib04c/ipl10.nas)
set(07_day_harib04c_HEADB ${root_BINARY_DIR}/projects/07_day/harib04c/asmhead.bin)
set(07_day_harib04c_HEADS ${root_SOURCE_DIR}/projects/07_day/harib04c/asmhead.nas)
set(07_day_harib04c_BOOTB ${root_BINARY_DIR}/projects/07_day/harib04c/boot.bin)
set(07_day_harib04c_CCO   ${root_BINARY_DIR}/projects/07_day/harib04c/boot.o)
set(07_day_harib04c_CCS   ${root_SOURCE_DIR}/projects/07_day/harib04c/*.c)
set(07_day_harib04c_LDS   ${root_SOURCE_DIR}/projects/os.lds)
set(07_day_harib04c_FUNCS ${root_SOURCE_DIR}/projects/07_day/harib04c/naskfunc.nas)
set(07_day_harib04c_FUNCO ${root_BINARY_DIR}/projects/07_day/harib04c/naskfunc.o)
set(07_day_harib04c_FONTS ${root_SOURCE_DIR}/projects/07_day/harib04c/hankaku.txt)
set(07_day_harib04c_FONTB ${root_BINARY_DIR}/projects/07_day/harib04c/hankaku.bin)
set(07_day_harib04c_FONTO ${root_BINARY_DIR}/projects/07_day/harib04c/hankaku.o)
set(07_day_harib04c_LIBGE ${root_BINARY_DIR}/golibc/libgo.a)
set(07_day_harib04c_LIBGC ${root_BINARY_DIR}/projects/07_day/harib04c/libgo.o)

add_custom_target(07_day_harib04c_run
  COMMAND ${QEMU} ${QEMUOPT} ${07_day_harib04c_OS}
  DEPENDS 07_day_harib04c_img
)
add_custom_target(07_day_harib04c_debug
  COMMAND ${QEMU} -s -S ${QEMUOPT} ${07_day_harib04c_OS} ${QEMU_DEBUG_OPT}
  DEPENDS 07_day_harib04c_img
)
add_custom_target(07_day_harib04c_clean
  COMMAND rm -f ${07_day_harib04c_OS}
  COMMAND rm -f ${07_day_harib04c_SYS}
  COMMAND rm -f ${07_day_harib04c_IPLB}
  COMMAND rm -f ${07_day_harib04c_HEADB}
  COMMAND rm -f ${07_day_harib04c_BOOTB}
  COMMAND rm -f ${07_day_harib04c_WILDOBJ}
  COMMAND rm -f ${root_BINARY_DIR}/projects/07_day/harib04c/ipl10.bin
  COMMAND rm -f ${root_BINARY_DIR}/projects/07_day/harib04c/asmhead.bin
  COMMAND rm -f ${root_BINARY_DIR}/projects/07_day/harib04c/*_wine.bin
  COMMAND rm -f ${root_BINARY_DIR}/projects/07_day/harib04c/*.txt
)
add_custom_target(07_day_harib04c_ipl_build
  COMMAND ${GOSK_EXECUTABLE} ${07_day_harib04c_IPLS} ${07_day_harib04c_IPLB}
  DEPENDS ${07_day_harib04c_IPLS}
)
add_custom_command(
  OUTPUT ${07_day_harib04c_HEADB}
  COMMAND ${GOSK_EXECUTABLE} ${07_day_harib04c_HEADS} ${07_day_harib04c_HEADB}
  DEPENDS ${07_day_harib04c_HEADS}
  COMMENT "Generating asmhead.bin for 07_day_harib04c"
)

add_custom_target(07_day_harib04c_sys
  COMMAND ${FONT} ${07_day_harib04c_FONTS} ${07_day_harib04c_FONTB}
  COMMAND ${B2O}  ${07_day_harib04c_FONTB} ${07_day_harib04c_FONTO} _hankaku
  COMMAND ${CONV} -fcoff32 -nu ${07_day_harib04c_LIBGE} ${07_day_harib04c_LIBGC}
  COMMAND ${GOSK_EXECUTABLE} ${07_day_harib04c_FUNCS} ${07_day_harib04c_FUNCO}
  COMMAND gcc ${BINOPT} -T ${07_day_harib04c_LDS} ${07_day_harib04c_CCS} ${07_day_harib04c_WILDOBJ} -o ${07_day_harib04c_BOOTB}
  COMMAND cat ${07_day_harib04c_HEADB} ${07_day_harib04c_BOOTB} > ${07_day_harib04c_SYS}
  DEPENDS ${07_day_harib04c_HEADB} 07_day_harib04c_ipl_build
  DEPENDS ${07_day_harib04c_FUNCO}
  DEPENDS ${07_day_harib04c_FONTO}
)
add_custom_target(07_day_harib04c_img
  COMMAND mformat -f 1440 -l HARIBOTEOS -N 0xffffffff -C -B ${07_day_harib04c_IPLB} -i ${07_day_harib04c_OS}
  COMMAND mcopy -i ${07_day_harib04c_OS} ${07_day_harib04c_SYS} ::
  DEPENDS 07_day_harib04c_sys
)

########### next target ###############
set(07_day_harib04c_asmhead_SRCS ${root_SOURCE_DIR}/projects/07_day/harib04c/asmhead.nas)
set(07_day_harib04c_asmhead_OUTS ${root_BINARY_DIR}/projects/07_day/harib04c/asmhead.bin)

add_custom_target(07_day_harib04c_asmhead
  COMMAND ${GOSK_EXECUTABLE} ${07_day_harib04c_asmhead_SRCS} ${07_day_harib04c_asmhead_OUTS}
  DEPENDS ${07_day_harib04c_asmhead_SRCS}
)

set(07_day_harib04c_asmhead_WINE_OUTS ${root_BINARY_DIR}/projects/07_day/harib04c/asmhead_wine.bin)
add_custom_target(07_day_harib04c_asmhead_wine
  COMMAND ${WINE} ${WINE_NASK} ${07_day_harib04c_asmhead_SRCS} ${07_day_harib04c_asmhead_WINE_OUTS}
)
add_dependencies(wine 07_day_harib04c_asmhead_wine)

add_custom_target(07_day_harib04c_asmhead_od
  COMMAND ${OD} -t x1 ${07_day_harib04c_asmhead_OUTS}      > ${07_day_harib04c_asmhead_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${07_day_harib04c_asmhead_WINE_OUTS} > ${07_day_harib04c_asmhead_WINE_OUTS}_t.txt
  COMMAND diff -s ${07_day_harib04c_asmhead_OUTS}_f.txt ${07_day_harib04c_asmhead_WINE_OUTS}_t.txt
)

add_dependencies(07_day_harib04c_asmhead_od 07_day_harib04c_asmhead)
add_dependencies(07_day_harib04c_asmhead_od 07_day_harib04c_asmhead_wine)
add_dependencies(od 07_day_harib04c_asmhead_od)

#----------------------------------------------------------
########### next target ###############
set(07_day_harib04c_ipl10_SRCS ${root_SOURCE_DIR}/projects/07_day/harib04c/ipl10.nas)
set(07_day_harib04c_ipl10_OUTS ${root_BINARY_DIR}/projects/07_day/harib04c/ipl10.bin)

add_custom_target(07_day_harib04c_ipl10
  COMMAND ${GOSK_EXECUTABLE} ${07_day_harib04c_ipl10_SRCS} ${07_day_harib04c_ipl10_OUTS}
  DEPENDS ${07_day_harib04c_ipl10_SRCS}
)

set(07_day_harib04c_ipl10_WINE_OUTS ${root_BINARY_DIR}/projects/07_day/harib04c/ipl10_wine.bin)
add_custom_target(07_day_harib04c_ipl10_wine
  COMMAND ${WINE} ${WINE_NASK} ${07_day_harib04c_ipl10_SRCS} ${07_day_harib04c_ipl10_WINE_OUTS}
)
add_dependencies(wine 07_day_harib04c_ipl10_wine)

add_custom_target(07_day_harib04c_ipl10_od
  COMMAND ${OD} -t x1 ${07_day_harib04c_ipl10_OUTS}      > ${07_day_harib04c_ipl10_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${07_day_harib04c_ipl10_WINE_OUTS} > ${07_day_harib04c_ipl10_WINE_OUTS}_t.txt
  COMMAND diff -s ${07_day_harib04c_ipl10_OUTS}_f.txt ${07_day_harib04c_ipl10_WINE_OUTS}_t.txt
)

add_dependencies(07_day_harib04c_ipl10_od 07_day_harib04c_ipl10)
add_dependencies(07_day_harib04c_ipl10_od 07_day_harib04c_ipl10_wine)
add_dependencies(od 07_day_harib04c_ipl10_od)

#----------------------------------------------------------
########### next target ###############
set(07_day_harib04c_naskfunc_SRCS ${root_SOURCE_DIR}/projects/07_day/harib04c/naskfunc.nas)
set(07_day_harib04c_naskfunc_OUTS ${root_BINARY_DIR}/projects/07_day/harib04c/naskfunc.img)

add_custom_target(07_day_harib04c_naskfunc
  COMMAND ${GOSK_EXECUTABLE} ${07_day_harib04c_naskfunc_SRCS} ${07_day_harib04c_naskfunc_OUTS}
  DEPENDS ${07_day_harib04c_naskfunc_SRCS}
)

add_dependencies(images 07_day_harib04c_naskfunc)
set(07_day_harib04c_naskfunc_WINE_OUTS ${root_BINARY_DIR}/projects/07_day/harib04c/naskfunc_wine.img)
add_custom_target(07_day_harib04c_naskfunc_wine
  COMMAND ${WINE} ${WINE_NASK} ${07_day_harib04c_naskfunc_SRCS} ${07_day_harib04c_naskfunc_WINE_OUTS}
)
add_dependencies(wine 07_day_harib04c_naskfunc_wine)

add_custom_target(07_day_harib04c_naskfunc_od
  COMMAND ${OD} -t x1 ${07_day_harib04c_naskfunc_OUTS}      > ${07_day_harib04c_naskfunc_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${07_day_harib04c_naskfunc_WINE_OUTS} > ${07_day_harib04c_naskfunc_WINE_OUTS}_t.txt
  COMMAND diff -s ${07_day_harib04c_naskfunc_OUTS}_f.txt ${07_day_harib04c_naskfunc_WINE_OUTS}_t.txt
)

add_dependencies(07_day_harib04c_naskfunc_od 07_day_harib04c_naskfunc)
add_dependencies(07_day_harib04c_naskfunc_od 07_day_harib04c_naskfunc_wine)
add_dependencies(od 07_day_harib04c_naskfunc_od)

#----------------------------------------------------------
