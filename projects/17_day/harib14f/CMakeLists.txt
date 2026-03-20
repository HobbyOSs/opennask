#----------------------------------------------------------
message(STATUS "Entering directory projects/17_day/harib14f/")

set(FONT ${root_BINARY_DIR}/src/makefont)
set(B2O  ${root_BINARY_DIR}/src/bin2obj)
set(CONV ${root_BINARY_DIR}/objconv/objconv)
set(17_day_harib14f_WILDOBJ ${root_BINARY_DIR}/projects/17_day/harib14f/*.o)

# Full OS build rules
set(17_day_harib14f_OS    ${root_BINARY_DIR}/projects/17_day/harib14f/os.img)
set(17_day_harib14f_SYS   ${root_BINARY_DIR}/projects/17_day/harib14f/os.sys)
set(17_day_harib14f_IPLB  ${root_BINARY_DIR}/projects/17_day/harib14f/ipl.bin)
set(17_day_harib14f_IPLS  ${root_SOURCE_DIR}/projects/17_day/harib14f/ipl10.nas)
set(17_day_harib14f_HEADB ${root_BINARY_DIR}/projects/17_day/harib14f/asmhead.bin)
set(17_day_harib14f_HEADS ${root_SOURCE_DIR}/projects/17_day/harib14f/asmhead.nas)
set(17_day_harib14f_BOOTB ${root_BINARY_DIR}/projects/17_day/harib14f/boot.bin)
set(17_day_harib14f_CCO   ${root_BINARY_DIR}/projects/17_day/harib14f/boot.o)
set(17_day_harib14f_CCS   ${root_SOURCE_DIR}/projects/17_day/harib14f/*.c)
set(17_day_harib14f_LDS   ${root_SOURCE_DIR}/projects/os.lds)
set(17_day_harib14f_FUNCS ${root_SOURCE_DIR}/projects/17_day/harib14f/naskfunc.nas)
set(17_day_harib14f_FUNCO ${root_BINARY_DIR}/projects/17_day/harib14f/naskfunc.o)
set(17_day_harib14f_FONTS ${root_SOURCE_DIR}/projects/17_day/harib14f/hankaku.txt)
set(17_day_harib14f_FONTB ${root_BINARY_DIR}/projects/17_day/harib14f/hankaku.bin)
set(17_day_harib14f_FONTO ${root_BINARY_DIR}/projects/17_day/harib14f/hankaku.o)
set(17_day_harib14f_LIBGE ${root_BINARY_DIR}/golibc/libgo.a)
set(17_day_harib14f_LIBGC ${root_BINARY_DIR}/projects/17_day/harib14f/libgo.o)

add_custom_target(17_day_harib14f_run
  COMMAND ${QEMU} ${QEMUOPT} ${17_day_harib14f_OS}
  DEPENDS 17_day_harib14f_img
)
add_custom_target(17_day_harib14f_debug
  COMMAND ${QEMU} -s -S ${QEMUOPT} ${17_day_harib14f_OS} ${QEMU_DEBUG_OPT}
  DEPENDS 17_day_harib14f_img
)
add_custom_target(17_day_harib14f_clean
  COMMAND rm -f ${17_day_harib14f_OS}
  COMMAND rm -f ${17_day_harib14f_SYS}
  COMMAND rm -f ${17_day_harib14f_IPLB}
  COMMAND rm -f ${17_day_harib14f_HEADB}
  COMMAND rm -f ${17_day_harib14f_BOOTB}
  COMMAND rm -f ${17_day_harib14f_WILDOBJ}
  COMMAND rm -f ${root_BINARY_DIR}/projects/17_day/harib14f/ipl10.bin
  COMMAND rm -f ${root_BINARY_DIR}/projects/17_day/harib14f/asmhead.bin
  COMMAND rm -f ${root_BINARY_DIR}/projects/17_day/harib14f/*_wine.bin
  COMMAND rm -f ${root_BINARY_DIR}/projects/17_day/harib14f/*.txt
)
add_custom_target(17_day_harib14f_ipl_build
  COMMAND ${RAKUSK_EXECUTABLE} ${17_day_harib14f_IPLS} ${17_day_harib14f_IPLB}
  DEPENDS ${17_day_harib14f_IPLS}
)
add_custom_command(
  OUTPUT ${17_day_harib14f_HEADB}
  COMMAND ${RAKUSK_EXECUTABLE} ${17_day_harib14f_HEADS} ${17_day_harib14f_HEADB}
  DEPENDS ${17_day_harib14f_HEADS}
  COMMENT "Generating asmhead.bin for 17_day_harib14f"
)

add_custom_command(
  OUTPUT ${17_day_harib14f_FUNCO}
  COMMAND ${RAKUSK_EXECUTABLE} ${17_day_harib14f_FUNCS} ${17_day_harib14f_FUNCO}
  DEPENDS ${17_day_harib14f_FUNCS}
  COMMENT "Generating naskfunc.o for 17_day_harib14f"
)
add_custom_command(
  OUTPUT ${17_day_harib14f_FONTO}
  COMMAND ${FONT} ${17_day_harib14f_FONTS} ${17_day_harib14f_FONTB}
  COMMAND ${B2O}  ${17_day_harib14f_FONTB} ${17_day_harib14f_FONTO} _hankaku
  DEPENDS ${17_day_harib14f_FONTS}
  COMMENT "Generating hankaku.o for 17_day_harib14f"
)
add_custom_command(
  OUTPUT ${17_day_harib14f_LIBGC}
  COMMAND ${CONV} -fcoff32 -nu ${17_day_harib14f_LIBGE} ${17_day_harib14f_LIBGC}
  DEPENDS ${17_day_harib14f_LIBGE}
  COMMENT "Converting golibc for 17_day_harib14f"
)
add_custom_target(17_day_harib14f_sys
  COMMAND gcc ${BINOPT} -T ${17_day_harib14f_LDS} ${17_day_harib14f_CCS} ${17_day_harib14f_WILDOBJ} -o ${17_day_harib14f_BOOTB}
  COMMAND cat ${17_day_harib14f_HEADB} ${17_day_harib14f_BOOTB} > ${17_day_harib14f_SYS}
  DEPENDS ${17_day_harib14f_HEADB} 17_day_harib14f_ipl_build
  DEPENDS ${17_day_harib14f_FUNCO}
  DEPENDS ${17_day_harib14f_FONTO} ${17_day_harib14f_LIBGC}
)
add_custom_target(17_day_harib14f_img
  COMMAND mformat -f 1440 -l HARIBOTEOS -N 0xffffffff -C -B ${17_day_harib14f_IPLB} -i ${17_day_harib14f_OS}
  COMMAND mcopy -i ${17_day_harib14f_OS} ${17_day_harib14f_SYS} ::
  DEPENDS 17_day_harib14f_sys
)

########### next target ###############
set(17_day_harib14f_asmhead_SRCS ${root_SOURCE_DIR}/projects/17_day/harib14f/asmhead.nas)
set(17_day_harib14f_asmhead_OUTS ${root_BINARY_DIR}/projects/17_day/harib14f/asmhead.bin)

add_custom_target(17_day_harib14f_asmhead
  COMMAND ${RAKUSK_EXECUTABLE} ${17_day_harib14f_asmhead_SRCS} ${17_day_harib14f_asmhead_OUTS}
  DEPENDS ${17_day_harib14f_asmhead_SRCS}
)

set(17_day_harib14f_asmhead_WINE_OUTS ${root_BINARY_DIR}/projects/17_day/harib14f/asmhead_wine.bin)
add_custom_target(17_day_harib14f_asmhead_wine
  COMMAND ${WINE} ${WINE_NASK} ${17_day_harib14f_asmhead_SRCS} ${17_day_harib14f_asmhead_WINE_OUTS}
)
add_dependencies(wine 17_day_harib14f_asmhead_wine)

add_custom_target(17_day_harib14f_asmhead_od
  COMMAND ${OD} -t x1 ${17_day_harib14f_asmhead_OUTS}      > ${17_day_harib14f_asmhead_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${17_day_harib14f_asmhead_WINE_OUTS} > ${17_day_harib14f_asmhead_WINE_OUTS}_t.txt
  COMMAND diff -s ${17_day_harib14f_asmhead_OUTS}_f.txt ${17_day_harib14f_asmhead_WINE_OUTS}_t.txt
)

add_dependencies(17_day_harib14f_asmhead_od 17_day_harib14f_asmhead)
add_dependencies(17_day_harib14f_asmhead_od 17_day_harib14f_asmhead_wine)
add_dependencies(od 17_day_harib14f_asmhead_od)

#----------------------------------------------------------
########### next target ###############
set(17_day_harib14f_ipl10_SRCS ${root_SOURCE_DIR}/projects/17_day/harib14f/ipl10.nas)
set(17_day_harib14f_ipl10_OUTS ${root_BINARY_DIR}/projects/17_day/harib14f/ipl10.bin)

add_custom_target(17_day_harib14f_ipl10
  COMMAND ${RAKUSK_EXECUTABLE} ${17_day_harib14f_ipl10_SRCS} ${17_day_harib14f_ipl10_OUTS}
  DEPENDS ${17_day_harib14f_ipl10_SRCS}
)

set(17_day_harib14f_ipl10_WINE_OUTS ${root_BINARY_DIR}/projects/17_day/harib14f/ipl10_wine.bin)
add_custom_target(17_day_harib14f_ipl10_wine
  COMMAND ${WINE} ${WINE_NASK} ${17_day_harib14f_ipl10_SRCS} ${17_day_harib14f_ipl10_WINE_OUTS}
)
add_dependencies(wine 17_day_harib14f_ipl10_wine)

add_custom_target(17_day_harib14f_ipl10_od
  COMMAND ${OD} -t x1 ${17_day_harib14f_ipl10_OUTS}      > ${17_day_harib14f_ipl10_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${17_day_harib14f_ipl10_WINE_OUTS} > ${17_day_harib14f_ipl10_WINE_OUTS}_t.txt
  COMMAND diff -s ${17_day_harib14f_ipl10_OUTS}_f.txt ${17_day_harib14f_ipl10_WINE_OUTS}_t.txt
)

add_dependencies(17_day_harib14f_ipl10_od 17_day_harib14f_ipl10)
add_dependencies(17_day_harib14f_ipl10_od 17_day_harib14f_ipl10_wine)
add_dependencies(od 17_day_harib14f_ipl10_od)

#----------------------------------------------------------
########### next target ###############
set(17_day_harib14f_naskfunc_SRCS ${root_SOURCE_DIR}/projects/17_day/harib14f/naskfunc.nas)
set(17_day_harib14f_naskfunc_OUTS ${root_BINARY_DIR}/projects/17_day/harib14f/naskfunc.img)

add_custom_target(17_day_harib14f_naskfunc
  COMMAND ${RAKUSK_EXECUTABLE} ${17_day_harib14f_naskfunc_SRCS} ${17_day_harib14f_naskfunc_OUTS}
  DEPENDS ${17_day_harib14f_naskfunc_SRCS}
)

add_dependencies(images 17_day_harib14f_naskfunc)
set(17_day_harib14f_naskfunc_WINE_OUTS ${root_BINARY_DIR}/projects/17_day/harib14f/naskfunc_wine.img)
add_custom_target(17_day_harib14f_naskfunc_wine
  COMMAND ${WINE} ${WINE_NASK} ${17_day_harib14f_naskfunc_SRCS} ${17_day_harib14f_naskfunc_WINE_OUTS}
)
add_dependencies(wine 17_day_harib14f_naskfunc_wine)

add_custom_target(17_day_harib14f_naskfunc_od
  COMMAND ${OD} -t x1 ${17_day_harib14f_naskfunc_OUTS}      > ${17_day_harib14f_naskfunc_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${17_day_harib14f_naskfunc_WINE_OUTS} > ${17_day_harib14f_naskfunc_WINE_OUTS}_t.txt
  COMMAND diff -s ${17_day_harib14f_naskfunc_OUTS}_f.txt ${17_day_harib14f_naskfunc_WINE_OUTS}_t.txt
)

add_dependencies(17_day_harib14f_naskfunc_od 17_day_harib14f_naskfunc)
add_dependencies(17_day_harib14f_naskfunc_od 17_day_harib14f_naskfunc_wine)
add_dependencies(od 17_day_harib14f_naskfunc_od)

#----------------------------------------------------------
