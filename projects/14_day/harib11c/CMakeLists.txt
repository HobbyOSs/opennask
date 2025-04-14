#----------------------------------------------------------
message(STATUS "Entering directory projects/14_day/harib11c/")

set(FONT ${root_BINARY_DIR}/src/makefont)
set(B2O  ${root_BINARY_DIR}/src/bin2obj)
set(CONV ${root_BINARY_DIR}/objconv/objconv)
set(14_day_harib11c_WILDOBJ ${root_BINARY_DIR}/projects/14_day/harib11c/*.o)

# Full OS build rules
set(14_day_harib11c_OS    ${root_BINARY_DIR}/projects/14_day/harib11c/os.img)
set(14_day_harib11c_SYS   ${root_BINARY_DIR}/projects/14_day/harib11c/os.sys)
set(14_day_harib11c_IPLB  ${root_BINARY_DIR}/projects/14_day/harib11c/ipl.bin)
set(14_day_harib11c_IPLS  ${root_SOURCE_DIR}/projects/14_day/harib11c/ipl10.nas)
set(14_day_harib11c_HEADB ${root_BINARY_DIR}/projects/14_day/harib11c/asmhead.bin)
set(14_day_harib11c_HEADS ${root_SOURCE_DIR}/projects/14_day/harib11c/asmhead.nas)
set(14_day_harib11c_BOOTB ${root_BINARY_DIR}/projects/14_day/harib11c/boot.bin)
set(14_day_harib11c_CCO   ${root_BINARY_DIR}/projects/14_day/harib11c/boot.o)
set(14_day_harib11c_CCS   ${root_SOURCE_DIR}/projects/14_day/harib11c/*.c)
set(14_day_harib11c_LDS   ${root_SOURCE_DIR}/projects/os.lds)
set(14_day_harib11c_FUNCS ${root_SOURCE_DIR}/projects/14_day/harib11c/naskfunc.nas)
set(14_day_harib11c_FUNCO ${root_BINARY_DIR}/projects/14_day/harib11c/naskfunc.o)
set(14_day_harib11c_FONTS ${root_SOURCE_DIR}/projects/14_day/harib11c/hankaku.txt)
set(14_day_harib11c_FONTB ${root_BINARY_DIR}/projects/14_day/harib11c/hankaku.bin)
set(14_day_harib11c_FONTO ${root_BINARY_DIR}/projects/14_day/harib11c/hankaku.o)
set(14_day_harib11c_LIBGE ${root_BINARY_DIR}/golibc/libgo.a)
set(14_day_harib11c_LIBGC ${root_BINARY_DIR}/projects/14_day/harib11c/libgo.o)

add_custom_target(14_day_harib11c_run
  COMMAND ${QEMU} ${QEMUOPT} ${14_day_harib11c_OS}
  DEPENDS 14_day_harib11c_img
)
add_custom_target(14_day_harib11c_debug
  COMMAND ${QEMU} -s -S ${QEMUOPT} ${14_day_harib11c_OS} ${QEMU_DEBUG_OPT}
  DEPENDS 14_day_harib11c_img
)
add_custom_target(14_day_harib11c_clean
  COMMAND rm -f ${14_day_harib11c_OS}
  COMMAND rm -f ${14_day_harib11c_SYS}
  COMMAND rm -f ${14_day_harib11c_IPLB}
  COMMAND rm -f ${14_day_harib11c_HEADB}
  COMMAND rm -f ${14_day_harib11c_BOOTB}
  COMMAND rm -f ${14_day_harib11c_WILDOBJ}
  COMMAND rm -f ${root_BINARY_DIR}/projects/14_day/harib11c/ipl10.bin
  COMMAND rm -f ${root_BINARY_DIR}/projects/14_day/harib11c/asmhead.bin
  COMMAND rm -f ${root_BINARY_DIR}/projects/14_day/harib11c/*_wine.bin
  COMMAND rm -f ${root_BINARY_DIR}/projects/14_day/harib11c/*.txt
)
add_custom_target(14_day_harib11c_ipl_build
  COMMAND ${GOSK_EXECUTABLE} ${14_day_harib11c_IPLS} ${14_day_harib11c_IPLB}
  DEPENDS ${14_day_harib11c_IPLS}
)
add_custom_command(
  OUTPUT ${14_day_harib11c_HEADB}
  COMMAND ${GOSK_EXECUTABLE} ${14_day_harib11c_HEADS} ${14_day_harib11c_HEADB}
  DEPENDS ${14_day_harib11c_HEADS}
  COMMENT "Generating asmhead.bin for 14_day_harib11c"
)

add_custom_target(14_day_harib11c_sys
  COMMAND ${FONT} ${14_day_harib11c_FONTS} ${14_day_harib11c_FONTB}
  COMMAND ${B2O}  ${14_day_harib11c_FONTB} ${14_day_harib11c_FONTO} _hankaku
  COMMAND ${CONV} -fcoff32 -nu ${14_day_harib11c_LIBGE} ${14_day_harib11c_LIBGC}
  COMMAND ${GOSK_EXECUTABLE} ${14_day_harib11c_FUNCS} ${14_day_harib11c_FUNCO}
  COMMAND gcc ${BINOPT} -T ${14_day_harib11c_LDS} ${14_day_harib11c_CCS} ${14_day_harib11c_WILDOBJ} -o ${14_day_harib11c_BOOTB}
  COMMAND cat ${14_day_harib11c_HEADB} ${14_day_harib11c_BOOTB} > ${14_day_harib11c_SYS}
  DEPENDS ${14_day_harib11c_HEADB} 14_day_harib11c_ipl_build
  DEPENDS ${14_day_harib11c_FUNCO}
  DEPENDS ${14_day_harib11c_FONTO}
)
add_custom_target(14_day_harib11c_img
  COMMAND mformat -f 1440 -l HARIBOTEOS -N 0xffffffff -C -B ${14_day_harib11c_IPLB} -i ${14_day_harib11c_OS}
  COMMAND mcopy -i ${14_day_harib11c_OS} ${14_day_harib11c_SYS} ::
  DEPENDS 14_day_harib11c_sys
)

########### next target ###############
set(14_day_harib11c_asmhead_SRCS ${root_SOURCE_DIR}/projects/14_day/harib11c/asmhead.nas)
set(14_day_harib11c_asmhead_OUTS ${root_BINARY_DIR}/projects/14_day/harib11c/asmhead.bin)

add_custom_target(14_day_harib11c_asmhead
  COMMAND ${GOSK_EXECUTABLE} ${14_day_harib11c_asmhead_SRCS} ${14_day_harib11c_asmhead_OUTS}
  DEPENDS ${14_day_harib11c_asmhead_SRCS}
)

set(14_day_harib11c_asmhead_WINE_OUTS ${root_BINARY_DIR}/projects/14_day/harib11c/asmhead_wine.bin)
add_custom_target(14_day_harib11c_asmhead_wine
  COMMAND ${WINE} ${WINE_NASK} ${14_day_harib11c_asmhead_SRCS} ${14_day_harib11c_asmhead_WINE_OUTS}
)
add_dependencies(wine 14_day_harib11c_asmhead_wine)

add_custom_target(14_day_harib11c_asmhead_od
  COMMAND ${OD} -t x1 ${14_day_harib11c_asmhead_OUTS}      > ${14_day_harib11c_asmhead_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${14_day_harib11c_asmhead_WINE_OUTS} > ${14_day_harib11c_asmhead_WINE_OUTS}_t.txt
  COMMAND diff -s ${14_day_harib11c_asmhead_OUTS}_f.txt ${14_day_harib11c_asmhead_WINE_OUTS}_t.txt
)

add_dependencies(14_day_harib11c_asmhead_od 14_day_harib11c_asmhead)
add_dependencies(14_day_harib11c_asmhead_od 14_day_harib11c_asmhead_wine)
add_dependencies(od 14_day_harib11c_asmhead_od)

#----------------------------------------------------------
########### next target ###############
set(14_day_harib11c_ipl10_SRCS ${root_SOURCE_DIR}/projects/14_day/harib11c/ipl10.nas)
set(14_day_harib11c_ipl10_OUTS ${root_BINARY_DIR}/projects/14_day/harib11c/ipl10.bin)

add_custom_target(14_day_harib11c_ipl10
  COMMAND ${GOSK_EXECUTABLE} ${14_day_harib11c_ipl10_SRCS} ${14_day_harib11c_ipl10_OUTS}
  DEPENDS ${14_day_harib11c_ipl10_SRCS}
)

set(14_day_harib11c_ipl10_WINE_OUTS ${root_BINARY_DIR}/projects/14_day/harib11c/ipl10_wine.bin)
add_custom_target(14_day_harib11c_ipl10_wine
  COMMAND ${WINE} ${WINE_NASK} ${14_day_harib11c_ipl10_SRCS} ${14_day_harib11c_ipl10_WINE_OUTS}
)
add_dependencies(wine 14_day_harib11c_ipl10_wine)

add_custom_target(14_day_harib11c_ipl10_od
  COMMAND ${OD} -t x1 ${14_day_harib11c_ipl10_OUTS}      > ${14_day_harib11c_ipl10_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${14_day_harib11c_ipl10_WINE_OUTS} > ${14_day_harib11c_ipl10_WINE_OUTS}_t.txt
  COMMAND diff -s ${14_day_harib11c_ipl10_OUTS}_f.txt ${14_day_harib11c_ipl10_WINE_OUTS}_t.txt
)

add_dependencies(14_day_harib11c_ipl10_od 14_day_harib11c_ipl10)
add_dependencies(14_day_harib11c_ipl10_od 14_day_harib11c_ipl10_wine)
add_dependencies(od 14_day_harib11c_ipl10_od)

#----------------------------------------------------------
########### next target ###############
set(14_day_harib11c_naskfunc_SRCS ${root_SOURCE_DIR}/projects/14_day/harib11c/naskfunc.nas)
set(14_day_harib11c_naskfunc_OUTS ${root_BINARY_DIR}/projects/14_day/harib11c/naskfunc.img)

add_custom_target(14_day_harib11c_naskfunc
  COMMAND ${GOSK_EXECUTABLE} ${14_day_harib11c_naskfunc_SRCS} ${14_day_harib11c_naskfunc_OUTS}
  DEPENDS ${14_day_harib11c_naskfunc_SRCS}
)

add_dependencies(images 14_day_harib11c_naskfunc)
set(14_day_harib11c_naskfunc_WINE_OUTS ${root_BINARY_DIR}/projects/14_day/harib11c/naskfunc_wine.img)
add_custom_target(14_day_harib11c_naskfunc_wine
  COMMAND ${WINE} ${WINE_NASK} ${14_day_harib11c_naskfunc_SRCS} ${14_day_harib11c_naskfunc_WINE_OUTS}
)
add_dependencies(wine 14_day_harib11c_naskfunc_wine)

add_custom_target(14_day_harib11c_naskfunc_od
  COMMAND ${OD} -t x1 ${14_day_harib11c_naskfunc_OUTS}      > ${14_day_harib11c_naskfunc_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${14_day_harib11c_naskfunc_WINE_OUTS} > ${14_day_harib11c_naskfunc_WINE_OUTS}_t.txt
  COMMAND diff -s ${14_day_harib11c_naskfunc_OUTS}_f.txt ${14_day_harib11c_naskfunc_WINE_OUTS}_t.txt
)

add_dependencies(14_day_harib11c_naskfunc_od 14_day_harib11c_naskfunc)
add_dependencies(14_day_harib11c_naskfunc_od 14_day_harib11c_naskfunc_wine)
add_dependencies(od 14_day_harib11c_naskfunc_od)

#----------------------------------------------------------
