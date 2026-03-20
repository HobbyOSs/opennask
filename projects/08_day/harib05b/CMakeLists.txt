#----------------------------------------------------------
message(STATUS "Entering directory projects/08_day/harib05b/")

set(FONT ${RAKUSK_EXECUTABLE} --makefont)
set(B2O  ${RAKUSK_EXECUTABLE} --bin2obj)
set(CONV ${root_BINARY_DIR}/objconv/objconv)
set(08_day_harib05b_WILDOBJ ${root_BINARY_DIR}/projects/08_day/harib05b/*.o)

# Full OS build rules
set(08_day_harib05b_OS    ${root_BINARY_DIR}/projects/08_day/harib05b/os.img)
set(08_day_harib05b_SYS   ${root_BINARY_DIR}/projects/08_day/harib05b/os.sys)
set(08_day_harib05b_IPLB  ${root_BINARY_DIR}/projects/08_day/harib05b/ipl.bin)
set(08_day_harib05b_IPLS  ${root_SOURCE_DIR}/projects/08_day/harib05b/ipl10.nas)
set(08_day_harib05b_HEADB ${root_BINARY_DIR}/projects/08_day/harib05b/asmhead.bin)
set(08_day_harib05b_HEADS ${root_SOURCE_DIR}/projects/08_day/harib05b/asmhead.nas)
set(08_day_harib05b_BOOTB ${root_BINARY_DIR}/projects/08_day/harib05b/boot.bin)
set(08_day_harib05b_CCO   ${root_BINARY_DIR}/projects/08_day/harib05b/boot.o)
set(08_day_harib05b_CCS   ${root_SOURCE_DIR}/projects/08_day/harib05b/*.c)
set(08_day_harib05b_LDS   ${root_SOURCE_DIR}/projects/os.lds)
set(08_day_harib05b_FUNCS ${root_SOURCE_DIR}/projects/08_day/harib05b/naskfunc.nas)
set(08_day_harib05b_FUNCO ${root_BINARY_DIR}/projects/08_day/harib05b/naskfunc.o)
set(08_day_harib05b_FONTS ${root_SOURCE_DIR}/projects/08_day/harib05b/hankaku.txt)
set(08_day_harib05b_FONTB ${root_BINARY_DIR}/projects/08_day/harib05b/hankaku.bin)
set(08_day_harib05b_FONTO ${root_BINARY_DIR}/projects/08_day/harib05b/hankaku.o)
set(08_day_harib05b_LIBGE ${root_BINARY_DIR}/golibc/libgo.a)
set(08_day_harib05b_LIBGC ${root_BINARY_DIR}/projects/08_day/harib05b/libgo.o)

add_custom_target(08_day_harib05b_run
  COMMAND ${QEMU} ${QEMUOPT} ${08_day_harib05b_OS}
  DEPENDS 08_day_harib05b_img
)
add_custom_target(08_day_harib05b_debug
  COMMAND ${QEMU} -s -S ${QEMUOPT} ${08_day_harib05b_OS} ${QEMU_DEBUG_OPT}
  DEPENDS 08_day_harib05b_img
)
add_custom_target(08_day_harib05b_clean
  COMMAND rm -f ${08_day_harib05b_OS}
  COMMAND rm -f ${08_day_harib05b_SYS}
  COMMAND rm -f ${08_day_harib05b_IPLB}
  COMMAND rm -f ${08_day_harib05b_HEADB}
  COMMAND rm -f ${08_day_harib05b_BOOTB}
  COMMAND rm -f ${08_day_harib05b_WILDOBJ}
  COMMAND rm -f ${root_BINARY_DIR}/projects/08_day/harib05b/ipl10.bin
  COMMAND rm -f ${root_BINARY_DIR}/projects/08_day/harib05b/asmhead.bin
  COMMAND rm -f ${root_BINARY_DIR}/projects/08_day/harib05b/*_wine.bin
  COMMAND rm -f ${root_BINARY_DIR}/projects/08_day/harib05b/*.txt
)
add_custom_target(08_day_harib05b_ipl_build
  COMMAND ${RAKUSK_EXECUTABLE} ${08_day_harib05b_IPLS} ${08_day_harib05b_IPLB}
  DEPENDS ${08_day_harib05b_IPLS}
)
add_custom_command(
  OUTPUT ${08_day_harib05b_HEADB}
  COMMAND ${RAKUSK_EXECUTABLE} ${08_day_harib05b_HEADS} ${08_day_harib05b_HEADB}
  DEPENDS ${08_day_harib05b_HEADS}
  COMMENT "Generating asmhead.bin for 08_day_harib05b"
)

add_custom_target(08_day_harib05b_sys
  COMMAND ${FONT} ${08_day_harib05b_FONTS} ${08_day_harib05b_FONTB}
  COMMAND ${B2O}  ${08_day_harib05b_FONTB} ${08_day_harib05b_FONTO} _hankaku
  COMMAND ${CONV} -fcoff32 -nu ${08_day_harib05b_LIBGE} ${08_day_harib05b_LIBGC}
  COMMAND ${RAKUSK_EXECUTABLE} ${08_day_harib05b_FUNCS} ${08_day_harib05b_FUNCO}
  COMMAND gcc ${BINOPT} -T ${08_day_harib05b_LDS} ${08_day_harib05b_CCS} ${08_day_harib05b_WILDOBJ} -o ${08_day_harib05b_BOOTB}
  COMMAND cat ${08_day_harib05b_HEADB} ${08_day_harib05b_BOOTB} > ${08_day_harib05b_SYS}
  DEPENDS ${08_day_harib05b_HEADB} 08_day_harib05b_ipl_build
  DEPENDS ${08_day_harib05b_FUNCO}
  DEPENDS ${08_day_harib05b_FONTO}
)
add_custom_target(08_day_harib05b_img
  COMMAND mformat -f 1440 -l HARIBOTEOS -N 0xffffffff -C -B ${08_day_harib05b_IPLB} -i ${08_day_harib05b_OS}
  COMMAND mcopy -i ${08_day_harib05b_OS} ${08_day_harib05b_SYS} ::
  DEPENDS 08_day_harib05b_sys
)

########### next target ###############
set(08_day_harib05b_asmhead_SRCS ${root_SOURCE_DIR}/projects/08_day/harib05b/asmhead.nas)
set(08_day_harib05b_asmhead_OUTS ${root_BINARY_DIR}/projects/08_day/harib05b/asmhead.bin)

add_custom_target(08_day_harib05b_asmhead
  COMMAND ${RAKUSK_EXECUTABLE} ${08_day_harib05b_asmhead_SRCS} ${08_day_harib05b_asmhead_OUTS}
  DEPENDS ${08_day_harib05b_asmhead_SRCS}
)

set(08_day_harib05b_asmhead_WINE_OUTS ${root_BINARY_DIR}/projects/08_day/harib05b/asmhead_wine.bin)
add_custom_target(08_day_harib05b_asmhead_wine
  COMMAND ${WINE} ${WINE_NASK} ${08_day_harib05b_asmhead_SRCS} ${08_day_harib05b_asmhead_WINE_OUTS}
)
add_dependencies(wine 08_day_harib05b_asmhead_wine)

add_custom_target(08_day_harib05b_asmhead_od
  COMMAND ${OD} -t x1 ${08_day_harib05b_asmhead_OUTS}      > ${08_day_harib05b_asmhead_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${08_day_harib05b_asmhead_WINE_OUTS} > ${08_day_harib05b_asmhead_WINE_OUTS}_t.txt
  COMMAND diff -s ${08_day_harib05b_asmhead_OUTS}_f.txt ${08_day_harib05b_asmhead_WINE_OUTS}_t.txt
)

add_dependencies(08_day_harib05b_asmhead_od 08_day_harib05b_asmhead)
add_dependencies(08_day_harib05b_asmhead_od 08_day_harib05b_asmhead_wine)
add_dependencies(od 08_day_harib05b_asmhead_od)

#----------------------------------------------------------
########### next target ###############
set(08_day_harib05b_ipl10_SRCS ${root_SOURCE_DIR}/projects/08_day/harib05b/ipl10.nas)
set(08_day_harib05b_ipl10_OUTS ${root_BINARY_DIR}/projects/08_day/harib05b/ipl10.bin)

add_custom_target(08_day_harib05b_ipl10
  COMMAND ${RAKUSK_EXECUTABLE} ${08_day_harib05b_ipl10_SRCS} ${08_day_harib05b_ipl10_OUTS}
  DEPENDS ${08_day_harib05b_ipl10_SRCS}
)

set(08_day_harib05b_ipl10_WINE_OUTS ${root_BINARY_DIR}/projects/08_day/harib05b/ipl10_wine.bin)
add_custom_target(08_day_harib05b_ipl10_wine
  COMMAND ${WINE} ${WINE_NASK} ${08_day_harib05b_ipl10_SRCS} ${08_day_harib05b_ipl10_WINE_OUTS}
)
add_dependencies(wine 08_day_harib05b_ipl10_wine)

add_custom_target(08_day_harib05b_ipl10_od
  COMMAND ${OD} -t x1 ${08_day_harib05b_ipl10_OUTS}      > ${08_day_harib05b_ipl10_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${08_day_harib05b_ipl10_WINE_OUTS} > ${08_day_harib05b_ipl10_WINE_OUTS}_t.txt
  COMMAND diff -s ${08_day_harib05b_ipl10_OUTS}_f.txt ${08_day_harib05b_ipl10_WINE_OUTS}_t.txt
)

add_dependencies(08_day_harib05b_ipl10_od 08_day_harib05b_ipl10)
add_dependencies(08_day_harib05b_ipl10_od 08_day_harib05b_ipl10_wine)
add_dependencies(od 08_day_harib05b_ipl10_od)

#----------------------------------------------------------
########### next target ###############
set(08_day_harib05b_naskfunc_SRCS ${root_SOURCE_DIR}/projects/08_day/harib05b/naskfunc.nas)
set(08_day_harib05b_naskfunc_OUTS ${root_BINARY_DIR}/projects/08_day/harib05b/naskfunc.img)

add_custom_target(08_day_harib05b_naskfunc
  COMMAND ${RAKUSK_EXECUTABLE} ${08_day_harib05b_naskfunc_SRCS} ${08_day_harib05b_naskfunc_OUTS}
  DEPENDS ${08_day_harib05b_naskfunc_SRCS}
)

add_dependencies(images 08_day_harib05b_naskfunc)
set(08_day_harib05b_naskfunc_WINE_OUTS ${root_BINARY_DIR}/projects/08_day/harib05b/naskfunc_wine.img)
add_custom_target(08_day_harib05b_naskfunc_wine
  COMMAND ${WINE} ${WINE_NASK} ${08_day_harib05b_naskfunc_SRCS} ${08_day_harib05b_naskfunc_WINE_OUTS}
)
add_dependencies(wine 08_day_harib05b_naskfunc_wine)

add_custom_target(08_day_harib05b_naskfunc_od
  COMMAND ${OD} -t x1 ${08_day_harib05b_naskfunc_OUTS}      > ${08_day_harib05b_naskfunc_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${08_day_harib05b_naskfunc_WINE_OUTS} > ${08_day_harib05b_naskfunc_WINE_OUTS}_t.txt
  COMMAND diff -s ${08_day_harib05b_naskfunc_OUTS}_f.txt ${08_day_harib05b_naskfunc_WINE_OUTS}_t.txt
)

add_dependencies(08_day_harib05b_naskfunc_od 08_day_harib05b_naskfunc)
add_dependencies(08_day_harib05b_naskfunc_od 08_day_harib05b_naskfunc_wine)
add_dependencies(od 08_day_harib05b_naskfunc_od)

#----------------------------------------------------------
