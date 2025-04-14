#----------------------------------------------------------
message(STATUS "Entering directory projects/16_day/harib13e/")

set(FONT ${root_BINARY_DIR}/src/makefont)
set(B2O  ${root_BINARY_DIR}/src/bin2obj)
set(CONV ${root_BINARY_DIR}/objconv/objconv)
set(16_day_harib13e_WILDOBJ ${root_BINARY_DIR}/projects/16_day/harib13e/*.o)

# Full OS build rules
set(16_day_harib13e_OS    ${root_BINARY_DIR}/projects/16_day/harib13e/os.img)
set(16_day_harib13e_SYS   ${root_BINARY_DIR}/projects/16_day/harib13e/os.sys)
set(16_day_harib13e_IPLB  ${root_BINARY_DIR}/projects/16_day/harib13e/ipl.bin)
set(16_day_harib13e_IPLS  ${root_SOURCE_DIR}/projects/16_day/harib13e/ipl10.nas)
set(16_day_harib13e_HEADB ${root_BINARY_DIR}/projects/16_day/harib13e/asmhead.bin)
set(16_day_harib13e_HEADS ${root_SOURCE_DIR}/projects/16_day/harib13e/asmhead.nas)
set(16_day_harib13e_BOOTB ${root_BINARY_DIR}/projects/16_day/harib13e/boot.bin)
set(16_day_harib13e_CCO   ${root_BINARY_DIR}/projects/16_day/harib13e/boot.o)
set(16_day_harib13e_CCS   ${root_SOURCE_DIR}/projects/16_day/harib13e/*.c)
set(16_day_harib13e_LDS   ${root_SOURCE_DIR}/projects/os.lds)
set(16_day_harib13e_FUNCS ${root_SOURCE_DIR}/projects/16_day/harib13e/naskfunc.nas)
set(16_day_harib13e_FUNCO ${root_BINARY_DIR}/projects/16_day/harib13e/naskfunc.o)
set(16_day_harib13e_FONTS ${root_SOURCE_DIR}/projects/16_day/harib13e/hankaku.txt)
set(16_day_harib13e_FONTB ${root_BINARY_DIR}/projects/16_day/harib13e/hankaku.bin)
set(16_day_harib13e_FONTO ${root_BINARY_DIR}/projects/16_day/harib13e/hankaku.o)
set(16_day_harib13e_LIBGE ${root_BINARY_DIR}/golibc/libgo.a)
set(16_day_harib13e_LIBGC ${root_BINARY_DIR}/projects/16_day/harib13e/libgo.o)

add_custom_target(16_day_harib13e_run
  COMMAND ${QEMU} ${QEMUOPT} ${16_day_harib13e_OS}
  DEPENDS 16_day_harib13e_img
)
add_custom_target(16_day_harib13e_debug
  COMMAND ${QEMU} -s -S ${QEMUOPT} ${16_day_harib13e_OS} ${QEMU_DEBUG_OPT}
  DEPENDS 16_day_harib13e_img
)
add_custom_target(16_day_harib13e_clean
  COMMAND rm -f ${16_day_harib13e_OS}
  COMMAND rm -f ${16_day_harib13e_SYS}
  COMMAND rm -f ${16_day_harib13e_IPLB}
  COMMAND rm -f ${16_day_harib13e_HEADB}
  COMMAND rm -f ${16_day_harib13e_BOOTB}
  COMMAND rm -f ${16_day_harib13e_WILDOBJ}
  COMMAND rm -f ${root_BINARY_DIR}/projects/16_day/harib13e/ipl10.bin
  COMMAND rm -f ${root_BINARY_DIR}/projects/16_day/harib13e/asmhead.bin
  COMMAND rm -f ${root_BINARY_DIR}/projects/16_day/harib13e/*_wine.bin
  COMMAND rm -f ${root_BINARY_DIR}/projects/16_day/harib13e/*.txt
)
add_custom_target(16_day_harib13e_ipl_build
  COMMAND ${GOSK_EXECUTABLE} ${16_day_harib13e_IPLS} ${16_day_harib13e_IPLB}
  DEPENDS ${16_day_harib13e_IPLS}
)
add_custom_command(
  OUTPUT ${16_day_harib13e_HEADB}
  COMMAND ${GOSK_EXECUTABLE} ${16_day_harib13e_HEADS} ${16_day_harib13e_HEADB}
  DEPENDS ${16_day_harib13e_HEADS}
  COMMENT "Generating asmhead.bin for 16_day_harib13e"
)

add_custom_target(16_day_harib13e_sys
  COMMAND ${FONT} ${16_day_harib13e_FONTS} ${16_day_harib13e_FONTB}
  COMMAND ${B2O}  ${16_day_harib13e_FONTB} ${16_day_harib13e_FONTO} _hankaku
  COMMAND ${CONV} -fcoff32 -nu ${16_day_harib13e_LIBGE} ${16_day_harib13e_LIBGC}
  COMMAND ${GOSK_EXECUTABLE} ${16_day_harib13e_FUNCS} ${16_day_harib13e_FUNCO}
  COMMAND gcc ${BINOPT} -T ${16_day_harib13e_LDS} ${16_day_harib13e_CCS} ${16_day_harib13e_WILDOBJ} -o ${16_day_harib13e_BOOTB}
  COMMAND cat ${16_day_harib13e_HEADB} ${16_day_harib13e_BOOTB} > ${16_day_harib13e_SYS}
  DEPENDS ${16_day_harib13e_HEADB} 16_day_harib13e_ipl_build
  DEPENDS ${16_day_harib13e_FUNCO}
  DEPENDS ${16_day_harib13e_FONTO}
)
add_custom_target(16_day_harib13e_img
  COMMAND mformat -f 1440 -l HARIBOTEOS -N 0xffffffff -C -B ${16_day_harib13e_IPLB} -i ${16_day_harib13e_OS}
  COMMAND mcopy -i ${16_day_harib13e_OS} ${16_day_harib13e_SYS} ::
  DEPENDS 16_day_harib13e_sys
)

########### next target ###############
set(16_day_harib13e_asmhead_SRCS ${root_SOURCE_DIR}/projects/16_day/harib13e/asmhead.nas)
set(16_day_harib13e_asmhead_OUTS ${root_BINARY_DIR}/projects/16_day/harib13e/asmhead.bin)

add_custom_target(16_day_harib13e_asmhead
  COMMAND ${GOSK_EXECUTABLE} ${16_day_harib13e_asmhead_SRCS} ${16_day_harib13e_asmhead_OUTS}
  DEPENDS ${16_day_harib13e_asmhead_SRCS}
)

set(16_day_harib13e_asmhead_WINE_OUTS ${root_BINARY_DIR}/projects/16_day/harib13e/asmhead_wine.bin)
add_custom_target(16_day_harib13e_asmhead_wine
  COMMAND ${WINE} ${WINE_NASK} ${16_day_harib13e_asmhead_SRCS} ${16_day_harib13e_asmhead_WINE_OUTS}
)
add_dependencies(wine 16_day_harib13e_asmhead_wine)

add_custom_target(16_day_harib13e_asmhead_od
  COMMAND ${OD} -t x1 ${16_day_harib13e_asmhead_OUTS}      > ${16_day_harib13e_asmhead_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${16_day_harib13e_asmhead_WINE_OUTS} > ${16_day_harib13e_asmhead_WINE_OUTS}_t.txt
  COMMAND diff -s ${16_day_harib13e_asmhead_OUTS}_f.txt ${16_day_harib13e_asmhead_WINE_OUTS}_t.txt
)

add_dependencies(16_day_harib13e_asmhead_od 16_day_harib13e_asmhead)
add_dependencies(16_day_harib13e_asmhead_od 16_day_harib13e_asmhead_wine)
add_dependencies(od 16_day_harib13e_asmhead_od)

#----------------------------------------------------------
########### next target ###############
set(16_day_harib13e_ipl10_SRCS ${root_SOURCE_DIR}/projects/16_day/harib13e/ipl10.nas)
set(16_day_harib13e_ipl10_OUTS ${root_BINARY_DIR}/projects/16_day/harib13e/ipl10.bin)

add_custom_target(16_day_harib13e_ipl10
  COMMAND ${GOSK_EXECUTABLE} ${16_day_harib13e_ipl10_SRCS} ${16_day_harib13e_ipl10_OUTS}
  DEPENDS ${16_day_harib13e_ipl10_SRCS}
)

set(16_day_harib13e_ipl10_WINE_OUTS ${root_BINARY_DIR}/projects/16_day/harib13e/ipl10_wine.bin)
add_custom_target(16_day_harib13e_ipl10_wine
  COMMAND ${WINE} ${WINE_NASK} ${16_day_harib13e_ipl10_SRCS} ${16_day_harib13e_ipl10_WINE_OUTS}
)
add_dependencies(wine 16_day_harib13e_ipl10_wine)

add_custom_target(16_day_harib13e_ipl10_od
  COMMAND ${OD} -t x1 ${16_day_harib13e_ipl10_OUTS}      > ${16_day_harib13e_ipl10_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${16_day_harib13e_ipl10_WINE_OUTS} > ${16_day_harib13e_ipl10_WINE_OUTS}_t.txt
  COMMAND diff -s ${16_day_harib13e_ipl10_OUTS}_f.txt ${16_day_harib13e_ipl10_WINE_OUTS}_t.txt
)

add_dependencies(16_day_harib13e_ipl10_od 16_day_harib13e_ipl10)
add_dependencies(16_day_harib13e_ipl10_od 16_day_harib13e_ipl10_wine)
add_dependencies(od 16_day_harib13e_ipl10_od)

#----------------------------------------------------------
########### next target ###############
set(16_day_harib13e_naskfunc_SRCS ${root_SOURCE_DIR}/projects/16_day/harib13e/naskfunc.nas)
set(16_day_harib13e_naskfunc_OUTS ${root_BINARY_DIR}/projects/16_day/harib13e/naskfunc.img)

add_custom_target(16_day_harib13e_naskfunc
  COMMAND ${GOSK_EXECUTABLE} ${16_day_harib13e_naskfunc_SRCS} ${16_day_harib13e_naskfunc_OUTS}
  DEPENDS ${16_day_harib13e_naskfunc_SRCS}
)

add_dependencies(images 16_day_harib13e_naskfunc)
set(16_day_harib13e_naskfunc_WINE_OUTS ${root_BINARY_DIR}/projects/16_day/harib13e/naskfunc_wine.img)
add_custom_target(16_day_harib13e_naskfunc_wine
  COMMAND ${WINE} ${WINE_NASK} ${16_day_harib13e_naskfunc_SRCS} ${16_day_harib13e_naskfunc_WINE_OUTS}
)
add_dependencies(wine 16_day_harib13e_naskfunc_wine)

add_custom_target(16_day_harib13e_naskfunc_od
  COMMAND ${OD} -t x1 ${16_day_harib13e_naskfunc_OUTS}      > ${16_day_harib13e_naskfunc_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${16_day_harib13e_naskfunc_WINE_OUTS} > ${16_day_harib13e_naskfunc_WINE_OUTS}_t.txt
  COMMAND diff -s ${16_day_harib13e_naskfunc_OUTS}_f.txt ${16_day_harib13e_naskfunc_WINE_OUTS}_t.txt
)

add_dependencies(16_day_harib13e_naskfunc_od 16_day_harib13e_naskfunc)
add_dependencies(16_day_harib13e_naskfunc_od 16_day_harib13e_naskfunc_wine)
add_dependencies(od 16_day_harib13e_naskfunc_od)

#----------------------------------------------------------
