#----------------------------------------------------------
message(STATUS "Entering directory projects/13_day/harib10f/")

set(FONT ${RAKUSK_EXECUTABLE} --makefont)
set(B2O  ${RAKUSK_EXECUTABLE} --bin2obj)
set(CONV ${root_BINARY_DIR}/objconv/objconv)
set(13_day_harib10f_WILDOBJ ${root_BINARY_DIR}/projects/13_day/harib10f/*.o)

# Full OS build rules
set(13_day_harib10f_OS    ${root_BINARY_DIR}/projects/13_day/harib10f/os.img)
set(13_day_harib10f_SYS   ${root_BINARY_DIR}/projects/13_day/harib10f/os.sys)
set(13_day_harib10f_IPLB  ${root_BINARY_DIR}/projects/13_day/harib10f/ipl.bin)
set(13_day_harib10f_IPLS  ${root_SOURCE_DIR}/projects/13_day/harib10f/ipl10.nas)
set(13_day_harib10f_HEADB ${root_BINARY_DIR}/projects/13_day/harib10f/asmhead.bin)
set(13_day_harib10f_HEADS ${root_SOURCE_DIR}/projects/13_day/harib10f/asmhead.nas)
set(13_day_harib10f_BOOTB ${root_BINARY_DIR}/projects/13_day/harib10f/boot.bin)
set(13_day_harib10f_CCO   ${root_BINARY_DIR}/projects/13_day/harib10f/boot.o)
set(13_day_harib10f_CCS   ${root_SOURCE_DIR}/projects/13_day/harib10f/*.c)
set(13_day_harib10f_LDS   ${root_SOURCE_DIR}/projects/os.lds)
set(13_day_harib10f_FUNCS ${root_SOURCE_DIR}/projects/13_day/harib10f/naskfunc.nas)
set(13_day_harib10f_FUNCO ${root_BINARY_DIR}/projects/13_day/harib10f/naskfunc.o)
set(13_day_harib10f_FONTS ${root_SOURCE_DIR}/projects/13_day/harib10f/hankaku.txt)
set(13_day_harib10f_FONTB ${root_BINARY_DIR}/projects/13_day/harib10f/hankaku.bin)
set(13_day_harib10f_FONTO ${root_BINARY_DIR}/projects/13_day/harib10f/hankaku.o)
set(13_day_harib10f_LIBGE ${root_BINARY_DIR}/golibc/libgo.a)
set(13_day_harib10f_LIBGC ${root_BINARY_DIR}/projects/13_day/harib10f/libgo.o)

add_custom_target(13_day_harib10f_run
  COMMAND ${QEMU} ${QEMUOPT} ${13_day_harib10f_OS}
  DEPENDS 13_day_harib10f_img
)
add_custom_target(13_day_harib10f_debug
  COMMAND ${QEMU} -s -S ${QEMUOPT} ${13_day_harib10f_OS} ${QEMU_DEBUG_OPT}
  DEPENDS 13_day_harib10f_img
)
add_custom_target(13_day_harib10f_clean
  COMMAND rm -f ${13_day_harib10f_OS}
  COMMAND rm -f ${13_day_harib10f_SYS}
  COMMAND rm -f ${13_day_harib10f_IPLB}
  COMMAND rm -f ${13_day_harib10f_HEADB}
  COMMAND rm -f ${13_day_harib10f_BOOTB}
  COMMAND rm -f ${13_day_harib10f_WILDOBJ}
  COMMAND rm -f ${root_BINARY_DIR}/projects/13_day/harib10f/ipl10.bin
  COMMAND rm -f ${root_BINARY_DIR}/projects/13_day/harib10f/asmhead.bin
  COMMAND rm -f ${root_BINARY_DIR}/projects/13_day/harib10f/*_wine.bin
  COMMAND rm -f ${root_BINARY_DIR}/projects/13_day/harib10f/*.txt
)
add_custom_target(13_day_harib10f_ipl_build
  COMMAND ${RAKUSK_EXECUTABLE} ${13_day_harib10f_IPLS} ${13_day_harib10f_IPLB}
  DEPENDS ${13_day_harib10f_IPLS}
)
add_custom_command(
  OUTPUT ${13_day_harib10f_HEADB}
  COMMAND ${RAKUSK_EXECUTABLE} ${13_day_harib10f_HEADS} ${13_day_harib10f_HEADB}
  DEPENDS ${13_day_harib10f_HEADS}
  COMMENT "Generating asmhead.bin for 13_day_harib10f"
)

add_custom_target(13_day_harib10f_sys
  COMMAND ${FONT} ${13_day_harib10f_FONTS} ${13_day_harib10f_FONTB}
  COMMAND ${B2O}  ${13_day_harib10f_FONTB} ${13_day_harib10f_FONTO} _hankaku
  COMMAND ${CONV} -fcoff32 -nu ${13_day_harib10f_LIBGE} ${13_day_harib10f_LIBGC}
  COMMAND ${RAKUSK_EXECUTABLE} ${13_day_harib10f_FUNCS} ${13_day_harib10f_FUNCO}
  COMMAND gcc ${BINOPT} -T ${13_day_harib10f_LDS} ${13_day_harib10f_CCS} ${13_day_harib10f_WILDOBJ} -o ${13_day_harib10f_BOOTB}
  COMMAND cat ${13_day_harib10f_HEADB} ${13_day_harib10f_BOOTB} > ${13_day_harib10f_SYS}
  DEPENDS ${13_day_harib10f_HEADB} 13_day_harib10f_ipl_build
  DEPENDS ${13_day_harib10f_FUNCO}
  DEPENDS ${13_day_harib10f_FONTO}
)
add_custom_target(13_day_harib10f_img
  COMMAND mformat -f 1440 -l HARIBOTEOS -N 0xffffffff -C -B ${13_day_harib10f_IPLB} -i ${13_day_harib10f_OS}
  COMMAND mcopy -i ${13_day_harib10f_OS} ${13_day_harib10f_SYS} ::
  DEPENDS 13_day_harib10f_sys
)

########### next target ###############
set(13_day_harib10f_asmhead_SRCS ${root_SOURCE_DIR}/projects/13_day/harib10f/asmhead.nas)
set(13_day_harib10f_asmhead_OUTS ${root_BINARY_DIR}/projects/13_day/harib10f/asmhead.bin)

add_custom_target(13_day_harib10f_asmhead
  COMMAND ${RAKUSK_EXECUTABLE} ${13_day_harib10f_asmhead_SRCS} ${13_day_harib10f_asmhead_OUTS}
  DEPENDS ${13_day_harib10f_asmhead_SRCS}
)

set(13_day_harib10f_asmhead_WINE_OUTS ${root_BINARY_DIR}/projects/13_day/harib10f/asmhead_wine.bin)
add_custom_target(13_day_harib10f_asmhead_wine
  COMMAND ${WINE} ${WINE_NASK} ${13_day_harib10f_asmhead_SRCS} ${13_day_harib10f_asmhead_WINE_OUTS}
)
add_dependencies(wine 13_day_harib10f_asmhead_wine)

add_custom_target(13_day_harib10f_asmhead_od
  COMMAND ${OD} -t x1 ${13_day_harib10f_asmhead_OUTS}      > ${13_day_harib10f_asmhead_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${13_day_harib10f_asmhead_WINE_OUTS} > ${13_day_harib10f_asmhead_WINE_OUTS}_t.txt
  COMMAND diff -s ${13_day_harib10f_asmhead_OUTS}_f.txt ${13_day_harib10f_asmhead_WINE_OUTS}_t.txt
)

add_dependencies(13_day_harib10f_asmhead_od 13_day_harib10f_asmhead)
add_dependencies(13_day_harib10f_asmhead_od 13_day_harib10f_asmhead_wine)
add_dependencies(od 13_day_harib10f_asmhead_od)

#----------------------------------------------------------
########### next target ###############
set(13_day_harib10f_ipl10_SRCS ${root_SOURCE_DIR}/projects/13_day/harib10f/ipl10.nas)
set(13_day_harib10f_ipl10_OUTS ${root_BINARY_DIR}/projects/13_day/harib10f/ipl10.bin)

add_custom_target(13_day_harib10f_ipl10
  COMMAND ${RAKUSK_EXECUTABLE} ${13_day_harib10f_ipl10_SRCS} ${13_day_harib10f_ipl10_OUTS}
  DEPENDS ${13_day_harib10f_ipl10_SRCS}
)

set(13_day_harib10f_ipl10_WINE_OUTS ${root_BINARY_DIR}/projects/13_day/harib10f/ipl10_wine.bin)
add_custom_target(13_day_harib10f_ipl10_wine
  COMMAND ${WINE} ${WINE_NASK} ${13_day_harib10f_ipl10_SRCS} ${13_day_harib10f_ipl10_WINE_OUTS}
)
add_dependencies(wine 13_day_harib10f_ipl10_wine)

add_custom_target(13_day_harib10f_ipl10_od
  COMMAND ${OD} -t x1 ${13_day_harib10f_ipl10_OUTS}      > ${13_day_harib10f_ipl10_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${13_day_harib10f_ipl10_WINE_OUTS} > ${13_day_harib10f_ipl10_WINE_OUTS}_t.txt
  COMMAND diff -s ${13_day_harib10f_ipl10_OUTS}_f.txt ${13_day_harib10f_ipl10_WINE_OUTS}_t.txt
)

add_dependencies(13_day_harib10f_ipl10_od 13_day_harib10f_ipl10)
add_dependencies(13_day_harib10f_ipl10_od 13_day_harib10f_ipl10_wine)
add_dependencies(od 13_day_harib10f_ipl10_od)

#----------------------------------------------------------
########### next target ###############
set(13_day_harib10f_naskfunc_SRCS ${root_SOURCE_DIR}/projects/13_day/harib10f/naskfunc.nas)
set(13_day_harib10f_naskfunc_OUTS ${root_BINARY_DIR}/projects/13_day/harib10f/naskfunc.img)

add_custom_target(13_day_harib10f_naskfunc
  COMMAND ${RAKUSK_EXECUTABLE} ${13_day_harib10f_naskfunc_SRCS} ${13_day_harib10f_naskfunc_OUTS}
  DEPENDS ${13_day_harib10f_naskfunc_SRCS}
)

add_dependencies(images 13_day_harib10f_naskfunc)
set(13_day_harib10f_naskfunc_WINE_OUTS ${root_BINARY_DIR}/projects/13_day/harib10f/naskfunc_wine.img)
add_custom_target(13_day_harib10f_naskfunc_wine
  COMMAND ${WINE} ${WINE_NASK} ${13_day_harib10f_naskfunc_SRCS} ${13_day_harib10f_naskfunc_WINE_OUTS}
)
add_dependencies(wine 13_day_harib10f_naskfunc_wine)

add_custom_target(13_day_harib10f_naskfunc_od
  COMMAND ${OD} -t x1 ${13_day_harib10f_naskfunc_OUTS}      > ${13_day_harib10f_naskfunc_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${13_day_harib10f_naskfunc_WINE_OUTS} > ${13_day_harib10f_naskfunc_WINE_OUTS}_t.txt
  COMMAND diff -s ${13_day_harib10f_naskfunc_OUTS}_f.txt ${13_day_harib10f_naskfunc_WINE_OUTS}_t.txt
)

add_dependencies(13_day_harib10f_naskfunc_od 13_day_harib10f_naskfunc)
add_dependencies(13_day_harib10f_naskfunc_od 13_day_harib10f_naskfunc_wine)
add_dependencies(od 13_day_harib10f_naskfunc_od)

#----------------------------------------------------------
