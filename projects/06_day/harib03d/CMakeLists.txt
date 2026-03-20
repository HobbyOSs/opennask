#----------------------------------------------------------
message(STATUS "Entering directory projects/06_day/harib03d/")

set(FONT ${root_BINARY_DIR}/src/makefont)
set(B2O  ${root_BINARY_DIR}/src/bin2obj)
set(CONV ${root_BINARY_DIR}/objconv/objconv)
set(06_day_harib03d_WILDOBJ ${root_BINARY_DIR}/projects/06_day/harib03d/*.o)

# Full OS build rules
set(06_day_harib03d_OS    ${root_BINARY_DIR}/projects/06_day/harib03d/os.img)
set(06_day_harib03d_SYS   ${root_BINARY_DIR}/projects/06_day/harib03d/os.sys)
set(06_day_harib03d_IPLB  ${root_BINARY_DIR}/projects/06_day/harib03d/ipl.bin)
set(06_day_harib03d_IPLS  ${root_SOURCE_DIR}/projects/06_day/harib03d/ipl10.nas)
set(06_day_harib03d_HEADB ${root_BINARY_DIR}/projects/06_day/harib03d/asmhead.bin)
set(06_day_harib03d_HEADS ${root_SOURCE_DIR}/projects/06_day/harib03d/asmhead.nas)
set(06_day_harib03d_BOOTB ${root_BINARY_DIR}/projects/06_day/harib03d/boot.bin)
set(06_day_harib03d_CCO   ${root_BINARY_DIR}/projects/06_day/harib03d/boot.o)
set(06_day_harib03d_CCS   ${root_SOURCE_DIR}/projects/06_day/harib03d/*.c)
set(06_day_harib03d_LDS   ${root_SOURCE_DIR}/projects/os.lds)
set(06_day_harib03d_FUNCS ${root_SOURCE_DIR}/projects/06_day/harib03d/naskfunc.nas)
set(06_day_harib03d_FUNCO ${root_BINARY_DIR}/projects/06_day/harib03d/naskfunc.o)
set(06_day_harib03d_FONTS ${root_SOURCE_DIR}/projects/06_day/harib03d/hankaku.txt)
set(06_day_harib03d_FONTB ${root_BINARY_DIR}/projects/06_day/harib03d/hankaku.bin)
set(06_day_harib03d_FONTO ${root_BINARY_DIR}/projects/06_day/harib03d/hankaku.o)
set(06_day_harib03d_LIBGE ${root_BINARY_DIR}/golibc/libgo.a)
set(06_day_harib03d_LIBGC ${root_BINARY_DIR}/projects/06_day/harib03d/libgo.o)

add_custom_target(06_day_harib03d_run
  COMMAND ${QEMU} ${QEMUOPT} ${06_day_harib03d_OS}
  DEPENDS 06_day_harib03d_img
)
add_custom_target(06_day_harib03d_debug
  COMMAND ${QEMU} -s -S ${QEMUOPT} ${06_day_harib03d_OS} ${QEMU_DEBUG_OPT}
  DEPENDS 06_day_harib03d_img
)
add_custom_target(06_day_harib03d_clean
  COMMAND rm -f ${06_day_harib03d_OS}
  COMMAND rm -f ${06_day_harib03d_SYS}
  COMMAND rm -f ${06_day_harib03d_IPLB}
  COMMAND rm -f ${06_day_harib03d_HEADB}
  COMMAND rm -f ${06_day_harib03d_BOOTB}
  COMMAND rm -f ${06_day_harib03d_WILDOBJ}
  COMMAND rm -f ${root_BINARY_DIR}/projects/06_day/harib03d/ipl10.bin
  COMMAND rm -f ${root_BINARY_DIR}/projects/06_day/harib03d/asmhead.bin
  COMMAND rm -f ${root_BINARY_DIR}/projects/06_day/harib03d/*_wine.bin
  COMMAND rm -f ${root_BINARY_DIR}/projects/06_day/harib03d/*.txt
)
add_custom_target(06_day_harib03d_ipl_build
  COMMAND ${RAKUSK_EXECUTABLE} ${06_day_harib03d_IPLS} ${06_day_harib03d_IPLB}
  DEPENDS ${06_day_harib03d_IPLS}
)
add_custom_command(
  OUTPUT ${06_day_harib03d_HEADB}
  COMMAND ${RAKUSK_EXECUTABLE} ${06_day_harib03d_HEADS} ${06_day_harib03d_HEADB}
  DEPENDS ${06_day_harib03d_HEADS}
  COMMENT "Generating asmhead.bin for 06_day_harib03d"
)

add_custom_command(
  OUTPUT ${06_day_harib03d_FUNCO}
  COMMAND ${RAKUSK_EXECUTABLE} ${06_day_harib03d_FUNCS} ${06_day_harib03d_FUNCO}
  DEPENDS ${06_day_harib03d_FUNCS}
  COMMENT "Generating naskfunc.o for 06_day_harib03d"
)
add_custom_command(
  OUTPUT ${06_day_harib03d_FONTO}
  COMMAND ${FONT} ${06_day_harib03d_FONTS} ${06_day_harib03d_FONTB}
  COMMAND ${B2O}  ${06_day_harib03d_FONTB} ${06_day_harib03d_FONTO} _hankaku
  DEPENDS ${06_day_harib03d_FONTS}
  COMMENT "Generating hankaku.o for 06_day_harib03d"
)
add_custom_command(
  OUTPUT ${06_day_harib03d_LIBGC}
  COMMAND ${CONV} -fcoff32 -nu ${06_day_harib03d_LIBGE} ${06_day_harib03d_LIBGC}
  DEPENDS ${06_day_harib03d_LIBGE}
  COMMENT "Converting golibc for 06_day_harib03d"
)
add_custom_target(06_day_harib03d_sys
  COMMAND gcc ${BINOPT} -T ${06_day_harib03d_LDS} ${06_day_harib03d_CCS} ${06_day_harib03d_WILDOBJ} -o ${06_day_harib03d_BOOTB}
  COMMAND cat ${06_day_harib03d_HEADB} ${06_day_harib03d_BOOTB} > ${06_day_harib03d_SYS}
  DEPENDS ${06_day_harib03d_HEADB} 06_day_harib03d_ipl_build
  DEPENDS ${06_day_harib03d_FUNCO}
  DEPENDS ${06_day_harib03d_FONTO} ${06_day_harib03d_LIBGC}
)
add_custom_target(06_day_harib03d_img
  COMMAND mformat -f 1440 -l HARIBOTEOS -N 0xffffffff -C -B ${06_day_harib03d_IPLB} -i ${06_day_harib03d_OS}
  COMMAND mcopy -i ${06_day_harib03d_OS} ${06_day_harib03d_SYS} ::
  DEPENDS 06_day_harib03d_sys
)

########### next target ###############
set(06_day_harib03d_asmhead_SRCS ${root_SOURCE_DIR}/projects/06_day/harib03d/asmhead.nas)
set(06_day_harib03d_asmhead_OUTS ${root_BINARY_DIR}/projects/06_day/harib03d/asmhead.bin)

add_custom_target(06_day_harib03d_asmhead
  COMMAND ${RAKUSK_EXECUTABLE} ${06_day_harib03d_asmhead_SRCS} ${06_day_harib03d_asmhead_OUTS}
  DEPENDS ${06_day_harib03d_asmhead_SRCS}
)

set(06_day_harib03d_asmhead_WINE_OUTS ${root_BINARY_DIR}/projects/06_day/harib03d/asmhead_wine.bin)
add_custom_target(06_day_harib03d_asmhead_wine
  COMMAND ${WINE} ${WINE_NASK} ${06_day_harib03d_asmhead_SRCS} ${06_day_harib03d_asmhead_WINE_OUTS}
)
add_dependencies(wine 06_day_harib03d_asmhead_wine)

add_custom_target(06_day_harib03d_asmhead_od
  COMMAND ${OD} -t x1 ${06_day_harib03d_asmhead_OUTS}      > ${06_day_harib03d_asmhead_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${06_day_harib03d_asmhead_WINE_OUTS} > ${06_day_harib03d_asmhead_WINE_OUTS}_t.txt
  COMMAND diff -s ${06_day_harib03d_asmhead_OUTS}_f.txt ${06_day_harib03d_asmhead_WINE_OUTS}_t.txt
)

add_dependencies(06_day_harib03d_asmhead_od 06_day_harib03d_asmhead)
add_dependencies(06_day_harib03d_asmhead_od 06_day_harib03d_asmhead_wine)
add_dependencies(od 06_day_harib03d_asmhead_od)

#----------------------------------------------------------
########### next target ###############
set(06_day_harib03d_ipl10_SRCS ${root_SOURCE_DIR}/projects/06_day/harib03d/ipl10.nas)
set(06_day_harib03d_ipl10_OUTS ${root_BINARY_DIR}/projects/06_day/harib03d/ipl10.bin)

add_custom_target(06_day_harib03d_ipl10
  COMMAND ${RAKUSK_EXECUTABLE} ${06_day_harib03d_ipl10_SRCS} ${06_day_harib03d_ipl10_OUTS}
  DEPENDS ${06_day_harib03d_ipl10_SRCS}
)

set(06_day_harib03d_ipl10_WINE_OUTS ${root_BINARY_DIR}/projects/06_day/harib03d/ipl10_wine.bin)
add_custom_target(06_day_harib03d_ipl10_wine
  COMMAND ${WINE} ${WINE_NASK} ${06_day_harib03d_ipl10_SRCS} ${06_day_harib03d_ipl10_WINE_OUTS}
)
add_dependencies(wine 06_day_harib03d_ipl10_wine)

add_custom_target(06_day_harib03d_ipl10_od
  COMMAND ${OD} -t x1 ${06_day_harib03d_ipl10_OUTS}      > ${06_day_harib03d_ipl10_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${06_day_harib03d_ipl10_WINE_OUTS} > ${06_day_harib03d_ipl10_WINE_OUTS}_t.txt
  COMMAND diff -s ${06_day_harib03d_ipl10_OUTS}_f.txt ${06_day_harib03d_ipl10_WINE_OUTS}_t.txt
)

add_dependencies(06_day_harib03d_ipl10_od 06_day_harib03d_ipl10)
add_dependencies(06_day_harib03d_ipl10_od 06_day_harib03d_ipl10_wine)
add_dependencies(od 06_day_harib03d_ipl10_od)

#----------------------------------------------------------
########### next target ###############
set(06_day_harib03d_naskfunc_SRCS ${root_SOURCE_DIR}/projects/06_day/harib03d/naskfunc.nas)
set(06_day_harib03d_naskfunc_OUTS ${root_BINARY_DIR}/projects/06_day/harib03d/naskfunc.img)

add_custom_target(06_day_harib03d_naskfunc
  COMMAND ${RAKUSK_EXECUTABLE} ${06_day_harib03d_naskfunc_SRCS} ${06_day_harib03d_naskfunc_OUTS}
  DEPENDS ${06_day_harib03d_naskfunc_SRCS}
)

add_dependencies(images 06_day_harib03d_naskfunc)
set(06_day_harib03d_naskfunc_WINE_OUTS ${root_BINARY_DIR}/projects/06_day/harib03d/naskfunc_wine.img)
add_custom_target(06_day_harib03d_naskfunc_wine
  COMMAND ${WINE} ${WINE_NASK} ${06_day_harib03d_naskfunc_SRCS} ${06_day_harib03d_naskfunc_WINE_OUTS}
)
add_dependencies(wine 06_day_harib03d_naskfunc_wine)

add_custom_target(06_day_harib03d_naskfunc_od
  COMMAND ${OD} -t x1 ${06_day_harib03d_naskfunc_OUTS}      > ${06_day_harib03d_naskfunc_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${06_day_harib03d_naskfunc_WINE_OUTS} > ${06_day_harib03d_naskfunc_WINE_OUTS}_t.txt
  COMMAND diff -s ${06_day_harib03d_naskfunc_OUTS}_f.txt ${06_day_harib03d_naskfunc_WINE_OUTS}_t.txt
)

add_dependencies(06_day_harib03d_naskfunc_od 06_day_harib03d_naskfunc)
add_dependencies(06_day_harib03d_naskfunc_od 06_day_harib03d_naskfunc_wine)
add_dependencies(od 06_day_harib03d_naskfunc_od)

#----------------------------------------------------------
