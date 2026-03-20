#----------------------------------------------------------
message(STATUS "Entering directory projects/10_day/harib07b/")

set(FONT ${root_BINARY_DIR}/src/makefont)
set(B2O  ${root_BINARY_DIR}/src/bin2obj)
set(CONV ${root_BINARY_DIR}/objconv/objconv)
set(10_day_harib07b_WILDOBJ ${root_BINARY_DIR}/projects/10_day/harib07b/*.o)

# Full OS build rules
set(10_day_harib07b_OS    ${root_BINARY_DIR}/projects/10_day/harib07b/os.img)
set(10_day_harib07b_SYS   ${root_BINARY_DIR}/projects/10_day/harib07b/os.sys)
set(10_day_harib07b_IPLB  ${root_BINARY_DIR}/projects/10_day/harib07b/ipl.bin)
set(10_day_harib07b_IPLS  ${root_SOURCE_DIR}/projects/10_day/harib07b/ipl10.nas)
set(10_day_harib07b_HEADB ${root_BINARY_DIR}/projects/10_day/harib07b/asmhead.bin)
set(10_day_harib07b_HEADS ${root_SOURCE_DIR}/projects/10_day/harib07b/asmhead.nas)
set(10_day_harib07b_BOOTB ${root_BINARY_DIR}/projects/10_day/harib07b/boot.bin)
set(10_day_harib07b_CCO   ${root_BINARY_DIR}/projects/10_day/harib07b/boot.o)
set(10_day_harib07b_CCS   ${root_SOURCE_DIR}/projects/10_day/harib07b/*.c)
set(10_day_harib07b_LDS   ${root_SOURCE_DIR}/projects/os.lds)
set(10_day_harib07b_FUNCS ${root_SOURCE_DIR}/projects/10_day/harib07b/naskfunc.nas)
set(10_day_harib07b_FUNCO ${root_BINARY_DIR}/projects/10_day/harib07b/naskfunc.o)
set(10_day_harib07b_FONTS ${root_SOURCE_DIR}/projects/10_day/harib07b/hankaku.txt)
set(10_day_harib07b_FONTB ${root_BINARY_DIR}/projects/10_day/harib07b/hankaku.bin)
set(10_day_harib07b_FONTO ${root_BINARY_DIR}/projects/10_day/harib07b/hankaku.o)
set(10_day_harib07b_LIBGE ${root_BINARY_DIR}/golibc/libgo.a)
set(10_day_harib07b_LIBGC ${root_BINARY_DIR}/projects/10_day/harib07b/libgo.o)

add_custom_target(10_day_harib07b_run
  COMMAND ${QEMU} ${QEMUOPT} ${10_day_harib07b_OS}
  DEPENDS 10_day_harib07b_img
)
add_custom_target(10_day_harib07b_debug
  COMMAND ${QEMU} -s -S ${QEMUOPT} ${10_day_harib07b_OS} ${QEMU_DEBUG_OPT}
  DEPENDS 10_day_harib07b_img
)
add_custom_target(10_day_harib07b_clean
  COMMAND rm -f ${10_day_harib07b_OS}
  COMMAND rm -f ${10_day_harib07b_SYS}
  COMMAND rm -f ${10_day_harib07b_IPLB}
  COMMAND rm -f ${10_day_harib07b_HEADB}
  COMMAND rm -f ${10_day_harib07b_BOOTB}
  COMMAND rm -f ${10_day_harib07b_WILDOBJ}
  COMMAND rm -f ${root_BINARY_DIR}/projects/10_day/harib07b/ipl10.bin
  COMMAND rm -f ${root_BINARY_DIR}/projects/10_day/harib07b/asmhead.bin
  COMMAND rm -f ${root_BINARY_DIR}/projects/10_day/harib07b/*_wine.bin
  COMMAND rm -f ${root_BINARY_DIR}/projects/10_day/harib07b/*.txt
)
add_custom_target(10_day_harib07b_ipl_build
  COMMAND ${RAKUSK_EXECUTABLE} ${10_day_harib07b_IPLS} ${10_day_harib07b_IPLB}
  DEPENDS ${10_day_harib07b_IPLS}
)
add_custom_command(
  OUTPUT ${10_day_harib07b_HEADB}
  COMMAND ${RAKUSK_EXECUTABLE} ${10_day_harib07b_HEADS} ${10_day_harib07b_HEADB}
  DEPENDS ${10_day_harib07b_HEADS}
  COMMENT "Generating asmhead.bin for 10_day_harib07b"
)

add_custom_command(
  OUTPUT ${10_day_harib07b_FUNCO}
  COMMAND ${RAKUSK_EXECUTABLE} ${10_day_harib07b_FUNCS} ${10_day_harib07b_FUNCO}
  DEPENDS ${10_day_harib07b_FUNCS}
  COMMENT "Generating naskfunc.o for 10_day_harib07b"
)
add_custom_command(
  OUTPUT ${10_day_harib07b_FONTO}
  COMMAND ${FONT} ${10_day_harib07b_FONTS} ${10_day_harib07b_FONTB}
  COMMAND ${B2O}  ${10_day_harib07b_FONTB} ${10_day_harib07b_FONTO} _hankaku
  DEPENDS ${10_day_harib07b_FONTS}
  COMMENT "Generating hankaku.o for 10_day_harib07b"
)
add_custom_command(
  OUTPUT ${10_day_harib07b_LIBGC}
  COMMAND ${CONV} -fcoff32 -nu ${10_day_harib07b_LIBGE} ${10_day_harib07b_LIBGC}
  DEPENDS ${10_day_harib07b_LIBGE}
  COMMENT "Converting golibc for 10_day_harib07b"
)
add_custom_target(10_day_harib07b_sys
  COMMAND gcc ${BINOPT} -T ${10_day_harib07b_LDS} ${10_day_harib07b_CCS} ${10_day_harib07b_WILDOBJ} -o ${10_day_harib07b_BOOTB}
  COMMAND cat ${10_day_harib07b_HEADB} ${10_day_harib07b_BOOTB} > ${10_day_harib07b_SYS}
  DEPENDS ${10_day_harib07b_HEADB} 10_day_harib07b_ipl_build
  DEPENDS ${10_day_harib07b_FUNCO}
  DEPENDS ${10_day_harib07b_FONTO} ${10_day_harib07b_LIBGC}
)
add_custom_target(10_day_harib07b_img
  COMMAND mformat -f 1440 -l HARIBOTEOS -N 0xffffffff -C -B ${10_day_harib07b_IPLB} -i ${10_day_harib07b_OS}
  COMMAND mcopy -i ${10_day_harib07b_OS} ${10_day_harib07b_SYS} ::
  DEPENDS 10_day_harib07b_sys
)

########### next target ###############
set(10_day_harib07b_asmhead_SRCS ${root_SOURCE_DIR}/projects/10_day/harib07b/asmhead.nas)
set(10_day_harib07b_asmhead_OUTS ${root_BINARY_DIR}/projects/10_day/harib07b/asmhead.bin)

add_custom_target(10_day_harib07b_asmhead
  COMMAND ${RAKUSK_EXECUTABLE} ${10_day_harib07b_asmhead_SRCS} ${10_day_harib07b_asmhead_OUTS}
  DEPENDS ${10_day_harib07b_asmhead_SRCS}
)

set(10_day_harib07b_asmhead_WINE_OUTS ${root_BINARY_DIR}/projects/10_day/harib07b/asmhead_wine.bin)
add_custom_target(10_day_harib07b_asmhead_wine
  COMMAND ${WINE} ${WINE_NASK} ${10_day_harib07b_asmhead_SRCS} ${10_day_harib07b_asmhead_WINE_OUTS}
)
add_dependencies(wine 10_day_harib07b_asmhead_wine)

add_custom_target(10_day_harib07b_asmhead_od
  COMMAND ${OD} -t x1 ${10_day_harib07b_asmhead_OUTS}      > ${10_day_harib07b_asmhead_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${10_day_harib07b_asmhead_WINE_OUTS} > ${10_day_harib07b_asmhead_WINE_OUTS}_t.txt
  COMMAND diff -s ${10_day_harib07b_asmhead_OUTS}_f.txt ${10_day_harib07b_asmhead_WINE_OUTS}_t.txt
)

add_dependencies(10_day_harib07b_asmhead_od 10_day_harib07b_asmhead)
add_dependencies(10_day_harib07b_asmhead_od 10_day_harib07b_asmhead_wine)
add_dependencies(od 10_day_harib07b_asmhead_od)

#----------------------------------------------------------
########### next target ###############
set(10_day_harib07b_ipl10_SRCS ${root_SOURCE_DIR}/projects/10_day/harib07b/ipl10.nas)
set(10_day_harib07b_ipl10_OUTS ${root_BINARY_DIR}/projects/10_day/harib07b/ipl10.bin)

add_custom_target(10_day_harib07b_ipl10
  COMMAND ${RAKUSK_EXECUTABLE} ${10_day_harib07b_ipl10_SRCS} ${10_day_harib07b_ipl10_OUTS}
  DEPENDS ${10_day_harib07b_ipl10_SRCS}
)

set(10_day_harib07b_ipl10_WINE_OUTS ${root_BINARY_DIR}/projects/10_day/harib07b/ipl10_wine.bin)
add_custom_target(10_day_harib07b_ipl10_wine
  COMMAND ${WINE} ${WINE_NASK} ${10_day_harib07b_ipl10_SRCS} ${10_day_harib07b_ipl10_WINE_OUTS}
)
add_dependencies(wine 10_day_harib07b_ipl10_wine)

add_custom_target(10_day_harib07b_ipl10_od
  COMMAND ${OD} -t x1 ${10_day_harib07b_ipl10_OUTS}      > ${10_day_harib07b_ipl10_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${10_day_harib07b_ipl10_WINE_OUTS} > ${10_day_harib07b_ipl10_WINE_OUTS}_t.txt
  COMMAND diff -s ${10_day_harib07b_ipl10_OUTS}_f.txt ${10_day_harib07b_ipl10_WINE_OUTS}_t.txt
)

add_dependencies(10_day_harib07b_ipl10_od 10_day_harib07b_ipl10)
add_dependencies(10_day_harib07b_ipl10_od 10_day_harib07b_ipl10_wine)
add_dependencies(od 10_day_harib07b_ipl10_od)

#----------------------------------------------------------
########### next target ###############
set(10_day_harib07b_naskfunc_SRCS ${root_SOURCE_DIR}/projects/10_day/harib07b/naskfunc.nas)
set(10_day_harib07b_naskfunc_OUTS ${root_BINARY_DIR}/projects/10_day/harib07b/naskfunc.img)

add_custom_target(10_day_harib07b_naskfunc
  COMMAND ${RAKUSK_EXECUTABLE} ${10_day_harib07b_naskfunc_SRCS} ${10_day_harib07b_naskfunc_OUTS}
  DEPENDS ${10_day_harib07b_naskfunc_SRCS}
)

add_dependencies(images 10_day_harib07b_naskfunc)
set(10_day_harib07b_naskfunc_WINE_OUTS ${root_BINARY_DIR}/projects/10_day/harib07b/naskfunc_wine.img)
add_custom_target(10_day_harib07b_naskfunc_wine
  COMMAND ${WINE} ${WINE_NASK} ${10_day_harib07b_naskfunc_SRCS} ${10_day_harib07b_naskfunc_WINE_OUTS}
)
add_dependencies(wine 10_day_harib07b_naskfunc_wine)

add_custom_target(10_day_harib07b_naskfunc_od
  COMMAND ${OD} -t x1 ${10_day_harib07b_naskfunc_OUTS}      > ${10_day_harib07b_naskfunc_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${10_day_harib07b_naskfunc_WINE_OUTS} > ${10_day_harib07b_naskfunc_WINE_OUTS}_t.txt
  COMMAND diff -s ${10_day_harib07b_naskfunc_OUTS}_f.txt ${10_day_harib07b_naskfunc_WINE_OUTS}_t.txt
)

add_dependencies(10_day_harib07b_naskfunc_od 10_day_harib07b_naskfunc)
add_dependencies(10_day_harib07b_naskfunc_od 10_day_harib07b_naskfunc_wine)
add_dependencies(od 10_day_harib07b_naskfunc_od)

#----------------------------------------------------------
