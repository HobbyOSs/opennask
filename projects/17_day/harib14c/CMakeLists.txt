#----------------------------------------------------------
message(STATUS "Entering directory projects/17_day/harib14c/")

set(FONT ${root_BINARY_DIR}/src/makefont)
set(B2O  ${root_BINARY_DIR}/src/bin2obj)
set(CONV ${root_BINARY_DIR}/objconv/objconv)
set(17_day_harib14c_WILDOBJ ${root_BINARY_DIR}/projects/17_day/harib14c/*.o)

# Full OS build rules
set(17_day_harib14c_OS    ${root_BINARY_DIR}/projects/17_day/harib14c/os.img)
set(17_day_harib14c_SYS   ${root_BINARY_DIR}/projects/17_day/harib14c/os.sys)
set(17_day_harib14c_IPLB  ${root_BINARY_DIR}/projects/17_day/harib14c/ipl.bin)
set(17_day_harib14c_IPLS  ${root_SOURCE_DIR}/projects/17_day/harib14c/ipl10.nas)
set(17_day_harib14c_HEADB ${root_BINARY_DIR}/projects/17_day/harib14c/asmhead.bin)
set(17_day_harib14c_HEADS ${root_SOURCE_DIR}/projects/17_day/harib14c/asmhead.nas)
set(17_day_harib14c_BOOTB ${root_BINARY_DIR}/projects/17_day/harib14c/boot.bin)
set(17_day_harib14c_CCO   ${root_BINARY_DIR}/projects/17_day/harib14c/boot.o)
set(17_day_harib14c_CCS   ${root_SOURCE_DIR}/projects/17_day/harib14c/*.c)
set(17_day_harib14c_LDS   ${root_SOURCE_DIR}/projects/os.lds)
set(17_day_harib14c_FUNCS ${root_SOURCE_DIR}/projects/17_day/harib14c/naskfunc.nas)
set(17_day_harib14c_FUNCO ${root_BINARY_DIR}/projects/17_day/harib14c/naskfunc.o)
set(17_day_harib14c_FONTS ${root_SOURCE_DIR}/projects/17_day/harib14c/hankaku.txt)
set(17_day_harib14c_FONTB ${root_BINARY_DIR}/projects/17_day/harib14c/hankaku.bin)
set(17_day_harib14c_FONTO ${root_BINARY_DIR}/projects/17_day/harib14c/hankaku.o)
set(17_day_harib14c_LIBGE ${root_BINARY_DIR}/golibc/libgo.a)
set(17_day_harib14c_LIBGC ${root_BINARY_DIR}/projects/17_day/harib14c/libgo.o)

add_custom_target(17_day_harib14c_run
  COMMAND ${QEMU} ${QEMUOPT} ${17_day_harib14c_OS}
  DEPENDS 17_day_harib14c_img
)
add_custom_target(17_day_harib14c_debug
  COMMAND ${QEMU} -s -S ${QEMUOPT} ${17_day_harib14c_OS} ${QEMU_DEBUG_OPT}
  DEPENDS 17_day_harib14c_img
)
add_custom_target(17_day_harib14c_clean
  COMMAND rm -f ${17_day_harib14c_OS}
  COMMAND rm -f ${17_day_harib14c_SYS}
  COMMAND rm -f ${17_day_harib14c_IPLB}
  COMMAND rm -f ${17_day_harib14c_HEADB}
  COMMAND rm -f ${17_day_harib14c_BOOTB}
  COMMAND rm -f ${17_day_harib14c_WILDOBJ}
  COMMAND rm -f ${root_BINARY_DIR}/projects/17_day/harib14c/ipl10.bin
  COMMAND rm -f ${root_BINARY_DIR}/projects/17_day/harib14c/asmhead.bin
  COMMAND rm -f ${root_BINARY_DIR}/projects/17_day/harib14c/*_wine.bin
  COMMAND rm -f ${root_BINARY_DIR}/projects/17_day/harib14c/*.txt
)
add_custom_target(17_day_harib14c_ipl_build
  COMMAND ${GOSK_EXECUTABLE} ${17_day_harib14c_IPLS} ${17_day_harib14c_IPLB}
  DEPENDS ${17_day_harib14c_IPLS}
)
add_custom_command(
  OUTPUT ${17_day_harib14c_HEADB}
  COMMAND ${GOSK_EXECUTABLE} ${17_day_harib14c_HEADS} ${17_day_harib14c_HEADB}
  DEPENDS ${17_day_harib14c_HEADS}
  COMMENT "Generating asmhead.bin for 17_day_harib14c"
)

add_custom_target(17_day_harib14c_sys
  COMMAND ${FONT} ${17_day_harib14c_FONTS} ${17_day_harib14c_FONTB}
  COMMAND ${B2O}  ${17_day_harib14c_FONTB} ${17_day_harib14c_FONTO} _hankaku
  COMMAND ${CONV} -fcoff32 -nu ${17_day_harib14c_LIBGE} ${17_day_harib14c_LIBGC}
  COMMAND ${GOSK_EXECUTABLE} ${17_day_harib14c_FUNCS} ${17_day_harib14c_FUNCO}
  COMMAND gcc ${BINOPT} -T ${17_day_harib14c_LDS} ${17_day_harib14c_CCS} ${17_day_harib14c_WILDOBJ} -o ${17_day_harib14c_BOOTB}
  COMMAND cat ${17_day_harib14c_HEADB} ${17_day_harib14c_BOOTB} > ${17_day_harib14c_SYS}
  DEPENDS ${17_day_harib14c_HEADB} 17_day_harib14c_ipl_build
  DEPENDS ${17_day_harib14c_FUNCO}
  DEPENDS ${17_day_harib14c_FONTO}
)
add_custom_target(17_day_harib14c_img
  COMMAND mformat -f 1440 -l HARIBOTEOS -N 0xffffffff -C -B ${17_day_harib14c_IPLB} -i ${17_day_harib14c_OS}
  COMMAND mcopy -i ${17_day_harib14c_OS} ${17_day_harib14c_SYS} ::
  DEPENDS 17_day_harib14c_sys
)

########### next target ###############
set(17_day_harib14c_asmhead_SRCS ${root_SOURCE_DIR}/projects/17_day/harib14c/asmhead.nas)
set(17_day_harib14c_asmhead_OUTS ${root_BINARY_DIR}/projects/17_day/harib14c/asmhead.bin)

add_custom_target(17_day_harib14c_asmhead
  COMMAND ${GOSK_EXECUTABLE} ${17_day_harib14c_asmhead_SRCS} ${17_day_harib14c_asmhead_OUTS}
  DEPENDS ${17_day_harib14c_asmhead_SRCS}
)

set(17_day_harib14c_asmhead_WINE_OUTS ${root_BINARY_DIR}/projects/17_day/harib14c/asmhead_wine.bin)
add_custom_target(17_day_harib14c_asmhead_wine
  COMMAND ${WINE} ${WINE_NASK} ${17_day_harib14c_asmhead_SRCS} ${17_day_harib14c_asmhead_WINE_OUTS}
)
add_dependencies(wine 17_day_harib14c_asmhead_wine)

add_custom_target(17_day_harib14c_asmhead_od
  COMMAND ${OD} -t x1 ${17_day_harib14c_asmhead_OUTS}      > ${17_day_harib14c_asmhead_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${17_day_harib14c_asmhead_WINE_OUTS} > ${17_day_harib14c_asmhead_WINE_OUTS}_t.txt
  COMMAND diff -s ${17_day_harib14c_asmhead_OUTS}_f.txt ${17_day_harib14c_asmhead_WINE_OUTS}_t.txt
)

add_dependencies(17_day_harib14c_asmhead_od 17_day_harib14c_asmhead)
add_dependencies(17_day_harib14c_asmhead_od 17_day_harib14c_asmhead_wine)
add_dependencies(od 17_day_harib14c_asmhead_od)

#----------------------------------------------------------
########### next target ###############
set(17_day_harib14c_ipl10_SRCS ${root_SOURCE_DIR}/projects/17_day/harib14c/ipl10.nas)
set(17_day_harib14c_ipl10_OUTS ${root_BINARY_DIR}/projects/17_day/harib14c/ipl10.bin)

add_custom_target(17_day_harib14c_ipl10
  COMMAND ${GOSK_EXECUTABLE} ${17_day_harib14c_ipl10_SRCS} ${17_day_harib14c_ipl10_OUTS}
  DEPENDS ${17_day_harib14c_ipl10_SRCS}
)

set(17_day_harib14c_ipl10_WINE_OUTS ${root_BINARY_DIR}/projects/17_day/harib14c/ipl10_wine.bin)
add_custom_target(17_day_harib14c_ipl10_wine
  COMMAND ${WINE} ${WINE_NASK} ${17_day_harib14c_ipl10_SRCS} ${17_day_harib14c_ipl10_WINE_OUTS}
)
add_dependencies(wine 17_day_harib14c_ipl10_wine)

add_custom_target(17_day_harib14c_ipl10_od
  COMMAND ${OD} -t x1 ${17_day_harib14c_ipl10_OUTS}      > ${17_day_harib14c_ipl10_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${17_day_harib14c_ipl10_WINE_OUTS} > ${17_day_harib14c_ipl10_WINE_OUTS}_t.txt
  COMMAND diff -s ${17_day_harib14c_ipl10_OUTS}_f.txt ${17_day_harib14c_ipl10_WINE_OUTS}_t.txt
)

add_dependencies(17_day_harib14c_ipl10_od 17_day_harib14c_ipl10)
add_dependencies(17_day_harib14c_ipl10_od 17_day_harib14c_ipl10_wine)
add_dependencies(od 17_day_harib14c_ipl10_od)

#----------------------------------------------------------
########### next target ###############
set(17_day_harib14c_naskfunc_SRCS ${root_SOURCE_DIR}/projects/17_day/harib14c/naskfunc.nas)
set(17_day_harib14c_naskfunc_OUTS ${root_BINARY_DIR}/projects/17_day/harib14c/naskfunc.img)

add_custom_target(17_day_harib14c_naskfunc
  COMMAND ${GOSK_EXECUTABLE} ${17_day_harib14c_naskfunc_SRCS} ${17_day_harib14c_naskfunc_OUTS}
  DEPENDS ${17_day_harib14c_naskfunc_SRCS}
)

add_dependencies(images 17_day_harib14c_naskfunc)
set(17_day_harib14c_naskfunc_WINE_OUTS ${root_BINARY_DIR}/projects/17_day/harib14c/naskfunc_wine.img)
add_custom_target(17_day_harib14c_naskfunc_wine
  COMMAND ${WINE} ${WINE_NASK} ${17_day_harib14c_naskfunc_SRCS} ${17_day_harib14c_naskfunc_WINE_OUTS}
)
add_dependencies(wine 17_day_harib14c_naskfunc_wine)

add_custom_target(17_day_harib14c_naskfunc_od
  COMMAND ${OD} -t x1 ${17_day_harib14c_naskfunc_OUTS}      > ${17_day_harib14c_naskfunc_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${17_day_harib14c_naskfunc_WINE_OUTS} > ${17_day_harib14c_naskfunc_WINE_OUTS}_t.txt
  COMMAND diff -s ${17_day_harib14c_naskfunc_OUTS}_f.txt ${17_day_harib14c_naskfunc_WINE_OUTS}_t.txt
)

add_dependencies(17_day_harib14c_naskfunc_od 17_day_harib14c_naskfunc)
add_dependencies(17_day_harib14c_naskfunc_od 17_day_harib14c_naskfunc_wine)
add_dependencies(od 17_day_harib14c_naskfunc_od)

#----------------------------------------------------------
