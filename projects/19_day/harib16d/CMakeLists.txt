#----------------------------------------------------------
message(STATUS "Entering directory projects/19_day/harib16d/")

set(FONT ${root_BINARY_DIR}/src/makefont)
set(B2O  ${root_BINARY_DIR}/src/bin2obj)
set(CONV ${root_BINARY_DIR}/objconv/objconv)
set(19_day_harib16d_WILDOBJ ${root_BINARY_DIR}/projects/19_day/harib16d/*.o)

# Full OS build rules
set(19_day_harib16d_OS    ${root_BINARY_DIR}/projects/19_day/harib16d/os.img)
set(19_day_harib16d_SYS   ${root_BINARY_DIR}/projects/19_day/harib16d/os.sys)
set(19_day_harib16d_IPLB  ${root_BINARY_DIR}/projects/19_day/harib16d/ipl.bin)
set(19_day_harib16d_IPLS  ${root_SOURCE_DIR}/projects/19_day/harib16d/ipl10.nas)
set(19_day_harib16d_HEADB ${root_BINARY_DIR}/projects/19_day/harib16d/asmhead.bin)
set(19_day_harib16d_HEADS ${root_SOURCE_DIR}/projects/19_day/harib16d/asmhead.nas)
set(19_day_harib16d_BOOTB ${root_BINARY_DIR}/projects/19_day/harib16d/boot.bin)
set(19_day_harib16d_CCO   ${root_BINARY_DIR}/projects/19_day/harib16d/boot.o)
set(19_day_harib16d_CCS   ${root_SOURCE_DIR}/projects/19_day/harib16d/*.c)
set(19_day_harib16d_LDS   ${root_SOURCE_DIR}/projects/os.lds)
set(19_day_harib16d_FUNCS ${root_SOURCE_DIR}/projects/19_day/harib16d/naskfunc.nas)
set(19_day_harib16d_FUNCO ${root_BINARY_DIR}/projects/19_day/harib16d/naskfunc.o)
set(19_day_harib16d_FONTS ${root_SOURCE_DIR}/projects/19_day/harib16d/hankaku.txt)
set(19_day_harib16d_FONTB ${root_BINARY_DIR}/projects/19_day/harib16d/hankaku.bin)
set(19_day_harib16d_FONTO ${root_BINARY_DIR}/projects/19_day/harib16d/hankaku.o)
set(19_day_harib16d_LIBGE ${root_BINARY_DIR}/golibc/libgo.a)
set(19_day_harib16d_LIBGC ${root_BINARY_DIR}/projects/19_day/harib16d/libgo.o)

add_custom_target(19_day_harib16d_run
  COMMAND ${QEMU} ${QEMUOPT} ${19_day_harib16d_OS}
  DEPENDS 19_day_harib16d_img
)
add_custom_target(19_day_harib16d_debug
  COMMAND ${QEMU} -s -S ${QEMUOPT} ${19_day_harib16d_OS} ${QEMU_DEBUG_OPT}
  DEPENDS 19_day_harib16d_img
)
add_custom_target(19_day_harib16d_clean
  COMMAND rm -f ${19_day_harib16d_OS}
  COMMAND rm -f ${19_day_harib16d_SYS}
  COMMAND rm -f ${19_day_harib16d_IPLB}
  COMMAND rm -f ${19_day_harib16d_HEADB}
  COMMAND rm -f ${19_day_harib16d_BOOTB}
  COMMAND rm -f ${19_day_harib16d_WILDOBJ}
  COMMAND rm -f ${root_BINARY_DIR}/projects/19_day/harib16d/ipl10.bin
  COMMAND rm -f ${root_BINARY_DIR}/projects/19_day/harib16d/asmhead.bin
  COMMAND rm -f ${root_BINARY_DIR}/projects/19_day/harib16d/*_wine.bin
  COMMAND rm -f ${root_BINARY_DIR}/projects/19_day/harib16d/*.txt
)
add_custom_target(19_day_harib16d_ipl_build
  COMMAND ${GOSK_EXECUTABLE} ${19_day_harib16d_IPLS} ${19_day_harib16d_IPLB}
  DEPENDS ${19_day_harib16d_IPLS}
)
add_custom_command(
  OUTPUT ${19_day_harib16d_HEADB}
  COMMAND ${GOSK_EXECUTABLE} ${19_day_harib16d_HEADS} ${19_day_harib16d_HEADB}
  DEPENDS ${19_day_harib16d_HEADS}
  COMMENT "Generating asmhead.bin for 19_day_harib16d"
)

add_custom_target(19_day_harib16d_sys
  COMMAND ${FONT} ${19_day_harib16d_FONTS} ${19_day_harib16d_FONTB}
  COMMAND ${B2O}  ${19_day_harib16d_FONTB} ${19_day_harib16d_FONTO} _hankaku
  COMMAND ${CONV} -fcoff32 -nu ${19_day_harib16d_LIBGE} ${19_day_harib16d_LIBGC}
  COMMAND ${GOSK_EXECUTABLE} ${19_day_harib16d_FUNCS} ${19_day_harib16d_FUNCO}
  COMMAND gcc ${BINOPT} -T ${19_day_harib16d_LDS} ${19_day_harib16d_CCS} ${19_day_harib16d_WILDOBJ} -o ${19_day_harib16d_BOOTB}
  COMMAND cat ${19_day_harib16d_HEADB} ${19_day_harib16d_BOOTB} > ${19_day_harib16d_SYS}
  DEPENDS ${19_day_harib16d_HEADB} 19_day_harib16d_ipl_build
  DEPENDS ${19_day_harib16d_FUNCO}
  DEPENDS ${19_day_harib16d_FONTO}
)
add_custom_target(19_day_harib16d_img
  COMMAND mformat -f 1440 -l HARIBOTEOS -N 0xffffffff -C -B ${19_day_harib16d_IPLB} -i ${19_day_harib16d_OS}
  COMMAND mcopy -i ${19_day_harib16d_OS} ${19_day_harib16d_SYS} ::
  DEPENDS 19_day_harib16d_sys
)

########### next target ###############
set(19_day_harib16d_asmhead_SRCS ${root_SOURCE_DIR}/projects/19_day/harib16d/asmhead.nas)
set(19_day_harib16d_asmhead_OUTS ${root_BINARY_DIR}/projects/19_day/harib16d/asmhead.bin)

add_custom_target(19_day_harib16d_asmhead
  COMMAND ${GOSK_EXECUTABLE} ${19_day_harib16d_asmhead_SRCS} ${19_day_harib16d_asmhead_OUTS}
  DEPENDS ${19_day_harib16d_asmhead_SRCS}
)

set(19_day_harib16d_asmhead_WINE_OUTS ${root_BINARY_DIR}/projects/19_day/harib16d/asmhead_wine.bin)
add_custom_target(19_day_harib16d_asmhead_wine
  COMMAND ${WINE} ${WINE_NASK} ${19_day_harib16d_asmhead_SRCS} ${19_day_harib16d_asmhead_WINE_OUTS}
)
add_dependencies(wine 19_day_harib16d_asmhead_wine)

add_custom_target(19_day_harib16d_asmhead_od
  COMMAND ${OD} -t x1 ${19_day_harib16d_asmhead_OUTS}      > ${19_day_harib16d_asmhead_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${19_day_harib16d_asmhead_WINE_OUTS} > ${19_day_harib16d_asmhead_WINE_OUTS}_t.txt
  COMMAND diff -s ${19_day_harib16d_asmhead_OUTS}_f.txt ${19_day_harib16d_asmhead_WINE_OUTS}_t.txt
)

add_dependencies(19_day_harib16d_asmhead_od 19_day_harib16d_asmhead)
add_dependencies(19_day_harib16d_asmhead_od 19_day_harib16d_asmhead_wine)
add_dependencies(od 19_day_harib16d_asmhead_od)

#----------------------------------------------------------
########### next target ###############
set(19_day_harib16d_ipl10_SRCS ${root_SOURCE_DIR}/projects/19_day/harib16d/ipl10.nas)
set(19_day_harib16d_ipl10_OUTS ${root_BINARY_DIR}/projects/19_day/harib16d/ipl10.bin)

add_custom_target(19_day_harib16d_ipl10
  COMMAND ${GOSK_EXECUTABLE} ${19_day_harib16d_ipl10_SRCS} ${19_day_harib16d_ipl10_OUTS}
  DEPENDS ${19_day_harib16d_ipl10_SRCS}
)

set(19_day_harib16d_ipl10_WINE_OUTS ${root_BINARY_DIR}/projects/19_day/harib16d/ipl10_wine.bin)
add_custom_target(19_day_harib16d_ipl10_wine
  COMMAND ${WINE} ${WINE_NASK} ${19_day_harib16d_ipl10_SRCS} ${19_day_harib16d_ipl10_WINE_OUTS}
)
add_dependencies(wine 19_day_harib16d_ipl10_wine)

add_custom_target(19_day_harib16d_ipl10_od
  COMMAND ${OD} -t x1 ${19_day_harib16d_ipl10_OUTS}      > ${19_day_harib16d_ipl10_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${19_day_harib16d_ipl10_WINE_OUTS} > ${19_day_harib16d_ipl10_WINE_OUTS}_t.txt
  COMMAND diff -s ${19_day_harib16d_ipl10_OUTS}_f.txt ${19_day_harib16d_ipl10_WINE_OUTS}_t.txt
)

add_dependencies(19_day_harib16d_ipl10_od 19_day_harib16d_ipl10)
add_dependencies(19_day_harib16d_ipl10_od 19_day_harib16d_ipl10_wine)
add_dependencies(od 19_day_harib16d_ipl10_od)

#----------------------------------------------------------
########### next target ###############
set(19_day_harib16d_naskfunc_SRCS ${root_SOURCE_DIR}/projects/19_day/harib16d/naskfunc.nas)
set(19_day_harib16d_naskfunc_OUTS ${root_BINARY_DIR}/projects/19_day/harib16d/naskfunc.img)

add_custom_target(19_day_harib16d_naskfunc
  COMMAND ${GOSK_EXECUTABLE} ${19_day_harib16d_naskfunc_SRCS} ${19_day_harib16d_naskfunc_OUTS}
  DEPENDS ${19_day_harib16d_naskfunc_SRCS}
)

add_dependencies(images 19_day_harib16d_naskfunc)
set(19_day_harib16d_naskfunc_WINE_OUTS ${root_BINARY_DIR}/projects/19_day/harib16d/naskfunc_wine.img)
add_custom_target(19_day_harib16d_naskfunc_wine
  COMMAND ${WINE} ${WINE_NASK} ${19_day_harib16d_naskfunc_SRCS} ${19_day_harib16d_naskfunc_WINE_OUTS}
)
add_dependencies(wine 19_day_harib16d_naskfunc_wine)

add_custom_target(19_day_harib16d_naskfunc_od
  COMMAND ${OD} -t x1 ${19_day_harib16d_naskfunc_OUTS}      > ${19_day_harib16d_naskfunc_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${19_day_harib16d_naskfunc_WINE_OUTS} > ${19_day_harib16d_naskfunc_WINE_OUTS}_t.txt
  COMMAND diff -s ${19_day_harib16d_naskfunc_OUTS}_f.txt ${19_day_harib16d_naskfunc_WINE_OUTS}_t.txt
)

add_dependencies(19_day_harib16d_naskfunc_od 19_day_harib16d_naskfunc)
add_dependencies(19_day_harib16d_naskfunc_od 19_day_harib16d_naskfunc_wine)
add_dependencies(od 19_day_harib16d_naskfunc_od)

#----------------------------------------------------------
