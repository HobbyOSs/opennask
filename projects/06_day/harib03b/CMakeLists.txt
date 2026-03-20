#----------------------------------------------------------
message(STATUS "Entering directory projects/06_day/harib03b/")

set(FONT ${RAKUSK_EXECUTABLE} --makefont)
set(B2O  ${RAKUSK_EXECUTABLE} --bin2obj)
set(CONV ${root_BINARY_DIR}/objconv/objconv)
set(06_day_harib03b_WILDOBJ ${root_BINARY_DIR}/projects/06_day/harib03b/*.o)

# Full OS build rules
set(06_day_harib03b_OS    ${root_BINARY_DIR}/projects/06_day/harib03b/os.img)
set(06_day_harib03b_SYS   ${root_BINARY_DIR}/projects/06_day/harib03b/os.sys)
set(06_day_harib03b_IPLB  ${root_BINARY_DIR}/projects/06_day/harib03b/ipl.bin)
set(06_day_harib03b_IPLS  ${root_SOURCE_DIR}/projects/06_day/harib03b/ipl10.nas)
set(06_day_harib03b_HEADB ${root_BINARY_DIR}/projects/06_day/harib03b/asmhead.bin)
set(06_day_harib03b_HEADS ${root_SOURCE_DIR}/projects/06_day/harib03b/asmhead.nas)
set(06_day_harib03b_BOOTB ${root_BINARY_DIR}/projects/06_day/harib03b/boot.bin)
set(06_day_harib03b_CCO   ${root_BINARY_DIR}/projects/06_day/harib03b/boot.o)
set(06_day_harib03b_CCS   ${root_SOURCE_DIR}/projects/06_day/harib03b/*.c)
set(06_day_harib03b_LDS   ${root_SOURCE_DIR}/projects/os.lds)
set(06_day_harib03b_FUNCS ${root_SOURCE_DIR}/projects/06_day/harib03b/naskfunc.nas)
set(06_day_harib03b_FUNCO ${root_BINARY_DIR}/projects/06_day/harib03b/naskfunc.o)
set(06_day_harib03b_FONTS ${root_SOURCE_DIR}/projects/06_day/harib03b/hankaku.txt)
set(06_day_harib03b_FONTB ${root_BINARY_DIR}/projects/06_day/harib03b/hankaku.bin)
set(06_day_harib03b_FONTO ${root_BINARY_DIR}/projects/06_day/harib03b/hankaku.o)
set(06_day_harib03b_LIBGE ${root_BINARY_DIR}/golibc/libgo.a)
set(06_day_harib03b_LIBGC ${root_BINARY_DIR}/projects/06_day/harib03b/libgo.o)

add_custom_target(06_day_harib03b_run
  COMMAND ${QEMU} ${QEMUOPT} ${06_day_harib03b_OS}
  DEPENDS 06_day_harib03b_img
)
add_custom_target(06_day_harib03b_debug
  COMMAND ${QEMU} -s -S ${QEMUOPT} ${06_day_harib03b_OS} ${QEMU_DEBUG_OPT}
  DEPENDS 06_day_harib03b_img
)
add_custom_target(06_day_harib03b_clean
  COMMAND rm -f ${06_day_harib03b_OS}
  COMMAND rm -f ${06_day_harib03b_SYS}
  COMMAND rm -f ${06_day_harib03b_IPLB}
  COMMAND rm -f ${06_day_harib03b_HEADB}
  COMMAND rm -f ${06_day_harib03b_BOOTB}
  COMMAND rm -f ${06_day_harib03b_WILDOBJ}
  COMMAND rm -f ${root_BINARY_DIR}/projects/06_day/harib03b/ipl10.bin
  COMMAND rm -f ${root_BINARY_DIR}/projects/06_day/harib03b/asmhead.bin
  COMMAND rm -f ${root_BINARY_DIR}/projects/06_day/harib03b/*_wine.bin
  COMMAND rm -f ${root_BINARY_DIR}/projects/06_day/harib03b/*.txt
)
add_custom_target(06_day_harib03b_ipl_build
  COMMAND ${RAKUSK_EXECUTABLE} ${06_day_harib03b_IPLS} ${06_day_harib03b_IPLB}
  DEPENDS ${06_day_harib03b_IPLS}
)
add_custom_command(
  OUTPUT ${06_day_harib03b_HEADB}
  COMMAND ${RAKUSK_EXECUTABLE} ${06_day_harib03b_HEADS} ${06_day_harib03b_HEADB}
  DEPENDS ${06_day_harib03b_HEADS}
  COMMENT "Generating asmhead.bin for 06_day_harib03b"
)

add_custom_target(06_day_harib03b_sys
  COMMAND ${FONT} ${06_day_harib03b_FONTS} ${06_day_harib03b_FONTB}
  COMMAND ${B2O}  ${06_day_harib03b_FONTB} ${06_day_harib03b_FONTO} _hankaku
  COMMAND ${CONV} -fcoff32 -nu ${06_day_harib03b_LIBGE} ${06_day_harib03b_LIBGC}
  COMMAND ${RAKUSK_EXECUTABLE} ${06_day_harib03b_FUNCS} ${06_day_harib03b_FUNCO}
  COMMAND gcc ${BINOPT} -T ${06_day_harib03b_LDS} ${06_day_harib03b_CCS} ${06_day_harib03b_WILDOBJ} -o ${06_day_harib03b_BOOTB}
  COMMAND cat ${06_day_harib03b_HEADB} ${06_day_harib03b_BOOTB} > ${06_day_harib03b_SYS}
  DEPENDS ${06_day_harib03b_HEADB} 06_day_harib03b_ipl_build
  DEPENDS ${06_day_harib03b_FUNCO}
  DEPENDS ${06_day_harib03b_FONTO}
)
add_custom_target(06_day_harib03b_img
  COMMAND mformat -f 1440 -l HARIBOTEOS -N 0xffffffff -C -B ${06_day_harib03b_IPLB} -i ${06_day_harib03b_OS}
  COMMAND mcopy -i ${06_day_harib03b_OS} ${06_day_harib03b_SYS} ::
  DEPENDS 06_day_harib03b_sys
)

########### next target ###############
set(06_day_harib03b_asmhead_SRCS ${root_SOURCE_DIR}/projects/06_day/harib03b/asmhead.nas)
set(06_day_harib03b_asmhead_OUTS ${root_BINARY_DIR}/projects/06_day/harib03b/asmhead.bin)

add_custom_target(06_day_harib03b_asmhead
  COMMAND ${RAKUSK_EXECUTABLE} ${06_day_harib03b_asmhead_SRCS} ${06_day_harib03b_asmhead_OUTS}
  DEPENDS ${06_day_harib03b_asmhead_SRCS}
)

set(06_day_harib03b_asmhead_WINE_OUTS ${root_BINARY_DIR}/projects/06_day/harib03b/asmhead_wine.bin)
add_custom_target(06_day_harib03b_asmhead_wine
  COMMAND ${WINE} ${WINE_NASK} ${06_day_harib03b_asmhead_SRCS} ${06_day_harib03b_asmhead_WINE_OUTS}
)
add_dependencies(wine 06_day_harib03b_asmhead_wine)

add_custom_target(06_day_harib03b_asmhead_od
  COMMAND ${OD} -t x1 ${06_day_harib03b_asmhead_OUTS}      > ${06_day_harib03b_asmhead_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${06_day_harib03b_asmhead_WINE_OUTS} > ${06_day_harib03b_asmhead_WINE_OUTS}_t.txt
  COMMAND diff -s ${06_day_harib03b_asmhead_OUTS}_f.txt ${06_day_harib03b_asmhead_WINE_OUTS}_t.txt
)

add_dependencies(06_day_harib03b_asmhead_od 06_day_harib03b_asmhead)
add_dependencies(06_day_harib03b_asmhead_od 06_day_harib03b_asmhead_wine)
add_dependencies(od 06_day_harib03b_asmhead_od)

#----------------------------------------------------------
########### next target ###############
set(06_day_harib03b_ipl10_SRCS ${root_SOURCE_DIR}/projects/06_day/harib03b/ipl10.nas)
set(06_day_harib03b_ipl10_OUTS ${root_BINARY_DIR}/projects/06_day/harib03b/ipl10.bin)

add_custom_target(06_day_harib03b_ipl10
  COMMAND ${RAKUSK_EXECUTABLE} ${06_day_harib03b_ipl10_SRCS} ${06_day_harib03b_ipl10_OUTS}
  DEPENDS ${06_day_harib03b_ipl10_SRCS}
)

set(06_day_harib03b_ipl10_WINE_OUTS ${root_BINARY_DIR}/projects/06_day/harib03b/ipl10_wine.bin)
add_custom_target(06_day_harib03b_ipl10_wine
  COMMAND ${WINE} ${WINE_NASK} ${06_day_harib03b_ipl10_SRCS} ${06_day_harib03b_ipl10_WINE_OUTS}
)
add_dependencies(wine 06_day_harib03b_ipl10_wine)

add_custom_target(06_day_harib03b_ipl10_od
  COMMAND ${OD} -t x1 ${06_day_harib03b_ipl10_OUTS}      > ${06_day_harib03b_ipl10_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${06_day_harib03b_ipl10_WINE_OUTS} > ${06_day_harib03b_ipl10_WINE_OUTS}_t.txt
  COMMAND diff -s ${06_day_harib03b_ipl10_OUTS}_f.txt ${06_day_harib03b_ipl10_WINE_OUTS}_t.txt
)

add_dependencies(06_day_harib03b_ipl10_od 06_day_harib03b_ipl10)
add_dependencies(06_day_harib03b_ipl10_od 06_day_harib03b_ipl10_wine)
add_dependencies(od 06_day_harib03b_ipl10_od)

#----------------------------------------------------------
########### next target ###############
set(06_day_harib03b_naskfunc_SRCS ${root_SOURCE_DIR}/projects/06_day/harib03b/naskfunc.nas)
set(06_day_harib03b_naskfunc_OUTS ${root_BINARY_DIR}/projects/06_day/harib03b/naskfunc.img)

add_custom_target(06_day_harib03b_naskfunc
  COMMAND ${RAKUSK_EXECUTABLE} ${06_day_harib03b_naskfunc_SRCS} ${06_day_harib03b_naskfunc_OUTS}
  DEPENDS ${06_day_harib03b_naskfunc_SRCS}
)

add_dependencies(images 06_day_harib03b_naskfunc)
set(06_day_harib03b_naskfunc_WINE_OUTS ${root_BINARY_DIR}/projects/06_day/harib03b/naskfunc_wine.img)
add_custom_target(06_day_harib03b_naskfunc_wine
  COMMAND ${WINE} ${WINE_NASK} ${06_day_harib03b_naskfunc_SRCS} ${06_day_harib03b_naskfunc_WINE_OUTS}
)
add_dependencies(wine 06_day_harib03b_naskfunc_wine)

add_custom_target(06_day_harib03b_naskfunc_od
  COMMAND ${OD} -t x1 ${06_day_harib03b_naskfunc_OUTS}      > ${06_day_harib03b_naskfunc_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${06_day_harib03b_naskfunc_WINE_OUTS} > ${06_day_harib03b_naskfunc_WINE_OUTS}_t.txt
  COMMAND diff -s ${06_day_harib03b_naskfunc_OUTS}_f.txt ${06_day_harib03b_naskfunc_WINE_OUTS}_t.txt
)

add_dependencies(06_day_harib03b_naskfunc_od 06_day_harib03b_naskfunc)
add_dependencies(06_day_harib03b_naskfunc_od 06_day_harib03b_naskfunc_wine)
add_dependencies(od 06_day_harib03b_naskfunc_od)

#----------------------------------------------------------
