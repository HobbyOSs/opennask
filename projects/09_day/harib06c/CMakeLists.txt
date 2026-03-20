#----------------------------------------------------------
message(STATUS "Entering directory projects/09_day/harib06c/")

set(FONT ${root_BINARY_DIR}/src/makefont)
set(B2O  ${root_BINARY_DIR}/src/bin2obj)
set(CONV ${root_BINARY_DIR}/objconv/objconv)
set(09_day_harib06c_WILDOBJ ${root_BINARY_DIR}/projects/09_day/harib06c/*.o)

# Full OS build rules
set(09_day_harib06c_OS    ${root_BINARY_DIR}/projects/09_day/harib06c/os.img)
set(09_day_harib06c_SYS   ${root_BINARY_DIR}/projects/09_day/harib06c/os.sys)
set(09_day_harib06c_IPLB  ${root_BINARY_DIR}/projects/09_day/harib06c/ipl.bin)
set(09_day_harib06c_IPLS  ${root_SOURCE_DIR}/projects/09_day/harib06c/ipl10.nas)
set(09_day_harib06c_HEADB ${root_BINARY_DIR}/projects/09_day/harib06c/asmhead.bin)
set(09_day_harib06c_HEADS ${root_SOURCE_DIR}/projects/09_day/harib06c/asmhead.nas)
set(09_day_harib06c_BOOTB ${root_BINARY_DIR}/projects/09_day/harib06c/boot.bin)
set(09_day_harib06c_CCO   ${root_BINARY_DIR}/projects/09_day/harib06c/boot.o)
set(09_day_harib06c_CCS   ${root_SOURCE_DIR}/projects/09_day/harib06c/*.c)
set(09_day_harib06c_LDS   ${root_SOURCE_DIR}/projects/os.lds)
set(09_day_harib06c_FUNCS ${root_SOURCE_DIR}/projects/09_day/harib06c/naskfunc.nas)
set(09_day_harib06c_FUNCO ${root_BINARY_DIR}/projects/09_day/harib06c/naskfunc.o)
set(09_day_harib06c_FONTS ${root_SOURCE_DIR}/projects/09_day/harib06c/hankaku.txt)
set(09_day_harib06c_FONTB ${root_BINARY_DIR}/projects/09_day/harib06c/hankaku.bin)
set(09_day_harib06c_FONTO ${root_BINARY_DIR}/projects/09_day/harib06c/hankaku.o)
set(09_day_harib06c_LIBGE ${root_BINARY_DIR}/golibc/libgo.a)
set(09_day_harib06c_LIBGC ${root_BINARY_DIR}/projects/09_day/harib06c/libgo.o)

add_custom_target(09_day_harib06c_run
  COMMAND ${QEMU} ${QEMUOPT} ${09_day_harib06c_OS}
  DEPENDS 09_day_harib06c_img
)
add_custom_target(09_day_harib06c_debug
  COMMAND ${QEMU} -s -S ${QEMUOPT} ${09_day_harib06c_OS} ${QEMU_DEBUG_OPT}
  DEPENDS 09_day_harib06c_img
)
add_custom_target(09_day_harib06c_clean
  COMMAND rm -f ${09_day_harib06c_OS}
  COMMAND rm -f ${09_day_harib06c_SYS}
  COMMAND rm -f ${09_day_harib06c_IPLB}
  COMMAND rm -f ${09_day_harib06c_HEADB}
  COMMAND rm -f ${09_day_harib06c_BOOTB}
  COMMAND rm -f ${09_day_harib06c_WILDOBJ}
  COMMAND rm -f ${root_BINARY_DIR}/projects/09_day/harib06c/ipl10.bin
  COMMAND rm -f ${root_BINARY_DIR}/projects/09_day/harib06c/asmhead.bin
  COMMAND rm -f ${root_BINARY_DIR}/projects/09_day/harib06c/*_wine.bin
  COMMAND rm -f ${root_BINARY_DIR}/projects/09_day/harib06c/*.txt
)
add_custom_target(09_day_harib06c_ipl_build
  COMMAND ${RAKUSK_EXECUTABLE} ${09_day_harib06c_IPLS} ${09_day_harib06c_IPLB}
  DEPENDS ${09_day_harib06c_IPLS}
)
add_custom_command(
  OUTPUT ${09_day_harib06c_HEADB}
  COMMAND ${RAKUSK_EXECUTABLE} ${09_day_harib06c_HEADS} ${09_day_harib06c_HEADB}
  DEPENDS ${09_day_harib06c_HEADS}
  COMMENT "Generating asmhead.bin for 09_day_harib06c"
)

add_custom_command(
  OUTPUT ${09_day_harib06c_FUNCO}
  COMMAND ${RAKUSK_EXECUTABLE} ${09_day_harib06c_FUNCS} ${09_day_harib06c_FUNCO}
  DEPENDS ${09_day_harib06c_FUNCS}
  COMMENT "Generating naskfunc.o for 09_day_harib06c"
)
add_custom_command(
  OUTPUT ${09_day_harib06c_FONTO}
  COMMAND ${FONT} ${09_day_harib06c_FONTS} ${09_day_harib06c_FONTB}
  COMMAND ${B2O}  ${09_day_harib06c_FONTB} ${09_day_harib06c_FONTO} _hankaku
  DEPENDS ${09_day_harib06c_FONTS}
  COMMENT "Generating hankaku.o for 09_day_harib06c"
)
add_custom_command(
  OUTPUT ${09_day_harib06c_LIBGC}
  COMMAND ${CONV} -fcoff32 -nu ${09_day_harib06c_LIBGE} ${09_day_harib06c_LIBGC}
  DEPENDS ${09_day_harib06c_LIBGE}
  COMMENT "Converting golibc for 09_day_harib06c"
)
add_custom_target(09_day_harib06c_sys
  COMMAND gcc ${BINOPT} -T ${09_day_harib06c_LDS} ${09_day_harib06c_CCS} ${09_day_harib06c_WILDOBJ} -o ${09_day_harib06c_BOOTB}
  COMMAND cat ${09_day_harib06c_HEADB} ${09_day_harib06c_BOOTB} > ${09_day_harib06c_SYS}
  DEPENDS ${09_day_harib06c_HEADB} 09_day_harib06c_ipl_build
  DEPENDS ${09_day_harib06c_FUNCO}
  DEPENDS ${09_day_harib06c_FONTO} ${09_day_harib06c_LIBGC}
)
add_custom_target(09_day_harib06c_img
  COMMAND mformat -f 1440 -l HARIBOTEOS -N 0xffffffff -C -B ${09_day_harib06c_IPLB} -i ${09_day_harib06c_OS}
  COMMAND mcopy -i ${09_day_harib06c_OS} ${09_day_harib06c_SYS} ::
  DEPENDS 09_day_harib06c_sys
)

########### next target ###############
set(09_day_harib06c_asmhead_SRCS ${root_SOURCE_DIR}/projects/09_day/harib06c/asmhead.nas)
set(09_day_harib06c_asmhead_OUTS ${root_BINARY_DIR}/projects/09_day/harib06c/asmhead.bin)

add_custom_target(09_day_harib06c_asmhead
  COMMAND ${RAKUSK_EXECUTABLE} ${09_day_harib06c_asmhead_SRCS} ${09_day_harib06c_asmhead_OUTS}
  DEPENDS ${09_day_harib06c_asmhead_SRCS}
)

set(09_day_harib06c_asmhead_WINE_OUTS ${root_BINARY_DIR}/projects/09_day/harib06c/asmhead_wine.bin)
add_custom_target(09_day_harib06c_asmhead_wine
  COMMAND ${WINE} ${WINE_NASK} ${09_day_harib06c_asmhead_SRCS} ${09_day_harib06c_asmhead_WINE_OUTS}
)
add_dependencies(wine 09_day_harib06c_asmhead_wine)

add_custom_target(09_day_harib06c_asmhead_od
  COMMAND ${OD} -t x1 ${09_day_harib06c_asmhead_OUTS}      > ${09_day_harib06c_asmhead_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${09_day_harib06c_asmhead_WINE_OUTS} > ${09_day_harib06c_asmhead_WINE_OUTS}_t.txt
  COMMAND diff -s ${09_day_harib06c_asmhead_OUTS}_f.txt ${09_day_harib06c_asmhead_WINE_OUTS}_t.txt
)

add_dependencies(09_day_harib06c_asmhead_od 09_day_harib06c_asmhead)
add_dependencies(09_day_harib06c_asmhead_od 09_day_harib06c_asmhead_wine)
add_dependencies(od 09_day_harib06c_asmhead_od)

#----------------------------------------------------------
########### next target ###############
set(09_day_harib06c_ipl10_SRCS ${root_SOURCE_DIR}/projects/09_day/harib06c/ipl10.nas)
set(09_day_harib06c_ipl10_OUTS ${root_BINARY_DIR}/projects/09_day/harib06c/ipl10.bin)

add_custom_target(09_day_harib06c_ipl10
  COMMAND ${RAKUSK_EXECUTABLE} ${09_day_harib06c_ipl10_SRCS} ${09_day_harib06c_ipl10_OUTS}
  DEPENDS ${09_day_harib06c_ipl10_SRCS}
)

set(09_day_harib06c_ipl10_WINE_OUTS ${root_BINARY_DIR}/projects/09_day/harib06c/ipl10_wine.bin)
add_custom_target(09_day_harib06c_ipl10_wine
  COMMAND ${WINE} ${WINE_NASK} ${09_day_harib06c_ipl10_SRCS} ${09_day_harib06c_ipl10_WINE_OUTS}
)
add_dependencies(wine 09_day_harib06c_ipl10_wine)

add_custom_target(09_day_harib06c_ipl10_od
  COMMAND ${OD} -t x1 ${09_day_harib06c_ipl10_OUTS}      > ${09_day_harib06c_ipl10_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${09_day_harib06c_ipl10_WINE_OUTS} > ${09_day_harib06c_ipl10_WINE_OUTS}_t.txt
  COMMAND diff -s ${09_day_harib06c_ipl10_OUTS}_f.txt ${09_day_harib06c_ipl10_WINE_OUTS}_t.txt
)

add_dependencies(09_day_harib06c_ipl10_od 09_day_harib06c_ipl10)
add_dependencies(09_day_harib06c_ipl10_od 09_day_harib06c_ipl10_wine)
add_dependencies(od 09_day_harib06c_ipl10_od)

#----------------------------------------------------------
########### next target ###############
set(09_day_harib06c_naskfunc_SRCS ${root_SOURCE_DIR}/projects/09_day/harib06c/naskfunc.nas)
set(09_day_harib06c_naskfunc_OUTS ${root_BINARY_DIR}/projects/09_day/harib06c/naskfunc.img)

add_custom_target(09_day_harib06c_naskfunc
  COMMAND ${RAKUSK_EXECUTABLE} ${09_day_harib06c_naskfunc_SRCS} ${09_day_harib06c_naskfunc_OUTS}
  DEPENDS ${09_day_harib06c_naskfunc_SRCS}
)

add_dependencies(images 09_day_harib06c_naskfunc)
set(09_day_harib06c_naskfunc_WINE_OUTS ${root_BINARY_DIR}/projects/09_day/harib06c/naskfunc_wine.img)
add_custom_target(09_day_harib06c_naskfunc_wine
  COMMAND ${WINE} ${WINE_NASK} ${09_day_harib06c_naskfunc_SRCS} ${09_day_harib06c_naskfunc_WINE_OUTS}
)
add_dependencies(wine 09_day_harib06c_naskfunc_wine)

add_custom_target(09_day_harib06c_naskfunc_od
  COMMAND ${OD} -t x1 ${09_day_harib06c_naskfunc_OUTS}      > ${09_day_harib06c_naskfunc_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${09_day_harib06c_naskfunc_WINE_OUTS} > ${09_day_harib06c_naskfunc_WINE_OUTS}_t.txt
  COMMAND diff -s ${09_day_harib06c_naskfunc_OUTS}_f.txt ${09_day_harib06c_naskfunc_WINE_OUTS}_t.txt
)

add_dependencies(09_day_harib06c_naskfunc_od 09_day_harib06c_naskfunc)
add_dependencies(09_day_harib06c_naskfunc_od 09_day_harib06c_naskfunc_wine)
add_dependencies(od 09_day_harib06c_naskfunc_od)

#----------------------------------------------------------
