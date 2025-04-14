#----------------------------------------------------------
message(STATUS "Entering directory projects/11_day/harib08b/")

set(FONT ${root_BINARY_DIR}/src/makefont)
set(B2O  ${root_BINARY_DIR}/src/bin2obj)
set(CONV ${root_BINARY_DIR}/objconv/objconv)
set(11_day_harib08b_WILDOBJ ${root_BINARY_DIR}/projects/11_day/harib08b/*.o)

# Full OS build rules
set(11_day_harib08b_OS    ${root_BINARY_DIR}/projects/11_day/harib08b/os.img)
set(11_day_harib08b_SYS   ${root_BINARY_DIR}/projects/11_day/harib08b/os.sys)
set(11_day_harib08b_IPLB  ${root_BINARY_DIR}/projects/11_day/harib08b/ipl.bin)
set(11_day_harib08b_IPLS  ${root_SOURCE_DIR}/projects/11_day/harib08b/ipl10.nas)
set(11_day_harib08b_HEADB ${root_BINARY_DIR}/projects/11_day/harib08b/asmhead.bin)
set(11_day_harib08b_HEADS ${root_SOURCE_DIR}/projects/11_day/harib08b/asmhead.nas)
set(11_day_harib08b_BOOTB ${root_BINARY_DIR}/projects/11_day/harib08b/boot.bin)
set(11_day_harib08b_CCO   ${root_BINARY_DIR}/projects/11_day/harib08b/boot.o)
set(11_day_harib08b_CCS   ${root_SOURCE_DIR}/projects/11_day/harib08b/*.c)
set(11_day_harib08b_LDS   ${root_SOURCE_DIR}/projects/os.lds)
set(11_day_harib08b_FUNCS ${root_SOURCE_DIR}/projects/11_day/harib08b/naskfunc.nas)
set(11_day_harib08b_FUNCO ${root_BINARY_DIR}/projects/11_day/harib08b/naskfunc.o)
set(11_day_harib08b_FONTS ${root_SOURCE_DIR}/projects/11_day/harib08b/hankaku.txt)
set(11_day_harib08b_FONTB ${root_BINARY_DIR}/projects/11_day/harib08b/hankaku.bin)
set(11_day_harib08b_FONTO ${root_BINARY_DIR}/projects/11_day/harib08b/hankaku.o)
set(11_day_harib08b_LIBGE ${root_BINARY_DIR}/golibc/libgo.a)
set(11_day_harib08b_LIBGC ${root_BINARY_DIR}/projects/11_day/harib08b/libgo.o)

add_custom_target(11_day_harib08b_run
  COMMAND ${QEMU} ${QEMUOPT} ${11_day_harib08b_OS}
  DEPENDS 11_day_harib08b_img
)
add_custom_target(11_day_harib08b_debug
  COMMAND ${QEMU} -s -S ${QEMUOPT} ${11_day_harib08b_OS} ${QEMU_DEBUG_OPT}
  DEPENDS 11_day_harib08b_img
)
add_custom_target(11_day_harib08b_clean
  COMMAND rm -f ${11_day_harib08b_OS}
  COMMAND rm -f ${11_day_harib08b_SYS}
  COMMAND rm -f ${11_day_harib08b_IPLB}
  COMMAND rm -f ${11_day_harib08b_HEADB}
  COMMAND rm -f ${11_day_harib08b_BOOTB}
  COMMAND rm -f ${11_day_harib08b_WILDOBJ}
  COMMAND rm -f ${root_BINARY_DIR}/projects/11_day/harib08b/ipl10.bin
  COMMAND rm -f ${root_BINARY_DIR}/projects/11_day/harib08b/asmhead.bin
  COMMAND rm -f ${root_BINARY_DIR}/projects/11_day/harib08b/*_wine.bin
  COMMAND rm -f ${root_BINARY_DIR}/projects/11_day/harib08b/*.txt
)
add_custom_target(11_day_harib08b_ipl_build
  COMMAND ${GOSK_EXECUTABLE} ${11_day_harib08b_IPLS} ${11_day_harib08b_IPLB}
  DEPENDS ${11_day_harib08b_IPLS}
)
add_custom_command(
  OUTPUT ${11_day_harib08b_HEADB}
  COMMAND ${GOSK_EXECUTABLE} ${11_day_harib08b_HEADS} ${11_day_harib08b_HEADB}
  DEPENDS ${11_day_harib08b_HEADS}
  COMMENT "Generating asmhead.bin for 11_day_harib08b"
)

add_custom_target(11_day_harib08b_sys
  COMMAND ${FONT} ${11_day_harib08b_FONTS} ${11_day_harib08b_FONTB}
  COMMAND ${B2O}  ${11_day_harib08b_FONTB} ${11_day_harib08b_FONTO} _hankaku
  COMMAND ${CONV} -fcoff32 -nu ${11_day_harib08b_LIBGE} ${11_day_harib08b_LIBGC}
  COMMAND ${GOSK_EXECUTABLE} ${11_day_harib08b_FUNCS} ${11_day_harib08b_FUNCO}
  COMMAND gcc ${BINOPT} -T ${11_day_harib08b_LDS} ${11_day_harib08b_CCS} ${11_day_harib08b_WILDOBJ} -o ${11_day_harib08b_BOOTB}
  COMMAND cat ${11_day_harib08b_HEADB} ${11_day_harib08b_BOOTB} > ${11_day_harib08b_SYS}
  DEPENDS ${11_day_harib08b_HEADB} 11_day_harib08b_ipl_build
  DEPENDS ${11_day_harib08b_FUNCO}
  DEPENDS ${11_day_harib08b_FONTO}
)
add_custom_target(11_day_harib08b_img
  COMMAND mformat -f 1440 -l HARIBOTEOS -N 0xffffffff -C -B ${11_day_harib08b_IPLB} -i ${11_day_harib08b_OS}
  COMMAND mcopy -i ${11_day_harib08b_OS} ${11_day_harib08b_SYS} ::
  DEPENDS 11_day_harib08b_sys
)

########### next target ###############
set(11_day_harib08b_asmhead_SRCS ${root_SOURCE_DIR}/projects/11_day/harib08b/asmhead.nas)
set(11_day_harib08b_asmhead_OUTS ${root_BINARY_DIR}/projects/11_day/harib08b/asmhead.bin)

add_custom_target(11_day_harib08b_asmhead
  COMMAND ${GOSK_EXECUTABLE} ${11_day_harib08b_asmhead_SRCS} ${11_day_harib08b_asmhead_OUTS}
  DEPENDS ${11_day_harib08b_asmhead_SRCS}
)

set(11_day_harib08b_asmhead_WINE_OUTS ${root_BINARY_DIR}/projects/11_day/harib08b/asmhead_wine.bin)
add_custom_target(11_day_harib08b_asmhead_wine
  COMMAND ${WINE} ${WINE_NASK} ${11_day_harib08b_asmhead_SRCS} ${11_day_harib08b_asmhead_WINE_OUTS}
)
add_dependencies(wine 11_day_harib08b_asmhead_wine)

add_custom_target(11_day_harib08b_asmhead_od
  COMMAND ${OD} -t x1 ${11_day_harib08b_asmhead_OUTS}      > ${11_day_harib08b_asmhead_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${11_day_harib08b_asmhead_WINE_OUTS} > ${11_day_harib08b_asmhead_WINE_OUTS}_t.txt
  COMMAND diff -s ${11_day_harib08b_asmhead_OUTS}_f.txt ${11_day_harib08b_asmhead_WINE_OUTS}_t.txt
)

add_dependencies(11_day_harib08b_asmhead_od 11_day_harib08b_asmhead)
add_dependencies(11_day_harib08b_asmhead_od 11_day_harib08b_asmhead_wine)
add_dependencies(od 11_day_harib08b_asmhead_od)

#----------------------------------------------------------
########### next target ###############
set(11_day_harib08b_ipl10_SRCS ${root_SOURCE_DIR}/projects/11_day/harib08b/ipl10.nas)
set(11_day_harib08b_ipl10_OUTS ${root_BINARY_DIR}/projects/11_day/harib08b/ipl10.bin)

add_custom_target(11_day_harib08b_ipl10
  COMMAND ${GOSK_EXECUTABLE} ${11_day_harib08b_ipl10_SRCS} ${11_day_harib08b_ipl10_OUTS}
  DEPENDS ${11_day_harib08b_ipl10_SRCS}
)

set(11_day_harib08b_ipl10_WINE_OUTS ${root_BINARY_DIR}/projects/11_day/harib08b/ipl10_wine.bin)
add_custom_target(11_day_harib08b_ipl10_wine
  COMMAND ${WINE} ${WINE_NASK} ${11_day_harib08b_ipl10_SRCS} ${11_day_harib08b_ipl10_WINE_OUTS}
)
add_dependencies(wine 11_day_harib08b_ipl10_wine)

add_custom_target(11_day_harib08b_ipl10_od
  COMMAND ${OD} -t x1 ${11_day_harib08b_ipl10_OUTS}      > ${11_day_harib08b_ipl10_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${11_day_harib08b_ipl10_WINE_OUTS} > ${11_day_harib08b_ipl10_WINE_OUTS}_t.txt
  COMMAND diff -s ${11_day_harib08b_ipl10_OUTS}_f.txt ${11_day_harib08b_ipl10_WINE_OUTS}_t.txt
)

add_dependencies(11_day_harib08b_ipl10_od 11_day_harib08b_ipl10)
add_dependencies(11_day_harib08b_ipl10_od 11_day_harib08b_ipl10_wine)
add_dependencies(od 11_day_harib08b_ipl10_od)

#----------------------------------------------------------
########### next target ###############
set(11_day_harib08b_naskfunc_SRCS ${root_SOURCE_DIR}/projects/11_day/harib08b/naskfunc.nas)
set(11_day_harib08b_naskfunc_OUTS ${root_BINARY_DIR}/projects/11_day/harib08b/naskfunc.img)

add_custom_target(11_day_harib08b_naskfunc
  COMMAND ${GOSK_EXECUTABLE} ${11_day_harib08b_naskfunc_SRCS} ${11_day_harib08b_naskfunc_OUTS}
  DEPENDS ${11_day_harib08b_naskfunc_SRCS}
)

add_dependencies(images 11_day_harib08b_naskfunc)
set(11_day_harib08b_naskfunc_WINE_OUTS ${root_BINARY_DIR}/projects/11_day/harib08b/naskfunc_wine.img)
add_custom_target(11_day_harib08b_naskfunc_wine
  COMMAND ${WINE} ${WINE_NASK} ${11_day_harib08b_naskfunc_SRCS} ${11_day_harib08b_naskfunc_WINE_OUTS}
)
add_dependencies(wine 11_day_harib08b_naskfunc_wine)

add_custom_target(11_day_harib08b_naskfunc_od
  COMMAND ${OD} -t x1 ${11_day_harib08b_naskfunc_OUTS}      > ${11_day_harib08b_naskfunc_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${11_day_harib08b_naskfunc_WINE_OUTS} > ${11_day_harib08b_naskfunc_WINE_OUTS}_t.txt
  COMMAND diff -s ${11_day_harib08b_naskfunc_OUTS}_f.txt ${11_day_harib08b_naskfunc_WINE_OUTS}_t.txt
)

add_dependencies(11_day_harib08b_naskfunc_od 11_day_harib08b_naskfunc)
add_dependencies(11_day_harib08b_naskfunc_od 11_day_harib08b_naskfunc_wine)
add_dependencies(od 11_day_harib08b_naskfunc_od)

#----------------------------------------------------------
