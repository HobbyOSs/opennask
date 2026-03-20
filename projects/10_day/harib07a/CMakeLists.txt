#----------------------------------------------------------
message(STATUS "Entering directory projects/10_day/harib07a/")

set(FONT ${RAKUSK_EXECUTABLE} --makefont)
set(B2O  ${RAKUSK_EXECUTABLE} --bin2obj)
set(CONV ${root_BINARY_DIR}/objconv/objconv)
set(10_day_harib07a_WILDOBJ ${root_BINARY_DIR}/projects/10_day/harib07a/*.o)

# Full OS build rules
set(10_day_harib07a_OS    ${root_BINARY_DIR}/projects/10_day/harib07a/os.img)
set(10_day_harib07a_SYS   ${root_BINARY_DIR}/projects/10_day/harib07a/os.sys)
set(10_day_harib07a_IPLB  ${root_BINARY_DIR}/projects/10_day/harib07a/ipl.bin)
set(10_day_harib07a_IPLS  ${root_SOURCE_DIR}/projects/10_day/harib07a/ipl10.nas)
set(10_day_harib07a_HEADB ${root_BINARY_DIR}/projects/10_day/harib07a/asmhead.bin)
set(10_day_harib07a_HEADS ${root_SOURCE_DIR}/projects/10_day/harib07a/asmhead.nas)
set(10_day_harib07a_BOOTB ${root_BINARY_DIR}/projects/10_day/harib07a/boot.bin)
set(10_day_harib07a_CCO   ${root_BINARY_DIR}/projects/10_day/harib07a/boot.o)
set(10_day_harib07a_CCS   ${root_SOURCE_DIR}/projects/10_day/harib07a/*.c)
set(10_day_harib07a_LDS   ${root_SOURCE_DIR}/projects/os.lds)
set(10_day_harib07a_FUNCS ${root_SOURCE_DIR}/projects/10_day/harib07a/naskfunc.nas)
set(10_day_harib07a_FUNCO ${root_BINARY_DIR}/projects/10_day/harib07a/naskfunc.o)
set(10_day_harib07a_FONTS ${root_SOURCE_DIR}/projects/10_day/harib07a/hankaku.txt)
set(10_day_harib07a_FONTB ${root_BINARY_DIR}/projects/10_day/harib07a/hankaku.bin)
set(10_day_harib07a_FONTO ${root_BINARY_DIR}/projects/10_day/harib07a/hankaku.o)
set(10_day_harib07a_LIBGE ${root_BINARY_DIR}/golibc/libgo.a)
set(10_day_harib07a_LIBGC ${root_BINARY_DIR}/projects/10_day/harib07a/libgo.o)

add_custom_target(10_day_harib07a_run
  COMMAND ${QEMU} ${QEMUOPT} ${10_day_harib07a_OS}
  DEPENDS 10_day_harib07a_img
)
add_custom_target(10_day_harib07a_debug
  COMMAND ${QEMU} -s -S ${QEMUOPT} ${10_day_harib07a_OS} ${QEMU_DEBUG_OPT}
  DEPENDS 10_day_harib07a_img
)
add_custom_target(10_day_harib07a_clean
  COMMAND rm -f ${10_day_harib07a_OS}
  COMMAND rm -f ${10_day_harib07a_SYS}
  COMMAND rm -f ${10_day_harib07a_IPLB}
  COMMAND rm -f ${10_day_harib07a_HEADB}
  COMMAND rm -f ${10_day_harib07a_BOOTB}
  COMMAND rm -f ${10_day_harib07a_WILDOBJ}
  COMMAND rm -f ${root_BINARY_DIR}/projects/10_day/harib07a/ipl10.bin
  COMMAND rm -f ${root_BINARY_DIR}/projects/10_day/harib07a/asmhead.bin
  COMMAND rm -f ${root_BINARY_DIR}/projects/10_day/harib07a/*_wine.bin
  COMMAND rm -f ${root_BINARY_DIR}/projects/10_day/harib07a/*.txt
)
add_custom_target(10_day_harib07a_ipl_build
  COMMAND ${RAKUSK_EXECUTABLE} ${10_day_harib07a_IPLS} ${10_day_harib07a_IPLB}
  DEPENDS ${10_day_harib07a_IPLS}
)
add_custom_command(
  OUTPUT ${10_day_harib07a_HEADB}
  COMMAND ${RAKUSK_EXECUTABLE} ${10_day_harib07a_HEADS} ${10_day_harib07a_HEADB}
  DEPENDS ${10_day_harib07a_HEADS}
  COMMENT "Generating asmhead.bin for 10_day_harib07a"
)

add_custom_target(10_day_harib07a_sys
  COMMAND ${FONT} ${10_day_harib07a_FONTS} ${10_day_harib07a_FONTB}
  COMMAND ${B2O}  ${10_day_harib07a_FONTB} ${10_day_harib07a_FONTO} _hankaku
  COMMAND ${CONV} -fcoff32 -nu ${10_day_harib07a_LIBGE} ${10_day_harib07a_LIBGC}
  COMMAND ${RAKUSK_EXECUTABLE} ${10_day_harib07a_FUNCS} ${10_day_harib07a_FUNCO}
  COMMAND gcc ${BINOPT} -T ${10_day_harib07a_LDS} ${10_day_harib07a_CCS} ${10_day_harib07a_WILDOBJ} -o ${10_day_harib07a_BOOTB}
  COMMAND cat ${10_day_harib07a_HEADB} ${10_day_harib07a_BOOTB} > ${10_day_harib07a_SYS}
  DEPENDS ${10_day_harib07a_HEADB} 10_day_harib07a_ipl_build
  DEPENDS ${10_day_harib07a_FUNCO}
  DEPENDS ${10_day_harib07a_FONTO}
)
add_custom_target(10_day_harib07a_img
  COMMAND mformat -f 1440 -l HARIBOTEOS -N 0xffffffff -C -B ${10_day_harib07a_IPLB} -i ${10_day_harib07a_OS}
  COMMAND mcopy -i ${10_day_harib07a_OS} ${10_day_harib07a_SYS} ::
  DEPENDS 10_day_harib07a_sys
)

########### next target ###############
set(10_day_harib07a_asmhead_SRCS ${root_SOURCE_DIR}/projects/10_day/harib07a/asmhead.nas)
set(10_day_harib07a_asmhead_OUTS ${root_BINARY_DIR}/projects/10_day/harib07a/asmhead.bin)

add_custom_target(10_day_harib07a_asmhead
  COMMAND ${RAKUSK_EXECUTABLE} ${10_day_harib07a_asmhead_SRCS} ${10_day_harib07a_asmhead_OUTS}
  DEPENDS ${10_day_harib07a_asmhead_SRCS}
)

set(10_day_harib07a_asmhead_WINE_OUTS ${root_BINARY_DIR}/projects/10_day/harib07a/asmhead_wine.bin)
add_custom_target(10_day_harib07a_asmhead_wine
  COMMAND ${WINE} ${WINE_NASK} ${10_day_harib07a_asmhead_SRCS} ${10_day_harib07a_asmhead_WINE_OUTS}
)
add_dependencies(wine 10_day_harib07a_asmhead_wine)

add_custom_target(10_day_harib07a_asmhead_od
  COMMAND ${OD} -t x1 ${10_day_harib07a_asmhead_OUTS}      > ${10_day_harib07a_asmhead_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${10_day_harib07a_asmhead_WINE_OUTS} > ${10_day_harib07a_asmhead_WINE_OUTS}_t.txt
  COMMAND diff -s ${10_day_harib07a_asmhead_OUTS}_f.txt ${10_day_harib07a_asmhead_WINE_OUTS}_t.txt
)

add_dependencies(10_day_harib07a_asmhead_od 10_day_harib07a_asmhead)
add_dependencies(10_day_harib07a_asmhead_od 10_day_harib07a_asmhead_wine)
add_dependencies(od 10_day_harib07a_asmhead_od)

#----------------------------------------------------------
########### next target ###############
set(10_day_harib07a_ipl10_SRCS ${root_SOURCE_DIR}/projects/10_day/harib07a/ipl10.nas)
set(10_day_harib07a_ipl10_OUTS ${root_BINARY_DIR}/projects/10_day/harib07a/ipl10.bin)

add_custom_target(10_day_harib07a_ipl10
  COMMAND ${RAKUSK_EXECUTABLE} ${10_day_harib07a_ipl10_SRCS} ${10_day_harib07a_ipl10_OUTS}
  DEPENDS ${10_day_harib07a_ipl10_SRCS}
)

set(10_day_harib07a_ipl10_WINE_OUTS ${root_BINARY_DIR}/projects/10_day/harib07a/ipl10_wine.bin)
add_custom_target(10_day_harib07a_ipl10_wine
  COMMAND ${WINE} ${WINE_NASK} ${10_day_harib07a_ipl10_SRCS} ${10_day_harib07a_ipl10_WINE_OUTS}
)
add_dependencies(wine 10_day_harib07a_ipl10_wine)

add_custom_target(10_day_harib07a_ipl10_od
  COMMAND ${OD} -t x1 ${10_day_harib07a_ipl10_OUTS}      > ${10_day_harib07a_ipl10_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${10_day_harib07a_ipl10_WINE_OUTS} > ${10_day_harib07a_ipl10_WINE_OUTS}_t.txt
  COMMAND diff -s ${10_day_harib07a_ipl10_OUTS}_f.txt ${10_day_harib07a_ipl10_WINE_OUTS}_t.txt
)

add_dependencies(10_day_harib07a_ipl10_od 10_day_harib07a_ipl10)
add_dependencies(10_day_harib07a_ipl10_od 10_day_harib07a_ipl10_wine)
add_dependencies(od 10_day_harib07a_ipl10_od)

#----------------------------------------------------------
########### next target ###############
set(10_day_harib07a_naskfunc_SRCS ${root_SOURCE_DIR}/projects/10_day/harib07a/naskfunc.nas)
set(10_day_harib07a_naskfunc_OUTS ${root_BINARY_DIR}/projects/10_day/harib07a/naskfunc.img)

add_custom_target(10_day_harib07a_naskfunc
  COMMAND ${RAKUSK_EXECUTABLE} ${10_day_harib07a_naskfunc_SRCS} ${10_day_harib07a_naskfunc_OUTS}
  DEPENDS ${10_day_harib07a_naskfunc_SRCS}
)

add_dependencies(images 10_day_harib07a_naskfunc)
set(10_day_harib07a_naskfunc_WINE_OUTS ${root_BINARY_DIR}/projects/10_day/harib07a/naskfunc_wine.img)
add_custom_target(10_day_harib07a_naskfunc_wine
  COMMAND ${WINE} ${WINE_NASK} ${10_day_harib07a_naskfunc_SRCS} ${10_day_harib07a_naskfunc_WINE_OUTS}
)
add_dependencies(wine 10_day_harib07a_naskfunc_wine)

add_custom_target(10_day_harib07a_naskfunc_od
  COMMAND ${OD} -t x1 ${10_day_harib07a_naskfunc_OUTS}      > ${10_day_harib07a_naskfunc_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${10_day_harib07a_naskfunc_WINE_OUTS} > ${10_day_harib07a_naskfunc_WINE_OUTS}_t.txt
  COMMAND diff -s ${10_day_harib07a_naskfunc_OUTS}_f.txt ${10_day_harib07a_naskfunc_WINE_OUTS}_t.txt
)

add_dependencies(10_day_harib07a_naskfunc_od 10_day_harib07a_naskfunc)
add_dependencies(10_day_harib07a_naskfunc_od 10_day_harib07a_naskfunc_wine)
add_dependencies(od 10_day_harib07a_naskfunc_od)

#----------------------------------------------------------
