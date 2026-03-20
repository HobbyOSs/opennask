#----------------------------------------------------------
message(STATUS "Entering directory projects/13_day/harib10b/")

set(FONT ${root_BINARY_DIR}/src/makefont)
set(B2O  ${root_BINARY_DIR}/src/bin2obj)
set(CONV ${root_BINARY_DIR}/objconv/objconv)
set(13_day_harib10b_WILDOBJ ${root_BINARY_DIR}/projects/13_day/harib10b/*.o)

# Full OS build rules
set(13_day_harib10b_OS    ${root_BINARY_DIR}/projects/13_day/harib10b/os.img)
set(13_day_harib10b_SYS   ${root_BINARY_DIR}/projects/13_day/harib10b/os.sys)
set(13_day_harib10b_IPLB  ${root_BINARY_DIR}/projects/13_day/harib10b/ipl.bin)
set(13_day_harib10b_IPLS  ${root_SOURCE_DIR}/projects/13_day/harib10b/ipl10.nas)
set(13_day_harib10b_HEADB ${root_BINARY_DIR}/projects/13_day/harib10b/asmhead.bin)
set(13_day_harib10b_HEADS ${root_SOURCE_DIR}/projects/13_day/harib10b/asmhead.nas)
set(13_day_harib10b_BOOTB ${root_BINARY_DIR}/projects/13_day/harib10b/boot.bin)
set(13_day_harib10b_CCO   ${root_BINARY_DIR}/projects/13_day/harib10b/boot.o)
set(13_day_harib10b_CCS   ${root_SOURCE_DIR}/projects/13_day/harib10b/*.c)
set(13_day_harib10b_LDS   ${root_SOURCE_DIR}/projects/os.lds)
set(13_day_harib10b_FUNCS ${root_SOURCE_DIR}/projects/13_day/harib10b/naskfunc.nas)
set(13_day_harib10b_FUNCO ${root_BINARY_DIR}/projects/13_day/harib10b/naskfunc.o)
set(13_day_harib10b_FONTS ${root_SOURCE_DIR}/projects/13_day/harib10b/hankaku.txt)
set(13_day_harib10b_FONTB ${root_BINARY_DIR}/projects/13_day/harib10b/hankaku.bin)
set(13_day_harib10b_FONTO ${root_BINARY_DIR}/projects/13_day/harib10b/hankaku.o)
set(13_day_harib10b_LIBGE ${root_BINARY_DIR}/golibc/libgo.a)
set(13_day_harib10b_LIBGC ${root_BINARY_DIR}/projects/13_day/harib10b/libgo.o)

add_custom_target(13_day_harib10b_run
  COMMAND ${QEMU} ${QEMUOPT} ${13_day_harib10b_OS}
  DEPENDS 13_day_harib10b_img
)
add_custom_target(13_day_harib10b_debug
  COMMAND ${QEMU} -s -S ${QEMUOPT} ${13_day_harib10b_OS} ${QEMU_DEBUG_OPT}
  DEPENDS 13_day_harib10b_img
)
add_custom_target(13_day_harib10b_clean
  COMMAND rm -f ${13_day_harib10b_OS}
  COMMAND rm -f ${13_day_harib10b_SYS}
  COMMAND rm -f ${13_day_harib10b_IPLB}
  COMMAND rm -f ${13_day_harib10b_HEADB}
  COMMAND rm -f ${13_day_harib10b_BOOTB}
  COMMAND rm -f ${13_day_harib10b_WILDOBJ}
  COMMAND rm -f ${root_BINARY_DIR}/projects/13_day/harib10b/ipl10.bin
  COMMAND rm -f ${root_BINARY_DIR}/projects/13_day/harib10b/asmhead.bin
  COMMAND rm -f ${root_BINARY_DIR}/projects/13_day/harib10b/*_wine.bin
  COMMAND rm -f ${root_BINARY_DIR}/projects/13_day/harib10b/*.txt
)
add_custom_target(13_day_harib10b_ipl_build
  COMMAND ${RAKUSK_EXECUTABLE} ${13_day_harib10b_IPLS} ${13_day_harib10b_IPLB}
  DEPENDS ${13_day_harib10b_IPLS}
)
add_custom_command(
  OUTPUT ${13_day_harib10b_HEADB}
  COMMAND ${RAKUSK_EXECUTABLE} ${13_day_harib10b_HEADS} ${13_day_harib10b_HEADB}
  DEPENDS ${13_day_harib10b_HEADS}
  COMMENT "Generating asmhead.bin for 13_day_harib10b"
)

add_custom_command(
  OUTPUT ${13_day_harib10b_FUNCO}
  COMMAND ${RAKUSK_EXECUTABLE} ${13_day_harib10b_FUNCS} ${13_day_harib10b_FUNCO}
  DEPENDS ${13_day_harib10b_FUNCS}
  COMMENT "Generating naskfunc.o for 13_day_harib10b"
)
add_custom_command(
  OUTPUT ${13_day_harib10b_FONTO}
  COMMAND ${FONT} ${13_day_harib10b_FONTS} ${13_day_harib10b_FONTB}
  COMMAND ${B2O}  ${13_day_harib10b_FONTB} ${13_day_harib10b_FONTO} _hankaku
  DEPENDS ${13_day_harib10b_FONTS}
  COMMENT "Generating hankaku.o for 13_day_harib10b"
)
add_custom_command(
  OUTPUT ${13_day_harib10b_LIBGC}
  COMMAND ${CONV} -fcoff32 -nu ${13_day_harib10b_LIBGE} ${13_day_harib10b_LIBGC}
  DEPENDS ${13_day_harib10b_LIBGE}
  COMMENT "Converting golibc for 13_day_harib10b"
)
add_custom_target(13_day_harib10b_sys
  COMMAND gcc ${BINOPT} -T ${13_day_harib10b_LDS} ${13_day_harib10b_CCS} ${13_day_harib10b_WILDOBJ} -o ${13_day_harib10b_BOOTB}
  COMMAND cat ${13_day_harib10b_HEADB} ${13_day_harib10b_BOOTB} > ${13_day_harib10b_SYS}
  DEPENDS ${13_day_harib10b_HEADB} 13_day_harib10b_ipl_build
  DEPENDS ${13_day_harib10b_FUNCO}
  DEPENDS ${13_day_harib10b_FONTO} ${13_day_harib10b_LIBGC}
)
add_custom_target(13_day_harib10b_img
  COMMAND mformat -f 1440 -l HARIBOTEOS -N 0xffffffff -C -B ${13_day_harib10b_IPLB} -i ${13_day_harib10b_OS}
  COMMAND mcopy -i ${13_day_harib10b_OS} ${13_day_harib10b_SYS} ::
  DEPENDS 13_day_harib10b_sys
)

########### next target ###############
set(13_day_harib10b_asmhead_SRCS ${root_SOURCE_DIR}/projects/13_day/harib10b/asmhead.nas)
set(13_day_harib10b_asmhead_OUTS ${root_BINARY_DIR}/projects/13_day/harib10b/asmhead.bin)

add_custom_target(13_day_harib10b_asmhead
  COMMAND ${RAKUSK_EXECUTABLE} ${13_day_harib10b_asmhead_SRCS} ${13_day_harib10b_asmhead_OUTS}
  DEPENDS ${13_day_harib10b_asmhead_SRCS}
)

set(13_day_harib10b_asmhead_WINE_OUTS ${root_BINARY_DIR}/projects/13_day/harib10b/asmhead_wine.bin)
add_custom_target(13_day_harib10b_asmhead_wine
  COMMAND ${WINE} ${WINE_NASK} ${13_day_harib10b_asmhead_SRCS} ${13_day_harib10b_asmhead_WINE_OUTS}
)
add_dependencies(wine 13_day_harib10b_asmhead_wine)

add_custom_target(13_day_harib10b_asmhead_od
  COMMAND ${OD} -t x1 ${13_day_harib10b_asmhead_OUTS}      > ${13_day_harib10b_asmhead_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${13_day_harib10b_asmhead_WINE_OUTS} > ${13_day_harib10b_asmhead_WINE_OUTS}_t.txt
  COMMAND diff -s ${13_day_harib10b_asmhead_OUTS}_f.txt ${13_day_harib10b_asmhead_WINE_OUTS}_t.txt
)

add_dependencies(13_day_harib10b_asmhead_od 13_day_harib10b_asmhead)
add_dependencies(13_day_harib10b_asmhead_od 13_day_harib10b_asmhead_wine)
add_dependencies(od 13_day_harib10b_asmhead_od)

#----------------------------------------------------------
########### next target ###############
set(13_day_harib10b_ipl10_SRCS ${root_SOURCE_DIR}/projects/13_day/harib10b/ipl10.nas)
set(13_day_harib10b_ipl10_OUTS ${root_BINARY_DIR}/projects/13_day/harib10b/ipl10.bin)

add_custom_target(13_day_harib10b_ipl10
  COMMAND ${RAKUSK_EXECUTABLE} ${13_day_harib10b_ipl10_SRCS} ${13_day_harib10b_ipl10_OUTS}
  DEPENDS ${13_day_harib10b_ipl10_SRCS}
)

set(13_day_harib10b_ipl10_WINE_OUTS ${root_BINARY_DIR}/projects/13_day/harib10b/ipl10_wine.bin)
add_custom_target(13_day_harib10b_ipl10_wine
  COMMAND ${WINE} ${WINE_NASK} ${13_day_harib10b_ipl10_SRCS} ${13_day_harib10b_ipl10_WINE_OUTS}
)
add_dependencies(wine 13_day_harib10b_ipl10_wine)

add_custom_target(13_day_harib10b_ipl10_od
  COMMAND ${OD} -t x1 ${13_day_harib10b_ipl10_OUTS}      > ${13_day_harib10b_ipl10_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${13_day_harib10b_ipl10_WINE_OUTS} > ${13_day_harib10b_ipl10_WINE_OUTS}_t.txt
  COMMAND diff -s ${13_day_harib10b_ipl10_OUTS}_f.txt ${13_day_harib10b_ipl10_WINE_OUTS}_t.txt
)

add_dependencies(13_day_harib10b_ipl10_od 13_day_harib10b_ipl10)
add_dependencies(13_day_harib10b_ipl10_od 13_day_harib10b_ipl10_wine)
add_dependencies(od 13_day_harib10b_ipl10_od)

#----------------------------------------------------------
########### next target ###############
set(13_day_harib10b_naskfunc_SRCS ${root_SOURCE_DIR}/projects/13_day/harib10b/naskfunc.nas)
set(13_day_harib10b_naskfunc_OUTS ${root_BINARY_DIR}/projects/13_day/harib10b/naskfunc.img)

add_custom_target(13_day_harib10b_naskfunc
  COMMAND ${RAKUSK_EXECUTABLE} ${13_day_harib10b_naskfunc_SRCS} ${13_day_harib10b_naskfunc_OUTS}
  DEPENDS ${13_day_harib10b_naskfunc_SRCS}
)

add_dependencies(images 13_day_harib10b_naskfunc)
set(13_day_harib10b_naskfunc_WINE_OUTS ${root_BINARY_DIR}/projects/13_day/harib10b/naskfunc_wine.img)
add_custom_target(13_day_harib10b_naskfunc_wine
  COMMAND ${WINE} ${WINE_NASK} ${13_day_harib10b_naskfunc_SRCS} ${13_day_harib10b_naskfunc_WINE_OUTS}
)
add_dependencies(wine 13_day_harib10b_naskfunc_wine)

add_custom_target(13_day_harib10b_naskfunc_od
  COMMAND ${OD} -t x1 ${13_day_harib10b_naskfunc_OUTS}      > ${13_day_harib10b_naskfunc_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${13_day_harib10b_naskfunc_WINE_OUTS} > ${13_day_harib10b_naskfunc_WINE_OUTS}_t.txt
  COMMAND diff -s ${13_day_harib10b_naskfunc_OUTS}_f.txt ${13_day_harib10b_naskfunc_WINE_OUTS}_t.txt
)

add_dependencies(13_day_harib10b_naskfunc_od 13_day_harib10b_naskfunc)
add_dependencies(13_day_harib10b_naskfunc_od 13_day_harib10b_naskfunc_wine)
add_dependencies(od 13_day_harib10b_naskfunc_od)

#----------------------------------------------------------
