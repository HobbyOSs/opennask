#----------------------------------------------------------
message(STATUS "Entering directory projects/18_day/harib15b/")

set(FONT ${root_BINARY_DIR}/src/makefont)
set(B2O  ${root_BINARY_DIR}/src/bin2obj)
set(CONV ${root_BINARY_DIR}/objconv/objconv)
set(18_day_harib15b_WILDOBJ ${root_BINARY_DIR}/projects/18_day/harib15b/*.o)

# Full OS build rules
set(18_day_harib15b_OS    ${root_BINARY_DIR}/projects/18_day/harib15b/os.img)
set(18_day_harib15b_SYS   ${root_BINARY_DIR}/projects/18_day/harib15b/os.sys)
set(18_day_harib15b_IPLB  ${root_BINARY_DIR}/projects/18_day/harib15b/ipl.bin)
set(18_day_harib15b_IPLS  ${root_SOURCE_DIR}/projects/18_day/harib15b/ipl10.nas)
set(18_day_harib15b_HEADB ${root_BINARY_DIR}/projects/18_day/harib15b/asmhead.bin)
set(18_day_harib15b_HEADS ${root_SOURCE_DIR}/projects/18_day/harib15b/asmhead.nas)
set(18_day_harib15b_BOOTB ${root_BINARY_DIR}/projects/18_day/harib15b/boot.bin)
set(18_day_harib15b_CCO   ${root_BINARY_DIR}/projects/18_day/harib15b/boot.o)
set(18_day_harib15b_CCS   ${root_SOURCE_DIR}/projects/18_day/harib15b/*.c)
set(18_day_harib15b_LDS   ${root_SOURCE_DIR}/projects/os.lds)
set(18_day_harib15b_FUNCS ${root_SOURCE_DIR}/projects/18_day/harib15b/naskfunc.nas)
set(18_day_harib15b_FUNCO ${root_BINARY_DIR}/projects/18_day/harib15b/naskfunc.o)
set(18_day_harib15b_FONTS ${root_SOURCE_DIR}/projects/18_day/harib15b/hankaku.txt)
set(18_day_harib15b_FONTB ${root_BINARY_DIR}/projects/18_day/harib15b/hankaku.bin)
set(18_day_harib15b_FONTO ${root_BINARY_DIR}/projects/18_day/harib15b/hankaku.o)
set(18_day_harib15b_LIBGE ${root_BINARY_DIR}/golibc/libgo.a)
set(18_day_harib15b_LIBGC ${root_BINARY_DIR}/projects/18_day/harib15b/libgo.o)

add_custom_target(18_day_harib15b_run
  COMMAND ${QEMU} ${QEMUOPT} ${18_day_harib15b_OS}
  DEPENDS 18_day_harib15b_img
)
add_custom_target(18_day_harib15b_debug
  COMMAND ${QEMU} -s -S ${QEMUOPT} ${18_day_harib15b_OS} ${QEMU_DEBUG_OPT}
  DEPENDS 18_day_harib15b_img
)
add_custom_target(18_day_harib15b_clean
  COMMAND rm -f ${18_day_harib15b_OS}
  COMMAND rm -f ${18_day_harib15b_SYS}
  COMMAND rm -f ${18_day_harib15b_IPLB}
  COMMAND rm -f ${18_day_harib15b_HEADB}
  COMMAND rm -f ${18_day_harib15b_BOOTB}
  COMMAND rm -f ${18_day_harib15b_WILDOBJ}
  COMMAND rm -f ${root_BINARY_DIR}/projects/18_day/harib15b/ipl10.bin
  COMMAND rm -f ${root_BINARY_DIR}/projects/18_day/harib15b/asmhead.bin
  COMMAND rm -f ${root_BINARY_DIR}/projects/18_day/harib15b/*_wine.bin
  COMMAND rm -f ${root_BINARY_DIR}/projects/18_day/harib15b/*.txt
)
add_custom_target(18_day_harib15b_ipl_build
  COMMAND ${RAKUSK_EXECUTABLE} ${18_day_harib15b_IPLS} ${18_day_harib15b_IPLB}
  DEPENDS ${18_day_harib15b_IPLS}
)
add_custom_command(
  OUTPUT ${18_day_harib15b_HEADB}
  COMMAND ${RAKUSK_EXECUTABLE} ${18_day_harib15b_HEADS} ${18_day_harib15b_HEADB}
  DEPENDS ${18_day_harib15b_HEADS}
  COMMENT "Generating asmhead.bin for 18_day_harib15b"
)

add_custom_command(
  OUTPUT ${18_day_harib15b_FUNCO}
  COMMAND ${RAKUSK_EXECUTABLE} ${18_day_harib15b_FUNCS} ${18_day_harib15b_FUNCO}
  DEPENDS ${18_day_harib15b_FUNCS}
  COMMENT "Generating naskfunc.o for 18_day_harib15b"
)
add_custom_command(
  OUTPUT ${18_day_harib15b_FONTO}
  COMMAND ${FONT} ${18_day_harib15b_FONTS} ${18_day_harib15b_FONTB}
  COMMAND ${B2O}  ${18_day_harib15b_FONTB} ${18_day_harib15b_FONTO} _hankaku
  DEPENDS ${18_day_harib15b_FONTS}
  COMMENT "Generating hankaku.o for 18_day_harib15b"
)
add_custom_command(
  OUTPUT ${18_day_harib15b_LIBGC}
  COMMAND ${CONV} -fcoff32 -nu ${18_day_harib15b_LIBGE} ${18_day_harib15b_LIBGC}
  DEPENDS ${18_day_harib15b_LIBGE}
  COMMENT "Converting golibc for 18_day_harib15b"
)
add_custom_target(18_day_harib15b_sys
  COMMAND gcc ${BINOPT} -T ${18_day_harib15b_LDS} ${18_day_harib15b_CCS} ${18_day_harib15b_WILDOBJ} -o ${18_day_harib15b_BOOTB}
  COMMAND cat ${18_day_harib15b_HEADB} ${18_day_harib15b_BOOTB} > ${18_day_harib15b_SYS}
  DEPENDS ${18_day_harib15b_HEADB} 18_day_harib15b_ipl_build
  DEPENDS ${18_day_harib15b_FUNCO}
  DEPENDS ${18_day_harib15b_FONTO} ${18_day_harib15b_LIBGC}
)
add_custom_target(18_day_harib15b_img
  COMMAND mformat -f 1440 -l HARIBOTEOS -N 0xffffffff -C -B ${18_day_harib15b_IPLB} -i ${18_day_harib15b_OS}
  COMMAND mcopy -i ${18_day_harib15b_OS} ${18_day_harib15b_SYS} ::
  DEPENDS 18_day_harib15b_sys
)

########### next target ###############
set(18_day_harib15b_asmhead_SRCS ${root_SOURCE_DIR}/projects/18_day/harib15b/asmhead.nas)
set(18_day_harib15b_asmhead_OUTS ${root_BINARY_DIR}/projects/18_day/harib15b/asmhead.bin)

add_custom_target(18_day_harib15b_asmhead
  COMMAND ${RAKUSK_EXECUTABLE} ${18_day_harib15b_asmhead_SRCS} ${18_day_harib15b_asmhead_OUTS}
  DEPENDS ${18_day_harib15b_asmhead_SRCS}
)

set(18_day_harib15b_asmhead_WINE_OUTS ${root_BINARY_DIR}/projects/18_day/harib15b/asmhead_wine.bin)
add_custom_target(18_day_harib15b_asmhead_wine
  COMMAND ${WINE} ${WINE_NASK} ${18_day_harib15b_asmhead_SRCS} ${18_day_harib15b_asmhead_WINE_OUTS}
)
add_dependencies(wine 18_day_harib15b_asmhead_wine)

add_custom_target(18_day_harib15b_asmhead_od
  COMMAND ${OD} -t x1 ${18_day_harib15b_asmhead_OUTS}      > ${18_day_harib15b_asmhead_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${18_day_harib15b_asmhead_WINE_OUTS} > ${18_day_harib15b_asmhead_WINE_OUTS}_t.txt
  COMMAND diff -s ${18_day_harib15b_asmhead_OUTS}_f.txt ${18_day_harib15b_asmhead_WINE_OUTS}_t.txt
)

add_dependencies(18_day_harib15b_asmhead_od 18_day_harib15b_asmhead)
add_dependencies(18_day_harib15b_asmhead_od 18_day_harib15b_asmhead_wine)
add_dependencies(od 18_day_harib15b_asmhead_od)

#----------------------------------------------------------
########### next target ###############
set(18_day_harib15b_ipl10_SRCS ${root_SOURCE_DIR}/projects/18_day/harib15b/ipl10.nas)
set(18_day_harib15b_ipl10_OUTS ${root_BINARY_DIR}/projects/18_day/harib15b/ipl10.bin)

add_custom_target(18_day_harib15b_ipl10
  COMMAND ${RAKUSK_EXECUTABLE} ${18_day_harib15b_ipl10_SRCS} ${18_day_harib15b_ipl10_OUTS}
  DEPENDS ${18_day_harib15b_ipl10_SRCS}
)

set(18_day_harib15b_ipl10_WINE_OUTS ${root_BINARY_DIR}/projects/18_day/harib15b/ipl10_wine.bin)
add_custom_target(18_day_harib15b_ipl10_wine
  COMMAND ${WINE} ${WINE_NASK} ${18_day_harib15b_ipl10_SRCS} ${18_day_harib15b_ipl10_WINE_OUTS}
)
add_dependencies(wine 18_day_harib15b_ipl10_wine)

add_custom_target(18_day_harib15b_ipl10_od
  COMMAND ${OD} -t x1 ${18_day_harib15b_ipl10_OUTS}      > ${18_day_harib15b_ipl10_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${18_day_harib15b_ipl10_WINE_OUTS} > ${18_day_harib15b_ipl10_WINE_OUTS}_t.txt
  COMMAND diff -s ${18_day_harib15b_ipl10_OUTS}_f.txt ${18_day_harib15b_ipl10_WINE_OUTS}_t.txt
)

add_dependencies(18_day_harib15b_ipl10_od 18_day_harib15b_ipl10)
add_dependencies(18_day_harib15b_ipl10_od 18_day_harib15b_ipl10_wine)
add_dependencies(od 18_day_harib15b_ipl10_od)

#----------------------------------------------------------
########### next target ###############
set(18_day_harib15b_naskfunc_SRCS ${root_SOURCE_DIR}/projects/18_day/harib15b/naskfunc.nas)
set(18_day_harib15b_naskfunc_OUTS ${root_BINARY_DIR}/projects/18_day/harib15b/naskfunc.img)

add_custom_target(18_day_harib15b_naskfunc
  COMMAND ${RAKUSK_EXECUTABLE} ${18_day_harib15b_naskfunc_SRCS} ${18_day_harib15b_naskfunc_OUTS}
  DEPENDS ${18_day_harib15b_naskfunc_SRCS}
)

add_dependencies(images 18_day_harib15b_naskfunc)
set(18_day_harib15b_naskfunc_WINE_OUTS ${root_BINARY_DIR}/projects/18_day/harib15b/naskfunc_wine.img)
add_custom_target(18_day_harib15b_naskfunc_wine
  COMMAND ${WINE} ${WINE_NASK} ${18_day_harib15b_naskfunc_SRCS} ${18_day_harib15b_naskfunc_WINE_OUTS}
)
add_dependencies(wine 18_day_harib15b_naskfunc_wine)

add_custom_target(18_day_harib15b_naskfunc_od
  COMMAND ${OD} -t x1 ${18_day_harib15b_naskfunc_OUTS}      > ${18_day_harib15b_naskfunc_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${18_day_harib15b_naskfunc_WINE_OUTS} > ${18_day_harib15b_naskfunc_WINE_OUTS}_t.txt
  COMMAND diff -s ${18_day_harib15b_naskfunc_OUTS}_f.txt ${18_day_harib15b_naskfunc_WINE_OUTS}_t.txt
)

add_dependencies(18_day_harib15b_naskfunc_od 18_day_harib15b_naskfunc)
add_dependencies(18_day_harib15b_naskfunc_od 18_day_harib15b_naskfunc_wine)
add_dependencies(od 18_day_harib15b_naskfunc_od)

#----------------------------------------------------------
