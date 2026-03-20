#----------------------------------------------------------
message(STATUS "Entering directory projects/16_day/harib13b/")

set(FONT ${root_BINARY_DIR}/src/makefont)
set(B2O  ${root_BINARY_DIR}/src/bin2obj)
set(CONV ${root_BINARY_DIR}/objconv/objconv)
set(16_day_harib13b_WILDOBJ ${root_BINARY_DIR}/projects/16_day/harib13b/*.o)

# Full OS build rules
set(16_day_harib13b_OS    ${root_BINARY_DIR}/projects/16_day/harib13b/os.img)
set(16_day_harib13b_SYS   ${root_BINARY_DIR}/projects/16_day/harib13b/os.sys)
set(16_day_harib13b_IPLB  ${root_BINARY_DIR}/projects/16_day/harib13b/ipl.bin)
set(16_day_harib13b_IPLS  ${root_SOURCE_DIR}/projects/16_day/harib13b/ipl10.nas)
set(16_day_harib13b_HEADB ${root_BINARY_DIR}/projects/16_day/harib13b/asmhead.bin)
set(16_day_harib13b_HEADS ${root_SOURCE_DIR}/projects/16_day/harib13b/asmhead.nas)
set(16_day_harib13b_BOOTB ${root_BINARY_DIR}/projects/16_day/harib13b/boot.bin)
set(16_day_harib13b_CCO   ${root_BINARY_DIR}/projects/16_day/harib13b/boot.o)
set(16_day_harib13b_CCS   ${root_SOURCE_DIR}/projects/16_day/harib13b/*.c)
set(16_day_harib13b_LDS   ${root_SOURCE_DIR}/projects/os.lds)
set(16_day_harib13b_FUNCS ${root_SOURCE_DIR}/projects/16_day/harib13b/naskfunc.nas)
set(16_day_harib13b_FUNCO ${root_BINARY_DIR}/projects/16_day/harib13b/naskfunc.o)
set(16_day_harib13b_FONTS ${root_SOURCE_DIR}/projects/16_day/harib13b/hankaku.txt)
set(16_day_harib13b_FONTB ${root_BINARY_DIR}/projects/16_day/harib13b/hankaku.bin)
set(16_day_harib13b_FONTO ${root_BINARY_DIR}/projects/16_day/harib13b/hankaku.o)
set(16_day_harib13b_LIBGE ${root_BINARY_DIR}/golibc/libgo.a)
set(16_day_harib13b_LIBGC ${root_BINARY_DIR}/projects/16_day/harib13b/libgo.o)

add_custom_target(16_day_harib13b_run
  COMMAND ${QEMU} ${QEMUOPT} ${16_day_harib13b_OS}
  DEPENDS 16_day_harib13b_img
)
add_custom_target(16_day_harib13b_debug
  COMMAND ${QEMU} -s -S ${QEMUOPT} ${16_day_harib13b_OS} ${QEMU_DEBUG_OPT}
  DEPENDS 16_day_harib13b_img
)
add_custom_target(16_day_harib13b_clean
  COMMAND rm -f ${16_day_harib13b_OS}
  COMMAND rm -f ${16_day_harib13b_SYS}
  COMMAND rm -f ${16_day_harib13b_IPLB}
  COMMAND rm -f ${16_day_harib13b_HEADB}
  COMMAND rm -f ${16_day_harib13b_BOOTB}
  COMMAND rm -f ${16_day_harib13b_WILDOBJ}
  COMMAND rm -f ${root_BINARY_DIR}/projects/16_day/harib13b/ipl10.bin
  COMMAND rm -f ${root_BINARY_DIR}/projects/16_day/harib13b/asmhead.bin
  COMMAND rm -f ${root_BINARY_DIR}/projects/16_day/harib13b/*_wine.bin
  COMMAND rm -f ${root_BINARY_DIR}/projects/16_day/harib13b/*.txt
)
add_custom_target(16_day_harib13b_ipl_build
  COMMAND ${RAKUSK_EXECUTABLE} ${16_day_harib13b_IPLS} ${16_day_harib13b_IPLB}
  DEPENDS ${16_day_harib13b_IPLS}
)
add_custom_command(
  OUTPUT ${16_day_harib13b_HEADB}
  COMMAND ${RAKUSK_EXECUTABLE} ${16_day_harib13b_HEADS} ${16_day_harib13b_HEADB}
  DEPENDS ${16_day_harib13b_HEADS}
  COMMENT "Generating asmhead.bin for 16_day_harib13b"
)

add_custom_command(
  OUTPUT ${16_day_harib13b_FUNCO}
  COMMAND ${RAKUSK_EXECUTABLE} ${16_day_harib13b_FUNCS} ${16_day_harib13b_FUNCO}
  DEPENDS ${16_day_harib13b_FUNCS}
  COMMENT "Generating naskfunc.o for 16_day_harib13b"
)
add_custom_command(
  OUTPUT ${16_day_harib13b_FONTO}
  COMMAND ${FONT} ${16_day_harib13b_FONTS} ${16_day_harib13b_FONTB}
  COMMAND ${B2O}  ${16_day_harib13b_FONTB} ${16_day_harib13b_FONTO} _hankaku
  DEPENDS ${16_day_harib13b_FONTS}
  COMMENT "Generating hankaku.o for 16_day_harib13b"
)
add_custom_command(
  OUTPUT ${16_day_harib13b_LIBGC}
  COMMAND ${CONV} -fcoff32 -nu ${16_day_harib13b_LIBGE} ${16_day_harib13b_LIBGC}
  DEPENDS ${16_day_harib13b_LIBGE}
  COMMENT "Converting golibc for 16_day_harib13b"
)
add_custom_target(16_day_harib13b_sys
  COMMAND gcc ${BINOPT} -T ${16_day_harib13b_LDS} ${16_day_harib13b_CCS} ${16_day_harib13b_WILDOBJ} -o ${16_day_harib13b_BOOTB}
  COMMAND cat ${16_day_harib13b_HEADB} ${16_day_harib13b_BOOTB} > ${16_day_harib13b_SYS}
  DEPENDS ${16_day_harib13b_HEADB} 16_day_harib13b_ipl_build
  DEPENDS ${16_day_harib13b_FUNCO}
  DEPENDS ${16_day_harib13b_FONTO} ${16_day_harib13b_LIBGC}
)
add_custom_target(16_day_harib13b_img
  COMMAND mformat -f 1440 -l HARIBOTEOS -N 0xffffffff -C -B ${16_day_harib13b_IPLB} -i ${16_day_harib13b_OS}
  COMMAND mcopy -i ${16_day_harib13b_OS} ${16_day_harib13b_SYS} ::
  DEPENDS 16_day_harib13b_sys
)

########### next target ###############
set(16_day_harib13b_asmhead_SRCS ${root_SOURCE_DIR}/projects/16_day/harib13b/asmhead.nas)
set(16_day_harib13b_asmhead_OUTS ${root_BINARY_DIR}/projects/16_day/harib13b/asmhead.bin)

add_custom_target(16_day_harib13b_asmhead
  COMMAND ${RAKUSK_EXECUTABLE} ${16_day_harib13b_asmhead_SRCS} ${16_day_harib13b_asmhead_OUTS}
  DEPENDS ${16_day_harib13b_asmhead_SRCS}
)

set(16_day_harib13b_asmhead_WINE_OUTS ${root_BINARY_DIR}/projects/16_day/harib13b/asmhead_wine.bin)
add_custom_target(16_day_harib13b_asmhead_wine
  COMMAND ${WINE} ${WINE_NASK} ${16_day_harib13b_asmhead_SRCS} ${16_day_harib13b_asmhead_WINE_OUTS}
)
add_dependencies(wine 16_day_harib13b_asmhead_wine)

add_custom_target(16_day_harib13b_asmhead_od
  COMMAND ${OD} -t x1 ${16_day_harib13b_asmhead_OUTS}      > ${16_day_harib13b_asmhead_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${16_day_harib13b_asmhead_WINE_OUTS} > ${16_day_harib13b_asmhead_WINE_OUTS}_t.txt
  COMMAND diff -s ${16_day_harib13b_asmhead_OUTS}_f.txt ${16_day_harib13b_asmhead_WINE_OUTS}_t.txt
)

add_dependencies(16_day_harib13b_asmhead_od 16_day_harib13b_asmhead)
add_dependencies(16_day_harib13b_asmhead_od 16_day_harib13b_asmhead_wine)
add_dependencies(od 16_day_harib13b_asmhead_od)

#----------------------------------------------------------
########### next target ###############
set(16_day_harib13b_ipl10_SRCS ${root_SOURCE_DIR}/projects/16_day/harib13b/ipl10.nas)
set(16_day_harib13b_ipl10_OUTS ${root_BINARY_DIR}/projects/16_day/harib13b/ipl10.bin)

add_custom_target(16_day_harib13b_ipl10
  COMMAND ${RAKUSK_EXECUTABLE} ${16_day_harib13b_ipl10_SRCS} ${16_day_harib13b_ipl10_OUTS}
  DEPENDS ${16_day_harib13b_ipl10_SRCS}
)

set(16_day_harib13b_ipl10_WINE_OUTS ${root_BINARY_DIR}/projects/16_day/harib13b/ipl10_wine.bin)
add_custom_target(16_day_harib13b_ipl10_wine
  COMMAND ${WINE} ${WINE_NASK} ${16_day_harib13b_ipl10_SRCS} ${16_day_harib13b_ipl10_WINE_OUTS}
)
add_dependencies(wine 16_day_harib13b_ipl10_wine)

add_custom_target(16_day_harib13b_ipl10_od
  COMMAND ${OD} -t x1 ${16_day_harib13b_ipl10_OUTS}      > ${16_day_harib13b_ipl10_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${16_day_harib13b_ipl10_WINE_OUTS} > ${16_day_harib13b_ipl10_WINE_OUTS}_t.txt
  COMMAND diff -s ${16_day_harib13b_ipl10_OUTS}_f.txt ${16_day_harib13b_ipl10_WINE_OUTS}_t.txt
)

add_dependencies(16_day_harib13b_ipl10_od 16_day_harib13b_ipl10)
add_dependencies(16_day_harib13b_ipl10_od 16_day_harib13b_ipl10_wine)
add_dependencies(od 16_day_harib13b_ipl10_od)

#----------------------------------------------------------
########### next target ###############
set(16_day_harib13b_naskfunc_SRCS ${root_SOURCE_DIR}/projects/16_day/harib13b/naskfunc.nas)
set(16_day_harib13b_naskfunc_OUTS ${root_BINARY_DIR}/projects/16_day/harib13b/naskfunc.img)

add_custom_target(16_day_harib13b_naskfunc
  COMMAND ${RAKUSK_EXECUTABLE} ${16_day_harib13b_naskfunc_SRCS} ${16_day_harib13b_naskfunc_OUTS}
  DEPENDS ${16_day_harib13b_naskfunc_SRCS}
)

add_dependencies(images 16_day_harib13b_naskfunc)
set(16_day_harib13b_naskfunc_WINE_OUTS ${root_BINARY_DIR}/projects/16_day/harib13b/naskfunc_wine.img)
add_custom_target(16_day_harib13b_naskfunc_wine
  COMMAND ${WINE} ${WINE_NASK} ${16_day_harib13b_naskfunc_SRCS} ${16_day_harib13b_naskfunc_WINE_OUTS}
)
add_dependencies(wine 16_day_harib13b_naskfunc_wine)

add_custom_target(16_day_harib13b_naskfunc_od
  COMMAND ${OD} -t x1 ${16_day_harib13b_naskfunc_OUTS}      > ${16_day_harib13b_naskfunc_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${16_day_harib13b_naskfunc_WINE_OUTS} > ${16_day_harib13b_naskfunc_WINE_OUTS}_t.txt
  COMMAND diff -s ${16_day_harib13b_naskfunc_OUTS}_f.txt ${16_day_harib13b_naskfunc_WINE_OUTS}_t.txt
)

add_dependencies(16_day_harib13b_naskfunc_od 16_day_harib13b_naskfunc)
add_dependencies(16_day_harib13b_naskfunc_od 16_day_harib13b_naskfunc_wine)
add_dependencies(od 16_day_harib13b_naskfunc_od)

#----------------------------------------------------------
