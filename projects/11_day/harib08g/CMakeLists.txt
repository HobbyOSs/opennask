#----------------------------------------------------------
message(STATUS "Entering directory projects/11_day/harib08g/")

set(FONT ${root_BINARY_DIR}/src/makefont)
set(B2O  ${root_BINARY_DIR}/src/bin2obj)
set(CONV ${root_BINARY_DIR}/objconv/objconv)
set(11_day_harib08g_WILDOBJ ${root_BINARY_DIR}/projects/11_day/harib08g/*.o)

# Full OS build rules
set(11_day_harib08g_OS    ${root_BINARY_DIR}/projects/11_day/harib08g/os.img)
set(11_day_harib08g_SYS   ${root_BINARY_DIR}/projects/11_day/harib08g/os.sys)
set(11_day_harib08g_IPLB  ${root_BINARY_DIR}/projects/11_day/harib08g/ipl.bin)
set(11_day_harib08g_IPLS  ${root_SOURCE_DIR}/projects/11_day/harib08g/ipl10.nas)
set(11_day_harib08g_HEADB ${root_BINARY_DIR}/projects/11_day/harib08g/asmhead.bin)
set(11_day_harib08g_HEADS ${root_SOURCE_DIR}/projects/11_day/harib08g/asmhead.nas)
set(11_day_harib08g_BOOTB ${root_BINARY_DIR}/projects/11_day/harib08g/boot.bin)
set(11_day_harib08g_CCO   ${root_BINARY_DIR}/projects/11_day/harib08g/boot.o)
set(11_day_harib08g_CCS   ${root_SOURCE_DIR}/projects/11_day/harib08g/*.c)
set(11_day_harib08g_LDS   ${root_SOURCE_DIR}/projects/os.lds)
set(11_day_harib08g_FUNCS ${root_SOURCE_DIR}/projects/11_day/harib08g/naskfunc.nas)
set(11_day_harib08g_FUNCO ${root_BINARY_DIR}/projects/11_day/harib08g/naskfunc.o)
set(11_day_harib08g_FONTS ${root_SOURCE_DIR}/projects/11_day/harib08g/hankaku.txt)
set(11_day_harib08g_FONTB ${root_BINARY_DIR}/projects/11_day/harib08g/hankaku.bin)
set(11_day_harib08g_FONTO ${root_BINARY_DIR}/projects/11_day/harib08g/hankaku.o)
set(11_day_harib08g_LIBGE ${root_BINARY_DIR}/golibc/libgo.a)
set(11_day_harib08g_LIBGC ${root_BINARY_DIR}/projects/11_day/harib08g/libgo.o)

add_custom_target(11_day_harib08g_run
  COMMAND ${QEMU} ${QEMUOPT} ${11_day_harib08g_OS}
  DEPENDS 11_day_harib08g_img
)
add_custom_target(11_day_harib08g_debug
  COMMAND ${QEMU} -s -S ${QEMUOPT} ${11_day_harib08g_OS} ${QEMU_DEBUG_OPT}
  DEPENDS 11_day_harib08g_img
)
add_custom_target(11_day_harib08g_clean
  COMMAND rm -f ${11_day_harib08g_OS}
  COMMAND rm -f ${11_day_harib08g_SYS}
  COMMAND rm -f ${11_day_harib08g_IPLB}
  COMMAND rm -f ${11_day_harib08g_HEADB}
  COMMAND rm -f ${11_day_harib08g_BOOTB}
  COMMAND rm -f ${11_day_harib08g_WILDOBJ}
  COMMAND rm -f ${root_BINARY_DIR}/projects/11_day/harib08g/ipl10.bin
  COMMAND rm -f ${root_BINARY_DIR}/projects/11_day/harib08g/asmhead.bin
  COMMAND rm -f ${root_BINARY_DIR}/projects/11_day/harib08g/*_wine.bin
  COMMAND rm -f ${root_BINARY_DIR}/projects/11_day/harib08g/*.txt
)
add_custom_target(11_day_harib08g_ipl_build
  COMMAND ${RAKUSK_EXECUTABLE} ${11_day_harib08g_IPLS} ${11_day_harib08g_IPLB}
  DEPENDS ${11_day_harib08g_IPLS}
)
add_custom_command(
  OUTPUT ${11_day_harib08g_HEADB}
  COMMAND ${RAKUSK_EXECUTABLE} ${11_day_harib08g_HEADS} ${11_day_harib08g_HEADB}
  DEPENDS ${11_day_harib08g_HEADS}
  COMMENT "Generating asmhead.bin for 11_day_harib08g"
)

add_custom_command(
  OUTPUT ${11_day_harib08g_FUNCO}
  COMMAND ${RAKUSK_EXECUTABLE} ${11_day_harib08g_FUNCS} ${11_day_harib08g_FUNCO}
  DEPENDS ${11_day_harib08g_FUNCS}
  COMMENT "Generating naskfunc.o for 11_day_harib08g"
)
add_custom_command(
  OUTPUT ${11_day_harib08g_FONTO}
  COMMAND ${FONT} ${11_day_harib08g_FONTS} ${11_day_harib08g_FONTB}
  COMMAND ${B2O}  ${11_day_harib08g_FONTB} ${11_day_harib08g_FONTO} _hankaku
  DEPENDS ${11_day_harib08g_FONTS}
  COMMENT "Generating hankaku.o for 11_day_harib08g"
)
add_custom_command(
  OUTPUT ${11_day_harib08g_LIBGC}
  COMMAND ${CONV} -fcoff32 -nu ${11_day_harib08g_LIBGE} ${11_day_harib08g_LIBGC}
  DEPENDS ${11_day_harib08g_LIBGE}
  COMMENT "Converting golibc for 11_day_harib08g"
)
add_custom_target(11_day_harib08g_sys
  COMMAND gcc ${BINOPT} -T ${11_day_harib08g_LDS} ${11_day_harib08g_CCS} ${11_day_harib08g_WILDOBJ} -o ${11_day_harib08g_BOOTB}
  COMMAND cat ${11_day_harib08g_HEADB} ${11_day_harib08g_BOOTB} > ${11_day_harib08g_SYS}
  DEPENDS ${11_day_harib08g_HEADB} 11_day_harib08g_ipl_build
  DEPENDS ${11_day_harib08g_FUNCO}
  DEPENDS ${11_day_harib08g_FONTO} ${11_day_harib08g_LIBGC}
)
add_custom_target(11_day_harib08g_img
  COMMAND mformat -f 1440 -l HARIBOTEOS -N 0xffffffff -C -B ${11_day_harib08g_IPLB} -i ${11_day_harib08g_OS}
  COMMAND mcopy -i ${11_day_harib08g_OS} ${11_day_harib08g_SYS} ::
  DEPENDS 11_day_harib08g_sys
)

########### next target ###############
set(11_day_harib08g_asmhead_SRCS ${root_SOURCE_DIR}/projects/11_day/harib08g/asmhead.nas)
set(11_day_harib08g_asmhead_OUTS ${root_BINARY_DIR}/projects/11_day/harib08g/asmhead.bin)

add_custom_target(11_day_harib08g_asmhead
  COMMAND ${RAKUSK_EXECUTABLE} ${11_day_harib08g_asmhead_SRCS} ${11_day_harib08g_asmhead_OUTS}
  DEPENDS ${11_day_harib08g_asmhead_SRCS}
)

set(11_day_harib08g_asmhead_WINE_OUTS ${root_BINARY_DIR}/projects/11_day/harib08g/asmhead_wine.bin)
add_custom_target(11_day_harib08g_asmhead_wine
  COMMAND ${WINE} ${WINE_NASK} ${11_day_harib08g_asmhead_SRCS} ${11_day_harib08g_asmhead_WINE_OUTS}
)
add_dependencies(wine 11_day_harib08g_asmhead_wine)

add_custom_target(11_day_harib08g_asmhead_od
  COMMAND ${OD} -t x1 ${11_day_harib08g_asmhead_OUTS}      > ${11_day_harib08g_asmhead_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${11_day_harib08g_asmhead_WINE_OUTS} > ${11_day_harib08g_asmhead_WINE_OUTS}_t.txt
  COMMAND diff -s ${11_day_harib08g_asmhead_OUTS}_f.txt ${11_day_harib08g_asmhead_WINE_OUTS}_t.txt
)

add_dependencies(11_day_harib08g_asmhead_od 11_day_harib08g_asmhead)
add_dependencies(11_day_harib08g_asmhead_od 11_day_harib08g_asmhead_wine)
add_dependencies(od 11_day_harib08g_asmhead_od)

#----------------------------------------------------------
########### next target ###############
set(11_day_harib08g_ipl10_SRCS ${root_SOURCE_DIR}/projects/11_day/harib08g/ipl10.nas)
set(11_day_harib08g_ipl10_OUTS ${root_BINARY_DIR}/projects/11_day/harib08g/ipl10.bin)

add_custom_target(11_day_harib08g_ipl10
  COMMAND ${RAKUSK_EXECUTABLE} ${11_day_harib08g_ipl10_SRCS} ${11_day_harib08g_ipl10_OUTS}
  DEPENDS ${11_day_harib08g_ipl10_SRCS}
)

set(11_day_harib08g_ipl10_WINE_OUTS ${root_BINARY_DIR}/projects/11_day/harib08g/ipl10_wine.bin)
add_custom_target(11_day_harib08g_ipl10_wine
  COMMAND ${WINE} ${WINE_NASK} ${11_day_harib08g_ipl10_SRCS} ${11_day_harib08g_ipl10_WINE_OUTS}
)
add_dependencies(wine 11_day_harib08g_ipl10_wine)

add_custom_target(11_day_harib08g_ipl10_od
  COMMAND ${OD} -t x1 ${11_day_harib08g_ipl10_OUTS}      > ${11_day_harib08g_ipl10_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${11_day_harib08g_ipl10_WINE_OUTS} > ${11_day_harib08g_ipl10_WINE_OUTS}_t.txt
  COMMAND diff -s ${11_day_harib08g_ipl10_OUTS}_f.txt ${11_day_harib08g_ipl10_WINE_OUTS}_t.txt
)

add_dependencies(11_day_harib08g_ipl10_od 11_day_harib08g_ipl10)
add_dependencies(11_day_harib08g_ipl10_od 11_day_harib08g_ipl10_wine)
add_dependencies(od 11_day_harib08g_ipl10_od)

#----------------------------------------------------------
########### next target ###############
set(11_day_harib08g_naskfunc_SRCS ${root_SOURCE_DIR}/projects/11_day/harib08g/naskfunc.nas)
set(11_day_harib08g_naskfunc_OUTS ${root_BINARY_DIR}/projects/11_day/harib08g/naskfunc.img)

add_custom_target(11_day_harib08g_naskfunc
  COMMAND ${RAKUSK_EXECUTABLE} ${11_day_harib08g_naskfunc_SRCS} ${11_day_harib08g_naskfunc_OUTS}
  DEPENDS ${11_day_harib08g_naskfunc_SRCS}
)

add_dependencies(images 11_day_harib08g_naskfunc)
set(11_day_harib08g_naskfunc_WINE_OUTS ${root_BINARY_DIR}/projects/11_day/harib08g/naskfunc_wine.img)
add_custom_target(11_day_harib08g_naskfunc_wine
  COMMAND ${WINE} ${WINE_NASK} ${11_day_harib08g_naskfunc_SRCS} ${11_day_harib08g_naskfunc_WINE_OUTS}
)
add_dependencies(wine 11_day_harib08g_naskfunc_wine)

add_custom_target(11_day_harib08g_naskfunc_od
  COMMAND ${OD} -t x1 ${11_day_harib08g_naskfunc_OUTS}      > ${11_day_harib08g_naskfunc_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${11_day_harib08g_naskfunc_WINE_OUTS} > ${11_day_harib08g_naskfunc_WINE_OUTS}_t.txt
  COMMAND diff -s ${11_day_harib08g_naskfunc_OUTS}_f.txt ${11_day_harib08g_naskfunc_WINE_OUTS}_t.txt
)

add_dependencies(11_day_harib08g_naskfunc_od 11_day_harib08g_naskfunc)
add_dependencies(11_day_harib08g_naskfunc_od 11_day_harib08g_naskfunc_wine)
add_dependencies(od 11_day_harib08g_naskfunc_od)

#----------------------------------------------------------
