#----------------------------------------------------------
message(STATUS "Entering directory projects/18_day/harib15e/")

set(FONT ${root_BINARY_DIR}/src/makefont)
set(B2O  ${root_BINARY_DIR}/src/bin2obj)
set(CONV ${root_BINARY_DIR}/objconv/objconv)
set(18_day_harib15e_WILDOBJ ${root_BINARY_DIR}/projects/18_day/harib15e/*.o)

# Full OS build rules
set(18_day_harib15e_OS    ${root_BINARY_DIR}/projects/18_day/harib15e/os.img)
set(18_day_harib15e_SYS   ${root_BINARY_DIR}/projects/18_day/harib15e/os.sys)
set(18_day_harib15e_IPLB  ${root_BINARY_DIR}/projects/18_day/harib15e/ipl.bin)
set(18_day_harib15e_IPLS  ${root_SOURCE_DIR}/projects/18_day/harib15e/ipl10.nas)
set(18_day_harib15e_HEADB ${root_BINARY_DIR}/projects/18_day/harib15e/asmhead.bin)
set(18_day_harib15e_HEADS ${root_SOURCE_DIR}/projects/18_day/harib15e/asmhead.nas)
set(18_day_harib15e_BOOTB ${root_BINARY_DIR}/projects/18_day/harib15e/boot.bin)
set(18_day_harib15e_CCO   ${root_BINARY_DIR}/projects/18_day/harib15e/boot.o)
set(18_day_harib15e_CCS   ${root_SOURCE_DIR}/projects/18_day/harib15e/*.c)
set(18_day_harib15e_LDS   ${root_SOURCE_DIR}/projects/os.lds)
set(18_day_harib15e_FUNCS ${root_SOURCE_DIR}/projects/18_day/harib15e/naskfunc.nas)
set(18_day_harib15e_FUNCO ${root_BINARY_DIR}/projects/18_day/harib15e/naskfunc.o)
set(18_day_harib15e_FONTS ${root_SOURCE_DIR}/projects/18_day/harib15e/hankaku.txt)
set(18_day_harib15e_FONTB ${root_BINARY_DIR}/projects/18_day/harib15e/hankaku.bin)
set(18_day_harib15e_FONTO ${root_BINARY_DIR}/projects/18_day/harib15e/hankaku.o)
set(18_day_harib15e_LIBGE ${root_BINARY_DIR}/golibc/libgo.a)
set(18_day_harib15e_LIBGC ${root_BINARY_DIR}/projects/18_day/harib15e/libgo.o)

add_custom_target(18_day_harib15e_run
  COMMAND ${QEMU} ${QEMUOPT} ${18_day_harib15e_OS}
  DEPENDS 18_day_harib15e_img
)
add_custom_target(18_day_harib15e_debug
  COMMAND ${QEMU} -s -S ${QEMUOPT} ${18_day_harib15e_OS} ${QEMU_DEBUG_OPT}
  DEPENDS 18_day_harib15e_img
)
add_custom_target(18_day_harib15e_clean
  COMMAND rm -f ${18_day_harib15e_OS}
  COMMAND rm -f ${18_day_harib15e_SYS}
  COMMAND rm -f ${18_day_harib15e_IPLB}
  COMMAND rm -f ${18_day_harib15e_HEADB}
  COMMAND rm -f ${18_day_harib15e_BOOTB}
  COMMAND rm -f ${18_day_harib15e_WILDOBJ}
  COMMAND rm -f ${root_BINARY_DIR}/projects/18_day/harib15e/ipl10.bin
  COMMAND rm -f ${root_BINARY_DIR}/projects/18_day/harib15e/asmhead.bin
  COMMAND rm -f ${root_BINARY_DIR}/projects/18_day/harib15e/*_wine.bin
  COMMAND rm -f ${root_BINARY_DIR}/projects/18_day/harib15e/*.txt
)
add_custom_target(18_day_harib15e_ipl_build
  COMMAND ${RAKUSK_EXECUTABLE} ${18_day_harib15e_IPLS} ${18_day_harib15e_IPLB}
  DEPENDS ${18_day_harib15e_IPLS}
)
add_custom_command(
  OUTPUT ${18_day_harib15e_HEADB}
  COMMAND ${RAKUSK_EXECUTABLE} ${18_day_harib15e_HEADS} ${18_day_harib15e_HEADB}
  DEPENDS ${18_day_harib15e_HEADS}
  COMMENT "Generating asmhead.bin for 18_day_harib15e"
)

add_custom_command(
  OUTPUT ${18_day_harib15e_FUNCO}
  COMMAND ${RAKUSK_EXECUTABLE} ${18_day_harib15e_FUNCS} ${18_day_harib15e_FUNCO}
  DEPENDS ${18_day_harib15e_FUNCS}
  COMMENT "Generating naskfunc.o for 18_day_harib15e"
)
add_custom_command(
  OUTPUT ${18_day_harib15e_FONTO}
  COMMAND ${FONT} ${18_day_harib15e_FONTS} ${18_day_harib15e_FONTB}
  COMMAND ${B2O}  ${18_day_harib15e_FONTB} ${18_day_harib15e_FONTO} _hankaku
  DEPENDS ${18_day_harib15e_FONTS}
  COMMENT "Generating hankaku.o for 18_day_harib15e"
)
add_custom_command(
  OUTPUT ${18_day_harib15e_LIBGC}
  COMMAND ${CONV} -fcoff32 -nu ${18_day_harib15e_LIBGE} ${18_day_harib15e_LIBGC}
  DEPENDS ${18_day_harib15e_LIBGE}
  COMMENT "Converting golibc for 18_day_harib15e"
)
add_custom_target(18_day_harib15e_sys
  COMMAND gcc ${BINOPT} -T ${18_day_harib15e_LDS} ${18_day_harib15e_CCS} ${18_day_harib15e_WILDOBJ} -o ${18_day_harib15e_BOOTB}
  COMMAND cat ${18_day_harib15e_HEADB} ${18_day_harib15e_BOOTB} > ${18_day_harib15e_SYS}
  DEPENDS ${18_day_harib15e_HEADB} 18_day_harib15e_ipl_build
  DEPENDS ${18_day_harib15e_FUNCO}
  DEPENDS ${18_day_harib15e_FONTO} ${18_day_harib15e_LIBGC}
)
add_custom_target(18_day_harib15e_img
  COMMAND mformat -f 1440 -l HARIBOTEOS -N 0xffffffff -C -B ${18_day_harib15e_IPLB} -i ${18_day_harib15e_OS}
  COMMAND mcopy -i ${18_day_harib15e_OS} ${18_day_harib15e_SYS} ::
  DEPENDS 18_day_harib15e_sys
)

########### next target ###############
set(18_day_harib15e_asmhead_SRCS ${root_SOURCE_DIR}/projects/18_day/harib15e/asmhead.nas)
set(18_day_harib15e_asmhead_OUTS ${root_BINARY_DIR}/projects/18_day/harib15e/asmhead.bin)

add_custom_target(18_day_harib15e_asmhead
  COMMAND ${RAKUSK_EXECUTABLE} ${18_day_harib15e_asmhead_SRCS} ${18_day_harib15e_asmhead_OUTS}
  DEPENDS ${18_day_harib15e_asmhead_SRCS}
)

set(18_day_harib15e_asmhead_WINE_OUTS ${root_BINARY_DIR}/projects/18_day/harib15e/asmhead_wine.bin)
add_custom_target(18_day_harib15e_asmhead_wine
  COMMAND ${WINE} ${WINE_NASK} ${18_day_harib15e_asmhead_SRCS} ${18_day_harib15e_asmhead_WINE_OUTS}
)
add_dependencies(wine 18_day_harib15e_asmhead_wine)

add_custom_target(18_day_harib15e_asmhead_od
  COMMAND ${OD} -t x1 ${18_day_harib15e_asmhead_OUTS}      > ${18_day_harib15e_asmhead_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${18_day_harib15e_asmhead_WINE_OUTS} > ${18_day_harib15e_asmhead_WINE_OUTS}_t.txt
  COMMAND diff -s ${18_day_harib15e_asmhead_OUTS}_f.txt ${18_day_harib15e_asmhead_WINE_OUTS}_t.txt
)

add_dependencies(18_day_harib15e_asmhead_od 18_day_harib15e_asmhead)
add_dependencies(18_day_harib15e_asmhead_od 18_day_harib15e_asmhead_wine)
add_dependencies(od 18_day_harib15e_asmhead_od)

#----------------------------------------------------------
########### next target ###############
set(18_day_harib15e_ipl10_SRCS ${root_SOURCE_DIR}/projects/18_day/harib15e/ipl10.nas)
set(18_day_harib15e_ipl10_OUTS ${root_BINARY_DIR}/projects/18_day/harib15e/ipl10.bin)

add_custom_target(18_day_harib15e_ipl10
  COMMAND ${RAKUSK_EXECUTABLE} ${18_day_harib15e_ipl10_SRCS} ${18_day_harib15e_ipl10_OUTS}
  DEPENDS ${18_day_harib15e_ipl10_SRCS}
)

set(18_day_harib15e_ipl10_WINE_OUTS ${root_BINARY_DIR}/projects/18_day/harib15e/ipl10_wine.bin)
add_custom_target(18_day_harib15e_ipl10_wine
  COMMAND ${WINE} ${WINE_NASK} ${18_day_harib15e_ipl10_SRCS} ${18_day_harib15e_ipl10_WINE_OUTS}
)
add_dependencies(wine 18_day_harib15e_ipl10_wine)

add_custom_target(18_day_harib15e_ipl10_od
  COMMAND ${OD} -t x1 ${18_day_harib15e_ipl10_OUTS}      > ${18_day_harib15e_ipl10_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${18_day_harib15e_ipl10_WINE_OUTS} > ${18_day_harib15e_ipl10_WINE_OUTS}_t.txt
  COMMAND diff -s ${18_day_harib15e_ipl10_OUTS}_f.txt ${18_day_harib15e_ipl10_WINE_OUTS}_t.txt
)

add_dependencies(18_day_harib15e_ipl10_od 18_day_harib15e_ipl10)
add_dependencies(18_day_harib15e_ipl10_od 18_day_harib15e_ipl10_wine)
add_dependencies(od 18_day_harib15e_ipl10_od)

#----------------------------------------------------------
########### next target ###############
set(18_day_harib15e_naskfunc_SRCS ${root_SOURCE_DIR}/projects/18_day/harib15e/naskfunc.nas)
set(18_day_harib15e_naskfunc_OUTS ${root_BINARY_DIR}/projects/18_day/harib15e/naskfunc.img)

add_custom_target(18_day_harib15e_naskfunc
  COMMAND ${RAKUSK_EXECUTABLE} ${18_day_harib15e_naskfunc_SRCS} ${18_day_harib15e_naskfunc_OUTS}
  DEPENDS ${18_day_harib15e_naskfunc_SRCS}
)

add_dependencies(images 18_day_harib15e_naskfunc)
set(18_day_harib15e_naskfunc_WINE_OUTS ${root_BINARY_DIR}/projects/18_day/harib15e/naskfunc_wine.img)
add_custom_target(18_day_harib15e_naskfunc_wine
  COMMAND ${WINE} ${WINE_NASK} ${18_day_harib15e_naskfunc_SRCS} ${18_day_harib15e_naskfunc_WINE_OUTS}
)
add_dependencies(wine 18_day_harib15e_naskfunc_wine)

add_custom_target(18_day_harib15e_naskfunc_od
  COMMAND ${OD} -t x1 ${18_day_harib15e_naskfunc_OUTS}      > ${18_day_harib15e_naskfunc_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${18_day_harib15e_naskfunc_WINE_OUTS} > ${18_day_harib15e_naskfunc_WINE_OUTS}_t.txt
  COMMAND diff -s ${18_day_harib15e_naskfunc_OUTS}_f.txt ${18_day_harib15e_naskfunc_WINE_OUTS}_t.txt
)

add_dependencies(18_day_harib15e_naskfunc_od 18_day_harib15e_naskfunc)
add_dependencies(18_day_harib15e_naskfunc_od 18_day_harib15e_naskfunc_wine)
add_dependencies(od 18_day_harib15e_naskfunc_od)

#----------------------------------------------------------
