#----------------------------------------------------------
message(STATUS "Entering directory projects/08_day/harib05a/")

set(FONT ${root_BINARY_DIR}/src/makefont)
set(B2O  ${root_BINARY_DIR}/src/bin2obj)
set(CONV ${root_BINARY_DIR}/objconv/objconv)
set(08_day_harib05a_WILDOBJ ${root_BINARY_DIR}/projects/08_day/harib05a/*.o)

# Full OS build rules
set(08_day_harib05a_OS    ${root_BINARY_DIR}/projects/08_day/harib05a/os.img)
set(08_day_harib05a_SYS   ${root_BINARY_DIR}/projects/08_day/harib05a/os.sys)
set(08_day_harib05a_IPLB  ${root_BINARY_DIR}/projects/08_day/harib05a/ipl.bin)
set(08_day_harib05a_IPLS  ${root_SOURCE_DIR}/projects/08_day/harib05a/ipl10.nas)
set(08_day_harib05a_HEADB ${root_BINARY_DIR}/projects/08_day/harib05a/asmhead.bin)
set(08_day_harib05a_HEADS ${root_SOURCE_DIR}/projects/08_day/harib05a/asmhead.nas)
set(08_day_harib05a_BOOTB ${root_BINARY_DIR}/projects/08_day/harib05a/boot.bin)
set(08_day_harib05a_CCO   ${root_BINARY_DIR}/projects/08_day/harib05a/boot.o)
set(08_day_harib05a_CCS   ${root_SOURCE_DIR}/projects/08_day/harib05a/*.c)
set(08_day_harib05a_LDS   ${root_SOURCE_DIR}/projects/os.lds)
set(08_day_harib05a_FUNCS ${root_SOURCE_DIR}/projects/08_day/harib05a/naskfunc.nas)
set(08_day_harib05a_FUNCO ${root_BINARY_DIR}/projects/08_day/harib05a/naskfunc.o)
set(08_day_harib05a_FONTS ${root_SOURCE_DIR}/projects/08_day/harib05a/hankaku.txt)
set(08_day_harib05a_FONTB ${root_BINARY_DIR}/projects/08_day/harib05a/hankaku.bin)
set(08_day_harib05a_FONTO ${root_BINARY_DIR}/projects/08_day/harib05a/hankaku.o)
set(08_day_harib05a_LIBGE ${root_BINARY_DIR}/golibc/libgo.a)
set(08_day_harib05a_LIBGC ${root_BINARY_DIR}/projects/08_day/harib05a/libgo.o)

add_custom_target(08_day_harib05a_run
  COMMAND ${QEMU} ${QEMUOPT} ${08_day_harib05a_OS}
  DEPENDS 08_day_harib05a_img
)
add_custom_target(08_day_harib05a_debug
  COMMAND ${QEMU} -s -S ${QEMUOPT} ${08_day_harib05a_OS} ${QEMU_DEBUG_OPT}
  DEPENDS 08_day_harib05a_img
)
add_custom_target(08_day_harib05a_clean
  COMMAND rm -f ${08_day_harib05a_OS}
  COMMAND rm -f ${08_day_harib05a_SYS}
  COMMAND rm -f ${08_day_harib05a_IPLB}
  COMMAND rm -f ${08_day_harib05a_HEADB}
  COMMAND rm -f ${08_day_harib05a_BOOTB}
  COMMAND rm -f ${08_day_harib05a_WILDOBJ}
  COMMAND rm -f ${root_BINARY_DIR}/projects/08_day/harib05a/ipl10.bin
  COMMAND rm -f ${root_BINARY_DIR}/projects/08_day/harib05a/asmhead.bin
  COMMAND rm -f ${root_BINARY_DIR}/projects/08_day/harib05a/*_wine.bin
  COMMAND rm -f ${root_BINARY_DIR}/projects/08_day/harib05a/*.txt
)
add_custom_target(08_day_harib05a_ipl_build
  COMMAND ${GOSK_EXECUTABLE} ${08_day_harib05a_IPLS} ${08_day_harib05a_IPLB}
  DEPENDS ${08_day_harib05a_IPLS}
)
add_custom_command(
  OUTPUT ${08_day_harib05a_HEADB}
  COMMAND ${GOSK_EXECUTABLE} ${08_day_harib05a_HEADS} ${08_day_harib05a_HEADB}
  DEPENDS ${08_day_harib05a_HEADS}
  COMMENT "Generating asmhead.bin for 08_day_harib05a"
)

add_custom_target(08_day_harib05a_sys
  COMMAND ${FONT} ${08_day_harib05a_FONTS} ${08_day_harib05a_FONTB}
  COMMAND ${B2O}  ${08_day_harib05a_FONTB} ${08_day_harib05a_FONTO} _hankaku
  COMMAND ${CONV} -fcoff32 -nu ${08_day_harib05a_LIBGE} ${08_day_harib05a_LIBGC}
  COMMAND ${GOSK_EXECUTABLE} ${08_day_harib05a_FUNCS} ${08_day_harib05a_FUNCO}
  COMMAND gcc ${BINOPT} -T ${08_day_harib05a_LDS} ${08_day_harib05a_CCS} ${08_day_harib05a_WILDOBJ} -o ${08_day_harib05a_BOOTB}
  COMMAND cat ${08_day_harib05a_HEADB} ${08_day_harib05a_BOOTB} > ${08_day_harib05a_SYS}
  DEPENDS ${08_day_harib05a_HEADB} 08_day_harib05a_ipl_build
  DEPENDS ${08_day_harib05a_FUNCO}
  DEPENDS ${08_day_harib05a_FONTO}
)
add_custom_target(08_day_harib05a_img
  COMMAND mformat -f 1440 -l HARIBOTEOS -N 0xffffffff -C -B ${08_day_harib05a_IPLB} -i ${08_day_harib05a_OS}
  COMMAND mcopy -i ${08_day_harib05a_OS} ${08_day_harib05a_SYS} ::
  DEPENDS 08_day_harib05a_sys
)

########### next target ###############
set(08_day_harib05a_asmhead_SRCS ${root_SOURCE_DIR}/projects/08_day/harib05a/asmhead.nas)
set(08_day_harib05a_asmhead_OUTS ${root_BINARY_DIR}/projects/08_day/harib05a/asmhead.bin)

add_custom_target(08_day_harib05a_asmhead
  COMMAND ${GOSK_EXECUTABLE} ${08_day_harib05a_asmhead_SRCS} ${08_day_harib05a_asmhead_OUTS}
  DEPENDS ${08_day_harib05a_asmhead_SRCS}
)

set(08_day_harib05a_asmhead_WINE_OUTS ${root_BINARY_DIR}/projects/08_day/harib05a/asmhead_wine.bin)
add_custom_target(08_day_harib05a_asmhead_wine
  COMMAND ${WINE} ${WINE_NASK} ${08_day_harib05a_asmhead_SRCS} ${08_day_harib05a_asmhead_WINE_OUTS}
)
add_dependencies(wine 08_day_harib05a_asmhead_wine)

add_custom_target(08_day_harib05a_asmhead_od
  COMMAND ${OD} -t x1 ${08_day_harib05a_asmhead_OUTS}      > ${08_day_harib05a_asmhead_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${08_day_harib05a_asmhead_WINE_OUTS} > ${08_day_harib05a_asmhead_WINE_OUTS}_t.txt
  COMMAND diff -s ${08_day_harib05a_asmhead_OUTS}_f.txt ${08_day_harib05a_asmhead_WINE_OUTS}_t.txt
)

add_dependencies(08_day_harib05a_asmhead_od 08_day_harib05a_asmhead)
add_dependencies(08_day_harib05a_asmhead_od 08_day_harib05a_asmhead_wine)
add_dependencies(od 08_day_harib05a_asmhead_od)

#----------------------------------------------------------
########### next target ###############
set(08_day_harib05a_ipl10_SRCS ${root_SOURCE_DIR}/projects/08_day/harib05a/ipl10.nas)
set(08_day_harib05a_ipl10_OUTS ${root_BINARY_DIR}/projects/08_day/harib05a/ipl10.bin)

add_custom_target(08_day_harib05a_ipl10
  COMMAND ${GOSK_EXECUTABLE} ${08_day_harib05a_ipl10_SRCS} ${08_day_harib05a_ipl10_OUTS}
  DEPENDS ${08_day_harib05a_ipl10_SRCS}
)

set(08_day_harib05a_ipl10_WINE_OUTS ${root_BINARY_DIR}/projects/08_day/harib05a/ipl10_wine.bin)
add_custom_target(08_day_harib05a_ipl10_wine
  COMMAND ${WINE} ${WINE_NASK} ${08_day_harib05a_ipl10_SRCS} ${08_day_harib05a_ipl10_WINE_OUTS}
)
add_dependencies(wine 08_day_harib05a_ipl10_wine)

add_custom_target(08_day_harib05a_ipl10_od
  COMMAND ${OD} -t x1 ${08_day_harib05a_ipl10_OUTS}      > ${08_day_harib05a_ipl10_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${08_day_harib05a_ipl10_WINE_OUTS} > ${08_day_harib05a_ipl10_WINE_OUTS}_t.txt
  COMMAND diff -s ${08_day_harib05a_ipl10_OUTS}_f.txt ${08_day_harib05a_ipl10_WINE_OUTS}_t.txt
)

add_dependencies(08_day_harib05a_ipl10_od 08_day_harib05a_ipl10)
add_dependencies(08_day_harib05a_ipl10_od 08_day_harib05a_ipl10_wine)
add_dependencies(od 08_day_harib05a_ipl10_od)

#----------------------------------------------------------
########### next target ###############
set(08_day_harib05a_naskfunc_SRCS ${root_SOURCE_DIR}/projects/08_day/harib05a/naskfunc.nas)
set(08_day_harib05a_naskfunc_OUTS ${root_BINARY_DIR}/projects/08_day/harib05a/naskfunc.img)

add_custom_target(08_day_harib05a_naskfunc
  COMMAND ${GOSK_EXECUTABLE} ${08_day_harib05a_naskfunc_SRCS} ${08_day_harib05a_naskfunc_OUTS}
  DEPENDS ${08_day_harib05a_naskfunc_SRCS}
)

add_dependencies(images 08_day_harib05a_naskfunc)
set(08_day_harib05a_naskfunc_WINE_OUTS ${root_BINARY_DIR}/projects/08_day/harib05a/naskfunc_wine.img)
add_custom_target(08_day_harib05a_naskfunc_wine
  COMMAND ${WINE} ${WINE_NASK} ${08_day_harib05a_naskfunc_SRCS} ${08_day_harib05a_naskfunc_WINE_OUTS}
)
add_dependencies(wine 08_day_harib05a_naskfunc_wine)

add_custom_target(08_day_harib05a_naskfunc_od
  COMMAND ${OD} -t x1 ${08_day_harib05a_naskfunc_OUTS}      > ${08_day_harib05a_naskfunc_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${08_day_harib05a_naskfunc_WINE_OUTS} > ${08_day_harib05a_naskfunc_WINE_OUTS}_t.txt
  COMMAND diff -s ${08_day_harib05a_naskfunc_OUTS}_f.txt ${08_day_harib05a_naskfunc_WINE_OUTS}_t.txt
)

add_dependencies(08_day_harib05a_naskfunc_od 08_day_harib05a_naskfunc)
add_dependencies(08_day_harib05a_naskfunc_od 08_day_harib05a_naskfunc_wine)
add_dependencies(od 08_day_harib05a_naskfunc_od)

#----------------------------------------------------------
