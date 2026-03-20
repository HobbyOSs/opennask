#----------------------------------------------------------
message(STATUS "Entering directory projects/13_day/harib10e/")

set(FONT ${RAKUSK_EXECUTABLE} --makefont)
set(B2O  ${RAKUSK_EXECUTABLE} --bin2obj)
set(CONV ${root_BINARY_DIR}/objconv/objconv)
set(13_day_harib10e_WILDOBJ ${root_BINARY_DIR}/projects/13_day/harib10e/*.o)

# Full OS build rules
set(13_day_harib10e_OS    ${root_BINARY_DIR}/projects/13_day/harib10e/os.img)
set(13_day_harib10e_SYS   ${root_BINARY_DIR}/projects/13_day/harib10e/os.sys)
set(13_day_harib10e_IPLB  ${root_BINARY_DIR}/projects/13_day/harib10e/ipl.bin)
set(13_day_harib10e_IPLS  ${root_SOURCE_DIR}/projects/13_day/harib10e/ipl10.nas)
set(13_day_harib10e_HEADB ${root_BINARY_DIR}/projects/13_day/harib10e/asmhead.bin)
set(13_day_harib10e_HEADS ${root_SOURCE_DIR}/projects/13_day/harib10e/asmhead.nas)
set(13_day_harib10e_BOOTB ${root_BINARY_DIR}/projects/13_day/harib10e/boot.bin)
set(13_day_harib10e_CCO   ${root_BINARY_DIR}/projects/13_day/harib10e/boot.o)
set(13_day_harib10e_CCS   ${root_SOURCE_DIR}/projects/13_day/harib10e/*.c)
set(13_day_harib10e_LDS   ${root_SOURCE_DIR}/projects/os.lds)
set(13_day_harib10e_FUNCS ${root_SOURCE_DIR}/projects/13_day/harib10e/naskfunc.nas)
set(13_day_harib10e_FUNCO ${root_BINARY_DIR}/projects/13_day/harib10e/naskfunc.o)
set(13_day_harib10e_FONTS ${root_SOURCE_DIR}/projects/13_day/harib10e/hankaku.txt)
set(13_day_harib10e_FONTB ${root_BINARY_DIR}/projects/13_day/harib10e/hankaku.bin)
set(13_day_harib10e_FONTO ${root_BINARY_DIR}/projects/13_day/harib10e/hankaku.o)
set(13_day_harib10e_LIBGE ${root_BINARY_DIR}/golibc/libgo.a)
set(13_day_harib10e_LIBGC ${root_BINARY_DIR}/projects/13_day/harib10e/libgo.o)

add_custom_target(13_day_harib10e_run
  COMMAND ${QEMU} ${QEMUOPT} ${13_day_harib10e_OS}
  DEPENDS 13_day_harib10e_img
)
add_custom_target(13_day_harib10e_debug
  COMMAND ${QEMU} -s -S ${QEMUOPT} ${13_day_harib10e_OS} ${QEMU_DEBUG_OPT}
  DEPENDS 13_day_harib10e_img
)
add_custom_target(13_day_harib10e_clean
  COMMAND rm -f ${13_day_harib10e_OS}
  COMMAND rm -f ${13_day_harib10e_SYS}
  COMMAND rm -f ${13_day_harib10e_IPLB}
  COMMAND rm -f ${13_day_harib10e_HEADB}
  COMMAND rm -f ${13_day_harib10e_BOOTB}
  COMMAND rm -f ${13_day_harib10e_WILDOBJ}
  COMMAND rm -f ${root_BINARY_DIR}/projects/13_day/harib10e/ipl10.bin
  COMMAND rm -f ${root_BINARY_DIR}/projects/13_day/harib10e/asmhead.bin
  COMMAND rm -f ${root_BINARY_DIR}/projects/13_day/harib10e/*_wine.bin
  COMMAND rm -f ${root_BINARY_DIR}/projects/13_day/harib10e/*.txt
)
add_custom_target(13_day_harib10e_ipl_build
  COMMAND ${RAKUSK_EXECUTABLE} ${13_day_harib10e_IPLS} ${13_day_harib10e_IPLB}
  DEPENDS ${13_day_harib10e_IPLS}
)
add_custom_command(
  OUTPUT ${13_day_harib10e_HEADB}
  COMMAND ${RAKUSK_EXECUTABLE} ${13_day_harib10e_HEADS} ${13_day_harib10e_HEADB}
  DEPENDS ${13_day_harib10e_HEADS}
  COMMENT "Generating asmhead.bin for 13_day_harib10e"
)

add_custom_target(13_day_harib10e_sys
  COMMAND ${FONT} ${13_day_harib10e_FONTS} ${13_day_harib10e_FONTB}
  COMMAND ${B2O}  ${13_day_harib10e_FONTB} ${13_day_harib10e_FONTO} _hankaku
  COMMAND ${CONV} -fcoff32 -nu ${13_day_harib10e_LIBGE} ${13_day_harib10e_LIBGC}
  COMMAND ${RAKUSK_EXECUTABLE} ${13_day_harib10e_FUNCS} ${13_day_harib10e_FUNCO}
  COMMAND gcc ${BINOPT} -T ${13_day_harib10e_LDS} ${13_day_harib10e_CCS} ${13_day_harib10e_WILDOBJ} -o ${13_day_harib10e_BOOTB}
  COMMAND cat ${13_day_harib10e_HEADB} ${13_day_harib10e_BOOTB} > ${13_day_harib10e_SYS}
  DEPENDS ${13_day_harib10e_HEADB} 13_day_harib10e_ipl_build
  DEPENDS ${13_day_harib10e_FUNCO}
  DEPENDS ${13_day_harib10e_FONTO}
)
add_custom_target(13_day_harib10e_img
  COMMAND mformat -f 1440 -l HARIBOTEOS -N 0xffffffff -C -B ${13_day_harib10e_IPLB} -i ${13_day_harib10e_OS}
  COMMAND mcopy -i ${13_day_harib10e_OS} ${13_day_harib10e_SYS} ::
  DEPENDS 13_day_harib10e_sys
)

########### next target ###############
set(13_day_harib10e_asmhead_SRCS ${root_SOURCE_DIR}/projects/13_day/harib10e/asmhead.nas)
set(13_day_harib10e_asmhead_OUTS ${root_BINARY_DIR}/projects/13_day/harib10e/asmhead.bin)

add_custom_target(13_day_harib10e_asmhead
  COMMAND ${RAKUSK_EXECUTABLE} ${13_day_harib10e_asmhead_SRCS} ${13_day_harib10e_asmhead_OUTS}
  DEPENDS ${13_day_harib10e_asmhead_SRCS}
)

set(13_day_harib10e_asmhead_WINE_OUTS ${root_BINARY_DIR}/projects/13_day/harib10e/asmhead_wine.bin)
add_custom_target(13_day_harib10e_asmhead_wine
  COMMAND ${WINE} ${WINE_NASK} ${13_day_harib10e_asmhead_SRCS} ${13_day_harib10e_asmhead_WINE_OUTS}
)
add_dependencies(wine 13_day_harib10e_asmhead_wine)

add_custom_target(13_day_harib10e_asmhead_od
  COMMAND ${OD} -t x1 ${13_day_harib10e_asmhead_OUTS}      > ${13_day_harib10e_asmhead_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${13_day_harib10e_asmhead_WINE_OUTS} > ${13_day_harib10e_asmhead_WINE_OUTS}_t.txt
  COMMAND diff -s ${13_day_harib10e_asmhead_OUTS}_f.txt ${13_day_harib10e_asmhead_WINE_OUTS}_t.txt
)

add_dependencies(13_day_harib10e_asmhead_od 13_day_harib10e_asmhead)
add_dependencies(13_day_harib10e_asmhead_od 13_day_harib10e_asmhead_wine)
add_dependencies(od 13_day_harib10e_asmhead_od)

#----------------------------------------------------------
########### next target ###############
set(13_day_harib10e_ipl10_SRCS ${root_SOURCE_DIR}/projects/13_day/harib10e/ipl10.nas)
set(13_day_harib10e_ipl10_OUTS ${root_BINARY_DIR}/projects/13_day/harib10e/ipl10.bin)

add_custom_target(13_day_harib10e_ipl10
  COMMAND ${RAKUSK_EXECUTABLE} ${13_day_harib10e_ipl10_SRCS} ${13_day_harib10e_ipl10_OUTS}
  DEPENDS ${13_day_harib10e_ipl10_SRCS}
)

set(13_day_harib10e_ipl10_WINE_OUTS ${root_BINARY_DIR}/projects/13_day/harib10e/ipl10_wine.bin)
add_custom_target(13_day_harib10e_ipl10_wine
  COMMAND ${WINE} ${WINE_NASK} ${13_day_harib10e_ipl10_SRCS} ${13_day_harib10e_ipl10_WINE_OUTS}
)
add_dependencies(wine 13_day_harib10e_ipl10_wine)

add_custom_target(13_day_harib10e_ipl10_od
  COMMAND ${OD} -t x1 ${13_day_harib10e_ipl10_OUTS}      > ${13_day_harib10e_ipl10_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${13_day_harib10e_ipl10_WINE_OUTS} > ${13_day_harib10e_ipl10_WINE_OUTS}_t.txt
  COMMAND diff -s ${13_day_harib10e_ipl10_OUTS}_f.txt ${13_day_harib10e_ipl10_WINE_OUTS}_t.txt
)

add_dependencies(13_day_harib10e_ipl10_od 13_day_harib10e_ipl10)
add_dependencies(13_day_harib10e_ipl10_od 13_day_harib10e_ipl10_wine)
add_dependencies(od 13_day_harib10e_ipl10_od)

#----------------------------------------------------------
########### next target ###############
set(13_day_harib10e_naskfunc_SRCS ${root_SOURCE_DIR}/projects/13_day/harib10e/naskfunc.nas)
set(13_day_harib10e_naskfunc_OUTS ${root_BINARY_DIR}/projects/13_day/harib10e/naskfunc.img)

add_custom_target(13_day_harib10e_naskfunc
  COMMAND ${RAKUSK_EXECUTABLE} ${13_day_harib10e_naskfunc_SRCS} ${13_day_harib10e_naskfunc_OUTS}
  DEPENDS ${13_day_harib10e_naskfunc_SRCS}
)

add_dependencies(images 13_day_harib10e_naskfunc)
set(13_day_harib10e_naskfunc_WINE_OUTS ${root_BINARY_DIR}/projects/13_day/harib10e/naskfunc_wine.img)
add_custom_target(13_day_harib10e_naskfunc_wine
  COMMAND ${WINE} ${WINE_NASK} ${13_day_harib10e_naskfunc_SRCS} ${13_day_harib10e_naskfunc_WINE_OUTS}
)
add_dependencies(wine 13_day_harib10e_naskfunc_wine)

add_custom_target(13_day_harib10e_naskfunc_od
  COMMAND ${OD} -t x1 ${13_day_harib10e_naskfunc_OUTS}      > ${13_day_harib10e_naskfunc_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${13_day_harib10e_naskfunc_WINE_OUTS} > ${13_day_harib10e_naskfunc_WINE_OUTS}_t.txt
  COMMAND diff -s ${13_day_harib10e_naskfunc_OUTS}_f.txt ${13_day_harib10e_naskfunc_WINE_OUTS}_t.txt
)

add_dependencies(13_day_harib10e_naskfunc_od 13_day_harib10e_naskfunc)
add_dependencies(13_day_harib10e_naskfunc_od 13_day_harib10e_naskfunc_wine)
add_dependencies(od 13_day_harib10e_naskfunc_od)

#----------------------------------------------------------
