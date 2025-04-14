#----------------------------------------------------------
message(STATUS "Entering directory projects/14_day/harib11a/")

set(FONT ${root_BINARY_DIR}/src/makefont)
set(B2O  ${root_BINARY_DIR}/src/bin2obj)
set(CONV ${root_BINARY_DIR}/objconv/objconv)
set(14_day_harib11a_WILDOBJ ${root_BINARY_DIR}/projects/14_day/harib11a/*.o)

# Full OS build rules
set(14_day_harib11a_OS    ${root_BINARY_DIR}/projects/14_day/harib11a/os.img)
set(14_day_harib11a_SYS   ${root_BINARY_DIR}/projects/14_day/harib11a/os.sys)
set(14_day_harib11a_IPLB  ${root_BINARY_DIR}/projects/14_day/harib11a/ipl.bin)
set(14_day_harib11a_IPLS  ${root_SOURCE_DIR}/projects/14_day/harib11a/ipl10.nas)
set(14_day_harib11a_HEADB ${root_BINARY_DIR}/projects/14_day/harib11a/asmhead.bin)
set(14_day_harib11a_HEADS ${root_SOURCE_DIR}/projects/14_day/harib11a/asmhead.nas)
set(14_day_harib11a_BOOTB ${root_BINARY_DIR}/projects/14_day/harib11a/boot.bin)
set(14_day_harib11a_CCO   ${root_BINARY_DIR}/projects/14_day/harib11a/boot.o)
set(14_day_harib11a_CCS   ${root_SOURCE_DIR}/projects/14_day/harib11a/*.c)
set(14_day_harib11a_LDS   ${root_SOURCE_DIR}/projects/os.lds)
set(14_day_harib11a_FUNCS ${root_SOURCE_DIR}/projects/14_day/harib11a/naskfunc.nas)
set(14_day_harib11a_FUNCO ${root_BINARY_DIR}/projects/14_day/harib11a/naskfunc.o)
set(14_day_harib11a_FONTS ${root_SOURCE_DIR}/projects/14_day/harib11a/hankaku.txt)
set(14_day_harib11a_FONTB ${root_BINARY_DIR}/projects/14_day/harib11a/hankaku.bin)
set(14_day_harib11a_FONTO ${root_BINARY_DIR}/projects/14_day/harib11a/hankaku.o)
set(14_day_harib11a_LIBGE ${root_BINARY_DIR}/golibc/libgo.a)
set(14_day_harib11a_LIBGC ${root_BINARY_DIR}/projects/14_day/harib11a/libgo.o)

add_custom_target(14_day_harib11a_run
  COMMAND ${QEMU} ${QEMUOPT} ${14_day_harib11a_OS}
  DEPENDS 14_day_harib11a_img
)
add_custom_target(14_day_harib11a_debug
  COMMAND ${QEMU} -s -S ${QEMUOPT} ${14_day_harib11a_OS} ${QEMU_DEBUG_OPT}
  DEPENDS 14_day_harib11a_img
)
add_custom_target(14_day_harib11a_clean
  COMMAND rm -f ${14_day_harib11a_OS}
  COMMAND rm -f ${14_day_harib11a_SYS}
  COMMAND rm -f ${14_day_harib11a_IPLB}
  COMMAND rm -f ${14_day_harib11a_HEADB}
  COMMAND rm -f ${14_day_harib11a_BOOTB}
  COMMAND rm -f ${14_day_harib11a_WILDOBJ}
  COMMAND rm -f ${root_BINARY_DIR}/projects/14_day/harib11a/ipl10.bin
  COMMAND rm -f ${root_BINARY_DIR}/projects/14_day/harib11a/asmhead.bin
  COMMAND rm -f ${root_BINARY_DIR}/projects/14_day/harib11a/*_wine.bin
  COMMAND rm -f ${root_BINARY_DIR}/projects/14_day/harib11a/*.txt
)
add_custom_target(14_day_harib11a_ipl_build
  COMMAND ${GOSK_EXECUTABLE} ${14_day_harib11a_IPLS} ${14_day_harib11a_IPLB}
  DEPENDS ${14_day_harib11a_IPLS}
)
add_custom_command(
  OUTPUT ${14_day_harib11a_HEADB}
  COMMAND ${GOSK_EXECUTABLE} ${14_day_harib11a_HEADS} ${14_day_harib11a_HEADB}
  DEPENDS ${14_day_harib11a_HEADS}
  COMMENT "Generating asmhead.bin for 14_day_harib11a"
)

add_custom_target(14_day_harib11a_sys
  COMMAND ${FONT} ${14_day_harib11a_FONTS} ${14_day_harib11a_FONTB}
  COMMAND ${B2O}  ${14_day_harib11a_FONTB} ${14_day_harib11a_FONTO} _hankaku
  COMMAND ${CONV} -fcoff32 -nu ${14_day_harib11a_LIBGE} ${14_day_harib11a_LIBGC}
  COMMAND ${GOSK_EXECUTABLE} ${14_day_harib11a_FUNCS} ${14_day_harib11a_FUNCO}
  COMMAND gcc ${BINOPT} -T ${14_day_harib11a_LDS} ${14_day_harib11a_CCS} ${14_day_harib11a_WILDOBJ} -o ${14_day_harib11a_BOOTB}
  COMMAND cat ${14_day_harib11a_HEADB} ${14_day_harib11a_BOOTB} > ${14_day_harib11a_SYS}
  DEPENDS ${14_day_harib11a_HEADB} 14_day_harib11a_ipl_build
  DEPENDS ${14_day_harib11a_FUNCO}
  DEPENDS ${14_day_harib11a_FONTO}
)
add_custom_target(14_day_harib11a_img
  COMMAND mformat -f 1440 -l HARIBOTEOS -N 0xffffffff -C -B ${14_day_harib11a_IPLB} -i ${14_day_harib11a_OS}
  COMMAND mcopy -i ${14_day_harib11a_OS} ${14_day_harib11a_SYS} ::
  DEPENDS 14_day_harib11a_sys
)

########### next target ###############
set(14_day_harib11a_asmhead_SRCS ${root_SOURCE_DIR}/projects/14_day/harib11a/asmhead.nas)
set(14_day_harib11a_asmhead_OUTS ${root_BINARY_DIR}/projects/14_day/harib11a/asmhead.bin)

add_custom_target(14_day_harib11a_asmhead
  COMMAND ${GOSK_EXECUTABLE} ${14_day_harib11a_asmhead_SRCS} ${14_day_harib11a_asmhead_OUTS}
  DEPENDS ${14_day_harib11a_asmhead_SRCS}
)

set(14_day_harib11a_asmhead_WINE_OUTS ${root_BINARY_DIR}/projects/14_day/harib11a/asmhead_wine.bin)
add_custom_target(14_day_harib11a_asmhead_wine
  COMMAND ${WINE} ${WINE_NASK} ${14_day_harib11a_asmhead_SRCS} ${14_day_harib11a_asmhead_WINE_OUTS}
)
add_dependencies(wine 14_day_harib11a_asmhead_wine)

add_custom_target(14_day_harib11a_asmhead_od
  COMMAND ${OD} -t x1 ${14_day_harib11a_asmhead_OUTS}      > ${14_day_harib11a_asmhead_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${14_day_harib11a_asmhead_WINE_OUTS} > ${14_day_harib11a_asmhead_WINE_OUTS}_t.txt
  COMMAND diff -s ${14_day_harib11a_asmhead_OUTS}_f.txt ${14_day_harib11a_asmhead_WINE_OUTS}_t.txt
)

add_dependencies(14_day_harib11a_asmhead_od 14_day_harib11a_asmhead)
add_dependencies(14_day_harib11a_asmhead_od 14_day_harib11a_asmhead_wine)
add_dependencies(od 14_day_harib11a_asmhead_od)

#----------------------------------------------------------
########### next target ###############
set(14_day_harib11a_ipl10_SRCS ${root_SOURCE_DIR}/projects/14_day/harib11a/ipl10.nas)
set(14_day_harib11a_ipl10_OUTS ${root_BINARY_DIR}/projects/14_day/harib11a/ipl10.bin)

add_custom_target(14_day_harib11a_ipl10
  COMMAND ${GOSK_EXECUTABLE} ${14_day_harib11a_ipl10_SRCS} ${14_day_harib11a_ipl10_OUTS}
  DEPENDS ${14_day_harib11a_ipl10_SRCS}
)

set(14_day_harib11a_ipl10_WINE_OUTS ${root_BINARY_DIR}/projects/14_day/harib11a/ipl10_wine.bin)
add_custom_target(14_day_harib11a_ipl10_wine
  COMMAND ${WINE} ${WINE_NASK} ${14_day_harib11a_ipl10_SRCS} ${14_day_harib11a_ipl10_WINE_OUTS}
)
add_dependencies(wine 14_day_harib11a_ipl10_wine)

add_custom_target(14_day_harib11a_ipl10_od
  COMMAND ${OD} -t x1 ${14_day_harib11a_ipl10_OUTS}      > ${14_day_harib11a_ipl10_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${14_day_harib11a_ipl10_WINE_OUTS} > ${14_day_harib11a_ipl10_WINE_OUTS}_t.txt
  COMMAND diff -s ${14_day_harib11a_ipl10_OUTS}_f.txt ${14_day_harib11a_ipl10_WINE_OUTS}_t.txt
)

add_dependencies(14_day_harib11a_ipl10_od 14_day_harib11a_ipl10)
add_dependencies(14_day_harib11a_ipl10_od 14_day_harib11a_ipl10_wine)
add_dependencies(od 14_day_harib11a_ipl10_od)

#----------------------------------------------------------
########### next target ###############
set(14_day_harib11a_naskfunc_SRCS ${root_SOURCE_DIR}/projects/14_day/harib11a/naskfunc.nas)
set(14_day_harib11a_naskfunc_OUTS ${root_BINARY_DIR}/projects/14_day/harib11a/naskfunc.img)

add_custom_target(14_day_harib11a_naskfunc
  COMMAND ${GOSK_EXECUTABLE} ${14_day_harib11a_naskfunc_SRCS} ${14_day_harib11a_naskfunc_OUTS}
  DEPENDS ${14_day_harib11a_naskfunc_SRCS}
)

add_dependencies(images 14_day_harib11a_naskfunc)
set(14_day_harib11a_naskfunc_WINE_OUTS ${root_BINARY_DIR}/projects/14_day/harib11a/naskfunc_wine.img)
add_custom_target(14_day_harib11a_naskfunc_wine
  COMMAND ${WINE} ${WINE_NASK} ${14_day_harib11a_naskfunc_SRCS} ${14_day_harib11a_naskfunc_WINE_OUTS}
)
add_dependencies(wine 14_day_harib11a_naskfunc_wine)

add_custom_target(14_day_harib11a_naskfunc_od
  COMMAND ${OD} -t x1 ${14_day_harib11a_naskfunc_OUTS}      > ${14_day_harib11a_naskfunc_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${14_day_harib11a_naskfunc_WINE_OUTS} > ${14_day_harib11a_naskfunc_WINE_OUTS}_t.txt
  COMMAND diff -s ${14_day_harib11a_naskfunc_OUTS}_f.txt ${14_day_harib11a_naskfunc_WINE_OUTS}_t.txt
)

add_dependencies(14_day_harib11a_naskfunc_od 14_day_harib11a_naskfunc)
add_dependencies(14_day_harib11a_naskfunc_od 14_day_harib11a_naskfunc_wine)
add_dependencies(od 14_day_harib11a_naskfunc_od)

#----------------------------------------------------------
