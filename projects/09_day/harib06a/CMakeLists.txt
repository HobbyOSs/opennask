#----------------------------------------------------------
message(STATUS "Entering directory projects/09_day/harib06a/")

set(FONT ${root_BINARY_DIR}/src/makefont)
set(B2O  ${root_BINARY_DIR}/src/bin2obj)
set(CONV ${root_BINARY_DIR}/objconv/objconv)
set(09_day_harib06a_WILDOBJ ${root_BINARY_DIR}/projects/09_day/harib06a/*.o)

# Full OS build rules
set(09_day_harib06a_OS    ${root_BINARY_DIR}/projects/09_day/harib06a/os.img)
set(09_day_harib06a_SYS   ${root_BINARY_DIR}/projects/09_day/harib06a/os.sys)
set(09_day_harib06a_IPLB  ${root_BINARY_DIR}/projects/09_day/harib06a/ipl.bin)
set(09_day_harib06a_IPLS  ${root_SOURCE_DIR}/projects/09_day/harib06a/ipl10.nas)
set(09_day_harib06a_HEADB ${root_BINARY_DIR}/projects/09_day/harib06a/asmhead.bin)
set(09_day_harib06a_HEADS ${root_SOURCE_DIR}/projects/09_day/harib06a/asmhead.nas)
set(09_day_harib06a_BOOTB ${root_BINARY_DIR}/projects/09_day/harib06a/boot.bin)
set(09_day_harib06a_CCO   ${root_BINARY_DIR}/projects/09_day/harib06a/boot.o)
set(09_day_harib06a_CCS   ${root_SOURCE_DIR}/projects/09_day/harib06a/*.c)
set(09_day_harib06a_LDS   ${root_SOURCE_DIR}/projects/os.lds)
set(09_day_harib06a_FUNCS ${root_SOURCE_DIR}/projects/09_day/harib06a/naskfunc.nas)
set(09_day_harib06a_FUNCO ${root_BINARY_DIR}/projects/09_day/harib06a/naskfunc.o)
set(09_day_harib06a_FONTS ${root_SOURCE_DIR}/projects/09_day/harib06a/hankaku.txt)
set(09_day_harib06a_FONTB ${root_BINARY_DIR}/projects/09_day/harib06a/hankaku.bin)
set(09_day_harib06a_FONTO ${root_BINARY_DIR}/projects/09_day/harib06a/hankaku.o)
set(09_day_harib06a_LIBGE ${root_BINARY_DIR}/golibc/libgo.a)
set(09_day_harib06a_LIBGC ${root_BINARY_DIR}/projects/09_day/harib06a/libgo.o)

add_custom_target(09_day_harib06a_run
  COMMAND ${QEMU} ${QEMUOPT} ${09_day_harib06a_OS}
  DEPENDS 09_day_harib06a_img
)
add_custom_target(09_day_harib06a_debug
  COMMAND ${QEMU} -s -S ${QEMUOPT} ${09_day_harib06a_OS} ${QEMU_DEBUG_OPT}
  DEPENDS 09_day_harib06a_img
)
add_custom_target(09_day_harib06a_clean
  COMMAND rm -f ${09_day_harib06a_OS}
  COMMAND rm -f ${09_day_harib06a_SYS}
  COMMAND rm -f ${09_day_harib06a_IPLB}
  COMMAND rm -f ${09_day_harib06a_HEADB}
  COMMAND rm -f ${09_day_harib06a_BOOTB}
  COMMAND rm -f ${09_day_harib06a_WILDOBJ}
  COMMAND rm -f ${root_BINARY_DIR}/projects/09_day/harib06a/ipl10.bin
  COMMAND rm -f ${root_BINARY_DIR}/projects/09_day/harib06a/asmhead.bin
  COMMAND rm -f ${root_BINARY_DIR}/projects/09_day/harib06a/*_wine.bin
  COMMAND rm -f ${root_BINARY_DIR}/projects/09_day/harib06a/*.txt
)
add_custom_target(09_day_harib06a_ipl_build
  COMMAND ${RAKUSK_EXECUTABLE} ${09_day_harib06a_IPLS} ${09_day_harib06a_IPLB}
  DEPENDS ${09_day_harib06a_IPLS}
)
add_custom_command(
  OUTPUT ${09_day_harib06a_HEADB}
  COMMAND ${RAKUSK_EXECUTABLE} ${09_day_harib06a_HEADS} ${09_day_harib06a_HEADB}
  DEPENDS ${09_day_harib06a_HEADS}
  COMMENT "Generating asmhead.bin for 09_day_harib06a"
)

add_custom_command(
  OUTPUT ${09_day_harib06a_FUNCO}
  COMMAND ${RAKUSK_EXECUTABLE} ${09_day_harib06a_FUNCS} ${09_day_harib06a_FUNCO}
  DEPENDS ${09_day_harib06a_FUNCS}
  COMMENT "Generating naskfunc.o for 09_day_harib06a"
)
add_custom_command(
  OUTPUT ${09_day_harib06a_FONTO}
  COMMAND ${FONT} ${09_day_harib06a_FONTS} ${09_day_harib06a_FONTB}
  COMMAND ${B2O}  ${09_day_harib06a_FONTB} ${09_day_harib06a_FONTO} _hankaku
  DEPENDS ${09_day_harib06a_FONTS}
  COMMENT "Generating hankaku.o for 09_day_harib06a"
)
add_custom_command(
  OUTPUT ${09_day_harib06a_LIBGC}
  COMMAND ${CONV} -fcoff32 -nu ${09_day_harib06a_LIBGE} ${09_day_harib06a_LIBGC}
  DEPENDS ${09_day_harib06a_LIBGE}
  COMMENT "Converting golibc for 09_day_harib06a"
)
add_custom_target(09_day_harib06a_sys
  COMMAND gcc ${BINOPT} -T ${09_day_harib06a_LDS} ${09_day_harib06a_CCS} ${09_day_harib06a_WILDOBJ} -o ${09_day_harib06a_BOOTB}
  COMMAND cat ${09_day_harib06a_HEADB} ${09_day_harib06a_BOOTB} > ${09_day_harib06a_SYS}
  DEPENDS ${09_day_harib06a_HEADB} 09_day_harib06a_ipl_build
  DEPENDS ${09_day_harib06a_FUNCO}
  DEPENDS ${09_day_harib06a_FONTO} ${09_day_harib06a_LIBGC}
)
add_custom_target(09_day_harib06a_img
  COMMAND mformat -f 1440 -l HARIBOTEOS -N 0xffffffff -C -B ${09_day_harib06a_IPLB} -i ${09_day_harib06a_OS}
  COMMAND mcopy -i ${09_day_harib06a_OS} ${09_day_harib06a_SYS} ::
  DEPENDS 09_day_harib06a_sys
)

########### next target ###############
set(09_day_harib06a_asmhead_SRCS ${root_SOURCE_DIR}/projects/09_day/harib06a/asmhead.nas)
set(09_day_harib06a_asmhead_OUTS ${root_BINARY_DIR}/projects/09_day/harib06a/asmhead.bin)

add_custom_target(09_day_harib06a_asmhead
  COMMAND ${RAKUSK_EXECUTABLE} ${09_day_harib06a_asmhead_SRCS} ${09_day_harib06a_asmhead_OUTS}
  DEPENDS ${09_day_harib06a_asmhead_SRCS}
)

set(09_day_harib06a_asmhead_WINE_OUTS ${root_BINARY_DIR}/projects/09_day/harib06a/asmhead_wine.bin)
add_custom_target(09_day_harib06a_asmhead_wine
  COMMAND ${WINE} ${WINE_NASK} ${09_day_harib06a_asmhead_SRCS} ${09_day_harib06a_asmhead_WINE_OUTS}
)
add_dependencies(wine 09_day_harib06a_asmhead_wine)

add_custom_target(09_day_harib06a_asmhead_od
  COMMAND ${OD} -t x1 ${09_day_harib06a_asmhead_OUTS}      > ${09_day_harib06a_asmhead_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${09_day_harib06a_asmhead_WINE_OUTS} > ${09_day_harib06a_asmhead_WINE_OUTS}_t.txt
  COMMAND diff -s ${09_day_harib06a_asmhead_OUTS}_f.txt ${09_day_harib06a_asmhead_WINE_OUTS}_t.txt
)

add_dependencies(09_day_harib06a_asmhead_od 09_day_harib06a_asmhead)
add_dependencies(09_day_harib06a_asmhead_od 09_day_harib06a_asmhead_wine)
add_dependencies(od 09_day_harib06a_asmhead_od)

#----------------------------------------------------------
########### next target ###############
set(09_day_harib06a_ipl10_SRCS ${root_SOURCE_DIR}/projects/09_day/harib06a/ipl10.nas)
set(09_day_harib06a_ipl10_OUTS ${root_BINARY_DIR}/projects/09_day/harib06a/ipl10.bin)

add_custom_target(09_day_harib06a_ipl10
  COMMAND ${RAKUSK_EXECUTABLE} ${09_day_harib06a_ipl10_SRCS} ${09_day_harib06a_ipl10_OUTS}
  DEPENDS ${09_day_harib06a_ipl10_SRCS}
)

set(09_day_harib06a_ipl10_WINE_OUTS ${root_BINARY_DIR}/projects/09_day/harib06a/ipl10_wine.bin)
add_custom_target(09_day_harib06a_ipl10_wine
  COMMAND ${WINE} ${WINE_NASK} ${09_day_harib06a_ipl10_SRCS} ${09_day_harib06a_ipl10_WINE_OUTS}
)
add_dependencies(wine 09_day_harib06a_ipl10_wine)

add_custom_target(09_day_harib06a_ipl10_od
  COMMAND ${OD} -t x1 ${09_day_harib06a_ipl10_OUTS}      > ${09_day_harib06a_ipl10_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${09_day_harib06a_ipl10_WINE_OUTS} > ${09_day_harib06a_ipl10_WINE_OUTS}_t.txt
  COMMAND diff -s ${09_day_harib06a_ipl10_OUTS}_f.txt ${09_day_harib06a_ipl10_WINE_OUTS}_t.txt
)

add_dependencies(09_day_harib06a_ipl10_od 09_day_harib06a_ipl10)
add_dependencies(09_day_harib06a_ipl10_od 09_day_harib06a_ipl10_wine)
add_dependencies(od 09_day_harib06a_ipl10_od)

#----------------------------------------------------------
########### next target ###############
set(09_day_harib06a_naskfunc_SRCS ${root_SOURCE_DIR}/projects/09_day/harib06a/naskfunc.nas)
set(09_day_harib06a_naskfunc_OUTS ${root_BINARY_DIR}/projects/09_day/harib06a/naskfunc.img)

add_custom_target(09_day_harib06a_naskfunc
  COMMAND ${RAKUSK_EXECUTABLE} ${09_day_harib06a_naskfunc_SRCS} ${09_day_harib06a_naskfunc_OUTS}
  DEPENDS ${09_day_harib06a_naskfunc_SRCS}
)

add_dependencies(images 09_day_harib06a_naskfunc)
set(09_day_harib06a_naskfunc_WINE_OUTS ${root_BINARY_DIR}/projects/09_day/harib06a/naskfunc_wine.img)
add_custom_target(09_day_harib06a_naskfunc_wine
  COMMAND ${WINE} ${WINE_NASK} ${09_day_harib06a_naskfunc_SRCS} ${09_day_harib06a_naskfunc_WINE_OUTS}
)
add_dependencies(wine 09_day_harib06a_naskfunc_wine)

add_custom_target(09_day_harib06a_naskfunc_od
  COMMAND ${OD} -t x1 ${09_day_harib06a_naskfunc_OUTS}      > ${09_day_harib06a_naskfunc_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${09_day_harib06a_naskfunc_WINE_OUTS} > ${09_day_harib06a_naskfunc_WINE_OUTS}_t.txt
  COMMAND diff -s ${09_day_harib06a_naskfunc_OUTS}_f.txt ${09_day_harib06a_naskfunc_WINE_OUTS}_t.txt
)

add_dependencies(09_day_harib06a_naskfunc_od 09_day_harib06a_naskfunc)
add_dependencies(09_day_harib06a_naskfunc_od 09_day_harib06a_naskfunc_wine)
add_dependencies(od 09_day_harib06a_naskfunc_od)

#----------------------------------------------------------
