#----------------------------------------------------------
message(STATUS "Entering directory projects/06_day/harib03a/")

set(FONT ${RAKUSK_EXECUTABLE} --makefont)
set(B2O  ${RAKUSK_EXECUTABLE} --bin2obj)
set(CONV ${root_BINARY_DIR}/objconv/objconv)
set(06_day_harib03a_WILDOBJ ${root_BINARY_DIR}/projects/06_day/harib03a/*.o)

# Full OS build rules
set(06_day_harib03a_OS    ${root_BINARY_DIR}/projects/06_day/harib03a/os.img)
set(06_day_harib03a_SYS   ${root_BINARY_DIR}/projects/06_day/harib03a/os.sys)
set(06_day_harib03a_IPLB  ${root_BINARY_DIR}/projects/06_day/harib03a/ipl.bin)
set(06_day_harib03a_IPLS  ${root_SOURCE_DIR}/projects/06_day/harib03a/ipl10.nas)
set(06_day_harib03a_HEADB ${root_BINARY_DIR}/projects/06_day/harib03a/asmhead.bin)
set(06_day_harib03a_HEADS ${root_SOURCE_DIR}/projects/06_day/harib03a/asmhead.nas)
set(06_day_harib03a_BOOTB ${root_BINARY_DIR}/projects/06_day/harib03a/boot.bin)
set(06_day_harib03a_CCO   ${root_BINARY_DIR}/projects/06_day/harib03a/boot.o)
set(06_day_harib03a_CCS   ${root_SOURCE_DIR}/projects/06_day/harib03a/*.c)
set(06_day_harib03a_LDS   ${root_SOURCE_DIR}/projects/os.lds)
set(06_day_harib03a_FUNCS ${root_SOURCE_DIR}/projects/06_day/harib03a/naskfunc.nas)
set(06_day_harib03a_FUNCO ${root_BINARY_DIR}/projects/06_day/harib03a/naskfunc.o)
set(06_day_harib03a_FONTS ${root_SOURCE_DIR}/projects/06_day/harib03a/hankaku.txt)
set(06_day_harib03a_FONTB ${root_BINARY_DIR}/projects/06_day/harib03a/hankaku.bin)
set(06_day_harib03a_FONTO ${root_BINARY_DIR}/projects/06_day/harib03a/hankaku.o)
set(06_day_harib03a_LIBGE ${root_BINARY_DIR}/golibc/libgo.a)
set(06_day_harib03a_LIBGC ${root_BINARY_DIR}/projects/06_day/harib03a/libgo.o)

add_custom_target(06_day_harib03a_run
  COMMAND ${QEMU} ${QEMUOPT} ${06_day_harib03a_OS}
  DEPENDS 06_day_harib03a_img
)
add_custom_target(06_day_harib03a_debug
  COMMAND ${QEMU} -s -S ${QEMUOPT} ${06_day_harib03a_OS} ${QEMU_DEBUG_OPT}
  DEPENDS 06_day_harib03a_img
)
add_custom_target(06_day_harib03a_clean
  COMMAND rm -f ${06_day_harib03a_OS}
  COMMAND rm -f ${06_day_harib03a_SYS}
  COMMAND rm -f ${06_day_harib03a_IPLB}
  COMMAND rm -f ${06_day_harib03a_HEADB}
  COMMAND rm -f ${06_day_harib03a_BOOTB}
  COMMAND rm -f ${06_day_harib03a_WILDOBJ}
  COMMAND rm -f ${root_BINARY_DIR}/projects/06_day/harib03a/ipl10.bin
  COMMAND rm -f ${root_BINARY_DIR}/projects/06_day/harib03a/asmhead.bin
  COMMAND rm -f ${root_BINARY_DIR}/projects/06_day/harib03a/*_wine.bin
  COMMAND rm -f ${root_BINARY_DIR}/projects/06_day/harib03a/*.txt
)
add_custom_target(06_day_harib03a_ipl_build
  COMMAND ${RAKUSK_EXECUTABLE} ${06_day_harib03a_IPLS} ${06_day_harib03a_IPLB}
  DEPENDS ${06_day_harib03a_IPLS}
)
add_custom_command(
  OUTPUT ${06_day_harib03a_HEADB}
  COMMAND ${RAKUSK_EXECUTABLE} ${06_day_harib03a_HEADS} ${06_day_harib03a_HEADB}
  DEPENDS ${06_day_harib03a_HEADS}
  COMMENT "Generating asmhead.bin for 06_day_harib03a"
)

add_custom_target(06_day_harib03a_sys
  COMMAND ${FONT} ${06_day_harib03a_FONTS} ${06_day_harib03a_FONTB}
  COMMAND ${B2O}  ${06_day_harib03a_FONTB} ${06_day_harib03a_FONTO} _hankaku
  COMMAND ${CONV} -fcoff32 -nu ${06_day_harib03a_LIBGE} ${06_day_harib03a_LIBGC}
  COMMAND ${RAKUSK_EXECUTABLE} ${06_day_harib03a_FUNCS} ${06_day_harib03a_FUNCO}
  COMMAND gcc ${BINOPT} -T ${06_day_harib03a_LDS} ${06_day_harib03a_CCS} ${06_day_harib03a_WILDOBJ} -o ${06_day_harib03a_BOOTB}
  COMMAND cat ${06_day_harib03a_HEADB} ${06_day_harib03a_BOOTB} > ${06_day_harib03a_SYS}
  DEPENDS ${06_day_harib03a_HEADB} 06_day_harib03a_ipl_build
  DEPENDS ${06_day_harib03a_FUNCO}
  DEPENDS ${06_day_harib03a_FONTO}
)
add_custom_target(06_day_harib03a_img
  COMMAND mformat -f 1440 -l HARIBOTEOS -N 0xffffffff -C -B ${06_day_harib03a_IPLB} -i ${06_day_harib03a_OS}
  COMMAND mcopy -i ${06_day_harib03a_OS} ${06_day_harib03a_SYS} ::
  DEPENDS 06_day_harib03a_sys
)

########### next target ###############
set(06_day_harib03a_asmhead_SRCS ${root_SOURCE_DIR}/projects/06_day/harib03a/asmhead.nas)
set(06_day_harib03a_asmhead_OUTS ${root_BINARY_DIR}/projects/06_day/harib03a/asmhead.bin)

add_custom_target(06_day_harib03a_asmhead
  COMMAND ${RAKUSK_EXECUTABLE} ${06_day_harib03a_asmhead_SRCS} ${06_day_harib03a_asmhead_OUTS}
  DEPENDS ${06_day_harib03a_asmhead_SRCS}
)

set(06_day_harib03a_asmhead_WINE_OUTS ${root_BINARY_DIR}/projects/06_day/harib03a/asmhead_wine.bin)
add_custom_target(06_day_harib03a_asmhead_wine
  COMMAND ${WINE} ${WINE_NASK} ${06_day_harib03a_asmhead_SRCS} ${06_day_harib03a_asmhead_WINE_OUTS}
)
add_dependencies(wine 06_day_harib03a_asmhead_wine)

add_custom_target(06_day_harib03a_asmhead_od
  COMMAND ${OD} -t x1 ${06_day_harib03a_asmhead_OUTS}      > ${06_day_harib03a_asmhead_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${06_day_harib03a_asmhead_WINE_OUTS} > ${06_day_harib03a_asmhead_WINE_OUTS}_t.txt
  COMMAND diff -s ${06_day_harib03a_asmhead_OUTS}_f.txt ${06_day_harib03a_asmhead_WINE_OUTS}_t.txt
)

add_dependencies(06_day_harib03a_asmhead_od 06_day_harib03a_asmhead)
add_dependencies(06_day_harib03a_asmhead_od 06_day_harib03a_asmhead_wine)
add_dependencies(od 06_day_harib03a_asmhead_od)

#----------------------------------------------------------
########### next target ###############
set(06_day_harib03a_ipl10_SRCS ${root_SOURCE_DIR}/projects/06_day/harib03a/ipl10.nas)
set(06_day_harib03a_ipl10_OUTS ${root_BINARY_DIR}/projects/06_day/harib03a/ipl10.bin)

add_custom_target(06_day_harib03a_ipl10
  COMMAND ${RAKUSK_EXECUTABLE} ${06_day_harib03a_ipl10_SRCS} ${06_day_harib03a_ipl10_OUTS}
  DEPENDS ${06_day_harib03a_ipl10_SRCS}
)

set(06_day_harib03a_ipl10_WINE_OUTS ${root_BINARY_DIR}/projects/06_day/harib03a/ipl10_wine.bin)
add_custom_target(06_day_harib03a_ipl10_wine
  COMMAND ${WINE} ${WINE_NASK} ${06_day_harib03a_ipl10_SRCS} ${06_day_harib03a_ipl10_WINE_OUTS}
)
add_dependencies(wine 06_day_harib03a_ipl10_wine)

add_custom_target(06_day_harib03a_ipl10_od
  COMMAND ${OD} -t x1 ${06_day_harib03a_ipl10_OUTS}      > ${06_day_harib03a_ipl10_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${06_day_harib03a_ipl10_WINE_OUTS} > ${06_day_harib03a_ipl10_WINE_OUTS}_t.txt
  COMMAND diff -s ${06_day_harib03a_ipl10_OUTS}_f.txt ${06_day_harib03a_ipl10_WINE_OUTS}_t.txt
)

add_dependencies(06_day_harib03a_ipl10_od 06_day_harib03a_ipl10)
add_dependencies(06_day_harib03a_ipl10_od 06_day_harib03a_ipl10_wine)
add_dependencies(od 06_day_harib03a_ipl10_od)

#----------------------------------------------------------
########### next target ###############
set(06_day_harib03a_naskfunc_SRCS ${root_SOURCE_DIR}/projects/06_day/harib03a/naskfunc.nas)
set(06_day_harib03a_naskfunc_OUTS ${root_BINARY_DIR}/projects/06_day/harib03a/naskfunc.img)

add_custom_target(06_day_harib03a_naskfunc
  COMMAND ${RAKUSK_EXECUTABLE} ${06_day_harib03a_naskfunc_SRCS} ${06_day_harib03a_naskfunc_OUTS}
  DEPENDS ${06_day_harib03a_naskfunc_SRCS}
)

add_dependencies(images 06_day_harib03a_naskfunc)
set(06_day_harib03a_naskfunc_WINE_OUTS ${root_BINARY_DIR}/projects/06_day/harib03a/naskfunc_wine.img)
add_custom_target(06_day_harib03a_naskfunc_wine
  COMMAND ${WINE} ${WINE_NASK} ${06_day_harib03a_naskfunc_SRCS} ${06_day_harib03a_naskfunc_WINE_OUTS}
)
add_dependencies(wine 06_day_harib03a_naskfunc_wine)

add_custom_target(06_day_harib03a_naskfunc_od
  COMMAND ${OD} -t x1 ${06_day_harib03a_naskfunc_OUTS}      > ${06_day_harib03a_naskfunc_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${06_day_harib03a_naskfunc_WINE_OUTS} > ${06_day_harib03a_naskfunc_WINE_OUTS}_t.txt
  COMMAND diff -s ${06_day_harib03a_naskfunc_OUTS}_f.txt ${06_day_harib03a_naskfunc_WINE_OUTS}_t.txt
)

add_dependencies(06_day_harib03a_naskfunc_od 06_day_harib03a_naskfunc)
add_dependencies(06_day_harib03a_naskfunc_od 06_day_harib03a_naskfunc_wine)
add_dependencies(od 06_day_harib03a_naskfunc_od)

#----------------------------------------------------------
