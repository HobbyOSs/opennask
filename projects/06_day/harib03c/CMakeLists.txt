#----------------------------------------------------------
message(STATUS "Entering directory projects/06_day/harib03c/")

set(FONT ${root_BINARY_DIR}/src/makefont)
set(B2O  ${root_BINARY_DIR}/src/bin2obj)
set(CONV ${root_BINARY_DIR}/objconv/objconv)
set(06_day_harib03c_WILDOBJ ${root_BINARY_DIR}/projects/06_day/harib03c/*.o)

# Full OS build rules
set(06_day_harib03c_OS    ${root_BINARY_DIR}/projects/06_day/harib03c/os.img)
set(06_day_harib03c_SYS   ${root_BINARY_DIR}/projects/06_day/harib03c/os.sys)
set(06_day_harib03c_IPLB  ${root_BINARY_DIR}/projects/06_day/harib03c/ipl.bin)
set(06_day_harib03c_IPLS  ${root_SOURCE_DIR}/projects/06_day/harib03c/ipl10.nas)
set(06_day_harib03c_HEADB ${root_BINARY_DIR}/projects/06_day/harib03c/asmhead.bin)
set(06_day_harib03c_HEADS ${root_SOURCE_DIR}/projects/06_day/harib03c/asmhead.nas)
set(06_day_harib03c_BOOTB ${root_BINARY_DIR}/projects/06_day/harib03c/boot.bin)
set(06_day_harib03c_CCO   ${root_BINARY_DIR}/projects/06_day/harib03c/boot.o)
set(06_day_harib03c_CCS   ${root_SOURCE_DIR}/projects/06_day/harib03c/*.c)
set(06_day_harib03c_LDS   ${root_SOURCE_DIR}/projects/os.lds)
set(06_day_harib03c_FUNCS ${root_SOURCE_DIR}/projects/06_day/harib03c/naskfunc.nas)
set(06_day_harib03c_FUNCO ${root_BINARY_DIR}/projects/06_day/harib03c/naskfunc.o)
set(06_day_harib03c_FONTS ${root_SOURCE_DIR}/projects/06_day/harib03c/hankaku.txt)
set(06_day_harib03c_FONTB ${root_BINARY_DIR}/projects/06_day/harib03c/hankaku.bin)
set(06_day_harib03c_FONTO ${root_BINARY_DIR}/projects/06_day/harib03c/hankaku.o)
set(06_day_harib03c_LIBGE ${root_BINARY_DIR}/golibc/libgo.a)
set(06_day_harib03c_LIBGC ${root_BINARY_DIR}/projects/06_day/harib03c/libgo.o)

add_custom_target(06_day_harib03c_run
  COMMAND ${QEMU} ${QEMUOPT} ${06_day_harib03c_OS}
  DEPENDS 06_day_harib03c_img
)
add_custom_target(06_day_harib03c_debug
  COMMAND ${QEMU} -s -S ${QEMUOPT} ${06_day_harib03c_OS} ${QEMU_DEBUG_OPT}
  DEPENDS 06_day_harib03c_img
)
add_custom_target(06_day_harib03c_clean
  COMMAND rm -f ${06_day_harib03c_OS}
  COMMAND rm -f ${06_day_harib03c_SYS}
  COMMAND rm -f ${06_day_harib03c_IPLB}
  COMMAND rm -f ${06_day_harib03c_HEADB}
  COMMAND rm -f ${06_day_harib03c_BOOTB}
  COMMAND rm -f ${06_day_harib03c_WILDOBJ}
  COMMAND rm -f ${root_BINARY_DIR}/projects/06_day/harib03c/ipl10.bin
  COMMAND rm -f ${root_BINARY_DIR}/projects/06_day/harib03c/asmhead.bin
  COMMAND rm -f ${root_BINARY_DIR}/projects/06_day/harib03c/*_wine.bin
  COMMAND rm -f ${root_BINARY_DIR}/projects/06_day/harib03c/*.txt
)
add_custom_target(06_day_harib03c_ipl_build
  COMMAND ${RAKUSK_EXECUTABLE} ${06_day_harib03c_IPLS} ${06_day_harib03c_IPLB}
  DEPENDS ${06_day_harib03c_IPLS}
)
add_custom_command(
  OUTPUT ${06_day_harib03c_HEADB}
  COMMAND ${RAKUSK_EXECUTABLE} ${06_day_harib03c_HEADS} ${06_day_harib03c_HEADB}
  DEPENDS ${06_day_harib03c_HEADS}
  COMMENT "Generating asmhead.bin for 06_day_harib03c"
)

add_custom_command(
  OUTPUT ${06_day_harib03c_FUNCO}
  COMMAND ${RAKUSK_EXECUTABLE} ${06_day_harib03c_FUNCS} ${06_day_harib03c_FUNCO}
  DEPENDS ${06_day_harib03c_FUNCS}
  COMMENT "Generating naskfunc.o for 06_day_harib03c"
)
add_custom_command(
  OUTPUT ${06_day_harib03c_FONTO}
  COMMAND ${FONT} ${06_day_harib03c_FONTS} ${06_day_harib03c_FONTB}
  COMMAND ${B2O}  ${06_day_harib03c_FONTB} ${06_day_harib03c_FONTO} _hankaku
  DEPENDS ${06_day_harib03c_FONTS}
  COMMENT "Generating hankaku.o for 06_day_harib03c"
)
add_custom_command(
  OUTPUT ${06_day_harib03c_LIBGC}
  COMMAND ${CONV} -fcoff32 -nu ${06_day_harib03c_LIBGE} ${06_day_harib03c_LIBGC}
  DEPENDS ${06_day_harib03c_LIBGE}
  COMMENT "Converting golibc for 06_day_harib03c"
)
add_custom_target(06_day_harib03c_sys
  COMMAND gcc ${BINOPT} -T ${06_day_harib03c_LDS} ${06_day_harib03c_CCS} ${06_day_harib03c_WILDOBJ} -o ${06_day_harib03c_BOOTB}
  COMMAND cat ${06_day_harib03c_HEADB} ${06_day_harib03c_BOOTB} > ${06_day_harib03c_SYS}
  DEPENDS ${06_day_harib03c_HEADB} 06_day_harib03c_ipl_build
  DEPENDS ${06_day_harib03c_FUNCO}
  DEPENDS ${06_day_harib03c_FONTO} ${06_day_harib03c_LIBGC}
)
add_custom_target(06_day_harib03c_img
  COMMAND mformat -f 1440 -l HARIBOTEOS -N 0xffffffff -C -B ${06_day_harib03c_IPLB} -i ${06_day_harib03c_OS}
  COMMAND mcopy -i ${06_day_harib03c_OS} ${06_day_harib03c_SYS} ::
  DEPENDS 06_day_harib03c_sys
)

########### next target ###############
set(06_day_harib03c_asmhead_SRCS ${root_SOURCE_DIR}/projects/06_day/harib03c/asmhead.nas)
set(06_day_harib03c_asmhead_OUTS ${root_BINARY_DIR}/projects/06_day/harib03c/asmhead.bin)

add_custom_target(06_day_harib03c_asmhead
  COMMAND ${RAKUSK_EXECUTABLE} ${06_day_harib03c_asmhead_SRCS} ${06_day_harib03c_asmhead_OUTS}
  DEPENDS ${06_day_harib03c_asmhead_SRCS}
)

set(06_day_harib03c_asmhead_WINE_OUTS ${root_BINARY_DIR}/projects/06_day/harib03c/asmhead_wine.bin)
add_custom_target(06_day_harib03c_asmhead_wine
  COMMAND ${WINE} ${WINE_NASK} ${06_day_harib03c_asmhead_SRCS} ${06_day_harib03c_asmhead_WINE_OUTS}
)
add_dependencies(wine 06_day_harib03c_asmhead_wine)

add_custom_target(06_day_harib03c_asmhead_od
  COMMAND ${OD} -t x1 ${06_day_harib03c_asmhead_OUTS}      > ${06_day_harib03c_asmhead_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${06_day_harib03c_asmhead_WINE_OUTS} > ${06_day_harib03c_asmhead_WINE_OUTS}_t.txt
  COMMAND diff -s ${06_day_harib03c_asmhead_OUTS}_f.txt ${06_day_harib03c_asmhead_WINE_OUTS}_t.txt
)

add_dependencies(06_day_harib03c_asmhead_od 06_day_harib03c_asmhead)
add_dependencies(06_day_harib03c_asmhead_od 06_day_harib03c_asmhead_wine)
add_dependencies(od 06_day_harib03c_asmhead_od)

#----------------------------------------------------------
########### next target ###############
set(06_day_harib03c_ipl10_SRCS ${root_SOURCE_DIR}/projects/06_day/harib03c/ipl10.nas)
set(06_day_harib03c_ipl10_OUTS ${root_BINARY_DIR}/projects/06_day/harib03c/ipl10.bin)

add_custom_target(06_day_harib03c_ipl10
  COMMAND ${RAKUSK_EXECUTABLE} ${06_day_harib03c_ipl10_SRCS} ${06_day_harib03c_ipl10_OUTS}
  DEPENDS ${06_day_harib03c_ipl10_SRCS}
)

set(06_day_harib03c_ipl10_WINE_OUTS ${root_BINARY_DIR}/projects/06_day/harib03c/ipl10_wine.bin)
add_custom_target(06_day_harib03c_ipl10_wine
  COMMAND ${WINE} ${WINE_NASK} ${06_day_harib03c_ipl10_SRCS} ${06_day_harib03c_ipl10_WINE_OUTS}
)
add_dependencies(wine 06_day_harib03c_ipl10_wine)

add_custom_target(06_day_harib03c_ipl10_od
  COMMAND ${OD} -t x1 ${06_day_harib03c_ipl10_OUTS}      > ${06_day_harib03c_ipl10_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${06_day_harib03c_ipl10_WINE_OUTS} > ${06_day_harib03c_ipl10_WINE_OUTS}_t.txt
  COMMAND diff -s ${06_day_harib03c_ipl10_OUTS}_f.txt ${06_day_harib03c_ipl10_WINE_OUTS}_t.txt
)

add_dependencies(06_day_harib03c_ipl10_od 06_day_harib03c_ipl10)
add_dependencies(06_day_harib03c_ipl10_od 06_day_harib03c_ipl10_wine)
add_dependencies(od 06_day_harib03c_ipl10_od)

#----------------------------------------------------------
########### next target ###############
set(06_day_harib03c_naskfunc_SRCS ${root_SOURCE_DIR}/projects/06_day/harib03c/naskfunc.nas)
set(06_day_harib03c_naskfunc_OUTS ${root_BINARY_DIR}/projects/06_day/harib03c/naskfunc.img)

add_custom_target(06_day_harib03c_naskfunc
  COMMAND ${RAKUSK_EXECUTABLE} ${06_day_harib03c_naskfunc_SRCS} ${06_day_harib03c_naskfunc_OUTS}
  DEPENDS ${06_day_harib03c_naskfunc_SRCS}
)

add_dependencies(images 06_day_harib03c_naskfunc)
set(06_day_harib03c_naskfunc_WINE_OUTS ${root_BINARY_DIR}/projects/06_day/harib03c/naskfunc_wine.img)
add_custom_target(06_day_harib03c_naskfunc_wine
  COMMAND ${WINE} ${WINE_NASK} ${06_day_harib03c_naskfunc_SRCS} ${06_day_harib03c_naskfunc_WINE_OUTS}
)
add_dependencies(wine 06_day_harib03c_naskfunc_wine)

add_custom_target(06_day_harib03c_naskfunc_od
  COMMAND ${OD} -t x1 ${06_day_harib03c_naskfunc_OUTS}      > ${06_day_harib03c_naskfunc_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${06_day_harib03c_naskfunc_WINE_OUTS} > ${06_day_harib03c_naskfunc_WINE_OUTS}_t.txt
  COMMAND diff -s ${06_day_harib03c_naskfunc_OUTS}_f.txt ${06_day_harib03c_naskfunc_WINE_OUTS}_t.txt
)

add_dependencies(06_day_harib03c_naskfunc_od 06_day_harib03c_naskfunc)
add_dependencies(06_day_harib03c_naskfunc_od 06_day_harib03c_naskfunc_wine)
add_dependencies(od 06_day_harib03c_naskfunc_od)

#----------------------------------------------------------
