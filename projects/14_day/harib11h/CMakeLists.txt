#----------------------------------------------------------
message(STATUS "Entering directory projects/14_day/harib11h/")

set(FONT ${root_BINARY_DIR}/src/makefont)
set(B2O  ${root_BINARY_DIR}/src/bin2obj)
set(CONV ${root_BINARY_DIR}/objconv/objconv)
set(14_day_harib11h_WILDOBJ ${root_BINARY_DIR}/projects/14_day/harib11h/*.o)

# Full OS build rules
set(14_day_harib11h_OS    ${root_BINARY_DIR}/projects/14_day/harib11h/os.img)
set(14_day_harib11h_SYS   ${root_BINARY_DIR}/projects/14_day/harib11h/os.sys)
set(14_day_harib11h_IPLB  ${root_BINARY_DIR}/projects/14_day/harib11h/ipl.bin)
set(14_day_harib11h_IPLS  ${root_SOURCE_DIR}/projects/14_day/harib11h/ipl10.nas)
set(14_day_harib11h_HEADB ${root_BINARY_DIR}/projects/14_day/harib11h/asmhead.bin)
set(14_day_harib11h_HEADS ${root_SOURCE_DIR}/projects/14_day/harib11h/asmhead.nas)
set(14_day_harib11h_BOOTB ${root_BINARY_DIR}/projects/14_day/harib11h/boot.bin)
set(14_day_harib11h_CCO   ${root_BINARY_DIR}/projects/14_day/harib11h/boot.o)
set(14_day_harib11h_CCS   ${root_SOURCE_DIR}/projects/14_day/harib11h/*.c)
set(14_day_harib11h_LDS   ${root_SOURCE_DIR}/projects/os.lds)
set(14_day_harib11h_FUNCS ${root_SOURCE_DIR}/projects/14_day/harib11h/naskfunc.nas)
set(14_day_harib11h_FUNCO ${root_BINARY_DIR}/projects/14_day/harib11h/naskfunc.o)
set(14_day_harib11h_FONTS ${root_SOURCE_DIR}/projects/14_day/harib11h/hankaku.txt)
set(14_day_harib11h_FONTB ${root_BINARY_DIR}/projects/14_day/harib11h/hankaku.bin)
set(14_day_harib11h_FONTO ${root_BINARY_DIR}/projects/14_day/harib11h/hankaku.o)
set(14_day_harib11h_LIBGE ${root_BINARY_DIR}/golibc/libgo.a)
set(14_day_harib11h_LIBGC ${root_BINARY_DIR}/projects/14_day/harib11h/libgo.o)

add_custom_target(14_day_harib11h_run
  COMMAND ${QEMU} ${QEMUOPT} ${14_day_harib11h_OS}
  DEPENDS 14_day_harib11h_img
)
add_custom_target(14_day_harib11h_debug
  COMMAND ${QEMU} -s -S ${QEMUOPT} ${14_day_harib11h_OS} ${QEMU_DEBUG_OPT}
  DEPENDS 14_day_harib11h_img
)
add_custom_target(14_day_harib11h_clean
  COMMAND rm -f ${14_day_harib11h_OS}
  COMMAND rm -f ${14_day_harib11h_SYS}
  COMMAND rm -f ${14_day_harib11h_IPLB}
  COMMAND rm -f ${14_day_harib11h_HEADB}
  COMMAND rm -f ${14_day_harib11h_BOOTB}
  COMMAND rm -f ${14_day_harib11h_WILDOBJ}
  COMMAND rm -f ${root_BINARY_DIR}/projects/14_day/harib11h/ipl10.bin
  COMMAND rm -f ${root_BINARY_DIR}/projects/14_day/harib11h/asmhead.bin
  COMMAND rm -f ${root_BINARY_DIR}/projects/14_day/harib11h/*_wine.bin
  COMMAND rm -f ${root_BINARY_DIR}/projects/14_day/harib11h/*.txt
)
add_custom_target(14_day_harib11h_ipl_build
  COMMAND ${RAKUSK_EXECUTABLE} ${14_day_harib11h_IPLS} ${14_day_harib11h_IPLB}
  DEPENDS ${14_day_harib11h_IPLS}
)
add_custom_command(
  OUTPUT ${14_day_harib11h_HEADB}
  COMMAND ${RAKUSK_EXECUTABLE} ${14_day_harib11h_HEADS} ${14_day_harib11h_HEADB}
  DEPENDS ${14_day_harib11h_HEADS}
  COMMENT "Generating asmhead.bin for 14_day_harib11h"
)

add_custom_command(
  OUTPUT ${14_day_harib11h_FUNCO}
  COMMAND ${RAKUSK_EXECUTABLE} ${14_day_harib11h_FUNCS} ${14_day_harib11h_FUNCO}
  DEPENDS ${14_day_harib11h_FUNCS}
  COMMENT "Generating naskfunc.o for 14_day_harib11h"
)
add_custom_command(
  OUTPUT ${14_day_harib11h_FONTO}
  COMMAND ${FONT} ${14_day_harib11h_FONTS} ${14_day_harib11h_FONTB}
  COMMAND ${B2O}  ${14_day_harib11h_FONTB} ${14_day_harib11h_FONTO} _hankaku
  DEPENDS ${14_day_harib11h_FONTS}
  COMMENT "Generating hankaku.o for 14_day_harib11h"
)
add_custom_command(
  OUTPUT ${14_day_harib11h_LIBGC}
  COMMAND ${CONV} -fcoff32 -nu ${14_day_harib11h_LIBGE} ${14_day_harib11h_LIBGC}
  DEPENDS ${14_day_harib11h_LIBGE}
  COMMENT "Converting golibc for 14_day_harib11h"
)
add_custom_target(14_day_harib11h_sys
  COMMAND gcc ${BINOPT} -T ${14_day_harib11h_LDS} ${14_day_harib11h_CCS} ${14_day_harib11h_WILDOBJ} -o ${14_day_harib11h_BOOTB}
  COMMAND cat ${14_day_harib11h_HEADB} ${14_day_harib11h_BOOTB} > ${14_day_harib11h_SYS}
  DEPENDS ${14_day_harib11h_HEADB} 14_day_harib11h_ipl_build
  DEPENDS ${14_day_harib11h_FUNCO}
  DEPENDS ${14_day_harib11h_FONTO} ${14_day_harib11h_LIBGC}
)
add_custom_target(14_day_harib11h_img
  COMMAND mformat -f 1440 -l HARIBOTEOS -N 0xffffffff -C -B ${14_day_harib11h_IPLB} -i ${14_day_harib11h_OS}
  COMMAND mcopy -i ${14_day_harib11h_OS} ${14_day_harib11h_SYS} ::
  DEPENDS 14_day_harib11h_sys
)

########### next target ###############
set(14_day_harib11h_asmhead_SRCS ${root_SOURCE_DIR}/projects/14_day/harib11h/asmhead.nas)
set(14_day_harib11h_asmhead_OUTS ${root_BINARY_DIR}/projects/14_day/harib11h/asmhead.bin)

add_custom_target(14_day_harib11h_asmhead
  COMMAND ${RAKUSK_EXECUTABLE} ${14_day_harib11h_asmhead_SRCS} ${14_day_harib11h_asmhead_OUTS}
  DEPENDS ${14_day_harib11h_asmhead_SRCS}
)

set(14_day_harib11h_asmhead_WINE_OUTS ${root_BINARY_DIR}/projects/14_day/harib11h/asmhead_wine.bin)
add_custom_target(14_day_harib11h_asmhead_wine
  COMMAND ${WINE} ${WINE_NASK} ${14_day_harib11h_asmhead_SRCS} ${14_day_harib11h_asmhead_WINE_OUTS}
)
add_dependencies(wine 14_day_harib11h_asmhead_wine)

add_custom_target(14_day_harib11h_asmhead_od
  COMMAND ${OD} -t x1 ${14_day_harib11h_asmhead_OUTS}      > ${14_day_harib11h_asmhead_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${14_day_harib11h_asmhead_WINE_OUTS} > ${14_day_harib11h_asmhead_WINE_OUTS}_t.txt
  COMMAND diff -s ${14_day_harib11h_asmhead_OUTS}_f.txt ${14_day_harib11h_asmhead_WINE_OUTS}_t.txt
)

add_dependencies(14_day_harib11h_asmhead_od 14_day_harib11h_asmhead)
add_dependencies(14_day_harib11h_asmhead_od 14_day_harib11h_asmhead_wine)
add_dependencies(od 14_day_harib11h_asmhead_od)

#----------------------------------------------------------
########### next target ###############
set(14_day_harib11h_ipl10_SRCS ${root_SOURCE_DIR}/projects/14_day/harib11h/ipl10.nas)
set(14_day_harib11h_ipl10_OUTS ${root_BINARY_DIR}/projects/14_day/harib11h/ipl10.bin)

add_custom_target(14_day_harib11h_ipl10
  COMMAND ${RAKUSK_EXECUTABLE} ${14_day_harib11h_ipl10_SRCS} ${14_day_harib11h_ipl10_OUTS}
  DEPENDS ${14_day_harib11h_ipl10_SRCS}
)

set(14_day_harib11h_ipl10_WINE_OUTS ${root_BINARY_DIR}/projects/14_day/harib11h/ipl10_wine.bin)
add_custom_target(14_day_harib11h_ipl10_wine
  COMMAND ${WINE} ${WINE_NASK} ${14_day_harib11h_ipl10_SRCS} ${14_day_harib11h_ipl10_WINE_OUTS}
)
add_dependencies(wine 14_day_harib11h_ipl10_wine)

add_custom_target(14_day_harib11h_ipl10_od
  COMMAND ${OD} -t x1 ${14_day_harib11h_ipl10_OUTS}      > ${14_day_harib11h_ipl10_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${14_day_harib11h_ipl10_WINE_OUTS} > ${14_day_harib11h_ipl10_WINE_OUTS}_t.txt
  COMMAND diff -s ${14_day_harib11h_ipl10_OUTS}_f.txt ${14_day_harib11h_ipl10_WINE_OUTS}_t.txt
)

add_dependencies(14_day_harib11h_ipl10_od 14_day_harib11h_ipl10)
add_dependencies(14_day_harib11h_ipl10_od 14_day_harib11h_ipl10_wine)
add_dependencies(od 14_day_harib11h_ipl10_od)

#----------------------------------------------------------
########### next target ###############
set(14_day_harib11h_naskfunc_SRCS ${root_SOURCE_DIR}/projects/14_day/harib11h/naskfunc.nas)
set(14_day_harib11h_naskfunc_OUTS ${root_BINARY_DIR}/projects/14_day/harib11h/naskfunc.img)

add_custom_target(14_day_harib11h_naskfunc
  COMMAND ${RAKUSK_EXECUTABLE} ${14_day_harib11h_naskfunc_SRCS} ${14_day_harib11h_naskfunc_OUTS}
  DEPENDS ${14_day_harib11h_naskfunc_SRCS}
)

add_dependencies(images 14_day_harib11h_naskfunc)
set(14_day_harib11h_naskfunc_WINE_OUTS ${root_BINARY_DIR}/projects/14_day/harib11h/naskfunc_wine.img)
add_custom_target(14_day_harib11h_naskfunc_wine
  COMMAND ${WINE} ${WINE_NASK} ${14_day_harib11h_naskfunc_SRCS} ${14_day_harib11h_naskfunc_WINE_OUTS}
)
add_dependencies(wine 14_day_harib11h_naskfunc_wine)

add_custom_target(14_day_harib11h_naskfunc_od
  COMMAND ${OD} -t x1 ${14_day_harib11h_naskfunc_OUTS}      > ${14_day_harib11h_naskfunc_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${14_day_harib11h_naskfunc_WINE_OUTS} > ${14_day_harib11h_naskfunc_WINE_OUTS}_t.txt
  COMMAND diff -s ${14_day_harib11h_naskfunc_OUTS}_f.txt ${14_day_harib11h_naskfunc_WINE_OUTS}_t.txt
)

add_dependencies(14_day_harib11h_naskfunc_od 14_day_harib11h_naskfunc)
add_dependencies(14_day_harib11h_naskfunc_od 14_day_harib11h_naskfunc_wine)
add_dependencies(od 14_day_harib11h_naskfunc_od)

#----------------------------------------------------------
