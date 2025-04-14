#----------------------------------------------------------
message(STATUS "Entering directory projects/18_day/harib15d/")

set(FONT ${root_BINARY_DIR}/src/makefont)
set(B2O  ${root_BINARY_DIR}/src/bin2obj)
set(CONV ${root_BINARY_DIR}/objconv/objconv)
set(18_day_harib15d_WILDOBJ ${root_BINARY_DIR}/projects/18_day/harib15d/*.o)

# Full OS build rules
set(18_day_harib15d_OS    ${root_BINARY_DIR}/projects/18_day/harib15d/os.img)
set(18_day_harib15d_SYS   ${root_BINARY_DIR}/projects/18_day/harib15d/os.sys)
set(18_day_harib15d_IPLB  ${root_BINARY_DIR}/projects/18_day/harib15d/ipl.bin)
set(18_day_harib15d_IPLS  ${root_SOURCE_DIR}/projects/18_day/harib15d/ipl10.nas)
set(18_day_harib15d_HEADB ${root_BINARY_DIR}/projects/18_day/harib15d/asmhead.bin)
set(18_day_harib15d_HEADS ${root_SOURCE_DIR}/projects/18_day/harib15d/asmhead.nas)
set(18_day_harib15d_BOOTB ${root_BINARY_DIR}/projects/18_day/harib15d/boot.bin)
set(18_day_harib15d_CCO   ${root_BINARY_DIR}/projects/18_day/harib15d/boot.o)
set(18_day_harib15d_CCS   ${root_SOURCE_DIR}/projects/18_day/harib15d/*.c)
set(18_day_harib15d_LDS   ${root_SOURCE_DIR}/projects/os.lds)
set(18_day_harib15d_FUNCS ${root_SOURCE_DIR}/projects/18_day/harib15d/naskfunc.nas)
set(18_day_harib15d_FUNCO ${root_BINARY_DIR}/projects/18_day/harib15d/naskfunc.o)
set(18_day_harib15d_FONTS ${root_SOURCE_DIR}/projects/18_day/harib15d/hankaku.txt)
set(18_day_harib15d_FONTB ${root_BINARY_DIR}/projects/18_day/harib15d/hankaku.bin)
set(18_day_harib15d_FONTO ${root_BINARY_DIR}/projects/18_day/harib15d/hankaku.o)
set(18_day_harib15d_LIBGE ${root_BINARY_DIR}/golibc/libgo.a)
set(18_day_harib15d_LIBGC ${root_BINARY_DIR}/projects/18_day/harib15d/libgo.o)

add_custom_target(18_day_harib15d_run
  COMMAND ${QEMU} ${QEMUOPT} ${18_day_harib15d_OS}
  DEPENDS 18_day_harib15d_img
)
add_custom_target(18_day_harib15d_debug
  COMMAND ${QEMU} -s -S ${QEMUOPT} ${18_day_harib15d_OS} ${QEMU_DEBUG_OPT}
  DEPENDS 18_day_harib15d_img
)
add_custom_target(18_day_harib15d_clean
  COMMAND rm -f ${18_day_harib15d_OS}
  COMMAND rm -f ${18_day_harib15d_SYS}
  COMMAND rm -f ${18_day_harib15d_IPLB}
  COMMAND rm -f ${18_day_harib15d_HEADB}
  COMMAND rm -f ${18_day_harib15d_BOOTB}
  COMMAND rm -f ${18_day_harib15d_WILDOBJ}
  COMMAND rm -f ${root_BINARY_DIR}/projects/18_day/harib15d/ipl10.bin
  COMMAND rm -f ${root_BINARY_DIR}/projects/18_day/harib15d/asmhead.bin
  COMMAND rm -f ${root_BINARY_DIR}/projects/18_day/harib15d/*_wine.bin
  COMMAND rm -f ${root_BINARY_DIR}/projects/18_day/harib15d/*.txt
)
add_custom_target(18_day_harib15d_ipl_build
  COMMAND ${GOSK_EXECUTABLE} ${18_day_harib15d_IPLS} ${18_day_harib15d_IPLB}
  DEPENDS ${18_day_harib15d_IPLS}
)
add_custom_command(
  OUTPUT ${18_day_harib15d_HEADB}
  COMMAND ${GOSK_EXECUTABLE} ${18_day_harib15d_HEADS} ${18_day_harib15d_HEADB}
  DEPENDS ${18_day_harib15d_HEADS}
  COMMENT "Generating asmhead.bin for 18_day_harib15d"
)

add_custom_target(18_day_harib15d_sys
  COMMAND ${FONT} ${18_day_harib15d_FONTS} ${18_day_harib15d_FONTB}
  COMMAND ${B2O}  ${18_day_harib15d_FONTB} ${18_day_harib15d_FONTO} _hankaku
  COMMAND ${CONV} -fcoff32 -nu ${18_day_harib15d_LIBGE} ${18_day_harib15d_LIBGC}
  COMMAND ${GOSK_EXECUTABLE} ${18_day_harib15d_FUNCS} ${18_day_harib15d_FUNCO}
  COMMAND gcc ${BINOPT} -T ${18_day_harib15d_LDS} ${18_day_harib15d_CCS} ${18_day_harib15d_WILDOBJ} -o ${18_day_harib15d_BOOTB}
  COMMAND cat ${18_day_harib15d_HEADB} ${18_day_harib15d_BOOTB} > ${18_day_harib15d_SYS}
  DEPENDS ${18_day_harib15d_HEADB} 18_day_harib15d_ipl_build
  DEPENDS ${18_day_harib15d_FUNCO}
  DEPENDS ${18_day_harib15d_FONTO}
)
add_custom_target(18_day_harib15d_img
  COMMAND mformat -f 1440 -l HARIBOTEOS -N 0xffffffff -C -B ${18_day_harib15d_IPLB} -i ${18_day_harib15d_OS}
  COMMAND mcopy -i ${18_day_harib15d_OS} ${18_day_harib15d_SYS} ::
  DEPENDS 18_day_harib15d_sys
)

########### next target ###############
set(18_day_harib15d_asmhead_SRCS ${root_SOURCE_DIR}/projects/18_day/harib15d/asmhead.nas)
set(18_day_harib15d_asmhead_OUTS ${root_BINARY_DIR}/projects/18_day/harib15d/asmhead.bin)

add_custom_target(18_day_harib15d_asmhead
  COMMAND ${GOSK_EXECUTABLE} ${18_day_harib15d_asmhead_SRCS} ${18_day_harib15d_asmhead_OUTS}
  DEPENDS ${18_day_harib15d_asmhead_SRCS}
)

set(18_day_harib15d_asmhead_WINE_OUTS ${root_BINARY_DIR}/projects/18_day/harib15d/asmhead_wine.bin)
add_custom_target(18_day_harib15d_asmhead_wine
  COMMAND ${WINE} ${WINE_NASK} ${18_day_harib15d_asmhead_SRCS} ${18_day_harib15d_asmhead_WINE_OUTS}
)
add_dependencies(wine 18_day_harib15d_asmhead_wine)

add_custom_target(18_day_harib15d_asmhead_od
  COMMAND ${OD} -t x1 ${18_day_harib15d_asmhead_OUTS}      > ${18_day_harib15d_asmhead_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${18_day_harib15d_asmhead_WINE_OUTS} > ${18_day_harib15d_asmhead_WINE_OUTS}_t.txt
  COMMAND diff -s ${18_day_harib15d_asmhead_OUTS}_f.txt ${18_day_harib15d_asmhead_WINE_OUTS}_t.txt
)

add_dependencies(18_day_harib15d_asmhead_od 18_day_harib15d_asmhead)
add_dependencies(18_day_harib15d_asmhead_od 18_day_harib15d_asmhead_wine)
add_dependencies(od 18_day_harib15d_asmhead_od)

#----------------------------------------------------------
########### next target ###############
set(18_day_harib15d_ipl10_SRCS ${root_SOURCE_DIR}/projects/18_day/harib15d/ipl10.nas)
set(18_day_harib15d_ipl10_OUTS ${root_BINARY_DIR}/projects/18_day/harib15d/ipl10.bin)

add_custom_target(18_day_harib15d_ipl10
  COMMAND ${GOSK_EXECUTABLE} ${18_day_harib15d_ipl10_SRCS} ${18_day_harib15d_ipl10_OUTS}
  DEPENDS ${18_day_harib15d_ipl10_SRCS}
)

set(18_day_harib15d_ipl10_WINE_OUTS ${root_BINARY_DIR}/projects/18_day/harib15d/ipl10_wine.bin)
add_custom_target(18_day_harib15d_ipl10_wine
  COMMAND ${WINE} ${WINE_NASK} ${18_day_harib15d_ipl10_SRCS} ${18_day_harib15d_ipl10_WINE_OUTS}
)
add_dependencies(wine 18_day_harib15d_ipl10_wine)

add_custom_target(18_day_harib15d_ipl10_od
  COMMAND ${OD} -t x1 ${18_day_harib15d_ipl10_OUTS}      > ${18_day_harib15d_ipl10_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${18_day_harib15d_ipl10_WINE_OUTS} > ${18_day_harib15d_ipl10_WINE_OUTS}_t.txt
  COMMAND diff -s ${18_day_harib15d_ipl10_OUTS}_f.txt ${18_day_harib15d_ipl10_WINE_OUTS}_t.txt
)

add_dependencies(18_day_harib15d_ipl10_od 18_day_harib15d_ipl10)
add_dependencies(18_day_harib15d_ipl10_od 18_day_harib15d_ipl10_wine)
add_dependencies(od 18_day_harib15d_ipl10_od)

#----------------------------------------------------------
########### next target ###############
set(18_day_harib15d_naskfunc_SRCS ${root_SOURCE_DIR}/projects/18_day/harib15d/naskfunc.nas)
set(18_day_harib15d_naskfunc_OUTS ${root_BINARY_DIR}/projects/18_day/harib15d/naskfunc.img)

add_custom_target(18_day_harib15d_naskfunc
  COMMAND ${GOSK_EXECUTABLE} ${18_day_harib15d_naskfunc_SRCS} ${18_day_harib15d_naskfunc_OUTS}
  DEPENDS ${18_day_harib15d_naskfunc_SRCS}
)

add_dependencies(images 18_day_harib15d_naskfunc)
set(18_day_harib15d_naskfunc_WINE_OUTS ${root_BINARY_DIR}/projects/18_day/harib15d/naskfunc_wine.img)
add_custom_target(18_day_harib15d_naskfunc_wine
  COMMAND ${WINE} ${WINE_NASK} ${18_day_harib15d_naskfunc_SRCS} ${18_day_harib15d_naskfunc_WINE_OUTS}
)
add_dependencies(wine 18_day_harib15d_naskfunc_wine)

add_custom_target(18_day_harib15d_naskfunc_od
  COMMAND ${OD} -t x1 ${18_day_harib15d_naskfunc_OUTS}      > ${18_day_harib15d_naskfunc_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${18_day_harib15d_naskfunc_WINE_OUTS} > ${18_day_harib15d_naskfunc_WINE_OUTS}_t.txt
  COMMAND diff -s ${18_day_harib15d_naskfunc_OUTS}_f.txt ${18_day_harib15d_naskfunc_WINE_OUTS}_t.txt
)

add_dependencies(18_day_harib15d_naskfunc_od 18_day_harib15d_naskfunc)
add_dependencies(18_day_harib15d_naskfunc_od 18_day_harib15d_naskfunc_wine)
add_dependencies(od 18_day_harib15d_naskfunc_od)

#----------------------------------------------------------
