#----------------------------------------------------------
message(STATUS "Entering directory projects/15_day/harib12g/")

set(FONT ${RAKUSK_EXECUTABLE} --makefont)
set(B2O  ${RAKUSK_EXECUTABLE} --bin2obj)
set(CONV ${root_BINARY_DIR}/objconv/objconv)
set(15_day_harib12g_WILDOBJ ${root_BINARY_DIR}/projects/15_day/harib12g/*.o)

# Full OS build rules
set(15_day_harib12g_OS    ${root_BINARY_DIR}/projects/15_day/harib12g/os.img)
set(15_day_harib12g_SYS   ${root_BINARY_DIR}/projects/15_day/harib12g/os.sys)
set(15_day_harib12g_IPLB  ${root_BINARY_DIR}/projects/15_day/harib12g/ipl.bin)
set(15_day_harib12g_IPLS  ${root_SOURCE_DIR}/projects/15_day/harib12g/ipl10.nas)
set(15_day_harib12g_HEADB ${root_BINARY_DIR}/projects/15_day/harib12g/asmhead.bin)
set(15_day_harib12g_HEADS ${root_SOURCE_DIR}/projects/15_day/harib12g/asmhead.nas)
set(15_day_harib12g_BOOTB ${root_BINARY_DIR}/projects/15_day/harib12g/boot.bin)
set(15_day_harib12g_CCO   ${root_BINARY_DIR}/projects/15_day/harib12g/boot.o)
set(15_day_harib12g_CCS   ${root_SOURCE_DIR}/projects/15_day/harib12g/*.c)
set(15_day_harib12g_LDS   ${root_SOURCE_DIR}/projects/os.lds)
set(15_day_harib12g_FUNCS ${root_SOURCE_DIR}/projects/15_day/harib12g/naskfunc.nas)
set(15_day_harib12g_FUNCO ${root_BINARY_DIR}/projects/15_day/harib12g/naskfunc.o)
set(15_day_harib12g_FONTS ${root_SOURCE_DIR}/projects/15_day/harib12g/hankaku.txt)
set(15_day_harib12g_FONTB ${root_BINARY_DIR}/projects/15_day/harib12g/hankaku.bin)
set(15_day_harib12g_FONTO ${root_BINARY_DIR}/projects/15_day/harib12g/hankaku.o)
set(15_day_harib12g_LIBGE ${root_BINARY_DIR}/golibc/libgo.a)
set(15_day_harib12g_LIBGC ${root_BINARY_DIR}/projects/15_day/harib12g/libgo.o)

add_custom_target(15_day_harib12g_run
  COMMAND ${QEMU} ${QEMUOPT} ${15_day_harib12g_OS}
  DEPENDS 15_day_harib12g_img
)
add_custom_target(15_day_harib12g_debug
  COMMAND ${QEMU} -s -S ${QEMUOPT} ${15_day_harib12g_OS} ${QEMU_DEBUG_OPT}
  DEPENDS 15_day_harib12g_img
)
add_custom_target(15_day_harib12g_clean
  COMMAND rm -f ${15_day_harib12g_OS}
  COMMAND rm -f ${15_day_harib12g_SYS}
  COMMAND rm -f ${15_day_harib12g_IPLB}
  COMMAND rm -f ${15_day_harib12g_HEADB}
  COMMAND rm -f ${15_day_harib12g_BOOTB}
  COMMAND rm -f ${15_day_harib12g_WILDOBJ}
  COMMAND rm -f ${root_BINARY_DIR}/projects/15_day/harib12g/ipl10.bin
  COMMAND rm -f ${root_BINARY_DIR}/projects/15_day/harib12g/asmhead.bin
  COMMAND rm -f ${root_BINARY_DIR}/projects/15_day/harib12g/*_wine.bin
  COMMAND rm -f ${root_BINARY_DIR}/projects/15_day/harib12g/*.txt
)
add_custom_target(15_day_harib12g_ipl_build
  COMMAND ${RAKUSK_EXECUTABLE} ${15_day_harib12g_IPLS} ${15_day_harib12g_IPLB}
  DEPENDS ${15_day_harib12g_IPLS}
)
add_custom_command(
  OUTPUT ${15_day_harib12g_HEADB}
  COMMAND ${RAKUSK_EXECUTABLE} ${15_day_harib12g_HEADS} ${15_day_harib12g_HEADB}
  DEPENDS ${15_day_harib12g_HEADS}
  COMMENT "Generating asmhead.bin for 15_day_harib12g"
)

add_custom_target(15_day_harib12g_sys
  COMMAND ${FONT} ${15_day_harib12g_FONTS} ${15_day_harib12g_FONTB}
  COMMAND ${B2O}  ${15_day_harib12g_FONTB} ${15_day_harib12g_FONTO} _hankaku
  COMMAND ${CONV} -fcoff32 -nu ${15_day_harib12g_LIBGE} ${15_day_harib12g_LIBGC}
  COMMAND ${RAKUSK_EXECUTABLE} ${15_day_harib12g_FUNCS} ${15_day_harib12g_FUNCO}
  COMMAND gcc ${BINOPT} -T ${15_day_harib12g_LDS} ${15_day_harib12g_CCS} ${15_day_harib12g_WILDOBJ} -o ${15_day_harib12g_BOOTB}
  COMMAND cat ${15_day_harib12g_HEADB} ${15_day_harib12g_BOOTB} > ${15_day_harib12g_SYS}
  DEPENDS ${15_day_harib12g_HEADB} 15_day_harib12g_ipl_build
  DEPENDS ${15_day_harib12g_FUNCO}
  DEPENDS ${15_day_harib12g_FONTO}
)
add_custom_target(15_day_harib12g_img
  COMMAND mformat -f 1440 -l HARIBOTEOS -N 0xffffffff -C -B ${15_day_harib12g_IPLB} -i ${15_day_harib12g_OS}
  COMMAND mcopy -i ${15_day_harib12g_OS} ${15_day_harib12g_SYS} ::
  DEPENDS 15_day_harib12g_sys
)

########### next target ###############
set(15_day_harib12g_asmhead_SRCS ${root_SOURCE_DIR}/projects/15_day/harib12g/asmhead.nas)
set(15_day_harib12g_asmhead_OUTS ${root_BINARY_DIR}/projects/15_day/harib12g/asmhead.bin)

add_custom_target(15_day_harib12g_asmhead
  COMMAND ${RAKUSK_EXECUTABLE} ${15_day_harib12g_asmhead_SRCS} ${15_day_harib12g_asmhead_OUTS}
  DEPENDS ${15_day_harib12g_asmhead_SRCS}
)

set(15_day_harib12g_asmhead_WINE_OUTS ${root_BINARY_DIR}/projects/15_day/harib12g/asmhead_wine.bin)
add_custom_target(15_day_harib12g_asmhead_wine
  COMMAND ${WINE} ${WINE_NASK} ${15_day_harib12g_asmhead_SRCS} ${15_day_harib12g_asmhead_WINE_OUTS}
)
add_dependencies(wine 15_day_harib12g_asmhead_wine)

add_custom_target(15_day_harib12g_asmhead_od
  COMMAND ${OD} -t x1 ${15_day_harib12g_asmhead_OUTS}      > ${15_day_harib12g_asmhead_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${15_day_harib12g_asmhead_WINE_OUTS} > ${15_day_harib12g_asmhead_WINE_OUTS}_t.txt
  COMMAND diff -s ${15_day_harib12g_asmhead_OUTS}_f.txt ${15_day_harib12g_asmhead_WINE_OUTS}_t.txt
)

add_dependencies(15_day_harib12g_asmhead_od 15_day_harib12g_asmhead)
add_dependencies(15_day_harib12g_asmhead_od 15_day_harib12g_asmhead_wine)
add_dependencies(od 15_day_harib12g_asmhead_od)

#----------------------------------------------------------
########### next target ###############
set(15_day_harib12g_ipl10_SRCS ${root_SOURCE_DIR}/projects/15_day/harib12g/ipl10.nas)
set(15_day_harib12g_ipl10_OUTS ${root_BINARY_DIR}/projects/15_day/harib12g/ipl10.bin)

add_custom_target(15_day_harib12g_ipl10
  COMMAND ${RAKUSK_EXECUTABLE} ${15_day_harib12g_ipl10_SRCS} ${15_day_harib12g_ipl10_OUTS}
  DEPENDS ${15_day_harib12g_ipl10_SRCS}
)

set(15_day_harib12g_ipl10_WINE_OUTS ${root_BINARY_DIR}/projects/15_day/harib12g/ipl10_wine.bin)
add_custom_target(15_day_harib12g_ipl10_wine
  COMMAND ${WINE} ${WINE_NASK} ${15_day_harib12g_ipl10_SRCS} ${15_day_harib12g_ipl10_WINE_OUTS}
)
add_dependencies(wine 15_day_harib12g_ipl10_wine)

add_custom_target(15_day_harib12g_ipl10_od
  COMMAND ${OD} -t x1 ${15_day_harib12g_ipl10_OUTS}      > ${15_day_harib12g_ipl10_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${15_day_harib12g_ipl10_WINE_OUTS} > ${15_day_harib12g_ipl10_WINE_OUTS}_t.txt
  COMMAND diff -s ${15_day_harib12g_ipl10_OUTS}_f.txt ${15_day_harib12g_ipl10_WINE_OUTS}_t.txt
)

add_dependencies(15_day_harib12g_ipl10_od 15_day_harib12g_ipl10)
add_dependencies(15_day_harib12g_ipl10_od 15_day_harib12g_ipl10_wine)
add_dependencies(od 15_day_harib12g_ipl10_od)

#----------------------------------------------------------
########### next target ###############
set(15_day_harib12g_naskfunc_SRCS ${root_SOURCE_DIR}/projects/15_day/harib12g/naskfunc.nas)
set(15_day_harib12g_naskfunc_OUTS ${root_BINARY_DIR}/projects/15_day/harib12g/naskfunc.img)

add_custom_target(15_day_harib12g_naskfunc
  COMMAND ${RAKUSK_EXECUTABLE} ${15_day_harib12g_naskfunc_SRCS} ${15_day_harib12g_naskfunc_OUTS}
  DEPENDS ${15_day_harib12g_naskfunc_SRCS}
)

add_dependencies(images 15_day_harib12g_naskfunc)
set(15_day_harib12g_naskfunc_WINE_OUTS ${root_BINARY_DIR}/projects/15_day/harib12g/naskfunc_wine.img)
add_custom_target(15_day_harib12g_naskfunc_wine
  COMMAND ${WINE} ${WINE_NASK} ${15_day_harib12g_naskfunc_SRCS} ${15_day_harib12g_naskfunc_WINE_OUTS}
)
add_dependencies(wine 15_day_harib12g_naskfunc_wine)

add_custom_target(15_day_harib12g_naskfunc_od
  COMMAND ${OD} -t x1 ${15_day_harib12g_naskfunc_OUTS}      > ${15_day_harib12g_naskfunc_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${15_day_harib12g_naskfunc_WINE_OUTS} > ${15_day_harib12g_naskfunc_WINE_OUTS}_t.txt
  COMMAND diff -s ${15_day_harib12g_naskfunc_OUTS}_f.txt ${15_day_harib12g_naskfunc_WINE_OUTS}_t.txt
)

add_dependencies(15_day_harib12g_naskfunc_od 15_day_harib12g_naskfunc)
add_dependencies(15_day_harib12g_naskfunc_od 15_day_harib12g_naskfunc_wine)
add_dependencies(od 15_day_harib12g_naskfunc_od)

#----------------------------------------------------------
