#----------------------------------------------------------
message(STATUS "Entering directory projects/17_day/harib14a/")

set(FONT ${root_BINARY_DIR}/src/makefont)
set(B2O  ${root_BINARY_DIR}/src/bin2obj)
set(CONV ${root_BINARY_DIR}/objconv/objconv)
set(17_day_harib14a_WILDOBJ ${root_BINARY_DIR}/projects/17_day/harib14a/*.o)

# Full OS build rules
set(17_day_harib14a_OS    ${root_BINARY_DIR}/projects/17_day/harib14a/os.img)
set(17_day_harib14a_SYS   ${root_BINARY_DIR}/projects/17_day/harib14a/os.sys)
set(17_day_harib14a_IPLB  ${root_BINARY_DIR}/projects/17_day/harib14a/ipl.bin)
set(17_day_harib14a_IPLS  ${root_SOURCE_DIR}/projects/17_day/harib14a/ipl10.nas)
set(17_day_harib14a_HEADB ${root_BINARY_DIR}/projects/17_day/harib14a/asmhead.bin)
set(17_day_harib14a_HEADS ${root_SOURCE_DIR}/projects/17_day/harib14a/asmhead.nas)
set(17_day_harib14a_BOOTB ${root_BINARY_DIR}/projects/17_day/harib14a/boot.bin)
set(17_day_harib14a_CCO   ${root_BINARY_DIR}/projects/17_day/harib14a/boot.o)
set(17_day_harib14a_CCS   ${root_SOURCE_DIR}/projects/17_day/harib14a/*.c)
set(17_day_harib14a_LDS   ${root_SOURCE_DIR}/projects/os.lds)
set(17_day_harib14a_FUNCS ${root_SOURCE_DIR}/projects/17_day/harib14a/naskfunc.nas)
set(17_day_harib14a_FUNCO ${root_BINARY_DIR}/projects/17_day/harib14a/naskfunc.o)
set(17_day_harib14a_FONTS ${root_SOURCE_DIR}/projects/17_day/harib14a/hankaku.txt)
set(17_day_harib14a_FONTB ${root_BINARY_DIR}/projects/17_day/harib14a/hankaku.bin)
set(17_day_harib14a_FONTO ${root_BINARY_DIR}/projects/17_day/harib14a/hankaku.o)
set(17_day_harib14a_LIBGE ${root_BINARY_DIR}/golibc/libgo.a)
set(17_day_harib14a_LIBGC ${root_BINARY_DIR}/projects/17_day/harib14a/libgo.o)

add_custom_target(17_day_harib14a_run
  COMMAND ${QEMU} ${QEMUOPT} ${17_day_harib14a_OS}
  DEPENDS 17_day_harib14a_img
)
add_custom_target(17_day_harib14a_debug
  COMMAND ${QEMU} -s -S ${QEMUOPT} ${17_day_harib14a_OS} ${QEMU_DEBUG_OPT}
  DEPENDS 17_day_harib14a_img
)
add_custom_target(17_day_harib14a_clean
  COMMAND rm -f ${17_day_harib14a_OS}
  COMMAND rm -f ${17_day_harib14a_SYS}
  COMMAND rm -f ${17_day_harib14a_IPLB}
  COMMAND rm -f ${17_day_harib14a_HEADB}
  COMMAND rm -f ${17_day_harib14a_BOOTB}
  COMMAND rm -f ${17_day_harib14a_WILDOBJ}
  COMMAND rm -f ${root_BINARY_DIR}/projects/17_day/harib14a/ipl10.bin
  COMMAND rm -f ${root_BINARY_DIR}/projects/17_day/harib14a/asmhead.bin
  COMMAND rm -f ${root_BINARY_DIR}/projects/17_day/harib14a/*_wine.bin
  COMMAND rm -f ${root_BINARY_DIR}/projects/17_day/harib14a/*.txt
)
add_custom_target(17_day_harib14a_ipl_build
  COMMAND ${RAKUSK_EXECUTABLE} ${17_day_harib14a_IPLS} ${17_day_harib14a_IPLB}
  DEPENDS ${17_day_harib14a_IPLS}
)
add_custom_command(
  OUTPUT ${17_day_harib14a_HEADB}
  COMMAND ${RAKUSK_EXECUTABLE} ${17_day_harib14a_HEADS} ${17_day_harib14a_HEADB}
  DEPENDS ${17_day_harib14a_HEADS}
  COMMENT "Generating asmhead.bin for 17_day_harib14a"
)

add_custom_command(
  OUTPUT ${17_day_harib14a_FUNCO}
  COMMAND ${RAKUSK_EXECUTABLE} ${17_day_harib14a_FUNCS} ${17_day_harib14a_FUNCO}
  DEPENDS ${17_day_harib14a_FUNCS}
  COMMENT "Generating naskfunc.o for 17_day_harib14a"
)
add_custom_command(
  OUTPUT ${17_day_harib14a_FONTO}
  COMMAND ${FONT} ${17_day_harib14a_FONTS} ${17_day_harib14a_FONTB}
  COMMAND ${B2O}  ${17_day_harib14a_FONTB} ${17_day_harib14a_FONTO} _hankaku
  DEPENDS ${17_day_harib14a_FONTS}
  COMMENT "Generating hankaku.o for 17_day_harib14a"
)
add_custom_command(
  OUTPUT ${17_day_harib14a_LIBGC}
  COMMAND ${CONV} -fcoff32 -nu ${17_day_harib14a_LIBGE} ${17_day_harib14a_LIBGC}
  DEPENDS ${17_day_harib14a_LIBGE}
  COMMENT "Converting golibc for 17_day_harib14a"
)
add_custom_target(17_day_harib14a_sys
  COMMAND gcc ${BINOPT} -T ${17_day_harib14a_LDS} ${17_day_harib14a_CCS} ${17_day_harib14a_WILDOBJ} -o ${17_day_harib14a_BOOTB}
  COMMAND cat ${17_day_harib14a_HEADB} ${17_day_harib14a_BOOTB} > ${17_day_harib14a_SYS}
  DEPENDS ${17_day_harib14a_HEADB} 17_day_harib14a_ipl_build
  DEPENDS ${17_day_harib14a_FUNCO}
  DEPENDS ${17_day_harib14a_FONTO} ${17_day_harib14a_LIBGC}
)
add_custom_target(17_day_harib14a_img
  COMMAND mformat -f 1440 -l HARIBOTEOS -N 0xffffffff -C -B ${17_day_harib14a_IPLB} -i ${17_day_harib14a_OS}
  COMMAND mcopy -i ${17_day_harib14a_OS} ${17_day_harib14a_SYS} ::
  DEPENDS 17_day_harib14a_sys
)

########### next target ###############
set(17_day_harib14a_asmhead_SRCS ${root_SOURCE_DIR}/projects/17_day/harib14a/asmhead.nas)
set(17_day_harib14a_asmhead_OUTS ${root_BINARY_DIR}/projects/17_day/harib14a/asmhead.bin)

add_custom_target(17_day_harib14a_asmhead
  COMMAND ${RAKUSK_EXECUTABLE} ${17_day_harib14a_asmhead_SRCS} ${17_day_harib14a_asmhead_OUTS}
  DEPENDS ${17_day_harib14a_asmhead_SRCS}
)

set(17_day_harib14a_asmhead_WINE_OUTS ${root_BINARY_DIR}/projects/17_day/harib14a/asmhead_wine.bin)
add_custom_target(17_day_harib14a_asmhead_wine
  COMMAND ${WINE} ${WINE_NASK} ${17_day_harib14a_asmhead_SRCS} ${17_day_harib14a_asmhead_WINE_OUTS}
)
add_dependencies(wine 17_day_harib14a_asmhead_wine)

add_custom_target(17_day_harib14a_asmhead_od
  COMMAND ${OD} -t x1 ${17_day_harib14a_asmhead_OUTS}      > ${17_day_harib14a_asmhead_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${17_day_harib14a_asmhead_WINE_OUTS} > ${17_day_harib14a_asmhead_WINE_OUTS}_t.txt
  COMMAND diff -s ${17_day_harib14a_asmhead_OUTS}_f.txt ${17_day_harib14a_asmhead_WINE_OUTS}_t.txt
)

add_dependencies(17_day_harib14a_asmhead_od 17_day_harib14a_asmhead)
add_dependencies(17_day_harib14a_asmhead_od 17_day_harib14a_asmhead_wine)
add_dependencies(od 17_day_harib14a_asmhead_od)

#----------------------------------------------------------
########### next target ###############
set(17_day_harib14a_ipl10_SRCS ${root_SOURCE_DIR}/projects/17_day/harib14a/ipl10.nas)
set(17_day_harib14a_ipl10_OUTS ${root_BINARY_DIR}/projects/17_day/harib14a/ipl10.bin)

add_custom_target(17_day_harib14a_ipl10
  COMMAND ${RAKUSK_EXECUTABLE} ${17_day_harib14a_ipl10_SRCS} ${17_day_harib14a_ipl10_OUTS}
  DEPENDS ${17_day_harib14a_ipl10_SRCS}
)

set(17_day_harib14a_ipl10_WINE_OUTS ${root_BINARY_DIR}/projects/17_day/harib14a/ipl10_wine.bin)
add_custom_target(17_day_harib14a_ipl10_wine
  COMMAND ${WINE} ${WINE_NASK} ${17_day_harib14a_ipl10_SRCS} ${17_day_harib14a_ipl10_WINE_OUTS}
)
add_dependencies(wine 17_day_harib14a_ipl10_wine)

add_custom_target(17_day_harib14a_ipl10_od
  COMMAND ${OD} -t x1 ${17_day_harib14a_ipl10_OUTS}      > ${17_day_harib14a_ipl10_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${17_day_harib14a_ipl10_WINE_OUTS} > ${17_day_harib14a_ipl10_WINE_OUTS}_t.txt
  COMMAND diff -s ${17_day_harib14a_ipl10_OUTS}_f.txt ${17_day_harib14a_ipl10_WINE_OUTS}_t.txt
)

add_dependencies(17_day_harib14a_ipl10_od 17_day_harib14a_ipl10)
add_dependencies(17_day_harib14a_ipl10_od 17_day_harib14a_ipl10_wine)
add_dependencies(od 17_day_harib14a_ipl10_od)

#----------------------------------------------------------
########### next target ###############
set(17_day_harib14a_naskfunc_SRCS ${root_SOURCE_DIR}/projects/17_day/harib14a/naskfunc.nas)
set(17_day_harib14a_naskfunc_OUTS ${root_BINARY_DIR}/projects/17_day/harib14a/naskfunc.img)

add_custom_target(17_day_harib14a_naskfunc
  COMMAND ${RAKUSK_EXECUTABLE} ${17_day_harib14a_naskfunc_SRCS} ${17_day_harib14a_naskfunc_OUTS}
  DEPENDS ${17_day_harib14a_naskfunc_SRCS}
)

add_dependencies(images 17_day_harib14a_naskfunc)
set(17_day_harib14a_naskfunc_WINE_OUTS ${root_BINARY_DIR}/projects/17_day/harib14a/naskfunc_wine.img)
add_custom_target(17_day_harib14a_naskfunc_wine
  COMMAND ${WINE} ${WINE_NASK} ${17_day_harib14a_naskfunc_SRCS} ${17_day_harib14a_naskfunc_WINE_OUTS}
)
add_dependencies(wine 17_day_harib14a_naskfunc_wine)

add_custom_target(17_day_harib14a_naskfunc_od
  COMMAND ${OD} -t x1 ${17_day_harib14a_naskfunc_OUTS}      > ${17_day_harib14a_naskfunc_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${17_day_harib14a_naskfunc_WINE_OUTS} > ${17_day_harib14a_naskfunc_WINE_OUTS}_t.txt
  COMMAND diff -s ${17_day_harib14a_naskfunc_OUTS}_f.txt ${17_day_harib14a_naskfunc_WINE_OUTS}_t.txt
)

add_dependencies(17_day_harib14a_naskfunc_od 17_day_harib14a_naskfunc)
add_dependencies(17_day_harib14a_naskfunc_od 17_day_harib14a_naskfunc_wine)
add_dependencies(od 17_day_harib14a_naskfunc_od)

#----------------------------------------------------------
