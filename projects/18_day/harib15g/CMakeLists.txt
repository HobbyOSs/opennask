#----------------------------------------------------------
message(STATUS "Entering directory projects/18_day/harib15g/")

set(FONT ${root_BINARY_DIR}/src/makefont)
set(B2O  ${root_BINARY_DIR}/src/bin2obj)
set(CONV ${root_BINARY_DIR}/objconv/objconv)
set(18_day_harib15g_WILDOBJ ${root_BINARY_DIR}/projects/18_day/harib15g/*.o)

# Full OS build rules
set(18_day_harib15g_OS    ${root_BINARY_DIR}/projects/18_day/harib15g/os.img)
set(18_day_harib15g_SYS   ${root_BINARY_DIR}/projects/18_day/harib15g/os.sys)
set(18_day_harib15g_IPLB  ${root_BINARY_DIR}/projects/18_day/harib15g/ipl.bin)
set(18_day_harib15g_IPLS  ${root_SOURCE_DIR}/projects/18_day/harib15g/ipl10.nas)
set(18_day_harib15g_HEADB ${root_BINARY_DIR}/projects/18_day/harib15g/asmhead.bin)
set(18_day_harib15g_HEADS ${root_SOURCE_DIR}/projects/18_day/harib15g/asmhead.nas)
set(18_day_harib15g_BOOTB ${root_BINARY_DIR}/projects/18_day/harib15g/boot.bin)
set(18_day_harib15g_CCO   ${root_BINARY_DIR}/projects/18_day/harib15g/boot.o)
set(18_day_harib15g_CCS   ${root_SOURCE_DIR}/projects/18_day/harib15g/*.c)
set(18_day_harib15g_LDS   ${root_SOURCE_DIR}/projects/os.lds)
set(18_day_harib15g_FUNCS ${root_SOURCE_DIR}/projects/18_day/harib15g/naskfunc.nas)
set(18_day_harib15g_FUNCO ${root_BINARY_DIR}/projects/18_day/harib15g/naskfunc.o)
set(18_day_harib15g_FONTS ${root_SOURCE_DIR}/projects/18_day/harib15g/hankaku.txt)
set(18_day_harib15g_FONTB ${root_BINARY_DIR}/projects/18_day/harib15g/hankaku.bin)
set(18_day_harib15g_FONTO ${root_BINARY_DIR}/projects/18_day/harib15g/hankaku.o)
set(18_day_harib15g_LIBGE ${root_BINARY_DIR}/golibc/libgo.a)
set(18_day_harib15g_LIBGC ${root_BINARY_DIR}/projects/18_day/harib15g/libgo.o)

add_custom_target(18_day_harib15g_run
  COMMAND ${QEMU} ${QEMUOPT} ${18_day_harib15g_OS}
  DEPENDS 18_day_harib15g_img
)
add_custom_target(18_day_harib15g_debug
  COMMAND ${QEMU} -s -S ${QEMUOPT} ${18_day_harib15g_OS} ${QEMU_DEBUG_OPT}
  DEPENDS 18_day_harib15g_img
)
add_custom_target(18_day_harib15g_clean
  COMMAND rm -f ${18_day_harib15g_OS}
  COMMAND rm -f ${18_day_harib15g_SYS}
  COMMAND rm -f ${18_day_harib15g_IPLB}
  COMMAND rm -f ${18_day_harib15g_HEADB}
  COMMAND rm -f ${18_day_harib15g_BOOTB}
  COMMAND rm -f ${18_day_harib15g_WILDOBJ}
  COMMAND rm -f ${root_BINARY_DIR}/projects/18_day/harib15g/ipl10.bin
  COMMAND rm -f ${root_BINARY_DIR}/projects/18_day/harib15g/asmhead.bin
  COMMAND rm -f ${root_BINARY_DIR}/projects/18_day/harib15g/*_wine.bin
  COMMAND rm -f ${root_BINARY_DIR}/projects/18_day/harib15g/*.txt
)
add_custom_target(18_day_harib15g_ipl_build
  COMMAND ${RAKUSK_EXECUTABLE} ${18_day_harib15g_IPLS} ${18_day_harib15g_IPLB}
  DEPENDS ${18_day_harib15g_IPLS}
)
add_custom_command(
  OUTPUT ${18_day_harib15g_HEADB}
  COMMAND ${RAKUSK_EXECUTABLE} ${18_day_harib15g_HEADS} ${18_day_harib15g_HEADB}
  DEPENDS ${18_day_harib15g_HEADS}
  COMMENT "Generating asmhead.bin for 18_day_harib15g"
)

add_custom_command(
  OUTPUT ${18_day_harib15g_FUNCO}
  COMMAND ${RAKUSK_EXECUTABLE} ${18_day_harib15g_FUNCS} ${18_day_harib15g_FUNCO}
  DEPENDS ${18_day_harib15g_FUNCS}
  COMMENT "Generating naskfunc.o for 18_day_harib15g"
)
add_custom_command(
  OUTPUT ${18_day_harib15g_FONTO}
  COMMAND ${FONT} ${18_day_harib15g_FONTS} ${18_day_harib15g_FONTB}
  COMMAND ${B2O}  ${18_day_harib15g_FONTB} ${18_day_harib15g_FONTO} _hankaku
  DEPENDS ${18_day_harib15g_FONTS}
  COMMENT "Generating hankaku.o for 18_day_harib15g"
)
add_custom_command(
  OUTPUT ${18_day_harib15g_LIBGC}
  COMMAND ${CONV} -fcoff32 -nu ${18_day_harib15g_LIBGE} ${18_day_harib15g_LIBGC}
  DEPENDS ${18_day_harib15g_LIBGE}
  COMMENT "Converting golibc for 18_day_harib15g"
)
add_custom_target(18_day_harib15g_sys
  COMMAND gcc ${BINOPT} -T ${18_day_harib15g_LDS} ${18_day_harib15g_CCS} ${18_day_harib15g_WILDOBJ} -o ${18_day_harib15g_BOOTB}
  COMMAND cat ${18_day_harib15g_HEADB} ${18_day_harib15g_BOOTB} > ${18_day_harib15g_SYS}
  DEPENDS ${18_day_harib15g_HEADB} 18_day_harib15g_ipl_build
  DEPENDS ${18_day_harib15g_FUNCO}
  DEPENDS ${18_day_harib15g_FONTO} ${18_day_harib15g_LIBGC}
)
add_custom_target(18_day_harib15g_img
  COMMAND mformat -f 1440 -l HARIBOTEOS -N 0xffffffff -C -B ${18_day_harib15g_IPLB} -i ${18_day_harib15g_OS}
  COMMAND mcopy -i ${18_day_harib15g_OS} ${18_day_harib15g_SYS} ::
  DEPENDS 18_day_harib15g_sys
)

########### next target ###############
set(18_day_harib15g_asmhead_SRCS ${root_SOURCE_DIR}/projects/18_day/harib15g/asmhead.nas)
set(18_day_harib15g_asmhead_OUTS ${root_BINARY_DIR}/projects/18_day/harib15g/asmhead.bin)

add_custom_target(18_day_harib15g_asmhead
  COMMAND ${RAKUSK_EXECUTABLE} ${18_day_harib15g_asmhead_SRCS} ${18_day_harib15g_asmhead_OUTS}
  DEPENDS ${18_day_harib15g_asmhead_SRCS}
)

set(18_day_harib15g_asmhead_WINE_OUTS ${root_BINARY_DIR}/projects/18_day/harib15g/asmhead_wine.bin)
add_custom_target(18_day_harib15g_asmhead_wine
  COMMAND ${WINE} ${WINE_NASK} ${18_day_harib15g_asmhead_SRCS} ${18_day_harib15g_asmhead_WINE_OUTS}
)
add_dependencies(wine 18_day_harib15g_asmhead_wine)

add_custom_target(18_day_harib15g_asmhead_od
  COMMAND ${OD} -t x1 ${18_day_harib15g_asmhead_OUTS}      > ${18_day_harib15g_asmhead_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${18_day_harib15g_asmhead_WINE_OUTS} > ${18_day_harib15g_asmhead_WINE_OUTS}_t.txt
  COMMAND diff -s ${18_day_harib15g_asmhead_OUTS}_f.txt ${18_day_harib15g_asmhead_WINE_OUTS}_t.txt
)

add_dependencies(18_day_harib15g_asmhead_od 18_day_harib15g_asmhead)
add_dependencies(18_day_harib15g_asmhead_od 18_day_harib15g_asmhead_wine)
add_dependencies(od 18_day_harib15g_asmhead_od)

#----------------------------------------------------------
########### next target ###############
set(18_day_harib15g_ipl10_SRCS ${root_SOURCE_DIR}/projects/18_day/harib15g/ipl10.nas)
set(18_day_harib15g_ipl10_OUTS ${root_BINARY_DIR}/projects/18_day/harib15g/ipl10.bin)

add_custom_target(18_day_harib15g_ipl10
  COMMAND ${RAKUSK_EXECUTABLE} ${18_day_harib15g_ipl10_SRCS} ${18_day_harib15g_ipl10_OUTS}
  DEPENDS ${18_day_harib15g_ipl10_SRCS}
)

set(18_day_harib15g_ipl10_WINE_OUTS ${root_BINARY_DIR}/projects/18_day/harib15g/ipl10_wine.bin)
add_custom_target(18_day_harib15g_ipl10_wine
  COMMAND ${WINE} ${WINE_NASK} ${18_day_harib15g_ipl10_SRCS} ${18_day_harib15g_ipl10_WINE_OUTS}
)
add_dependencies(wine 18_day_harib15g_ipl10_wine)

add_custom_target(18_day_harib15g_ipl10_od
  COMMAND ${OD} -t x1 ${18_day_harib15g_ipl10_OUTS}      > ${18_day_harib15g_ipl10_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${18_day_harib15g_ipl10_WINE_OUTS} > ${18_day_harib15g_ipl10_WINE_OUTS}_t.txt
  COMMAND diff -s ${18_day_harib15g_ipl10_OUTS}_f.txt ${18_day_harib15g_ipl10_WINE_OUTS}_t.txt
)

add_dependencies(18_day_harib15g_ipl10_od 18_day_harib15g_ipl10)
add_dependencies(18_day_harib15g_ipl10_od 18_day_harib15g_ipl10_wine)
add_dependencies(od 18_day_harib15g_ipl10_od)

#----------------------------------------------------------
########### next target ###############
set(18_day_harib15g_naskfunc_SRCS ${root_SOURCE_DIR}/projects/18_day/harib15g/naskfunc.nas)
set(18_day_harib15g_naskfunc_OUTS ${root_BINARY_DIR}/projects/18_day/harib15g/naskfunc.img)

add_custom_target(18_day_harib15g_naskfunc
  COMMAND ${RAKUSK_EXECUTABLE} ${18_day_harib15g_naskfunc_SRCS} ${18_day_harib15g_naskfunc_OUTS}
  DEPENDS ${18_day_harib15g_naskfunc_SRCS}
)

add_dependencies(images 18_day_harib15g_naskfunc)
set(18_day_harib15g_naskfunc_WINE_OUTS ${root_BINARY_DIR}/projects/18_day/harib15g/naskfunc_wine.img)
add_custom_target(18_day_harib15g_naskfunc_wine
  COMMAND ${WINE} ${WINE_NASK} ${18_day_harib15g_naskfunc_SRCS} ${18_day_harib15g_naskfunc_WINE_OUTS}
)
add_dependencies(wine 18_day_harib15g_naskfunc_wine)

add_custom_target(18_day_harib15g_naskfunc_od
  COMMAND ${OD} -t x1 ${18_day_harib15g_naskfunc_OUTS}      > ${18_day_harib15g_naskfunc_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${18_day_harib15g_naskfunc_WINE_OUTS} > ${18_day_harib15g_naskfunc_WINE_OUTS}_t.txt
  COMMAND diff -s ${18_day_harib15g_naskfunc_OUTS}_f.txt ${18_day_harib15g_naskfunc_WINE_OUTS}_t.txt
)

add_dependencies(18_day_harib15g_naskfunc_od 18_day_harib15g_naskfunc)
add_dependencies(18_day_harib15g_naskfunc_od 18_day_harib15g_naskfunc_wine)
add_dependencies(od 18_day_harib15g_naskfunc_od)

#----------------------------------------------------------
