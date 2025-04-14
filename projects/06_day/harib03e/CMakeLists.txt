#----------------------------------------------------------
message(STATUS "Entering directory projects/06_day/harib03e/")

set(FONT ${root_BINARY_DIR}/src/makefont)
set(B2O  ${root_BINARY_DIR}/src/bin2obj)
set(CONV ${root_BINARY_DIR}/objconv/objconv)
set(06_day_harib03e_WILDOBJ ${root_BINARY_DIR}/projects/06_day/harib03e/*.o)

# Full OS build rules
set(06_day_harib03e_OS    ${root_BINARY_DIR}/projects/06_day/harib03e/os.img)
set(06_day_harib03e_SYS   ${root_BINARY_DIR}/projects/06_day/harib03e/os.sys)
set(06_day_harib03e_IPLB  ${root_BINARY_DIR}/projects/06_day/harib03e/ipl.bin)
set(06_day_harib03e_IPLS  ${root_SOURCE_DIR}/projects/06_day/harib03e/ipl10.nas)
set(06_day_harib03e_HEADB ${root_BINARY_DIR}/projects/06_day/harib03e/asmhead.bin)
set(06_day_harib03e_HEADS ${root_SOURCE_DIR}/projects/06_day/harib03e/asmhead.nas)
set(06_day_harib03e_BOOTB ${root_BINARY_DIR}/projects/06_day/harib03e/boot.bin)
set(06_day_harib03e_CCO   ${root_BINARY_DIR}/projects/06_day/harib03e/boot.o)
set(06_day_harib03e_CCS   ${root_SOURCE_DIR}/projects/06_day/harib03e/*.c)
set(06_day_harib03e_LDS   ${root_SOURCE_DIR}/projects/os.lds)
set(06_day_harib03e_FUNCS ${root_SOURCE_DIR}/projects/06_day/harib03e/naskfunc.nas)
set(06_day_harib03e_FUNCO ${root_BINARY_DIR}/projects/06_day/harib03e/naskfunc.o)
set(06_day_harib03e_FONTS ${root_SOURCE_DIR}/projects/06_day/harib03e/hankaku.txt)
set(06_day_harib03e_FONTB ${root_BINARY_DIR}/projects/06_day/harib03e/hankaku.bin)
set(06_day_harib03e_FONTO ${root_BINARY_DIR}/projects/06_day/harib03e/hankaku.o)
set(06_day_harib03e_LIBGE ${root_BINARY_DIR}/golibc/libgo.a)
set(06_day_harib03e_LIBGC ${root_BINARY_DIR}/projects/06_day/harib03e/libgo.o)

add_custom_target(06_day_harib03e_run
  COMMAND ${QEMU} ${QEMUOPT} ${06_day_harib03e_OS}
  DEPENDS 06_day_harib03e_img
)
add_custom_target(06_day_harib03e_debug
  COMMAND ${QEMU} -s -S ${QEMUOPT} ${06_day_harib03e_OS} ${QEMU_DEBUG_OPT}
  DEPENDS 06_day_harib03e_img
)
add_custom_target(06_day_harib03e_clean
  COMMAND rm -f ${06_day_harib03e_OS}
  COMMAND rm -f ${06_day_harib03e_SYS}
  COMMAND rm -f ${06_day_harib03e_IPLB}
  COMMAND rm -f ${06_day_harib03e_HEADB}
  COMMAND rm -f ${06_day_harib03e_BOOTB}
  COMMAND rm -f ${06_day_harib03e_WILDOBJ}
  COMMAND rm -f ${root_BINARY_DIR}/projects/06_day/harib03e/ipl10.bin
  COMMAND rm -f ${root_BINARY_DIR}/projects/06_day/harib03e/asmhead.bin
  COMMAND rm -f ${root_BINARY_DIR}/projects/06_day/harib03e/*_wine.bin
  COMMAND rm -f ${root_BINARY_DIR}/projects/06_day/harib03e/*.txt
)
add_custom_target(06_day_harib03e_ipl_build
  COMMAND ${GOSK_EXECUTABLE} ${06_day_harib03e_IPLS} ${06_day_harib03e_IPLB}
  DEPENDS ${06_day_harib03e_IPLS}
)
add_custom_command(
  OUTPUT ${06_day_harib03e_HEADB}
  COMMAND ${GOSK_EXECUTABLE} ${06_day_harib03e_HEADS} ${06_day_harib03e_HEADB}
  DEPENDS ${06_day_harib03e_HEADS}
  COMMENT "Generating asmhead.bin for 06_day_harib03e"
)

add_custom_target(06_day_harib03e_sys
  COMMAND ${FONT} ${06_day_harib03e_FONTS} ${06_day_harib03e_FONTB}
  COMMAND ${B2O}  ${06_day_harib03e_FONTB} ${06_day_harib03e_FONTO} _hankaku
  COMMAND ${CONV} -fcoff32 -nu ${06_day_harib03e_LIBGE} ${06_day_harib03e_LIBGC}
  COMMAND ${GOSK_EXECUTABLE} ${06_day_harib03e_FUNCS} ${06_day_harib03e_FUNCO}
  COMMAND gcc ${BINOPT} -T ${06_day_harib03e_LDS} ${06_day_harib03e_CCS} ${06_day_harib03e_WILDOBJ} -o ${06_day_harib03e_BOOTB}
  COMMAND cat ${06_day_harib03e_HEADB} ${06_day_harib03e_BOOTB} > ${06_day_harib03e_SYS}
  DEPENDS ${06_day_harib03e_HEADB} 06_day_harib03e_ipl_build
  DEPENDS ${06_day_harib03e_FUNCO}
  DEPENDS ${06_day_harib03e_FONTO}
)
add_custom_target(06_day_harib03e_img
  COMMAND mformat -f 1440 -l HARIBOTEOS -N 0xffffffff -C -B ${06_day_harib03e_IPLB} -i ${06_day_harib03e_OS}
  COMMAND mcopy -i ${06_day_harib03e_OS} ${06_day_harib03e_SYS} ::
  DEPENDS 06_day_harib03e_sys
)

########### next target ###############
set(06_day_harib03e_asmhead_SRCS ${root_SOURCE_DIR}/projects/06_day/harib03e/asmhead.nas)
set(06_day_harib03e_asmhead_OUTS ${root_BINARY_DIR}/projects/06_day/harib03e/asmhead.bin)

add_custom_target(06_day_harib03e_asmhead
  COMMAND ${GOSK_EXECUTABLE} ${06_day_harib03e_asmhead_SRCS} ${06_day_harib03e_asmhead_OUTS}
  DEPENDS ${06_day_harib03e_asmhead_SRCS}
)

set(06_day_harib03e_asmhead_WINE_OUTS ${root_BINARY_DIR}/projects/06_day/harib03e/asmhead_wine.bin)
add_custom_target(06_day_harib03e_asmhead_wine
  COMMAND ${WINE} ${WINE_NASK} ${06_day_harib03e_asmhead_SRCS} ${06_day_harib03e_asmhead_WINE_OUTS}
)
add_dependencies(wine 06_day_harib03e_asmhead_wine)

add_custom_target(06_day_harib03e_asmhead_od
  COMMAND ${OD} -t x1 ${06_day_harib03e_asmhead_OUTS}      > ${06_day_harib03e_asmhead_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${06_day_harib03e_asmhead_WINE_OUTS} > ${06_day_harib03e_asmhead_WINE_OUTS}_t.txt
  COMMAND diff -s ${06_day_harib03e_asmhead_OUTS}_f.txt ${06_day_harib03e_asmhead_WINE_OUTS}_t.txt
)

add_dependencies(06_day_harib03e_asmhead_od 06_day_harib03e_asmhead)
add_dependencies(06_day_harib03e_asmhead_od 06_day_harib03e_asmhead_wine)
add_dependencies(od 06_day_harib03e_asmhead_od)

#----------------------------------------------------------
########### next target ###############
set(06_day_harib03e_ipl10_SRCS ${root_SOURCE_DIR}/projects/06_day/harib03e/ipl10.nas)
set(06_day_harib03e_ipl10_OUTS ${root_BINARY_DIR}/projects/06_day/harib03e/ipl10.bin)

add_custom_target(06_day_harib03e_ipl10
  COMMAND ${GOSK_EXECUTABLE} ${06_day_harib03e_ipl10_SRCS} ${06_day_harib03e_ipl10_OUTS}
  DEPENDS ${06_day_harib03e_ipl10_SRCS}
)

set(06_day_harib03e_ipl10_WINE_OUTS ${root_BINARY_DIR}/projects/06_day/harib03e/ipl10_wine.bin)
add_custom_target(06_day_harib03e_ipl10_wine
  COMMAND ${WINE} ${WINE_NASK} ${06_day_harib03e_ipl10_SRCS} ${06_day_harib03e_ipl10_WINE_OUTS}
)
add_dependencies(wine 06_day_harib03e_ipl10_wine)

add_custom_target(06_day_harib03e_ipl10_od
  COMMAND ${OD} -t x1 ${06_day_harib03e_ipl10_OUTS}      > ${06_day_harib03e_ipl10_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${06_day_harib03e_ipl10_WINE_OUTS} > ${06_day_harib03e_ipl10_WINE_OUTS}_t.txt
  COMMAND diff -s ${06_day_harib03e_ipl10_OUTS}_f.txt ${06_day_harib03e_ipl10_WINE_OUTS}_t.txt
)

add_dependencies(06_day_harib03e_ipl10_od 06_day_harib03e_ipl10)
add_dependencies(06_day_harib03e_ipl10_od 06_day_harib03e_ipl10_wine)
add_dependencies(od 06_day_harib03e_ipl10_od)

#----------------------------------------------------------
########### next target ###############
set(06_day_harib03e_naskfunc_SRCS ${root_SOURCE_DIR}/projects/06_day/harib03e/naskfunc.nas)
set(06_day_harib03e_naskfunc_OUTS ${root_BINARY_DIR}/projects/06_day/harib03e/naskfunc.img)

add_custom_target(06_day_harib03e_naskfunc
  COMMAND ${GOSK_EXECUTABLE} ${06_day_harib03e_naskfunc_SRCS} ${06_day_harib03e_naskfunc_OUTS}
  DEPENDS ${06_day_harib03e_naskfunc_SRCS}
)

add_dependencies(images 06_day_harib03e_naskfunc)
set(06_day_harib03e_naskfunc_WINE_OUTS ${root_BINARY_DIR}/projects/06_day/harib03e/naskfunc_wine.img)
add_custom_target(06_day_harib03e_naskfunc_wine
  COMMAND ${WINE} ${WINE_NASK} ${06_day_harib03e_naskfunc_SRCS} ${06_day_harib03e_naskfunc_WINE_OUTS}
)
add_dependencies(wine 06_day_harib03e_naskfunc_wine)

add_custom_target(06_day_harib03e_naskfunc_od
  COMMAND ${OD} -t x1 ${06_day_harib03e_naskfunc_OUTS}      > ${06_day_harib03e_naskfunc_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${06_day_harib03e_naskfunc_WINE_OUTS} > ${06_day_harib03e_naskfunc_WINE_OUTS}_t.txt
  COMMAND diff -s ${06_day_harib03e_naskfunc_OUTS}_f.txt ${06_day_harib03e_naskfunc_WINE_OUTS}_t.txt
)

add_dependencies(06_day_harib03e_naskfunc_od 06_day_harib03e_naskfunc)
add_dependencies(06_day_harib03e_naskfunc_od 06_day_harib03e_naskfunc_wine)
add_dependencies(od 06_day_harib03e_naskfunc_od)

#----------------------------------------------------------
