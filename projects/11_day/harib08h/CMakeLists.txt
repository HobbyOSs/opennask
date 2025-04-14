#----------------------------------------------------------
message(STATUS "Entering directory projects/11_day/harib08h/")

set(FONT ${root_BINARY_DIR}/src/makefont)
set(B2O  ${root_BINARY_DIR}/src/bin2obj)
set(CONV ${root_BINARY_DIR}/objconv/objconv)
set(11_day_harib08h_WILDOBJ ${root_BINARY_DIR}/projects/11_day/harib08h/*.o)

# Full OS build rules
set(11_day_harib08h_OS    ${root_BINARY_DIR}/projects/11_day/harib08h/os.img)
set(11_day_harib08h_SYS   ${root_BINARY_DIR}/projects/11_day/harib08h/os.sys)
set(11_day_harib08h_IPLB  ${root_BINARY_DIR}/projects/11_day/harib08h/ipl.bin)
set(11_day_harib08h_IPLS  ${root_SOURCE_DIR}/projects/11_day/harib08h/ipl10.nas)
set(11_day_harib08h_HEADB ${root_BINARY_DIR}/projects/11_day/harib08h/asmhead.bin)
set(11_day_harib08h_HEADS ${root_SOURCE_DIR}/projects/11_day/harib08h/asmhead.nas)
set(11_day_harib08h_BOOTB ${root_BINARY_DIR}/projects/11_day/harib08h/boot.bin)
set(11_day_harib08h_CCO   ${root_BINARY_DIR}/projects/11_day/harib08h/boot.o)
set(11_day_harib08h_CCS   ${root_SOURCE_DIR}/projects/11_day/harib08h/*.c)
set(11_day_harib08h_LDS   ${root_SOURCE_DIR}/projects/os.lds)
set(11_day_harib08h_FUNCS ${root_SOURCE_DIR}/projects/11_day/harib08h/naskfunc.nas)
set(11_day_harib08h_FUNCO ${root_BINARY_DIR}/projects/11_day/harib08h/naskfunc.o)
set(11_day_harib08h_FONTS ${root_SOURCE_DIR}/projects/11_day/harib08h/hankaku.txt)
set(11_day_harib08h_FONTB ${root_BINARY_DIR}/projects/11_day/harib08h/hankaku.bin)
set(11_day_harib08h_FONTO ${root_BINARY_DIR}/projects/11_day/harib08h/hankaku.o)
set(11_day_harib08h_LIBGE ${root_BINARY_DIR}/golibc/libgo.a)
set(11_day_harib08h_LIBGC ${root_BINARY_DIR}/projects/11_day/harib08h/libgo.o)

add_custom_target(11_day_harib08h_run
  COMMAND ${QEMU} ${QEMUOPT} ${11_day_harib08h_OS}
  DEPENDS 11_day_harib08h_img
)
add_custom_target(11_day_harib08h_debug
  COMMAND ${QEMU} -s -S ${QEMUOPT} ${11_day_harib08h_OS} ${QEMU_DEBUG_OPT}
  DEPENDS 11_day_harib08h_img
)
add_custom_target(11_day_harib08h_clean
  COMMAND rm -f ${11_day_harib08h_OS}
  COMMAND rm -f ${11_day_harib08h_SYS}
  COMMAND rm -f ${11_day_harib08h_IPLB}
  COMMAND rm -f ${11_day_harib08h_HEADB}
  COMMAND rm -f ${11_day_harib08h_BOOTB}
  COMMAND rm -f ${11_day_harib08h_WILDOBJ}
  COMMAND rm -f ${root_BINARY_DIR}/projects/11_day/harib08h/ipl10.bin
  COMMAND rm -f ${root_BINARY_DIR}/projects/11_day/harib08h/asmhead.bin
  COMMAND rm -f ${root_BINARY_DIR}/projects/11_day/harib08h/*_wine.bin
  COMMAND rm -f ${root_BINARY_DIR}/projects/11_day/harib08h/*.txt
)
add_custom_target(11_day_harib08h_ipl_build
  COMMAND ${GOSK_EXECUTABLE} ${11_day_harib08h_IPLS} ${11_day_harib08h_IPLB}
  DEPENDS ${11_day_harib08h_IPLS}
)
add_custom_command(
  OUTPUT ${11_day_harib08h_HEADB}
  COMMAND ${GOSK_EXECUTABLE} ${11_day_harib08h_HEADS} ${11_day_harib08h_HEADB}
  DEPENDS ${11_day_harib08h_HEADS}
  COMMENT "Generating asmhead.bin for 11_day_harib08h"
)

add_custom_target(11_day_harib08h_sys
  COMMAND ${FONT} ${11_day_harib08h_FONTS} ${11_day_harib08h_FONTB}
  COMMAND ${B2O}  ${11_day_harib08h_FONTB} ${11_day_harib08h_FONTO} _hankaku
  COMMAND ${CONV} -fcoff32 -nu ${11_day_harib08h_LIBGE} ${11_day_harib08h_LIBGC}
  COMMAND ${GOSK_EXECUTABLE} ${11_day_harib08h_FUNCS} ${11_day_harib08h_FUNCO}
  COMMAND gcc ${BINOPT} -T ${11_day_harib08h_LDS} ${11_day_harib08h_CCS} ${11_day_harib08h_WILDOBJ} -o ${11_day_harib08h_BOOTB}
  COMMAND cat ${11_day_harib08h_HEADB} ${11_day_harib08h_BOOTB} > ${11_day_harib08h_SYS}
  DEPENDS ${11_day_harib08h_HEADB} 11_day_harib08h_ipl_build
  DEPENDS ${11_day_harib08h_FUNCO}
  DEPENDS ${11_day_harib08h_FONTO}
)
add_custom_target(11_day_harib08h_img
  COMMAND mformat -f 1440 -l HARIBOTEOS -N 0xffffffff -C -B ${11_day_harib08h_IPLB} -i ${11_day_harib08h_OS}
  COMMAND mcopy -i ${11_day_harib08h_OS} ${11_day_harib08h_SYS} ::
  DEPENDS 11_day_harib08h_sys
)

########### next target ###############
set(11_day_harib08h_asmhead_SRCS ${root_SOURCE_DIR}/projects/11_day/harib08h/asmhead.nas)
set(11_day_harib08h_asmhead_OUTS ${root_BINARY_DIR}/projects/11_day/harib08h/asmhead.bin)

add_custom_target(11_day_harib08h_asmhead
  COMMAND ${GOSK_EXECUTABLE} ${11_day_harib08h_asmhead_SRCS} ${11_day_harib08h_asmhead_OUTS}
  DEPENDS ${11_day_harib08h_asmhead_SRCS}
)

set(11_day_harib08h_asmhead_WINE_OUTS ${root_BINARY_DIR}/projects/11_day/harib08h/asmhead_wine.bin)
add_custom_target(11_day_harib08h_asmhead_wine
  COMMAND ${WINE} ${WINE_NASK} ${11_day_harib08h_asmhead_SRCS} ${11_day_harib08h_asmhead_WINE_OUTS}
)
add_dependencies(wine 11_day_harib08h_asmhead_wine)

add_custom_target(11_day_harib08h_asmhead_od
  COMMAND ${OD} -t x1 ${11_day_harib08h_asmhead_OUTS}      > ${11_day_harib08h_asmhead_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${11_day_harib08h_asmhead_WINE_OUTS} > ${11_day_harib08h_asmhead_WINE_OUTS}_t.txt
  COMMAND diff -s ${11_day_harib08h_asmhead_OUTS}_f.txt ${11_day_harib08h_asmhead_WINE_OUTS}_t.txt
)

add_dependencies(11_day_harib08h_asmhead_od 11_day_harib08h_asmhead)
add_dependencies(11_day_harib08h_asmhead_od 11_day_harib08h_asmhead_wine)
add_dependencies(od 11_day_harib08h_asmhead_od)

#----------------------------------------------------------
########### next target ###############
set(11_day_harib08h_ipl10_SRCS ${root_SOURCE_DIR}/projects/11_day/harib08h/ipl10.nas)
set(11_day_harib08h_ipl10_OUTS ${root_BINARY_DIR}/projects/11_day/harib08h/ipl10.bin)

add_custom_target(11_day_harib08h_ipl10
  COMMAND ${GOSK_EXECUTABLE} ${11_day_harib08h_ipl10_SRCS} ${11_day_harib08h_ipl10_OUTS}
  DEPENDS ${11_day_harib08h_ipl10_SRCS}
)

set(11_day_harib08h_ipl10_WINE_OUTS ${root_BINARY_DIR}/projects/11_day/harib08h/ipl10_wine.bin)
add_custom_target(11_day_harib08h_ipl10_wine
  COMMAND ${WINE} ${WINE_NASK} ${11_day_harib08h_ipl10_SRCS} ${11_day_harib08h_ipl10_WINE_OUTS}
)
add_dependencies(wine 11_day_harib08h_ipl10_wine)

add_custom_target(11_day_harib08h_ipl10_od
  COMMAND ${OD} -t x1 ${11_day_harib08h_ipl10_OUTS}      > ${11_day_harib08h_ipl10_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${11_day_harib08h_ipl10_WINE_OUTS} > ${11_day_harib08h_ipl10_WINE_OUTS}_t.txt
  COMMAND diff -s ${11_day_harib08h_ipl10_OUTS}_f.txt ${11_day_harib08h_ipl10_WINE_OUTS}_t.txt
)

add_dependencies(11_day_harib08h_ipl10_od 11_day_harib08h_ipl10)
add_dependencies(11_day_harib08h_ipl10_od 11_day_harib08h_ipl10_wine)
add_dependencies(od 11_day_harib08h_ipl10_od)

#----------------------------------------------------------
########### next target ###############
set(11_day_harib08h_naskfunc_SRCS ${root_SOURCE_DIR}/projects/11_day/harib08h/naskfunc.nas)
set(11_day_harib08h_naskfunc_OUTS ${root_BINARY_DIR}/projects/11_day/harib08h/naskfunc.img)

add_custom_target(11_day_harib08h_naskfunc
  COMMAND ${GOSK_EXECUTABLE} ${11_day_harib08h_naskfunc_SRCS} ${11_day_harib08h_naskfunc_OUTS}
  DEPENDS ${11_day_harib08h_naskfunc_SRCS}
)

add_dependencies(images 11_day_harib08h_naskfunc)
set(11_day_harib08h_naskfunc_WINE_OUTS ${root_BINARY_DIR}/projects/11_day/harib08h/naskfunc_wine.img)
add_custom_target(11_day_harib08h_naskfunc_wine
  COMMAND ${WINE} ${WINE_NASK} ${11_day_harib08h_naskfunc_SRCS} ${11_day_harib08h_naskfunc_WINE_OUTS}
)
add_dependencies(wine 11_day_harib08h_naskfunc_wine)

add_custom_target(11_day_harib08h_naskfunc_od
  COMMAND ${OD} -t x1 ${11_day_harib08h_naskfunc_OUTS}      > ${11_day_harib08h_naskfunc_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${11_day_harib08h_naskfunc_WINE_OUTS} > ${11_day_harib08h_naskfunc_WINE_OUTS}_t.txt
  COMMAND diff -s ${11_day_harib08h_naskfunc_OUTS}_f.txt ${11_day_harib08h_naskfunc_WINE_OUTS}_t.txt
)

add_dependencies(11_day_harib08h_naskfunc_od 11_day_harib08h_naskfunc)
add_dependencies(11_day_harib08h_naskfunc_od 11_day_harib08h_naskfunc_wine)
add_dependencies(od 11_day_harib08h_naskfunc_od)

#----------------------------------------------------------
