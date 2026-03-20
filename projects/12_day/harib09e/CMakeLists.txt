#----------------------------------------------------------
message(STATUS "Entering directory projects/12_day/harib09e/")

set(FONT ${root_BINARY_DIR}/src/makefont)
set(B2O  ${root_BINARY_DIR}/src/bin2obj)
set(CONV ${root_BINARY_DIR}/objconv/objconv)
set(12_day_harib09e_WILDOBJ ${root_BINARY_DIR}/projects/12_day/harib09e/*.o)

# Full OS build rules
set(12_day_harib09e_OS    ${root_BINARY_DIR}/projects/12_day/harib09e/os.img)
set(12_day_harib09e_SYS   ${root_BINARY_DIR}/projects/12_day/harib09e/os.sys)
set(12_day_harib09e_IPLB  ${root_BINARY_DIR}/projects/12_day/harib09e/ipl.bin)
set(12_day_harib09e_IPLS  ${root_SOURCE_DIR}/projects/12_day/harib09e/ipl10.nas)
set(12_day_harib09e_HEADB ${root_BINARY_DIR}/projects/12_day/harib09e/asmhead.bin)
set(12_day_harib09e_HEADS ${root_SOURCE_DIR}/projects/12_day/harib09e/asmhead.nas)
set(12_day_harib09e_BOOTB ${root_BINARY_DIR}/projects/12_day/harib09e/boot.bin)
set(12_day_harib09e_CCO   ${root_BINARY_DIR}/projects/12_day/harib09e/boot.o)
set(12_day_harib09e_CCS   ${root_SOURCE_DIR}/projects/12_day/harib09e/*.c)
set(12_day_harib09e_LDS   ${root_SOURCE_DIR}/projects/os.lds)
set(12_day_harib09e_FUNCS ${root_SOURCE_DIR}/projects/12_day/harib09e/naskfunc.nas)
set(12_day_harib09e_FUNCO ${root_BINARY_DIR}/projects/12_day/harib09e/naskfunc.o)
set(12_day_harib09e_FONTS ${root_SOURCE_DIR}/projects/12_day/harib09e/hankaku.txt)
set(12_day_harib09e_FONTB ${root_BINARY_DIR}/projects/12_day/harib09e/hankaku.bin)
set(12_day_harib09e_FONTO ${root_BINARY_DIR}/projects/12_day/harib09e/hankaku.o)
set(12_day_harib09e_LIBGE ${root_BINARY_DIR}/golibc/libgo.a)
set(12_day_harib09e_LIBGC ${root_BINARY_DIR}/projects/12_day/harib09e/libgo.o)

add_custom_target(12_day_harib09e_run
  COMMAND ${QEMU} ${QEMUOPT} ${12_day_harib09e_OS}
  DEPENDS 12_day_harib09e_img
)
add_custom_target(12_day_harib09e_debug
  COMMAND ${QEMU} -s -S ${QEMUOPT} ${12_day_harib09e_OS} ${QEMU_DEBUG_OPT}
  DEPENDS 12_day_harib09e_img
)
add_custom_target(12_day_harib09e_clean
  COMMAND rm -f ${12_day_harib09e_OS}
  COMMAND rm -f ${12_day_harib09e_SYS}
  COMMAND rm -f ${12_day_harib09e_IPLB}
  COMMAND rm -f ${12_day_harib09e_HEADB}
  COMMAND rm -f ${12_day_harib09e_BOOTB}
  COMMAND rm -f ${12_day_harib09e_WILDOBJ}
  COMMAND rm -f ${root_BINARY_DIR}/projects/12_day/harib09e/ipl10.bin
  COMMAND rm -f ${root_BINARY_DIR}/projects/12_day/harib09e/asmhead.bin
  COMMAND rm -f ${root_BINARY_DIR}/projects/12_day/harib09e/*_wine.bin
  COMMAND rm -f ${root_BINARY_DIR}/projects/12_day/harib09e/*.txt
)
add_custom_target(12_day_harib09e_ipl_build
  COMMAND ${RAKUSK_EXECUTABLE} ${12_day_harib09e_IPLS} ${12_day_harib09e_IPLB}
  DEPENDS ${12_day_harib09e_IPLS}
)
add_custom_command(
  OUTPUT ${12_day_harib09e_HEADB}
  COMMAND ${RAKUSK_EXECUTABLE} ${12_day_harib09e_HEADS} ${12_day_harib09e_HEADB}
  DEPENDS ${12_day_harib09e_HEADS}
  COMMENT "Generating asmhead.bin for 12_day_harib09e"
)

add_custom_command(
  OUTPUT ${12_day_harib09e_FUNCO}
  COMMAND ${RAKUSK_EXECUTABLE} ${12_day_harib09e_FUNCS} ${12_day_harib09e_FUNCO}
  DEPENDS ${12_day_harib09e_FUNCS}
  COMMENT "Generating naskfunc.o for 12_day_harib09e"
)
add_custom_command(
  OUTPUT ${12_day_harib09e_FONTO}
  COMMAND ${FONT} ${12_day_harib09e_FONTS} ${12_day_harib09e_FONTB}
  COMMAND ${B2O}  ${12_day_harib09e_FONTB} ${12_day_harib09e_FONTO} _hankaku
  DEPENDS ${12_day_harib09e_FONTS}
  COMMENT "Generating hankaku.o for 12_day_harib09e"
)
add_custom_command(
  OUTPUT ${12_day_harib09e_LIBGC}
  COMMAND ${CONV} -fcoff32 -nu ${12_day_harib09e_LIBGE} ${12_day_harib09e_LIBGC}
  DEPENDS ${12_day_harib09e_LIBGE}
  COMMENT "Converting golibc for 12_day_harib09e"
)
add_custom_target(12_day_harib09e_sys
  COMMAND gcc ${BINOPT} -T ${12_day_harib09e_LDS} ${12_day_harib09e_CCS} ${12_day_harib09e_WILDOBJ} -o ${12_day_harib09e_BOOTB}
  COMMAND cat ${12_day_harib09e_HEADB} ${12_day_harib09e_BOOTB} > ${12_day_harib09e_SYS}
  DEPENDS ${12_day_harib09e_HEADB} 12_day_harib09e_ipl_build
  DEPENDS ${12_day_harib09e_FUNCO}
  DEPENDS ${12_day_harib09e_FONTO} ${12_day_harib09e_LIBGC}
)
add_custom_target(12_day_harib09e_img
  COMMAND mformat -f 1440 -l HARIBOTEOS -N 0xffffffff -C -B ${12_day_harib09e_IPLB} -i ${12_day_harib09e_OS}
  COMMAND mcopy -i ${12_day_harib09e_OS} ${12_day_harib09e_SYS} ::
  DEPENDS 12_day_harib09e_sys
)

########### next target ###############
set(12_day_harib09e_asmhead_SRCS ${root_SOURCE_DIR}/projects/12_day/harib09e/asmhead.nas)
set(12_day_harib09e_asmhead_OUTS ${root_BINARY_DIR}/projects/12_day/harib09e/asmhead.bin)

add_custom_target(12_day_harib09e_asmhead
  COMMAND ${RAKUSK_EXECUTABLE} ${12_day_harib09e_asmhead_SRCS} ${12_day_harib09e_asmhead_OUTS}
  DEPENDS ${12_day_harib09e_asmhead_SRCS}
)

set(12_day_harib09e_asmhead_WINE_OUTS ${root_BINARY_DIR}/projects/12_day/harib09e/asmhead_wine.bin)
add_custom_target(12_day_harib09e_asmhead_wine
  COMMAND ${WINE} ${WINE_NASK} ${12_day_harib09e_asmhead_SRCS} ${12_day_harib09e_asmhead_WINE_OUTS}
)
add_dependencies(wine 12_day_harib09e_asmhead_wine)

add_custom_target(12_day_harib09e_asmhead_od
  COMMAND ${OD} -t x1 ${12_day_harib09e_asmhead_OUTS}      > ${12_day_harib09e_asmhead_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${12_day_harib09e_asmhead_WINE_OUTS} > ${12_day_harib09e_asmhead_WINE_OUTS}_t.txt
  COMMAND diff -s ${12_day_harib09e_asmhead_OUTS}_f.txt ${12_day_harib09e_asmhead_WINE_OUTS}_t.txt
)

add_dependencies(12_day_harib09e_asmhead_od 12_day_harib09e_asmhead)
add_dependencies(12_day_harib09e_asmhead_od 12_day_harib09e_asmhead_wine)
add_dependencies(od 12_day_harib09e_asmhead_od)

#----------------------------------------------------------
########### next target ###############
set(12_day_harib09e_ipl10_SRCS ${root_SOURCE_DIR}/projects/12_day/harib09e/ipl10.nas)
set(12_day_harib09e_ipl10_OUTS ${root_BINARY_DIR}/projects/12_day/harib09e/ipl10.bin)

add_custom_target(12_day_harib09e_ipl10
  COMMAND ${RAKUSK_EXECUTABLE} ${12_day_harib09e_ipl10_SRCS} ${12_day_harib09e_ipl10_OUTS}
  DEPENDS ${12_day_harib09e_ipl10_SRCS}
)

set(12_day_harib09e_ipl10_WINE_OUTS ${root_BINARY_DIR}/projects/12_day/harib09e/ipl10_wine.bin)
add_custom_target(12_day_harib09e_ipl10_wine
  COMMAND ${WINE} ${WINE_NASK} ${12_day_harib09e_ipl10_SRCS} ${12_day_harib09e_ipl10_WINE_OUTS}
)
add_dependencies(wine 12_day_harib09e_ipl10_wine)

add_custom_target(12_day_harib09e_ipl10_od
  COMMAND ${OD} -t x1 ${12_day_harib09e_ipl10_OUTS}      > ${12_day_harib09e_ipl10_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${12_day_harib09e_ipl10_WINE_OUTS} > ${12_day_harib09e_ipl10_WINE_OUTS}_t.txt
  COMMAND diff -s ${12_day_harib09e_ipl10_OUTS}_f.txt ${12_day_harib09e_ipl10_WINE_OUTS}_t.txt
)

add_dependencies(12_day_harib09e_ipl10_od 12_day_harib09e_ipl10)
add_dependencies(12_day_harib09e_ipl10_od 12_day_harib09e_ipl10_wine)
add_dependencies(od 12_day_harib09e_ipl10_od)

#----------------------------------------------------------
########### next target ###############
set(12_day_harib09e_naskfunc_SRCS ${root_SOURCE_DIR}/projects/12_day/harib09e/naskfunc.nas)
set(12_day_harib09e_naskfunc_OUTS ${root_BINARY_DIR}/projects/12_day/harib09e/naskfunc.img)

add_custom_target(12_day_harib09e_naskfunc
  COMMAND ${RAKUSK_EXECUTABLE} ${12_day_harib09e_naskfunc_SRCS} ${12_day_harib09e_naskfunc_OUTS}
  DEPENDS ${12_day_harib09e_naskfunc_SRCS}
)

add_dependencies(images 12_day_harib09e_naskfunc)
set(12_day_harib09e_naskfunc_WINE_OUTS ${root_BINARY_DIR}/projects/12_day/harib09e/naskfunc_wine.img)
add_custom_target(12_day_harib09e_naskfunc_wine
  COMMAND ${WINE} ${WINE_NASK} ${12_day_harib09e_naskfunc_SRCS} ${12_day_harib09e_naskfunc_WINE_OUTS}
)
add_dependencies(wine 12_day_harib09e_naskfunc_wine)

add_custom_target(12_day_harib09e_naskfunc_od
  COMMAND ${OD} -t x1 ${12_day_harib09e_naskfunc_OUTS}      > ${12_day_harib09e_naskfunc_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${12_day_harib09e_naskfunc_WINE_OUTS} > ${12_day_harib09e_naskfunc_WINE_OUTS}_t.txt
  COMMAND diff -s ${12_day_harib09e_naskfunc_OUTS}_f.txt ${12_day_harib09e_naskfunc_WINE_OUTS}_t.txt
)

add_dependencies(12_day_harib09e_naskfunc_od 12_day_harib09e_naskfunc)
add_dependencies(12_day_harib09e_naskfunc_od 12_day_harib09e_naskfunc_wine)
add_dependencies(od 12_day_harib09e_naskfunc_od)

#----------------------------------------------------------
