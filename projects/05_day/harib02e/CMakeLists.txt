#----------------------------------------------------------
message(STATUS "Entering directory projects/05_day/harib02e/")

set(FONT ${root_BINARY_DIR}/src/makefont)
set(B2O  ${root_BINARY_DIR}/src/bin2obj)
set(CONV ${root_BINARY_DIR}/objconv/objconv)
set(05_day_harib02e_WILDOBJ ${root_BINARY_DIR}/projects/05_day/harib02e/*.o)

# Full OS build rules
set(05_day_harib02e_OS    ${root_BINARY_DIR}/projects/05_day/harib02e/os.img)
set(05_day_harib02e_SYS   ${root_BINARY_DIR}/projects/05_day/harib02e/os.sys)
set(05_day_harib02e_IPLB  ${root_BINARY_DIR}/projects/05_day/harib02e/ipl.bin)
set(05_day_harib02e_IPLS  ${root_SOURCE_DIR}/projects/05_day/harib02e/ipl10.nas)
set(05_day_harib02e_HEADB ${root_BINARY_DIR}/projects/05_day/harib02e/asmhead.bin)
set(05_day_harib02e_HEADS ${root_SOURCE_DIR}/projects/05_day/harib02e/asmhead.nas)
set(05_day_harib02e_BOOTB ${root_BINARY_DIR}/projects/05_day/harib02e/boot.bin)
set(05_day_harib02e_CCO   ${root_BINARY_DIR}/projects/05_day/harib02e/boot.o)
set(05_day_harib02e_CCS   ${root_SOURCE_DIR}/projects/05_day/harib02e/*.c)
set(05_day_harib02e_LDS   ${root_SOURCE_DIR}/projects/os.lds)
set(05_day_harib02e_FUNCS ${root_SOURCE_DIR}/projects/05_day/harib02e/naskfunc.nas)
set(05_day_harib02e_FUNCO ${root_BINARY_DIR}/projects/05_day/harib02e/naskfunc.o)
set(05_day_harib02e_FONTS ${root_SOURCE_DIR}/projects/05_day/harib02e/hankaku.txt)
set(05_day_harib02e_FONTB ${root_BINARY_DIR}/projects/05_day/harib02e/hankaku.bin)
set(05_day_harib02e_FONTO ${root_BINARY_DIR}/projects/05_day/harib02e/hankaku.o)
set(05_day_harib02e_LIBGE ${root_BINARY_DIR}/golibc/libgo.a)
set(05_day_harib02e_LIBGC ${root_BINARY_DIR}/projects/05_day/harib02e/libgo.o)

add_custom_target(05_day_harib02e_run
  COMMAND ${QEMU} ${QEMUOPT} ${05_day_harib02e_OS}
  DEPENDS 05_day_harib02e_img
)
add_custom_target(05_day_harib02e_debug
  COMMAND ${QEMU} -s -S ${QEMUOPT} ${05_day_harib02e_OS} ${QEMU_DEBUG_OPT}
  DEPENDS 05_day_harib02e_img
)
add_custom_target(05_day_harib02e_clean
  COMMAND rm -f ${05_day_harib02e_OS}
  COMMAND rm -f ${05_day_harib02e_SYS}
  COMMAND rm -f ${05_day_harib02e_IPLB}
  COMMAND rm -f ${05_day_harib02e_HEADB}
  COMMAND rm -f ${05_day_harib02e_BOOTB}
  COMMAND rm -f ${05_day_harib02e_WILDOBJ}
  COMMAND rm -f ${root_BINARY_DIR}/projects/05_day/harib02e/ipl10.bin
  COMMAND rm -f ${root_BINARY_DIR}/projects/05_day/harib02e/asmhead.bin
  COMMAND rm -f ${root_BINARY_DIR}/projects/05_day/harib02e/*_wine.bin
  COMMAND rm -f ${root_BINARY_DIR}/projects/05_day/harib02e/*.txt
)
add_custom_target(05_day_harib02e_ipl_build
  COMMAND ${RAKUSK_EXECUTABLE} ${05_day_harib02e_IPLS} ${05_day_harib02e_IPLB}
  DEPENDS ${05_day_harib02e_IPLS}
)
add_custom_command(
  OUTPUT ${05_day_harib02e_HEADB}
  COMMAND ${RAKUSK_EXECUTABLE} ${05_day_harib02e_HEADS} ${05_day_harib02e_HEADB}
  DEPENDS ${05_day_harib02e_HEADS}
  COMMENT "Generating asmhead.bin for 05_day_harib02e"
)

add_custom_command(
  OUTPUT ${05_day_harib02e_FUNCO}
  COMMAND ${RAKUSK_EXECUTABLE} ${05_day_harib02e_FUNCS} ${05_day_harib02e_FUNCO}
  DEPENDS ${05_day_harib02e_FUNCS}
  COMMENT "Generating naskfunc.o for 05_day_harib02e"
)
add_custom_command(
  OUTPUT ${05_day_harib02e_FONTO}
  COMMAND ${FONT} ${05_day_harib02e_FONTS} ${05_day_harib02e_FONTB}
  COMMAND ${B2O}  ${05_day_harib02e_FONTB} ${05_day_harib02e_FONTO} _hankaku
  DEPENDS ${05_day_harib02e_FONTS}
  COMMENT "Generating hankaku.o for 05_day_harib02e"
)
add_custom_command(
  OUTPUT ${05_day_harib02e_LIBGC}
  COMMAND ${CONV} -fcoff32 -nu ${05_day_harib02e_LIBGE} ${05_day_harib02e_LIBGC}
  DEPENDS ${05_day_harib02e_LIBGE}
  COMMENT "Converting golibc for 05_day_harib02e"
)
add_custom_target(05_day_harib02e_sys
  COMMAND gcc ${BINOPT} -T ${05_day_harib02e_LDS} ${05_day_harib02e_CCS} ${05_day_harib02e_WILDOBJ} -o ${05_day_harib02e_BOOTB}
  COMMAND cat ${05_day_harib02e_HEADB} ${05_day_harib02e_BOOTB} > ${05_day_harib02e_SYS}
  DEPENDS ${05_day_harib02e_HEADB} 05_day_harib02e_ipl_build
  DEPENDS ${05_day_harib02e_FUNCO}
  DEPENDS ${05_day_harib02e_FONTO} ${05_day_harib02e_LIBGC}
)
add_custom_target(05_day_harib02e_img
  COMMAND mformat -f 1440 -l HARIBOTEOS -N 0xffffffff -C -B ${05_day_harib02e_IPLB} -i ${05_day_harib02e_OS}
  COMMAND mcopy -i ${05_day_harib02e_OS} ${05_day_harib02e_SYS} ::
  DEPENDS 05_day_harib02e_sys
)

########### next target ###############
set(05_day_harib02e_asmhead_SRCS ${root_SOURCE_DIR}/projects/05_day/harib02e/asmhead.nas)
set(05_day_harib02e_asmhead_OUTS ${root_BINARY_DIR}/projects/05_day/harib02e/asmhead.bin)

add_custom_target(05_day_harib02e_asmhead
  COMMAND ${RAKUSK_EXECUTABLE} ${05_day_harib02e_asmhead_SRCS} ${05_day_harib02e_asmhead_OUTS}
  DEPENDS ${05_day_harib02e_asmhead_SRCS}
)

set(05_day_harib02e_asmhead_WINE_OUTS ${root_BINARY_DIR}/projects/05_day/harib02e/asmhead_wine.bin)
add_custom_target(05_day_harib02e_asmhead_wine
  COMMAND ${WINE} ${WINE_NASK} ${05_day_harib02e_asmhead_SRCS} ${05_day_harib02e_asmhead_WINE_OUTS}
)
add_dependencies(wine 05_day_harib02e_asmhead_wine)

add_custom_target(05_day_harib02e_asmhead_od
  COMMAND ${OD} -t x1 ${05_day_harib02e_asmhead_OUTS}      > ${05_day_harib02e_asmhead_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${05_day_harib02e_asmhead_WINE_OUTS} > ${05_day_harib02e_asmhead_WINE_OUTS}_t.txt
  COMMAND diff -s ${05_day_harib02e_asmhead_OUTS}_f.txt ${05_day_harib02e_asmhead_WINE_OUTS}_t.txt
)

add_dependencies(05_day_harib02e_asmhead_od 05_day_harib02e_asmhead)
add_dependencies(05_day_harib02e_asmhead_od 05_day_harib02e_asmhead_wine)
add_dependencies(od 05_day_harib02e_asmhead_od)

#----------------------------------------------------------
########### next target ###############
set(05_day_harib02e_ipl10_SRCS ${root_SOURCE_DIR}/projects/05_day/harib02e/ipl10.nas)
set(05_day_harib02e_ipl10_OUTS ${root_BINARY_DIR}/projects/05_day/harib02e/ipl10.bin)

add_custom_target(05_day_harib02e_ipl10
  COMMAND ${RAKUSK_EXECUTABLE} ${05_day_harib02e_ipl10_SRCS} ${05_day_harib02e_ipl10_OUTS}
  DEPENDS ${05_day_harib02e_ipl10_SRCS}
)

set(05_day_harib02e_ipl10_WINE_OUTS ${root_BINARY_DIR}/projects/05_day/harib02e/ipl10_wine.bin)
add_custom_target(05_day_harib02e_ipl10_wine
  COMMAND ${WINE} ${WINE_NASK} ${05_day_harib02e_ipl10_SRCS} ${05_day_harib02e_ipl10_WINE_OUTS}
)
add_dependencies(wine 05_day_harib02e_ipl10_wine)

add_custom_target(05_day_harib02e_ipl10_od
  COMMAND ${OD} -t x1 ${05_day_harib02e_ipl10_OUTS}      > ${05_day_harib02e_ipl10_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${05_day_harib02e_ipl10_WINE_OUTS} > ${05_day_harib02e_ipl10_WINE_OUTS}_t.txt
  COMMAND diff -s ${05_day_harib02e_ipl10_OUTS}_f.txt ${05_day_harib02e_ipl10_WINE_OUTS}_t.txt
)

add_dependencies(05_day_harib02e_ipl10_od 05_day_harib02e_ipl10)
add_dependencies(05_day_harib02e_ipl10_od 05_day_harib02e_ipl10_wine)
add_dependencies(od 05_day_harib02e_ipl10_od)

#----------------------------------------------------------
########### next target ###############
set(05_day_harib02e_naskfunc_SRCS ${root_SOURCE_DIR}/projects/05_day/harib02e/naskfunc.nas)
set(05_day_harib02e_naskfunc_OUTS ${root_BINARY_DIR}/projects/05_day/harib02e/naskfunc.img)

add_custom_target(05_day_harib02e_naskfunc
  COMMAND ${RAKUSK_EXECUTABLE} ${05_day_harib02e_naskfunc_SRCS} ${05_day_harib02e_naskfunc_OUTS}
  DEPENDS ${05_day_harib02e_naskfunc_SRCS}
)

add_dependencies(images 05_day_harib02e_naskfunc)
set(05_day_harib02e_naskfunc_WINE_OUTS ${root_BINARY_DIR}/projects/05_day/harib02e/naskfunc_wine.img)
add_custom_target(05_day_harib02e_naskfunc_wine
  COMMAND ${WINE} ${WINE_NASK} ${05_day_harib02e_naskfunc_SRCS} ${05_day_harib02e_naskfunc_WINE_OUTS}
)
add_dependencies(wine 05_day_harib02e_naskfunc_wine)

add_custom_target(05_day_harib02e_naskfunc_od
  COMMAND ${OD} -t x1 ${05_day_harib02e_naskfunc_OUTS}      > ${05_day_harib02e_naskfunc_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${05_day_harib02e_naskfunc_WINE_OUTS} > ${05_day_harib02e_naskfunc_WINE_OUTS}_t.txt
  COMMAND diff -s ${05_day_harib02e_naskfunc_OUTS}_f.txt ${05_day_harib02e_naskfunc_WINE_OUTS}_t.txt
)

add_dependencies(05_day_harib02e_naskfunc_od 05_day_harib02e_naskfunc)
add_dependencies(05_day_harib02e_naskfunc_od 05_day_harib02e_naskfunc_wine)
add_dependencies(od 05_day_harib02e_naskfunc_od)

#----------------------------------------------------------
