#----------------------------------------------------------
message(STATUS "Entering directory projects/17_day/harib14g/")

set(FONT ${root_BINARY_DIR}/src/makefont)
set(B2O  ${root_BINARY_DIR}/src/bin2obj)
set(CONV ${root_BINARY_DIR}/objconv/objconv)
set(17_day_harib14g_WILDOBJ ${root_BINARY_DIR}/projects/17_day/harib14g/*.o)

# Full OS build rules
set(17_day_harib14g_OS    ${root_BINARY_DIR}/projects/17_day/harib14g/os.img)
set(17_day_harib14g_SYS   ${root_BINARY_DIR}/projects/17_day/harib14g/os.sys)
set(17_day_harib14g_IPLB  ${root_BINARY_DIR}/projects/17_day/harib14g/ipl.bin)
set(17_day_harib14g_IPLS  ${root_SOURCE_DIR}/projects/17_day/harib14g/ipl10.nas)
set(17_day_harib14g_HEADB ${root_BINARY_DIR}/projects/17_day/harib14g/asmhead.bin)
set(17_day_harib14g_HEADS ${root_SOURCE_DIR}/projects/17_day/harib14g/asmhead.nas)
set(17_day_harib14g_BOOTB ${root_BINARY_DIR}/projects/17_day/harib14g/boot.bin)
set(17_day_harib14g_CCO   ${root_BINARY_DIR}/projects/17_day/harib14g/boot.o)
set(17_day_harib14g_CCS   ${root_SOURCE_DIR}/projects/17_day/harib14g/*.c)
set(17_day_harib14g_LDS   ${root_SOURCE_DIR}/projects/os.lds)
set(17_day_harib14g_FUNCS ${root_SOURCE_DIR}/projects/17_day/harib14g/naskfunc.nas)
set(17_day_harib14g_FUNCO ${root_BINARY_DIR}/projects/17_day/harib14g/naskfunc.o)
set(17_day_harib14g_FONTS ${root_SOURCE_DIR}/projects/17_day/harib14g/hankaku.txt)
set(17_day_harib14g_FONTB ${root_BINARY_DIR}/projects/17_day/harib14g/hankaku.bin)
set(17_day_harib14g_FONTO ${root_BINARY_DIR}/projects/17_day/harib14g/hankaku.o)
set(17_day_harib14g_LIBGE ${root_BINARY_DIR}/golibc/libgo.a)
set(17_day_harib14g_LIBGC ${root_BINARY_DIR}/projects/17_day/harib14g/libgo.o)

add_custom_target(17_day_harib14g_run
  COMMAND ${QEMU} ${QEMUOPT} ${17_day_harib14g_OS}
  DEPENDS 17_day_harib14g_img
)
add_custom_target(17_day_harib14g_debug
  COMMAND ${QEMU} -s -S ${QEMUOPT} ${17_day_harib14g_OS} ${QEMU_DEBUG_OPT}
  DEPENDS 17_day_harib14g_img
)
add_custom_target(17_day_harib14g_clean
  COMMAND rm -f ${17_day_harib14g_OS}
  COMMAND rm -f ${17_day_harib14g_SYS}
  COMMAND rm -f ${17_day_harib14g_IPLB}
  COMMAND rm -f ${17_day_harib14g_HEADB}
  COMMAND rm -f ${17_day_harib14g_BOOTB}
  COMMAND rm -f ${17_day_harib14g_WILDOBJ}
  COMMAND rm -f ${root_BINARY_DIR}/projects/17_day/harib14g/ipl10.bin
  COMMAND rm -f ${root_BINARY_DIR}/projects/17_day/harib14g/asmhead.bin
  COMMAND rm -f ${root_BINARY_DIR}/projects/17_day/harib14g/*_wine.bin
  COMMAND rm -f ${root_BINARY_DIR}/projects/17_day/harib14g/*.txt
)
add_custom_target(17_day_harib14g_ipl_build
  COMMAND ${GOSK_EXECUTABLE} ${17_day_harib14g_IPLS} ${17_day_harib14g_IPLB}
  DEPENDS ${17_day_harib14g_IPLS}
)
add_custom_command(
  OUTPUT ${17_day_harib14g_HEADB}
  COMMAND ${GOSK_EXECUTABLE} ${17_day_harib14g_HEADS} ${17_day_harib14g_HEADB}
  DEPENDS ${17_day_harib14g_HEADS}
  COMMENT "Generating asmhead.bin for 17_day_harib14g"
)

add_custom_target(17_day_harib14g_sys
  COMMAND ${FONT} ${17_day_harib14g_FONTS} ${17_day_harib14g_FONTB}
  COMMAND ${B2O}  ${17_day_harib14g_FONTB} ${17_day_harib14g_FONTO} _hankaku
  COMMAND ${CONV} -fcoff32 -nu ${17_day_harib14g_LIBGE} ${17_day_harib14g_LIBGC}
  COMMAND ${GOSK_EXECUTABLE} ${17_day_harib14g_FUNCS} ${17_day_harib14g_FUNCO}
  COMMAND gcc ${BINOPT} -T ${17_day_harib14g_LDS} ${17_day_harib14g_CCS} ${17_day_harib14g_WILDOBJ} -o ${17_day_harib14g_BOOTB}
  COMMAND cat ${17_day_harib14g_HEADB} ${17_day_harib14g_BOOTB} > ${17_day_harib14g_SYS}
  DEPENDS ${17_day_harib14g_HEADB} 17_day_harib14g_ipl_build
  DEPENDS ${17_day_harib14g_FUNCO}
  DEPENDS ${17_day_harib14g_FONTO}
)
add_custom_target(17_day_harib14g_img
  COMMAND mformat -f 1440 -l HARIBOTEOS -N 0xffffffff -C -B ${17_day_harib14g_IPLB} -i ${17_day_harib14g_OS}
  COMMAND mcopy -i ${17_day_harib14g_OS} ${17_day_harib14g_SYS} ::
  DEPENDS 17_day_harib14g_sys
)

########### next target ###############
set(17_day_harib14g_asmhead_SRCS ${root_SOURCE_DIR}/projects/17_day/harib14g/asmhead.nas)
set(17_day_harib14g_asmhead_OUTS ${root_BINARY_DIR}/projects/17_day/harib14g/asmhead.bin)

add_custom_target(17_day_harib14g_asmhead
  COMMAND ${GOSK_EXECUTABLE} ${17_day_harib14g_asmhead_SRCS} ${17_day_harib14g_asmhead_OUTS}
  DEPENDS ${17_day_harib14g_asmhead_SRCS}
)

set(17_day_harib14g_asmhead_WINE_OUTS ${root_BINARY_DIR}/projects/17_day/harib14g/asmhead_wine.bin)
add_custom_target(17_day_harib14g_asmhead_wine
  COMMAND ${WINE} ${WINE_NASK} ${17_day_harib14g_asmhead_SRCS} ${17_day_harib14g_asmhead_WINE_OUTS}
)
add_dependencies(wine 17_day_harib14g_asmhead_wine)

add_custom_target(17_day_harib14g_asmhead_od
  COMMAND ${OD} -t x1 ${17_day_harib14g_asmhead_OUTS}      > ${17_day_harib14g_asmhead_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${17_day_harib14g_asmhead_WINE_OUTS} > ${17_day_harib14g_asmhead_WINE_OUTS}_t.txt
  COMMAND diff -s ${17_day_harib14g_asmhead_OUTS}_f.txt ${17_day_harib14g_asmhead_WINE_OUTS}_t.txt
)

add_dependencies(17_day_harib14g_asmhead_od 17_day_harib14g_asmhead)
add_dependencies(17_day_harib14g_asmhead_od 17_day_harib14g_asmhead_wine)
add_dependencies(od 17_day_harib14g_asmhead_od)

#----------------------------------------------------------
########### next target ###############
set(17_day_harib14g_ipl10_SRCS ${root_SOURCE_DIR}/projects/17_day/harib14g/ipl10.nas)
set(17_day_harib14g_ipl10_OUTS ${root_BINARY_DIR}/projects/17_day/harib14g/ipl10.bin)

add_custom_target(17_day_harib14g_ipl10
  COMMAND ${GOSK_EXECUTABLE} ${17_day_harib14g_ipl10_SRCS} ${17_day_harib14g_ipl10_OUTS}
  DEPENDS ${17_day_harib14g_ipl10_SRCS}
)

set(17_day_harib14g_ipl10_WINE_OUTS ${root_BINARY_DIR}/projects/17_day/harib14g/ipl10_wine.bin)
add_custom_target(17_day_harib14g_ipl10_wine
  COMMAND ${WINE} ${WINE_NASK} ${17_day_harib14g_ipl10_SRCS} ${17_day_harib14g_ipl10_WINE_OUTS}
)
add_dependencies(wine 17_day_harib14g_ipl10_wine)

add_custom_target(17_day_harib14g_ipl10_od
  COMMAND ${OD} -t x1 ${17_day_harib14g_ipl10_OUTS}      > ${17_day_harib14g_ipl10_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${17_day_harib14g_ipl10_WINE_OUTS} > ${17_day_harib14g_ipl10_WINE_OUTS}_t.txt
  COMMAND diff -s ${17_day_harib14g_ipl10_OUTS}_f.txt ${17_day_harib14g_ipl10_WINE_OUTS}_t.txt
)

add_dependencies(17_day_harib14g_ipl10_od 17_day_harib14g_ipl10)
add_dependencies(17_day_harib14g_ipl10_od 17_day_harib14g_ipl10_wine)
add_dependencies(od 17_day_harib14g_ipl10_od)

#----------------------------------------------------------
########### next target ###############
set(17_day_harib14g_naskfunc_SRCS ${root_SOURCE_DIR}/projects/17_day/harib14g/naskfunc.nas)
set(17_day_harib14g_naskfunc_OUTS ${root_BINARY_DIR}/projects/17_day/harib14g/naskfunc.img)

add_custom_target(17_day_harib14g_naskfunc
  COMMAND ${GOSK_EXECUTABLE} ${17_day_harib14g_naskfunc_SRCS} ${17_day_harib14g_naskfunc_OUTS}
  DEPENDS ${17_day_harib14g_naskfunc_SRCS}
)

add_dependencies(images 17_day_harib14g_naskfunc)
set(17_day_harib14g_naskfunc_WINE_OUTS ${root_BINARY_DIR}/projects/17_day/harib14g/naskfunc_wine.img)
add_custom_target(17_day_harib14g_naskfunc_wine
  COMMAND ${WINE} ${WINE_NASK} ${17_day_harib14g_naskfunc_SRCS} ${17_day_harib14g_naskfunc_WINE_OUTS}
)
add_dependencies(wine 17_day_harib14g_naskfunc_wine)

add_custom_target(17_day_harib14g_naskfunc_od
  COMMAND ${OD} -t x1 ${17_day_harib14g_naskfunc_OUTS}      > ${17_day_harib14g_naskfunc_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${17_day_harib14g_naskfunc_WINE_OUTS} > ${17_day_harib14g_naskfunc_WINE_OUTS}_t.txt
  COMMAND diff -s ${17_day_harib14g_naskfunc_OUTS}_f.txt ${17_day_harib14g_naskfunc_WINE_OUTS}_t.txt
)

add_dependencies(17_day_harib14g_naskfunc_od 17_day_harib14g_naskfunc)
add_dependencies(17_day_harib14g_naskfunc_od 17_day_harib14g_naskfunc_wine)
add_dependencies(od 17_day_harib14g_naskfunc_od)

#----------------------------------------------------------
