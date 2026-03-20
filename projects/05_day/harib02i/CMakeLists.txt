#----------------------------------------------------------
message(STATUS "Entering directory projects/05_day/harib02i/")

set(FONT ${RAKUSK_EXECUTABLE} --makefont)
set(B2O  ${RAKUSK_EXECUTABLE} --bin2obj)
set(CONV ${root_BINARY_DIR}/objconv/objconv)
set(05_day_harib02i_WILDOBJ ${root_BINARY_DIR}/projects/05_day/harib02i/*.o)

# Full OS build rules
set(05_day_harib02i_OS    ${root_BINARY_DIR}/projects/05_day/harib02i/os.img)
set(05_day_harib02i_SYS   ${root_BINARY_DIR}/projects/05_day/harib02i/os.sys)
set(05_day_harib02i_IPLB  ${root_BINARY_DIR}/projects/05_day/harib02i/ipl.bin)
set(05_day_harib02i_IPLS  ${root_SOURCE_DIR}/projects/05_day/harib02i/ipl10.nas)
set(05_day_harib02i_HEADB ${root_BINARY_DIR}/projects/05_day/harib02i/asmhead.bin)
set(05_day_harib02i_HEADS ${root_SOURCE_DIR}/projects/05_day/harib02i/asmhead.nas)
set(05_day_harib02i_BOOTB ${root_BINARY_DIR}/projects/05_day/harib02i/boot.bin)
set(05_day_harib02i_CCO   ${root_BINARY_DIR}/projects/05_day/harib02i/boot.o)
set(05_day_harib02i_CCS   ${root_SOURCE_DIR}/projects/05_day/harib02i/*.c)
set(05_day_harib02i_LDS   ${root_SOURCE_DIR}/projects/os.lds)
set(05_day_harib02i_FUNCS ${root_SOURCE_DIR}/projects/05_day/harib02i/naskfunc.nas)
set(05_day_harib02i_FUNCO ${root_BINARY_DIR}/projects/05_day/harib02i/naskfunc.o)
set(05_day_harib02i_FONTS ${root_SOURCE_DIR}/projects/05_day/harib02i/hankaku.txt)
set(05_day_harib02i_FONTB ${root_BINARY_DIR}/projects/05_day/harib02i/hankaku.bin)
set(05_day_harib02i_FONTO ${root_BINARY_DIR}/projects/05_day/harib02i/hankaku.o)
set(05_day_harib02i_LIBGE ${root_BINARY_DIR}/golibc/libgo.a)
set(05_day_harib02i_LIBGC ${root_BINARY_DIR}/projects/05_day/harib02i/libgo.o)

add_custom_target(05_day_harib02i_run
  COMMAND ${QEMU} ${QEMUOPT} ${05_day_harib02i_OS}
  DEPENDS 05_day_harib02i_img
)
add_custom_target(05_day_harib02i_debug
  COMMAND ${QEMU} -s -S ${QEMUOPT} ${05_day_harib02i_OS} ${QEMU_DEBUG_OPT}
  DEPENDS 05_day_harib02i_img
)
add_custom_target(05_day_harib02i_clean
  COMMAND rm -f ${05_day_harib02i_OS}
  COMMAND rm -f ${05_day_harib02i_SYS}
  COMMAND rm -f ${05_day_harib02i_IPLB}
  COMMAND rm -f ${05_day_harib02i_HEADB}
  COMMAND rm -f ${05_day_harib02i_BOOTB}
  COMMAND rm -f ${05_day_harib02i_WILDOBJ}
  COMMAND rm -f ${root_BINARY_DIR}/projects/05_day/harib02i/ipl10.bin
  COMMAND rm -f ${root_BINARY_DIR}/projects/05_day/harib02i/asmhead.bin
  COMMAND rm -f ${root_BINARY_DIR}/projects/05_day/harib02i/*_wine.bin
  COMMAND rm -f ${root_BINARY_DIR}/projects/05_day/harib02i/*.txt
)
add_custom_target(05_day_harib02i_ipl_build
  COMMAND ${RAKUSK_EXECUTABLE} ${05_day_harib02i_IPLS} ${05_day_harib02i_IPLB}
  DEPENDS ${05_day_harib02i_IPLS}
)
add_custom_command(
  OUTPUT ${05_day_harib02i_HEADB}
  COMMAND ${RAKUSK_EXECUTABLE} ${05_day_harib02i_HEADS} ${05_day_harib02i_HEADB}
  DEPENDS ${05_day_harib02i_HEADS}
  COMMENT "Generating asmhead.bin for 05_day_harib02i"
)

add_custom_target(05_day_harib02i_sys
  COMMAND ${FONT} ${05_day_harib02i_FONTS} ${05_day_harib02i_FONTB}
  COMMAND ${B2O}  ${05_day_harib02i_FONTB} ${05_day_harib02i_FONTO} _hankaku
  COMMAND ${CONV} -fcoff32 -nu ${05_day_harib02i_LIBGE} ${05_day_harib02i_LIBGC}
  COMMAND ${RAKUSK_EXECUTABLE} ${05_day_harib02i_FUNCS} ${05_day_harib02i_FUNCO}
  COMMAND gcc ${BINOPT} -T ${05_day_harib02i_LDS} ${05_day_harib02i_CCS} ${05_day_harib02i_WILDOBJ} -o ${05_day_harib02i_BOOTB}
  COMMAND cat ${05_day_harib02i_HEADB} ${05_day_harib02i_BOOTB} > ${05_day_harib02i_SYS}
  DEPENDS ${05_day_harib02i_HEADB} 05_day_harib02i_ipl_build
  DEPENDS ${05_day_harib02i_FUNCO}
  DEPENDS ${05_day_harib02i_FONTO}
)
add_custom_target(05_day_harib02i_img
  COMMAND mformat -f 1440 -l HARIBOTEOS -N 0xffffffff -C -B ${05_day_harib02i_IPLB} -i ${05_day_harib02i_OS}
  COMMAND mcopy -i ${05_day_harib02i_OS} ${05_day_harib02i_SYS} ::
  DEPENDS 05_day_harib02i_sys
)

########### next target ###############
set(05_day_harib02i_asmhead_SRCS ${root_SOURCE_DIR}/projects/05_day/harib02i/asmhead.nas)
set(05_day_harib02i_asmhead_OUTS ${root_BINARY_DIR}/projects/05_day/harib02i/asmhead.bin)

add_custom_target(05_day_harib02i_asmhead
  COMMAND ${RAKUSK_EXECUTABLE} ${05_day_harib02i_asmhead_SRCS} ${05_day_harib02i_asmhead_OUTS}
  DEPENDS ${05_day_harib02i_asmhead_SRCS}
)

set(05_day_harib02i_asmhead_WINE_OUTS ${root_BINARY_DIR}/projects/05_day/harib02i/asmhead_wine.bin)
add_custom_target(05_day_harib02i_asmhead_wine
  COMMAND ${WINE} ${WINE_NASK} ${05_day_harib02i_asmhead_SRCS} ${05_day_harib02i_asmhead_WINE_OUTS}
)
add_dependencies(wine 05_day_harib02i_asmhead_wine)

add_custom_target(05_day_harib02i_asmhead_od
  COMMAND ${OD} -t x1 ${05_day_harib02i_asmhead_OUTS}      > ${05_day_harib02i_asmhead_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${05_day_harib02i_asmhead_WINE_OUTS} > ${05_day_harib02i_asmhead_WINE_OUTS}_t.txt
  COMMAND diff -s ${05_day_harib02i_asmhead_OUTS}_f.txt ${05_day_harib02i_asmhead_WINE_OUTS}_t.txt
)

add_dependencies(05_day_harib02i_asmhead_od 05_day_harib02i_asmhead)
add_dependencies(05_day_harib02i_asmhead_od 05_day_harib02i_asmhead_wine)
add_dependencies(od 05_day_harib02i_asmhead_od)

#----------------------------------------------------------
########### next target ###############
set(05_day_harib02i_ipl10_SRCS ${root_SOURCE_DIR}/projects/05_day/harib02i/ipl10.nas)
set(05_day_harib02i_ipl10_OUTS ${root_BINARY_DIR}/projects/05_day/harib02i/ipl10.bin)

add_custom_target(05_day_harib02i_ipl10
  COMMAND ${RAKUSK_EXECUTABLE} ${05_day_harib02i_ipl10_SRCS} ${05_day_harib02i_ipl10_OUTS}
  DEPENDS ${05_day_harib02i_ipl10_SRCS}
)

set(05_day_harib02i_ipl10_WINE_OUTS ${root_BINARY_DIR}/projects/05_day/harib02i/ipl10_wine.bin)
add_custom_target(05_day_harib02i_ipl10_wine
  COMMAND ${WINE} ${WINE_NASK} ${05_day_harib02i_ipl10_SRCS} ${05_day_harib02i_ipl10_WINE_OUTS}
)
add_dependencies(wine 05_day_harib02i_ipl10_wine)

add_custom_target(05_day_harib02i_ipl10_od
  COMMAND ${OD} -t x1 ${05_day_harib02i_ipl10_OUTS}      > ${05_day_harib02i_ipl10_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${05_day_harib02i_ipl10_WINE_OUTS} > ${05_day_harib02i_ipl10_WINE_OUTS}_t.txt
  COMMAND diff -s ${05_day_harib02i_ipl10_OUTS}_f.txt ${05_day_harib02i_ipl10_WINE_OUTS}_t.txt
)

add_dependencies(05_day_harib02i_ipl10_od 05_day_harib02i_ipl10)
add_dependencies(05_day_harib02i_ipl10_od 05_day_harib02i_ipl10_wine)
add_dependencies(od 05_day_harib02i_ipl10_od)

#----------------------------------------------------------
########### next target ###############
set(05_day_harib02i_naskfunc_SRCS ${root_SOURCE_DIR}/projects/05_day/harib02i/naskfunc.nas)
set(05_day_harib02i_naskfunc_OUTS ${root_BINARY_DIR}/projects/05_day/harib02i/naskfunc.img)

add_custom_target(05_day_harib02i_naskfunc
  COMMAND ${RAKUSK_EXECUTABLE} ${05_day_harib02i_naskfunc_SRCS} ${05_day_harib02i_naskfunc_OUTS}
  DEPENDS ${05_day_harib02i_naskfunc_SRCS}
)

add_dependencies(images 05_day_harib02i_naskfunc)
set(05_day_harib02i_naskfunc_WINE_OUTS ${root_BINARY_DIR}/projects/05_day/harib02i/naskfunc_wine.img)
add_custom_target(05_day_harib02i_naskfunc_wine
  COMMAND ${WINE} ${WINE_NASK} ${05_day_harib02i_naskfunc_SRCS} ${05_day_harib02i_naskfunc_WINE_OUTS}
)
add_dependencies(wine 05_day_harib02i_naskfunc_wine)

add_custom_target(05_day_harib02i_naskfunc_od
  COMMAND ${OD} -t x1 ${05_day_harib02i_naskfunc_OUTS}      > ${05_day_harib02i_naskfunc_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${05_day_harib02i_naskfunc_WINE_OUTS} > ${05_day_harib02i_naskfunc_WINE_OUTS}_t.txt
  COMMAND diff -s ${05_day_harib02i_naskfunc_OUTS}_f.txt ${05_day_harib02i_naskfunc_WINE_OUTS}_t.txt
)

add_dependencies(05_day_harib02i_naskfunc_od 05_day_harib02i_naskfunc)
add_dependencies(05_day_harib02i_naskfunc_od 05_day_harib02i_naskfunc_wine)
add_dependencies(od 05_day_harib02i_naskfunc_od)

#----------------------------------------------------------
