#----------------------------------------------------------
message(STATUS "Entering directory projects/17_day/harib14b/")

set(FONT ${RAKUSK_EXECUTABLE} --makefont)
set(B2O  ${RAKUSK_EXECUTABLE} --bin2obj)
set(CONV ${root_BINARY_DIR}/objconv/objconv)
set(17_day_harib14b_WILDOBJ ${root_BINARY_DIR}/projects/17_day/harib14b/*.o)

# Full OS build rules
set(17_day_harib14b_OS    ${root_BINARY_DIR}/projects/17_day/harib14b/os.img)
set(17_day_harib14b_SYS   ${root_BINARY_DIR}/projects/17_day/harib14b/os.sys)
set(17_day_harib14b_IPLB  ${root_BINARY_DIR}/projects/17_day/harib14b/ipl.bin)
set(17_day_harib14b_IPLS  ${root_SOURCE_DIR}/projects/17_day/harib14b/ipl10.nas)
set(17_day_harib14b_HEADB ${root_BINARY_DIR}/projects/17_day/harib14b/asmhead.bin)
set(17_day_harib14b_HEADS ${root_SOURCE_DIR}/projects/17_day/harib14b/asmhead.nas)
set(17_day_harib14b_BOOTB ${root_BINARY_DIR}/projects/17_day/harib14b/boot.bin)
set(17_day_harib14b_CCO   ${root_BINARY_DIR}/projects/17_day/harib14b/boot.o)
set(17_day_harib14b_CCS   ${root_SOURCE_DIR}/projects/17_day/harib14b/*.c)
set(17_day_harib14b_LDS   ${root_SOURCE_DIR}/projects/os.lds)
set(17_day_harib14b_FUNCS ${root_SOURCE_DIR}/projects/17_day/harib14b/naskfunc.nas)
set(17_day_harib14b_FUNCO ${root_BINARY_DIR}/projects/17_day/harib14b/naskfunc.o)
set(17_day_harib14b_FONTS ${root_SOURCE_DIR}/projects/17_day/harib14b/hankaku.txt)
set(17_day_harib14b_FONTB ${root_BINARY_DIR}/projects/17_day/harib14b/hankaku.bin)
set(17_day_harib14b_FONTO ${root_BINARY_DIR}/projects/17_day/harib14b/hankaku.o)
set(17_day_harib14b_LIBGE ${root_BINARY_DIR}/golibc/libgo.a)
set(17_day_harib14b_LIBGC ${root_BINARY_DIR}/projects/17_day/harib14b/libgo.o)

add_custom_target(17_day_harib14b_run
  COMMAND ${QEMU} ${QEMUOPT} ${17_day_harib14b_OS}
  DEPENDS 17_day_harib14b_img
)
add_custom_target(17_day_harib14b_debug
  COMMAND ${QEMU} -s -S ${QEMUOPT} ${17_day_harib14b_OS} ${QEMU_DEBUG_OPT}
  DEPENDS 17_day_harib14b_img
)
add_custom_target(17_day_harib14b_clean
  COMMAND rm -f ${17_day_harib14b_OS}
  COMMAND rm -f ${17_day_harib14b_SYS}
  COMMAND rm -f ${17_day_harib14b_IPLB}
  COMMAND rm -f ${17_day_harib14b_HEADB}
  COMMAND rm -f ${17_day_harib14b_BOOTB}
  COMMAND rm -f ${17_day_harib14b_WILDOBJ}
  COMMAND rm -f ${root_BINARY_DIR}/projects/17_day/harib14b/ipl10.bin
  COMMAND rm -f ${root_BINARY_DIR}/projects/17_day/harib14b/asmhead.bin
  COMMAND rm -f ${root_BINARY_DIR}/projects/17_day/harib14b/*_wine.bin
  COMMAND rm -f ${root_BINARY_DIR}/projects/17_day/harib14b/*.txt
)
add_custom_target(17_day_harib14b_ipl_build
  COMMAND ${RAKUSK_EXECUTABLE} ${17_day_harib14b_IPLS} ${17_day_harib14b_IPLB}
  DEPENDS ${17_day_harib14b_IPLS}
)
add_custom_command(
  OUTPUT ${17_day_harib14b_HEADB}
  COMMAND ${RAKUSK_EXECUTABLE} ${17_day_harib14b_HEADS} ${17_day_harib14b_HEADB}
  DEPENDS ${17_day_harib14b_HEADS}
  COMMENT "Generating asmhead.bin for 17_day_harib14b"
)

add_custom_target(17_day_harib14b_sys
  COMMAND ${FONT} ${17_day_harib14b_FONTS} ${17_day_harib14b_FONTB}
  COMMAND ${B2O}  ${17_day_harib14b_FONTB} ${17_day_harib14b_FONTO} _hankaku
  COMMAND ${CONV} -fcoff32 -nu ${17_day_harib14b_LIBGE} ${17_day_harib14b_LIBGC}
  COMMAND ${RAKUSK_EXECUTABLE} ${17_day_harib14b_FUNCS} ${17_day_harib14b_FUNCO}
  COMMAND gcc ${BINOPT} -T ${17_day_harib14b_LDS} ${17_day_harib14b_CCS} ${17_day_harib14b_WILDOBJ} -o ${17_day_harib14b_BOOTB}
  COMMAND cat ${17_day_harib14b_HEADB} ${17_day_harib14b_BOOTB} > ${17_day_harib14b_SYS}
  DEPENDS ${17_day_harib14b_HEADB} 17_day_harib14b_ipl_build
  DEPENDS ${17_day_harib14b_FUNCO}
  DEPENDS ${17_day_harib14b_FONTO}
)
add_custom_target(17_day_harib14b_img
  COMMAND mformat -f 1440 -l HARIBOTEOS -N 0xffffffff -C -B ${17_day_harib14b_IPLB} -i ${17_day_harib14b_OS}
  COMMAND mcopy -i ${17_day_harib14b_OS} ${17_day_harib14b_SYS} ::
  DEPENDS 17_day_harib14b_sys
)

########### next target ###############
set(17_day_harib14b_asmhead_SRCS ${root_SOURCE_DIR}/projects/17_day/harib14b/asmhead.nas)
set(17_day_harib14b_asmhead_OUTS ${root_BINARY_DIR}/projects/17_day/harib14b/asmhead.bin)

add_custom_target(17_day_harib14b_asmhead
  COMMAND ${RAKUSK_EXECUTABLE} ${17_day_harib14b_asmhead_SRCS} ${17_day_harib14b_asmhead_OUTS}
  DEPENDS ${17_day_harib14b_asmhead_SRCS}
)

set(17_day_harib14b_asmhead_WINE_OUTS ${root_BINARY_DIR}/projects/17_day/harib14b/asmhead_wine.bin)
add_custom_target(17_day_harib14b_asmhead_wine
  COMMAND ${WINE} ${WINE_NASK} ${17_day_harib14b_asmhead_SRCS} ${17_day_harib14b_asmhead_WINE_OUTS}
)
add_dependencies(wine 17_day_harib14b_asmhead_wine)

add_custom_target(17_day_harib14b_asmhead_od
  COMMAND ${OD} -t x1 ${17_day_harib14b_asmhead_OUTS}      > ${17_day_harib14b_asmhead_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${17_day_harib14b_asmhead_WINE_OUTS} > ${17_day_harib14b_asmhead_WINE_OUTS}_t.txt
  COMMAND diff -s ${17_day_harib14b_asmhead_OUTS}_f.txt ${17_day_harib14b_asmhead_WINE_OUTS}_t.txt
)

add_dependencies(17_day_harib14b_asmhead_od 17_day_harib14b_asmhead)
add_dependencies(17_day_harib14b_asmhead_od 17_day_harib14b_asmhead_wine)
add_dependencies(od 17_day_harib14b_asmhead_od)

#----------------------------------------------------------
########### next target ###############
set(17_day_harib14b_ipl10_SRCS ${root_SOURCE_DIR}/projects/17_day/harib14b/ipl10.nas)
set(17_day_harib14b_ipl10_OUTS ${root_BINARY_DIR}/projects/17_day/harib14b/ipl10.bin)

add_custom_target(17_day_harib14b_ipl10
  COMMAND ${RAKUSK_EXECUTABLE} ${17_day_harib14b_ipl10_SRCS} ${17_day_harib14b_ipl10_OUTS}
  DEPENDS ${17_day_harib14b_ipl10_SRCS}
)

set(17_day_harib14b_ipl10_WINE_OUTS ${root_BINARY_DIR}/projects/17_day/harib14b/ipl10_wine.bin)
add_custom_target(17_day_harib14b_ipl10_wine
  COMMAND ${WINE} ${WINE_NASK} ${17_day_harib14b_ipl10_SRCS} ${17_day_harib14b_ipl10_WINE_OUTS}
)
add_dependencies(wine 17_day_harib14b_ipl10_wine)

add_custom_target(17_day_harib14b_ipl10_od
  COMMAND ${OD} -t x1 ${17_day_harib14b_ipl10_OUTS}      > ${17_day_harib14b_ipl10_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${17_day_harib14b_ipl10_WINE_OUTS} > ${17_day_harib14b_ipl10_WINE_OUTS}_t.txt
  COMMAND diff -s ${17_day_harib14b_ipl10_OUTS}_f.txt ${17_day_harib14b_ipl10_WINE_OUTS}_t.txt
)

add_dependencies(17_day_harib14b_ipl10_od 17_day_harib14b_ipl10)
add_dependencies(17_day_harib14b_ipl10_od 17_day_harib14b_ipl10_wine)
add_dependencies(od 17_day_harib14b_ipl10_od)

#----------------------------------------------------------
########### next target ###############
set(17_day_harib14b_naskfunc_SRCS ${root_SOURCE_DIR}/projects/17_day/harib14b/naskfunc.nas)
set(17_day_harib14b_naskfunc_OUTS ${root_BINARY_DIR}/projects/17_day/harib14b/naskfunc.img)

add_custom_target(17_day_harib14b_naskfunc
  COMMAND ${RAKUSK_EXECUTABLE} ${17_day_harib14b_naskfunc_SRCS} ${17_day_harib14b_naskfunc_OUTS}
  DEPENDS ${17_day_harib14b_naskfunc_SRCS}
)

add_dependencies(images 17_day_harib14b_naskfunc)
set(17_day_harib14b_naskfunc_WINE_OUTS ${root_BINARY_DIR}/projects/17_day/harib14b/naskfunc_wine.img)
add_custom_target(17_day_harib14b_naskfunc_wine
  COMMAND ${WINE} ${WINE_NASK} ${17_day_harib14b_naskfunc_SRCS} ${17_day_harib14b_naskfunc_WINE_OUTS}
)
add_dependencies(wine 17_day_harib14b_naskfunc_wine)

add_custom_target(17_day_harib14b_naskfunc_od
  COMMAND ${OD} -t x1 ${17_day_harib14b_naskfunc_OUTS}      > ${17_day_harib14b_naskfunc_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${17_day_harib14b_naskfunc_WINE_OUTS} > ${17_day_harib14b_naskfunc_WINE_OUTS}_t.txt
  COMMAND diff -s ${17_day_harib14b_naskfunc_OUTS}_f.txt ${17_day_harib14b_naskfunc_WINE_OUTS}_t.txt
)

add_dependencies(17_day_harib14b_naskfunc_od 17_day_harib14b_naskfunc)
add_dependencies(17_day_harib14b_naskfunc_od 17_day_harib14b_naskfunc_wine)
add_dependencies(od 17_day_harib14b_naskfunc_od)

#----------------------------------------------------------
