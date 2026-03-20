#----------------------------------------------------------
message(STATUS "Entering directory projects/18_day/harib15a/")

set(FONT ${RAKUSK_EXECUTABLE} --makefont)
set(B2O  ${RAKUSK_EXECUTABLE} --bin2obj)
set(CONV ${root_BINARY_DIR}/objconv/objconv)
set(18_day_harib15a_WILDOBJ ${root_BINARY_DIR}/projects/18_day/harib15a/*.o)

# Full OS build rules
set(18_day_harib15a_OS    ${root_BINARY_DIR}/projects/18_day/harib15a/os.img)
set(18_day_harib15a_SYS   ${root_BINARY_DIR}/projects/18_day/harib15a/os.sys)
set(18_day_harib15a_IPLB  ${root_BINARY_DIR}/projects/18_day/harib15a/ipl.bin)
set(18_day_harib15a_IPLS  ${root_SOURCE_DIR}/projects/18_day/harib15a/ipl10.nas)
set(18_day_harib15a_HEADB ${root_BINARY_DIR}/projects/18_day/harib15a/asmhead.bin)
set(18_day_harib15a_HEADS ${root_SOURCE_DIR}/projects/18_day/harib15a/asmhead.nas)
set(18_day_harib15a_BOOTB ${root_BINARY_DIR}/projects/18_day/harib15a/boot.bin)
set(18_day_harib15a_CCO   ${root_BINARY_DIR}/projects/18_day/harib15a/boot.o)
set(18_day_harib15a_CCS   ${root_SOURCE_DIR}/projects/18_day/harib15a/*.c)
set(18_day_harib15a_LDS   ${root_SOURCE_DIR}/projects/os.lds)
set(18_day_harib15a_FUNCS ${root_SOURCE_DIR}/projects/18_day/harib15a/naskfunc.nas)
set(18_day_harib15a_FUNCO ${root_BINARY_DIR}/projects/18_day/harib15a/naskfunc.o)
set(18_day_harib15a_FONTS ${root_SOURCE_DIR}/projects/18_day/harib15a/hankaku.txt)
set(18_day_harib15a_FONTB ${root_BINARY_DIR}/projects/18_day/harib15a/hankaku.bin)
set(18_day_harib15a_FONTO ${root_BINARY_DIR}/projects/18_day/harib15a/hankaku.o)
set(18_day_harib15a_LIBGE ${root_BINARY_DIR}/golibc/libgo.a)
set(18_day_harib15a_LIBGC ${root_BINARY_DIR}/projects/18_day/harib15a/libgo.o)

add_custom_target(18_day_harib15a_run
  COMMAND ${QEMU} ${QEMUOPT} ${18_day_harib15a_OS}
  DEPENDS 18_day_harib15a_img
)
add_custom_target(18_day_harib15a_debug
  COMMAND ${QEMU} -s -S ${QEMUOPT} ${18_day_harib15a_OS} ${QEMU_DEBUG_OPT}
  DEPENDS 18_day_harib15a_img
)
add_custom_target(18_day_harib15a_clean
  COMMAND rm -f ${18_day_harib15a_OS}
  COMMAND rm -f ${18_day_harib15a_SYS}
  COMMAND rm -f ${18_day_harib15a_IPLB}
  COMMAND rm -f ${18_day_harib15a_HEADB}
  COMMAND rm -f ${18_day_harib15a_BOOTB}
  COMMAND rm -f ${18_day_harib15a_WILDOBJ}
  COMMAND rm -f ${root_BINARY_DIR}/projects/18_day/harib15a/ipl10.bin
  COMMAND rm -f ${root_BINARY_DIR}/projects/18_day/harib15a/asmhead.bin
  COMMAND rm -f ${root_BINARY_DIR}/projects/18_day/harib15a/*_wine.bin
  COMMAND rm -f ${root_BINARY_DIR}/projects/18_day/harib15a/*.txt
)
add_custom_target(18_day_harib15a_ipl_build
  COMMAND ${RAKUSK_EXECUTABLE} ${18_day_harib15a_IPLS} ${18_day_harib15a_IPLB}
  DEPENDS ${18_day_harib15a_IPLS}
)
add_custom_command(
  OUTPUT ${18_day_harib15a_HEADB}
  COMMAND ${RAKUSK_EXECUTABLE} ${18_day_harib15a_HEADS} ${18_day_harib15a_HEADB}
  DEPENDS ${18_day_harib15a_HEADS}
  COMMENT "Generating asmhead.bin for 18_day_harib15a"
)

add_custom_target(18_day_harib15a_sys
  COMMAND ${FONT} ${18_day_harib15a_FONTS} ${18_day_harib15a_FONTB}
  COMMAND ${B2O}  ${18_day_harib15a_FONTB} ${18_day_harib15a_FONTO} _hankaku
  COMMAND ${CONV} -fcoff32 -nu ${18_day_harib15a_LIBGE} ${18_day_harib15a_LIBGC}
  COMMAND ${RAKUSK_EXECUTABLE} ${18_day_harib15a_FUNCS} ${18_day_harib15a_FUNCO}
  COMMAND gcc ${BINOPT} -T ${18_day_harib15a_LDS} ${18_day_harib15a_CCS} ${18_day_harib15a_WILDOBJ} -o ${18_day_harib15a_BOOTB}
  COMMAND cat ${18_day_harib15a_HEADB} ${18_day_harib15a_BOOTB} > ${18_day_harib15a_SYS}
  DEPENDS ${18_day_harib15a_HEADB} 18_day_harib15a_ipl_build
  DEPENDS ${18_day_harib15a_FUNCO}
  DEPENDS ${18_day_harib15a_FONTO}
)
add_custom_target(18_day_harib15a_img
  COMMAND mformat -f 1440 -l HARIBOTEOS -N 0xffffffff -C -B ${18_day_harib15a_IPLB} -i ${18_day_harib15a_OS}
  COMMAND mcopy -i ${18_day_harib15a_OS} ${18_day_harib15a_SYS} ::
  DEPENDS 18_day_harib15a_sys
)

########### next target ###############
set(18_day_harib15a_asmhead_SRCS ${root_SOURCE_DIR}/projects/18_day/harib15a/asmhead.nas)
set(18_day_harib15a_asmhead_OUTS ${root_BINARY_DIR}/projects/18_day/harib15a/asmhead.bin)

add_custom_target(18_day_harib15a_asmhead
  COMMAND ${RAKUSK_EXECUTABLE} ${18_day_harib15a_asmhead_SRCS} ${18_day_harib15a_asmhead_OUTS}
  DEPENDS ${18_day_harib15a_asmhead_SRCS}
)

set(18_day_harib15a_asmhead_WINE_OUTS ${root_BINARY_DIR}/projects/18_day/harib15a/asmhead_wine.bin)
add_custom_target(18_day_harib15a_asmhead_wine
  COMMAND ${WINE} ${WINE_NASK} ${18_day_harib15a_asmhead_SRCS} ${18_day_harib15a_asmhead_WINE_OUTS}
)
add_dependencies(wine 18_day_harib15a_asmhead_wine)

add_custom_target(18_day_harib15a_asmhead_od
  COMMAND ${OD} -t x1 ${18_day_harib15a_asmhead_OUTS}      > ${18_day_harib15a_asmhead_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${18_day_harib15a_asmhead_WINE_OUTS} > ${18_day_harib15a_asmhead_WINE_OUTS}_t.txt
  COMMAND diff -s ${18_day_harib15a_asmhead_OUTS}_f.txt ${18_day_harib15a_asmhead_WINE_OUTS}_t.txt
)

add_dependencies(18_day_harib15a_asmhead_od 18_day_harib15a_asmhead)
add_dependencies(18_day_harib15a_asmhead_od 18_day_harib15a_asmhead_wine)
add_dependencies(od 18_day_harib15a_asmhead_od)

#----------------------------------------------------------
########### next target ###############
set(18_day_harib15a_ipl10_SRCS ${root_SOURCE_DIR}/projects/18_day/harib15a/ipl10.nas)
set(18_day_harib15a_ipl10_OUTS ${root_BINARY_DIR}/projects/18_day/harib15a/ipl10.bin)

add_custom_target(18_day_harib15a_ipl10
  COMMAND ${RAKUSK_EXECUTABLE} ${18_day_harib15a_ipl10_SRCS} ${18_day_harib15a_ipl10_OUTS}
  DEPENDS ${18_day_harib15a_ipl10_SRCS}
)

set(18_day_harib15a_ipl10_WINE_OUTS ${root_BINARY_DIR}/projects/18_day/harib15a/ipl10_wine.bin)
add_custom_target(18_day_harib15a_ipl10_wine
  COMMAND ${WINE} ${WINE_NASK} ${18_day_harib15a_ipl10_SRCS} ${18_day_harib15a_ipl10_WINE_OUTS}
)
add_dependencies(wine 18_day_harib15a_ipl10_wine)

add_custom_target(18_day_harib15a_ipl10_od
  COMMAND ${OD} -t x1 ${18_day_harib15a_ipl10_OUTS}      > ${18_day_harib15a_ipl10_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${18_day_harib15a_ipl10_WINE_OUTS} > ${18_day_harib15a_ipl10_WINE_OUTS}_t.txt
  COMMAND diff -s ${18_day_harib15a_ipl10_OUTS}_f.txt ${18_day_harib15a_ipl10_WINE_OUTS}_t.txt
)

add_dependencies(18_day_harib15a_ipl10_od 18_day_harib15a_ipl10)
add_dependencies(18_day_harib15a_ipl10_od 18_day_harib15a_ipl10_wine)
add_dependencies(od 18_day_harib15a_ipl10_od)

#----------------------------------------------------------
########### next target ###############
set(18_day_harib15a_naskfunc_SRCS ${root_SOURCE_DIR}/projects/18_day/harib15a/naskfunc.nas)
set(18_day_harib15a_naskfunc_OUTS ${root_BINARY_DIR}/projects/18_day/harib15a/naskfunc.img)

add_custom_target(18_day_harib15a_naskfunc
  COMMAND ${RAKUSK_EXECUTABLE} ${18_day_harib15a_naskfunc_SRCS} ${18_day_harib15a_naskfunc_OUTS}
  DEPENDS ${18_day_harib15a_naskfunc_SRCS}
)

add_dependencies(images 18_day_harib15a_naskfunc)
set(18_day_harib15a_naskfunc_WINE_OUTS ${root_BINARY_DIR}/projects/18_day/harib15a/naskfunc_wine.img)
add_custom_target(18_day_harib15a_naskfunc_wine
  COMMAND ${WINE} ${WINE_NASK} ${18_day_harib15a_naskfunc_SRCS} ${18_day_harib15a_naskfunc_WINE_OUTS}
)
add_dependencies(wine 18_day_harib15a_naskfunc_wine)

add_custom_target(18_day_harib15a_naskfunc_od
  COMMAND ${OD} -t x1 ${18_day_harib15a_naskfunc_OUTS}      > ${18_day_harib15a_naskfunc_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${18_day_harib15a_naskfunc_WINE_OUTS} > ${18_day_harib15a_naskfunc_WINE_OUTS}_t.txt
  COMMAND diff -s ${18_day_harib15a_naskfunc_OUTS}_f.txt ${18_day_harib15a_naskfunc_WINE_OUTS}_t.txt
)

add_dependencies(18_day_harib15a_naskfunc_od 18_day_harib15a_naskfunc)
add_dependencies(18_day_harib15a_naskfunc_od 18_day_harib15a_naskfunc_wine)
add_dependencies(od 18_day_harib15a_naskfunc_od)

#----------------------------------------------------------
