#----------------------------------------------------------
message(STATUS "Entering directory projects/15_day/harib12a/")

set(FONT ${root_BINARY_DIR}/src/makefont)
set(B2O  ${root_BINARY_DIR}/src/bin2obj)
set(CONV ${root_BINARY_DIR}/objconv/objconv)
set(15_day_harib12a_WILDOBJ ${root_BINARY_DIR}/projects/15_day/harib12a/*.o)

# Full OS build rules
set(15_day_harib12a_OS    ${root_BINARY_DIR}/projects/15_day/harib12a/os.img)
set(15_day_harib12a_SYS   ${root_BINARY_DIR}/projects/15_day/harib12a/os.sys)
set(15_day_harib12a_IPLB  ${root_BINARY_DIR}/projects/15_day/harib12a/ipl.bin)
set(15_day_harib12a_IPLS  ${root_SOURCE_DIR}/projects/15_day/harib12a/ipl10.nas)
set(15_day_harib12a_HEADB ${root_BINARY_DIR}/projects/15_day/harib12a/asmhead.bin)
set(15_day_harib12a_HEADS ${root_SOURCE_DIR}/projects/15_day/harib12a/asmhead.nas)
set(15_day_harib12a_BOOTB ${root_BINARY_DIR}/projects/15_day/harib12a/boot.bin)
set(15_day_harib12a_CCO   ${root_BINARY_DIR}/projects/15_day/harib12a/boot.o)
set(15_day_harib12a_CCS   ${root_SOURCE_DIR}/projects/15_day/harib12a/*.c)
set(15_day_harib12a_LDS   ${root_SOURCE_DIR}/projects/os.lds)
set(15_day_harib12a_FUNCS ${root_SOURCE_DIR}/projects/15_day/harib12a/naskfunc.nas)
set(15_day_harib12a_FUNCO ${root_BINARY_DIR}/projects/15_day/harib12a/naskfunc.o)
set(15_day_harib12a_FONTS ${root_SOURCE_DIR}/projects/15_day/harib12a/hankaku.txt)
set(15_day_harib12a_FONTB ${root_BINARY_DIR}/projects/15_day/harib12a/hankaku.bin)
set(15_day_harib12a_FONTO ${root_BINARY_DIR}/projects/15_day/harib12a/hankaku.o)
set(15_day_harib12a_LIBGE ${root_BINARY_DIR}/golibc/libgo.a)
set(15_day_harib12a_LIBGC ${root_BINARY_DIR}/projects/15_day/harib12a/libgo.o)

add_custom_target(15_day_harib12a_run
  COMMAND ${QEMU} ${QEMUOPT} ${15_day_harib12a_OS}
  DEPENDS 15_day_harib12a_img
)
add_custom_target(15_day_harib12a_debug
  COMMAND ${QEMU} -s -S ${QEMUOPT} ${15_day_harib12a_OS} ${QEMU_DEBUG_OPT}
  DEPENDS 15_day_harib12a_img
)
add_custom_target(15_day_harib12a_clean
  COMMAND rm -f ${15_day_harib12a_OS}
  COMMAND rm -f ${15_day_harib12a_SYS}
  COMMAND rm -f ${15_day_harib12a_IPLB}
  COMMAND rm -f ${15_day_harib12a_HEADB}
  COMMAND rm -f ${15_day_harib12a_BOOTB}
  COMMAND rm -f ${15_day_harib12a_WILDOBJ}
  COMMAND rm -f ${root_BINARY_DIR}/projects/15_day/harib12a/ipl10.bin
  COMMAND rm -f ${root_BINARY_DIR}/projects/15_day/harib12a/asmhead.bin
  COMMAND rm -f ${root_BINARY_DIR}/projects/15_day/harib12a/*_wine.bin
  COMMAND rm -f ${root_BINARY_DIR}/projects/15_day/harib12a/*.txt
)
add_custom_target(15_day_harib12a_ipl_build
  COMMAND ${RAKUSK_EXECUTABLE} ${15_day_harib12a_IPLS} ${15_day_harib12a_IPLB}
  DEPENDS ${15_day_harib12a_IPLS}
)
add_custom_command(
  OUTPUT ${15_day_harib12a_HEADB}
  COMMAND ${RAKUSK_EXECUTABLE} ${15_day_harib12a_HEADS} ${15_day_harib12a_HEADB}
  DEPENDS ${15_day_harib12a_HEADS}
  COMMENT "Generating asmhead.bin for 15_day_harib12a"
)

add_custom_command(
  OUTPUT ${15_day_harib12a_FUNCO}
  COMMAND ${RAKUSK_EXECUTABLE} ${15_day_harib12a_FUNCS} ${15_day_harib12a_FUNCO}
  DEPENDS ${15_day_harib12a_FUNCS}
  COMMENT "Generating naskfunc.o for 15_day_harib12a"
)
add_custom_command(
  OUTPUT ${15_day_harib12a_FONTO}
  COMMAND ${FONT} ${15_day_harib12a_FONTS} ${15_day_harib12a_FONTB}
  COMMAND ${B2O}  ${15_day_harib12a_FONTB} ${15_day_harib12a_FONTO} _hankaku
  DEPENDS ${15_day_harib12a_FONTS}
  COMMENT "Generating hankaku.o for 15_day_harib12a"
)
add_custom_command(
  OUTPUT ${15_day_harib12a_LIBGC}
  COMMAND ${CONV} -fcoff32 -nu ${15_day_harib12a_LIBGE} ${15_day_harib12a_LIBGC}
  DEPENDS ${15_day_harib12a_LIBGE}
  COMMENT "Converting golibc for 15_day_harib12a"
)
add_custom_target(15_day_harib12a_sys
  COMMAND gcc ${BINOPT} -T ${15_day_harib12a_LDS} ${15_day_harib12a_CCS} ${15_day_harib12a_WILDOBJ} -o ${15_day_harib12a_BOOTB}
  COMMAND cat ${15_day_harib12a_HEADB} ${15_day_harib12a_BOOTB} > ${15_day_harib12a_SYS}
  DEPENDS ${15_day_harib12a_HEADB} 15_day_harib12a_ipl_build
  DEPENDS ${15_day_harib12a_FUNCO}
  DEPENDS ${15_day_harib12a_FONTO} ${15_day_harib12a_LIBGC}
)
add_custom_target(15_day_harib12a_img
  COMMAND mformat -f 1440 -l HARIBOTEOS -N 0xffffffff -C -B ${15_day_harib12a_IPLB} -i ${15_day_harib12a_OS}
  COMMAND mcopy -i ${15_day_harib12a_OS} ${15_day_harib12a_SYS} ::
  DEPENDS 15_day_harib12a_sys
)

########### next target ###############
set(15_day_harib12a_asmhead_SRCS ${root_SOURCE_DIR}/projects/15_day/harib12a/asmhead.nas)
set(15_day_harib12a_asmhead_OUTS ${root_BINARY_DIR}/projects/15_day/harib12a/asmhead.bin)

add_custom_target(15_day_harib12a_asmhead
  COMMAND ${RAKUSK_EXECUTABLE} ${15_day_harib12a_asmhead_SRCS} ${15_day_harib12a_asmhead_OUTS}
  DEPENDS ${15_day_harib12a_asmhead_SRCS}
)

set(15_day_harib12a_asmhead_WINE_OUTS ${root_BINARY_DIR}/projects/15_day/harib12a/asmhead_wine.bin)
add_custom_target(15_day_harib12a_asmhead_wine
  COMMAND ${WINE} ${WINE_NASK} ${15_day_harib12a_asmhead_SRCS} ${15_day_harib12a_asmhead_WINE_OUTS}
)
add_dependencies(wine 15_day_harib12a_asmhead_wine)

add_custom_target(15_day_harib12a_asmhead_od
  COMMAND ${OD} -t x1 ${15_day_harib12a_asmhead_OUTS}      > ${15_day_harib12a_asmhead_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${15_day_harib12a_asmhead_WINE_OUTS} > ${15_day_harib12a_asmhead_WINE_OUTS}_t.txt
  COMMAND diff -s ${15_day_harib12a_asmhead_OUTS}_f.txt ${15_day_harib12a_asmhead_WINE_OUTS}_t.txt
)

add_dependencies(15_day_harib12a_asmhead_od 15_day_harib12a_asmhead)
add_dependencies(15_day_harib12a_asmhead_od 15_day_harib12a_asmhead_wine)
add_dependencies(od 15_day_harib12a_asmhead_od)

#----------------------------------------------------------
########### next target ###############
set(15_day_harib12a_ipl10_SRCS ${root_SOURCE_DIR}/projects/15_day/harib12a/ipl10.nas)
set(15_day_harib12a_ipl10_OUTS ${root_BINARY_DIR}/projects/15_day/harib12a/ipl10.bin)

add_custom_target(15_day_harib12a_ipl10
  COMMAND ${RAKUSK_EXECUTABLE} ${15_day_harib12a_ipl10_SRCS} ${15_day_harib12a_ipl10_OUTS}
  DEPENDS ${15_day_harib12a_ipl10_SRCS}
)

set(15_day_harib12a_ipl10_WINE_OUTS ${root_BINARY_DIR}/projects/15_day/harib12a/ipl10_wine.bin)
add_custom_target(15_day_harib12a_ipl10_wine
  COMMAND ${WINE} ${WINE_NASK} ${15_day_harib12a_ipl10_SRCS} ${15_day_harib12a_ipl10_WINE_OUTS}
)
add_dependencies(wine 15_day_harib12a_ipl10_wine)

add_custom_target(15_day_harib12a_ipl10_od
  COMMAND ${OD} -t x1 ${15_day_harib12a_ipl10_OUTS}      > ${15_day_harib12a_ipl10_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${15_day_harib12a_ipl10_WINE_OUTS} > ${15_day_harib12a_ipl10_WINE_OUTS}_t.txt
  COMMAND diff -s ${15_day_harib12a_ipl10_OUTS}_f.txt ${15_day_harib12a_ipl10_WINE_OUTS}_t.txt
)

add_dependencies(15_day_harib12a_ipl10_od 15_day_harib12a_ipl10)
add_dependencies(15_day_harib12a_ipl10_od 15_day_harib12a_ipl10_wine)
add_dependencies(od 15_day_harib12a_ipl10_od)

#----------------------------------------------------------
########### next target ###############
set(15_day_harib12a_naskfunc_SRCS ${root_SOURCE_DIR}/projects/15_day/harib12a/naskfunc.nas)
set(15_day_harib12a_naskfunc_OUTS ${root_BINARY_DIR}/projects/15_day/harib12a/naskfunc.img)

add_custom_target(15_day_harib12a_naskfunc
  COMMAND ${RAKUSK_EXECUTABLE} ${15_day_harib12a_naskfunc_SRCS} ${15_day_harib12a_naskfunc_OUTS}
  DEPENDS ${15_day_harib12a_naskfunc_SRCS}
)

add_dependencies(images 15_day_harib12a_naskfunc)
set(15_day_harib12a_naskfunc_WINE_OUTS ${root_BINARY_DIR}/projects/15_day/harib12a/naskfunc_wine.img)
add_custom_target(15_day_harib12a_naskfunc_wine
  COMMAND ${WINE} ${WINE_NASK} ${15_day_harib12a_naskfunc_SRCS} ${15_day_harib12a_naskfunc_WINE_OUTS}
)
add_dependencies(wine 15_day_harib12a_naskfunc_wine)

add_custom_target(15_day_harib12a_naskfunc_od
  COMMAND ${OD} -t x1 ${15_day_harib12a_naskfunc_OUTS}      > ${15_day_harib12a_naskfunc_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${15_day_harib12a_naskfunc_WINE_OUTS} > ${15_day_harib12a_naskfunc_WINE_OUTS}_t.txt
  COMMAND diff -s ${15_day_harib12a_naskfunc_OUTS}_f.txt ${15_day_harib12a_naskfunc_WINE_OUTS}_t.txt
)

add_dependencies(15_day_harib12a_naskfunc_od 15_day_harib12a_naskfunc)
add_dependencies(15_day_harib12a_naskfunc_od 15_day_harib12a_naskfunc_wine)
add_dependencies(od 15_day_harib12a_naskfunc_od)

#----------------------------------------------------------
