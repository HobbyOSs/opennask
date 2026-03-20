#----------------------------------------------------------
message(STATUS "Entering directory projects/09_day/harib06b/")

set(FONT ${RAKUSK_EXECUTABLE} --makefont)
set(B2O  ${RAKUSK_EXECUTABLE} --bin2obj)
set(CONV ${root_BINARY_DIR}/objconv/objconv)
set(09_day_harib06b_WILDOBJ ${root_BINARY_DIR}/projects/09_day/harib06b/*.o)

# Full OS build rules
set(09_day_harib06b_OS    ${root_BINARY_DIR}/projects/09_day/harib06b/os.img)
set(09_day_harib06b_SYS   ${root_BINARY_DIR}/projects/09_day/harib06b/os.sys)
set(09_day_harib06b_IPLB  ${root_BINARY_DIR}/projects/09_day/harib06b/ipl.bin)
set(09_day_harib06b_IPLS  ${root_SOURCE_DIR}/projects/09_day/harib06b/ipl10.nas)
set(09_day_harib06b_HEADB ${root_BINARY_DIR}/projects/09_day/harib06b/asmhead.bin)
set(09_day_harib06b_HEADS ${root_SOURCE_DIR}/projects/09_day/harib06b/asmhead.nas)
set(09_day_harib06b_BOOTB ${root_BINARY_DIR}/projects/09_day/harib06b/boot.bin)
set(09_day_harib06b_CCO   ${root_BINARY_DIR}/projects/09_day/harib06b/boot.o)
set(09_day_harib06b_CCS   ${root_SOURCE_DIR}/projects/09_day/harib06b/*.c)
set(09_day_harib06b_LDS   ${root_SOURCE_DIR}/projects/os.lds)
set(09_day_harib06b_FUNCS ${root_SOURCE_DIR}/projects/09_day/harib06b/naskfunc.nas)
set(09_day_harib06b_FUNCO ${root_BINARY_DIR}/projects/09_day/harib06b/naskfunc.o)
set(09_day_harib06b_FONTS ${root_SOURCE_DIR}/projects/09_day/harib06b/hankaku.txt)
set(09_day_harib06b_FONTB ${root_BINARY_DIR}/projects/09_day/harib06b/hankaku.bin)
set(09_day_harib06b_FONTO ${root_BINARY_DIR}/projects/09_day/harib06b/hankaku.o)
set(09_day_harib06b_LIBGE ${root_BINARY_DIR}/golibc/libgo.a)
set(09_day_harib06b_LIBGC ${root_BINARY_DIR}/projects/09_day/harib06b/libgo.o)

add_custom_target(09_day_harib06b_run
  COMMAND ${QEMU} ${QEMUOPT} ${09_day_harib06b_OS}
  DEPENDS 09_day_harib06b_img
)
add_custom_target(09_day_harib06b_debug
  COMMAND ${QEMU} -s -S ${QEMUOPT} ${09_day_harib06b_OS} ${QEMU_DEBUG_OPT}
  DEPENDS 09_day_harib06b_img
)
add_custom_target(09_day_harib06b_clean
  COMMAND rm -f ${09_day_harib06b_OS}
  COMMAND rm -f ${09_day_harib06b_SYS}
  COMMAND rm -f ${09_day_harib06b_IPLB}
  COMMAND rm -f ${09_day_harib06b_HEADB}
  COMMAND rm -f ${09_day_harib06b_BOOTB}
  COMMAND rm -f ${09_day_harib06b_WILDOBJ}
  COMMAND rm -f ${root_BINARY_DIR}/projects/09_day/harib06b/ipl10.bin
  COMMAND rm -f ${root_BINARY_DIR}/projects/09_day/harib06b/asmhead.bin
  COMMAND rm -f ${root_BINARY_DIR}/projects/09_day/harib06b/*_wine.bin
  COMMAND rm -f ${root_BINARY_DIR}/projects/09_day/harib06b/*.txt
)
add_custom_target(09_day_harib06b_ipl_build
  COMMAND ${RAKUSK_EXECUTABLE} ${09_day_harib06b_IPLS} ${09_day_harib06b_IPLB}
  DEPENDS ${09_day_harib06b_IPLS}
)
add_custom_command(
  OUTPUT ${09_day_harib06b_HEADB}
  COMMAND ${RAKUSK_EXECUTABLE} ${09_day_harib06b_HEADS} ${09_day_harib06b_HEADB}
  DEPENDS ${09_day_harib06b_HEADS}
  COMMENT "Generating asmhead.bin for 09_day_harib06b"
)

add_custom_target(09_day_harib06b_sys
  COMMAND ${FONT} ${09_day_harib06b_FONTS} ${09_day_harib06b_FONTB}
  COMMAND ${B2O}  ${09_day_harib06b_FONTB} ${09_day_harib06b_FONTO} _hankaku
  COMMAND ${CONV} -fcoff32 -nu ${09_day_harib06b_LIBGE} ${09_day_harib06b_LIBGC}
  COMMAND ${RAKUSK_EXECUTABLE} ${09_day_harib06b_FUNCS} ${09_day_harib06b_FUNCO}
  COMMAND gcc ${BINOPT} -T ${09_day_harib06b_LDS} ${09_day_harib06b_CCS} ${09_day_harib06b_WILDOBJ} -o ${09_day_harib06b_BOOTB}
  COMMAND cat ${09_day_harib06b_HEADB} ${09_day_harib06b_BOOTB} > ${09_day_harib06b_SYS}
  DEPENDS ${09_day_harib06b_HEADB} 09_day_harib06b_ipl_build
  DEPENDS ${09_day_harib06b_FUNCO}
  DEPENDS ${09_day_harib06b_FONTO}
)
add_custom_target(09_day_harib06b_img
  COMMAND mformat -f 1440 -l HARIBOTEOS -N 0xffffffff -C -B ${09_day_harib06b_IPLB} -i ${09_day_harib06b_OS}
  COMMAND mcopy -i ${09_day_harib06b_OS} ${09_day_harib06b_SYS} ::
  DEPENDS 09_day_harib06b_sys
)

########### next target ###############
set(09_day_harib06b_asmhead_SRCS ${root_SOURCE_DIR}/projects/09_day/harib06b/asmhead.nas)
set(09_day_harib06b_asmhead_OUTS ${root_BINARY_DIR}/projects/09_day/harib06b/asmhead.bin)

add_custom_target(09_day_harib06b_asmhead
  COMMAND ${RAKUSK_EXECUTABLE} ${09_day_harib06b_asmhead_SRCS} ${09_day_harib06b_asmhead_OUTS}
  DEPENDS ${09_day_harib06b_asmhead_SRCS}
)

set(09_day_harib06b_asmhead_WINE_OUTS ${root_BINARY_DIR}/projects/09_day/harib06b/asmhead_wine.bin)
add_custom_target(09_day_harib06b_asmhead_wine
  COMMAND ${WINE} ${WINE_NASK} ${09_day_harib06b_asmhead_SRCS} ${09_day_harib06b_asmhead_WINE_OUTS}
)
add_dependencies(wine 09_day_harib06b_asmhead_wine)

add_custom_target(09_day_harib06b_asmhead_od
  COMMAND ${OD} -t x1 ${09_day_harib06b_asmhead_OUTS}      > ${09_day_harib06b_asmhead_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${09_day_harib06b_asmhead_WINE_OUTS} > ${09_day_harib06b_asmhead_WINE_OUTS}_t.txt
  COMMAND diff -s ${09_day_harib06b_asmhead_OUTS}_f.txt ${09_day_harib06b_asmhead_WINE_OUTS}_t.txt
)

add_dependencies(09_day_harib06b_asmhead_od 09_day_harib06b_asmhead)
add_dependencies(09_day_harib06b_asmhead_od 09_day_harib06b_asmhead_wine)
add_dependencies(od 09_day_harib06b_asmhead_od)

#----------------------------------------------------------
########### next target ###############
set(09_day_harib06b_ipl10_SRCS ${root_SOURCE_DIR}/projects/09_day/harib06b/ipl10.nas)
set(09_day_harib06b_ipl10_OUTS ${root_BINARY_DIR}/projects/09_day/harib06b/ipl10.bin)

add_custom_target(09_day_harib06b_ipl10
  COMMAND ${RAKUSK_EXECUTABLE} ${09_day_harib06b_ipl10_SRCS} ${09_day_harib06b_ipl10_OUTS}
  DEPENDS ${09_day_harib06b_ipl10_SRCS}
)

set(09_day_harib06b_ipl10_WINE_OUTS ${root_BINARY_DIR}/projects/09_day/harib06b/ipl10_wine.bin)
add_custom_target(09_day_harib06b_ipl10_wine
  COMMAND ${WINE} ${WINE_NASK} ${09_day_harib06b_ipl10_SRCS} ${09_day_harib06b_ipl10_WINE_OUTS}
)
add_dependencies(wine 09_day_harib06b_ipl10_wine)

add_custom_target(09_day_harib06b_ipl10_od
  COMMAND ${OD} -t x1 ${09_day_harib06b_ipl10_OUTS}      > ${09_day_harib06b_ipl10_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${09_day_harib06b_ipl10_WINE_OUTS} > ${09_day_harib06b_ipl10_WINE_OUTS}_t.txt
  COMMAND diff -s ${09_day_harib06b_ipl10_OUTS}_f.txt ${09_day_harib06b_ipl10_WINE_OUTS}_t.txt
)

add_dependencies(09_day_harib06b_ipl10_od 09_day_harib06b_ipl10)
add_dependencies(09_day_harib06b_ipl10_od 09_day_harib06b_ipl10_wine)
add_dependencies(od 09_day_harib06b_ipl10_od)

#----------------------------------------------------------
########### next target ###############
set(09_day_harib06b_naskfunc_SRCS ${root_SOURCE_DIR}/projects/09_day/harib06b/naskfunc.nas)
set(09_day_harib06b_naskfunc_OUTS ${root_BINARY_DIR}/projects/09_day/harib06b/naskfunc.img)

add_custom_target(09_day_harib06b_naskfunc
  COMMAND ${RAKUSK_EXECUTABLE} ${09_day_harib06b_naskfunc_SRCS} ${09_day_harib06b_naskfunc_OUTS}
  DEPENDS ${09_day_harib06b_naskfunc_SRCS}
)

add_dependencies(images 09_day_harib06b_naskfunc)
set(09_day_harib06b_naskfunc_WINE_OUTS ${root_BINARY_DIR}/projects/09_day/harib06b/naskfunc_wine.img)
add_custom_target(09_day_harib06b_naskfunc_wine
  COMMAND ${WINE} ${WINE_NASK} ${09_day_harib06b_naskfunc_SRCS} ${09_day_harib06b_naskfunc_WINE_OUTS}
)
add_dependencies(wine 09_day_harib06b_naskfunc_wine)

add_custom_target(09_day_harib06b_naskfunc_od
  COMMAND ${OD} -t x1 ${09_day_harib06b_naskfunc_OUTS}      > ${09_day_harib06b_naskfunc_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${09_day_harib06b_naskfunc_WINE_OUTS} > ${09_day_harib06b_naskfunc_WINE_OUTS}_t.txt
  COMMAND diff -s ${09_day_harib06b_naskfunc_OUTS}_f.txt ${09_day_harib06b_naskfunc_WINE_OUTS}_t.txt
)

add_dependencies(09_day_harib06b_naskfunc_od 09_day_harib06b_naskfunc)
add_dependencies(09_day_harib06b_naskfunc_od 09_day_harib06b_naskfunc_wine)
add_dependencies(od 09_day_harib06b_naskfunc_od)

#----------------------------------------------------------
