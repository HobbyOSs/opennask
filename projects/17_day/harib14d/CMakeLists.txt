#----------------------------------------------------------
message(STATUS "Entering directory projects/17_day/harib14d/")

set(FONT ${root_BINARY_DIR}/src/makefont)
set(B2O  ${root_BINARY_DIR}/src/bin2obj)
set(CONV ${root_BINARY_DIR}/objconv/objconv)
set(17_day_harib14d_WILDOBJ ${root_BINARY_DIR}/projects/17_day/harib14d/*.o)

# Full OS build rules
set(17_day_harib14d_OS    ${root_BINARY_DIR}/projects/17_day/harib14d/os.img)
set(17_day_harib14d_SYS   ${root_BINARY_DIR}/projects/17_day/harib14d/os.sys)
set(17_day_harib14d_IPLB  ${root_BINARY_DIR}/projects/17_day/harib14d/ipl.bin)
set(17_day_harib14d_IPLS  ${root_SOURCE_DIR}/projects/17_day/harib14d/ipl10.nas)
set(17_day_harib14d_HEADB ${root_BINARY_DIR}/projects/17_day/harib14d/asmhead.bin)
set(17_day_harib14d_HEADS ${root_SOURCE_DIR}/projects/17_day/harib14d/asmhead.nas)
set(17_day_harib14d_BOOTB ${root_BINARY_DIR}/projects/17_day/harib14d/boot.bin)
set(17_day_harib14d_CCO   ${root_BINARY_DIR}/projects/17_day/harib14d/boot.o)
set(17_day_harib14d_CCS   ${root_SOURCE_DIR}/projects/17_day/harib14d/*.c)
set(17_day_harib14d_LDS   ${root_SOURCE_DIR}/projects/os.lds)
set(17_day_harib14d_FUNCS ${root_SOURCE_DIR}/projects/17_day/harib14d/naskfunc.nas)
set(17_day_harib14d_FUNCO ${root_BINARY_DIR}/projects/17_day/harib14d/naskfunc.o)
set(17_day_harib14d_FONTS ${root_SOURCE_DIR}/projects/17_day/harib14d/hankaku.txt)
set(17_day_harib14d_FONTB ${root_BINARY_DIR}/projects/17_day/harib14d/hankaku.bin)
set(17_day_harib14d_FONTO ${root_BINARY_DIR}/projects/17_day/harib14d/hankaku.o)
set(17_day_harib14d_LIBGE ${root_BINARY_DIR}/golibc/libgo.a)
set(17_day_harib14d_LIBGC ${root_BINARY_DIR}/projects/17_day/harib14d/libgo.o)

add_custom_target(17_day_harib14d_run
  COMMAND ${QEMU} ${QEMUOPT} ${17_day_harib14d_OS}
  DEPENDS 17_day_harib14d_img
)
add_custom_target(17_day_harib14d_debug
  COMMAND ${QEMU} -s -S ${QEMUOPT} ${17_day_harib14d_OS} ${QEMU_DEBUG_OPT}
  DEPENDS 17_day_harib14d_img
)
add_custom_target(17_day_harib14d_clean
  COMMAND rm -f ${17_day_harib14d_OS}
  COMMAND rm -f ${17_day_harib14d_SYS}
  COMMAND rm -f ${17_day_harib14d_IPLB}
  COMMAND rm -f ${17_day_harib14d_HEADB}
  COMMAND rm -f ${17_day_harib14d_BOOTB}
  COMMAND rm -f ${17_day_harib14d_WILDOBJ}
  COMMAND rm -f ${root_BINARY_DIR}/projects/17_day/harib14d/ipl10.bin
  COMMAND rm -f ${root_BINARY_DIR}/projects/17_day/harib14d/asmhead.bin
  COMMAND rm -f ${root_BINARY_DIR}/projects/17_day/harib14d/*_wine.bin
  COMMAND rm -f ${root_BINARY_DIR}/projects/17_day/harib14d/*.txt
)
add_custom_target(17_day_harib14d_ipl_build
  COMMAND ${GOSK_EXECUTABLE} ${17_day_harib14d_IPLS} ${17_day_harib14d_IPLB}
  DEPENDS ${17_day_harib14d_IPLS}
)
add_custom_command(
  OUTPUT ${17_day_harib14d_HEADB}
  COMMAND ${GOSK_EXECUTABLE} ${17_day_harib14d_HEADS} ${17_day_harib14d_HEADB}
  DEPENDS ${17_day_harib14d_HEADS}
  COMMENT "Generating asmhead.bin for 17_day_harib14d"
)

add_custom_target(17_day_harib14d_sys
  COMMAND ${FONT} ${17_day_harib14d_FONTS} ${17_day_harib14d_FONTB}
  COMMAND ${B2O}  ${17_day_harib14d_FONTB} ${17_day_harib14d_FONTO} _hankaku
  COMMAND ${CONV} -fcoff32 -nu ${17_day_harib14d_LIBGE} ${17_day_harib14d_LIBGC}
  COMMAND ${GOSK_EXECUTABLE} ${17_day_harib14d_FUNCS} ${17_day_harib14d_FUNCO}
  COMMAND gcc ${BINOPT} -T ${17_day_harib14d_LDS} ${17_day_harib14d_CCS} ${17_day_harib14d_WILDOBJ} -o ${17_day_harib14d_BOOTB}
  COMMAND cat ${17_day_harib14d_HEADB} ${17_day_harib14d_BOOTB} > ${17_day_harib14d_SYS}
  DEPENDS ${17_day_harib14d_HEADB} 17_day_harib14d_ipl_build
  DEPENDS ${17_day_harib14d_FUNCO}
  DEPENDS ${17_day_harib14d_FONTO}
)
add_custom_target(17_day_harib14d_img
  COMMAND mformat -f 1440 -l HARIBOTEOS -N 0xffffffff -C -B ${17_day_harib14d_IPLB} -i ${17_day_harib14d_OS}
  COMMAND mcopy -i ${17_day_harib14d_OS} ${17_day_harib14d_SYS} ::
  DEPENDS 17_day_harib14d_sys
)

########### next target ###############
set(17_day_harib14d_asmhead_SRCS ${root_SOURCE_DIR}/projects/17_day/harib14d/asmhead.nas)
set(17_day_harib14d_asmhead_OUTS ${root_BINARY_DIR}/projects/17_day/harib14d/asmhead.bin)

add_custom_target(17_day_harib14d_asmhead
  COMMAND ${GOSK_EXECUTABLE} ${17_day_harib14d_asmhead_SRCS} ${17_day_harib14d_asmhead_OUTS}
  DEPENDS ${17_day_harib14d_asmhead_SRCS}
)

set(17_day_harib14d_asmhead_WINE_OUTS ${root_BINARY_DIR}/projects/17_day/harib14d/asmhead_wine.bin)
add_custom_target(17_day_harib14d_asmhead_wine
  COMMAND ${WINE} ${WINE_NASK} ${17_day_harib14d_asmhead_SRCS} ${17_day_harib14d_asmhead_WINE_OUTS}
)
add_dependencies(wine 17_day_harib14d_asmhead_wine)

add_custom_target(17_day_harib14d_asmhead_od
  COMMAND ${OD} -t x1 ${17_day_harib14d_asmhead_OUTS}      > ${17_day_harib14d_asmhead_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${17_day_harib14d_asmhead_WINE_OUTS} > ${17_day_harib14d_asmhead_WINE_OUTS}_t.txt
  COMMAND diff -s ${17_day_harib14d_asmhead_OUTS}_f.txt ${17_day_harib14d_asmhead_WINE_OUTS}_t.txt
)

add_dependencies(17_day_harib14d_asmhead_od 17_day_harib14d_asmhead)
add_dependencies(17_day_harib14d_asmhead_od 17_day_harib14d_asmhead_wine)
add_dependencies(od 17_day_harib14d_asmhead_od)

#----------------------------------------------------------
########### next target ###############
set(17_day_harib14d_ipl10_SRCS ${root_SOURCE_DIR}/projects/17_day/harib14d/ipl10.nas)
set(17_day_harib14d_ipl10_OUTS ${root_BINARY_DIR}/projects/17_day/harib14d/ipl10.bin)

add_custom_target(17_day_harib14d_ipl10
  COMMAND ${GOSK_EXECUTABLE} ${17_day_harib14d_ipl10_SRCS} ${17_day_harib14d_ipl10_OUTS}
  DEPENDS ${17_day_harib14d_ipl10_SRCS}
)

set(17_day_harib14d_ipl10_WINE_OUTS ${root_BINARY_DIR}/projects/17_day/harib14d/ipl10_wine.bin)
add_custom_target(17_day_harib14d_ipl10_wine
  COMMAND ${WINE} ${WINE_NASK} ${17_day_harib14d_ipl10_SRCS} ${17_day_harib14d_ipl10_WINE_OUTS}
)
add_dependencies(wine 17_day_harib14d_ipl10_wine)

add_custom_target(17_day_harib14d_ipl10_od
  COMMAND ${OD} -t x1 ${17_day_harib14d_ipl10_OUTS}      > ${17_day_harib14d_ipl10_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${17_day_harib14d_ipl10_WINE_OUTS} > ${17_day_harib14d_ipl10_WINE_OUTS}_t.txt
  COMMAND diff -s ${17_day_harib14d_ipl10_OUTS}_f.txt ${17_day_harib14d_ipl10_WINE_OUTS}_t.txt
)

add_dependencies(17_day_harib14d_ipl10_od 17_day_harib14d_ipl10)
add_dependencies(17_day_harib14d_ipl10_od 17_day_harib14d_ipl10_wine)
add_dependencies(od 17_day_harib14d_ipl10_od)

#----------------------------------------------------------
########### next target ###############
set(17_day_harib14d_naskfunc_SRCS ${root_SOURCE_DIR}/projects/17_day/harib14d/naskfunc.nas)
set(17_day_harib14d_naskfunc_OUTS ${root_BINARY_DIR}/projects/17_day/harib14d/naskfunc.img)

add_custom_target(17_day_harib14d_naskfunc
  COMMAND ${GOSK_EXECUTABLE} ${17_day_harib14d_naskfunc_SRCS} ${17_day_harib14d_naskfunc_OUTS}
  DEPENDS ${17_day_harib14d_naskfunc_SRCS}
)

add_dependencies(images 17_day_harib14d_naskfunc)
set(17_day_harib14d_naskfunc_WINE_OUTS ${root_BINARY_DIR}/projects/17_day/harib14d/naskfunc_wine.img)
add_custom_target(17_day_harib14d_naskfunc_wine
  COMMAND ${WINE} ${WINE_NASK} ${17_day_harib14d_naskfunc_SRCS} ${17_day_harib14d_naskfunc_WINE_OUTS}
)
add_dependencies(wine 17_day_harib14d_naskfunc_wine)

add_custom_target(17_day_harib14d_naskfunc_od
  COMMAND ${OD} -t x1 ${17_day_harib14d_naskfunc_OUTS}      > ${17_day_harib14d_naskfunc_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${17_day_harib14d_naskfunc_WINE_OUTS} > ${17_day_harib14d_naskfunc_WINE_OUTS}_t.txt
  COMMAND diff -s ${17_day_harib14d_naskfunc_OUTS}_f.txt ${17_day_harib14d_naskfunc_WINE_OUTS}_t.txt
)

add_dependencies(17_day_harib14d_naskfunc_od 17_day_harib14d_naskfunc)
add_dependencies(17_day_harib14d_naskfunc_od 17_day_harib14d_naskfunc_wine)
add_dependencies(od 17_day_harib14d_naskfunc_od)

#----------------------------------------------------------
