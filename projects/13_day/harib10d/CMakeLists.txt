#----------------------------------------------------------
message(STATUS "Entering directory projects/13_day/harib10d/")

set(FONT ${root_BINARY_DIR}/src/makefont)
set(B2O  ${root_BINARY_DIR}/src/bin2obj)
set(CONV ${root_BINARY_DIR}/objconv/objconv)
set(13_day_harib10d_WILDOBJ ${root_BINARY_DIR}/projects/13_day/harib10d/*.o)

# Full OS build rules
set(13_day_harib10d_OS    ${root_BINARY_DIR}/projects/13_day/harib10d/os.img)
set(13_day_harib10d_SYS   ${root_BINARY_DIR}/projects/13_day/harib10d/os.sys)
set(13_day_harib10d_IPLB  ${root_BINARY_DIR}/projects/13_day/harib10d/ipl.bin)
set(13_day_harib10d_IPLS  ${root_SOURCE_DIR}/projects/13_day/harib10d/ipl10.nas)
set(13_day_harib10d_HEADB ${root_BINARY_DIR}/projects/13_day/harib10d/asmhead.bin)
set(13_day_harib10d_HEADS ${root_SOURCE_DIR}/projects/13_day/harib10d/asmhead.nas)
set(13_day_harib10d_BOOTB ${root_BINARY_DIR}/projects/13_day/harib10d/boot.bin)
set(13_day_harib10d_CCO   ${root_BINARY_DIR}/projects/13_day/harib10d/boot.o)
set(13_day_harib10d_CCS   ${root_SOURCE_DIR}/projects/13_day/harib10d/*.c)
set(13_day_harib10d_LDS   ${root_SOURCE_DIR}/projects/os.lds)
set(13_day_harib10d_FUNCS ${root_SOURCE_DIR}/projects/13_day/harib10d/naskfunc.nas)
set(13_day_harib10d_FUNCO ${root_BINARY_DIR}/projects/13_day/harib10d/naskfunc.o)
set(13_day_harib10d_FONTS ${root_SOURCE_DIR}/projects/13_day/harib10d/hankaku.txt)
set(13_day_harib10d_FONTB ${root_BINARY_DIR}/projects/13_day/harib10d/hankaku.bin)
set(13_day_harib10d_FONTO ${root_BINARY_DIR}/projects/13_day/harib10d/hankaku.o)
set(13_day_harib10d_LIBGE ${root_BINARY_DIR}/golibc/libgo.a)
set(13_day_harib10d_LIBGC ${root_BINARY_DIR}/projects/13_day/harib10d/libgo.o)

add_custom_target(13_day_harib10d_run
  COMMAND ${QEMU} ${QEMUOPT} ${13_day_harib10d_OS}
  DEPENDS 13_day_harib10d_img
)
add_custom_target(13_day_harib10d_debug
  COMMAND ${QEMU} -s -S ${QEMUOPT} ${13_day_harib10d_OS} ${QEMU_DEBUG_OPT}
  DEPENDS 13_day_harib10d_img
)
add_custom_target(13_day_harib10d_clean
  COMMAND rm -f ${13_day_harib10d_OS}
  COMMAND rm -f ${13_day_harib10d_SYS}
  COMMAND rm -f ${13_day_harib10d_IPLB}
  COMMAND rm -f ${13_day_harib10d_HEADB}
  COMMAND rm -f ${13_day_harib10d_BOOTB}
  COMMAND rm -f ${13_day_harib10d_WILDOBJ}
  COMMAND rm -f ${root_BINARY_DIR}/projects/13_day/harib10d/ipl10.bin
  COMMAND rm -f ${root_BINARY_DIR}/projects/13_day/harib10d/asmhead.bin
  COMMAND rm -f ${root_BINARY_DIR}/projects/13_day/harib10d/*_wine.bin
  COMMAND rm -f ${root_BINARY_DIR}/projects/13_day/harib10d/*.txt
)
add_custom_target(13_day_harib10d_ipl_build
  COMMAND ${GOSK_EXECUTABLE} ${13_day_harib10d_IPLS} ${13_day_harib10d_IPLB}
  DEPENDS ${13_day_harib10d_IPLS}
)
add_custom_command(
  OUTPUT ${13_day_harib10d_HEADB}
  COMMAND ${GOSK_EXECUTABLE} ${13_day_harib10d_HEADS} ${13_day_harib10d_HEADB}
  DEPENDS ${13_day_harib10d_HEADS}
  COMMENT "Generating asmhead.bin for 13_day_harib10d"
)

add_custom_target(13_day_harib10d_sys
  COMMAND ${FONT} ${13_day_harib10d_FONTS} ${13_day_harib10d_FONTB}
  COMMAND ${B2O}  ${13_day_harib10d_FONTB} ${13_day_harib10d_FONTO} _hankaku
  COMMAND ${CONV} -fcoff32 -nu ${13_day_harib10d_LIBGE} ${13_day_harib10d_LIBGC}
  COMMAND ${GOSK_EXECUTABLE} ${13_day_harib10d_FUNCS} ${13_day_harib10d_FUNCO}
  COMMAND gcc ${BINOPT} -T ${13_day_harib10d_LDS} ${13_day_harib10d_CCS} ${13_day_harib10d_WILDOBJ} -o ${13_day_harib10d_BOOTB}
  COMMAND cat ${13_day_harib10d_HEADB} ${13_day_harib10d_BOOTB} > ${13_day_harib10d_SYS}
  DEPENDS ${13_day_harib10d_HEADB} 13_day_harib10d_ipl_build
  DEPENDS ${13_day_harib10d_FUNCO}
  DEPENDS ${13_day_harib10d_FONTO}
)
add_custom_target(13_day_harib10d_img
  COMMAND mformat -f 1440 -l HARIBOTEOS -N 0xffffffff -C -B ${13_day_harib10d_IPLB} -i ${13_day_harib10d_OS}
  COMMAND mcopy -i ${13_day_harib10d_OS} ${13_day_harib10d_SYS} ::
  DEPENDS 13_day_harib10d_sys
)

########### next target ###############
set(13_day_harib10d_asmhead_SRCS ${root_SOURCE_DIR}/projects/13_day/harib10d/asmhead.nas)
set(13_day_harib10d_asmhead_OUTS ${root_BINARY_DIR}/projects/13_day/harib10d/asmhead.bin)

add_custom_target(13_day_harib10d_asmhead
  COMMAND ${GOSK_EXECUTABLE} ${13_day_harib10d_asmhead_SRCS} ${13_day_harib10d_asmhead_OUTS}
  DEPENDS ${13_day_harib10d_asmhead_SRCS}
)

set(13_day_harib10d_asmhead_WINE_OUTS ${root_BINARY_DIR}/projects/13_day/harib10d/asmhead_wine.bin)
add_custom_target(13_day_harib10d_asmhead_wine
  COMMAND ${WINE} ${WINE_NASK} ${13_day_harib10d_asmhead_SRCS} ${13_day_harib10d_asmhead_WINE_OUTS}
)
add_dependencies(wine 13_day_harib10d_asmhead_wine)

add_custom_target(13_day_harib10d_asmhead_od
  COMMAND ${OD} -t x1 ${13_day_harib10d_asmhead_OUTS}      > ${13_day_harib10d_asmhead_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${13_day_harib10d_asmhead_WINE_OUTS} > ${13_day_harib10d_asmhead_WINE_OUTS}_t.txt
  COMMAND diff -s ${13_day_harib10d_asmhead_OUTS}_f.txt ${13_day_harib10d_asmhead_WINE_OUTS}_t.txt
)

add_dependencies(13_day_harib10d_asmhead_od 13_day_harib10d_asmhead)
add_dependencies(13_day_harib10d_asmhead_od 13_day_harib10d_asmhead_wine)
add_dependencies(od 13_day_harib10d_asmhead_od)

#----------------------------------------------------------
########### next target ###############
set(13_day_harib10d_ipl10_SRCS ${root_SOURCE_DIR}/projects/13_day/harib10d/ipl10.nas)
set(13_day_harib10d_ipl10_OUTS ${root_BINARY_DIR}/projects/13_day/harib10d/ipl10.bin)

add_custom_target(13_day_harib10d_ipl10
  COMMAND ${GOSK_EXECUTABLE} ${13_day_harib10d_ipl10_SRCS} ${13_day_harib10d_ipl10_OUTS}
  DEPENDS ${13_day_harib10d_ipl10_SRCS}
)

set(13_day_harib10d_ipl10_WINE_OUTS ${root_BINARY_DIR}/projects/13_day/harib10d/ipl10_wine.bin)
add_custom_target(13_day_harib10d_ipl10_wine
  COMMAND ${WINE} ${WINE_NASK} ${13_day_harib10d_ipl10_SRCS} ${13_day_harib10d_ipl10_WINE_OUTS}
)
add_dependencies(wine 13_day_harib10d_ipl10_wine)

add_custom_target(13_day_harib10d_ipl10_od
  COMMAND ${OD} -t x1 ${13_day_harib10d_ipl10_OUTS}      > ${13_day_harib10d_ipl10_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${13_day_harib10d_ipl10_WINE_OUTS} > ${13_day_harib10d_ipl10_WINE_OUTS}_t.txt
  COMMAND diff -s ${13_day_harib10d_ipl10_OUTS}_f.txt ${13_day_harib10d_ipl10_WINE_OUTS}_t.txt
)

add_dependencies(13_day_harib10d_ipl10_od 13_day_harib10d_ipl10)
add_dependencies(13_day_harib10d_ipl10_od 13_day_harib10d_ipl10_wine)
add_dependencies(od 13_day_harib10d_ipl10_od)

#----------------------------------------------------------
########### next target ###############
set(13_day_harib10d_naskfunc_SRCS ${root_SOURCE_DIR}/projects/13_day/harib10d/naskfunc.nas)
set(13_day_harib10d_naskfunc_OUTS ${root_BINARY_DIR}/projects/13_day/harib10d/naskfunc.img)

add_custom_target(13_day_harib10d_naskfunc
  COMMAND ${GOSK_EXECUTABLE} ${13_day_harib10d_naskfunc_SRCS} ${13_day_harib10d_naskfunc_OUTS}
  DEPENDS ${13_day_harib10d_naskfunc_SRCS}
)

add_dependencies(images 13_day_harib10d_naskfunc)
set(13_day_harib10d_naskfunc_WINE_OUTS ${root_BINARY_DIR}/projects/13_day/harib10d/naskfunc_wine.img)
add_custom_target(13_day_harib10d_naskfunc_wine
  COMMAND ${WINE} ${WINE_NASK} ${13_day_harib10d_naskfunc_SRCS} ${13_day_harib10d_naskfunc_WINE_OUTS}
)
add_dependencies(wine 13_day_harib10d_naskfunc_wine)

add_custom_target(13_day_harib10d_naskfunc_od
  COMMAND ${OD} -t x1 ${13_day_harib10d_naskfunc_OUTS}      > ${13_day_harib10d_naskfunc_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${13_day_harib10d_naskfunc_WINE_OUTS} > ${13_day_harib10d_naskfunc_WINE_OUTS}_t.txt
  COMMAND diff -s ${13_day_harib10d_naskfunc_OUTS}_f.txt ${13_day_harib10d_naskfunc_WINE_OUTS}_t.txt
)

add_dependencies(13_day_harib10d_naskfunc_od 13_day_harib10d_naskfunc)
add_dependencies(13_day_harib10d_naskfunc_od 13_day_harib10d_naskfunc_wine)
add_dependencies(od 13_day_harib10d_naskfunc_od)

#----------------------------------------------------------
