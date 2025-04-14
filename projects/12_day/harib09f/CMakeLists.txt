#----------------------------------------------------------
message(STATUS "Entering directory projects/12_day/harib09f/")

set(FONT ${root_BINARY_DIR}/src/makefont)
set(B2O  ${root_BINARY_DIR}/src/bin2obj)
set(CONV ${root_BINARY_DIR}/objconv/objconv)
set(12_day_harib09f_WILDOBJ ${root_BINARY_DIR}/projects/12_day/harib09f/*.o)

# Full OS build rules
set(12_day_harib09f_OS    ${root_BINARY_DIR}/projects/12_day/harib09f/os.img)
set(12_day_harib09f_SYS   ${root_BINARY_DIR}/projects/12_day/harib09f/os.sys)
set(12_day_harib09f_IPLB  ${root_BINARY_DIR}/projects/12_day/harib09f/ipl.bin)
set(12_day_harib09f_IPLS  ${root_SOURCE_DIR}/projects/12_day/harib09f/ipl10.nas)
set(12_day_harib09f_HEADB ${root_BINARY_DIR}/projects/12_day/harib09f/asmhead.bin)
set(12_day_harib09f_HEADS ${root_SOURCE_DIR}/projects/12_day/harib09f/asmhead.nas)
set(12_day_harib09f_BOOTB ${root_BINARY_DIR}/projects/12_day/harib09f/boot.bin)
set(12_day_harib09f_CCO   ${root_BINARY_DIR}/projects/12_day/harib09f/boot.o)
set(12_day_harib09f_CCS   ${root_SOURCE_DIR}/projects/12_day/harib09f/*.c)
set(12_day_harib09f_LDS   ${root_SOURCE_DIR}/projects/os.lds)
set(12_day_harib09f_FUNCS ${root_SOURCE_DIR}/projects/12_day/harib09f/naskfunc.nas)
set(12_day_harib09f_FUNCO ${root_BINARY_DIR}/projects/12_day/harib09f/naskfunc.o)
set(12_day_harib09f_FONTS ${root_SOURCE_DIR}/projects/12_day/harib09f/hankaku.txt)
set(12_day_harib09f_FONTB ${root_BINARY_DIR}/projects/12_day/harib09f/hankaku.bin)
set(12_day_harib09f_FONTO ${root_BINARY_DIR}/projects/12_day/harib09f/hankaku.o)
set(12_day_harib09f_LIBGE ${root_BINARY_DIR}/golibc/libgo.a)
set(12_day_harib09f_LIBGC ${root_BINARY_DIR}/projects/12_day/harib09f/libgo.o)

add_custom_target(12_day_harib09f_run
  COMMAND ${QEMU} ${QEMUOPT} ${12_day_harib09f_OS}
  DEPENDS 12_day_harib09f_img
)
add_custom_target(12_day_harib09f_debug
  COMMAND ${QEMU} -s -S ${QEMUOPT} ${12_day_harib09f_OS} ${QEMU_DEBUG_OPT}
  DEPENDS 12_day_harib09f_img
)
add_custom_target(12_day_harib09f_clean
  COMMAND rm -f ${12_day_harib09f_OS}
  COMMAND rm -f ${12_day_harib09f_SYS}
  COMMAND rm -f ${12_day_harib09f_IPLB}
  COMMAND rm -f ${12_day_harib09f_HEADB}
  COMMAND rm -f ${12_day_harib09f_BOOTB}
  COMMAND rm -f ${12_day_harib09f_WILDOBJ}
  COMMAND rm -f ${root_BINARY_DIR}/projects/12_day/harib09f/ipl10.bin
  COMMAND rm -f ${root_BINARY_DIR}/projects/12_day/harib09f/asmhead.bin
  COMMAND rm -f ${root_BINARY_DIR}/projects/12_day/harib09f/*_wine.bin
  COMMAND rm -f ${root_BINARY_DIR}/projects/12_day/harib09f/*.txt
)
add_custom_target(12_day_harib09f_ipl_build
  COMMAND ${GOSK_EXECUTABLE} ${12_day_harib09f_IPLS} ${12_day_harib09f_IPLB}
  DEPENDS ${12_day_harib09f_IPLS}
)
add_custom_command(
  OUTPUT ${12_day_harib09f_HEADB}
  COMMAND ${GOSK_EXECUTABLE} ${12_day_harib09f_HEADS} ${12_day_harib09f_HEADB}
  DEPENDS ${12_day_harib09f_HEADS}
  COMMENT "Generating asmhead.bin for 12_day_harib09f"
)

add_custom_target(12_day_harib09f_sys
  COMMAND ${FONT} ${12_day_harib09f_FONTS} ${12_day_harib09f_FONTB}
  COMMAND ${B2O}  ${12_day_harib09f_FONTB} ${12_day_harib09f_FONTO} _hankaku
  COMMAND ${CONV} -fcoff32 -nu ${12_day_harib09f_LIBGE} ${12_day_harib09f_LIBGC}
  COMMAND ${GOSK_EXECUTABLE} ${12_day_harib09f_FUNCS} ${12_day_harib09f_FUNCO}
  COMMAND gcc ${BINOPT} -T ${12_day_harib09f_LDS} ${12_day_harib09f_CCS} ${12_day_harib09f_WILDOBJ} -o ${12_day_harib09f_BOOTB}
  COMMAND cat ${12_day_harib09f_HEADB} ${12_day_harib09f_BOOTB} > ${12_day_harib09f_SYS}
  DEPENDS ${12_day_harib09f_HEADB} 12_day_harib09f_ipl_build
  DEPENDS ${12_day_harib09f_FUNCO}
  DEPENDS ${12_day_harib09f_FONTO}
)
add_custom_target(12_day_harib09f_img
  COMMAND mformat -f 1440 -l HARIBOTEOS -N 0xffffffff -C -B ${12_day_harib09f_IPLB} -i ${12_day_harib09f_OS}
  COMMAND mcopy -i ${12_day_harib09f_OS} ${12_day_harib09f_SYS} ::
  DEPENDS 12_day_harib09f_sys
)

########### next target ###############
set(12_day_harib09f_asmhead_SRCS ${root_SOURCE_DIR}/projects/12_day/harib09f/asmhead.nas)
set(12_day_harib09f_asmhead_OUTS ${root_BINARY_DIR}/projects/12_day/harib09f/asmhead.bin)

add_custom_target(12_day_harib09f_asmhead
  COMMAND ${GOSK_EXECUTABLE} ${12_day_harib09f_asmhead_SRCS} ${12_day_harib09f_asmhead_OUTS}
  DEPENDS ${12_day_harib09f_asmhead_SRCS}
)

set(12_day_harib09f_asmhead_WINE_OUTS ${root_BINARY_DIR}/projects/12_day/harib09f/asmhead_wine.bin)
add_custom_target(12_day_harib09f_asmhead_wine
  COMMAND ${WINE} ${WINE_NASK} ${12_day_harib09f_asmhead_SRCS} ${12_day_harib09f_asmhead_WINE_OUTS}
)
add_dependencies(wine 12_day_harib09f_asmhead_wine)

add_custom_target(12_day_harib09f_asmhead_od
  COMMAND ${OD} -t x1 ${12_day_harib09f_asmhead_OUTS}      > ${12_day_harib09f_asmhead_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${12_day_harib09f_asmhead_WINE_OUTS} > ${12_day_harib09f_asmhead_WINE_OUTS}_t.txt
  COMMAND diff -s ${12_day_harib09f_asmhead_OUTS}_f.txt ${12_day_harib09f_asmhead_WINE_OUTS}_t.txt
)

add_dependencies(12_day_harib09f_asmhead_od 12_day_harib09f_asmhead)
add_dependencies(12_day_harib09f_asmhead_od 12_day_harib09f_asmhead_wine)
add_dependencies(od 12_day_harib09f_asmhead_od)

#----------------------------------------------------------
########### next target ###############
set(12_day_harib09f_ipl10_SRCS ${root_SOURCE_DIR}/projects/12_day/harib09f/ipl10.nas)
set(12_day_harib09f_ipl10_OUTS ${root_BINARY_DIR}/projects/12_day/harib09f/ipl10.bin)

add_custom_target(12_day_harib09f_ipl10
  COMMAND ${GOSK_EXECUTABLE} ${12_day_harib09f_ipl10_SRCS} ${12_day_harib09f_ipl10_OUTS}
  DEPENDS ${12_day_harib09f_ipl10_SRCS}
)

set(12_day_harib09f_ipl10_WINE_OUTS ${root_BINARY_DIR}/projects/12_day/harib09f/ipl10_wine.bin)
add_custom_target(12_day_harib09f_ipl10_wine
  COMMAND ${WINE} ${WINE_NASK} ${12_day_harib09f_ipl10_SRCS} ${12_day_harib09f_ipl10_WINE_OUTS}
)
add_dependencies(wine 12_day_harib09f_ipl10_wine)

add_custom_target(12_day_harib09f_ipl10_od
  COMMAND ${OD} -t x1 ${12_day_harib09f_ipl10_OUTS}      > ${12_day_harib09f_ipl10_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${12_day_harib09f_ipl10_WINE_OUTS} > ${12_day_harib09f_ipl10_WINE_OUTS}_t.txt
  COMMAND diff -s ${12_day_harib09f_ipl10_OUTS}_f.txt ${12_day_harib09f_ipl10_WINE_OUTS}_t.txt
)

add_dependencies(12_day_harib09f_ipl10_od 12_day_harib09f_ipl10)
add_dependencies(12_day_harib09f_ipl10_od 12_day_harib09f_ipl10_wine)
add_dependencies(od 12_day_harib09f_ipl10_od)

#----------------------------------------------------------
########### next target ###############
set(12_day_harib09f_naskfunc_SRCS ${root_SOURCE_DIR}/projects/12_day/harib09f/naskfunc.nas)
set(12_day_harib09f_naskfunc_OUTS ${root_BINARY_DIR}/projects/12_day/harib09f/naskfunc.img)

add_custom_target(12_day_harib09f_naskfunc
  COMMAND ${GOSK_EXECUTABLE} ${12_day_harib09f_naskfunc_SRCS} ${12_day_harib09f_naskfunc_OUTS}
  DEPENDS ${12_day_harib09f_naskfunc_SRCS}
)

add_dependencies(images 12_day_harib09f_naskfunc)
set(12_day_harib09f_naskfunc_WINE_OUTS ${root_BINARY_DIR}/projects/12_day/harib09f/naskfunc_wine.img)
add_custom_target(12_day_harib09f_naskfunc_wine
  COMMAND ${WINE} ${WINE_NASK} ${12_day_harib09f_naskfunc_SRCS} ${12_day_harib09f_naskfunc_WINE_OUTS}
)
add_dependencies(wine 12_day_harib09f_naskfunc_wine)

add_custom_target(12_day_harib09f_naskfunc_od
  COMMAND ${OD} -t x1 ${12_day_harib09f_naskfunc_OUTS}      > ${12_day_harib09f_naskfunc_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${12_day_harib09f_naskfunc_WINE_OUTS} > ${12_day_harib09f_naskfunc_WINE_OUTS}_t.txt
  COMMAND diff -s ${12_day_harib09f_naskfunc_OUTS}_f.txt ${12_day_harib09f_naskfunc_WINE_OUTS}_t.txt
)

add_dependencies(12_day_harib09f_naskfunc_od 12_day_harib09f_naskfunc)
add_dependencies(12_day_harib09f_naskfunc_od 12_day_harib09f_naskfunc_wine)
add_dependencies(od 12_day_harib09f_naskfunc_od)

#----------------------------------------------------------
