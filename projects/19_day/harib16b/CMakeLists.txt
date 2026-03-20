#----------------------------------------------------------
message(STATUS "Entering directory projects/19_day/harib16b/")

set(FONT ${RAKUSK_EXECUTABLE} --makefont)
set(B2O  ${RAKUSK_EXECUTABLE} --bin2obj)
set(CONV ${root_BINARY_DIR}/objconv/objconv)
set(19_day_harib16b_WILDOBJ ${root_BINARY_DIR}/projects/19_day/harib16b/*.o)

# Full OS build rules
set(19_day_harib16b_OS    ${root_BINARY_DIR}/projects/19_day/harib16b/os.img)
set(19_day_harib16b_SYS   ${root_BINARY_DIR}/projects/19_day/harib16b/os.sys)
set(19_day_harib16b_IPLB  ${root_BINARY_DIR}/projects/19_day/harib16b/ipl.bin)
set(19_day_harib16b_IPLS  ${root_SOURCE_DIR}/projects/19_day/harib16b/ipl10.nas)
set(19_day_harib16b_HEADB ${root_BINARY_DIR}/projects/19_day/harib16b/asmhead.bin)
set(19_day_harib16b_HEADS ${root_SOURCE_DIR}/projects/19_day/harib16b/asmhead.nas)
set(19_day_harib16b_BOOTB ${root_BINARY_DIR}/projects/19_day/harib16b/boot.bin)
set(19_day_harib16b_CCO   ${root_BINARY_DIR}/projects/19_day/harib16b/boot.o)
set(19_day_harib16b_CCS   ${root_SOURCE_DIR}/projects/19_day/harib16b/*.c)
set(19_day_harib16b_LDS   ${root_SOURCE_DIR}/projects/os.lds)
set(19_day_harib16b_FUNCS ${root_SOURCE_DIR}/projects/19_day/harib16b/naskfunc.nas)
set(19_day_harib16b_FUNCO ${root_BINARY_DIR}/projects/19_day/harib16b/naskfunc.o)
set(19_day_harib16b_FONTS ${root_SOURCE_DIR}/projects/19_day/harib16b/hankaku.txt)
set(19_day_harib16b_FONTB ${root_BINARY_DIR}/projects/19_day/harib16b/hankaku.bin)
set(19_day_harib16b_FONTO ${root_BINARY_DIR}/projects/19_day/harib16b/hankaku.o)
set(19_day_harib16b_LIBGE ${root_BINARY_DIR}/golibc/libgo.a)
set(19_day_harib16b_LIBGC ${root_BINARY_DIR}/projects/19_day/harib16b/libgo.o)

add_custom_target(19_day_harib16b_run
  COMMAND ${QEMU} ${QEMUOPT} ${19_day_harib16b_OS}
  DEPENDS 19_day_harib16b_img
)
add_custom_target(19_day_harib16b_debug
  COMMAND ${QEMU} -s -S ${QEMUOPT} ${19_day_harib16b_OS} ${QEMU_DEBUG_OPT}
  DEPENDS 19_day_harib16b_img
)
add_custom_target(19_day_harib16b_clean
  COMMAND rm -f ${19_day_harib16b_OS}
  COMMAND rm -f ${19_day_harib16b_SYS}
  COMMAND rm -f ${19_day_harib16b_IPLB}
  COMMAND rm -f ${19_day_harib16b_HEADB}
  COMMAND rm -f ${19_day_harib16b_BOOTB}
  COMMAND rm -f ${19_day_harib16b_WILDOBJ}
  COMMAND rm -f ${root_BINARY_DIR}/projects/19_day/harib16b/ipl10.bin
  COMMAND rm -f ${root_BINARY_DIR}/projects/19_day/harib16b/asmhead.bin
  COMMAND rm -f ${root_BINARY_DIR}/projects/19_day/harib16b/*_wine.bin
  COMMAND rm -f ${root_BINARY_DIR}/projects/19_day/harib16b/*.txt
)
add_custom_target(19_day_harib16b_ipl_build
  COMMAND ${RAKUSK_EXECUTABLE} ${19_day_harib16b_IPLS} ${19_day_harib16b_IPLB}
  DEPENDS ${19_day_harib16b_IPLS}
)
add_custom_command(
  OUTPUT ${19_day_harib16b_HEADB}
  COMMAND ${RAKUSK_EXECUTABLE} ${19_day_harib16b_HEADS} ${19_day_harib16b_HEADB}
  DEPENDS ${19_day_harib16b_HEADS}
  COMMENT "Generating asmhead.bin for 19_day_harib16b"
)

add_custom_target(19_day_harib16b_sys
  COMMAND ${FONT} ${19_day_harib16b_FONTS} ${19_day_harib16b_FONTB}
  COMMAND ${B2O}  ${19_day_harib16b_FONTB} ${19_day_harib16b_FONTO} _hankaku
  COMMAND ${CONV} -fcoff32 -nu ${19_day_harib16b_LIBGE} ${19_day_harib16b_LIBGC}
  COMMAND ${RAKUSK_EXECUTABLE} ${19_day_harib16b_FUNCS} ${19_day_harib16b_FUNCO}
  COMMAND gcc ${BINOPT} -T ${19_day_harib16b_LDS} ${19_day_harib16b_CCS} ${19_day_harib16b_WILDOBJ} -o ${19_day_harib16b_BOOTB}
  COMMAND cat ${19_day_harib16b_HEADB} ${19_day_harib16b_BOOTB} > ${19_day_harib16b_SYS}
  DEPENDS ${19_day_harib16b_HEADB} 19_day_harib16b_ipl_build
  DEPENDS ${19_day_harib16b_FUNCO}
  DEPENDS ${19_day_harib16b_FONTO}
)
add_custom_target(19_day_harib16b_img
  COMMAND mformat -f 1440 -l HARIBOTEOS -N 0xffffffff -C -B ${19_day_harib16b_IPLB} -i ${19_day_harib16b_OS}
  COMMAND mcopy -i ${19_day_harib16b_OS} ${19_day_harib16b_SYS} ::
  DEPENDS 19_day_harib16b_sys
)

########### next target ###############
set(19_day_harib16b_asmhead_SRCS ${root_SOURCE_DIR}/projects/19_day/harib16b/asmhead.nas)
set(19_day_harib16b_asmhead_OUTS ${root_BINARY_DIR}/projects/19_day/harib16b/asmhead.bin)

add_custom_target(19_day_harib16b_asmhead
  COMMAND ${RAKUSK_EXECUTABLE} ${19_day_harib16b_asmhead_SRCS} ${19_day_harib16b_asmhead_OUTS}
  DEPENDS ${19_day_harib16b_asmhead_SRCS}
)

set(19_day_harib16b_asmhead_WINE_OUTS ${root_BINARY_DIR}/projects/19_day/harib16b/asmhead_wine.bin)
add_custom_target(19_day_harib16b_asmhead_wine
  COMMAND ${WINE} ${WINE_NASK} ${19_day_harib16b_asmhead_SRCS} ${19_day_harib16b_asmhead_WINE_OUTS}
)
add_dependencies(wine 19_day_harib16b_asmhead_wine)

add_custom_target(19_day_harib16b_asmhead_od
  COMMAND ${OD} -t x1 ${19_day_harib16b_asmhead_OUTS}      > ${19_day_harib16b_asmhead_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${19_day_harib16b_asmhead_WINE_OUTS} > ${19_day_harib16b_asmhead_WINE_OUTS}_t.txt
  COMMAND diff -s ${19_day_harib16b_asmhead_OUTS}_f.txt ${19_day_harib16b_asmhead_WINE_OUTS}_t.txt
)

add_dependencies(19_day_harib16b_asmhead_od 19_day_harib16b_asmhead)
add_dependencies(19_day_harib16b_asmhead_od 19_day_harib16b_asmhead_wine)
add_dependencies(od 19_day_harib16b_asmhead_od)

#----------------------------------------------------------
########### next target ###############
set(19_day_harib16b_ipl10_SRCS ${root_SOURCE_DIR}/projects/19_day/harib16b/ipl10.nas)
set(19_day_harib16b_ipl10_OUTS ${root_BINARY_DIR}/projects/19_day/harib16b/ipl10.bin)

add_custom_target(19_day_harib16b_ipl10
  COMMAND ${RAKUSK_EXECUTABLE} ${19_day_harib16b_ipl10_SRCS} ${19_day_harib16b_ipl10_OUTS}
  DEPENDS ${19_day_harib16b_ipl10_SRCS}
)

set(19_day_harib16b_ipl10_WINE_OUTS ${root_BINARY_DIR}/projects/19_day/harib16b/ipl10_wine.bin)
add_custom_target(19_day_harib16b_ipl10_wine
  COMMAND ${WINE} ${WINE_NASK} ${19_day_harib16b_ipl10_SRCS} ${19_day_harib16b_ipl10_WINE_OUTS}
)
add_dependencies(wine 19_day_harib16b_ipl10_wine)

add_custom_target(19_day_harib16b_ipl10_od
  COMMAND ${OD} -t x1 ${19_day_harib16b_ipl10_OUTS}      > ${19_day_harib16b_ipl10_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${19_day_harib16b_ipl10_WINE_OUTS} > ${19_day_harib16b_ipl10_WINE_OUTS}_t.txt
  COMMAND diff -s ${19_day_harib16b_ipl10_OUTS}_f.txt ${19_day_harib16b_ipl10_WINE_OUTS}_t.txt
)

add_dependencies(19_day_harib16b_ipl10_od 19_day_harib16b_ipl10)
add_dependencies(19_day_harib16b_ipl10_od 19_day_harib16b_ipl10_wine)
add_dependencies(od 19_day_harib16b_ipl10_od)

#----------------------------------------------------------
########### next target ###############
set(19_day_harib16b_naskfunc_SRCS ${root_SOURCE_DIR}/projects/19_day/harib16b/naskfunc.nas)
set(19_day_harib16b_naskfunc_OUTS ${root_BINARY_DIR}/projects/19_day/harib16b/naskfunc.img)

add_custom_target(19_day_harib16b_naskfunc
  COMMAND ${RAKUSK_EXECUTABLE} ${19_day_harib16b_naskfunc_SRCS} ${19_day_harib16b_naskfunc_OUTS}
  DEPENDS ${19_day_harib16b_naskfunc_SRCS}
)

add_dependencies(images 19_day_harib16b_naskfunc)
set(19_day_harib16b_naskfunc_WINE_OUTS ${root_BINARY_DIR}/projects/19_day/harib16b/naskfunc_wine.img)
add_custom_target(19_day_harib16b_naskfunc_wine
  COMMAND ${WINE} ${WINE_NASK} ${19_day_harib16b_naskfunc_SRCS} ${19_day_harib16b_naskfunc_WINE_OUTS}
)
add_dependencies(wine 19_day_harib16b_naskfunc_wine)

add_custom_target(19_day_harib16b_naskfunc_od
  COMMAND ${OD} -t x1 ${19_day_harib16b_naskfunc_OUTS}      > ${19_day_harib16b_naskfunc_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${19_day_harib16b_naskfunc_WINE_OUTS} > ${19_day_harib16b_naskfunc_WINE_OUTS}_t.txt
  COMMAND diff -s ${19_day_harib16b_naskfunc_OUTS}_f.txt ${19_day_harib16b_naskfunc_WINE_OUTS}_t.txt
)

add_dependencies(19_day_harib16b_naskfunc_od 19_day_harib16b_naskfunc)
add_dependencies(19_day_harib16b_naskfunc_od 19_day_harib16b_naskfunc_wine)
add_dependencies(od 19_day_harib16b_naskfunc_od)

#----------------------------------------------------------
