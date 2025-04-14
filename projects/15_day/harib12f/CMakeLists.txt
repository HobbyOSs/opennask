#----------------------------------------------------------
message(STATUS "Entering directory projects/15_day/harib12f/")

set(FONT ${root_BINARY_DIR}/src/makefont)
set(B2O  ${root_BINARY_DIR}/src/bin2obj)
set(CONV ${root_BINARY_DIR}/objconv/objconv)
set(15_day_harib12f_WILDOBJ ${root_BINARY_DIR}/projects/15_day/harib12f/*.o)

# Full OS build rules
set(15_day_harib12f_OS    ${root_BINARY_DIR}/projects/15_day/harib12f/os.img)
set(15_day_harib12f_SYS   ${root_BINARY_DIR}/projects/15_day/harib12f/os.sys)
set(15_day_harib12f_IPLB  ${root_BINARY_DIR}/projects/15_day/harib12f/ipl.bin)
set(15_day_harib12f_IPLS  ${root_SOURCE_DIR}/projects/15_day/harib12f/ipl10.nas)
set(15_day_harib12f_HEADB ${root_BINARY_DIR}/projects/15_day/harib12f/asmhead.bin)
set(15_day_harib12f_HEADS ${root_SOURCE_DIR}/projects/15_day/harib12f/asmhead.nas)
set(15_day_harib12f_BOOTB ${root_BINARY_DIR}/projects/15_day/harib12f/boot.bin)
set(15_day_harib12f_CCO   ${root_BINARY_DIR}/projects/15_day/harib12f/boot.o)
set(15_day_harib12f_CCS   ${root_SOURCE_DIR}/projects/15_day/harib12f/*.c)
set(15_day_harib12f_LDS   ${root_SOURCE_DIR}/projects/os.lds)
set(15_day_harib12f_FUNCS ${root_SOURCE_DIR}/projects/15_day/harib12f/naskfunc.nas)
set(15_day_harib12f_FUNCO ${root_BINARY_DIR}/projects/15_day/harib12f/naskfunc.o)
set(15_day_harib12f_FONTS ${root_SOURCE_DIR}/projects/15_day/harib12f/hankaku.txt)
set(15_day_harib12f_FONTB ${root_BINARY_DIR}/projects/15_day/harib12f/hankaku.bin)
set(15_day_harib12f_FONTO ${root_BINARY_DIR}/projects/15_day/harib12f/hankaku.o)
set(15_day_harib12f_LIBGE ${root_BINARY_DIR}/golibc/libgo.a)
set(15_day_harib12f_LIBGC ${root_BINARY_DIR}/projects/15_day/harib12f/libgo.o)

add_custom_target(15_day_harib12f_run
  COMMAND ${QEMU} ${QEMUOPT} ${15_day_harib12f_OS}
  DEPENDS 15_day_harib12f_img
)
add_custom_target(15_day_harib12f_debug
  COMMAND ${QEMU} -s -S ${QEMUOPT} ${15_day_harib12f_OS} ${QEMU_DEBUG_OPT}
  DEPENDS 15_day_harib12f_img
)
add_custom_target(15_day_harib12f_clean
  COMMAND rm -f ${15_day_harib12f_OS}
  COMMAND rm -f ${15_day_harib12f_SYS}
  COMMAND rm -f ${15_day_harib12f_IPLB}
  COMMAND rm -f ${15_day_harib12f_HEADB}
  COMMAND rm -f ${15_day_harib12f_BOOTB}
  COMMAND rm -f ${15_day_harib12f_WILDOBJ}
  COMMAND rm -f ${root_BINARY_DIR}/projects/15_day/harib12f/ipl10.bin
  COMMAND rm -f ${root_BINARY_DIR}/projects/15_day/harib12f/asmhead.bin
  COMMAND rm -f ${root_BINARY_DIR}/projects/15_day/harib12f/*_wine.bin
  COMMAND rm -f ${root_BINARY_DIR}/projects/15_day/harib12f/*.txt
)
add_custom_target(15_day_harib12f_ipl_build
  COMMAND ${GOSK_EXECUTABLE} ${15_day_harib12f_IPLS} ${15_day_harib12f_IPLB}
  DEPENDS ${15_day_harib12f_IPLS}
)
add_custom_command(
  OUTPUT ${15_day_harib12f_HEADB}
  COMMAND ${GOSK_EXECUTABLE} ${15_day_harib12f_HEADS} ${15_day_harib12f_HEADB}
  DEPENDS ${15_day_harib12f_HEADS}
  COMMENT "Generating asmhead.bin for 15_day_harib12f"
)

add_custom_target(15_day_harib12f_sys
  COMMAND ${FONT} ${15_day_harib12f_FONTS} ${15_day_harib12f_FONTB}
  COMMAND ${B2O}  ${15_day_harib12f_FONTB} ${15_day_harib12f_FONTO} _hankaku
  COMMAND ${CONV} -fcoff32 -nu ${15_day_harib12f_LIBGE} ${15_day_harib12f_LIBGC}
  COMMAND ${GOSK_EXECUTABLE} ${15_day_harib12f_FUNCS} ${15_day_harib12f_FUNCO}
  COMMAND gcc ${BINOPT} -T ${15_day_harib12f_LDS} ${15_day_harib12f_CCS} ${15_day_harib12f_WILDOBJ} -o ${15_day_harib12f_BOOTB}
  COMMAND cat ${15_day_harib12f_HEADB} ${15_day_harib12f_BOOTB} > ${15_day_harib12f_SYS}
  DEPENDS ${15_day_harib12f_HEADB} 15_day_harib12f_ipl_build
  DEPENDS ${15_day_harib12f_FUNCO}
  DEPENDS ${15_day_harib12f_FONTO}
)
add_custom_target(15_day_harib12f_img
  COMMAND mformat -f 1440 -l HARIBOTEOS -N 0xffffffff -C -B ${15_day_harib12f_IPLB} -i ${15_day_harib12f_OS}
  COMMAND mcopy -i ${15_day_harib12f_OS} ${15_day_harib12f_SYS} ::
  DEPENDS 15_day_harib12f_sys
)

########### next target ###############
set(15_day_harib12f_asmhead_SRCS ${root_SOURCE_DIR}/projects/15_day/harib12f/asmhead.nas)
set(15_day_harib12f_asmhead_OUTS ${root_BINARY_DIR}/projects/15_day/harib12f/asmhead.bin)

add_custom_target(15_day_harib12f_asmhead
  COMMAND ${GOSK_EXECUTABLE} ${15_day_harib12f_asmhead_SRCS} ${15_day_harib12f_asmhead_OUTS}
  DEPENDS ${15_day_harib12f_asmhead_SRCS}
)

set(15_day_harib12f_asmhead_WINE_OUTS ${root_BINARY_DIR}/projects/15_day/harib12f/asmhead_wine.bin)
add_custom_target(15_day_harib12f_asmhead_wine
  COMMAND ${WINE} ${WINE_NASK} ${15_day_harib12f_asmhead_SRCS} ${15_day_harib12f_asmhead_WINE_OUTS}
)
add_dependencies(wine 15_day_harib12f_asmhead_wine)

add_custom_target(15_day_harib12f_asmhead_od
  COMMAND ${OD} -t x1 ${15_day_harib12f_asmhead_OUTS}      > ${15_day_harib12f_asmhead_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${15_day_harib12f_asmhead_WINE_OUTS} > ${15_day_harib12f_asmhead_WINE_OUTS}_t.txt
  COMMAND diff -s ${15_day_harib12f_asmhead_OUTS}_f.txt ${15_day_harib12f_asmhead_WINE_OUTS}_t.txt
)

add_dependencies(15_day_harib12f_asmhead_od 15_day_harib12f_asmhead)
add_dependencies(15_day_harib12f_asmhead_od 15_day_harib12f_asmhead_wine)
add_dependencies(od 15_day_harib12f_asmhead_od)

#----------------------------------------------------------
########### next target ###############
set(15_day_harib12f_ipl10_SRCS ${root_SOURCE_DIR}/projects/15_day/harib12f/ipl10.nas)
set(15_day_harib12f_ipl10_OUTS ${root_BINARY_DIR}/projects/15_day/harib12f/ipl10.bin)

add_custom_target(15_day_harib12f_ipl10
  COMMAND ${GOSK_EXECUTABLE} ${15_day_harib12f_ipl10_SRCS} ${15_day_harib12f_ipl10_OUTS}
  DEPENDS ${15_day_harib12f_ipl10_SRCS}
)

set(15_day_harib12f_ipl10_WINE_OUTS ${root_BINARY_DIR}/projects/15_day/harib12f/ipl10_wine.bin)
add_custom_target(15_day_harib12f_ipl10_wine
  COMMAND ${WINE} ${WINE_NASK} ${15_day_harib12f_ipl10_SRCS} ${15_day_harib12f_ipl10_WINE_OUTS}
)
add_dependencies(wine 15_day_harib12f_ipl10_wine)

add_custom_target(15_day_harib12f_ipl10_od
  COMMAND ${OD} -t x1 ${15_day_harib12f_ipl10_OUTS}      > ${15_day_harib12f_ipl10_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${15_day_harib12f_ipl10_WINE_OUTS} > ${15_day_harib12f_ipl10_WINE_OUTS}_t.txt
  COMMAND diff -s ${15_day_harib12f_ipl10_OUTS}_f.txt ${15_day_harib12f_ipl10_WINE_OUTS}_t.txt
)

add_dependencies(15_day_harib12f_ipl10_od 15_day_harib12f_ipl10)
add_dependencies(15_day_harib12f_ipl10_od 15_day_harib12f_ipl10_wine)
add_dependencies(od 15_day_harib12f_ipl10_od)

#----------------------------------------------------------
########### next target ###############
set(15_day_harib12f_naskfunc_SRCS ${root_SOURCE_DIR}/projects/15_day/harib12f/naskfunc.nas)
set(15_day_harib12f_naskfunc_OUTS ${root_BINARY_DIR}/projects/15_day/harib12f/naskfunc.img)

add_custom_target(15_day_harib12f_naskfunc
  COMMAND ${GOSK_EXECUTABLE} ${15_day_harib12f_naskfunc_SRCS} ${15_day_harib12f_naskfunc_OUTS}
  DEPENDS ${15_day_harib12f_naskfunc_SRCS}
)

add_dependencies(images 15_day_harib12f_naskfunc)
set(15_day_harib12f_naskfunc_WINE_OUTS ${root_BINARY_DIR}/projects/15_day/harib12f/naskfunc_wine.img)
add_custom_target(15_day_harib12f_naskfunc_wine
  COMMAND ${WINE} ${WINE_NASK} ${15_day_harib12f_naskfunc_SRCS} ${15_day_harib12f_naskfunc_WINE_OUTS}
)
add_dependencies(wine 15_day_harib12f_naskfunc_wine)

add_custom_target(15_day_harib12f_naskfunc_od
  COMMAND ${OD} -t x1 ${15_day_harib12f_naskfunc_OUTS}      > ${15_day_harib12f_naskfunc_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${15_day_harib12f_naskfunc_WINE_OUTS} > ${15_day_harib12f_naskfunc_WINE_OUTS}_t.txt
  COMMAND diff -s ${15_day_harib12f_naskfunc_OUTS}_f.txt ${15_day_harib12f_naskfunc_WINE_OUTS}_t.txt
)

add_dependencies(15_day_harib12f_naskfunc_od 15_day_harib12f_naskfunc)
add_dependencies(15_day_harib12f_naskfunc_od 15_day_harib12f_naskfunc_wine)
add_dependencies(od 15_day_harib12f_naskfunc_od)

#----------------------------------------------------------
