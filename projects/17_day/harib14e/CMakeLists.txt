#----------------------------------------------------------
message(STATUS "Entering directory projects/17_day/harib14e/")

set(FONT ${RAKUSK_EXECUTABLE} --makefont)
set(B2O  ${RAKUSK_EXECUTABLE} --bin2obj)
set(CONV ${root_BINARY_DIR}/objconv/objconv)
set(17_day_harib14e_WILDOBJ ${root_BINARY_DIR}/projects/17_day/harib14e/*.o)

# Full OS build rules
set(17_day_harib14e_OS    ${root_BINARY_DIR}/projects/17_day/harib14e/os.img)
set(17_day_harib14e_SYS   ${root_BINARY_DIR}/projects/17_day/harib14e/os.sys)
set(17_day_harib14e_IPLB  ${root_BINARY_DIR}/projects/17_day/harib14e/ipl.bin)
set(17_day_harib14e_IPLS  ${root_SOURCE_DIR}/projects/17_day/harib14e/ipl10.nas)
set(17_day_harib14e_HEADB ${root_BINARY_DIR}/projects/17_day/harib14e/asmhead.bin)
set(17_day_harib14e_HEADS ${root_SOURCE_DIR}/projects/17_day/harib14e/asmhead.nas)
set(17_day_harib14e_BOOTB ${root_BINARY_DIR}/projects/17_day/harib14e/boot.bin)
set(17_day_harib14e_CCO   ${root_BINARY_DIR}/projects/17_day/harib14e/boot.o)
set(17_day_harib14e_CCS   ${root_SOURCE_DIR}/projects/17_day/harib14e/*.c)
set(17_day_harib14e_LDS   ${root_SOURCE_DIR}/projects/os.lds)
set(17_day_harib14e_FUNCS ${root_SOURCE_DIR}/projects/17_day/harib14e/naskfunc.nas)
set(17_day_harib14e_FUNCO ${root_BINARY_DIR}/projects/17_day/harib14e/naskfunc.o)
set(17_day_harib14e_FONTS ${root_SOURCE_DIR}/projects/17_day/harib14e/hankaku.txt)
set(17_day_harib14e_FONTB ${root_BINARY_DIR}/projects/17_day/harib14e/hankaku.bin)
set(17_day_harib14e_FONTO ${root_BINARY_DIR}/projects/17_day/harib14e/hankaku.o)
set(17_day_harib14e_LIBGE ${root_BINARY_DIR}/golibc/libgo.a)
set(17_day_harib14e_LIBGC ${root_BINARY_DIR}/projects/17_day/harib14e/libgo.o)

add_custom_target(17_day_harib14e_run
  COMMAND ${QEMU} ${QEMUOPT} ${17_day_harib14e_OS}
  DEPENDS 17_day_harib14e_img
)
add_custom_target(17_day_harib14e_debug
  COMMAND ${QEMU} -s -S ${QEMUOPT} ${17_day_harib14e_OS} ${QEMU_DEBUG_OPT}
  DEPENDS 17_day_harib14e_img
)
add_custom_target(17_day_harib14e_clean
  COMMAND rm -f ${17_day_harib14e_OS}
  COMMAND rm -f ${17_day_harib14e_SYS}
  COMMAND rm -f ${17_day_harib14e_IPLB}
  COMMAND rm -f ${17_day_harib14e_HEADB}
  COMMAND rm -f ${17_day_harib14e_BOOTB}
  COMMAND rm -f ${17_day_harib14e_WILDOBJ}
  COMMAND rm -f ${root_BINARY_DIR}/projects/17_day/harib14e/ipl10.bin
  COMMAND rm -f ${root_BINARY_DIR}/projects/17_day/harib14e/asmhead.bin
  COMMAND rm -f ${root_BINARY_DIR}/projects/17_day/harib14e/*_wine.bin
  COMMAND rm -f ${root_BINARY_DIR}/projects/17_day/harib14e/*.txt
)
add_custom_target(17_day_harib14e_ipl_build
  COMMAND ${RAKUSK_EXECUTABLE} ${17_day_harib14e_IPLS} ${17_day_harib14e_IPLB}
  DEPENDS ${17_day_harib14e_IPLS}
)
add_custom_command(
  OUTPUT ${17_day_harib14e_HEADB}
  COMMAND ${RAKUSK_EXECUTABLE} ${17_day_harib14e_HEADS} ${17_day_harib14e_HEADB}
  DEPENDS ${17_day_harib14e_HEADS}
  COMMENT "Generating asmhead.bin for 17_day_harib14e"
)

add_custom_target(17_day_harib14e_sys
  COMMAND ${FONT} ${17_day_harib14e_FONTS} ${17_day_harib14e_FONTB}
  COMMAND ${B2O}  ${17_day_harib14e_FONTB} ${17_day_harib14e_FONTO} _hankaku
  COMMAND ${CONV} -fcoff32 -nu ${17_day_harib14e_LIBGE} ${17_day_harib14e_LIBGC}
  COMMAND ${RAKUSK_EXECUTABLE} ${17_day_harib14e_FUNCS} ${17_day_harib14e_FUNCO}
  COMMAND gcc ${BINOPT} -T ${17_day_harib14e_LDS} ${17_day_harib14e_CCS} ${17_day_harib14e_WILDOBJ} -o ${17_day_harib14e_BOOTB}
  COMMAND cat ${17_day_harib14e_HEADB} ${17_day_harib14e_BOOTB} > ${17_day_harib14e_SYS}
  DEPENDS ${17_day_harib14e_HEADB} 17_day_harib14e_ipl_build
  DEPENDS ${17_day_harib14e_FUNCO}
  DEPENDS ${17_day_harib14e_FONTO}
)
add_custom_target(17_day_harib14e_img
  COMMAND mformat -f 1440 -l HARIBOTEOS -N 0xffffffff -C -B ${17_day_harib14e_IPLB} -i ${17_day_harib14e_OS}
  COMMAND mcopy -i ${17_day_harib14e_OS} ${17_day_harib14e_SYS} ::
  DEPENDS 17_day_harib14e_sys
)

########### next target ###############
set(17_day_harib14e_asmhead_SRCS ${root_SOURCE_DIR}/projects/17_day/harib14e/asmhead.nas)
set(17_day_harib14e_asmhead_OUTS ${root_BINARY_DIR}/projects/17_day/harib14e/asmhead.bin)

add_custom_target(17_day_harib14e_asmhead
  COMMAND ${RAKUSK_EXECUTABLE} ${17_day_harib14e_asmhead_SRCS} ${17_day_harib14e_asmhead_OUTS}
  DEPENDS ${17_day_harib14e_asmhead_SRCS}
)

set(17_day_harib14e_asmhead_WINE_OUTS ${root_BINARY_DIR}/projects/17_day/harib14e/asmhead_wine.bin)
add_custom_target(17_day_harib14e_asmhead_wine
  COMMAND ${WINE} ${WINE_NASK} ${17_day_harib14e_asmhead_SRCS} ${17_day_harib14e_asmhead_WINE_OUTS}
)
add_dependencies(wine 17_day_harib14e_asmhead_wine)

add_custom_target(17_day_harib14e_asmhead_od
  COMMAND ${OD} -t x1 ${17_day_harib14e_asmhead_OUTS}      > ${17_day_harib14e_asmhead_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${17_day_harib14e_asmhead_WINE_OUTS} > ${17_day_harib14e_asmhead_WINE_OUTS}_t.txt
  COMMAND diff -s ${17_day_harib14e_asmhead_OUTS}_f.txt ${17_day_harib14e_asmhead_WINE_OUTS}_t.txt
)

add_dependencies(17_day_harib14e_asmhead_od 17_day_harib14e_asmhead)
add_dependencies(17_day_harib14e_asmhead_od 17_day_harib14e_asmhead_wine)
add_dependencies(od 17_day_harib14e_asmhead_od)

#----------------------------------------------------------
########### next target ###############
set(17_day_harib14e_ipl10_SRCS ${root_SOURCE_DIR}/projects/17_day/harib14e/ipl10.nas)
set(17_day_harib14e_ipl10_OUTS ${root_BINARY_DIR}/projects/17_day/harib14e/ipl10.bin)

add_custom_target(17_day_harib14e_ipl10
  COMMAND ${RAKUSK_EXECUTABLE} ${17_day_harib14e_ipl10_SRCS} ${17_day_harib14e_ipl10_OUTS}
  DEPENDS ${17_day_harib14e_ipl10_SRCS}
)

set(17_day_harib14e_ipl10_WINE_OUTS ${root_BINARY_DIR}/projects/17_day/harib14e/ipl10_wine.bin)
add_custom_target(17_day_harib14e_ipl10_wine
  COMMAND ${WINE} ${WINE_NASK} ${17_day_harib14e_ipl10_SRCS} ${17_day_harib14e_ipl10_WINE_OUTS}
)
add_dependencies(wine 17_day_harib14e_ipl10_wine)

add_custom_target(17_day_harib14e_ipl10_od
  COMMAND ${OD} -t x1 ${17_day_harib14e_ipl10_OUTS}      > ${17_day_harib14e_ipl10_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${17_day_harib14e_ipl10_WINE_OUTS} > ${17_day_harib14e_ipl10_WINE_OUTS}_t.txt
  COMMAND diff -s ${17_day_harib14e_ipl10_OUTS}_f.txt ${17_day_harib14e_ipl10_WINE_OUTS}_t.txt
)

add_dependencies(17_day_harib14e_ipl10_od 17_day_harib14e_ipl10)
add_dependencies(17_day_harib14e_ipl10_od 17_day_harib14e_ipl10_wine)
add_dependencies(od 17_day_harib14e_ipl10_od)

#----------------------------------------------------------
########### next target ###############
set(17_day_harib14e_naskfunc_SRCS ${root_SOURCE_DIR}/projects/17_day/harib14e/naskfunc.nas)
set(17_day_harib14e_naskfunc_OUTS ${root_BINARY_DIR}/projects/17_day/harib14e/naskfunc.img)

add_custom_target(17_day_harib14e_naskfunc
  COMMAND ${RAKUSK_EXECUTABLE} ${17_day_harib14e_naskfunc_SRCS} ${17_day_harib14e_naskfunc_OUTS}
  DEPENDS ${17_day_harib14e_naskfunc_SRCS}
)

add_dependencies(images 17_day_harib14e_naskfunc)
set(17_day_harib14e_naskfunc_WINE_OUTS ${root_BINARY_DIR}/projects/17_day/harib14e/naskfunc_wine.img)
add_custom_target(17_day_harib14e_naskfunc_wine
  COMMAND ${WINE} ${WINE_NASK} ${17_day_harib14e_naskfunc_SRCS} ${17_day_harib14e_naskfunc_WINE_OUTS}
)
add_dependencies(wine 17_day_harib14e_naskfunc_wine)

add_custom_target(17_day_harib14e_naskfunc_od
  COMMAND ${OD} -t x1 ${17_day_harib14e_naskfunc_OUTS}      > ${17_day_harib14e_naskfunc_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${17_day_harib14e_naskfunc_WINE_OUTS} > ${17_day_harib14e_naskfunc_WINE_OUTS}_t.txt
  COMMAND diff -s ${17_day_harib14e_naskfunc_OUTS}_f.txt ${17_day_harib14e_naskfunc_WINE_OUTS}_t.txt
)

add_dependencies(17_day_harib14e_naskfunc_od 17_day_harib14e_naskfunc)
add_dependencies(17_day_harib14e_naskfunc_od 17_day_harib14e_naskfunc_wine)
add_dependencies(od 17_day_harib14e_naskfunc_od)

#----------------------------------------------------------
