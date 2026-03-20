#----------------------------------------------------------
message(STATUS "Entering directory projects/14_day/harib11b/")

set(FONT ${RAKUSK_EXECUTABLE} --makefont)
set(B2O  ${RAKUSK_EXECUTABLE} --bin2obj)
set(CONV ${root_BINARY_DIR}/objconv/objconv)
set(14_day_harib11b_WILDOBJ ${root_BINARY_DIR}/projects/14_day/harib11b/*.o)

# Full OS build rules
set(14_day_harib11b_OS    ${root_BINARY_DIR}/projects/14_day/harib11b/os.img)
set(14_day_harib11b_SYS   ${root_BINARY_DIR}/projects/14_day/harib11b/os.sys)
set(14_day_harib11b_IPLB  ${root_BINARY_DIR}/projects/14_day/harib11b/ipl.bin)
set(14_day_harib11b_IPLS  ${root_SOURCE_DIR}/projects/14_day/harib11b/ipl10.nas)
set(14_day_harib11b_HEADB ${root_BINARY_DIR}/projects/14_day/harib11b/asmhead.bin)
set(14_day_harib11b_HEADS ${root_SOURCE_DIR}/projects/14_day/harib11b/asmhead.nas)
set(14_day_harib11b_BOOTB ${root_BINARY_DIR}/projects/14_day/harib11b/boot.bin)
set(14_day_harib11b_CCO   ${root_BINARY_DIR}/projects/14_day/harib11b/boot.o)
set(14_day_harib11b_CCS   ${root_SOURCE_DIR}/projects/14_day/harib11b/*.c)
set(14_day_harib11b_LDS   ${root_SOURCE_DIR}/projects/os.lds)
set(14_day_harib11b_FUNCS ${root_SOURCE_DIR}/projects/14_day/harib11b/naskfunc.nas)
set(14_day_harib11b_FUNCO ${root_BINARY_DIR}/projects/14_day/harib11b/naskfunc.o)
set(14_day_harib11b_FONTS ${root_SOURCE_DIR}/projects/14_day/harib11b/hankaku.txt)
set(14_day_harib11b_FONTB ${root_BINARY_DIR}/projects/14_day/harib11b/hankaku.bin)
set(14_day_harib11b_FONTO ${root_BINARY_DIR}/projects/14_day/harib11b/hankaku.o)
set(14_day_harib11b_LIBGE ${root_BINARY_DIR}/golibc/libgo.a)
set(14_day_harib11b_LIBGC ${root_BINARY_DIR}/projects/14_day/harib11b/libgo.o)

add_custom_target(14_day_harib11b_run
  COMMAND ${QEMU} ${QEMUOPT} ${14_day_harib11b_OS}
  DEPENDS 14_day_harib11b_img
)
add_custom_target(14_day_harib11b_debug
  COMMAND ${QEMU} -s -S ${QEMUOPT} ${14_day_harib11b_OS} ${QEMU_DEBUG_OPT}
  DEPENDS 14_day_harib11b_img
)
add_custom_target(14_day_harib11b_clean
  COMMAND rm -f ${14_day_harib11b_OS}
  COMMAND rm -f ${14_day_harib11b_SYS}
  COMMAND rm -f ${14_day_harib11b_IPLB}
  COMMAND rm -f ${14_day_harib11b_HEADB}
  COMMAND rm -f ${14_day_harib11b_BOOTB}
  COMMAND rm -f ${14_day_harib11b_WILDOBJ}
  COMMAND rm -f ${root_BINARY_DIR}/projects/14_day/harib11b/ipl10.bin
  COMMAND rm -f ${root_BINARY_DIR}/projects/14_day/harib11b/asmhead.bin
  COMMAND rm -f ${root_BINARY_DIR}/projects/14_day/harib11b/*_wine.bin
  COMMAND rm -f ${root_BINARY_DIR}/projects/14_day/harib11b/*.txt
)
add_custom_target(14_day_harib11b_ipl_build
  COMMAND ${RAKUSK_EXECUTABLE} ${14_day_harib11b_IPLS} ${14_day_harib11b_IPLB}
  DEPENDS ${14_day_harib11b_IPLS}
)
add_custom_command(
  OUTPUT ${14_day_harib11b_HEADB}
  COMMAND ${RAKUSK_EXECUTABLE} ${14_day_harib11b_HEADS} ${14_day_harib11b_HEADB}
  DEPENDS ${14_day_harib11b_HEADS}
  COMMENT "Generating asmhead.bin for 14_day_harib11b"
)

add_custom_target(14_day_harib11b_sys
  COMMAND ${FONT} ${14_day_harib11b_FONTS} ${14_day_harib11b_FONTB}
  COMMAND ${B2O}  ${14_day_harib11b_FONTB} ${14_day_harib11b_FONTO} _hankaku
  COMMAND ${CONV} -fcoff32 -nu ${14_day_harib11b_LIBGE} ${14_day_harib11b_LIBGC}
  COMMAND ${RAKUSK_EXECUTABLE} ${14_day_harib11b_FUNCS} ${14_day_harib11b_FUNCO}
  COMMAND gcc ${BINOPT} -T ${14_day_harib11b_LDS} ${14_day_harib11b_CCS} ${14_day_harib11b_WILDOBJ} -o ${14_day_harib11b_BOOTB}
  COMMAND cat ${14_day_harib11b_HEADB} ${14_day_harib11b_BOOTB} > ${14_day_harib11b_SYS}
  DEPENDS ${14_day_harib11b_HEADB} 14_day_harib11b_ipl_build
  DEPENDS ${14_day_harib11b_FUNCO}
  DEPENDS ${14_day_harib11b_FONTO}
)
add_custom_target(14_day_harib11b_img
  COMMAND mformat -f 1440 -l HARIBOTEOS -N 0xffffffff -C -B ${14_day_harib11b_IPLB} -i ${14_day_harib11b_OS}
  COMMAND mcopy -i ${14_day_harib11b_OS} ${14_day_harib11b_SYS} ::
  DEPENDS 14_day_harib11b_sys
)

########### next target ###############
set(14_day_harib11b_asmhead_SRCS ${root_SOURCE_DIR}/projects/14_day/harib11b/asmhead.nas)
set(14_day_harib11b_asmhead_OUTS ${root_BINARY_DIR}/projects/14_day/harib11b/asmhead.bin)

add_custom_target(14_day_harib11b_asmhead
  COMMAND ${RAKUSK_EXECUTABLE} ${14_day_harib11b_asmhead_SRCS} ${14_day_harib11b_asmhead_OUTS}
  DEPENDS ${14_day_harib11b_asmhead_SRCS}
)

set(14_day_harib11b_asmhead_WINE_OUTS ${root_BINARY_DIR}/projects/14_day/harib11b/asmhead_wine.bin)
add_custom_target(14_day_harib11b_asmhead_wine
  COMMAND ${WINE} ${WINE_NASK} ${14_day_harib11b_asmhead_SRCS} ${14_day_harib11b_asmhead_WINE_OUTS}
)
add_dependencies(wine 14_day_harib11b_asmhead_wine)

add_custom_target(14_day_harib11b_asmhead_od
  COMMAND ${OD} -t x1 ${14_day_harib11b_asmhead_OUTS}      > ${14_day_harib11b_asmhead_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${14_day_harib11b_asmhead_WINE_OUTS} > ${14_day_harib11b_asmhead_WINE_OUTS}_t.txt
  COMMAND diff -s ${14_day_harib11b_asmhead_OUTS}_f.txt ${14_day_harib11b_asmhead_WINE_OUTS}_t.txt
)

add_dependencies(14_day_harib11b_asmhead_od 14_day_harib11b_asmhead)
add_dependencies(14_day_harib11b_asmhead_od 14_day_harib11b_asmhead_wine)
add_dependencies(od 14_day_harib11b_asmhead_od)

#----------------------------------------------------------
########### next target ###############
set(14_day_harib11b_ipl10_SRCS ${root_SOURCE_DIR}/projects/14_day/harib11b/ipl10.nas)
set(14_day_harib11b_ipl10_OUTS ${root_BINARY_DIR}/projects/14_day/harib11b/ipl10.bin)

add_custom_target(14_day_harib11b_ipl10
  COMMAND ${RAKUSK_EXECUTABLE} ${14_day_harib11b_ipl10_SRCS} ${14_day_harib11b_ipl10_OUTS}
  DEPENDS ${14_day_harib11b_ipl10_SRCS}
)

set(14_day_harib11b_ipl10_WINE_OUTS ${root_BINARY_DIR}/projects/14_day/harib11b/ipl10_wine.bin)
add_custom_target(14_day_harib11b_ipl10_wine
  COMMAND ${WINE} ${WINE_NASK} ${14_day_harib11b_ipl10_SRCS} ${14_day_harib11b_ipl10_WINE_OUTS}
)
add_dependencies(wine 14_day_harib11b_ipl10_wine)

add_custom_target(14_day_harib11b_ipl10_od
  COMMAND ${OD} -t x1 ${14_day_harib11b_ipl10_OUTS}      > ${14_day_harib11b_ipl10_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${14_day_harib11b_ipl10_WINE_OUTS} > ${14_day_harib11b_ipl10_WINE_OUTS}_t.txt
  COMMAND diff -s ${14_day_harib11b_ipl10_OUTS}_f.txt ${14_day_harib11b_ipl10_WINE_OUTS}_t.txt
)

add_dependencies(14_day_harib11b_ipl10_od 14_day_harib11b_ipl10)
add_dependencies(14_day_harib11b_ipl10_od 14_day_harib11b_ipl10_wine)
add_dependencies(od 14_day_harib11b_ipl10_od)

#----------------------------------------------------------
########### next target ###############
set(14_day_harib11b_naskfunc_SRCS ${root_SOURCE_DIR}/projects/14_day/harib11b/naskfunc.nas)
set(14_day_harib11b_naskfunc_OUTS ${root_BINARY_DIR}/projects/14_day/harib11b/naskfunc.img)

add_custom_target(14_day_harib11b_naskfunc
  COMMAND ${RAKUSK_EXECUTABLE} ${14_day_harib11b_naskfunc_SRCS} ${14_day_harib11b_naskfunc_OUTS}
  DEPENDS ${14_day_harib11b_naskfunc_SRCS}
)

add_dependencies(images 14_day_harib11b_naskfunc)
set(14_day_harib11b_naskfunc_WINE_OUTS ${root_BINARY_DIR}/projects/14_day/harib11b/naskfunc_wine.img)
add_custom_target(14_day_harib11b_naskfunc_wine
  COMMAND ${WINE} ${WINE_NASK} ${14_day_harib11b_naskfunc_SRCS} ${14_day_harib11b_naskfunc_WINE_OUTS}
)
add_dependencies(wine 14_day_harib11b_naskfunc_wine)

add_custom_target(14_day_harib11b_naskfunc_od
  COMMAND ${OD} -t x1 ${14_day_harib11b_naskfunc_OUTS}      > ${14_day_harib11b_naskfunc_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${14_day_harib11b_naskfunc_WINE_OUTS} > ${14_day_harib11b_naskfunc_WINE_OUTS}_t.txt
  COMMAND diff -s ${14_day_harib11b_naskfunc_OUTS}_f.txt ${14_day_harib11b_naskfunc_WINE_OUTS}_t.txt
)

add_dependencies(14_day_harib11b_naskfunc_od 14_day_harib11b_naskfunc)
add_dependencies(14_day_harib11b_naskfunc_od 14_day_harib11b_naskfunc_wine)
add_dependencies(od 14_day_harib11b_naskfunc_od)

#----------------------------------------------------------
