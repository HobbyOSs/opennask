#----------------------------------------------------------
message(STATUS "Entering directory projects/13_day/harib10i/")

set(FONT ${root_BINARY_DIR}/src/makefont)
set(B2O  ${root_BINARY_DIR}/src/bin2obj)
set(CONV ${root_BINARY_DIR}/objconv/objconv)
set(13_day_harib10i_WILDOBJ ${root_BINARY_DIR}/projects/13_day/harib10i/*.o)

# Full OS build rules
set(13_day_harib10i_OS    ${root_BINARY_DIR}/projects/13_day/harib10i/os.img)
set(13_day_harib10i_SYS   ${root_BINARY_DIR}/projects/13_day/harib10i/os.sys)
set(13_day_harib10i_IPLB  ${root_BINARY_DIR}/projects/13_day/harib10i/ipl.bin)
set(13_day_harib10i_IPLS  ${root_SOURCE_DIR}/projects/13_day/harib10i/ipl10.nas)
set(13_day_harib10i_HEADB ${root_BINARY_DIR}/projects/13_day/harib10i/asmhead.bin)
set(13_day_harib10i_HEADS ${root_SOURCE_DIR}/projects/13_day/harib10i/asmhead.nas)
set(13_day_harib10i_BOOTB ${root_BINARY_DIR}/projects/13_day/harib10i/boot.bin)
set(13_day_harib10i_CCO   ${root_BINARY_DIR}/projects/13_day/harib10i/boot.o)
set(13_day_harib10i_CCS   ${root_SOURCE_DIR}/projects/13_day/harib10i/*.c)
set(13_day_harib10i_LDS   ${root_SOURCE_DIR}/projects/os.lds)
set(13_day_harib10i_FUNCS ${root_SOURCE_DIR}/projects/13_day/harib10i/naskfunc.nas)
set(13_day_harib10i_FUNCO ${root_BINARY_DIR}/projects/13_day/harib10i/naskfunc.o)
set(13_day_harib10i_FONTS ${root_SOURCE_DIR}/projects/13_day/harib10i/hankaku.txt)
set(13_day_harib10i_FONTB ${root_BINARY_DIR}/projects/13_day/harib10i/hankaku.bin)
set(13_day_harib10i_FONTO ${root_BINARY_DIR}/projects/13_day/harib10i/hankaku.o)
set(13_day_harib10i_LIBGE ${root_BINARY_DIR}/golibc/libgo.a)
set(13_day_harib10i_LIBGC ${root_BINARY_DIR}/projects/13_day/harib10i/libgo.o)

add_custom_target(13_day_harib10i_run
  COMMAND ${QEMU} ${QEMUOPT} ${13_day_harib10i_OS}
  DEPENDS 13_day_harib10i_img
)
add_custom_target(13_day_harib10i_debug
  COMMAND ${QEMU} -s -S ${QEMUOPT} ${13_day_harib10i_OS} ${QEMU_DEBUG_OPT}
  DEPENDS 13_day_harib10i_img
)
add_custom_target(13_day_harib10i_clean
  COMMAND rm -f ${13_day_harib10i_OS}
  COMMAND rm -f ${13_day_harib10i_SYS}
  COMMAND rm -f ${13_day_harib10i_IPLB}
  COMMAND rm -f ${13_day_harib10i_HEADB}
  COMMAND rm -f ${13_day_harib10i_BOOTB}
  COMMAND rm -f ${13_day_harib10i_WILDOBJ}
  COMMAND rm -f ${root_BINARY_DIR}/projects/13_day/harib10i/ipl10.bin
  COMMAND rm -f ${root_BINARY_DIR}/projects/13_day/harib10i/asmhead.bin
  COMMAND rm -f ${root_BINARY_DIR}/projects/13_day/harib10i/*_wine.bin
  COMMAND rm -f ${root_BINARY_DIR}/projects/13_day/harib10i/*.txt
)
add_custom_target(13_day_harib10i_ipl_build
  COMMAND ${RAKUSK_EXECUTABLE} ${13_day_harib10i_IPLS} ${13_day_harib10i_IPLB}
  DEPENDS ${13_day_harib10i_IPLS}
)
add_custom_command(
  OUTPUT ${13_day_harib10i_HEADB}
  COMMAND ${RAKUSK_EXECUTABLE} ${13_day_harib10i_HEADS} ${13_day_harib10i_HEADB}
  DEPENDS ${13_day_harib10i_HEADS}
  COMMENT "Generating asmhead.bin for 13_day_harib10i"
)

add_custom_command(
  OUTPUT ${13_day_harib10i_FUNCO}
  COMMAND ${RAKUSK_EXECUTABLE} ${13_day_harib10i_FUNCS} ${13_day_harib10i_FUNCO}
  DEPENDS ${13_day_harib10i_FUNCS}
  COMMENT "Generating naskfunc.o for 13_day_harib10i"
)
add_custom_command(
  OUTPUT ${13_day_harib10i_FONTO}
  COMMAND ${FONT} ${13_day_harib10i_FONTS} ${13_day_harib10i_FONTB}
  COMMAND ${B2O}  ${13_day_harib10i_FONTB} ${13_day_harib10i_FONTO} _hankaku
  DEPENDS ${13_day_harib10i_FONTS}
  COMMENT "Generating hankaku.o for 13_day_harib10i"
)
add_custom_command(
  OUTPUT ${13_day_harib10i_LIBGC}
  COMMAND ${CONV} -fcoff32 -nu ${13_day_harib10i_LIBGE} ${13_day_harib10i_LIBGC}
  DEPENDS ${13_day_harib10i_LIBGE}
  COMMENT "Converting golibc for 13_day_harib10i"
)
add_custom_target(13_day_harib10i_sys
  COMMAND gcc ${BINOPT} -T ${13_day_harib10i_LDS} ${13_day_harib10i_CCS} ${13_day_harib10i_WILDOBJ} -o ${13_day_harib10i_BOOTB}
  COMMAND cat ${13_day_harib10i_HEADB} ${13_day_harib10i_BOOTB} > ${13_day_harib10i_SYS}
  DEPENDS ${13_day_harib10i_HEADB} 13_day_harib10i_ipl_build
  DEPENDS ${13_day_harib10i_FUNCO}
  DEPENDS ${13_day_harib10i_FONTO} ${13_day_harib10i_LIBGC}
)
add_custom_target(13_day_harib10i_img
  COMMAND mformat -f 1440 -l HARIBOTEOS -N 0xffffffff -C -B ${13_day_harib10i_IPLB} -i ${13_day_harib10i_OS}
  COMMAND mcopy -i ${13_day_harib10i_OS} ${13_day_harib10i_SYS} ::
  DEPENDS 13_day_harib10i_sys
)

########### next target ###############
set(13_day_harib10i_asmhead_SRCS ${root_SOURCE_DIR}/projects/13_day/harib10i/asmhead.nas)
set(13_day_harib10i_asmhead_OUTS ${root_BINARY_DIR}/projects/13_day/harib10i/asmhead.bin)

add_custom_target(13_day_harib10i_asmhead
  COMMAND ${RAKUSK_EXECUTABLE} ${13_day_harib10i_asmhead_SRCS} ${13_day_harib10i_asmhead_OUTS}
  DEPENDS ${13_day_harib10i_asmhead_SRCS}
)

set(13_day_harib10i_asmhead_WINE_OUTS ${root_BINARY_DIR}/projects/13_day/harib10i/asmhead_wine.bin)
add_custom_target(13_day_harib10i_asmhead_wine
  COMMAND ${WINE} ${WINE_NASK} ${13_day_harib10i_asmhead_SRCS} ${13_day_harib10i_asmhead_WINE_OUTS}
)
add_dependencies(wine 13_day_harib10i_asmhead_wine)

add_custom_target(13_day_harib10i_asmhead_od
  COMMAND ${OD} -t x1 ${13_day_harib10i_asmhead_OUTS}      > ${13_day_harib10i_asmhead_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${13_day_harib10i_asmhead_WINE_OUTS} > ${13_day_harib10i_asmhead_WINE_OUTS}_t.txt
  COMMAND diff -s ${13_day_harib10i_asmhead_OUTS}_f.txt ${13_day_harib10i_asmhead_WINE_OUTS}_t.txt
)

add_dependencies(13_day_harib10i_asmhead_od 13_day_harib10i_asmhead)
add_dependencies(13_day_harib10i_asmhead_od 13_day_harib10i_asmhead_wine)
add_dependencies(od 13_day_harib10i_asmhead_od)

#----------------------------------------------------------
########### next target ###############
set(13_day_harib10i_ipl10_SRCS ${root_SOURCE_DIR}/projects/13_day/harib10i/ipl10.nas)
set(13_day_harib10i_ipl10_OUTS ${root_BINARY_DIR}/projects/13_day/harib10i/ipl10.bin)

add_custom_target(13_day_harib10i_ipl10
  COMMAND ${RAKUSK_EXECUTABLE} ${13_day_harib10i_ipl10_SRCS} ${13_day_harib10i_ipl10_OUTS}
  DEPENDS ${13_day_harib10i_ipl10_SRCS}
)

set(13_day_harib10i_ipl10_WINE_OUTS ${root_BINARY_DIR}/projects/13_day/harib10i/ipl10_wine.bin)
add_custom_target(13_day_harib10i_ipl10_wine
  COMMAND ${WINE} ${WINE_NASK} ${13_day_harib10i_ipl10_SRCS} ${13_day_harib10i_ipl10_WINE_OUTS}
)
add_dependencies(wine 13_day_harib10i_ipl10_wine)

add_custom_target(13_day_harib10i_ipl10_od
  COMMAND ${OD} -t x1 ${13_day_harib10i_ipl10_OUTS}      > ${13_day_harib10i_ipl10_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${13_day_harib10i_ipl10_WINE_OUTS} > ${13_day_harib10i_ipl10_WINE_OUTS}_t.txt
  COMMAND diff -s ${13_day_harib10i_ipl10_OUTS}_f.txt ${13_day_harib10i_ipl10_WINE_OUTS}_t.txt
)

add_dependencies(13_day_harib10i_ipl10_od 13_day_harib10i_ipl10)
add_dependencies(13_day_harib10i_ipl10_od 13_day_harib10i_ipl10_wine)
add_dependencies(od 13_day_harib10i_ipl10_od)

#----------------------------------------------------------
########### next target ###############
set(13_day_harib10i_naskfunc_SRCS ${root_SOURCE_DIR}/projects/13_day/harib10i/naskfunc.nas)
set(13_day_harib10i_naskfunc_OUTS ${root_BINARY_DIR}/projects/13_day/harib10i/naskfunc.img)

add_custom_target(13_day_harib10i_naskfunc
  COMMAND ${RAKUSK_EXECUTABLE} ${13_day_harib10i_naskfunc_SRCS} ${13_day_harib10i_naskfunc_OUTS}
  DEPENDS ${13_day_harib10i_naskfunc_SRCS}
)

add_dependencies(images 13_day_harib10i_naskfunc)
set(13_day_harib10i_naskfunc_WINE_OUTS ${root_BINARY_DIR}/projects/13_day/harib10i/naskfunc_wine.img)
add_custom_target(13_day_harib10i_naskfunc_wine
  COMMAND ${WINE} ${WINE_NASK} ${13_day_harib10i_naskfunc_SRCS} ${13_day_harib10i_naskfunc_WINE_OUTS}
)
add_dependencies(wine 13_day_harib10i_naskfunc_wine)

add_custom_target(13_day_harib10i_naskfunc_od
  COMMAND ${OD} -t x1 ${13_day_harib10i_naskfunc_OUTS}      > ${13_day_harib10i_naskfunc_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${13_day_harib10i_naskfunc_WINE_OUTS} > ${13_day_harib10i_naskfunc_WINE_OUTS}_t.txt
  COMMAND diff -s ${13_day_harib10i_naskfunc_OUTS}_f.txt ${13_day_harib10i_naskfunc_WINE_OUTS}_t.txt
)

add_dependencies(13_day_harib10i_naskfunc_od 13_day_harib10i_naskfunc)
add_dependencies(13_day_harib10i_naskfunc_od 13_day_harib10i_naskfunc_wine)
add_dependencies(od 13_day_harib10i_naskfunc_od)

#----------------------------------------------------------
