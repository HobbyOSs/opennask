#----------------------------------------------------------
message(STATUS "Entering directory projects/11_day/harib08d/")

set(FONT ${root_BINARY_DIR}/src/makefont)
set(B2O  ${root_BINARY_DIR}/src/bin2obj)
set(CONV ${root_BINARY_DIR}/objconv/objconv)
set(11_day_harib08d_WILDOBJ ${root_BINARY_DIR}/projects/11_day/harib08d/*.o)

# Full OS build rules
set(11_day_harib08d_OS    ${root_BINARY_DIR}/projects/11_day/harib08d/os.img)
set(11_day_harib08d_SYS   ${root_BINARY_DIR}/projects/11_day/harib08d/os.sys)
set(11_day_harib08d_IPLB  ${root_BINARY_DIR}/projects/11_day/harib08d/ipl.bin)
set(11_day_harib08d_IPLS  ${root_SOURCE_DIR}/projects/11_day/harib08d/ipl10.nas)
set(11_day_harib08d_HEADB ${root_BINARY_DIR}/projects/11_day/harib08d/asmhead.bin)
set(11_day_harib08d_HEADS ${root_SOURCE_DIR}/projects/11_day/harib08d/asmhead.nas)
set(11_day_harib08d_BOOTB ${root_BINARY_DIR}/projects/11_day/harib08d/boot.bin)
set(11_day_harib08d_CCO   ${root_BINARY_DIR}/projects/11_day/harib08d/boot.o)
set(11_day_harib08d_CCS   ${root_SOURCE_DIR}/projects/11_day/harib08d/*.c)
set(11_day_harib08d_LDS   ${root_SOURCE_DIR}/projects/os.lds)
set(11_day_harib08d_FUNCS ${root_SOURCE_DIR}/projects/11_day/harib08d/naskfunc.nas)
set(11_day_harib08d_FUNCO ${root_BINARY_DIR}/projects/11_day/harib08d/naskfunc.o)
set(11_day_harib08d_FONTS ${root_SOURCE_DIR}/projects/11_day/harib08d/hankaku.txt)
set(11_day_harib08d_FONTB ${root_BINARY_DIR}/projects/11_day/harib08d/hankaku.bin)
set(11_day_harib08d_FONTO ${root_BINARY_DIR}/projects/11_day/harib08d/hankaku.o)
set(11_day_harib08d_LIBGE ${root_BINARY_DIR}/golibc/libgo.a)
set(11_day_harib08d_LIBGC ${root_BINARY_DIR}/projects/11_day/harib08d/libgo.o)

add_custom_target(11_day_harib08d_run
  COMMAND ${QEMU} ${QEMUOPT} ${11_day_harib08d_OS}
  DEPENDS 11_day_harib08d_img
)
add_custom_target(11_day_harib08d_debug
  COMMAND ${QEMU} -s -S ${QEMUOPT} ${11_day_harib08d_OS} ${QEMU_DEBUG_OPT}
  DEPENDS 11_day_harib08d_img
)
add_custom_target(11_day_harib08d_clean
  COMMAND rm -f ${11_day_harib08d_OS}
  COMMAND rm -f ${11_day_harib08d_SYS}
  COMMAND rm -f ${11_day_harib08d_IPLB}
  COMMAND rm -f ${11_day_harib08d_HEADB}
  COMMAND rm -f ${11_day_harib08d_BOOTB}
  COMMAND rm -f ${11_day_harib08d_WILDOBJ}
  COMMAND rm -f ${root_BINARY_DIR}/projects/11_day/harib08d/ipl10.bin
  COMMAND rm -f ${root_BINARY_DIR}/projects/11_day/harib08d/asmhead.bin
  COMMAND rm -f ${root_BINARY_DIR}/projects/11_day/harib08d/*_wine.bin
  COMMAND rm -f ${root_BINARY_DIR}/projects/11_day/harib08d/*.txt
)
add_custom_target(11_day_harib08d_ipl_build
  COMMAND ${GOSK_EXECUTABLE} ${11_day_harib08d_IPLS} ${11_day_harib08d_IPLB}
  DEPENDS ${11_day_harib08d_IPLS}
)
add_custom_command(
  OUTPUT ${11_day_harib08d_HEADB}
  COMMAND ${GOSK_EXECUTABLE} ${11_day_harib08d_HEADS} ${11_day_harib08d_HEADB}
  DEPENDS ${11_day_harib08d_HEADS}
  COMMENT "Generating asmhead.bin for 11_day_harib08d"
)

add_custom_target(11_day_harib08d_sys
  COMMAND ${FONT} ${11_day_harib08d_FONTS} ${11_day_harib08d_FONTB}
  COMMAND ${B2O}  ${11_day_harib08d_FONTB} ${11_day_harib08d_FONTO} _hankaku
  COMMAND ${CONV} -fcoff32 -nu ${11_day_harib08d_LIBGE} ${11_day_harib08d_LIBGC}
  COMMAND ${GOSK_EXECUTABLE} ${11_day_harib08d_FUNCS} ${11_day_harib08d_FUNCO}
  COMMAND gcc ${BINOPT} -T ${11_day_harib08d_LDS} ${11_day_harib08d_CCS} ${11_day_harib08d_WILDOBJ} -o ${11_day_harib08d_BOOTB}
  COMMAND cat ${11_day_harib08d_HEADB} ${11_day_harib08d_BOOTB} > ${11_day_harib08d_SYS}
  DEPENDS ${11_day_harib08d_HEADB} 11_day_harib08d_ipl_build
  DEPENDS ${11_day_harib08d_FUNCO}
  DEPENDS ${11_day_harib08d_FONTO}
)
add_custom_target(11_day_harib08d_img
  COMMAND mformat -f 1440 -l HARIBOTEOS -N 0xffffffff -C -B ${11_day_harib08d_IPLB} -i ${11_day_harib08d_OS}
  COMMAND mcopy -i ${11_day_harib08d_OS} ${11_day_harib08d_SYS} ::
  DEPENDS 11_day_harib08d_sys
)

########### next target ###############
set(11_day_harib08d_asmhead_SRCS ${root_SOURCE_DIR}/projects/11_day/harib08d/asmhead.nas)
set(11_day_harib08d_asmhead_OUTS ${root_BINARY_DIR}/projects/11_day/harib08d/asmhead.bin)

add_custom_target(11_day_harib08d_asmhead
  COMMAND ${GOSK_EXECUTABLE} ${11_day_harib08d_asmhead_SRCS} ${11_day_harib08d_asmhead_OUTS}
  DEPENDS ${11_day_harib08d_asmhead_SRCS}
)

set(11_day_harib08d_asmhead_WINE_OUTS ${root_BINARY_DIR}/projects/11_day/harib08d/asmhead_wine.bin)
add_custom_target(11_day_harib08d_asmhead_wine
  COMMAND ${WINE} ${WINE_NASK} ${11_day_harib08d_asmhead_SRCS} ${11_day_harib08d_asmhead_WINE_OUTS}
)
add_dependencies(wine 11_day_harib08d_asmhead_wine)

add_custom_target(11_day_harib08d_asmhead_od
  COMMAND ${OD} -t x1 ${11_day_harib08d_asmhead_OUTS}      > ${11_day_harib08d_asmhead_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${11_day_harib08d_asmhead_WINE_OUTS} > ${11_day_harib08d_asmhead_WINE_OUTS}_t.txt
  COMMAND diff -s ${11_day_harib08d_asmhead_OUTS}_f.txt ${11_day_harib08d_asmhead_WINE_OUTS}_t.txt
)

add_dependencies(11_day_harib08d_asmhead_od 11_day_harib08d_asmhead)
add_dependencies(11_day_harib08d_asmhead_od 11_day_harib08d_asmhead_wine)
add_dependencies(od 11_day_harib08d_asmhead_od)

#----------------------------------------------------------
########### next target ###############
set(11_day_harib08d_ipl10_SRCS ${root_SOURCE_DIR}/projects/11_day/harib08d/ipl10.nas)
set(11_day_harib08d_ipl10_OUTS ${root_BINARY_DIR}/projects/11_day/harib08d/ipl10.bin)

add_custom_target(11_day_harib08d_ipl10
  COMMAND ${GOSK_EXECUTABLE} ${11_day_harib08d_ipl10_SRCS} ${11_day_harib08d_ipl10_OUTS}
  DEPENDS ${11_day_harib08d_ipl10_SRCS}
)

set(11_day_harib08d_ipl10_WINE_OUTS ${root_BINARY_DIR}/projects/11_day/harib08d/ipl10_wine.bin)
add_custom_target(11_day_harib08d_ipl10_wine
  COMMAND ${WINE} ${WINE_NASK} ${11_day_harib08d_ipl10_SRCS} ${11_day_harib08d_ipl10_WINE_OUTS}
)
add_dependencies(wine 11_day_harib08d_ipl10_wine)

add_custom_target(11_day_harib08d_ipl10_od
  COMMAND ${OD} -t x1 ${11_day_harib08d_ipl10_OUTS}      > ${11_day_harib08d_ipl10_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${11_day_harib08d_ipl10_WINE_OUTS} > ${11_day_harib08d_ipl10_WINE_OUTS}_t.txt
  COMMAND diff -s ${11_day_harib08d_ipl10_OUTS}_f.txt ${11_day_harib08d_ipl10_WINE_OUTS}_t.txt
)

add_dependencies(11_day_harib08d_ipl10_od 11_day_harib08d_ipl10)
add_dependencies(11_day_harib08d_ipl10_od 11_day_harib08d_ipl10_wine)
add_dependencies(od 11_day_harib08d_ipl10_od)

#----------------------------------------------------------
########### next target ###############
set(11_day_harib08d_naskfunc_SRCS ${root_SOURCE_DIR}/projects/11_day/harib08d/naskfunc.nas)
set(11_day_harib08d_naskfunc_OUTS ${root_BINARY_DIR}/projects/11_day/harib08d/naskfunc.img)

add_custom_target(11_day_harib08d_naskfunc
  COMMAND ${GOSK_EXECUTABLE} ${11_day_harib08d_naskfunc_SRCS} ${11_day_harib08d_naskfunc_OUTS}
  DEPENDS ${11_day_harib08d_naskfunc_SRCS}
)

add_dependencies(images 11_day_harib08d_naskfunc)
set(11_day_harib08d_naskfunc_WINE_OUTS ${root_BINARY_DIR}/projects/11_day/harib08d/naskfunc_wine.img)
add_custom_target(11_day_harib08d_naskfunc_wine
  COMMAND ${WINE} ${WINE_NASK} ${11_day_harib08d_naskfunc_SRCS} ${11_day_harib08d_naskfunc_WINE_OUTS}
)
add_dependencies(wine 11_day_harib08d_naskfunc_wine)

add_custom_target(11_day_harib08d_naskfunc_od
  COMMAND ${OD} -t x1 ${11_day_harib08d_naskfunc_OUTS}      > ${11_day_harib08d_naskfunc_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${11_day_harib08d_naskfunc_WINE_OUTS} > ${11_day_harib08d_naskfunc_WINE_OUTS}_t.txt
  COMMAND diff -s ${11_day_harib08d_naskfunc_OUTS}_f.txt ${11_day_harib08d_naskfunc_WINE_OUTS}_t.txt
)

add_dependencies(11_day_harib08d_naskfunc_od 11_day_harib08d_naskfunc)
add_dependencies(11_day_harib08d_naskfunc_od 11_day_harib08d_naskfunc_wine)
add_dependencies(od 11_day_harib08d_naskfunc_od)

#----------------------------------------------------------
