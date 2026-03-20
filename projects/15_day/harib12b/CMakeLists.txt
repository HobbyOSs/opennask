#----------------------------------------------------------
message(STATUS "Entering directory projects/15_day/harib12b/")

set(FONT ${root_BINARY_DIR}/src/makefont)
set(B2O  ${root_BINARY_DIR}/src/bin2obj)
set(CONV ${root_BINARY_DIR}/objconv/objconv)
set(15_day_harib12b_WILDOBJ ${root_BINARY_DIR}/projects/15_day/harib12b/*.o)

# Full OS build rules
set(15_day_harib12b_OS    ${root_BINARY_DIR}/projects/15_day/harib12b/os.img)
set(15_day_harib12b_SYS   ${root_BINARY_DIR}/projects/15_day/harib12b/os.sys)
set(15_day_harib12b_IPLB  ${root_BINARY_DIR}/projects/15_day/harib12b/ipl.bin)
set(15_day_harib12b_IPLS  ${root_SOURCE_DIR}/projects/15_day/harib12b/ipl10.nas)
set(15_day_harib12b_HEADB ${root_BINARY_DIR}/projects/15_day/harib12b/asmhead.bin)
set(15_day_harib12b_HEADS ${root_SOURCE_DIR}/projects/15_day/harib12b/asmhead.nas)
set(15_day_harib12b_BOOTB ${root_BINARY_DIR}/projects/15_day/harib12b/boot.bin)
set(15_day_harib12b_CCO   ${root_BINARY_DIR}/projects/15_day/harib12b/boot.o)
set(15_day_harib12b_CCS   ${root_SOURCE_DIR}/projects/15_day/harib12b/*.c)
set(15_day_harib12b_LDS   ${root_SOURCE_DIR}/projects/os.lds)
set(15_day_harib12b_FUNCS ${root_SOURCE_DIR}/projects/15_day/harib12b/naskfunc.nas)
set(15_day_harib12b_FUNCO ${root_BINARY_DIR}/projects/15_day/harib12b/naskfunc.o)
set(15_day_harib12b_FONTS ${root_SOURCE_DIR}/projects/15_day/harib12b/hankaku.txt)
set(15_day_harib12b_FONTB ${root_BINARY_DIR}/projects/15_day/harib12b/hankaku.bin)
set(15_day_harib12b_FONTO ${root_BINARY_DIR}/projects/15_day/harib12b/hankaku.o)
set(15_day_harib12b_LIBGE ${root_BINARY_DIR}/golibc/libgo.a)
set(15_day_harib12b_LIBGC ${root_BINARY_DIR}/projects/15_day/harib12b/libgo.o)

add_custom_target(15_day_harib12b_run
  COMMAND ${QEMU} ${QEMUOPT} ${15_day_harib12b_OS}
  DEPENDS 15_day_harib12b_img
)
add_custom_target(15_day_harib12b_debug
  COMMAND ${QEMU} -s -S ${QEMUOPT} ${15_day_harib12b_OS} ${QEMU_DEBUG_OPT}
  DEPENDS 15_day_harib12b_img
)
add_custom_target(15_day_harib12b_clean
  COMMAND rm -f ${15_day_harib12b_OS}
  COMMAND rm -f ${15_day_harib12b_SYS}
  COMMAND rm -f ${15_day_harib12b_IPLB}
  COMMAND rm -f ${15_day_harib12b_HEADB}
  COMMAND rm -f ${15_day_harib12b_BOOTB}
  COMMAND rm -f ${15_day_harib12b_WILDOBJ}
  COMMAND rm -f ${root_BINARY_DIR}/projects/15_day/harib12b/ipl10.bin
  COMMAND rm -f ${root_BINARY_DIR}/projects/15_day/harib12b/asmhead.bin
  COMMAND rm -f ${root_BINARY_DIR}/projects/15_day/harib12b/*_wine.bin
  COMMAND rm -f ${root_BINARY_DIR}/projects/15_day/harib12b/*.txt
)
add_custom_target(15_day_harib12b_ipl_build
  COMMAND ${RAKUSK_EXECUTABLE} ${15_day_harib12b_IPLS} ${15_day_harib12b_IPLB}
  DEPENDS ${15_day_harib12b_IPLS}
)
add_custom_command(
  OUTPUT ${15_day_harib12b_HEADB}
  COMMAND ${RAKUSK_EXECUTABLE} ${15_day_harib12b_HEADS} ${15_day_harib12b_HEADB}
  DEPENDS ${15_day_harib12b_HEADS}
  COMMENT "Generating asmhead.bin for 15_day_harib12b"
)

add_custom_command(
  OUTPUT ${15_day_harib12b_FUNCO}
  COMMAND ${RAKUSK_EXECUTABLE} ${15_day_harib12b_FUNCS} ${15_day_harib12b_FUNCO}
  DEPENDS ${15_day_harib12b_FUNCS}
  COMMENT "Generating naskfunc.o for 15_day_harib12b"
)
add_custom_command(
  OUTPUT ${15_day_harib12b_FONTO}
  COMMAND ${FONT} ${15_day_harib12b_FONTS} ${15_day_harib12b_FONTB}
  COMMAND ${B2O}  ${15_day_harib12b_FONTB} ${15_day_harib12b_FONTO} _hankaku
  DEPENDS ${15_day_harib12b_FONTS}
  COMMENT "Generating hankaku.o for 15_day_harib12b"
)
add_custom_command(
  OUTPUT ${15_day_harib12b_LIBGC}
  COMMAND ${CONV} -fcoff32 -nu ${15_day_harib12b_LIBGE} ${15_day_harib12b_LIBGC}
  DEPENDS ${15_day_harib12b_LIBGE}
  COMMENT "Converting golibc for 15_day_harib12b"
)
add_custom_target(15_day_harib12b_sys
  COMMAND gcc ${BINOPT} -T ${15_day_harib12b_LDS} ${15_day_harib12b_CCS} ${15_day_harib12b_WILDOBJ} -o ${15_day_harib12b_BOOTB}
  COMMAND cat ${15_day_harib12b_HEADB} ${15_day_harib12b_BOOTB} > ${15_day_harib12b_SYS}
  DEPENDS ${15_day_harib12b_HEADB} 15_day_harib12b_ipl_build
  DEPENDS ${15_day_harib12b_FUNCO}
  DEPENDS ${15_day_harib12b_FONTO} ${15_day_harib12b_LIBGC}
)
add_custom_target(15_day_harib12b_img
  COMMAND mformat -f 1440 -l HARIBOTEOS -N 0xffffffff -C -B ${15_day_harib12b_IPLB} -i ${15_day_harib12b_OS}
  COMMAND mcopy -i ${15_day_harib12b_OS} ${15_day_harib12b_SYS} ::
  DEPENDS 15_day_harib12b_sys
)

########### next target ###############
set(15_day_harib12b_asmhead_SRCS ${root_SOURCE_DIR}/projects/15_day/harib12b/asmhead.nas)
set(15_day_harib12b_asmhead_OUTS ${root_BINARY_DIR}/projects/15_day/harib12b/asmhead.bin)

add_custom_target(15_day_harib12b_asmhead
  COMMAND ${RAKUSK_EXECUTABLE} ${15_day_harib12b_asmhead_SRCS} ${15_day_harib12b_asmhead_OUTS}
  DEPENDS ${15_day_harib12b_asmhead_SRCS}
)

set(15_day_harib12b_asmhead_WINE_OUTS ${root_BINARY_DIR}/projects/15_day/harib12b/asmhead_wine.bin)
add_custom_target(15_day_harib12b_asmhead_wine
  COMMAND ${WINE} ${WINE_NASK} ${15_day_harib12b_asmhead_SRCS} ${15_day_harib12b_asmhead_WINE_OUTS}
)
add_dependencies(wine 15_day_harib12b_asmhead_wine)

add_custom_target(15_day_harib12b_asmhead_od
  COMMAND ${OD} -t x1 ${15_day_harib12b_asmhead_OUTS}      > ${15_day_harib12b_asmhead_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${15_day_harib12b_asmhead_WINE_OUTS} > ${15_day_harib12b_asmhead_WINE_OUTS}_t.txt
  COMMAND diff -s ${15_day_harib12b_asmhead_OUTS}_f.txt ${15_day_harib12b_asmhead_WINE_OUTS}_t.txt
)

add_dependencies(15_day_harib12b_asmhead_od 15_day_harib12b_asmhead)
add_dependencies(15_day_harib12b_asmhead_od 15_day_harib12b_asmhead_wine)
add_dependencies(od 15_day_harib12b_asmhead_od)

#----------------------------------------------------------
########### next target ###############
set(15_day_harib12b_ipl10_SRCS ${root_SOURCE_DIR}/projects/15_day/harib12b/ipl10.nas)
set(15_day_harib12b_ipl10_OUTS ${root_BINARY_DIR}/projects/15_day/harib12b/ipl10.bin)

add_custom_target(15_day_harib12b_ipl10
  COMMAND ${RAKUSK_EXECUTABLE} ${15_day_harib12b_ipl10_SRCS} ${15_day_harib12b_ipl10_OUTS}
  DEPENDS ${15_day_harib12b_ipl10_SRCS}
)

set(15_day_harib12b_ipl10_WINE_OUTS ${root_BINARY_DIR}/projects/15_day/harib12b/ipl10_wine.bin)
add_custom_target(15_day_harib12b_ipl10_wine
  COMMAND ${WINE} ${WINE_NASK} ${15_day_harib12b_ipl10_SRCS} ${15_day_harib12b_ipl10_WINE_OUTS}
)
add_dependencies(wine 15_day_harib12b_ipl10_wine)

add_custom_target(15_day_harib12b_ipl10_od
  COMMAND ${OD} -t x1 ${15_day_harib12b_ipl10_OUTS}      > ${15_day_harib12b_ipl10_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${15_day_harib12b_ipl10_WINE_OUTS} > ${15_day_harib12b_ipl10_WINE_OUTS}_t.txt
  COMMAND diff -s ${15_day_harib12b_ipl10_OUTS}_f.txt ${15_day_harib12b_ipl10_WINE_OUTS}_t.txt
)

add_dependencies(15_day_harib12b_ipl10_od 15_day_harib12b_ipl10)
add_dependencies(15_day_harib12b_ipl10_od 15_day_harib12b_ipl10_wine)
add_dependencies(od 15_day_harib12b_ipl10_od)

#----------------------------------------------------------
########### next target ###############
set(15_day_harib12b_naskfunc_SRCS ${root_SOURCE_DIR}/projects/15_day/harib12b/naskfunc.nas)
set(15_day_harib12b_naskfunc_OUTS ${root_BINARY_DIR}/projects/15_day/harib12b/naskfunc.img)

add_custom_target(15_day_harib12b_naskfunc
  COMMAND ${RAKUSK_EXECUTABLE} ${15_day_harib12b_naskfunc_SRCS} ${15_day_harib12b_naskfunc_OUTS}
  DEPENDS ${15_day_harib12b_naskfunc_SRCS}
)

add_dependencies(images 15_day_harib12b_naskfunc)
set(15_day_harib12b_naskfunc_WINE_OUTS ${root_BINARY_DIR}/projects/15_day/harib12b/naskfunc_wine.img)
add_custom_target(15_day_harib12b_naskfunc_wine
  COMMAND ${WINE} ${WINE_NASK} ${15_day_harib12b_naskfunc_SRCS} ${15_day_harib12b_naskfunc_WINE_OUTS}
)
add_dependencies(wine 15_day_harib12b_naskfunc_wine)

add_custom_target(15_day_harib12b_naskfunc_od
  COMMAND ${OD} -t x1 ${15_day_harib12b_naskfunc_OUTS}      > ${15_day_harib12b_naskfunc_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${15_day_harib12b_naskfunc_WINE_OUTS} > ${15_day_harib12b_naskfunc_WINE_OUTS}_t.txt
  COMMAND diff -s ${15_day_harib12b_naskfunc_OUTS}_f.txt ${15_day_harib12b_naskfunc_WINE_OUTS}_t.txt
)

add_dependencies(15_day_harib12b_naskfunc_od 15_day_harib12b_naskfunc)
add_dependencies(15_day_harib12b_naskfunc_od 15_day_harib12b_naskfunc_wine)
add_dependencies(od 15_day_harib12b_naskfunc_od)

#----------------------------------------------------------
