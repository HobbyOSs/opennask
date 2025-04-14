#----------------------------------------------------------
message(STATUS "Entering directory projects/12_day/harib09d/")

set(FONT ${root_BINARY_DIR}/src/makefont)
set(B2O  ${root_BINARY_DIR}/src/bin2obj)
set(CONV ${root_BINARY_DIR}/objconv/objconv)
set(12_day_harib09d_WILDOBJ ${root_BINARY_DIR}/projects/12_day/harib09d/*.o)

# Full OS build rules
set(12_day_harib09d_OS    ${root_BINARY_DIR}/projects/12_day/harib09d/os.img)
set(12_day_harib09d_SYS   ${root_BINARY_DIR}/projects/12_day/harib09d/os.sys)
set(12_day_harib09d_IPLB  ${root_BINARY_DIR}/projects/12_day/harib09d/ipl.bin)
set(12_day_harib09d_IPLS  ${root_SOURCE_DIR}/projects/12_day/harib09d/ipl10.nas)
set(12_day_harib09d_HEADB ${root_BINARY_DIR}/projects/12_day/harib09d/asmhead.bin)
set(12_day_harib09d_HEADS ${root_SOURCE_DIR}/projects/12_day/harib09d/asmhead.nas)
set(12_day_harib09d_BOOTB ${root_BINARY_DIR}/projects/12_day/harib09d/boot.bin)
set(12_day_harib09d_CCO   ${root_BINARY_DIR}/projects/12_day/harib09d/boot.o)
set(12_day_harib09d_CCS   ${root_SOURCE_DIR}/projects/12_day/harib09d/*.c)
set(12_day_harib09d_LDS   ${root_SOURCE_DIR}/projects/os.lds)
set(12_day_harib09d_FUNCS ${root_SOURCE_DIR}/projects/12_day/harib09d/naskfunc.nas)
set(12_day_harib09d_FUNCO ${root_BINARY_DIR}/projects/12_day/harib09d/naskfunc.o)
set(12_day_harib09d_FONTS ${root_SOURCE_DIR}/projects/12_day/harib09d/hankaku.txt)
set(12_day_harib09d_FONTB ${root_BINARY_DIR}/projects/12_day/harib09d/hankaku.bin)
set(12_day_harib09d_FONTO ${root_BINARY_DIR}/projects/12_day/harib09d/hankaku.o)
set(12_day_harib09d_LIBGE ${root_BINARY_DIR}/golibc/libgo.a)
set(12_day_harib09d_LIBGC ${root_BINARY_DIR}/projects/12_day/harib09d/libgo.o)

add_custom_target(12_day_harib09d_run
  COMMAND ${QEMU} ${QEMUOPT} ${12_day_harib09d_OS}
  DEPENDS 12_day_harib09d_img
)
add_custom_target(12_day_harib09d_debug
  COMMAND ${QEMU} -s -S ${QEMUOPT} ${12_day_harib09d_OS} ${QEMU_DEBUG_OPT}
  DEPENDS 12_day_harib09d_img
)
add_custom_target(12_day_harib09d_clean
  COMMAND rm -f ${12_day_harib09d_OS}
  COMMAND rm -f ${12_day_harib09d_SYS}
  COMMAND rm -f ${12_day_harib09d_IPLB}
  COMMAND rm -f ${12_day_harib09d_HEADB}
  COMMAND rm -f ${12_day_harib09d_BOOTB}
  COMMAND rm -f ${12_day_harib09d_WILDOBJ}
  COMMAND rm -f ${root_BINARY_DIR}/projects/12_day/harib09d/ipl10.bin
  COMMAND rm -f ${root_BINARY_DIR}/projects/12_day/harib09d/asmhead.bin
  COMMAND rm -f ${root_BINARY_DIR}/projects/12_day/harib09d/*_wine.bin
  COMMAND rm -f ${root_BINARY_DIR}/projects/12_day/harib09d/*.txt
)
add_custom_target(12_day_harib09d_ipl_build
  COMMAND ${GOSK_EXECUTABLE} ${12_day_harib09d_IPLS} ${12_day_harib09d_IPLB}
  DEPENDS ${12_day_harib09d_IPLS}
)
add_custom_command(
  OUTPUT ${12_day_harib09d_HEADB}
  COMMAND ${GOSK_EXECUTABLE} ${12_day_harib09d_HEADS} ${12_day_harib09d_HEADB}
  DEPENDS ${12_day_harib09d_HEADS}
  COMMENT "Generating asmhead.bin for 12_day_harib09d"
)

add_custom_target(12_day_harib09d_sys
  COMMAND ${FONT} ${12_day_harib09d_FONTS} ${12_day_harib09d_FONTB}
  COMMAND ${B2O}  ${12_day_harib09d_FONTB} ${12_day_harib09d_FONTO} _hankaku
  COMMAND ${CONV} -fcoff32 -nu ${12_day_harib09d_LIBGE} ${12_day_harib09d_LIBGC}
  COMMAND ${GOSK_EXECUTABLE} ${12_day_harib09d_FUNCS} ${12_day_harib09d_FUNCO}
  COMMAND gcc ${BINOPT} -T ${12_day_harib09d_LDS} ${12_day_harib09d_CCS} ${12_day_harib09d_WILDOBJ} -o ${12_day_harib09d_BOOTB}
  COMMAND cat ${12_day_harib09d_HEADB} ${12_day_harib09d_BOOTB} > ${12_day_harib09d_SYS}
  DEPENDS ${12_day_harib09d_HEADB} 12_day_harib09d_ipl_build
  DEPENDS ${12_day_harib09d_FUNCO}
  DEPENDS ${12_day_harib09d_FONTO}
)
add_custom_target(12_day_harib09d_img
  COMMAND mformat -f 1440 -l HARIBOTEOS -N 0xffffffff -C -B ${12_day_harib09d_IPLB} -i ${12_day_harib09d_OS}
  COMMAND mcopy -i ${12_day_harib09d_OS} ${12_day_harib09d_SYS} ::
  DEPENDS 12_day_harib09d_sys
)

########### next target ###############
set(12_day_harib09d_asmhead_SRCS ${root_SOURCE_DIR}/projects/12_day/harib09d/asmhead.nas)
set(12_day_harib09d_asmhead_OUTS ${root_BINARY_DIR}/projects/12_day/harib09d/asmhead.bin)

add_custom_target(12_day_harib09d_asmhead
  COMMAND ${GOSK_EXECUTABLE} ${12_day_harib09d_asmhead_SRCS} ${12_day_harib09d_asmhead_OUTS}
  DEPENDS ${12_day_harib09d_asmhead_SRCS}
)

set(12_day_harib09d_asmhead_WINE_OUTS ${root_BINARY_DIR}/projects/12_day/harib09d/asmhead_wine.bin)
add_custom_target(12_day_harib09d_asmhead_wine
  COMMAND ${WINE} ${WINE_NASK} ${12_day_harib09d_asmhead_SRCS} ${12_day_harib09d_asmhead_WINE_OUTS}
)
add_dependencies(wine 12_day_harib09d_asmhead_wine)

add_custom_target(12_day_harib09d_asmhead_od
  COMMAND ${OD} -t x1 ${12_day_harib09d_asmhead_OUTS}      > ${12_day_harib09d_asmhead_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${12_day_harib09d_asmhead_WINE_OUTS} > ${12_day_harib09d_asmhead_WINE_OUTS}_t.txt
  COMMAND diff -s ${12_day_harib09d_asmhead_OUTS}_f.txt ${12_day_harib09d_asmhead_WINE_OUTS}_t.txt
)

add_dependencies(12_day_harib09d_asmhead_od 12_day_harib09d_asmhead)
add_dependencies(12_day_harib09d_asmhead_od 12_day_harib09d_asmhead_wine)
add_dependencies(od 12_day_harib09d_asmhead_od)

#----------------------------------------------------------
########### next target ###############
set(12_day_harib09d_ipl10_SRCS ${root_SOURCE_DIR}/projects/12_day/harib09d/ipl10.nas)
set(12_day_harib09d_ipl10_OUTS ${root_BINARY_DIR}/projects/12_day/harib09d/ipl10.bin)

add_custom_target(12_day_harib09d_ipl10
  COMMAND ${GOSK_EXECUTABLE} ${12_day_harib09d_ipl10_SRCS} ${12_day_harib09d_ipl10_OUTS}
  DEPENDS ${12_day_harib09d_ipl10_SRCS}
)

set(12_day_harib09d_ipl10_WINE_OUTS ${root_BINARY_DIR}/projects/12_day/harib09d/ipl10_wine.bin)
add_custom_target(12_day_harib09d_ipl10_wine
  COMMAND ${WINE} ${WINE_NASK} ${12_day_harib09d_ipl10_SRCS} ${12_day_harib09d_ipl10_WINE_OUTS}
)
add_dependencies(wine 12_day_harib09d_ipl10_wine)

add_custom_target(12_day_harib09d_ipl10_od
  COMMAND ${OD} -t x1 ${12_day_harib09d_ipl10_OUTS}      > ${12_day_harib09d_ipl10_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${12_day_harib09d_ipl10_WINE_OUTS} > ${12_day_harib09d_ipl10_WINE_OUTS}_t.txt
  COMMAND diff -s ${12_day_harib09d_ipl10_OUTS}_f.txt ${12_day_harib09d_ipl10_WINE_OUTS}_t.txt
)

add_dependencies(12_day_harib09d_ipl10_od 12_day_harib09d_ipl10)
add_dependencies(12_day_harib09d_ipl10_od 12_day_harib09d_ipl10_wine)
add_dependencies(od 12_day_harib09d_ipl10_od)

#----------------------------------------------------------
########### next target ###############
set(12_day_harib09d_naskfunc_SRCS ${root_SOURCE_DIR}/projects/12_day/harib09d/naskfunc.nas)
set(12_day_harib09d_naskfunc_OUTS ${root_BINARY_DIR}/projects/12_day/harib09d/naskfunc.img)

add_custom_target(12_day_harib09d_naskfunc
  COMMAND ${GOSK_EXECUTABLE} ${12_day_harib09d_naskfunc_SRCS} ${12_day_harib09d_naskfunc_OUTS}
  DEPENDS ${12_day_harib09d_naskfunc_SRCS}
)

add_dependencies(images 12_day_harib09d_naskfunc)
set(12_day_harib09d_naskfunc_WINE_OUTS ${root_BINARY_DIR}/projects/12_day/harib09d/naskfunc_wine.img)
add_custom_target(12_day_harib09d_naskfunc_wine
  COMMAND ${WINE} ${WINE_NASK} ${12_day_harib09d_naskfunc_SRCS} ${12_day_harib09d_naskfunc_WINE_OUTS}
)
add_dependencies(wine 12_day_harib09d_naskfunc_wine)

add_custom_target(12_day_harib09d_naskfunc_od
  COMMAND ${OD} -t x1 ${12_day_harib09d_naskfunc_OUTS}      > ${12_day_harib09d_naskfunc_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${12_day_harib09d_naskfunc_WINE_OUTS} > ${12_day_harib09d_naskfunc_WINE_OUTS}_t.txt
  COMMAND diff -s ${12_day_harib09d_naskfunc_OUTS}_f.txt ${12_day_harib09d_naskfunc_WINE_OUTS}_t.txt
)

add_dependencies(12_day_harib09d_naskfunc_od 12_day_harib09d_naskfunc)
add_dependencies(12_day_harib09d_naskfunc_od 12_day_harib09d_naskfunc_wine)
add_dependencies(od 12_day_harib09d_naskfunc_od)

#----------------------------------------------------------
