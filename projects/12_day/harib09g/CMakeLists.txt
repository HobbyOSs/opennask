#----------------------------------------------------------
message(STATUS "Entering directory projects/12_day/harib09g/")

set(FONT ${root_BINARY_DIR}/src/makefont)
set(B2O  ${root_BINARY_DIR}/src/bin2obj)
set(CONV ${root_BINARY_DIR}/objconv/objconv)
set(12_day_harib09g_WILDOBJ ${root_BINARY_DIR}/projects/12_day/harib09g/*.o)

# Full OS build rules
set(12_day_harib09g_OS    ${root_BINARY_DIR}/projects/12_day/harib09g/os.img)
set(12_day_harib09g_SYS   ${root_BINARY_DIR}/projects/12_day/harib09g/os.sys)
set(12_day_harib09g_IPLB  ${root_BINARY_DIR}/projects/12_day/harib09g/ipl.bin)
set(12_day_harib09g_IPLS  ${root_SOURCE_DIR}/projects/12_day/harib09g/ipl10.nas)
set(12_day_harib09g_HEADB ${root_BINARY_DIR}/projects/12_day/harib09g/asmhead.bin)
set(12_day_harib09g_HEADS ${root_SOURCE_DIR}/projects/12_day/harib09g/asmhead.nas)
set(12_day_harib09g_BOOTB ${root_BINARY_DIR}/projects/12_day/harib09g/boot.bin)
set(12_day_harib09g_CCO   ${root_BINARY_DIR}/projects/12_day/harib09g/boot.o)
set(12_day_harib09g_CCS   ${root_SOURCE_DIR}/projects/12_day/harib09g/*.c)
set(12_day_harib09g_LDS   ${root_SOURCE_DIR}/projects/os.lds)
set(12_day_harib09g_FUNCS ${root_SOURCE_DIR}/projects/12_day/harib09g/naskfunc.nas)
set(12_day_harib09g_FUNCO ${root_BINARY_DIR}/projects/12_day/harib09g/naskfunc.o)
set(12_day_harib09g_FONTS ${root_SOURCE_DIR}/projects/12_day/harib09g/hankaku.txt)
set(12_day_harib09g_FONTB ${root_BINARY_DIR}/projects/12_day/harib09g/hankaku.bin)
set(12_day_harib09g_FONTO ${root_BINARY_DIR}/projects/12_day/harib09g/hankaku.o)
set(12_day_harib09g_LIBGE ${root_BINARY_DIR}/golibc/libgo.a)
set(12_day_harib09g_LIBGC ${root_BINARY_DIR}/projects/12_day/harib09g/libgo.o)

add_custom_target(12_day_harib09g_run
  COMMAND ${QEMU} ${QEMUOPT} ${12_day_harib09g_OS}
  DEPENDS 12_day_harib09g_img
)
add_custom_target(12_day_harib09g_debug
  COMMAND ${QEMU} -s -S ${QEMUOPT} ${12_day_harib09g_OS} ${QEMU_DEBUG_OPT}
  DEPENDS 12_day_harib09g_img
)
add_custom_target(12_day_harib09g_clean
  COMMAND rm -f ${12_day_harib09g_OS}
  COMMAND rm -f ${12_day_harib09g_SYS}
  COMMAND rm -f ${12_day_harib09g_IPLB}
  COMMAND rm -f ${12_day_harib09g_HEADB}
  COMMAND rm -f ${12_day_harib09g_BOOTB}
  COMMAND rm -f ${12_day_harib09g_WILDOBJ}
  COMMAND rm -f ${root_BINARY_DIR}/projects/12_day/harib09g/ipl10.bin
  COMMAND rm -f ${root_BINARY_DIR}/projects/12_day/harib09g/asmhead.bin
  COMMAND rm -f ${root_BINARY_DIR}/projects/12_day/harib09g/*_wine.bin
  COMMAND rm -f ${root_BINARY_DIR}/projects/12_day/harib09g/*.txt
)
add_custom_target(12_day_harib09g_ipl_build
  COMMAND ${GOSK_EXECUTABLE} ${12_day_harib09g_IPLS} ${12_day_harib09g_IPLB}
  DEPENDS ${12_day_harib09g_IPLS}
)
add_custom_command(
  OUTPUT ${12_day_harib09g_HEADB}
  COMMAND ${GOSK_EXECUTABLE} ${12_day_harib09g_HEADS} ${12_day_harib09g_HEADB}
  DEPENDS ${12_day_harib09g_HEADS}
  COMMENT "Generating asmhead.bin for 12_day_harib09g"
)

add_custom_target(12_day_harib09g_sys
  COMMAND ${FONT} ${12_day_harib09g_FONTS} ${12_day_harib09g_FONTB}
  COMMAND ${B2O}  ${12_day_harib09g_FONTB} ${12_day_harib09g_FONTO} _hankaku
  COMMAND ${CONV} -fcoff32 -nu ${12_day_harib09g_LIBGE} ${12_day_harib09g_LIBGC}
  COMMAND ${GOSK_EXECUTABLE} ${12_day_harib09g_FUNCS} ${12_day_harib09g_FUNCO}
  COMMAND gcc ${BINOPT} -T ${12_day_harib09g_LDS} ${12_day_harib09g_CCS} ${12_day_harib09g_WILDOBJ} -o ${12_day_harib09g_BOOTB}
  COMMAND cat ${12_day_harib09g_HEADB} ${12_day_harib09g_BOOTB} > ${12_day_harib09g_SYS}
  DEPENDS ${12_day_harib09g_HEADB} 12_day_harib09g_ipl_build
  DEPENDS ${12_day_harib09g_FUNCO}
  DEPENDS ${12_day_harib09g_FONTO}
)
add_custom_target(12_day_harib09g_img
  COMMAND mformat -f 1440 -l HARIBOTEOS -N 0xffffffff -C -B ${12_day_harib09g_IPLB} -i ${12_day_harib09g_OS}
  COMMAND mcopy -i ${12_day_harib09g_OS} ${12_day_harib09g_SYS} ::
  DEPENDS 12_day_harib09g_sys
)

########### next target ###############
set(12_day_harib09g_asmhead_SRCS ${root_SOURCE_DIR}/projects/12_day/harib09g/asmhead.nas)
set(12_day_harib09g_asmhead_OUTS ${root_BINARY_DIR}/projects/12_day/harib09g/asmhead.bin)

add_custom_target(12_day_harib09g_asmhead
  COMMAND ${GOSK_EXECUTABLE} ${12_day_harib09g_asmhead_SRCS} ${12_day_harib09g_asmhead_OUTS}
  DEPENDS ${12_day_harib09g_asmhead_SRCS}
)

set(12_day_harib09g_asmhead_WINE_OUTS ${root_BINARY_DIR}/projects/12_day/harib09g/asmhead_wine.bin)
add_custom_target(12_day_harib09g_asmhead_wine
  COMMAND ${WINE} ${WINE_NASK} ${12_day_harib09g_asmhead_SRCS} ${12_day_harib09g_asmhead_WINE_OUTS}
)
add_dependencies(wine 12_day_harib09g_asmhead_wine)

add_custom_target(12_day_harib09g_asmhead_od
  COMMAND ${OD} -t x1 ${12_day_harib09g_asmhead_OUTS}      > ${12_day_harib09g_asmhead_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${12_day_harib09g_asmhead_WINE_OUTS} > ${12_day_harib09g_asmhead_WINE_OUTS}_t.txt
  COMMAND diff -s ${12_day_harib09g_asmhead_OUTS}_f.txt ${12_day_harib09g_asmhead_WINE_OUTS}_t.txt
)

add_dependencies(12_day_harib09g_asmhead_od 12_day_harib09g_asmhead)
add_dependencies(12_day_harib09g_asmhead_od 12_day_harib09g_asmhead_wine)
add_dependencies(od 12_day_harib09g_asmhead_od)

#----------------------------------------------------------
########### next target ###############
set(12_day_harib09g_ipl10_SRCS ${root_SOURCE_DIR}/projects/12_day/harib09g/ipl10.nas)
set(12_day_harib09g_ipl10_OUTS ${root_BINARY_DIR}/projects/12_day/harib09g/ipl10.bin)

add_custom_target(12_day_harib09g_ipl10
  COMMAND ${GOSK_EXECUTABLE} ${12_day_harib09g_ipl10_SRCS} ${12_day_harib09g_ipl10_OUTS}
  DEPENDS ${12_day_harib09g_ipl10_SRCS}
)

set(12_day_harib09g_ipl10_WINE_OUTS ${root_BINARY_DIR}/projects/12_day/harib09g/ipl10_wine.bin)
add_custom_target(12_day_harib09g_ipl10_wine
  COMMAND ${WINE} ${WINE_NASK} ${12_day_harib09g_ipl10_SRCS} ${12_day_harib09g_ipl10_WINE_OUTS}
)
add_dependencies(wine 12_day_harib09g_ipl10_wine)

add_custom_target(12_day_harib09g_ipl10_od
  COMMAND ${OD} -t x1 ${12_day_harib09g_ipl10_OUTS}      > ${12_day_harib09g_ipl10_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${12_day_harib09g_ipl10_WINE_OUTS} > ${12_day_harib09g_ipl10_WINE_OUTS}_t.txt
  COMMAND diff -s ${12_day_harib09g_ipl10_OUTS}_f.txt ${12_day_harib09g_ipl10_WINE_OUTS}_t.txt
)

add_dependencies(12_day_harib09g_ipl10_od 12_day_harib09g_ipl10)
add_dependencies(12_day_harib09g_ipl10_od 12_day_harib09g_ipl10_wine)
add_dependencies(od 12_day_harib09g_ipl10_od)

#----------------------------------------------------------
########### next target ###############
set(12_day_harib09g_naskfunc_SRCS ${root_SOURCE_DIR}/projects/12_day/harib09g/naskfunc.nas)
set(12_day_harib09g_naskfunc_OUTS ${root_BINARY_DIR}/projects/12_day/harib09g/naskfunc.img)

add_custom_target(12_day_harib09g_naskfunc
  COMMAND ${GOSK_EXECUTABLE} ${12_day_harib09g_naskfunc_SRCS} ${12_day_harib09g_naskfunc_OUTS}
  DEPENDS ${12_day_harib09g_naskfunc_SRCS}
)

add_dependencies(images 12_day_harib09g_naskfunc)
set(12_day_harib09g_naskfunc_WINE_OUTS ${root_BINARY_DIR}/projects/12_day/harib09g/naskfunc_wine.img)
add_custom_target(12_day_harib09g_naskfunc_wine
  COMMAND ${WINE} ${WINE_NASK} ${12_day_harib09g_naskfunc_SRCS} ${12_day_harib09g_naskfunc_WINE_OUTS}
)
add_dependencies(wine 12_day_harib09g_naskfunc_wine)

add_custom_target(12_day_harib09g_naskfunc_od
  COMMAND ${OD} -t x1 ${12_day_harib09g_naskfunc_OUTS}      > ${12_day_harib09g_naskfunc_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${12_day_harib09g_naskfunc_WINE_OUTS} > ${12_day_harib09g_naskfunc_WINE_OUTS}_t.txt
  COMMAND diff -s ${12_day_harib09g_naskfunc_OUTS}_f.txt ${12_day_harib09g_naskfunc_WINE_OUTS}_t.txt
)

add_dependencies(12_day_harib09g_naskfunc_od 12_day_harib09g_naskfunc)
add_dependencies(12_day_harib09g_naskfunc_od 12_day_harib09g_naskfunc_wine)
add_dependencies(od 12_day_harib09g_naskfunc_od)

#----------------------------------------------------------
