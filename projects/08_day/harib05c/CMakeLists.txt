#----------------------------------------------------------
message(STATUS "Entering directory projects/08_day/harib05c/")

set(FONT ${RAKUSK_EXECUTABLE} --makefont)
set(B2O  ${RAKUSK_EXECUTABLE} --bin2obj)
set(CONV ${root_BINARY_DIR}/objconv/objconv)
set(08_day_harib05c_WILDOBJ ${root_BINARY_DIR}/projects/08_day/harib05c/*.o)

# Full OS build rules
set(08_day_harib05c_OS    ${root_BINARY_DIR}/projects/08_day/harib05c/os.img)
set(08_day_harib05c_SYS   ${root_BINARY_DIR}/projects/08_day/harib05c/os.sys)
set(08_day_harib05c_IPLB  ${root_BINARY_DIR}/projects/08_day/harib05c/ipl.bin)
set(08_day_harib05c_IPLS  ${root_SOURCE_DIR}/projects/08_day/harib05c/ipl10.nas)
set(08_day_harib05c_HEADB ${root_BINARY_DIR}/projects/08_day/harib05c/asmhead.bin)
set(08_day_harib05c_HEADS ${root_SOURCE_DIR}/projects/08_day/harib05c/asmhead.nas)
set(08_day_harib05c_BOOTB ${root_BINARY_DIR}/projects/08_day/harib05c/boot.bin)
set(08_day_harib05c_CCO   ${root_BINARY_DIR}/projects/08_day/harib05c/boot.o)
set(08_day_harib05c_CCS   ${root_SOURCE_DIR}/projects/08_day/harib05c/*.c)
set(08_day_harib05c_LDS   ${root_SOURCE_DIR}/projects/os.lds)
set(08_day_harib05c_FUNCS ${root_SOURCE_DIR}/projects/08_day/harib05c/naskfunc.nas)
set(08_day_harib05c_FUNCO ${root_BINARY_DIR}/projects/08_day/harib05c/naskfunc.o)
set(08_day_harib05c_FONTS ${root_SOURCE_DIR}/projects/08_day/harib05c/hankaku.txt)
set(08_day_harib05c_FONTB ${root_BINARY_DIR}/projects/08_day/harib05c/hankaku.bin)
set(08_day_harib05c_FONTO ${root_BINARY_DIR}/projects/08_day/harib05c/hankaku.o)
set(08_day_harib05c_LIBGE ${root_BINARY_DIR}/golibc/libgo.a)
set(08_day_harib05c_LIBGC ${root_BINARY_DIR}/projects/08_day/harib05c/libgo.o)

add_custom_target(08_day_harib05c_run
  COMMAND ${QEMU} ${QEMUOPT} ${08_day_harib05c_OS}
  DEPENDS 08_day_harib05c_img
)
add_custom_target(08_day_harib05c_debug
  COMMAND ${QEMU} -s -S ${QEMUOPT} ${08_day_harib05c_OS} ${QEMU_DEBUG_OPT}
  DEPENDS 08_day_harib05c_img
)
add_custom_target(08_day_harib05c_clean
  COMMAND rm -f ${08_day_harib05c_OS}
  COMMAND rm -f ${08_day_harib05c_SYS}
  COMMAND rm -f ${08_day_harib05c_IPLB}
  COMMAND rm -f ${08_day_harib05c_HEADB}
  COMMAND rm -f ${08_day_harib05c_BOOTB}
  COMMAND rm -f ${08_day_harib05c_WILDOBJ}
  COMMAND rm -f ${root_BINARY_DIR}/projects/08_day/harib05c/ipl10.bin
  COMMAND rm -f ${root_BINARY_DIR}/projects/08_day/harib05c/asmhead.bin
  COMMAND rm -f ${root_BINARY_DIR}/projects/08_day/harib05c/*_wine.bin
  COMMAND rm -f ${root_BINARY_DIR}/projects/08_day/harib05c/*.txt
)
add_custom_target(08_day_harib05c_ipl_build
  COMMAND ${RAKUSK_EXECUTABLE} ${08_day_harib05c_IPLS} ${08_day_harib05c_IPLB}
  DEPENDS ${08_day_harib05c_IPLS}
)
add_custom_command(
  OUTPUT ${08_day_harib05c_HEADB}
  COMMAND ${RAKUSK_EXECUTABLE} ${08_day_harib05c_HEADS} ${08_day_harib05c_HEADB}
  DEPENDS ${08_day_harib05c_HEADS}
  COMMENT "Generating asmhead.bin for 08_day_harib05c"
)

add_custom_target(08_day_harib05c_sys
  COMMAND ${FONT} ${08_day_harib05c_FONTS} ${08_day_harib05c_FONTB}
  COMMAND ${B2O}  ${08_day_harib05c_FONTB} ${08_day_harib05c_FONTO} _hankaku
  COMMAND ${CONV} -fcoff32 -nu ${08_day_harib05c_LIBGE} ${08_day_harib05c_LIBGC}
  COMMAND ${RAKUSK_EXECUTABLE} ${08_day_harib05c_FUNCS} ${08_day_harib05c_FUNCO}
  COMMAND gcc ${BINOPT} -T ${08_day_harib05c_LDS} ${08_day_harib05c_CCS} ${08_day_harib05c_WILDOBJ} -o ${08_day_harib05c_BOOTB}
  COMMAND cat ${08_day_harib05c_HEADB} ${08_day_harib05c_BOOTB} > ${08_day_harib05c_SYS}
  DEPENDS ${08_day_harib05c_HEADB} 08_day_harib05c_ipl_build
  DEPENDS ${08_day_harib05c_FUNCO}
  DEPENDS ${08_day_harib05c_FONTO}
)
add_custom_target(08_day_harib05c_img
  COMMAND mformat -f 1440 -l HARIBOTEOS -N 0xffffffff -C -B ${08_day_harib05c_IPLB} -i ${08_day_harib05c_OS}
  COMMAND mcopy -i ${08_day_harib05c_OS} ${08_day_harib05c_SYS} ::
  DEPENDS 08_day_harib05c_sys
)

########### next target ###############
set(08_day_harib05c_asmhead_SRCS ${root_SOURCE_DIR}/projects/08_day/harib05c/asmhead.nas)
set(08_day_harib05c_asmhead_OUTS ${root_BINARY_DIR}/projects/08_day/harib05c/asmhead.bin)

add_custom_target(08_day_harib05c_asmhead
  COMMAND ${RAKUSK_EXECUTABLE} ${08_day_harib05c_asmhead_SRCS} ${08_day_harib05c_asmhead_OUTS}
  DEPENDS ${08_day_harib05c_asmhead_SRCS}
)

set(08_day_harib05c_asmhead_WINE_OUTS ${root_BINARY_DIR}/projects/08_day/harib05c/asmhead_wine.bin)
add_custom_target(08_day_harib05c_asmhead_wine
  COMMAND ${WINE} ${WINE_NASK} ${08_day_harib05c_asmhead_SRCS} ${08_day_harib05c_asmhead_WINE_OUTS}
)
add_dependencies(wine 08_day_harib05c_asmhead_wine)

add_custom_target(08_day_harib05c_asmhead_od
  COMMAND ${OD} -t x1 ${08_day_harib05c_asmhead_OUTS}      > ${08_day_harib05c_asmhead_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${08_day_harib05c_asmhead_WINE_OUTS} > ${08_day_harib05c_asmhead_WINE_OUTS}_t.txt
  COMMAND diff -s ${08_day_harib05c_asmhead_OUTS}_f.txt ${08_day_harib05c_asmhead_WINE_OUTS}_t.txt
)

add_dependencies(08_day_harib05c_asmhead_od 08_day_harib05c_asmhead)
add_dependencies(08_day_harib05c_asmhead_od 08_day_harib05c_asmhead_wine)
add_dependencies(od 08_day_harib05c_asmhead_od)

#----------------------------------------------------------
########### next target ###############
set(08_day_harib05c_ipl10_SRCS ${root_SOURCE_DIR}/projects/08_day/harib05c/ipl10.nas)
set(08_day_harib05c_ipl10_OUTS ${root_BINARY_DIR}/projects/08_day/harib05c/ipl10.bin)

add_custom_target(08_day_harib05c_ipl10
  COMMAND ${RAKUSK_EXECUTABLE} ${08_day_harib05c_ipl10_SRCS} ${08_day_harib05c_ipl10_OUTS}
  DEPENDS ${08_day_harib05c_ipl10_SRCS}
)

set(08_day_harib05c_ipl10_WINE_OUTS ${root_BINARY_DIR}/projects/08_day/harib05c/ipl10_wine.bin)
add_custom_target(08_day_harib05c_ipl10_wine
  COMMAND ${WINE} ${WINE_NASK} ${08_day_harib05c_ipl10_SRCS} ${08_day_harib05c_ipl10_WINE_OUTS}
)
add_dependencies(wine 08_day_harib05c_ipl10_wine)

add_custom_target(08_day_harib05c_ipl10_od
  COMMAND ${OD} -t x1 ${08_day_harib05c_ipl10_OUTS}      > ${08_day_harib05c_ipl10_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${08_day_harib05c_ipl10_WINE_OUTS} > ${08_day_harib05c_ipl10_WINE_OUTS}_t.txt
  COMMAND diff -s ${08_day_harib05c_ipl10_OUTS}_f.txt ${08_day_harib05c_ipl10_WINE_OUTS}_t.txt
)

add_dependencies(08_day_harib05c_ipl10_od 08_day_harib05c_ipl10)
add_dependencies(08_day_harib05c_ipl10_od 08_day_harib05c_ipl10_wine)
add_dependencies(od 08_day_harib05c_ipl10_od)

#----------------------------------------------------------
########### next target ###############
set(08_day_harib05c_naskfunc_SRCS ${root_SOURCE_DIR}/projects/08_day/harib05c/naskfunc.nas)
set(08_day_harib05c_naskfunc_OUTS ${root_BINARY_DIR}/projects/08_day/harib05c/naskfunc.img)

add_custom_target(08_day_harib05c_naskfunc
  COMMAND ${RAKUSK_EXECUTABLE} ${08_day_harib05c_naskfunc_SRCS} ${08_day_harib05c_naskfunc_OUTS}
  DEPENDS ${08_day_harib05c_naskfunc_SRCS}
)

add_dependencies(images 08_day_harib05c_naskfunc)
set(08_day_harib05c_naskfunc_WINE_OUTS ${root_BINARY_DIR}/projects/08_day/harib05c/naskfunc_wine.img)
add_custom_target(08_day_harib05c_naskfunc_wine
  COMMAND ${WINE} ${WINE_NASK} ${08_day_harib05c_naskfunc_SRCS} ${08_day_harib05c_naskfunc_WINE_OUTS}
)
add_dependencies(wine 08_day_harib05c_naskfunc_wine)

add_custom_target(08_day_harib05c_naskfunc_od
  COMMAND ${OD} -t x1 ${08_day_harib05c_naskfunc_OUTS}      > ${08_day_harib05c_naskfunc_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${08_day_harib05c_naskfunc_WINE_OUTS} > ${08_day_harib05c_naskfunc_WINE_OUTS}_t.txt
  COMMAND diff -s ${08_day_harib05c_naskfunc_OUTS}_f.txt ${08_day_harib05c_naskfunc_WINE_OUTS}_t.txt
)

add_dependencies(08_day_harib05c_naskfunc_od 08_day_harib05c_naskfunc)
add_dependencies(08_day_harib05c_naskfunc_od 08_day_harib05c_naskfunc_wine)
add_dependencies(od 08_day_harib05c_naskfunc_od)

#----------------------------------------------------------
