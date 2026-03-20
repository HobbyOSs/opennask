#----------------------------------------------------------
message(STATUS "Entering directory projects/15_day/harib12e/")

set(FONT ${RAKUSK_EXECUTABLE} --makefont)
set(B2O  ${RAKUSK_EXECUTABLE} --bin2obj)
set(CONV ${root_BINARY_DIR}/objconv/objconv)
set(15_day_harib12e_WILDOBJ ${root_BINARY_DIR}/projects/15_day/harib12e/*.o)

# Full OS build rules
set(15_day_harib12e_OS    ${root_BINARY_DIR}/projects/15_day/harib12e/os.img)
set(15_day_harib12e_SYS   ${root_BINARY_DIR}/projects/15_day/harib12e/os.sys)
set(15_day_harib12e_IPLB  ${root_BINARY_DIR}/projects/15_day/harib12e/ipl.bin)
set(15_day_harib12e_IPLS  ${root_SOURCE_DIR}/projects/15_day/harib12e/ipl10.nas)
set(15_day_harib12e_HEADB ${root_BINARY_DIR}/projects/15_day/harib12e/asmhead.bin)
set(15_day_harib12e_HEADS ${root_SOURCE_DIR}/projects/15_day/harib12e/asmhead.nas)
set(15_day_harib12e_BOOTB ${root_BINARY_DIR}/projects/15_day/harib12e/boot.bin)
set(15_day_harib12e_CCO   ${root_BINARY_DIR}/projects/15_day/harib12e/boot.o)
set(15_day_harib12e_CCS   ${root_SOURCE_DIR}/projects/15_day/harib12e/*.c)
set(15_day_harib12e_LDS   ${root_SOURCE_DIR}/projects/os.lds)
set(15_day_harib12e_FUNCS ${root_SOURCE_DIR}/projects/15_day/harib12e/naskfunc.nas)
set(15_day_harib12e_FUNCO ${root_BINARY_DIR}/projects/15_day/harib12e/naskfunc.o)
set(15_day_harib12e_FONTS ${root_SOURCE_DIR}/projects/15_day/harib12e/hankaku.txt)
set(15_day_harib12e_FONTB ${root_BINARY_DIR}/projects/15_day/harib12e/hankaku.bin)
set(15_day_harib12e_FONTO ${root_BINARY_DIR}/projects/15_day/harib12e/hankaku.o)
set(15_day_harib12e_LIBGE ${root_BINARY_DIR}/golibc/libgo.a)
set(15_day_harib12e_LIBGC ${root_BINARY_DIR}/projects/15_day/harib12e/libgo.o)

add_custom_target(15_day_harib12e_run
  COMMAND ${QEMU} ${QEMUOPT} ${15_day_harib12e_OS}
  DEPENDS 15_day_harib12e_img
)
add_custom_target(15_day_harib12e_debug
  COMMAND ${QEMU} -s -S ${QEMUOPT} ${15_day_harib12e_OS} ${QEMU_DEBUG_OPT}
  DEPENDS 15_day_harib12e_img
)
add_custom_target(15_day_harib12e_clean
  COMMAND rm -f ${15_day_harib12e_OS}
  COMMAND rm -f ${15_day_harib12e_SYS}
  COMMAND rm -f ${15_day_harib12e_IPLB}
  COMMAND rm -f ${15_day_harib12e_HEADB}
  COMMAND rm -f ${15_day_harib12e_BOOTB}
  COMMAND rm -f ${15_day_harib12e_WILDOBJ}
  COMMAND rm -f ${root_BINARY_DIR}/projects/15_day/harib12e/ipl10.bin
  COMMAND rm -f ${root_BINARY_DIR}/projects/15_day/harib12e/asmhead.bin
  COMMAND rm -f ${root_BINARY_DIR}/projects/15_day/harib12e/*_wine.bin
  COMMAND rm -f ${root_BINARY_DIR}/projects/15_day/harib12e/*.txt
)
add_custom_target(15_day_harib12e_ipl_build
  COMMAND ${RAKUSK_EXECUTABLE} ${15_day_harib12e_IPLS} ${15_day_harib12e_IPLB}
  DEPENDS ${15_day_harib12e_IPLS}
)
add_custom_command(
  OUTPUT ${15_day_harib12e_HEADB}
  COMMAND ${RAKUSK_EXECUTABLE} ${15_day_harib12e_HEADS} ${15_day_harib12e_HEADB}
  DEPENDS ${15_day_harib12e_HEADS}
  COMMENT "Generating asmhead.bin for 15_day_harib12e"
)

add_custom_target(15_day_harib12e_sys
  COMMAND ${FONT} ${15_day_harib12e_FONTS} ${15_day_harib12e_FONTB}
  COMMAND ${B2O}  ${15_day_harib12e_FONTB} ${15_day_harib12e_FONTO} _hankaku
  COMMAND ${CONV} -fcoff32 -nu ${15_day_harib12e_LIBGE} ${15_day_harib12e_LIBGC}
  COMMAND ${RAKUSK_EXECUTABLE} ${15_day_harib12e_FUNCS} ${15_day_harib12e_FUNCO}
  COMMAND gcc ${BINOPT} -T ${15_day_harib12e_LDS} ${15_day_harib12e_CCS} ${15_day_harib12e_WILDOBJ} -o ${15_day_harib12e_BOOTB}
  COMMAND cat ${15_day_harib12e_HEADB} ${15_day_harib12e_BOOTB} > ${15_day_harib12e_SYS}
  DEPENDS ${15_day_harib12e_HEADB} 15_day_harib12e_ipl_build
  DEPENDS ${15_day_harib12e_FUNCO}
  DEPENDS ${15_day_harib12e_FONTO}
)
add_custom_target(15_day_harib12e_img
  COMMAND mformat -f 1440 -l HARIBOTEOS -N 0xffffffff -C -B ${15_day_harib12e_IPLB} -i ${15_day_harib12e_OS}
  COMMAND mcopy -i ${15_day_harib12e_OS} ${15_day_harib12e_SYS} ::
  DEPENDS 15_day_harib12e_sys
)

########### next target ###############
set(15_day_harib12e_asmhead_SRCS ${root_SOURCE_DIR}/projects/15_day/harib12e/asmhead.nas)
set(15_day_harib12e_asmhead_OUTS ${root_BINARY_DIR}/projects/15_day/harib12e/asmhead.bin)

add_custom_target(15_day_harib12e_asmhead
  COMMAND ${RAKUSK_EXECUTABLE} ${15_day_harib12e_asmhead_SRCS} ${15_day_harib12e_asmhead_OUTS}
  DEPENDS ${15_day_harib12e_asmhead_SRCS}
)

set(15_day_harib12e_asmhead_WINE_OUTS ${root_BINARY_DIR}/projects/15_day/harib12e/asmhead_wine.bin)
add_custom_target(15_day_harib12e_asmhead_wine
  COMMAND ${WINE} ${WINE_NASK} ${15_day_harib12e_asmhead_SRCS} ${15_day_harib12e_asmhead_WINE_OUTS}
)
add_dependencies(wine 15_day_harib12e_asmhead_wine)

add_custom_target(15_day_harib12e_asmhead_od
  COMMAND ${OD} -t x1 ${15_day_harib12e_asmhead_OUTS}      > ${15_day_harib12e_asmhead_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${15_day_harib12e_asmhead_WINE_OUTS} > ${15_day_harib12e_asmhead_WINE_OUTS}_t.txt
  COMMAND diff -s ${15_day_harib12e_asmhead_OUTS}_f.txt ${15_day_harib12e_asmhead_WINE_OUTS}_t.txt
)

add_dependencies(15_day_harib12e_asmhead_od 15_day_harib12e_asmhead)
add_dependencies(15_day_harib12e_asmhead_od 15_day_harib12e_asmhead_wine)
add_dependencies(od 15_day_harib12e_asmhead_od)

#----------------------------------------------------------
########### next target ###############
set(15_day_harib12e_ipl10_SRCS ${root_SOURCE_DIR}/projects/15_day/harib12e/ipl10.nas)
set(15_day_harib12e_ipl10_OUTS ${root_BINARY_DIR}/projects/15_day/harib12e/ipl10.bin)

add_custom_target(15_day_harib12e_ipl10
  COMMAND ${RAKUSK_EXECUTABLE} ${15_day_harib12e_ipl10_SRCS} ${15_day_harib12e_ipl10_OUTS}
  DEPENDS ${15_day_harib12e_ipl10_SRCS}
)

set(15_day_harib12e_ipl10_WINE_OUTS ${root_BINARY_DIR}/projects/15_day/harib12e/ipl10_wine.bin)
add_custom_target(15_day_harib12e_ipl10_wine
  COMMAND ${WINE} ${WINE_NASK} ${15_day_harib12e_ipl10_SRCS} ${15_day_harib12e_ipl10_WINE_OUTS}
)
add_dependencies(wine 15_day_harib12e_ipl10_wine)

add_custom_target(15_day_harib12e_ipl10_od
  COMMAND ${OD} -t x1 ${15_day_harib12e_ipl10_OUTS}      > ${15_day_harib12e_ipl10_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${15_day_harib12e_ipl10_WINE_OUTS} > ${15_day_harib12e_ipl10_WINE_OUTS}_t.txt
  COMMAND diff -s ${15_day_harib12e_ipl10_OUTS}_f.txt ${15_day_harib12e_ipl10_WINE_OUTS}_t.txt
)

add_dependencies(15_day_harib12e_ipl10_od 15_day_harib12e_ipl10)
add_dependencies(15_day_harib12e_ipl10_od 15_day_harib12e_ipl10_wine)
add_dependencies(od 15_day_harib12e_ipl10_od)

#----------------------------------------------------------
########### next target ###############
set(15_day_harib12e_naskfunc_SRCS ${root_SOURCE_DIR}/projects/15_day/harib12e/naskfunc.nas)
set(15_day_harib12e_naskfunc_OUTS ${root_BINARY_DIR}/projects/15_day/harib12e/naskfunc.img)

add_custom_target(15_day_harib12e_naskfunc
  COMMAND ${RAKUSK_EXECUTABLE} ${15_day_harib12e_naskfunc_SRCS} ${15_day_harib12e_naskfunc_OUTS}
  DEPENDS ${15_day_harib12e_naskfunc_SRCS}
)

add_dependencies(images 15_day_harib12e_naskfunc)
set(15_day_harib12e_naskfunc_WINE_OUTS ${root_BINARY_DIR}/projects/15_day/harib12e/naskfunc_wine.img)
add_custom_target(15_day_harib12e_naskfunc_wine
  COMMAND ${WINE} ${WINE_NASK} ${15_day_harib12e_naskfunc_SRCS} ${15_day_harib12e_naskfunc_WINE_OUTS}
)
add_dependencies(wine 15_day_harib12e_naskfunc_wine)

add_custom_target(15_day_harib12e_naskfunc_od
  COMMAND ${OD} -t x1 ${15_day_harib12e_naskfunc_OUTS}      > ${15_day_harib12e_naskfunc_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${15_day_harib12e_naskfunc_WINE_OUTS} > ${15_day_harib12e_naskfunc_WINE_OUTS}_t.txt
  COMMAND diff -s ${15_day_harib12e_naskfunc_OUTS}_f.txt ${15_day_harib12e_naskfunc_WINE_OUTS}_t.txt
)

add_dependencies(15_day_harib12e_naskfunc_od 15_day_harib12e_naskfunc)
add_dependencies(15_day_harib12e_naskfunc_od 15_day_harib12e_naskfunc_wine)
add_dependencies(od 15_day_harib12e_naskfunc_od)

#----------------------------------------------------------
