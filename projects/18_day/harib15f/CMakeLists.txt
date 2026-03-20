#----------------------------------------------------------
message(STATUS "Entering directory projects/18_day/harib15f/")

set(FONT ${RAKUSK_EXECUTABLE} --makefont)
set(B2O  ${RAKUSK_EXECUTABLE} --bin2obj)
set(CONV ${root_BINARY_DIR}/objconv/objconv)
set(18_day_harib15f_WILDOBJ ${root_BINARY_DIR}/projects/18_day/harib15f/*.o)

# Full OS build rules
set(18_day_harib15f_OS    ${root_BINARY_DIR}/projects/18_day/harib15f/os.img)
set(18_day_harib15f_SYS   ${root_BINARY_DIR}/projects/18_day/harib15f/os.sys)
set(18_day_harib15f_IPLB  ${root_BINARY_DIR}/projects/18_day/harib15f/ipl.bin)
set(18_day_harib15f_IPLS  ${root_SOURCE_DIR}/projects/18_day/harib15f/ipl10.nas)
set(18_day_harib15f_HEADB ${root_BINARY_DIR}/projects/18_day/harib15f/asmhead.bin)
set(18_day_harib15f_HEADS ${root_SOURCE_DIR}/projects/18_day/harib15f/asmhead.nas)
set(18_day_harib15f_BOOTB ${root_BINARY_DIR}/projects/18_day/harib15f/boot.bin)
set(18_day_harib15f_CCO   ${root_BINARY_DIR}/projects/18_day/harib15f/boot.o)
set(18_day_harib15f_CCS   ${root_SOURCE_DIR}/projects/18_day/harib15f/*.c)
set(18_day_harib15f_LDS   ${root_SOURCE_DIR}/projects/os.lds)
set(18_day_harib15f_FUNCS ${root_SOURCE_DIR}/projects/18_day/harib15f/naskfunc.nas)
set(18_day_harib15f_FUNCO ${root_BINARY_DIR}/projects/18_day/harib15f/naskfunc.o)
set(18_day_harib15f_FONTS ${root_SOURCE_DIR}/projects/18_day/harib15f/hankaku.txt)
set(18_day_harib15f_FONTB ${root_BINARY_DIR}/projects/18_day/harib15f/hankaku.bin)
set(18_day_harib15f_FONTO ${root_BINARY_DIR}/projects/18_day/harib15f/hankaku.o)
set(18_day_harib15f_LIBGE ${root_BINARY_DIR}/golibc/libgo.a)
set(18_day_harib15f_LIBGC ${root_BINARY_DIR}/projects/18_day/harib15f/libgo.o)

add_custom_target(18_day_harib15f_run
  COMMAND ${QEMU} ${QEMUOPT} ${18_day_harib15f_OS}
  DEPENDS 18_day_harib15f_img
)
add_custom_target(18_day_harib15f_debug
  COMMAND ${QEMU} -s -S ${QEMUOPT} ${18_day_harib15f_OS} ${QEMU_DEBUG_OPT}
  DEPENDS 18_day_harib15f_img
)
add_custom_target(18_day_harib15f_clean
  COMMAND rm -f ${18_day_harib15f_OS}
  COMMAND rm -f ${18_day_harib15f_SYS}
  COMMAND rm -f ${18_day_harib15f_IPLB}
  COMMAND rm -f ${18_day_harib15f_HEADB}
  COMMAND rm -f ${18_day_harib15f_BOOTB}
  COMMAND rm -f ${18_day_harib15f_WILDOBJ}
  COMMAND rm -f ${root_BINARY_DIR}/projects/18_day/harib15f/ipl10.bin
  COMMAND rm -f ${root_BINARY_DIR}/projects/18_day/harib15f/asmhead.bin
  COMMAND rm -f ${root_BINARY_DIR}/projects/18_day/harib15f/*_wine.bin
  COMMAND rm -f ${root_BINARY_DIR}/projects/18_day/harib15f/*.txt
)
add_custom_target(18_day_harib15f_ipl_build
  COMMAND ${RAKUSK_EXECUTABLE} ${18_day_harib15f_IPLS} ${18_day_harib15f_IPLB}
  DEPENDS ${18_day_harib15f_IPLS}
)
add_custom_command(
  OUTPUT ${18_day_harib15f_HEADB}
  COMMAND ${RAKUSK_EXECUTABLE} ${18_day_harib15f_HEADS} ${18_day_harib15f_HEADB}
  DEPENDS ${18_day_harib15f_HEADS}
  COMMENT "Generating asmhead.bin for 18_day_harib15f"
)

add_custom_target(18_day_harib15f_sys
  COMMAND ${FONT} ${18_day_harib15f_FONTS} ${18_day_harib15f_FONTB}
  COMMAND ${B2O}  ${18_day_harib15f_FONTB} ${18_day_harib15f_FONTO} _hankaku
  COMMAND ${CONV} -fcoff32 -nu ${18_day_harib15f_LIBGE} ${18_day_harib15f_LIBGC}
  COMMAND ${RAKUSK_EXECUTABLE} ${18_day_harib15f_FUNCS} ${18_day_harib15f_FUNCO}
  COMMAND gcc ${BINOPT} -T ${18_day_harib15f_LDS} ${18_day_harib15f_CCS} ${18_day_harib15f_WILDOBJ} -o ${18_day_harib15f_BOOTB}
  COMMAND cat ${18_day_harib15f_HEADB} ${18_day_harib15f_BOOTB} > ${18_day_harib15f_SYS}
  DEPENDS ${18_day_harib15f_HEADB} 18_day_harib15f_ipl_build
  DEPENDS ${18_day_harib15f_FUNCO}
  DEPENDS ${18_day_harib15f_FONTO}
)
add_custom_target(18_day_harib15f_img
  COMMAND mformat -f 1440 -l HARIBOTEOS -N 0xffffffff -C -B ${18_day_harib15f_IPLB} -i ${18_day_harib15f_OS}
  COMMAND mcopy -i ${18_day_harib15f_OS} ${18_day_harib15f_SYS} ::
  DEPENDS 18_day_harib15f_sys
)

########### next target ###############
set(18_day_harib15f_asmhead_SRCS ${root_SOURCE_DIR}/projects/18_day/harib15f/asmhead.nas)
set(18_day_harib15f_asmhead_OUTS ${root_BINARY_DIR}/projects/18_day/harib15f/asmhead.bin)

add_custom_target(18_day_harib15f_asmhead
  COMMAND ${RAKUSK_EXECUTABLE} ${18_day_harib15f_asmhead_SRCS} ${18_day_harib15f_asmhead_OUTS}
  DEPENDS ${18_day_harib15f_asmhead_SRCS}
)

set(18_day_harib15f_asmhead_WINE_OUTS ${root_BINARY_DIR}/projects/18_day/harib15f/asmhead_wine.bin)
add_custom_target(18_day_harib15f_asmhead_wine
  COMMAND ${WINE} ${WINE_NASK} ${18_day_harib15f_asmhead_SRCS} ${18_day_harib15f_asmhead_WINE_OUTS}
)
add_dependencies(wine 18_day_harib15f_asmhead_wine)

add_custom_target(18_day_harib15f_asmhead_od
  COMMAND ${OD} -t x1 ${18_day_harib15f_asmhead_OUTS}      > ${18_day_harib15f_asmhead_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${18_day_harib15f_asmhead_WINE_OUTS} > ${18_day_harib15f_asmhead_WINE_OUTS}_t.txt
  COMMAND diff -s ${18_day_harib15f_asmhead_OUTS}_f.txt ${18_day_harib15f_asmhead_WINE_OUTS}_t.txt
)

add_dependencies(18_day_harib15f_asmhead_od 18_day_harib15f_asmhead)
add_dependencies(18_day_harib15f_asmhead_od 18_day_harib15f_asmhead_wine)
add_dependencies(od 18_day_harib15f_asmhead_od)

#----------------------------------------------------------
########### next target ###############
set(18_day_harib15f_ipl10_SRCS ${root_SOURCE_DIR}/projects/18_day/harib15f/ipl10.nas)
set(18_day_harib15f_ipl10_OUTS ${root_BINARY_DIR}/projects/18_day/harib15f/ipl10.bin)

add_custom_target(18_day_harib15f_ipl10
  COMMAND ${RAKUSK_EXECUTABLE} ${18_day_harib15f_ipl10_SRCS} ${18_day_harib15f_ipl10_OUTS}
  DEPENDS ${18_day_harib15f_ipl10_SRCS}
)

set(18_day_harib15f_ipl10_WINE_OUTS ${root_BINARY_DIR}/projects/18_day/harib15f/ipl10_wine.bin)
add_custom_target(18_day_harib15f_ipl10_wine
  COMMAND ${WINE} ${WINE_NASK} ${18_day_harib15f_ipl10_SRCS} ${18_day_harib15f_ipl10_WINE_OUTS}
)
add_dependencies(wine 18_day_harib15f_ipl10_wine)

add_custom_target(18_day_harib15f_ipl10_od
  COMMAND ${OD} -t x1 ${18_day_harib15f_ipl10_OUTS}      > ${18_day_harib15f_ipl10_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${18_day_harib15f_ipl10_WINE_OUTS} > ${18_day_harib15f_ipl10_WINE_OUTS}_t.txt
  COMMAND diff -s ${18_day_harib15f_ipl10_OUTS}_f.txt ${18_day_harib15f_ipl10_WINE_OUTS}_t.txt
)

add_dependencies(18_day_harib15f_ipl10_od 18_day_harib15f_ipl10)
add_dependencies(18_day_harib15f_ipl10_od 18_day_harib15f_ipl10_wine)
add_dependencies(od 18_day_harib15f_ipl10_od)

#----------------------------------------------------------
########### next target ###############
set(18_day_harib15f_naskfunc_SRCS ${root_SOURCE_DIR}/projects/18_day/harib15f/naskfunc.nas)
set(18_day_harib15f_naskfunc_OUTS ${root_BINARY_DIR}/projects/18_day/harib15f/naskfunc.img)

add_custom_target(18_day_harib15f_naskfunc
  COMMAND ${RAKUSK_EXECUTABLE} ${18_day_harib15f_naskfunc_SRCS} ${18_day_harib15f_naskfunc_OUTS}
  DEPENDS ${18_day_harib15f_naskfunc_SRCS}
)

add_dependencies(images 18_day_harib15f_naskfunc)
set(18_day_harib15f_naskfunc_WINE_OUTS ${root_BINARY_DIR}/projects/18_day/harib15f/naskfunc_wine.img)
add_custom_target(18_day_harib15f_naskfunc_wine
  COMMAND ${WINE} ${WINE_NASK} ${18_day_harib15f_naskfunc_SRCS} ${18_day_harib15f_naskfunc_WINE_OUTS}
)
add_dependencies(wine 18_day_harib15f_naskfunc_wine)

add_custom_target(18_day_harib15f_naskfunc_od
  COMMAND ${OD} -t x1 ${18_day_harib15f_naskfunc_OUTS}      > ${18_day_harib15f_naskfunc_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${18_day_harib15f_naskfunc_WINE_OUTS} > ${18_day_harib15f_naskfunc_WINE_OUTS}_t.txt
  COMMAND diff -s ${18_day_harib15f_naskfunc_OUTS}_f.txt ${18_day_harib15f_naskfunc_WINE_OUTS}_t.txt
)

add_dependencies(18_day_harib15f_naskfunc_od 18_day_harib15f_naskfunc)
add_dependencies(18_day_harib15f_naskfunc_od 18_day_harib15f_naskfunc_wine)
add_dependencies(od 18_day_harib15f_naskfunc_od)

#----------------------------------------------------------
