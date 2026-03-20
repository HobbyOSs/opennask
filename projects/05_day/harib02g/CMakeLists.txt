#----------------------------------------------------------
message(STATUS "Entering directory projects/05_day/harib02g/")

set(FONT ${root_BINARY_DIR}/src/makefont)
set(B2O  ${root_BINARY_DIR}/src/bin2obj)
set(CONV ${root_BINARY_DIR}/objconv/objconv)
set(05_day_harib02g_WILDOBJ ${root_BINARY_DIR}/projects/05_day/harib02g/*.o)

# Full OS build rules
set(05_day_harib02g_OS    ${root_BINARY_DIR}/projects/05_day/harib02g/os.img)
set(05_day_harib02g_SYS   ${root_BINARY_DIR}/projects/05_day/harib02g/os.sys)
set(05_day_harib02g_IPLB  ${root_BINARY_DIR}/projects/05_day/harib02g/ipl.bin)
set(05_day_harib02g_IPLS  ${root_SOURCE_DIR}/projects/05_day/harib02g/ipl10.nas)
set(05_day_harib02g_HEADB ${root_BINARY_DIR}/projects/05_day/harib02g/asmhead.bin)
set(05_day_harib02g_HEADS ${root_SOURCE_DIR}/projects/05_day/harib02g/asmhead.nas)
set(05_day_harib02g_BOOTB ${root_BINARY_DIR}/projects/05_day/harib02g/boot.bin)
set(05_day_harib02g_CCO   ${root_BINARY_DIR}/projects/05_day/harib02g/boot.o)
set(05_day_harib02g_CCS   ${root_SOURCE_DIR}/projects/05_day/harib02g/*.c)
set(05_day_harib02g_LDS   ${root_SOURCE_DIR}/projects/os.lds)
set(05_day_harib02g_FUNCS ${root_SOURCE_DIR}/projects/05_day/harib02g/naskfunc.nas)
set(05_day_harib02g_FUNCO ${root_BINARY_DIR}/projects/05_day/harib02g/naskfunc.o)
set(05_day_harib02g_FONTS ${root_SOURCE_DIR}/projects/05_day/harib02g/hankaku.txt)
set(05_day_harib02g_FONTB ${root_BINARY_DIR}/projects/05_day/harib02g/hankaku.bin)
set(05_day_harib02g_FONTO ${root_BINARY_DIR}/projects/05_day/harib02g/hankaku.o)
set(05_day_harib02g_LIBGE ${root_BINARY_DIR}/golibc/libgo.a)
set(05_day_harib02g_LIBGC ${root_BINARY_DIR}/projects/05_day/harib02g/libgo.o)

add_custom_target(05_day_harib02g_run
  COMMAND ${QEMU} ${QEMUOPT} ${05_day_harib02g_OS}
  DEPENDS 05_day_harib02g_img
)
add_custom_target(05_day_harib02g_debug
  COMMAND ${QEMU} -s -S ${QEMUOPT} ${05_day_harib02g_OS} ${QEMU_DEBUG_OPT}
  DEPENDS 05_day_harib02g_img
)
add_custom_target(05_day_harib02g_clean
  COMMAND rm -f ${05_day_harib02g_OS}
  COMMAND rm -f ${05_day_harib02g_SYS}
  COMMAND rm -f ${05_day_harib02g_IPLB}
  COMMAND rm -f ${05_day_harib02g_HEADB}
  COMMAND rm -f ${05_day_harib02g_BOOTB}
  COMMAND rm -f ${05_day_harib02g_WILDOBJ}
  COMMAND rm -f ${root_BINARY_DIR}/projects/05_day/harib02g/ipl10.bin
  COMMAND rm -f ${root_BINARY_DIR}/projects/05_day/harib02g/asmhead.bin
  COMMAND rm -f ${root_BINARY_DIR}/projects/05_day/harib02g/*_wine.bin
  COMMAND rm -f ${root_BINARY_DIR}/projects/05_day/harib02g/*.txt
)
add_custom_target(05_day_harib02g_ipl_build
  COMMAND ${RAKUSK_EXECUTABLE} ${05_day_harib02g_IPLS} ${05_day_harib02g_IPLB}
  DEPENDS ${05_day_harib02g_IPLS}
)
add_custom_command(
  OUTPUT ${05_day_harib02g_HEADB}
  COMMAND ${RAKUSK_EXECUTABLE} ${05_day_harib02g_HEADS} ${05_day_harib02g_HEADB}
  DEPENDS ${05_day_harib02g_HEADS}
  COMMENT "Generating asmhead.bin for 05_day_harib02g"
)

add_custom_command(
  OUTPUT ${05_day_harib02g_FUNCO}
  COMMAND ${RAKUSK_EXECUTABLE} ${05_day_harib02g_FUNCS} ${05_day_harib02g_FUNCO}
  DEPENDS ${05_day_harib02g_FUNCS}
  COMMENT "Generating naskfunc.o for 05_day_harib02g"
)
add_custom_command(
  OUTPUT ${05_day_harib02g_FONTO}
  COMMAND ${FONT} ${05_day_harib02g_FONTS} ${05_day_harib02g_FONTB}
  COMMAND ${B2O}  ${05_day_harib02g_FONTB} ${05_day_harib02g_FONTO} _hankaku
  DEPENDS ${05_day_harib02g_FONTS}
  COMMENT "Generating hankaku.o for 05_day_harib02g"
)
add_custom_command(
  OUTPUT ${05_day_harib02g_LIBGC}
  COMMAND ${CONV} -fcoff32 -nu ${05_day_harib02g_LIBGE} ${05_day_harib02g_LIBGC}
  DEPENDS ${05_day_harib02g_LIBGE}
  COMMENT "Converting golibc for 05_day_harib02g"
)
add_custom_target(05_day_harib02g_sys
  COMMAND gcc ${BINOPT} -T ${05_day_harib02g_LDS} ${05_day_harib02g_CCS} ${05_day_harib02g_WILDOBJ} -o ${05_day_harib02g_BOOTB}
  COMMAND cat ${05_day_harib02g_HEADB} ${05_day_harib02g_BOOTB} > ${05_day_harib02g_SYS}
  DEPENDS ${05_day_harib02g_HEADB} 05_day_harib02g_ipl_build
  DEPENDS ${05_day_harib02g_FUNCO}
  DEPENDS ${05_day_harib02g_FONTO} ${05_day_harib02g_LIBGC}
)
add_custom_target(05_day_harib02g_img
  COMMAND mformat -f 1440 -l HARIBOTEOS -N 0xffffffff -C -B ${05_day_harib02g_IPLB} -i ${05_day_harib02g_OS}
  COMMAND mcopy -i ${05_day_harib02g_OS} ${05_day_harib02g_SYS} ::
  DEPENDS 05_day_harib02g_sys
)

########### next target ###############
set(05_day_harib02g_asmhead_SRCS ${root_SOURCE_DIR}/projects/05_day/harib02g/asmhead.nas)
set(05_day_harib02g_asmhead_OUTS ${root_BINARY_DIR}/projects/05_day/harib02g/asmhead.bin)

add_custom_target(05_day_harib02g_asmhead
  COMMAND ${RAKUSK_EXECUTABLE} ${05_day_harib02g_asmhead_SRCS} ${05_day_harib02g_asmhead_OUTS}
  DEPENDS ${05_day_harib02g_asmhead_SRCS}
)

set(05_day_harib02g_asmhead_WINE_OUTS ${root_BINARY_DIR}/projects/05_day/harib02g/asmhead_wine.bin)
add_custom_target(05_day_harib02g_asmhead_wine
  COMMAND ${WINE} ${WINE_NASK} ${05_day_harib02g_asmhead_SRCS} ${05_day_harib02g_asmhead_WINE_OUTS}
)
add_dependencies(wine 05_day_harib02g_asmhead_wine)

add_custom_target(05_day_harib02g_asmhead_od
  COMMAND ${OD} -t x1 ${05_day_harib02g_asmhead_OUTS}      > ${05_day_harib02g_asmhead_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${05_day_harib02g_asmhead_WINE_OUTS} > ${05_day_harib02g_asmhead_WINE_OUTS}_t.txt
  COMMAND diff -s ${05_day_harib02g_asmhead_OUTS}_f.txt ${05_day_harib02g_asmhead_WINE_OUTS}_t.txt
)

add_dependencies(05_day_harib02g_asmhead_od 05_day_harib02g_asmhead)
add_dependencies(05_day_harib02g_asmhead_od 05_day_harib02g_asmhead_wine)
add_dependencies(od 05_day_harib02g_asmhead_od)

#----------------------------------------------------------
########### next target ###############
set(05_day_harib02g_ipl10_SRCS ${root_SOURCE_DIR}/projects/05_day/harib02g/ipl10.nas)
set(05_day_harib02g_ipl10_OUTS ${root_BINARY_DIR}/projects/05_day/harib02g/ipl10.bin)

add_custom_target(05_day_harib02g_ipl10
  COMMAND ${RAKUSK_EXECUTABLE} ${05_day_harib02g_ipl10_SRCS} ${05_day_harib02g_ipl10_OUTS}
  DEPENDS ${05_day_harib02g_ipl10_SRCS}
)

set(05_day_harib02g_ipl10_WINE_OUTS ${root_BINARY_DIR}/projects/05_day/harib02g/ipl10_wine.bin)
add_custom_target(05_day_harib02g_ipl10_wine
  COMMAND ${WINE} ${WINE_NASK} ${05_day_harib02g_ipl10_SRCS} ${05_day_harib02g_ipl10_WINE_OUTS}
)
add_dependencies(wine 05_day_harib02g_ipl10_wine)

add_custom_target(05_day_harib02g_ipl10_od
  COMMAND ${OD} -t x1 ${05_day_harib02g_ipl10_OUTS}      > ${05_day_harib02g_ipl10_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${05_day_harib02g_ipl10_WINE_OUTS} > ${05_day_harib02g_ipl10_WINE_OUTS}_t.txt
  COMMAND diff -s ${05_day_harib02g_ipl10_OUTS}_f.txt ${05_day_harib02g_ipl10_WINE_OUTS}_t.txt
)

add_dependencies(05_day_harib02g_ipl10_od 05_day_harib02g_ipl10)
add_dependencies(05_day_harib02g_ipl10_od 05_day_harib02g_ipl10_wine)
add_dependencies(od 05_day_harib02g_ipl10_od)

#----------------------------------------------------------
########### next target ###############
set(05_day_harib02g_naskfunc_SRCS ${root_SOURCE_DIR}/projects/05_day/harib02g/naskfunc.nas)
set(05_day_harib02g_naskfunc_OUTS ${root_BINARY_DIR}/projects/05_day/harib02g/naskfunc.img)

add_custom_target(05_day_harib02g_naskfunc
  COMMAND ${RAKUSK_EXECUTABLE} ${05_day_harib02g_naskfunc_SRCS} ${05_day_harib02g_naskfunc_OUTS}
  DEPENDS ${05_day_harib02g_naskfunc_SRCS}
)

add_dependencies(images 05_day_harib02g_naskfunc)
set(05_day_harib02g_naskfunc_WINE_OUTS ${root_BINARY_DIR}/projects/05_day/harib02g/naskfunc_wine.img)
add_custom_target(05_day_harib02g_naskfunc_wine
  COMMAND ${WINE} ${WINE_NASK} ${05_day_harib02g_naskfunc_SRCS} ${05_day_harib02g_naskfunc_WINE_OUTS}
)
add_dependencies(wine 05_day_harib02g_naskfunc_wine)

add_custom_target(05_day_harib02g_naskfunc_od
  COMMAND ${OD} -t x1 ${05_day_harib02g_naskfunc_OUTS}      > ${05_day_harib02g_naskfunc_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${05_day_harib02g_naskfunc_WINE_OUTS} > ${05_day_harib02g_naskfunc_WINE_OUTS}_t.txt
  COMMAND diff -s ${05_day_harib02g_naskfunc_OUTS}_f.txt ${05_day_harib02g_naskfunc_WINE_OUTS}_t.txt
)

add_dependencies(05_day_harib02g_naskfunc_od 05_day_harib02g_naskfunc)
add_dependencies(05_day_harib02g_naskfunc_od 05_day_harib02g_naskfunc_wine)
add_dependencies(od 05_day_harib02g_naskfunc_od)

#----------------------------------------------------------
