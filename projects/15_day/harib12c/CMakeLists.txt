#----------------------------------------------------------
message(STATUS "Entering directory projects/15_day/harib12c/")

set(FONT ${root_BINARY_DIR}/src/makefont)
set(B2O  ${root_BINARY_DIR}/src/bin2obj)
set(CONV ${root_BINARY_DIR}/objconv/objconv)
set(15_day_harib12c_WILDOBJ ${root_BINARY_DIR}/projects/15_day/harib12c/*.o)

# Full OS build rules
set(15_day_harib12c_OS    ${root_BINARY_DIR}/projects/15_day/harib12c/os.img)
set(15_day_harib12c_SYS   ${root_BINARY_DIR}/projects/15_day/harib12c/os.sys)
set(15_day_harib12c_IPLB  ${root_BINARY_DIR}/projects/15_day/harib12c/ipl.bin)
set(15_day_harib12c_IPLS  ${root_SOURCE_DIR}/projects/15_day/harib12c/ipl10.nas)
set(15_day_harib12c_HEADB ${root_BINARY_DIR}/projects/15_day/harib12c/asmhead.bin)
set(15_day_harib12c_HEADS ${root_SOURCE_DIR}/projects/15_day/harib12c/asmhead.nas)
set(15_day_harib12c_BOOTB ${root_BINARY_DIR}/projects/15_day/harib12c/boot.bin)
set(15_day_harib12c_CCO   ${root_BINARY_DIR}/projects/15_day/harib12c/boot.o)
set(15_day_harib12c_CCS   ${root_SOURCE_DIR}/projects/15_day/harib12c/*.c)
set(15_day_harib12c_LDS   ${root_SOURCE_DIR}/projects/os.lds)
set(15_day_harib12c_FUNCS ${root_SOURCE_DIR}/projects/15_day/harib12c/naskfunc.nas)
set(15_day_harib12c_FUNCO ${root_BINARY_DIR}/projects/15_day/harib12c/naskfunc.o)
set(15_day_harib12c_FONTS ${root_SOURCE_DIR}/projects/15_day/harib12c/hankaku.txt)
set(15_day_harib12c_FONTB ${root_BINARY_DIR}/projects/15_day/harib12c/hankaku.bin)
set(15_day_harib12c_FONTO ${root_BINARY_DIR}/projects/15_day/harib12c/hankaku.o)
set(15_day_harib12c_LIBGE ${root_BINARY_DIR}/golibc/libgo.a)
set(15_day_harib12c_LIBGC ${root_BINARY_DIR}/projects/15_day/harib12c/libgo.o)

add_custom_target(15_day_harib12c_run
  COMMAND ${QEMU} ${QEMUOPT} ${15_day_harib12c_OS}
  DEPENDS 15_day_harib12c_img
)
add_custom_target(15_day_harib12c_debug
  COMMAND ${QEMU} -s -S ${QEMUOPT} ${15_day_harib12c_OS} ${QEMU_DEBUG_OPT}
  DEPENDS 15_day_harib12c_img
)
add_custom_target(15_day_harib12c_clean
  COMMAND rm -f ${15_day_harib12c_OS}
  COMMAND rm -f ${15_day_harib12c_SYS}
  COMMAND rm -f ${15_day_harib12c_IPLB}
  COMMAND rm -f ${15_day_harib12c_HEADB}
  COMMAND rm -f ${15_day_harib12c_BOOTB}
  COMMAND rm -f ${15_day_harib12c_WILDOBJ}
  COMMAND rm -f ${root_BINARY_DIR}/projects/15_day/harib12c/ipl10.bin
  COMMAND rm -f ${root_BINARY_DIR}/projects/15_day/harib12c/asmhead.bin
  COMMAND rm -f ${root_BINARY_DIR}/projects/15_day/harib12c/*_wine.bin
  COMMAND rm -f ${root_BINARY_DIR}/projects/15_day/harib12c/*.txt
)
add_custom_target(15_day_harib12c_ipl_build
  COMMAND ${RAKUSK_EXECUTABLE} ${15_day_harib12c_IPLS} ${15_day_harib12c_IPLB}
  DEPENDS ${15_day_harib12c_IPLS}
)
add_custom_command(
  OUTPUT ${15_day_harib12c_HEADB}
  COMMAND ${RAKUSK_EXECUTABLE} ${15_day_harib12c_HEADS} ${15_day_harib12c_HEADB}
  DEPENDS ${15_day_harib12c_HEADS}
  COMMENT "Generating asmhead.bin for 15_day_harib12c"
)

add_custom_command(
  OUTPUT ${15_day_harib12c_FUNCO}
  COMMAND ${RAKUSK_EXECUTABLE} ${15_day_harib12c_FUNCS} ${15_day_harib12c_FUNCO}
  DEPENDS ${15_day_harib12c_FUNCS}
  COMMENT "Generating naskfunc.o for 15_day_harib12c"
)
add_custom_command(
  OUTPUT ${15_day_harib12c_FONTO}
  COMMAND ${FONT} ${15_day_harib12c_FONTS} ${15_day_harib12c_FONTB}
  COMMAND ${B2O}  ${15_day_harib12c_FONTB} ${15_day_harib12c_FONTO} _hankaku
  DEPENDS ${15_day_harib12c_FONTS}
  COMMENT "Generating hankaku.o for 15_day_harib12c"
)
add_custom_command(
  OUTPUT ${15_day_harib12c_LIBGC}
  COMMAND ${CONV} -fcoff32 -nu ${15_day_harib12c_LIBGE} ${15_day_harib12c_LIBGC}
  DEPENDS ${15_day_harib12c_LIBGE}
  COMMENT "Converting golibc for 15_day_harib12c"
)
add_custom_target(15_day_harib12c_sys
  COMMAND gcc ${BINOPT} -T ${15_day_harib12c_LDS} ${15_day_harib12c_CCS} ${15_day_harib12c_WILDOBJ} -o ${15_day_harib12c_BOOTB}
  COMMAND cat ${15_day_harib12c_HEADB} ${15_day_harib12c_BOOTB} > ${15_day_harib12c_SYS}
  DEPENDS ${15_day_harib12c_HEADB} 15_day_harib12c_ipl_build
  DEPENDS ${15_day_harib12c_FUNCO}
  DEPENDS ${15_day_harib12c_FONTO} ${15_day_harib12c_LIBGC}
)
add_custom_target(15_day_harib12c_img
  COMMAND mformat -f 1440 -l HARIBOTEOS -N 0xffffffff -C -B ${15_day_harib12c_IPLB} -i ${15_day_harib12c_OS}
  COMMAND mcopy -i ${15_day_harib12c_OS} ${15_day_harib12c_SYS} ::
  DEPENDS 15_day_harib12c_sys
)

########### next target ###############
set(15_day_harib12c_asmhead_SRCS ${root_SOURCE_DIR}/projects/15_day/harib12c/asmhead.nas)
set(15_day_harib12c_asmhead_OUTS ${root_BINARY_DIR}/projects/15_day/harib12c/asmhead.bin)

add_custom_target(15_day_harib12c_asmhead
  COMMAND ${RAKUSK_EXECUTABLE} ${15_day_harib12c_asmhead_SRCS} ${15_day_harib12c_asmhead_OUTS}
  DEPENDS ${15_day_harib12c_asmhead_SRCS}
)

set(15_day_harib12c_asmhead_WINE_OUTS ${root_BINARY_DIR}/projects/15_day/harib12c/asmhead_wine.bin)
add_custom_target(15_day_harib12c_asmhead_wine
  COMMAND ${WINE} ${WINE_NASK} ${15_day_harib12c_asmhead_SRCS} ${15_day_harib12c_asmhead_WINE_OUTS}
)
add_dependencies(wine 15_day_harib12c_asmhead_wine)

add_custom_target(15_day_harib12c_asmhead_od
  COMMAND ${OD} -t x1 ${15_day_harib12c_asmhead_OUTS}      > ${15_day_harib12c_asmhead_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${15_day_harib12c_asmhead_WINE_OUTS} > ${15_day_harib12c_asmhead_WINE_OUTS}_t.txt
  COMMAND diff -s ${15_day_harib12c_asmhead_OUTS}_f.txt ${15_day_harib12c_asmhead_WINE_OUTS}_t.txt
)

add_dependencies(15_day_harib12c_asmhead_od 15_day_harib12c_asmhead)
add_dependencies(15_day_harib12c_asmhead_od 15_day_harib12c_asmhead_wine)
add_dependencies(od 15_day_harib12c_asmhead_od)

#----------------------------------------------------------
########### next target ###############
set(15_day_harib12c_ipl10_SRCS ${root_SOURCE_DIR}/projects/15_day/harib12c/ipl10.nas)
set(15_day_harib12c_ipl10_OUTS ${root_BINARY_DIR}/projects/15_day/harib12c/ipl10.bin)

add_custom_target(15_day_harib12c_ipl10
  COMMAND ${RAKUSK_EXECUTABLE} ${15_day_harib12c_ipl10_SRCS} ${15_day_harib12c_ipl10_OUTS}
  DEPENDS ${15_day_harib12c_ipl10_SRCS}
)

set(15_day_harib12c_ipl10_WINE_OUTS ${root_BINARY_DIR}/projects/15_day/harib12c/ipl10_wine.bin)
add_custom_target(15_day_harib12c_ipl10_wine
  COMMAND ${WINE} ${WINE_NASK} ${15_day_harib12c_ipl10_SRCS} ${15_day_harib12c_ipl10_WINE_OUTS}
)
add_dependencies(wine 15_day_harib12c_ipl10_wine)

add_custom_target(15_day_harib12c_ipl10_od
  COMMAND ${OD} -t x1 ${15_day_harib12c_ipl10_OUTS}      > ${15_day_harib12c_ipl10_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${15_day_harib12c_ipl10_WINE_OUTS} > ${15_day_harib12c_ipl10_WINE_OUTS}_t.txt
  COMMAND diff -s ${15_day_harib12c_ipl10_OUTS}_f.txt ${15_day_harib12c_ipl10_WINE_OUTS}_t.txt
)

add_dependencies(15_day_harib12c_ipl10_od 15_day_harib12c_ipl10)
add_dependencies(15_day_harib12c_ipl10_od 15_day_harib12c_ipl10_wine)
add_dependencies(od 15_day_harib12c_ipl10_od)

#----------------------------------------------------------
########### next target ###############
set(15_day_harib12c_naskfunc_SRCS ${root_SOURCE_DIR}/projects/15_day/harib12c/naskfunc.nas)
set(15_day_harib12c_naskfunc_OUTS ${root_BINARY_DIR}/projects/15_day/harib12c/naskfunc.img)

add_custom_target(15_day_harib12c_naskfunc
  COMMAND ${RAKUSK_EXECUTABLE} ${15_day_harib12c_naskfunc_SRCS} ${15_day_harib12c_naskfunc_OUTS}
  DEPENDS ${15_day_harib12c_naskfunc_SRCS}
)

add_dependencies(images 15_day_harib12c_naskfunc)
set(15_day_harib12c_naskfunc_WINE_OUTS ${root_BINARY_DIR}/projects/15_day/harib12c/naskfunc_wine.img)
add_custom_target(15_day_harib12c_naskfunc_wine
  COMMAND ${WINE} ${WINE_NASK} ${15_day_harib12c_naskfunc_SRCS} ${15_day_harib12c_naskfunc_WINE_OUTS}
)
add_dependencies(wine 15_day_harib12c_naskfunc_wine)

add_custom_target(15_day_harib12c_naskfunc_od
  COMMAND ${OD} -t x1 ${15_day_harib12c_naskfunc_OUTS}      > ${15_day_harib12c_naskfunc_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${15_day_harib12c_naskfunc_WINE_OUTS} > ${15_day_harib12c_naskfunc_WINE_OUTS}_t.txt
  COMMAND diff -s ${15_day_harib12c_naskfunc_OUTS}_f.txt ${15_day_harib12c_naskfunc_WINE_OUTS}_t.txt
)

add_dependencies(15_day_harib12c_naskfunc_od 15_day_harib12c_naskfunc)
add_dependencies(15_day_harib12c_naskfunc_od 15_day_harib12c_naskfunc_wine)
add_dependencies(od 15_day_harib12c_naskfunc_od)

#----------------------------------------------------------
