#----------------------------------------------------------
message(STATUS "Entering directory projects/08_day/harib05d/")

set(FONT ${RAKUSK_EXECUTABLE} --makefont)
set(B2O  ${RAKUSK_EXECUTABLE} --bin2obj)
set(CONV ${root_BINARY_DIR}/objconv/objconv)
set(08_day_harib05d_WILDOBJ ${root_BINARY_DIR}/projects/08_day/harib05d/*.o)

# Full OS build rules
set(08_day_harib05d_OS    ${root_BINARY_DIR}/projects/08_day/harib05d/os.img)
set(08_day_harib05d_SYS   ${root_BINARY_DIR}/projects/08_day/harib05d/os.sys)
set(08_day_harib05d_IPLB  ${root_BINARY_DIR}/projects/08_day/harib05d/ipl.bin)
set(08_day_harib05d_IPLS  ${root_SOURCE_DIR}/projects/08_day/harib05d/ipl10.nas)
set(08_day_harib05d_HEADB ${root_BINARY_DIR}/projects/08_day/harib05d/asmhead.bin)
set(08_day_harib05d_HEADS ${root_SOURCE_DIR}/projects/08_day/harib05d/asmhead.nas)
set(08_day_harib05d_BOOTB ${root_BINARY_DIR}/projects/08_day/harib05d/boot.bin)
set(08_day_harib05d_CCO   ${root_BINARY_DIR}/projects/08_day/harib05d/boot.o)
set(08_day_harib05d_CCS   ${root_SOURCE_DIR}/projects/08_day/harib05d/*.c)
set(08_day_harib05d_LDS   ${root_SOURCE_DIR}/projects/os.lds)
set(08_day_harib05d_FUNCS ${root_SOURCE_DIR}/projects/08_day/harib05d/naskfunc.nas)
set(08_day_harib05d_FUNCO ${root_BINARY_DIR}/projects/08_day/harib05d/naskfunc.o)
set(08_day_harib05d_FONTS ${root_SOURCE_DIR}/projects/08_day/harib05d/hankaku.txt)
set(08_day_harib05d_FONTB ${root_BINARY_DIR}/projects/08_day/harib05d/hankaku.bin)
set(08_day_harib05d_FONTO ${root_BINARY_DIR}/projects/08_day/harib05d/hankaku.o)
set(08_day_harib05d_LIBGE ${root_BINARY_DIR}/golibc/libgo.a)
set(08_day_harib05d_LIBGC ${root_BINARY_DIR}/projects/08_day/harib05d/libgo.o)

add_custom_target(08_day_harib05d_run
  COMMAND ${QEMU} ${QEMUOPT} ${08_day_harib05d_OS}
  DEPENDS 08_day_harib05d_img
)
add_custom_target(08_day_harib05d_debug
  COMMAND ${QEMU} -s -S ${QEMUOPT} ${08_day_harib05d_OS} ${QEMU_DEBUG_OPT}
  DEPENDS 08_day_harib05d_img
)
add_custom_target(08_day_harib05d_clean
  COMMAND rm -f ${08_day_harib05d_OS}
  COMMAND rm -f ${08_day_harib05d_SYS}
  COMMAND rm -f ${08_day_harib05d_IPLB}
  COMMAND rm -f ${08_day_harib05d_HEADB}
  COMMAND rm -f ${08_day_harib05d_BOOTB}
  COMMAND rm -f ${08_day_harib05d_WILDOBJ}
  COMMAND rm -f ${root_BINARY_DIR}/projects/08_day/harib05d/ipl10.bin
  COMMAND rm -f ${root_BINARY_DIR}/projects/08_day/harib05d/asmhead.bin
  COMMAND rm -f ${root_BINARY_DIR}/projects/08_day/harib05d/*_wine.bin
  COMMAND rm -f ${root_BINARY_DIR}/projects/08_day/harib05d/*.txt
)
add_custom_target(08_day_harib05d_ipl_build
  COMMAND ${RAKUSK_EXECUTABLE} ${08_day_harib05d_IPLS} ${08_day_harib05d_IPLB}
  DEPENDS ${08_day_harib05d_IPLS}
)
add_custom_command(
  OUTPUT ${08_day_harib05d_HEADB}
  COMMAND ${RAKUSK_EXECUTABLE} ${08_day_harib05d_HEADS} ${08_day_harib05d_HEADB}
  DEPENDS ${08_day_harib05d_HEADS}
  COMMENT "Generating asmhead.bin for 08_day_harib05d"
)

add_custom_target(08_day_harib05d_sys
  COMMAND ${FONT} ${08_day_harib05d_FONTS} ${08_day_harib05d_FONTB}
  COMMAND ${B2O}  ${08_day_harib05d_FONTB} ${08_day_harib05d_FONTO} _hankaku
  COMMAND ${CONV} -fcoff32 -nu ${08_day_harib05d_LIBGE} ${08_day_harib05d_LIBGC}
  COMMAND ${RAKUSK_EXECUTABLE} ${08_day_harib05d_FUNCS} ${08_day_harib05d_FUNCO}
  COMMAND gcc ${BINOPT} -T ${08_day_harib05d_LDS} ${08_day_harib05d_CCS} ${08_day_harib05d_WILDOBJ} -o ${08_day_harib05d_BOOTB}
  COMMAND cat ${08_day_harib05d_HEADB} ${08_day_harib05d_BOOTB} > ${08_day_harib05d_SYS}
  DEPENDS ${08_day_harib05d_HEADB} 08_day_harib05d_ipl_build
  DEPENDS ${08_day_harib05d_FUNCO}
  DEPENDS ${08_day_harib05d_FONTO}
)
add_custom_target(08_day_harib05d_img
  COMMAND mformat -f 1440 -l HARIBOTEOS -N 0xffffffff -C -B ${08_day_harib05d_IPLB} -i ${08_day_harib05d_OS}
  COMMAND mcopy -i ${08_day_harib05d_OS} ${08_day_harib05d_SYS} ::
  DEPENDS 08_day_harib05d_sys
)

########### next target ###############
set(08_day_harib05d_asmhead_SRCS ${root_SOURCE_DIR}/projects/08_day/harib05d/asmhead.nas)
set(08_day_harib05d_asmhead_OUTS ${root_BINARY_DIR}/projects/08_day/harib05d/asmhead.bin)

add_custom_target(08_day_harib05d_asmhead
  COMMAND ${RAKUSK_EXECUTABLE} ${08_day_harib05d_asmhead_SRCS} ${08_day_harib05d_asmhead_OUTS}
  DEPENDS ${08_day_harib05d_asmhead_SRCS}
)

set(08_day_harib05d_asmhead_WINE_OUTS ${root_BINARY_DIR}/projects/08_day/harib05d/asmhead_wine.bin)
add_custom_target(08_day_harib05d_asmhead_wine
  COMMAND ${WINE} ${WINE_NASK} ${08_day_harib05d_asmhead_SRCS} ${08_day_harib05d_asmhead_WINE_OUTS}
)
add_dependencies(wine 08_day_harib05d_asmhead_wine)

add_custom_target(08_day_harib05d_asmhead_od
  COMMAND ${OD} -t x1 ${08_day_harib05d_asmhead_OUTS}      > ${08_day_harib05d_asmhead_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${08_day_harib05d_asmhead_WINE_OUTS} > ${08_day_harib05d_asmhead_WINE_OUTS}_t.txt
  COMMAND diff -s ${08_day_harib05d_asmhead_OUTS}_f.txt ${08_day_harib05d_asmhead_WINE_OUTS}_t.txt
)

add_dependencies(08_day_harib05d_asmhead_od 08_day_harib05d_asmhead)
add_dependencies(08_day_harib05d_asmhead_od 08_day_harib05d_asmhead_wine)
add_dependencies(od 08_day_harib05d_asmhead_od)

#----------------------------------------------------------
########### next target ###############
set(08_day_harib05d_ipl10_SRCS ${root_SOURCE_DIR}/projects/08_day/harib05d/ipl10.nas)
set(08_day_harib05d_ipl10_OUTS ${root_BINARY_DIR}/projects/08_day/harib05d/ipl10.bin)

add_custom_target(08_day_harib05d_ipl10
  COMMAND ${RAKUSK_EXECUTABLE} ${08_day_harib05d_ipl10_SRCS} ${08_day_harib05d_ipl10_OUTS}
  DEPENDS ${08_day_harib05d_ipl10_SRCS}
)

set(08_day_harib05d_ipl10_WINE_OUTS ${root_BINARY_DIR}/projects/08_day/harib05d/ipl10_wine.bin)
add_custom_target(08_day_harib05d_ipl10_wine
  COMMAND ${WINE} ${WINE_NASK} ${08_day_harib05d_ipl10_SRCS} ${08_day_harib05d_ipl10_WINE_OUTS}
)
add_dependencies(wine 08_day_harib05d_ipl10_wine)

add_custom_target(08_day_harib05d_ipl10_od
  COMMAND ${OD} -t x1 ${08_day_harib05d_ipl10_OUTS}      > ${08_day_harib05d_ipl10_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${08_day_harib05d_ipl10_WINE_OUTS} > ${08_day_harib05d_ipl10_WINE_OUTS}_t.txt
  COMMAND diff -s ${08_day_harib05d_ipl10_OUTS}_f.txt ${08_day_harib05d_ipl10_WINE_OUTS}_t.txt
)

add_dependencies(08_day_harib05d_ipl10_od 08_day_harib05d_ipl10)
add_dependencies(08_day_harib05d_ipl10_od 08_day_harib05d_ipl10_wine)
add_dependencies(od 08_day_harib05d_ipl10_od)

#----------------------------------------------------------
########### next target ###############
set(08_day_harib05d_naskfunc_SRCS ${root_SOURCE_DIR}/projects/08_day/harib05d/naskfunc.nas)
set(08_day_harib05d_naskfunc_OUTS ${root_BINARY_DIR}/projects/08_day/harib05d/naskfunc.img)

add_custom_target(08_day_harib05d_naskfunc
  COMMAND ${RAKUSK_EXECUTABLE} ${08_day_harib05d_naskfunc_SRCS} ${08_day_harib05d_naskfunc_OUTS}
  DEPENDS ${08_day_harib05d_naskfunc_SRCS}
)

add_dependencies(images 08_day_harib05d_naskfunc)
set(08_day_harib05d_naskfunc_WINE_OUTS ${root_BINARY_DIR}/projects/08_day/harib05d/naskfunc_wine.img)
add_custom_target(08_day_harib05d_naskfunc_wine
  COMMAND ${WINE} ${WINE_NASK} ${08_day_harib05d_naskfunc_SRCS} ${08_day_harib05d_naskfunc_WINE_OUTS}
)
add_dependencies(wine 08_day_harib05d_naskfunc_wine)

add_custom_target(08_day_harib05d_naskfunc_od
  COMMAND ${OD} -t x1 ${08_day_harib05d_naskfunc_OUTS}      > ${08_day_harib05d_naskfunc_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${08_day_harib05d_naskfunc_WINE_OUTS} > ${08_day_harib05d_naskfunc_WINE_OUTS}_t.txt
  COMMAND diff -s ${08_day_harib05d_naskfunc_OUTS}_f.txt ${08_day_harib05d_naskfunc_WINE_OUTS}_t.txt
)

add_dependencies(08_day_harib05d_naskfunc_od 08_day_harib05d_naskfunc)
add_dependencies(08_day_harib05d_naskfunc_od 08_day_harib05d_naskfunc_wine)
add_dependencies(od 08_day_harib05d_naskfunc_od)

#----------------------------------------------------------
