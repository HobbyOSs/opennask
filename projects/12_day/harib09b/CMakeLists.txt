#----------------------------------------------------------
message(STATUS "Entering directory projects/12_day/harib09b/")

set(FONT ${RAKUSK_EXECUTABLE} --makefont)
set(B2O  ${RAKUSK_EXECUTABLE} --bin2obj)
set(CONV ${root_BINARY_DIR}/objconv/objconv)
set(12_day_harib09b_WILDOBJ ${root_BINARY_DIR}/projects/12_day/harib09b/*.o)

# Full OS build rules
set(12_day_harib09b_OS    ${root_BINARY_DIR}/projects/12_day/harib09b/os.img)
set(12_day_harib09b_SYS   ${root_BINARY_DIR}/projects/12_day/harib09b/os.sys)
set(12_day_harib09b_IPLB  ${root_BINARY_DIR}/projects/12_day/harib09b/ipl.bin)
set(12_day_harib09b_IPLS  ${root_SOURCE_DIR}/projects/12_day/harib09b/ipl10.nas)
set(12_day_harib09b_HEADB ${root_BINARY_DIR}/projects/12_day/harib09b/asmhead.bin)
set(12_day_harib09b_HEADS ${root_SOURCE_DIR}/projects/12_day/harib09b/asmhead.nas)
set(12_day_harib09b_BOOTB ${root_BINARY_DIR}/projects/12_day/harib09b/boot.bin)
set(12_day_harib09b_CCO   ${root_BINARY_DIR}/projects/12_day/harib09b/boot.o)
set(12_day_harib09b_CCS   ${root_SOURCE_DIR}/projects/12_day/harib09b/*.c)
set(12_day_harib09b_LDS   ${root_SOURCE_DIR}/projects/os.lds)
set(12_day_harib09b_FUNCS ${root_SOURCE_DIR}/projects/12_day/harib09b/naskfunc.nas)
set(12_day_harib09b_FUNCO ${root_BINARY_DIR}/projects/12_day/harib09b/naskfunc.o)
set(12_day_harib09b_FONTS ${root_SOURCE_DIR}/projects/12_day/harib09b/hankaku.txt)
set(12_day_harib09b_FONTB ${root_BINARY_DIR}/projects/12_day/harib09b/hankaku.bin)
set(12_day_harib09b_FONTO ${root_BINARY_DIR}/projects/12_day/harib09b/hankaku.o)
set(12_day_harib09b_LIBGE ${root_BINARY_DIR}/golibc/libgo.a)
set(12_day_harib09b_LIBGC ${root_BINARY_DIR}/projects/12_day/harib09b/libgo.o)

add_custom_target(12_day_harib09b_run
  COMMAND ${QEMU} ${QEMUOPT} ${12_day_harib09b_OS}
  DEPENDS 12_day_harib09b_img
)
add_custom_target(12_day_harib09b_debug
  COMMAND ${QEMU} -s -S ${QEMUOPT} ${12_day_harib09b_OS} ${QEMU_DEBUG_OPT}
  DEPENDS 12_day_harib09b_img
)
add_custom_target(12_day_harib09b_clean
  COMMAND rm -f ${12_day_harib09b_OS}
  COMMAND rm -f ${12_day_harib09b_SYS}
  COMMAND rm -f ${12_day_harib09b_IPLB}
  COMMAND rm -f ${12_day_harib09b_HEADB}
  COMMAND rm -f ${12_day_harib09b_BOOTB}
  COMMAND rm -f ${12_day_harib09b_WILDOBJ}
  COMMAND rm -f ${root_BINARY_DIR}/projects/12_day/harib09b/ipl10.bin
  COMMAND rm -f ${root_BINARY_DIR}/projects/12_day/harib09b/asmhead.bin
  COMMAND rm -f ${root_BINARY_DIR}/projects/12_day/harib09b/*_wine.bin
  COMMAND rm -f ${root_BINARY_DIR}/projects/12_day/harib09b/*.txt
)
add_custom_target(12_day_harib09b_ipl_build
  COMMAND ${RAKUSK_EXECUTABLE} ${12_day_harib09b_IPLS} ${12_day_harib09b_IPLB}
  DEPENDS ${12_day_harib09b_IPLS}
)
add_custom_command(
  OUTPUT ${12_day_harib09b_HEADB}
  COMMAND ${RAKUSK_EXECUTABLE} ${12_day_harib09b_HEADS} ${12_day_harib09b_HEADB}
  DEPENDS ${12_day_harib09b_HEADS}
  COMMENT "Generating asmhead.bin for 12_day_harib09b"
)

add_custom_target(12_day_harib09b_sys
  COMMAND ${FONT} ${12_day_harib09b_FONTS} ${12_day_harib09b_FONTB}
  COMMAND ${B2O}  ${12_day_harib09b_FONTB} ${12_day_harib09b_FONTO} _hankaku
  COMMAND ${CONV} -fcoff32 -nu ${12_day_harib09b_LIBGE} ${12_day_harib09b_LIBGC}
  COMMAND ${RAKUSK_EXECUTABLE} ${12_day_harib09b_FUNCS} ${12_day_harib09b_FUNCO}
  COMMAND gcc ${BINOPT} -T ${12_day_harib09b_LDS} ${12_day_harib09b_CCS} ${12_day_harib09b_WILDOBJ} -o ${12_day_harib09b_BOOTB}
  COMMAND cat ${12_day_harib09b_HEADB} ${12_day_harib09b_BOOTB} > ${12_day_harib09b_SYS}
  DEPENDS ${12_day_harib09b_HEADB} 12_day_harib09b_ipl_build
  DEPENDS ${12_day_harib09b_FUNCO}
  DEPENDS ${12_day_harib09b_FONTO}
)
add_custom_target(12_day_harib09b_img
  COMMAND mformat -f 1440 -l HARIBOTEOS -N 0xffffffff -C -B ${12_day_harib09b_IPLB} -i ${12_day_harib09b_OS}
  COMMAND mcopy -i ${12_day_harib09b_OS} ${12_day_harib09b_SYS} ::
  DEPENDS 12_day_harib09b_sys
)

########### next target ###############
set(12_day_harib09b_asmhead_SRCS ${root_SOURCE_DIR}/projects/12_day/harib09b/asmhead.nas)
set(12_day_harib09b_asmhead_OUTS ${root_BINARY_DIR}/projects/12_day/harib09b/asmhead.bin)

add_custom_target(12_day_harib09b_asmhead
  COMMAND ${RAKUSK_EXECUTABLE} ${12_day_harib09b_asmhead_SRCS} ${12_day_harib09b_asmhead_OUTS}
  DEPENDS ${12_day_harib09b_asmhead_SRCS}
)

set(12_day_harib09b_asmhead_WINE_OUTS ${root_BINARY_DIR}/projects/12_day/harib09b/asmhead_wine.bin)
add_custom_target(12_day_harib09b_asmhead_wine
  COMMAND ${WINE} ${WINE_NASK} ${12_day_harib09b_asmhead_SRCS} ${12_day_harib09b_asmhead_WINE_OUTS}
)
add_dependencies(wine 12_day_harib09b_asmhead_wine)

add_custom_target(12_day_harib09b_asmhead_od
  COMMAND ${OD} -t x1 ${12_day_harib09b_asmhead_OUTS}      > ${12_day_harib09b_asmhead_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${12_day_harib09b_asmhead_WINE_OUTS} > ${12_day_harib09b_asmhead_WINE_OUTS}_t.txt
  COMMAND diff -s ${12_day_harib09b_asmhead_OUTS}_f.txt ${12_day_harib09b_asmhead_WINE_OUTS}_t.txt
)

add_dependencies(12_day_harib09b_asmhead_od 12_day_harib09b_asmhead)
add_dependencies(12_day_harib09b_asmhead_od 12_day_harib09b_asmhead_wine)
add_dependencies(od 12_day_harib09b_asmhead_od)

#----------------------------------------------------------
########### next target ###############
set(12_day_harib09b_ipl10_SRCS ${root_SOURCE_DIR}/projects/12_day/harib09b/ipl10.nas)
set(12_day_harib09b_ipl10_OUTS ${root_BINARY_DIR}/projects/12_day/harib09b/ipl10.bin)

add_custom_target(12_day_harib09b_ipl10
  COMMAND ${RAKUSK_EXECUTABLE} ${12_day_harib09b_ipl10_SRCS} ${12_day_harib09b_ipl10_OUTS}
  DEPENDS ${12_day_harib09b_ipl10_SRCS}
)

set(12_day_harib09b_ipl10_WINE_OUTS ${root_BINARY_DIR}/projects/12_day/harib09b/ipl10_wine.bin)
add_custom_target(12_day_harib09b_ipl10_wine
  COMMAND ${WINE} ${WINE_NASK} ${12_day_harib09b_ipl10_SRCS} ${12_day_harib09b_ipl10_WINE_OUTS}
)
add_dependencies(wine 12_day_harib09b_ipl10_wine)

add_custom_target(12_day_harib09b_ipl10_od
  COMMAND ${OD} -t x1 ${12_day_harib09b_ipl10_OUTS}      > ${12_day_harib09b_ipl10_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${12_day_harib09b_ipl10_WINE_OUTS} > ${12_day_harib09b_ipl10_WINE_OUTS}_t.txt
  COMMAND diff -s ${12_day_harib09b_ipl10_OUTS}_f.txt ${12_day_harib09b_ipl10_WINE_OUTS}_t.txt
)

add_dependencies(12_day_harib09b_ipl10_od 12_day_harib09b_ipl10)
add_dependencies(12_day_harib09b_ipl10_od 12_day_harib09b_ipl10_wine)
add_dependencies(od 12_day_harib09b_ipl10_od)

#----------------------------------------------------------
########### next target ###############
set(12_day_harib09b_naskfunc_SRCS ${root_SOURCE_DIR}/projects/12_day/harib09b/naskfunc.nas)
set(12_day_harib09b_naskfunc_OUTS ${root_BINARY_DIR}/projects/12_day/harib09b/naskfunc.img)

add_custom_target(12_day_harib09b_naskfunc
  COMMAND ${RAKUSK_EXECUTABLE} ${12_day_harib09b_naskfunc_SRCS} ${12_day_harib09b_naskfunc_OUTS}
  DEPENDS ${12_day_harib09b_naskfunc_SRCS}
)

add_dependencies(images 12_day_harib09b_naskfunc)
set(12_day_harib09b_naskfunc_WINE_OUTS ${root_BINARY_DIR}/projects/12_day/harib09b/naskfunc_wine.img)
add_custom_target(12_day_harib09b_naskfunc_wine
  COMMAND ${WINE} ${WINE_NASK} ${12_day_harib09b_naskfunc_SRCS} ${12_day_harib09b_naskfunc_WINE_OUTS}
)
add_dependencies(wine 12_day_harib09b_naskfunc_wine)

add_custom_target(12_day_harib09b_naskfunc_od
  COMMAND ${OD} -t x1 ${12_day_harib09b_naskfunc_OUTS}      > ${12_day_harib09b_naskfunc_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${12_day_harib09b_naskfunc_WINE_OUTS} > ${12_day_harib09b_naskfunc_WINE_OUTS}_t.txt
  COMMAND diff -s ${12_day_harib09b_naskfunc_OUTS}_f.txt ${12_day_harib09b_naskfunc_WINE_OUTS}_t.txt
)

add_dependencies(12_day_harib09b_naskfunc_od 12_day_harib09b_naskfunc)
add_dependencies(12_day_harib09b_naskfunc_od 12_day_harib09b_naskfunc_wine)
add_dependencies(od 12_day_harib09b_naskfunc_od)

#----------------------------------------------------------
