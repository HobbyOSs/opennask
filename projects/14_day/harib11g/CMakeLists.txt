#----------------------------------------------------------
message(STATUS "Entering directory projects/14_day/harib11g/")

set(FONT ${root_BINARY_DIR}/src/makefont)
set(B2O  ${root_BINARY_DIR}/src/bin2obj)
set(CONV ${root_BINARY_DIR}/objconv/objconv)
set(14_day_harib11g_WILDOBJ ${root_BINARY_DIR}/projects/14_day/harib11g/*.o)

# Full OS build rules
set(14_day_harib11g_OS    ${root_BINARY_DIR}/projects/14_day/harib11g/os.img)
set(14_day_harib11g_SYS   ${root_BINARY_DIR}/projects/14_day/harib11g/os.sys)
set(14_day_harib11g_IPLB  ${root_BINARY_DIR}/projects/14_day/harib11g/ipl.bin)
set(14_day_harib11g_IPLS  ${root_SOURCE_DIR}/projects/14_day/harib11g/ipl10.nas)
set(14_day_harib11g_HEADB ${root_BINARY_DIR}/projects/14_day/harib11g/asmhead.bin)
set(14_day_harib11g_HEADS ${root_SOURCE_DIR}/projects/14_day/harib11g/asmhead.nas)
set(14_day_harib11g_BOOTB ${root_BINARY_DIR}/projects/14_day/harib11g/boot.bin)
set(14_day_harib11g_CCO   ${root_BINARY_DIR}/projects/14_day/harib11g/boot.o)
set(14_day_harib11g_CCS   ${root_SOURCE_DIR}/projects/14_day/harib11g/*.c)
set(14_day_harib11g_LDS   ${root_SOURCE_DIR}/projects/os.lds)
set(14_day_harib11g_FUNCS ${root_SOURCE_DIR}/projects/14_day/harib11g/naskfunc.nas)
set(14_day_harib11g_FUNCO ${root_BINARY_DIR}/projects/14_day/harib11g/naskfunc.o)
set(14_day_harib11g_FONTS ${root_SOURCE_DIR}/projects/14_day/harib11g/hankaku.txt)
set(14_day_harib11g_FONTB ${root_BINARY_DIR}/projects/14_day/harib11g/hankaku.bin)
set(14_day_harib11g_FONTO ${root_BINARY_DIR}/projects/14_day/harib11g/hankaku.o)
set(14_day_harib11g_LIBGE ${root_BINARY_DIR}/golibc/libgo.a)
set(14_day_harib11g_LIBGC ${root_BINARY_DIR}/projects/14_day/harib11g/libgo.o)

add_custom_target(14_day_harib11g_run
  COMMAND ${QEMU} ${QEMUOPT} ${14_day_harib11g_OS}
  DEPENDS 14_day_harib11g_img
)
add_custom_target(14_day_harib11g_debug
  COMMAND ${QEMU} -s -S ${QEMUOPT} ${14_day_harib11g_OS} ${QEMU_DEBUG_OPT}
  DEPENDS 14_day_harib11g_img
)
add_custom_target(14_day_harib11g_clean
  COMMAND rm -f ${14_day_harib11g_OS}
  COMMAND rm -f ${14_day_harib11g_SYS}
  COMMAND rm -f ${14_day_harib11g_IPLB}
  COMMAND rm -f ${14_day_harib11g_HEADB}
  COMMAND rm -f ${14_day_harib11g_BOOTB}
  COMMAND rm -f ${14_day_harib11g_WILDOBJ}
  COMMAND rm -f ${root_BINARY_DIR}/projects/14_day/harib11g/ipl10.bin
  COMMAND rm -f ${root_BINARY_DIR}/projects/14_day/harib11g/asmhead.bin
  COMMAND rm -f ${root_BINARY_DIR}/projects/14_day/harib11g/*_wine.bin
  COMMAND rm -f ${root_BINARY_DIR}/projects/14_day/harib11g/*.txt
)
add_custom_target(14_day_harib11g_ipl_build
  COMMAND ${RAKUSK_EXECUTABLE} ${14_day_harib11g_IPLS} ${14_day_harib11g_IPLB}
  DEPENDS ${14_day_harib11g_IPLS}
)
add_custom_command(
  OUTPUT ${14_day_harib11g_HEADB}
  COMMAND ${RAKUSK_EXECUTABLE} ${14_day_harib11g_HEADS} ${14_day_harib11g_HEADB}
  DEPENDS ${14_day_harib11g_HEADS}
  COMMENT "Generating asmhead.bin for 14_day_harib11g"
)

add_custom_command(
  OUTPUT ${14_day_harib11g_FUNCO}
  COMMAND ${RAKUSK_EXECUTABLE} ${14_day_harib11g_FUNCS} ${14_day_harib11g_FUNCO}
  DEPENDS ${14_day_harib11g_FUNCS}
  COMMENT "Generating naskfunc.o for 14_day_harib11g"
)
add_custom_command(
  OUTPUT ${14_day_harib11g_FONTO}
  COMMAND ${FONT} ${14_day_harib11g_FONTS} ${14_day_harib11g_FONTB}
  COMMAND ${B2O}  ${14_day_harib11g_FONTB} ${14_day_harib11g_FONTO} _hankaku
  DEPENDS ${14_day_harib11g_FONTS}
  COMMENT "Generating hankaku.o for 14_day_harib11g"
)
add_custom_command(
  OUTPUT ${14_day_harib11g_LIBGC}
  COMMAND ${CONV} -fcoff32 -nu ${14_day_harib11g_LIBGE} ${14_day_harib11g_LIBGC}
  DEPENDS ${14_day_harib11g_LIBGE}
  COMMENT "Converting golibc for 14_day_harib11g"
)
add_custom_target(14_day_harib11g_sys
  COMMAND gcc ${BINOPT} -T ${14_day_harib11g_LDS} ${14_day_harib11g_CCS} ${14_day_harib11g_WILDOBJ} -o ${14_day_harib11g_BOOTB}
  COMMAND cat ${14_day_harib11g_HEADB} ${14_day_harib11g_BOOTB} > ${14_day_harib11g_SYS}
  DEPENDS ${14_day_harib11g_HEADB} 14_day_harib11g_ipl_build
  DEPENDS ${14_day_harib11g_FUNCO}
  DEPENDS ${14_day_harib11g_FONTO} ${14_day_harib11g_LIBGC}
)
add_custom_target(14_day_harib11g_img
  COMMAND mformat -f 1440 -l HARIBOTEOS -N 0xffffffff -C -B ${14_day_harib11g_IPLB} -i ${14_day_harib11g_OS}
  COMMAND mcopy -i ${14_day_harib11g_OS} ${14_day_harib11g_SYS} ::
  DEPENDS 14_day_harib11g_sys
)

########### next target ###############
set(14_day_harib11g_asmhead_SRCS ${root_SOURCE_DIR}/projects/14_day/harib11g/asmhead.nas)
set(14_day_harib11g_asmhead_OUTS ${root_BINARY_DIR}/projects/14_day/harib11g/asmhead.bin)

add_custom_target(14_day_harib11g_asmhead
  COMMAND ${RAKUSK_EXECUTABLE} ${14_day_harib11g_asmhead_SRCS} ${14_day_harib11g_asmhead_OUTS}
  DEPENDS ${14_day_harib11g_asmhead_SRCS}
)

set(14_day_harib11g_asmhead_WINE_OUTS ${root_BINARY_DIR}/projects/14_day/harib11g/asmhead_wine.bin)
add_custom_target(14_day_harib11g_asmhead_wine
  COMMAND ${WINE} ${WINE_NASK} ${14_day_harib11g_asmhead_SRCS} ${14_day_harib11g_asmhead_WINE_OUTS}
)
add_dependencies(wine 14_day_harib11g_asmhead_wine)

add_custom_target(14_day_harib11g_asmhead_od
  COMMAND ${OD} -t x1 ${14_day_harib11g_asmhead_OUTS}      > ${14_day_harib11g_asmhead_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${14_day_harib11g_asmhead_WINE_OUTS} > ${14_day_harib11g_asmhead_WINE_OUTS}_t.txt
  COMMAND diff -s ${14_day_harib11g_asmhead_OUTS}_f.txt ${14_day_harib11g_asmhead_WINE_OUTS}_t.txt
)

add_dependencies(14_day_harib11g_asmhead_od 14_day_harib11g_asmhead)
add_dependencies(14_day_harib11g_asmhead_od 14_day_harib11g_asmhead_wine)
add_dependencies(od 14_day_harib11g_asmhead_od)

#----------------------------------------------------------
########### next target ###############
set(14_day_harib11g_ipl10_SRCS ${root_SOURCE_DIR}/projects/14_day/harib11g/ipl10.nas)
set(14_day_harib11g_ipl10_OUTS ${root_BINARY_DIR}/projects/14_day/harib11g/ipl10.bin)

add_custom_target(14_day_harib11g_ipl10
  COMMAND ${RAKUSK_EXECUTABLE} ${14_day_harib11g_ipl10_SRCS} ${14_day_harib11g_ipl10_OUTS}
  DEPENDS ${14_day_harib11g_ipl10_SRCS}
)

set(14_day_harib11g_ipl10_WINE_OUTS ${root_BINARY_DIR}/projects/14_day/harib11g/ipl10_wine.bin)
add_custom_target(14_day_harib11g_ipl10_wine
  COMMAND ${WINE} ${WINE_NASK} ${14_day_harib11g_ipl10_SRCS} ${14_day_harib11g_ipl10_WINE_OUTS}
)
add_dependencies(wine 14_day_harib11g_ipl10_wine)

add_custom_target(14_day_harib11g_ipl10_od
  COMMAND ${OD} -t x1 ${14_day_harib11g_ipl10_OUTS}      > ${14_day_harib11g_ipl10_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${14_day_harib11g_ipl10_WINE_OUTS} > ${14_day_harib11g_ipl10_WINE_OUTS}_t.txt
  COMMAND diff -s ${14_day_harib11g_ipl10_OUTS}_f.txt ${14_day_harib11g_ipl10_WINE_OUTS}_t.txt
)

add_dependencies(14_day_harib11g_ipl10_od 14_day_harib11g_ipl10)
add_dependencies(14_day_harib11g_ipl10_od 14_day_harib11g_ipl10_wine)
add_dependencies(od 14_day_harib11g_ipl10_od)

#----------------------------------------------------------
########### next target ###############
set(14_day_harib11g_naskfunc_SRCS ${root_SOURCE_DIR}/projects/14_day/harib11g/naskfunc.nas)
set(14_day_harib11g_naskfunc_OUTS ${root_BINARY_DIR}/projects/14_day/harib11g/naskfunc.img)

add_custom_target(14_day_harib11g_naskfunc
  COMMAND ${RAKUSK_EXECUTABLE} ${14_day_harib11g_naskfunc_SRCS} ${14_day_harib11g_naskfunc_OUTS}
  DEPENDS ${14_day_harib11g_naskfunc_SRCS}
)

add_dependencies(images 14_day_harib11g_naskfunc)
set(14_day_harib11g_naskfunc_WINE_OUTS ${root_BINARY_DIR}/projects/14_day/harib11g/naskfunc_wine.img)
add_custom_target(14_day_harib11g_naskfunc_wine
  COMMAND ${WINE} ${WINE_NASK} ${14_day_harib11g_naskfunc_SRCS} ${14_day_harib11g_naskfunc_WINE_OUTS}
)
add_dependencies(wine 14_day_harib11g_naskfunc_wine)

add_custom_target(14_day_harib11g_naskfunc_od
  COMMAND ${OD} -t x1 ${14_day_harib11g_naskfunc_OUTS}      > ${14_day_harib11g_naskfunc_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${14_day_harib11g_naskfunc_WINE_OUTS} > ${14_day_harib11g_naskfunc_WINE_OUTS}_t.txt
  COMMAND diff -s ${14_day_harib11g_naskfunc_OUTS}_f.txt ${14_day_harib11g_naskfunc_WINE_OUTS}_t.txt
)

add_dependencies(14_day_harib11g_naskfunc_od 14_day_harib11g_naskfunc)
add_dependencies(14_day_harib11g_naskfunc_od 14_day_harib11g_naskfunc_wine)
add_dependencies(od 14_day_harib11g_naskfunc_od)

#----------------------------------------------------------
