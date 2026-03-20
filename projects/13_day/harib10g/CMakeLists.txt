#----------------------------------------------------------
message(STATUS "Entering directory projects/13_day/harib10g/")

set(FONT ${RAKUSK_EXECUTABLE} --makefont)
set(B2O  ${RAKUSK_EXECUTABLE} --bin2obj)
set(CONV ${root_BINARY_DIR}/objconv/objconv)
set(13_day_harib10g_WILDOBJ ${root_BINARY_DIR}/projects/13_day/harib10g/*.o)

# Full OS build rules
set(13_day_harib10g_OS    ${root_BINARY_DIR}/projects/13_day/harib10g/os.img)
set(13_day_harib10g_SYS   ${root_BINARY_DIR}/projects/13_day/harib10g/os.sys)
set(13_day_harib10g_IPLB  ${root_BINARY_DIR}/projects/13_day/harib10g/ipl.bin)
set(13_day_harib10g_IPLS  ${root_SOURCE_DIR}/projects/13_day/harib10g/ipl10.nas)
set(13_day_harib10g_HEADB ${root_BINARY_DIR}/projects/13_day/harib10g/asmhead.bin)
set(13_day_harib10g_HEADS ${root_SOURCE_DIR}/projects/13_day/harib10g/asmhead.nas)
set(13_day_harib10g_BOOTB ${root_BINARY_DIR}/projects/13_day/harib10g/boot.bin)
set(13_day_harib10g_CCO   ${root_BINARY_DIR}/projects/13_day/harib10g/boot.o)
set(13_day_harib10g_CCS   ${root_SOURCE_DIR}/projects/13_day/harib10g/*.c)
set(13_day_harib10g_LDS   ${root_SOURCE_DIR}/projects/os.lds)
set(13_day_harib10g_FUNCS ${root_SOURCE_DIR}/projects/13_day/harib10g/naskfunc.nas)
set(13_day_harib10g_FUNCO ${root_BINARY_DIR}/projects/13_day/harib10g/naskfunc.o)
set(13_day_harib10g_FONTS ${root_SOURCE_DIR}/projects/13_day/harib10g/hankaku.txt)
set(13_day_harib10g_FONTB ${root_BINARY_DIR}/projects/13_day/harib10g/hankaku.bin)
set(13_day_harib10g_FONTO ${root_BINARY_DIR}/projects/13_day/harib10g/hankaku.o)
set(13_day_harib10g_LIBGE ${root_BINARY_DIR}/golibc/libgo.a)
set(13_day_harib10g_LIBGC ${root_BINARY_DIR}/projects/13_day/harib10g/libgo.o)

add_custom_target(13_day_harib10g_run
  COMMAND ${QEMU} ${QEMUOPT} ${13_day_harib10g_OS}
  DEPENDS 13_day_harib10g_img
)
add_custom_target(13_day_harib10g_debug
  COMMAND ${QEMU} -s -S ${QEMUOPT} ${13_day_harib10g_OS} ${QEMU_DEBUG_OPT}
  DEPENDS 13_day_harib10g_img
)
add_custom_target(13_day_harib10g_clean
  COMMAND rm -f ${13_day_harib10g_OS}
  COMMAND rm -f ${13_day_harib10g_SYS}
  COMMAND rm -f ${13_day_harib10g_IPLB}
  COMMAND rm -f ${13_day_harib10g_HEADB}
  COMMAND rm -f ${13_day_harib10g_BOOTB}
  COMMAND rm -f ${13_day_harib10g_WILDOBJ}
  COMMAND rm -f ${root_BINARY_DIR}/projects/13_day/harib10g/ipl10.bin
  COMMAND rm -f ${root_BINARY_DIR}/projects/13_day/harib10g/asmhead.bin
  COMMAND rm -f ${root_BINARY_DIR}/projects/13_day/harib10g/*_wine.bin
  COMMAND rm -f ${root_BINARY_DIR}/projects/13_day/harib10g/*.txt
)
add_custom_target(13_day_harib10g_ipl_build
  COMMAND ${RAKUSK_EXECUTABLE} ${13_day_harib10g_IPLS} ${13_day_harib10g_IPLB}
  DEPENDS ${13_day_harib10g_IPLS}
)
add_custom_command(
  OUTPUT ${13_day_harib10g_HEADB}
  COMMAND ${RAKUSK_EXECUTABLE} ${13_day_harib10g_HEADS} ${13_day_harib10g_HEADB}
  DEPENDS ${13_day_harib10g_HEADS}
  COMMENT "Generating asmhead.bin for 13_day_harib10g"
)

add_custom_target(13_day_harib10g_sys
  COMMAND ${FONT} ${13_day_harib10g_FONTS} ${13_day_harib10g_FONTB}
  COMMAND ${B2O}  ${13_day_harib10g_FONTB} ${13_day_harib10g_FONTO} _hankaku
  COMMAND ${CONV} -fcoff32 -nu ${13_day_harib10g_LIBGE} ${13_day_harib10g_LIBGC}
  COMMAND ${RAKUSK_EXECUTABLE} ${13_day_harib10g_FUNCS} ${13_day_harib10g_FUNCO}
  COMMAND gcc ${BINOPT} -T ${13_day_harib10g_LDS} ${13_day_harib10g_CCS} ${13_day_harib10g_WILDOBJ} -o ${13_day_harib10g_BOOTB}
  COMMAND cat ${13_day_harib10g_HEADB} ${13_day_harib10g_BOOTB} > ${13_day_harib10g_SYS}
  DEPENDS ${13_day_harib10g_HEADB} 13_day_harib10g_ipl_build
  DEPENDS ${13_day_harib10g_FUNCO}
  DEPENDS ${13_day_harib10g_FONTO}
)
add_custom_target(13_day_harib10g_img
  COMMAND mformat -f 1440 -l HARIBOTEOS -N 0xffffffff -C -B ${13_day_harib10g_IPLB} -i ${13_day_harib10g_OS}
  COMMAND mcopy -i ${13_day_harib10g_OS} ${13_day_harib10g_SYS} ::
  DEPENDS 13_day_harib10g_sys
)

########### next target ###############
set(13_day_harib10g_asmhead_SRCS ${root_SOURCE_DIR}/projects/13_day/harib10g/asmhead.nas)
set(13_day_harib10g_asmhead_OUTS ${root_BINARY_DIR}/projects/13_day/harib10g/asmhead.bin)

add_custom_target(13_day_harib10g_asmhead
  COMMAND ${RAKUSK_EXECUTABLE} ${13_day_harib10g_asmhead_SRCS} ${13_day_harib10g_asmhead_OUTS}
  DEPENDS ${13_day_harib10g_asmhead_SRCS}
)

set(13_day_harib10g_asmhead_WINE_OUTS ${root_BINARY_DIR}/projects/13_day/harib10g/asmhead_wine.bin)
add_custom_target(13_day_harib10g_asmhead_wine
  COMMAND ${WINE} ${WINE_NASK} ${13_day_harib10g_asmhead_SRCS} ${13_day_harib10g_asmhead_WINE_OUTS}
)
add_dependencies(wine 13_day_harib10g_asmhead_wine)

add_custom_target(13_day_harib10g_asmhead_od
  COMMAND ${OD} -t x1 ${13_day_harib10g_asmhead_OUTS}      > ${13_day_harib10g_asmhead_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${13_day_harib10g_asmhead_WINE_OUTS} > ${13_day_harib10g_asmhead_WINE_OUTS}_t.txt
  COMMAND diff -s ${13_day_harib10g_asmhead_OUTS}_f.txt ${13_day_harib10g_asmhead_WINE_OUTS}_t.txt
)

add_dependencies(13_day_harib10g_asmhead_od 13_day_harib10g_asmhead)
add_dependencies(13_day_harib10g_asmhead_od 13_day_harib10g_asmhead_wine)
add_dependencies(od 13_day_harib10g_asmhead_od)

#----------------------------------------------------------
########### next target ###############
set(13_day_harib10g_ipl10_SRCS ${root_SOURCE_DIR}/projects/13_day/harib10g/ipl10.nas)
set(13_day_harib10g_ipl10_OUTS ${root_BINARY_DIR}/projects/13_day/harib10g/ipl10.bin)

add_custom_target(13_day_harib10g_ipl10
  COMMAND ${RAKUSK_EXECUTABLE} ${13_day_harib10g_ipl10_SRCS} ${13_day_harib10g_ipl10_OUTS}
  DEPENDS ${13_day_harib10g_ipl10_SRCS}
)

set(13_day_harib10g_ipl10_WINE_OUTS ${root_BINARY_DIR}/projects/13_day/harib10g/ipl10_wine.bin)
add_custom_target(13_day_harib10g_ipl10_wine
  COMMAND ${WINE} ${WINE_NASK} ${13_day_harib10g_ipl10_SRCS} ${13_day_harib10g_ipl10_WINE_OUTS}
)
add_dependencies(wine 13_day_harib10g_ipl10_wine)

add_custom_target(13_day_harib10g_ipl10_od
  COMMAND ${OD} -t x1 ${13_day_harib10g_ipl10_OUTS}      > ${13_day_harib10g_ipl10_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${13_day_harib10g_ipl10_WINE_OUTS} > ${13_day_harib10g_ipl10_WINE_OUTS}_t.txt
  COMMAND diff -s ${13_day_harib10g_ipl10_OUTS}_f.txt ${13_day_harib10g_ipl10_WINE_OUTS}_t.txt
)

add_dependencies(13_day_harib10g_ipl10_od 13_day_harib10g_ipl10)
add_dependencies(13_day_harib10g_ipl10_od 13_day_harib10g_ipl10_wine)
add_dependencies(od 13_day_harib10g_ipl10_od)

#----------------------------------------------------------
########### next target ###############
set(13_day_harib10g_naskfunc_SRCS ${root_SOURCE_DIR}/projects/13_day/harib10g/naskfunc.nas)
set(13_day_harib10g_naskfunc_OUTS ${root_BINARY_DIR}/projects/13_day/harib10g/naskfunc.img)

add_custom_target(13_day_harib10g_naskfunc
  COMMAND ${RAKUSK_EXECUTABLE} ${13_day_harib10g_naskfunc_SRCS} ${13_day_harib10g_naskfunc_OUTS}
  DEPENDS ${13_day_harib10g_naskfunc_SRCS}
)

add_dependencies(images 13_day_harib10g_naskfunc)
set(13_day_harib10g_naskfunc_WINE_OUTS ${root_BINARY_DIR}/projects/13_day/harib10g/naskfunc_wine.img)
add_custom_target(13_day_harib10g_naskfunc_wine
  COMMAND ${WINE} ${WINE_NASK} ${13_day_harib10g_naskfunc_SRCS} ${13_day_harib10g_naskfunc_WINE_OUTS}
)
add_dependencies(wine 13_day_harib10g_naskfunc_wine)

add_custom_target(13_day_harib10g_naskfunc_od
  COMMAND ${OD} -t x1 ${13_day_harib10g_naskfunc_OUTS}      > ${13_day_harib10g_naskfunc_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${13_day_harib10g_naskfunc_WINE_OUTS} > ${13_day_harib10g_naskfunc_WINE_OUTS}_t.txt
  COMMAND diff -s ${13_day_harib10g_naskfunc_OUTS}_f.txt ${13_day_harib10g_naskfunc_WINE_OUTS}_t.txt
)

add_dependencies(13_day_harib10g_naskfunc_od 13_day_harib10g_naskfunc)
add_dependencies(13_day_harib10g_naskfunc_od 13_day_harib10g_naskfunc_wine)
add_dependencies(od 13_day_harib10g_naskfunc_od)

#----------------------------------------------------------
