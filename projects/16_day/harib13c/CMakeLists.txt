#----------------------------------------------------------
message(STATUS "Entering directory projects/16_day/harib13c/")

set(FONT ${root_BINARY_DIR}/src/makefont)
set(B2O  ${root_BINARY_DIR}/src/bin2obj)
set(CONV ${root_BINARY_DIR}/objconv/objconv)
set(16_day_harib13c_WILDOBJ ${root_BINARY_DIR}/projects/16_day/harib13c/*.o)

# Full OS build rules
set(16_day_harib13c_OS    ${root_BINARY_DIR}/projects/16_day/harib13c/os.img)
set(16_day_harib13c_SYS   ${root_BINARY_DIR}/projects/16_day/harib13c/os.sys)
set(16_day_harib13c_IPLB  ${root_BINARY_DIR}/projects/16_day/harib13c/ipl.bin)
set(16_day_harib13c_IPLS  ${root_SOURCE_DIR}/projects/16_day/harib13c/ipl10.nas)
set(16_day_harib13c_HEADB ${root_BINARY_DIR}/projects/16_day/harib13c/asmhead.bin)
set(16_day_harib13c_HEADS ${root_SOURCE_DIR}/projects/16_day/harib13c/asmhead.nas)
set(16_day_harib13c_BOOTB ${root_BINARY_DIR}/projects/16_day/harib13c/boot.bin)
set(16_day_harib13c_CCO   ${root_BINARY_DIR}/projects/16_day/harib13c/boot.o)
set(16_day_harib13c_CCS   ${root_SOURCE_DIR}/projects/16_day/harib13c/*.c)
set(16_day_harib13c_LDS   ${root_SOURCE_DIR}/projects/os.lds)
set(16_day_harib13c_FUNCS ${root_SOURCE_DIR}/projects/16_day/harib13c/naskfunc.nas)
set(16_day_harib13c_FUNCO ${root_BINARY_DIR}/projects/16_day/harib13c/naskfunc.o)
set(16_day_harib13c_FONTS ${root_SOURCE_DIR}/projects/16_day/harib13c/hankaku.txt)
set(16_day_harib13c_FONTB ${root_BINARY_DIR}/projects/16_day/harib13c/hankaku.bin)
set(16_day_harib13c_FONTO ${root_BINARY_DIR}/projects/16_day/harib13c/hankaku.o)
set(16_day_harib13c_LIBGE ${root_BINARY_DIR}/golibc/libgo.a)
set(16_day_harib13c_LIBGC ${root_BINARY_DIR}/projects/16_day/harib13c/libgo.o)

add_custom_target(16_day_harib13c_run
  COMMAND ${QEMU} ${QEMUOPT} ${16_day_harib13c_OS}
  DEPENDS 16_day_harib13c_img
)
add_custom_target(16_day_harib13c_debug
  COMMAND ${QEMU} -s -S ${QEMUOPT} ${16_day_harib13c_OS} ${QEMU_DEBUG_OPT}
  DEPENDS 16_day_harib13c_img
)
add_custom_target(16_day_harib13c_clean
  COMMAND rm -f ${16_day_harib13c_OS}
  COMMAND rm -f ${16_day_harib13c_SYS}
  COMMAND rm -f ${16_day_harib13c_IPLB}
  COMMAND rm -f ${16_day_harib13c_HEADB}
  COMMAND rm -f ${16_day_harib13c_BOOTB}
  COMMAND rm -f ${16_day_harib13c_WILDOBJ}
  COMMAND rm -f ${root_BINARY_DIR}/projects/16_day/harib13c/ipl10.bin
  COMMAND rm -f ${root_BINARY_DIR}/projects/16_day/harib13c/asmhead.bin
  COMMAND rm -f ${root_BINARY_DIR}/projects/16_day/harib13c/*_wine.bin
  COMMAND rm -f ${root_BINARY_DIR}/projects/16_day/harib13c/*.txt
)
add_custom_target(16_day_harib13c_ipl_build
  COMMAND ${RAKUSK_EXECUTABLE} ${16_day_harib13c_IPLS} ${16_day_harib13c_IPLB}
  DEPENDS ${16_day_harib13c_IPLS}
)
add_custom_command(
  OUTPUT ${16_day_harib13c_HEADB}
  COMMAND ${RAKUSK_EXECUTABLE} ${16_day_harib13c_HEADS} ${16_day_harib13c_HEADB}
  DEPENDS ${16_day_harib13c_HEADS}
  COMMENT "Generating asmhead.bin for 16_day_harib13c"
)

add_custom_command(
  OUTPUT ${16_day_harib13c_FUNCO}
  COMMAND ${RAKUSK_EXECUTABLE} ${16_day_harib13c_FUNCS} ${16_day_harib13c_FUNCO}
  DEPENDS ${16_day_harib13c_FUNCS}
  COMMENT "Generating naskfunc.o for 16_day_harib13c"
)
add_custom_command(
  OUTPUT ${16_day_harib13c_FONTO}
  COMMAND ${FONT} ${16_day_harib13c_FONTS} ${16_day_harib13c_FONTB}
  COMMAND ${B2O}  ${16_day_harib13c_FONTB} ${16_day_harib13c_FONTO} _hankaku
  DEPENDS ${16_day_harib13c_FONTS}
  COMMENT "Generating hankaku.o for 16_day_harib13c"
)
add_custom_command(
  OUTPUT ${16_day_harib13c_LIBGC}
  COMMAND ${CONV} -fcoff32 -nu ${16_day_harib13c_LIBGE} ${16_day_harib13c_LIBGC}
  DEPENDS ${16_day_harib13c_LIBGE}
  COMMENT "Converting golibc for 16_day_harib13c"
)
add_custom_target(16_day_harib13c_sys
  COMMAND gcc ${BINOPT} -T ${16_day_harib13c_LDS} ${16_day_harib13c_CCS} ${16_day_harib13c_WILDOBJ} -o ${16_day_harib13c_BOOTB}
  COMMAND cat ${16_day_harib13c_HEADB} ${16_day_harib13c_BOOTB} > ${16_day_harib13c_SYS}
  DEPENDS ${16_day_harib13c_HEADB} 16_day_harib13c_ipl_build
  DEPENDS ${16_day_harib13c_FUNCO}
  DEPENDS ${16_day_harib13c_FONTO} ${16_day_harib13c_LIBGC}
)
add_custom_target(16_day_harib13c_img
  COMMAND mformat -f 1440 -l HARIBOTEOS -N 0xffffffff -C -B ${16_day_harib13c_IPLB} -i ${16_day_harib13c_OS}
  COMMAND mcopy -i ${16_day_harib13c_OS} ${16_day_harib13c_SYS} ::
  DEPENDS 16_day_harib13c_sys
)

########### next target ###############
set(16_day_harib13c_asmhead_SRCS ${root_SOURCE_DIR}/projects/16_day/harib13c/asmhead.nas)
set(16_day_harib13c_asmhead_OUTS ${root_BINARY_DIR}/projects/16_day/harib13c/asmhead.bin)

add_custom_target(16_day_harib13c_asmhead
  COMMAND ${RAKUSK_EXECUTABLE} ${16_day_harib13c_asmhead_SRCS} ${16_day_harib13c_asmhead_OUTS}
  DEPENDS ${16_day_harib13c_asmhead_SRCS}
)

set(16_day_harib13c_asmhead_WINE_OUTS ${root_BINARY_DIR}/projects/16_day/harib13c/asmhead_wine.bin)
add_custom_target(16_day_harib13c_asmhead_wine
  COMMAND ${WINE} ${WINE_NASK} ${16_day_harib13c_asmhead_SRCS} ${16_day_harib13c_asmhead_WINE_OUTS}
)
add_dependencies(wine 16_day_harib13c_asmhead_wine)

add_custom_target(16_day_harib13c_asmhead_od
  COMMAND ${OD} -t x1 ${16_day_harib13c_asmhead_OUTS}      > ${16_day_harib13c_asmhead_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${16_day_harib13c_asmhead_WINE_OUTS} > ${16_day_harib13c_asmhead_WINE_OUTS}_t.txt
  COMMAND diff -s ${16_day_harib13c_asmhead_OUTS}_f.txt ${16_day_harib13c_asmhead_WINE_OUTS}_t.txt
)

add_dependencies(16_day_harib13c_asmhead_od 16_day_harib13c_asmhead)
add_dependencies(16_day_harib13c_asmhead_od 16_day_harib13c_asmhead_wine)
add_dependencies(od 16_day_harib13c_asmhead_od)

#----------------------------------------------------------
########### next target ###############
set(16_day_harib13c_ipl10_SRCS ${root_SOURCE_DIR}/projects/16_day/harib13c/ipl10.nas)
set(16_day_harib13c_ipl10_OUTS ${root_BINARY_DIR}/projects/16_day/harib13c/ipl10.bin)

add_custom_target(16_day_harib13c_ipl10
  COMMAND ${RAKUSK_EXECUTABLE} ${16_day_harib13c_ipl10_SRCS} ${16_day_harib13c_ipl10_OUTS}
  DEPENDS ${16_day_harib13c_ipl10_SRCS}
)

set(16_day_harib13c_ipl10_WINE_OUTS ${root_BINARY_DIR}/projects/16_day/harib13c/ipl10_wine.bin)
add_custom_target(16_day_harib13c_ipl10_wine
  COMMAND ${WINE} ${WINE_NASK} ${16_day_harib13c_ipl10_SRCS} ${16_day_harib13c_ipl10_WINE_OUTS}
)
add_dependencies(wine 16_day_harib13c_ipl10_wine)

add_custom_target(16_day_harib13c_ipl10_od
  COMMAND ${OD} -t x1 ${16_day_harib13c_ipl10_OUTS}      > ${16_day_harib13c_ipl10_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${16_day_harib13c_ipl10_WINE_OUTS} > ${16_day_harib13c_ipl10_WINE_OUTS}_t.txt
  COMMAND diff -s ${16_day_harib13c_ipl10_OUTS}_f.txt ${16_day_harib13c_ipl10_WINE_OUTS}_t.txt
)

add_dependencies(16_day_harib13c_ipl10_od 16_day_harib13c_ipl10)
add_dependencies(16_day_harib13c_ipl10_od 16_day_harib13c_ipl10_wine)
add_dependencies(od 16_day_harib13c_ipl10_od)

#----------------------------------------------------------
########### next target ###############
set(16_day_harib13c_naskfunc_SRCS ${root_SOURCE_DIR}/projects/16_day/harib13c/naskfunc.nas)
set(16_day_harib13c_naskfunc_OUTS ${root_BINARY_DIR}/projects/16_day/harib13c/naskfunc.img)

add_custom_target(16_day_harib13c_naskfunc
  COMMAND ${RAKUSK_EXECUTABLE} ${16_day_harib13c_naskfunc_SRCS} ${16_day_harib13c_naskfunc_OUTS}
  DEPENDS ${16_day_harib13c_naskfunc_SRCS}
)

add_dependencies(images 16_day_harib13c_naskfunc)
set(16_day_harib13c_naskfunc_WINE_OUTS ${root_BINARY_DIR}/projects/16_day/harib13c/naskfunc_wine.img)
add_custom_target(16_day_harib13c_naskfunc_wine
  COMMAND ${WINE} ${WINE_NASK} ${16_day_harib13c_naskfunc_SRCS} ${16_day_harib13c_naskfunc_WINE_OUTS}
)
add_dependencies(wine 16_day_harib13c_naskfunc_wine)

add_custom_target(16_day_harib13c_naskfunc_od
  COMMAND ${OD} -t x1 ${16_day_harib13c_naskfunc_OUTS}      > ${16_day_harib13c_naskfunc_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${16_day_harib13c_naskfunc_WINE_OUTS} > ${16_day_harib13c_naskfunc_WINE_OUTS}_t.txt
  COMMAND diff -s ${16_day_harib13c_naskfunc_OUTS}_f.txt ${16_day_harib13c_naskfunc_WINE_OUTS}_t.txt
)

add_dependencies(16_day_harib13c_naskfunc_od 16_day_harib13c_naskfunc)
add_dependencies(16_day_harib13c_naskfunc_od 16_day_harib13c_naskfunc_wine)
add_dependencies(od 16_day_harib13c_naskfunc_od)

#----------------------------------------------------------
