#----------------------------------------------------------
message(STATUS "Entering directory projects/14_day/harib11d/")

set(FONT ${root_BINARY_DIR}/src/makefont)
set(B2O  ${root_BINARY_DIR}/src/bin2obj)
set(CONV ${root_BINARY_DIR}/objconv/objconv)
set(14_day_harib11d_WILDOBJ ${root_BINARY_DIR}/projects/14_day/harib11d/*.o)

# Full OS build rules
set(14_day_harib11d_OS    ${root_BINARY_DIR}/projects/14_day/harib11d/os.img)
set(14_day_harib11d_SYS   ${root_BINARY_DIR}/projects/14_day/harib11d/os.sys)
set(14_day_harib11d_IPLB  ${root_BINARY_DIR}/projects/14_day/harib11d/ipl.bin)
set(14_day_harib11d_IPLS  ${root_SOURCE_DIR}/projects/14_day/harib11d/ipl10.nas)
set(14_day_harib11d_HEADB ${root_BINARY_DIR}/projects/14_day/harib11d/asmhead.bin)
set(14_day_harib11d_HEADS ${root_SOURCE_DIR}/projects/14_day/harib11d/asmhead.nas)
set(14_day_harib11d_BOOTB ${root_BINARY_DIR}/projects/14_day/harib11d/boot.bin)
set(14_day_harib11d_CCO   ${root_BINARY_DIR}/projects/14_day/harib11d/boot.o)
set(14_day_harib11d_CCS   ${root_SOURCE_DIR}/projects/14_day/harib11d/*.c)
set(14_day_harib11d_LDS   ${root_SOURCE_DIR}/projects/os.lds)
set(14_day_harib11d_FUNCS ${root_SOURCE_DIR}/projects/14_day/harib11d/naskfunc.nas)
set(14_day_harib11d_FUNCO ${root_BINARY_DIR}/projects/14_day/harib11d/naskfunc.o)
set(14_day_harib11d_FONTS ${root_SOURCE_DIR}/projects/14_day/harib11d/hankaku.txt)
set(14_day_harib11d_FONTB ${root_BINARY_DIR}/projects/14_day/harib11d/hankaku.bin)
set(14_day_harib11d_FONTO ${root_BINARY_DIR}/projects/14_day/harib11d/hankaku.o)
set(14_day_harib11d_LIBGE ${root_BINARY_DIR}/golibc/libgo.a)
set(14_day_harib11d_LIBGC ${root_BINARY_DIR}/projects/14_day/harib11d/libgo.o)

add_custom_target(14_day_harib11d_run
  COMMAND ${QEMU} ${QEMUOPT} ${14_day_harib11d_OS}
  DEPENDS 14_day_harib11d_img
)
add_custom_target(14_day_harib11d_debug
  COMMAND ${QEMU} -s -S ${QEMUOPT} ${14_day_harib11d_OS} ${QEMU_DEBUG_OPT}
  DEPENDS 14_day_harib11d_img
)
add_custom_target(14_day_harib11d_clean
  COMMAND rm -f ${14_day_harib11d_OS}
  COMMAND rm -f ${14_day_harib11d_SYS}
  COMMAND rm -f ${14_day_harib11d_IPLB}
  COMMAND rm -f ${14_day_harib11d_HEADB}
  COMMAND rm -f ${14_day_harib11d_BOOTB}
  COMMAND rm -f ${14_day_harib11d_WILDOBJ}
  COMMAND rm -f ${root_BINARY_DIR}/projects/14_day/harib11d/ipl10.bin
  COMMAND rm -f ${root_BINARY_DIR}/projects/14_day/harib11d/asmhead.bin
  COMMAND rm -f ${root_BINARY_DIR}/projects/14_day/harib11d/*_wine.bin
  COMMAND rm -f ${root_BINARY_DIR}/projects/14_day/harib11d/*.txt
)
add_custom_target(14_day_harib11d_ipl_build
  COMMAND ${GOSK_EXECUTABLE} ${14_day_harib11d_IPLS} ${14_day_harib11d_IPLB}
  DEPENDS ${14_day_harib11d_IPLS}
)
add_custom_command(
  OUTPUT ${14_day_harib11d_HEADB}
  COMMAND ${GOSK_EXECUTABLE} ${14_day_harib11d_HEADS} ${14_day_harib11d_HEADB}
  DEPENDS ${14_day_harib11d_HEADS}
  COMMENT "Generating asmhead.bin for 14_day_harib11d"
)

add_custom_target(14_day_harib11d_sys
  COMMAND ${FONT} ${14_day_harib11d_FONTS} ${14_day_harib11d_FONTB}
  COMMAND ${B2O}  ${14_day_harib11d_FONTB} ${14_day_harib11d_FONTO} _hankaku
  COMMAND ${CONV} -fcoff32 -nu ${14_day_harib11d_LIBGE} ${14_day_harib11d_LIBGC}
  COMMAND ${GOSK_EXECUTABLE} ${14_day_harib11d_FUNCS} ${14_day_harib11d_FUNCO}
  COMMAND gcc ${BINOPT} -T ${14_day_harib11d_LDS} ${14_day_harib11d_CCS} ${14_day_harib11d_WILDOBJ} -o ${14_day_harib11d_BOOTB}
  COMMAND cat ${14_day_harib11d_HEADB} ${14_day_harib11d_BOOTB} > ${14_day_harib11d_SYS}
  DEPENDS ${14_day_harib11d_HEADB} 14_day_harib11d_ipl_build
  DEPENDS ${14_day_harib11d_FUNCO}
  DEPENDS ${14_day_harib11d_FONTO}
)
add_custom_target(14_day_harib11d_img
  COMMAND mformat -f 1440 -l HARIBOTEOS -N 0xffffffff -C -B ${14_day_harib11d_IPLB} -i ${14_day_harib11d_OS}
  COMMAND mcopy -i ${14_day_harib11d_OS} ${14_day_harib11d_SYS} ::
  DEPENDS 14_day_harib11d_sys
)

########### next target ###############
set(14_day_harib11d_asmhead_SRCS ${root_SOURCE_DIR}/projects/14_day/harib11d/asmhead.nas)
set(14_day_harib11d_asmhead_OUTS ${root_BINARY_DIR}/projects/14_day/harib11d/asmhead.bin)

add_custom_target(14_day_harib11d_asmhead
  COMMAND ${GOSK_EXECUTABLE} ${14_day_harib11d_asmhead_SRCS} ${14_day_harib11d_asmhead_OUTS}
  DEPENDS ${14_day_harib11d_asmhead_SRCS}
)

set(14_day_harib11d_asmhead_WINE_OUTS ${root_BINARY_DIR}/projects/14_day/harib11d/asmhead_wine.bin)
add_custom_target(14_day_harib11d_asmhead_wine
  COMMAND ${WINE} ${WINE_NASK} ${14_day_harib11d_asmhead_SRCS} ${14_day_harib11d_asmhead_WINE_OUTS}
)
add_dependencies(wine 14_day_harib11d_asmhead_wine)

add_custom_target(14_day_harib11d_asmhead_od
  COMMAND ${OD} -t x1 ${14_day_harib11d_asmhead_OUTS}      > ${14_day_harib11d_asmhead_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${14_day_harib11d_asmhead_WINE_OUTS} > ${14_day_harib11d_asmhead_WINE_OUTS}_t.txt
  COMMAND diff -s ${14_day_harib11d_asmhead_OUTS}_f.txt ${14_day_harib11d_asmhead_WINE_OUTS}_t.txt
)

add_dependencies(14_day_harib11d_asmhead_od 14_day_harib11d_asmhead)
add_dependencies(14_day_harib11d_asmhead_od 14_day_harib11d_asmhead_wine)
add_dependencies(od 14_day_harib11d_asmhead_od)

#----------------------------------------------------------
########### next target ###############
set(14_day_harib11d_ipl10_SRCS ${root_SOURCE_DIR}/projects/14_day/harib11d/ipl10.nas)
set(14_day_harib11d_ipl10_OUTS ${root_BINARY_DIR}/projects/14_day/harib11d/ipl10.bin)

add_custom_target(14_day_harib11d_ipl10
  COMMAND ${GOSK_EXECUTABLE} ${14_day_harib11d_ipl10_SRCS} ${14_day_harib11d_ipl10_OUTS}
  DEPENDS ${14_day_harib11d_ipl10_SRCS}
)

set(14_day_harib11d_ipl10_WINE_OUTS ${root_BINARY_DIR}/projects/14_day/harib11d/ipl10_wine.bin)
add_custom_target(14_day_harib11d_ipl10_wine
  COMMAND ${WINE} ${WINE_NASK} ${14_day_harib11d_ipl10_SRCS} ${14_day_harib11d_ipl10_WINE_OUTS}
)
add_dependencies(wine 14_day_harib11d_ipl10_wine)

add_custom_target(14_day_harib11d_ipl10_od
  COMMAND ${OD} -t x1 ${14_day_harib11d_ipl10_OUTS}      > ${14_day_harib11d_ipl10_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${14_day_harib11d_ipl10_WINE_OUTS} > ${14_day_harib11d_ipl10_WINE_OUTS}_t.txt
  COMMAND diff -s ${14_day_harib11d_ipl10_OUTS}_f.txt ${14_day_harib11d_ipl10_WINE_OUTS}_t.txt
)

add_dependencies(14_day_harib11d_ipl10_od 14_day_harib11d_ipl10)
add_dependencies(14_day_harib11d_ipl10_od 14_day_harib11d_ipl10_wine)
add_dependencies(od 14_day_harib11d_ipl10_od)

#----------------------------------------------------------
########### next target ###############
set(14_day_harib11d_naskfunc_SRCS ${root_SOURCE_DIR}/projects/14_day/harib11d/naskfunc.nas)
set(14_day_harib11d_naskfunc_OUTS ${root_BINARY_DIR}/projects/14_day/harib11d/naskfunc.img)

add_custom_target(14_day_harib11d_naskfunc
  COMMAND ${GOSK_EXECUTABLE} ${14_day_harib11d_naskfunc_SRCS} ${14_day_harib11d_naskfunc_OUTS}
  DEPENDS ${14_day_harib11d_naskfunc_SRCS}
)

add_dependencies(images 14_day_harib11d_naskfunc)
set(14_day_harib11d_naskfunc_WINE_OUTS ${root_BINARY_DIR}/projects/14_day/harib11d/naskfunc_wine.img)
add_custom_target(14_day_harib11d_naskfunc_wine
  COMMAND ${WINE} ${WINE_NASK} ${14_day_harib11d_naskfunc_SRCS} ${14_day_harib11d_naskfunc_WINE_OUTS}
)
add_dependencies(wine 14_day_harib11d_naskfunc_wine)

add_custom_target(14_day_harib11d_naskfunc_od
  COMMAND ${OD} -t x1 ${14_day_harib11d_naskfunc_OUTS}      > ${14_day_harib11d_naskfunc_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${14_day_harib11d_naskfunc_WINE_OUTS} > ${14_day_harib11d_naskfunc_WINE_OUTS}_t.txt
  COMMAND diff -s ${14_day_harib11d_naskfunc_OUTS}_f.txt ${14_day_harib11d_naskfunc_WINE_OUTS}_t.txt
)

add_dependencies(14_day_harib11d_naskfunc_od 14_day_harib11d_naskfunc)
add_dependencies(14_day_harib11d_naskfunc_od 14_day_harib11d_naskfunc_wine)
add_dependencies(od 14_day_harib11d_naskfunc_od)

#----------------------------------------------------------
