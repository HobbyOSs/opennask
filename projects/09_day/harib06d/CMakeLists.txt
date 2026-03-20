#----------------------------------------------------------
message(STATUS "Entering directory projects/09_day/harib06d/")

set(FONT ${RAKUSK_EXECUTABLE} --makefont)
set(B2O  ${RAKUSK_EXECUTABLE} --bin2obj)
set(CONV ${root_BINARY_DIR}/objconv/objconv)
set(09_day_harib06d_WILDOBJ ${root_BINARY_DIR}/projects/09_day/harib06d/*.o)

# Full OS build rules
set(09_day_harib06d_OS    ${root_BINARY_DIR}/projects/09_day/harib06d/os.img)
set(09_day_harib06d_SYS   ${root_BINARY_DIR}/projects/09_day/harib06d/os.sys)
set(09_day_harib06d_IPLB  ${root_BINARY_DIR}/projects/09_day/harib06d/ipl.bin)
set(09_day_harib06d_IPLS  ${root_SOURCE_DIR}/projects/09_day/harib06d/ipl10.nas)
set(09_day_harib06d_HEADB ${root_BINARY_DIR}/projects/09_day/harib06d/asmhead.bin)
set(09_day_harib06d_HEADS ${root_SOURCE_DIR}/projects/09_day/harib06d/asmhead.nas)
set(09_day_harib06d_BOOTB ${root_BINARY_DIR}/projects/09_day/harib06d/boot.bin)
set(09_day_harib06d_CCO   ${root_BINARY_DIR}/projects/09_day/harib06d/boot.o)
set(09_day_harib06d_CCS   ${root_SOURCE_DIR}/projects/09_day/harib06d/*.c)
set(09_day_harib06d_LDS   ${root_SOURCE_DIR}/projects/os.lds)
set(09_day_harib06d_FUNCS ${root_SOURCE_DIR}/projects/09_day/harib06d/naskfunc.nas)
set(09_day_harib06d_FUNCO ${root_BINARY_DIR}/projects/09_day/harib06d/naskfunc.o)
set(09_day_harib06d_FONTS ${root_SOURCE_DIR}/projects/09_day/harib06d/hankaku.txt)
set(09_day_harib06d_FONTB ${root_BINARY_DIR}/projects/09_day/harib06d/hankaku.bin)
set(09_day_harib06d_FONTO ${root_BINARY_DIR}/projects/09_day/harib06d/hankaku.o)
set(09_day_harib06d_LIBGE ${root_BINARY_DIR}/golibc/libgo.a)
set(09_day_harib06d_LIBGC ${root_BINARY_DIR}/projects/09_day/harib06d/libgo.o)

add_custom_target(09_day_harib06d_run
  COMMAND ${QEMU} ${QEMUOPT} ${09_day_harib06d_OS}
  DEPENDS 09_day_harib06d_img
)
add_custom_target(09_day_harib06d_debug
  COMMAND ${QEMU} -s -S ${QEMUOPT} ${09_day_harib06d_OS} ${QEMU_DEBUG_OPT}
  DEPENDS 09_day_harib06d_img
)
add_custom_target(09_day_harib06d_clean
  COMMAND rm -f ${09_day_harib06d_OS}
  COMMAND rm -f ${09_day_harib06d_SYS}
  COMMAND rm -f ${09_day_harib06d_IPLB}
  COMMAND rm -f ${09_day_harib06d_HEADB}
  COMMAND rm -f ${09_day_harib06d_BOOTB}
  COMMAND rm -f ${09_day_harib06d_WILDOBJ}
  COMMAND rm -f ${root_BINARY_DIR}/projects/09_day/harib06d/ipl10.bin
  COMMAND rm -f ${root_BINARY_DIR}/projects/09_day/harib06d/asmhead.bin
  COMMAND rm -f ${root_BINARY_DIR}/projects/09_day/harib06d/*_wine.bin
  COMMAND rm -f ${root_BINARY_DIR}/projects/09_day/harib06d/*.txt
)
add_custom_target(09_day_harib06d_ipl_build
  COMMAND ${RAKUSK_EXECUTABLE} ${09_day_harib06d_IPLS} ${09_day_harib06d_IPLB}
  DEPENDS ${09_day_harib06d_IPLS}
)
add_custom_command(
  OUTPUT ${09_day_harib06d_HEADB}
  COMMAND ${RAKUSK_EXECUTABLE} ${09_day_harib06d_HEADS} ${09_day_harib06d_HEADB}
  DEPENDS ${09_day_harib06d_HEADS}
  COMMENT "Generating asmhead.bin for 09_day_harib06d"
)

add_custom_target(09_day_harib06d_sys
  COMMAND ${FONT} ${09_day_harib06d_FONTS} ${09_day_harib06d_FONTB}
  COMMAND ${B2O}  ${09_day_harib06d_FONTB} ${09_day_harib06d_FONTO} _hankaku
  COMMAND ${CONV} -fcoff32 -nu ${09_day_harib06d_LIBGE} ${09_day_harib06d_LIBGC}
  COMMAND ${RAKUSK_EXECUTABLE} ${09_day_harib06d_FUNCS} ${09_day_harib06d_FUNCO}
  COMMAND gcc ${BINOPT} -T ${09_day_harib06d_LDS} ${09_day_harib06d_CCS} ${09_day_harib06d_WILDOBJ} -o ${09_day_harib06d_BOOTB}
  COMMAND cat ${09_day_harib06d_HEADB} ${09_day_harib06d_BOOTB} > ${09_day_harib06d_SYS}
  DEPENDS ${09_day_harib06d_HEADB} 09_day_harib06d_ipl_build
  DEPENDS ${09_day_harib06d_FUNCO}
  DEPENDS ${09_day_harib06d_FONTO}
)
add_custom_target(09_day_harib06d_img
  COMMAND mformat -f 1440 -l HARIBOTEOS -N 0xffffffff -C -B ${09_day_harib06d_IPLB} -i ${09_day_harib06d_OS}
  COMMAND mcopy -i ${09_day_harib06d_OS} ${09_day_harib06d_SYS} ::
  DEPENDS 09_day_harib06d_sys
)

########### next target ###############
set(09_day_harib06d_asmhead_SRCS ${root_SOURCE_DIR}/projects/09_day/harib06d/asmhead.nas)
set(09_day_harib06d_asmhead_OUTS ${root_BINARY_DIR}/projects/09_day/harib06d/asmhead.bin)

add_custom_target(09_day_harib06d_asmhead
  COMMAND ${RAKUSK_EXECUTABLE} ${09_day_harib06d_asmhead_SRCS} ${09_day_harib06d_asmhead_OUTS}
  DEPENDS ${09_day_harib06d_asmhead_SRCS}
)

set(09_day_harib06d_asmhead_WINE_OUTS ${root_BINARY_DIR}/projects/09_day/harib06d/asmhead_wine.bin)
add_custom_target(09_day_harib06d_asmhead_wine
  COMMAND ${WINE} ${WINE_NASK} ${09_day_harib06d_asmhead_SRCS} ${09_day_harib06d_asmhead_WINE_OUTS}
)
add_dependencies(wine 09_day_harib06d_asmhead_wine)

add_custom_target(09_day_harib06d_asmhead_od
  COMMAND ${OD} -t x1 ${09_day_harib06d_asmhead_OUTS}      > ${09_day_harib06d_asmhead_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${09_day_harib06d_asmhead_WINE_OUTS} > ${09_day_harib06d_asmhead_WINE_OUTS}_t.txt
  COMMAND diff -s ${09_day_harib06d_asmhead_OUTS}_f.txt ${09_day_harib06d_asmhead_WINE_OUTS}_t.txt
)

add_dependencies(09_day_harib06d_asmhead_od 09_day_harib06d_asmhead)
add_dependencies(09_day_harib06d_asmhead_od 09_day_harib06d_asmhead_wine)
add_dependencies(od 09_day_harib06d_asmhead_od)

#----------------------------------------------------------
########### next target ###############
set(09_day_harib06d_ipl10_SRCS ${root_SOURCE_DIR}/projects/09_day/harib06d/ipl10.nas)
set(09_day_harib06d_ipl10_OUTS ${root_BINARY_DIR}/projects/09_day/harib06d/ipl10.bin)

add_custom_target(09_day_harib06d_ipl10
  COMMAND ${RAKUSK_EXECUTABLE} ${09_day_harib06d_ipl10_SRCS} ${09_day_harib06d_ipl10_OUTS}
  DEPENDS ${09_day_harib06d_ipl10_SRCS}
)

set(09_day_harib06d_ipl10_WINE_OUTS ${root_BINARY_DIR}/projects/09_day/harib06d/ipl10_wine.bin)
add_custom_target(09_day_harib06d_ipl10_wine
  COMMAND ${WINE} ${WINE_NASK} ${09_day_harib06d_ipl10_SRCS} ${09_day_harib06d_ipl10_WINE_OUTS}
)
add_dependencies(wine 09_day_harib06d_ipl10_wine)

add_custom_target(09_day_harib06d_ipl10_od
  COMMAND ${OD} -t x1 ${09_day_harib06d_ipl10_OUTS}      > ${09_day_harib06d_ipl10_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${09_day_harib06d_ipl10_WINE_OUTS} > ${09_day_harib06d_ipl10_WINE_OUTS}_t.txt
  COMMAND diff -s ${09_day_harib06d_ipl10_OUTS}_f.txt ${09_day_harib06d_ipl10_WINE_OUTS}_t.txt
)

add_dependencies(09_day_harib06d_ipl10_od 09_day_harib06d_ipl10)
add_dependencies(09_day_harib06d_ipl10_od 09_day_harib06d_ipl10_wine)
add_dependencies(od 09_day_harib06d_ipl10_od)

#----------------------------------------------------------
########### next target ###############
set(09_day_harib06d_naskfunc_SRCS ${root_SOURCE_DIR}/projects/09_day/harib06d/naskfunc.nas)
set(09_day_harib06d_naskfunc_OUTS ${root_BINARY_DIR}/projects/09_day/harib06d/naskfunc.img)

add_custom_target(09_day_harib06d_naskfunc
  COMMAND ${RAKUSK_EXECUTABLE} ${09_day_harib06d_naskfunc_SRCS} ${09_day_harib06d_naskfunc_OUTS}
  DEPENDS ${09_day_harib06d_naskfunc_SRCS}
)

add_dependencies(images 09_day_harib06d_naskfunc)
set(09_day_harib06d_naskfunc_WINE_OUTS ${root_BINARY_DIR}/projects/09_day/harib06d/naskfunc_wine.img)
add_custom_target(09_day_harib06d_naskfunc_wine
  COMMAND ${WINE} ${WINE_NASK} ${09_day_harib06d_naskfunc_SRCS} ${09_day_harib06d_naskfunc_WINE_OUTS}
)
add_dependencies(wine 09_day_harib06d_naskfunc_wine)

add_custom_target(09_day_harib06d_naskfunc_od
  COMMAND ${OD} -t x1 ${09_day_harib06d_naskfunc_OUTS}      > ${09_day_harib06d_naskfunc_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${09_day_harib06d_naskfunc_WINE_OUTS} > ${09_day_harib06d_naskfunc_WINE_OUTS}_t.txt
  COMMAND diff -s ${09_day_harib06d_naskfunc_OUTS}_f.txt ${09_day_harib06d_naskfunc_WINE_OUTS}_t.txt
)

add_dependencies(09_day_harib06d_naskfunc_od 09_day_harib06d_naskfunc)
add_dependencies(09_day_harib06d_naskfunc_od 09_day_harib06d_naskfunc_wine)
add_dependencies(od 09_day_harib06d_naskfunc_od)

#----------------------------------------------------------
