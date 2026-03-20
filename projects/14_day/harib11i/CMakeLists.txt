#----------------------------------------------------------
message(STATUS "Entering directory projects/14_day/harib11i/")

set(FONT ${root_BINARY_DIR}/src/makefont)
set(B2O  ${root_BINARY_DIR}/src/bin2obj)
set(CONV ${root_BINARY_DIR}/objconv/objconv)
set(14_day_harib11i_WILDOBJ ${root_BINARY_DIR}/projects/14_day/harib11i/*.o)

# Full OS build rules
set(14_day_harib11i_OS    ${root_BINARY_DIR}/projects/14_day/harib11i/os.img)
set(14_day_harib11i_SYS   ${root_BINARY_DIR}/projects/14_day/harib11i/os.sys)
set(14_day_harib11i_IPLB  ${root_BINARY_DIR}/projects/14_day/harib11i/ipl.bin)
set(14_day_harib11i_IPLS  ${root_SOURCE_DIR}/projects/14_day/harib11i/ipl10.nas)
set(14_day_harib11i_HEADB ${root_BINARY_DIR}/projects/14_day/harib11i/asmhead.bin)
set(14_day_harib11i_HEADS ${root_SOURCE_DIR}/projects/14_day/harib11i/asmhead.nas)
set(14_day_harib11i_BOOTB ${root_BINARY_DIR}/projects/14_day/harib11i/boot.bin)
set(14_day_harib11i_CCO   ${root_BINARY_DIR}/projects/14_day/harib11i/boot.o)
set(14_day_harib11i_CCS   ${root_SOURCE_DIR}/projects/14_day/harib11i/*.c)
set(14_day_harib11i_LDS   ${root_SOURCE_DIR}/projects/os.lds)
set(14_day_harib11i_FUNCS ${root_SOURCE_DIR}/projects/14_day/harib11i/naskfunc.nas)
set(14_day_harib11i_FUNCO ${root_BINARY_DIR}/projects/14_day/harib11i/naskfunc.o)
set(14_day_harib11i_FONTS ${root_SOURCE_DIR}/projects/14_day/harib11i/hankaku.txt)
set(14_day_harib11i_FONTB ${root_BINARY_DIR}/projects/14_day/harib11i/hankaku.bin)
set(14_day_harib11i_FONTO ${root_BINARY_DIR}/projects/14_day/harib11i/hankaku.o)
set(14_day_harib11i_LIBGE ${root_BINARY_DIR}/golibc/libgo.a)
set(14_day_harib11i_LIBGC ${root_BINARY_DIR}/projects/14_day/harib11i/libgo.o)

add_custom_target(14_day_harib11i_run
  COMMAND ${QEMU} ${QEMUOPT} ${14_day_harib11i_OS}
  DEPENDS 14_day_harib11i_img
)
add_custom_target(14_day_harib11i_debug
  COMMAND ${QEMU} -s -S ${QEMUOPT} ${14_day_harib11i_OS} ${QEMU_DEBUG_OPT}
  DEPENDS 14_day_harib11i_img
)
add_custom_target(14_day_harib11i_clean
  COMMAND rm -f ${14_day_harib11i_OS}
  COMMAND rm -f ${14_day_harib11i_SYS}
  COMMAND rm -f ${14_day_harib11i_IPLB}
  COMMAND rm -f ${14_day_harib11i_HEADB}
  COMMAND rm -f ${14_day_harib11i_BOOTB}
  COMMAND rm -f ${14_day_harib11i_WILDOBJ}
  COMMAND rm -f ${root_BINARY_DIR}/projects/14_day/harib11i/ipl10.bin
  COMMAND rm -f ${root_BINARY_DIR}/projects/14_day/harib11i/asmhead.bin
  COMMAND rm -f ${root_BINARY_DIR}/projects/14_day/harib11i/*_wine.bin
  COMMAND rm -f ${root_BINARY_DIR}/projects/14_day/harib11i/*.txt
)
add_custom_target(14_day_harib11i_ipl_build
  COMMAND ${RAKUSK_EXECUTABLE} ${14_day_harib11i_IPLS} ${14_day_harib11i_IPLB}
  DEPENDS ${14_day_harib11i_IPLS}
)
add_custom_command(
  OUTPUT ${14_day_harib11i_HEADB}
  COMMAND ${RAKUSK_EXECUTABLE} ${14_day_harib11i_HEADS} ${14_day_harib11i_HEADB}
  DEPENDS ${14_day_harib11i_HEADS}
  COMMENT "Generating asmhead.bin for 14_day_harib11i"
)

add_custom_command(
  OUTPUT ${14_day_harib11i_FUNCO}
  COMMAND ${RAKUSK_EXECUTABLE} ${14_day_harib11i_FUNCS} ${14_day_harib11i_FUNCO}
  DEPENDS ${14_day_harib11i_FUNCS}
  COMMENT "Generating naskfunc.o for 14_day_harib11i"
)
add_custom_command(
  OUTPUT ${14_day_harib11i_FONTO}
  COMMAND ${FONT} ${14_day_harib11i_FONTS} ${14_day_harib11i_FONTB}
  COMMAND ${B2O}  ${14_day_harib11i_FONTB} ${14_day_harib11i_FONTO} _hankaku
  DEPENDS ${14_day_harib11i_FONTS}
  COMMENT "Generating hankaku.o for 14_day_harib11i"
)
add_custom_command(
  OUTPUT ${14_day_harib11i_LIBGC}
  COMMAND ${CONV} -fcoff32 -nu ${14_day_harib11i_LIBGE} ${14_day_harib11i_LIBGC}
  DEPENDS ${14_day_harib11i_LIBGE}
  COMMENT "Converting golibc for 14_day_harib11i"
)
add_custom_target(14_day_harib11i_sys
  COMMAND gcc ${BINOPT} -T ${14_day_harib11i_LDS} ${14_day_harib11i_CCS} ${14_day_harib11i_WILDOBJ} -o ${14_day_harib11i_BOOTB}
  COMMAND cat ${14_day_harib11i_HEADB} ${14_day_harib11i_BOOTB} > ${14_day_harib11i_SYS}
  DEPENDS ${14_day_harib11i_HEADB} 14_day_harib11i_ipl_build
  DEPENDS ${14_day_harib11i_FUNCO}
  DEPENDS ${14_day_harib11i_FONTO} ${14_day_harib11i_LIBGC}
)
add_custom_target(14_day_harib11i_img
  COMMAND mformat -f 1440 -l HARIBOTEOS -N 0xffffffff -C -B ${14_day_harib11i_IPLB} -i ${14_day_harib11i_OS}
  COMMAND mcopy -i ${14_day_harib11i_OS} ${14_day_harib11i_SYS} ::
  DEPENDS 14_day_harib11i_sys
)

########### next target ###############
set(14_day_harib11i_asmhead_SRCS ${root_SOURCE_DIR}/projects/14_day/harib11i/asmhead.nas)
set(14_day_harib11i_asmhead_OUTS ${root_BINARY_DIR}/projects/14_day/harib11i/asmhead.bin)

add_custom_target(14_day_harib11i_asmhead
  COMMAND ${RAKUSK_EXECUTABLE} ${14_day_harib11i_asmhead_SRCS} ${14_day_harib11i_asmhead_OUTS}
  DEPENDS ${14_day_harib11i_asmhead_SRCS}
)

set(14_day_harib11i_asmhead_WINE_OUTS ${root_BINARY_DIR}/projects/14_day/harib11i/asmhead_wine.bin)
add_custom_target(14_day_harib11i_asmhead_wine
  COMMAND ${WINE} ${WINE_NASK} ${14_day_harib11i_asmhead_SRCS} ${14_day_harib11i_asmhead_WINE_OUTS}
)
add_dependencies(wine 14_day_harib11i_asmhead_wine)

add_custom_target(14_day_harib11i_asmhead_od
  COMMAND ${OD} -t x1 ${14_day_harib11i_asmhead_OUTS}      > ${14_day_harib11i_asmhead_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${14_day_harib11i_asmhead_WINE_OUTS} > ${14_day_harib11i_asmhead_WINE_OUTS}_t.txt
  COMMAND diff -s ${14_day_harib11i_asmhead_OUTS}_f.txt ${14_day_harib11i_asmhead_WINE_OUTS}_t.txt
)

add_dependencies(14_day_harib11i_asmhead_od 14_day_harib11i_asmhead)
add_dependencies(14_day_harib11i_asmhead_od 14_day_harib11i_asmhead_wine)
add_dependencies(od 14_day_harib11i_asmhead_od)

#----------------------------------------------------------
########### next target ###############
set(14_day_harib11i_ipl10_SRCS ${root_SOURCE_DIR}/projects/14_day/harib11i/ipl10.nas)
set(14_day_harib11i_ipl10_OUTS ${root_BINARY_DIR}/projects/14_day/harib11i/ipl10.bin)

add_custom_target(14_day_harib11i_ipl10
  COMMAND ${RAKUSK_EXECUTABLE} ${14_day_harib11i_ipl10_SRCS} ${14_day_harib11i_ipl10_OUTS}
  DEPENDS ${14_day_harib11i_ipl10_SRCS}
)

set(14_day_harib11i_ipl10_WINE_OUTS ${root_BINARY_DIR}/projects/14_day/harib11i/ipl10_wine.bin)
add_custom_target(14_day_harib11i_ipl10_wine
  COMMAND ${WINE} ${WINE_NASK} ${14_day_harib11i_ipl10_SRCS} ${14_day_harib11i_ipl10_WINE_OUTS}
)
add_dependencies(wine 14_day_harib11i_ipl10_wine)

add_custom_target(14_day_harib11i_ipl10_od
  COMMAND ${OD} -t x1 ${14_day_harib11i_ipl10_OUTS}      > ${14_day_harib11i_ipl10_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${14_day_harib11i_ipl10_WINE_OUTS} > ${14_day_harib11i_ipl10_WINE_OUTS}_t.txt
  COMMAND diff -s ${14_day_harib11i_ipl10_OUTS}_f.txt ${14_day_harib11i_ipl10_WINE_OUTS}_t.txt
)

add_dependencies(14_day_harib11i_ipl10_od 14_day_harib11i_ipl10)
add_dependencies(14_day_harib11i_ipl10_od 14_day_harib11i_ipl10_wine)
add_dependencies(od 14_day_harib11i_ipl10_od)

#----------------------------------------------------------
########### next target ###############
set(14_day_harib11i_naskfunc_SRCS ${root_SOURCE_DIR}/projects/14_day/harib11i/naskfunc.nas)
set(14_day_harib11i_naskfunc_OUTS ${root_BINARY_DIR}/projects/14_day/harib11i/naskfunc.img)

add_custom_target(14_day_harib11i_naskfunc
  COMMAND ${RAKUSK_EXECUTABLE} ${14_day_harib11i_naskfunc_SRCS} ${14_day_harib11i_naskfunc_OUTS}
  DEPENDS ${14_day_harib11i_naskfunc_SRCS}
)

add_dependencies(images 14_day_harib11i_naskfunc)
set(14_day_harib11i_naskfunc_WINE_OUTS ${root_BINARY_DIR}/projects/14_day/harib11i/naskfunc_wine.img)
add_custom_target(14_day_harib11i_naskfunc_wine
  COMMAND ${WINE} ${WINE_NASK} ${14_day_harib11i_naskfunc_SRCS} ${14_day_harib11i_naskfunc_WINE_OUTS}
)
add_dependencies(wine 14_day_harib11i_naskfunc_wine)

add_custom_target(14_day_harib11i_naskfunc_od
  COMMAND ${OD} -t x1 ${14_day_harib11i_naskfunc_OUTS}      > ${14_day_harib11i_naskfunc_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${14_day_harib11i_naskfunc_WINE_OUTS} > ${14_day_harib11i_naskfunc_WINE_OUTS}_t.txt
  COMMAND diff -s ${14_day_harib11i_naskfunc_OUTS}_f.txt ${14_day_harib11i_naskfunc_WINE_OUTS}_t.txt
)

add_dependencies(14_day_harib11i_naskfunc_od 14_day_harib11i_naskfunc)
add_dependencies(14_day_harib11i_naskfunc_od 14_day_harib11i_naskfunc_wine)
add_dependencies(od 14_day_harib11i_naskfunc_od)

#----------------------------------------------------------
