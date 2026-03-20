#----------------------------------------------------------
message(STATUS "Entering directory projects/11_day/harib08c/")

set(FONT ${root_BINARY_DIR}/src/makefont)
set(B2O  ${root_BINARY_DIR}/src/bin2obj)
set(CONV ${root_BINARY_DIR}/objconv/objconv)
set(11_day_harib08c_WILDOBJ ${root_BINARY_DIR}/projects/11_day/harib08c/*.o)

# Full OS build rules
set(11_day_harib08c_OS    ${root_BINARY_DIR}/projects/11_day/harib08c/os.img)
set(11_day_harib08c_SYS   ${root_BINARY_DIR}/projects/11_day/harib08c/os.sys)
set(11_day_harib08c_IPLB  ${root_BINARY_DIR}/projects/11_day/harib08c/ipl.bin)
set(11_day_harib08c_IPLS  ${root_SOURCE_DIR}/projects/11_day/harib08c/ipl10.nas)
set(11_day_harib08c_HEADB ${root_BINARY_DIR}/projects/11_day/harib08c/asmhead.bin)
set(11_day_harib08c_HEADS ${root_SOURCE_DIR}/projects/11_day/harib08c/asmhead.nas)
set(11_day_harib08c_BOOTB ${root_BINARY_DIR}/projects/11_day/harib08c/boot.bin)
set(11_day_harib08c_CCO   ${root_BINARY_DIR}/projects/11_day/harib08c/boot.o)
set(11_day_harib08c_CCS   ${root_SOURCE_DIR}/projects/11_day/harib08c/*.c)
set(11_day_harib08c_LDS   ${root_SOURCE_DIR}/projects/os.lds)
set(11_day_harib08c_FUNCS ${root_SOURCE_DIR}/projects/11_day/harib08c/naskfunc.nas)
set(11_day_harib08c_FUNCO ${root_BINARY_DIR}/projects/11_day/harib08c/naskfunc.o)
set(11_day_harib08c_FONTS ${root_SOURCE_DIR}/projects/11_day/harib08c/hankaku.txt)
set(11_day_harib08c_FONTB ${root_BINARY_DIR}/projects/11_day/harib08c/hankaku.bin)
set(11_day_harib08c_FONTO ${root_BINARY_DIR}/projects/11_day/harib08c/hankaku.o)
set(11_day_harib08c_LIBGE ${root_BINARY_DIR}/golibc/libgo.a)
set(11_day_harib08c_LIBGC ${root_BINARY_DIR}/projects/11_day/harib08c/libgo.o)

add_custom_target(11_day_harib08c_run
  COMMAND ${QEMU} ${QEMUOPT} ${11_day_harib08c_OS}
  DEPENDS 11_day_harib08c_img
)
add_custom_target(11_day_harib08c_debug
  COMMAND ${QEMU} -s -S ${QEMUOPT} ${11_day_harib08c_OS} ${QEMU_DEBUG_OPT}
  DEPENDS 11_day_harib08c_img
)
add_custom_target(11_day_harib08c_clean
  COMMAND rm -f ${11_day_harib08c_OS}
  COMMAND rm -f ${11_day_harib08c_SYS}
  COMMAND rm -f ${11_day_harib08c_IPLB}
  COMMAND rm -f ${11_day_harib08c_HEADB}
  COMMAND rm -f ${11_day_harib08c_BOOTB}
  COMMAND rm -f ${11_day_harib08c_WILDOBJ}
  COMMAND rm -f ${root_BINARY_DIR}/projects/11_day/harib08c/ipl10.bin
  COMMAND rm -f ${root_BINARY_DIR}/projects/11_day/harib08c/asmhead.bin
  COMMAND rm -f ${root_BINARY_DIR}/projects/11_day/harib08c/*_wine.bin
  COMMAND rm -f ${root_BINARY_DIR}/projects/11_day/harib08c/*.txt
)
add_custom_target(11_day_harib08c_ipl_build
  COMMAND ${RAKUSK_EXECUTABLE} ${11_day_harib08c_IPLS} ${11_day_harib08c_IPLB}
  DEPENDS ${11_day_harib08c_IPLS}
)
add_custom_command(
  OUTPUT ${11_day_harib08c_HEADB}
  COMMAND ${RAKUSK_EXECUTABLE} ${11_day_harib08c_HEADS} ${11_day_harib08c_HEADB}
  DEPENDS ${11_day_harib08c_HEADS}
  COMMENT "Generating asmhead.bin for 11_day_harib08c"
)

add_custom_command(
  OUTPUT ${11_day_harib08c_FUNCO}
  COMMAND ${RAKUSK_EXECUTABLE} ${11_day_harib08c_FUNCS} ${11_day_harib08c_FUNCO}
  DEPENDS ${11_day_harib08c_FUNCS}
  COMMENT "Generating naskfunc.o for 11_day_harib08c"
)
add_custom_command(
  OUTPUT ${11_day_harib08c_FONTO}
  COMMAND ${FONT} ${11_day_harib08c_FONTS} ${11_day_harib08c_FONTB}
  COMMAND ${B2O}  ${11_day_harib08c_FONTB} ${11_day_harib08c_FONTO} _hankaku
  DEPENDS ${11_day_harib08c_FONTS}
  COMMENT "Generating hankaku.o for 11_day_harib08c"
)
add_custom_command(
  OUTPUT ${11_day_harib08c_LIBGC}
  COMMAND ${CONV} -fcoff32 -nu ${11_day_harib08c_LIBGE} ${11_day_harib08c_LIBGC}
  DEPENDS ${11_day_harib08c_LIBGE}
  COMMENT "Converting golibc for 11_day_harib08c"
)
add_custom_target(11_day_harib08c_sys
  COMMAND gcc ${BINOPT} -T ${11_day_harib08c_LDS} ${11_day_harib08c_CCS} ${11_day_harib08c_WILDOBJ} -o ${11_day_harib08c_BOOTB}
  COMMAND cat ${11_day_harib08c_HEADB} ${11_day_harib08c_BOOTB} > ${11_day_harib08c_SYS}
  DEPENDS ${11_day_harib08c_HEADB} 11_day_harib08c_ipl_build
  DEPENDS ${11_day_harib08c_FUNCO}
  DEPENDS ${11_day_harib08c_FONTO} ${11_day_harib08c_LIBGC}
)
add_custom_target(11_day_harib08c_img
  COMMAND mformat -f 1440 -l HARIBOTEOS -N 0xffffffff -C -B ${11_day_harib08c_IPLB} -i ${11_day_harib08c_OS}
  COMMAND mcopy -i ${11_day_harib08c_OS} ${11_day_harib08c_SYS} ::
  DEPENDS 11_day_harib08c_sys
)

########### next target ###############
set(11_day_harib08c_asmhead_SRCS ${root_SOURCE_DIR}/projects/11_day/harib08c/asmhead.nas)
set(11_day_harib08c_asmhead_OUTS ${root_BINARY_DIR}/projects/11_day/harib08c/asmhead.bin)

add_custom_target(11_day_harib08c_asmhead
  COMMAND ${RAKUSK_EXECUTABLE} ${11_day_harib08c_asmhead_SRCS} ${11_day_harib08c_asmhead_OUTS}
  DEPENDS ${11_day_harib08c_asmhead_SRCS}
)

set(11_day_harib08c_asmhead_WINE_OUTS ${root_BINARY_DIR}/projects/11_day/harib08c/asmhead_wine.bin)
add_custom_target(11_day_harib08c_asmhead_wine
  COMMAND ${WINE} ${WINE_NASK} ${11_day_harib08c_asmhead_SRCS} ${11_day_harib08c_asmhead_WINE_OUTS}
)
add_dependencies(wine 11_day_harib08c_asmhead_wine)

add_custom_target(11_day_harib08c_asmhead_od
  COMMAND ${OD} -t x1 ${11_day_harib08c_asmhead_OUTS}      > ${11_day_harib08c_asmhead_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${11_day_harib08c_asmhead_WINE_OUTS} > ${11_day_harib08c_asmhead_WINE_OUTS}_t.txt
  COMMAND diff -s ${11_day_harib08c_asmhead_OUTS}_f.txt ${11_day_harib08c_asmhead_WINE_OUTS}_t.txt
)

add_dependencies(11_day_harib08c_asmhead_od 11_day_harib08c_asmhead)
add_dependencies(11_day_harib08c_asmhead_od 11_day_harib08c_asmhead_wine)
add_dependencies(od 11_day_harib08c_asmhead_od)

#----------------------------------------------------------
########### next target ###############
set(11_day_harib08c_ipl10_SRCS ${root_SOURCE_DIR}/projects/11_day/harib08c/ipl10.nas)
set(11_day_harib08c_ipl10_OUTS ${root_BINARY_DIR}/projects/11_day/harib08c/ipl10.bin)

add_custom_target(11_day_harib08c_ipl10
  COMMAND ${RAKUSK_EXECUTABLE} ${11_day_harib08c_ipl10_SRCS} ${11_day_harib08c_ipl10_OUTS}
  DEPENDS ${11_day_harib08c_ipl10_SRCS}
)

set(11_day_harib08c_ipl10_WINE_OUTS ${root_BINARY_DIR}/projects/11_day/harib08c/ipl10_wine.bin)
add_custom_target(11_day_harib08c_ipl10_wine
  COMMAND ${WINE} ${WINE_NASK} ${11_day_harib08c_ipl10_SRCS} ${11_day_harib08c_ipl10_WINE_OUTS}
)
add_dependencies(wine 11_day_harib08c_ipl10_wine)

add_custom_target(11_day_harib08c_ipl10_od
  COMMAND ${OD} -t x1 ${11_day_harib08c_ipl10_OUTS}      > ${11_day_harib08c_ipl10_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${11_day_harib08c_ipl10_WINE_OUTS} > ${11_day_harib08c_ipl10_WINE_OUTS}_t.txt
  COMMAND diff -s ${11_day_harib08c_ipl10_OUTS}_f.txt ${11_day_harib08c_ipl10_WINE_OUTS}_t.txt
)

add_dependencies(11_day_harib08c_ipl10_od 11_day_harib08c_ipl10)
add_dependencies(11_day_harib08c_ipl10_od 11_day_harib08c_ipl10_wine)
add_dependencies(od 11_day_harib08c_ipl10_od)

#----------------------------------------------------------
########### next target ###############
set(11_day_harib08c_naskfunc_SRCS ${root_SOURCE_DIR}/projects/11_day/harib08c/naskfunc.nas)
set(11_day_harib08c_naskfunc_OUTS ${root_BINARY_DIR}/projects/11_day/harib08c/naskfunc.img)

add_custom_target(11_day_harib08c_naskfunc
  COMMAND ${RAKUSK_EXECUTABLE} ${11_day_harib08c_naskfunc_SRCS} ${11_day_harib08c_naskfunc_OUTS}
  DEPENDS ${11_day_harib08c_naskfunc_SRCS}
)

add_dependencies(images 11_day_harib08c_naskfunc)
set(11_day_harib08c_naskfunc_WINE_OUTS ${root_BINARY_DIR}/projects/11_day/harib08c/naskfunc_wine.img)
add_custom_target(11_day_harib08c_naskfunc_wine
  COMMAND ${WINE} ${WINE_NASK} ${11_day_harib08c_naskfunc_SRCS} ${11_day_harib08c_naskfunc_WINE_OUTS}
)
add_dependencies(wine 11_day_harib08c_naskfunc_wine)

add_custom_target(11_day_harib08c_naskfunc_od
  COMMAND ${OD} -t x1 ${11_day_harib08c_naskfunc_OUTS}      > ${11_day_harib08c_naskfunc_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${11_day_harib08c_naskfunc_WINE_OUTS} > ${11_day_harib08c_naskfunc_WINE_OUTS}_t.txt
  COMMAND diff -s ${11_day_harib08c_naskfunc_OUTS}_f.txt ${11_day_harib08c_naskfunc_WINE_OUTS}_t.txt
)

add_dependencies(11_day_harib08c_naskfunc_od 11_day_harib08c_naskfunc)
add_dependencies(11_day_harib08c_naskfunc_od 11_day_harib08c_naskfunc_wine)
add_dependencies(od 11_day_harib08c_naskfunc_od)

#----------------------------------------------------------
