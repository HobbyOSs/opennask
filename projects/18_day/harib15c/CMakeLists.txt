#----------------------------------------------------------
message(STATUS "Entering directory projects/18_day/harib15c/")

set(FONT ${root_BINARY_DIR}/src/makefont)
set(B2O  ${root_BINARY_DIR}/src/bin2obj)
set(CONV ${root_BINARY_DIR}/objconv/objconv)
set(18_day_harib15c_WILDOBJ ${root_BINARY_DIR}/projects/18_day/harib15c/*.o)

# Full OS build rules
set(18_day_harib15c_OS    ${root_BINARY_DIR}/projects/18_day/harib15c/os.img)
set(18_day_harib15c_SYS   ${root_BINARY_DIR}/projects/18_day/harib15c/os.sys)
set(18_day_harib15c_IPLB  ${root_BINARY_DIR}/projects/18_day/harib15c/ipl.bin)
set(18_day_harib15c_IPLS  ${root_SOURCE_DIR}/projects/18_day/harib15c/ipl10.nas)
set(18_day_harib15c_HEADB ${root_BINARY_DIR}/projects/18_day/harib15c/asmhead.bin)
set(18_day_harib15c_HEADS ${root_SOURCE_DIR}/projects/18_day/harib15c/asmhead.nas)
set(18_day_harib15c_BOOTB ${root_BINARY_DIR}/projects/18_day/harib15c/boot.bin)
set(18_day_harib15c_CCO   ${root_BINARY_DIR}/projects/18_day/harib15c/boot.o)
set(18_day_harib15c_CCS   ${root_SOURCE_DIR}/projects/18_day/harib15c/*.c)
set(18_day_harib15c_LDS   ${root_SOURCE_DIR}/projects/os.lds)
set(18_day_harib15c_FUNCS ${root_SOURCE_DIR}/projects/18_day/harib15c/naskfunc.nas)
set(18_day_harib15c_FUNCO ${root_BINARY_DIR}/projects/18_day/harib15c/naskfunc.o)
set(18_day_harib15c_FONTS ${root_SOURCE_DIR}/projects/18_day/harib15c/hankaku.txt)
set(18_day_harib15c_FONTB ${root_BINARY_DIR}/projects/18_day/harib15c/hankaku.bin)
set(18_day_harib15c_FONTO ${root_BINARY_DIR}/projects/18_day/harib15c/hankaku.o)
set(18_day_harib15c_LIBGE ${root_BINARY_DIR}/golibc/libgo.a)
set(18_day_harib15c_LIBGC ${root_BINARY_DIR}/projects/18_day/harib15c/libgo.o)

add_custom_target(18_day_harib15c_run
  COMMAND ${QEMU} ${QEMUOPT} ${18_day_harib15c_OS}
  DEPENDS 18_day_harib15c_img
)
add_custom_target(18_day_harib15c_debug
  COMMAND ${QEMU} -s -S ${QEMUOPT} ${18_day_harib15c_OS} ${QEMU_DEBUG_OPT}
  DEPENDS 18_day_harib15c_img
)
add_custom_target(18_day_harib15c_clean
  COMMAND rm -f ${18_day_harib15c_OS}
  COMMAND rm -f ${18_day_harib15c_SYS}
  COMMAND rm -f ${18_day_harib15c_IPLB}
  COMMAND rm -f ${18_day_harib15c_HEADB}
  COMMAND rm -f ${18_day_harib15c_BOOTB}
  COMMAND rm -f ${18_day_harib15c_WILDOBJ}
  COMMAND rm -f ${root_BINARY_DIR}/projects/18_day/harib15c/ipl10.bin
  COMMAND rm -f ${root_BINARY_DIR}/projects/18_day/harib15c/asmhead.bin
  COMMAND rm -f ${root_BINARY_DIR}/projects/18_day/harib15c/*_wine.bin
  COMMAND rm -f ${root_BINARY_DIR}/projects/18_day/harib15c/*.txt
)
add_custom_target(18_day_harib15c_ipl_build
  COMMAND ${GOSK_EXECUTABLE} ${18_day_harib15c_IPLS} ${18_day_harib15c_IPLB}
  DEPENDS ${18_day_harib15c_IPLS}
)
add_custom_command(
  OUTPUT ${18_day_harib15c_HEADB}
  COMMAND ${GOSK_EXECUTABLE} ${18_day_harib15c_HEADS} ${18_day_harib15c_HEADB}
  DEPENDS ${18_day_harib15c_HEADS}
  COMMENT "Generating asmhead.bin for 18_day_harib15c"
)

add_custom_target(18_day_harib15c_sys
  COMMAND ${FONT} ${18_day_harib15c_FONTS} ${18_day_harib15c_FONTB}
  COMMAND ${B2O}  ${18_day_harib15c_FONTB} ${18_day_harib15c_FONTO} _hankaku
  COMMAND ${CONV} -fcoff32 -nu ${18_day_harib15c_LIBGE} ${18_day_harib15c_LIBGC}
  COMMAND ${GOSK_EXECUTABLE} ${18_day_harib15c_FUNCS} ${18_day_harib15c_FUNCO}
  COMMAND gcc ${BINOPT} -T ${18_day_harib15c_LDS} ${18_day_harib15c_CCS} ${18_day_harib15c_WILDOBJ} -o ${18_day_harib15c_BOOTB}
  COMMAND cat ${18_day_harib15c_HEADB} ${18_day_harib15c_BOOTB} > ${18_day_harib15c_SYS}
  DEPENDS ${18_day_harib15c_HEADB} 18_day_harib15c_ipl_build
  DEPENDS ${18_day_harib15c_FUNCO}
  DEPENDS ${18_day_harib15c_FONTO}
)
add_custom_target(18_day_harib15c_img
  COMMAND mformat -f 1440 -l HARIBOTEOS -N 0xffffffff -C -B ${18_day_harib15c_IPLB} -i ${18_day_harib15c_OS}
  COMMAND mcopy -i ${18_day_harib15c_OS} ${18_day_harib15c_SYS} ::
  DEPENDS 18_day_harib15c_sys
)

########### next target ###############
set(18_day_harib15c_asmhead_SRCS ${root_SOURCE_DIR}/projects/18_day/harib15c/asmhead.nas)
set(18_day_harib15c_asmhead_OUTS ${root_BINARY_DIR}/projects/18_day/harib15c/asmhead.bin)

add_custom_target(18_day_harib15c_asmhead
  COMMAND ${GOSK_EXECUTABLE} ${18_day_harib15c_asmhead_SRCS} ${18_day_harib15c_asmhead_OUTS}
  DEPENDS ${18_day_harib15c_asmhead_SRCS}
)

set(18_day_harib15c_asmhead_WINE_OUTS ${root_BINARY_DIR}/projects/18_day/harib15c/asmhead_wine.bin)
add_custom_target(18_day_harib15c_asmhead_wine
  COMMAND ${WINE} ${WINE_NASK} ${18_day_harib15c_asmhead_SRCS} ${18_day_harib15c_asmhead_WINE_OUTS}
)
add_dependencies(wine 18_day_harib15c_asmhead_wine)

add_custom_target(18_day_harib15c_asmhead_od
  COMMAND ${OD} -t x1 ${18_day_harib15c_asmhead_OUTS}      > ${18_day_harib15c_asmhead_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${18_day_harib15c_asmhead_WINE_OUTS} > ${18_day_harib15c_asmhead_WINE_OUTS}_t.txt
  COMMAND diff -s ${18_day_harib15c_asmhead_OUTS}_f.txt ${18_day_harib15c_asmhead_WINE_OUTS}_t.txt
)

add_dependencies(18_day_harib15c_asmhead_od 18_day_harib15c_asmhead)
add_dependencies(18_day_harib15c_asmhead_od 18_day_harib15c_asmhead_wine)
add_dependencies(od 18_day_harib15c_asmhead_od)

#----------------------------------------------------------
########### next target ###############
set(18_day_harib15c_ipl10_SRCS ${root_SOURCE_DIR}/projects/18_day/harib15c/ipl10.nas)
set(18_day_harib15c_ipl10_OUTS ${root_BINARY_DIR}/projects/18_day/harib15c/ipl10.bin)

add_custom_target(18_day_harib15c_ipl10
  COMMAND ${GOSK_EXECUTABLE} ${18_day_harib15c_ipl10_SRCS} ${18_day_harib15c_ipl10_OUTS}
  DEPENDS ${18_day_harib15c_ipl10_SRCS}
)

set(18_day_harib15c_ipl10_WINE_OUTS ${root_BINARY_DIR}/projects/18_day/harib15c/ipl10_wine.bin)
add_custom_target(18_day_harib15c_ipl10_wine
  COMMAND ${WINE} ${WINE_NASK} ${18_day_harib15c_ipl10_SRCS} ${18_day_harib15c_ipl10_WINE_OUTS}
)
add_dependencies(wine 18_day_harib15c_ipl10_wine)

add_custom_target(18_day_harib15c_ipl10_od
  COMMAND ${OD} -t x1 ${18_day_harib15c_ipl10_OUTS}      > ${18_day_harib15c_ipl10_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${18_day_harib15c_ipl10_WINE_OUTS} > ${18_day_harib15c_ipl10_WINE_OUTS}_t.txt
  COMMAND diff -s ${18_day_harib15c_ipl10_OUTS}_f.txt ${18_day_harib15c_ipl10_WINE_OUTS}_t.txt
)

add_dependencies(18_day_harib15c_ipl10_od 18_day_harib15c_ipl10)
add_dependencies(18_day_harib15c_ipl10_od 18_day_harib15c_ipl10_wine)
add_dependencies(od 18_day_harib15c_ipl10_od)

#----------------------------------------------------------
########### next target ###############
set(18_day_harib15c_naskfunc_SRCS ${root_SOURCE_DIR}/projects/18_day/harib15c/naskfunc.nas)
set(18_day_harib15c_naskfunc_OUTS ${root_BINARY_DIR}/projects/18_day/harib15c/naskfunc.img)

add_custom_target(18_day_harib15c_naskfunc
  COMMAND ${GOSK_EXECUTABLE} ${18_day_harib15c_naskfunc_SRCS} ${18_day_harib15c_naskfunc_OUTS}
  DEPENDS ${18_day_harib15c_naskfunc_SRCS}
)

add_dependencies(images 18_day_harib15c_naskfunc)
set(18_day_harib15c_naskfunc_WINE_OUTS ${root_BINARY_DIR}/projects/18_day/harib15c/naskfunc_wine.img)
add_custom_target(18_day_harib15c_naskfunc_wine
  COMMAND ${WINE} ${WINE_NASK} ${18_day_harib15c_naskfunc_SRCS} ${18_day_harib15c_naskfunc_WINE_OUTS}
)
add_dependencies(wine 18_day_harib15c_naskfunc_wine)

add_custom_target(18_day_harib15c_naskfunc_od
  COMMAND ${OD} -t x1 ${18_day_harib15c_naskfunc_OUTS}      > ${18_day_harib15c_naskfunc_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${18_day_harib15c_naskfunc_WINE_OUTS} > ${18_day_harib15c_naskfunc_WINE_OUTS}_t.txt
  COMMAND diff -s ${18_day_harib15c_naskfunc_OUTS}_f.txt ${18_day_harib15c_naskfunc_WINE_OUTS}_t.txt
)

add_dependencies(18_day_harib15c_naskfunc_od 18_day_harib15c_naskfunc)
add_dependencies(18_day_harib15c_naskfunc_od 18_day_harib15c_naskfunc_wine)
add_dependencies(od 18_day_harib15c_naskfunc_od)

#----------------------------------------------------------
