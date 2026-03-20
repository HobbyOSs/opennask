#----------------------------------------------------------
message(STATUS "Entering directory projects/13_day/harib10a/")

set(FONT ${RAKUSK_EXECUTABLE} --makefont)
set(B2O  ${RAKUSK_EXECUTABLE} --bin2obj)
set(CONV ${root_BINARY_DIR}/objconv/objconv)
set(13_day_harib10a_WILDOBJ ${root_BINARY_DIR}/projects/13_day/harib10a/*.o)

# Full OS build rules
set(13_day_harib10a_OS    ${root_BINARY_DIR}/projects/13_day/harib10a/os.img)
set(13_day_harib10a_SYS   ${root_BINARY_DIR}/projects/13_day/harib10a/os.sys)
set(13_day_harib10a_IPLB  ${root_BINARY_DIR}/projects/13_day/harib10a/ipl.bin)
set(13_day_harib10a_IPLS  ${root_SOURCE_DIR}/projects/13_day/harib10a/ipl10.nas)
set(13_day_harib10a_HEADB ${root_BINARY_DIR}/projects/13_day/harib10a/asmhead.bin)
set(13_day_harib10a_HEADS ${root_SOURCE_DIR}/projects/13_day/harib10a/asmhead.nas)
set(13_day_harib10a_BOOTB ${root_BINARY_DIR}/projects/13_day/harib10a/boot.bin)
set(13_day_harib10a_CCO   ${root_BINARY_DIR}/projects/13_day/harib10a/boot.o)
set(13_day_harib10a_CCS   ${root_SOURCE_DIR}/projects/13_day/harib10a/*.c)
set(13_day_harib10a_LDS   ${root_SOURCE_DIR}/projects/os.lds)
set(13_day_harib10a_FUNCS ${root_SOURCE_DIR}/projects/13_day/harib10a/naskfunc.nas)
set(13_day_harib10a_FUNCO ${root_BINARY_DIR}/projects/13_day/harib10a/naskfunc.o)
set(13_day_harib10a_FONTS ${root_SOURCE_DIR}/projects/13_day/harib10a/hankaku.txt)
set(13_day_harib10a_FONTB ${root_BINARY_DIR}/projects/13_day/harib10a/hankaku.bin)
set(13_day_harib10a_FONTO ${root_BINARY_DIR}/projects/13_day/harib10a/hankaku.o)
set(13_day_harib10a_LIBGE ${root_BINARY_DIR}/golibc/libgo.a)
set(13_day_harib10a_LIBGC ${root_BINARY_DIR}/projects/13_day/harib10a/libgo.o)

add_custom_target(13_day_harib10a_run
  COMMAND ${QEMU} ${QEMUOPT} ${13_day_harib10a_OS}
  DEPENDS 13_day_harib10a_img
)
add_custom_target(13_day_harib10a_debug
  COMMAND ${QEMU} -s -S ${QEMUOPT} ${13_day_harib10a_OS} ${QEMU_DEBUG_OPT}
  DEPENDS 13_day_harib10a_img
)
add_custom_target(13_day_harib10a_clean
  COMMAND rm -f ${13_day_harib10a_OS}
  COMMAND rm -f ${13_day_harib10a_SYS}
  COMMAND rm -f ${13_day_harib10a_IPLB}
  COMMAND rm -f ${13_day_harib10a_HEADB}
  COMMAND rm -f ${13_day_harib10a_BOOTB}
  COMMAND rm -f ${13_day_harib10a_WILDOBJ}
  COMMAND rm -f ${root_BINARY_DIR}/projects/13_day/harib10a/ipl10.bin
  COMMAND rm -f ${root_BINARY_DIR}/projects/13_day/harib10a/asmhead.bin
  COMMAND rm -f ${root_BINARY_DIR}/projects/13_day/harib10a/*_wine.bin
  COMMAND rm -f ${root_BINARY_DIR}/projects/13_day/harib10a/*.txt
)
add_custom_target(13_day_harib10a_ipl_build
  COMMAND ${RAKUSK_EXECUTABLE} ${13_day_harib10a_IPLS} ${13_day_harib10a_IPLB}
  DEPENDS ${13_day_harib10a_IPLS}
)
add_custom_command(
  OUTPUT ${13_day_harib10a_HEADB}
  COMMAND ${RAKUSK_EXECUTABLE} ${13_day_harib10a_HEADS} ${13_day_harib10a_HEADB}
  DEPENDS ${13_day_harib10a_HEADS}
  COMMENT "Generating asmhead.bin for 13_day_harib10a"
)

add_custom_target(13_day_harib10a_sys
  COMMAND ${FONT} ${13_day_harib10a_FONTS} ${13_day_harib10a_FONTB}
  COMMAND ${B2O}  ${13_day_harib10a_FONTB} ${13_day_harib10a_FONTO} _hankaku
  COMMAND ${CONV} -fcoff32 -nu ${13_day_harib10a_LIBGE} ${13_day_harib10a_LIBGC}
  COMMAND ${RAKUSK_EXECUTABLE} ${13_day_harib10a_FUNCS} ${13_day_harib10a_FUNCO}
  COMMAND gcc ${BINOPT} -T ${13_day_harib10a_LDS} ${13_day_harib10a_CCS} ${13_day_harib10a_WILDOBJ} -o ${13_day_harib10a_BOOTB}
  COMMAND cat ${13_day_harib10a_HEADB} ${13_day_harib10a_BOOTB} > ${13_day_harib10a_SYS}
  DEPENDS ${13_day_harib10a_HEADB} 13_day_harib10a_ipl_build
  DEPENDS ${13_day_harib10a_FUNCO}
  DEPENDS ${13_day_harib10a_FONTO}
)
add_custom_target(13_day_harib10a_img
  COMMAND mformat -f 1440 -l HARIBOTEOS -N 0xffffffff -C -B ${13_day_harib10a_IPLB} -i ${13_day_harib10a_OS}
  COMMAND mcopy -i ${13_day_harib10a_OS} ${13_day_harib10a_SYS} ::
  DEPENDS 13_day_harib10a_sys
)

########### next target ###############
set(13_day_harib10a_asmhead_SRCS ${root_SOURCE_DIR}/projects/13_day/harib10a/asmhead.nas)
set(13_day_harib10a_asmhead_OUTS ${root_BINARY_DIR}/projects/13_day/harib10a/asmhead.bin)

add_custom_target(13_day_harib10a_asmhead
  COMMAND ${RAKUSK_EXECUTABLE} ${13_day_harib10a_asmhead_SRCS} ${13_day_harib10a_asmhead_OUTS}
  DEPENDS ${13_day_harib10a_asmhead_SRCS}
)

set(13_day_harib10a_asmhead_WINE_OUTS ${root_BINARY_DIR}/projects/13_day/harib10a/asmhead_wine.bin)
add_custom_target(13_day_harib10a_asmhead_wine
  COMMAND ${WINE} ${WINE_NASK} ${13_day_harib10a_asmhead_SRCS} ${13_day_harib10a_asmhead_WINE_OUTS}
)
add_dependencies(wine 13_day_harib10a_asmhead_wine)

add_custom_target(13_day_harib10a_asmhead_od
  COMMAND ${OD} -t x1 ${13_day_harib10a_asmhead_OUTS}      > ${13_day_harib10a_asmhead_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${13_day_harib10a_asmhead_WINE_OUTS} > ${13_day_harib10a_asmhead_WINE_OUTS}_t.txt
  COMMAND diff -s ${13_day_harib10a_asmhead_OUTS}_f.txt ${13_day_harib10a_asmhead_WINE_OUTS}_t.txt
)

add_dependencies(13_day_harib10a_asmhead_od 13_day_harib10a_asmhead)
add_dependencies(13_day_harib10a_asmhead_od 13_day_harib10a_asmhead_wine)
add_dependencies(od 13_day_harib10a_asmhead_od)

#----------------------------------------------------------
########### next target ###############
set(13_day_harib10a_ipl10_SRCS ${root_SOURCE_DIR}/projects/13_day/harib10a/ipl10.nas)
set(13_day_harib10a_ipl10_OUTS ${root_BINARY_DIR}/projects/13_day/harib10a/ipl10.bin)

add_custom_target(13_day_harib10a_ipl10
  COMMAND ${RAKUSK_EXECUTABLE} ${13_day_harib10a_ipl10_SRCS} ${13_day_harib10a_ipl10_OUTS}
  DEPENDS ${13_day_harib10a_ipl10_SRCS}
)

set(13_day_harib10a_ipl10_WINE_OUTS ${root_BINARY_DIR}/projects/13_day/harib10a/ipl10_wine.bin)
add_custom_target(13_day_harib10a_ipl10_wine
  COMMAND ${WINE} ${WINE_NASK} ${13_day_harib10a_ipl10_SRCS} ${13_day_harib10a_ipl10_WINE_OUTS}
)
add_dependencies(wine 13_day_harib10a_ipl10_wine)

add_custom_target(13_day_harib10a_ipl10_od
  COMMAND ${OD} -t x1 ${13_day_harib10a_ipl10_OUTS}      > ${13_day_harib10a_ipl10_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${13_day_harib10a_ipl10_WINE_OUTS} > ${13_day_harib10a_ipl10_WINE_OUTS}_t.txt
  COMMAND diff -s ${13_day_harib10a_ipl10_OUTS}_f.txt ${13_day_harib10a_ipl10_WINE_OUTS}_t.txt
)

add_dependencies(13_day_harib10a_ipl10_od 13_day_harib10a_ipl10)
add_dependencies(13_day_harib10a_ipl10_od 13_day_harib10a_ipl10_wine)
add_dependencies(od 13_day_harib10a_ipl10_od)

#----------------------------------------------------------
########### next target ###############
set(13_day_harib10a_naskfunc_SRCS ${root_SOURCE_DIR}/projects/13_day/harib10a/naskfunc.nas)
set(13_day_harib10a_naskfunc_OUTS ${root_BINARY_DIR}/projects/13_day/harib10a/naskfunc.img)

add_custom_target(13_day_harib10a_naskfunc
  COMMAND ${RAKUSK_EXECUTABLE} ${13_day_harib10a_naskfunc_SRCS} ${13_day_harib10a_naskfunc_OUTS}
  DEPENDS ${13_day_harib10a_naskfunc_SRCS}
)

add_dependencies(images 13_day_harib10a_naskfunc)
set(13_day_harib10a_naskfunc_WINE_OUTS ${root_BINARY_DIR}/projects/13_day/harib10a/naskfunc_wine.img)
add_custom_target(13_day_harib10a_naskfunc_wine
  COMMAND ${WINE} ${WINE_NASK} ${13_day_harib10a_naskfunc_SRCS} ${13_day_harib10a_naskfunc_WINE_OUTS}
)
add_dependencies(wine 13_day_harib10a_naskfunc_wine)

add_custom_target(13_day_harib10a_naskfunc_od
  COMMAND ${OD} -t x1 ${13_day_harib10a_naskfunc_OUTS}      > ${13_day_harib10a_naskfunc_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${13_day_harib10a_naskfunc_WINE_OUTS} > ${13_day_harib10a_naskfunc_WINE_OUTS}_t.txt
  COMMAND diff -s ${13_day_harib10a_naskfunc_OUTS}_f.txt ${13_day_harib10a_naskfunc_WINE_OUTS}_t.txt
)

add_dependencies(13_day_harib10a_naskfunc_od 13_day_harib10a_naskfunc)
add_dependencies(13_day_harib10a_naskfunc_od 13_day_harib10a_naskfunc_wine)
add_dependencies(od 13_day_harib10a_naskfunc_od)

#----------------------------------------------------------
