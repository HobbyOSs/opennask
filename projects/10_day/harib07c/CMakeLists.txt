#----------------------------------------------------------
message(STATUS "Entering directory projects/10_day/harib07c/")

set(FONT ${root_BINARY_DIR}/src/makefont)
set(B2O  ${root_BINARY_DIR}/src/bin2obj)
set(CONV ${root_BINARY_DIR}/objconv/objconv)
set(10_day_harib07c_WILDOBJ ${root_BINARY_DIR}/projects/10_day/harib07c/*.o)

# Full OS build rules
set(10_day_harib07c_OS    ${root_BINARY_DIR}/projects/10_day/harib07c/os.img)
set(10_day_harib07c_SYS   ${root_BINARY_DIR}/projects/10_day/harib07c/os.sys)
set(10_day_harib07c_IPLB  ${root_BINARY_DIR}/projects/10_day/harib07c/ipl.bin)
set(10_day_harib07c_IPLS  ${root_SOURCE_DIR}/projects/10_day/harib07c/ipl10.nas)
set(10_day_harib07c_HEADB ${root_BINARY_DIR}/projects/10_day/harib07c/asmhead.bin)
set(10_day_harib07c_HEADS ${root_SOURCE_DIR}/projects/10_day/harib07c/asmhead.nas)
set(10_day_harib07c_BOOTB ${root_BINARY_DIR}/projects/10_day/harib07c/boot.bin)
set(10_day_harib07c_CCO   ${root_BINARY_DIR}/projects/10_day/harib07c/boot.o)
set(10_day_harib07c_CCS   ${root_SOURCE_DIR}/projects/10_day/harib07c/*.c)
set(10_day_harib07c_LDS   ${root_SOURCE_DIR}/projects/os.lds)
set(10_day_harib07c_FUNCS ${root_SOURCE_DIR}/projects/10_day/harib07c/naskfunc.nas)
set(10_day_harib07c_FUNCO ${root_BINARY_DIR}/projects/10_day/harib07c/naskfunc.o)
set(10_day_harib07c_FONTS ${root_SOURCE_DIR}/projects/10_day/harib07c/hankaku.txt)
set(10_day_harib07c_FONTB ${root_BINARY_DIR}/projects/10_day/harib07c/hankaku.bin)
set(10_day_harib07c_FONTO ${root_BINARY_DIR}/projects/10_day/harib07c/hankaku.o)
set(10_day_harib07c_LIBGE ${root_BINARY_DIR}/golibc/libgo.a)
set(10_day_harib07c_LIBGC ${root_BINARY_DIR}/projects/10_day/harib07c/libgo.o)

add_custom_target(10_day_harib07c_run
  COMMAND ${QEMU} ${QEMUOPT} ${10_day_harib07c_OS}
  DEPENDS 10_day_harib07c_img
)
add_custom_target(10_day_harib07c_debug
  COMMAND ${QEMU} -s -S ${QEMUOPT} ${10_day_harib07c_OS} ${QEMU_DEBUG_OPT}
  DEPENDS 10_day_harib07c_img
)
add_custom_target(10_day_harib07c_clean
  COMMAND rm -f ${10_day_harib07c_OS}
  COMMAND rm -f ${10_day_harib07c_SYS}
  COMMAND rm -f ${10_day_harib07c_IPLB}
  COMMAND rm -f ${10_day_harib07c_HEADB}
  COMMAND rm -f ${10_day_harib07c_BOOTB}
  COMMAND rm -f ${10_day_harib07c_WILDOBJ}
  COMMAND rm -f ${root_BINARY_DIR}/projects/10_day/harib07c/ipl10.bin
  COMMAND rm -f ${root_BINARY_DIR}/projects/10_day/harib07c/asmhead.bin
  COMMAND rm -f ${root_BINARY_DIR}/projects/10_day/harib07c/*_wine.bin
  COMMAND rm -f ${root_BINARY_DIR}/projects/10_day/harib07c/*.txt
)
add_custom_target(10_day_harib07c_ipl_build
  COMMAND ${GOSK_EXECUTABLE} ${10_day_harib07c_IPLS} ${10_day_harib07c_IPLB}
  DEPENDS ${10_day_harib07c_IPLS}
)
add_custom_command(
  OUTPUT ${10_day_harib07c_HEADB}
  COMMAND ${GOSK_EXECUTABLE} ${10_day_harib07c_HEADS} ${10_day_harib07c_HEADB}
  DEPENDS ${10_day_harib07c_HEADS}
  COMMENT "Generating asmhead.bin for 10_day_harib07c"
)

add_custom_target(10_day_harib07c_sys
  COMMAND ${FONT} ${10_day_harib07c_FONTS} ${10_day_harib07c_FONTB}
  COMMAND ${B2O}  ${10_day_harib07c_FONTB} ${10_day_harib07c_FONTO} _hankaku
  COMMAND ${CONV} -fcoff32 -nu ${10_day_harib07c_LIBGE} ${10_day_harib07c_LIBGC}
  COMMAND ${GOSK_EXECUTABLE} ${10_day_harib07c_FUNCS} ${10_day_harib07c_FUNCO}
  COMMAND gcc ${BINOPT} -T ${10_day_harib07c_LDS} ${10_day_harib07c_CCS} ${10_day_harib07c_WILDOBJ} -o ${10_day_harib07c_BOOTB}
  COMMAND cat ${10_day_harib07c_HEADB} ${10_day_harib07c_BOOTB} > ${10_day_harib07c_SYS}
  DEPENDS ${10_day_harib07c_HEADB} 10_day_harib07c_ipl_build
  DEPENDS ${10_day_harib07c_FUNCO}
  DEPENDS ${10_day_harib07c_FONTO}
)
add_custom_target(10_day_harib07c_img
  COMMAND mformat -f 1440 -l HARIBOTEOS -N 0xffffffff -C -B ${10_day_harib07c_IPLB} -i ${10_day_harib07c_OS}
  COMMAND mcopy -i ${10_day_harib07c_OS} ${10_day_harib07c_SYS} ::
  DEPENDS 10_day_harib07c_sys
)

########### next target ###############
set(10_day_harib07c_asmhead_SRCS ${root_SOURCE_DIR}/projects/10_day/harib07c/asmhead.nas)
set(10_day_harib07c_asmhead_OUTS ${root_BINARY_DIR}/projects/10_day/harib07c/asmhead.bin)

add_custom_target(10_day_harib07c_asmhead
  COMMAND ${GOSK_EXECUTABLE} ${10_day_harib07c_asmhead_SRCS} ${10_day_harib07c_asmhead_OUTS}
  DEPENDS ${10_day_harib07c_asmhead_SRCS}
)

set(10_day_harib07c_asmhead_WINE_OUTS ${root_BINARY_DIR}/projects/10_day/harib07c/asmhead_wine.bin)
add_custom_target(10_day_harib07c_asmhead_wine
  COMMAND ${WINE} ${WINE_NASK} ${10_day_harib07c_asmhead_SRCS} ${10_day_harib07c_asmhead_WINE_OUTS}
)
add_dependencies(wine 10_day_harib07c_asmhead_wine)

add_custom_target(10_day_harib07c_asmhead_od
  COMMAND ${OD} -t x1 ${10_day_harib07c_asmhead_OUTS}      > ${10_day_harib07c_asmhead_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${10_day_harib07c_asmhead_WINE_OUTS} > ${10_day_harib07c_asmhead_WINE_OUTS}_t.txt
  COMMAND diff -s ${10_day_harib07c_asmhead_OUTS}_f.txt ${10_day_harib07c_asmhead_WINE_OUTS}_t.txt
)

add_dependencies(10_day_harib07c_asmhead_od 10_day_harib07c_asmhead)
add_dependencies(10_day_harib07c_asmhead_od 10_day_harib07c_asmhead_wine)
add_dependencies(od 10_day_harib07c_asmhead_od)

#----------------------------------------------------------
########### next target ###############
set(10_day_harib07c_ipl10_SRCS ${root_SOURCE_DIR}/projects/10_day/harib07c/ipl10.nas)
set(10_day_harib07c_ipl10_OUTS ${root_BINARY_DIR}/projects/10_day/harib07c/ipl10.bin)

add_custom_target(10_day_harib07c_ipl10
  COMMAND ${GOSK_EXECUTABLE} ${10_day_harib07c_ipl10_SRCS} ${10_day_harib07c_ipl10_OUTS}
  DEPENDS ${10_day_harib07c_ipl10_SRCS}
)

set(10_day_harib07c_ipl10_WINE_OUTS ${root_BINARY_DIR}/projects/10_day/harib07c/ipl10_wine.bin)
add_custom_target(10_day_harib07c_ipl10_wine
  COMMAND ${WINE} ${WINE_NASK} ${10_day_harib07c_ipl10_SRCS} ${10_day_harib07c_ipl10_WINE_OUTS}
)
add_dependencies(wine 10_day_harib07c_ipl10_wine)

add_custom_target(10_day_harib07c_ipl10_od
  COMMAND ${OD} -t x1 ${10_day_harib07c_ipl10_OUTS}      > ${10_day_harib07c_ipl10_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${10_day_harib07c_ipl10_WINE_OUTS} > ${10_day_harib07c_ipl10_WINE_OUTS}_t.txt
  COMMAND diff -s ${10_day_harib07c_ipl10_OUTS}_f.txt ${10_day_harib07c_ipl10_WINE_OUTS}_t.txt
)

add_dependencies(10_day_harib07c_ipl10_od 10_day_harib07c_ipl10)
add_dependencies(10_day_harib07c_ipl10_od 10_day_harib07c_ipl10_wine)
add_dependencies(od 10_day_harib07c_ipl10_od)

#----------------------------------------------------------
########### next target ###############
set(10_day_harib07c_naskfunc_SRCS ${root_SOURCE_DIR}/projects/10_day/harib07c/naskfunc.nas)
set(10_day_harib07c_naskfunc_OUTS ${root_BINARY_DIR}/projects/10_day/harib07c/naskfunc.img)

add_custom_target(10_day_harib07c_naskfunc
  COMMAND ${GOSK_EXECUTABLE} ${10_day_harib07c_naskfunc_SRCS} ${10_day_harib07c_naskfunc_OUTS}
  DEPENDS ${10_day_harib07c_naskfunc_SRCS}
)

add_dependencies(images 10_day_harib07c_naskfunc)
set(10_day_harib07c_naskfunc_WINE_OUTS ${root_BINARY_DIR}/projects/10_day/harib07c/naskfunc_wine.img)
add_custom_target(10_day_harib07c_naskfunc_wine
  COMMAND ${WINE} ${WINE_NASK} ${10_day_harib07c_naskfunc_SRCS} ${10_day_harib07c_naskfunc_WINE_OUTS}
)
add_dependencies(wine 10_day_harib07c_naskfunc_wine)

add_custom_target(10_day_harib07c_naskfunc_od
  COMMAND ${OD} -t x1 ${10_day_harib07c_naskfunc_OUTS}      > ${10_day_harib07c_naskfunc_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${10_day_harib07c_naskfunc_WINE_OUTS} > ${10_day_harib07c_naskfunc_WINE_OUTS}_t.txt
  COMMAND diff -s ${10_day_harib07c_naskfunc_OUTS}_f.txt ${10_day_harib07c_naskfunc_WINE_OUTS}_t.txt
)

add_dependencies(10_day_harib07c_naskfunc_od 10_day_harib07c_naskfunc)
add_dependencies(10_day_harib07c_naskfunc_od 10_day_harib07c_naskfunc_wine)
add_dependencies(od 10_day_harib07c_naskfunc_od)

#----------------------------------------------------------
