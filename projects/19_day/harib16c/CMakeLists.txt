#----------------------------------------------------------
message(STATUS "Entering directory projects/19_day/harib16c/")

set(FONT ${root_BINARY_DIR}/src/makefont)
set(B2O  ${root_BINARY_DIR}/src/bin2obj)
set(CONV ${root_BINARY_DIR}/objconv/objconv)
set(19_day_harib16c_WILDOBJ ${root_BINARY_DIR}/projects/19_day/harib16c/*.o)

# Full OS build rules
set(19_day_harib16c_OS    ${root_BINARY_DIR}/projects/19_day/harib16c/os.img)
set(19_day_harib16c_SYS   ${root_BINARY_DIR}/projects/19_day/harib16c/os.sys)
set(19_day_harib16c_IPLB  ${root_BINARY_DIR}/projects/19_day/harib16c/ipl.bin)
set(19_day_harib16c_IPLS  ${root_SOURCE_DIR}/projects/19_day/harib16c/ipl10.nas)
set(19_day_harib16c_HEADB ${root_BINARY_DIR}/projects/19_day/harib16c/asmhead.bin)
set(19_day_harib16c_HEADS ${root_SOURCE_DIR}/projects/19_day/harib16c/asmhead.nas)
set(19_day_harib16c_BOOTB ${root_BINARY_DIR}/projects/19_day/harib16c/boot.bin)
set(19_day_harib16c_CCO   ${root_BINARY_DIR}/projects/19_day/harib16c/boot.o)
set(19_day_harib16c_CCS   ${root_SOURCE_DIR}/projects/19_day/harib16c/*.c)
set(19_day_harib16c_LDS   ${root_SOURCE_DIR}/projects/os.lds)
set(19_day_harib16c_FUNCS ${root_SOURCE_DIR}/projects/19_day/harib16c/naskfunc.nas)
set(19_day_harib16c_FUNCO ${root_BINARY_DIR}/projects/19_day/harib16c/naskfunc.o)
set(19_day_harib16c_FONTS ${root_SOURCE_DIR}/projects/19_day/harib16c/hankaku.txt)
set(19_day_harib16c_FONTB ${root_BINARY_DIR}/projects/19_day/harib16c/hankaku.bin)
set(19_day_harib16c_FONTO ${root_BINARY_DIR}/projects/19_day/harib16c/hankaku.o)
set(19_day_harib16c_LIBGE ${root_BINARY_DIR}/golibc/libgo.a)
set(19_day_harib16c_LIBGC ${root_BINARY_DIR}/projects/19_day/harib16c/libgo.o)

add_custom_target(19_day_harib16c_run
  COMMAND ${QEMU} ${QEMUOPT} ${19_day_harib16c_OS}
  DEPENDS 19_day_harib16c_img
)
add_custom_target(19_day_harib16c_debug
  COMMAND ${QEMU} -s -S ${QEMUOPT} ${19_day_harib16c_OS} ${QEMU_DEBUG_OPT}
  DEPENDS 19_day_harib16c_img
)
add_custom_target(19_day_harib16c_clean
  COMMAND rm -f ${19_day_harib16c_OS}
  COMMAND rm -f ${19_day_harib16c_SYS}
  COMMAND rm -f ${19_day_harib16c_IPLB}
  COMMAND rm -f ${19_day_harib16c_HEADB}
  COMMAND rm -f ${19_day_harib16c_BOOTB}
  COMMAND rm -f ${19_day_harib16c_WILDOBJ}
  COMMAND rm -f ${root_BINARY_DIR}/projects/19_day/harib16c/ipl10.bin
  COMMAND rm -f ${root_BINARY_DIR}/projects/19_day/harib16c/asmhead.bin
  COMMAND rm -f ${root_BINARY_DIR}/projects/19_day/harib16c/*_wine.bin
  COMMAND rm -f ${root_BINARY_DIR}/projects/19_day/harib16c/*.txt
)
add_custom_target(19_day_harib16c_ipl_build
  COMMAND ${RAKUSK_EXECUTABLE} ${19_day_harib16c_IPLS} ${19_day_harib16c_IPLB}
  DEPENDS ${19_day_harib16c_IPLS}
)
add_custom_command(
  OUTPUT ${19_day_harib16c_HEADB}
  COMMAND ${RAKUSK_EXECUTABLE} ${19_day_harib16c_HEADS} ${19_day_harib16c_HEADB}
  DEPENDS ${19_day_harib16c_HEADS}
  COMMENT "Generating asmhead.bin for 19_day_harib16c"
)

add_custom_command(
  OUTPUT ${19_day_harib16c_FUNCO}
  COMMAND ${RAKUSK_EXECUTABLE} ${19_day_harib16c_FUNCS} ${19_day_harib16c_FUNCO}
  DEPENDS ${19_day_harib16c_FUNCS}
  COMMENT "Generating naskfunc.o for 19_day_harib16c"
)
add_custom_command(
  OUTPUT ${19_day_harib16c_FONTO}
  COMMAND ${FONT} ${19_day_harib16c_FONTS} ${19_day_harib16c_FONTB}
  COMMAND ${B2O}  ${19_day_harib16c_FONTB} ${19_day_harib16c_FONTO} _hankaku
  DEPENDS ${19_day_harib16c_FONTS}
  COMMENT "Generating hankaku.o for 19_day_harib16c"
)
add_custom_command(
  OUTPUT ${19_day_harib16c_LIBGC}
  COMMAND ${CONV} -fcoff32 -nu ${19_day_harib16c_LIBGE} ${19_day_harib16c_LIBGC}
  DEPENDS ${19_day_harib16c_LIBGE}
  COMMENT "Converting golibc for 19_day_harib16c"
)
add_custom_target(19_day_harib16c_sys
  COMMAND gcc ${BINOPT} -T ${19_day_harib16c_LDS} ${19_day_harib16c_CCS} ${19_day_harib16c_WILDOBJ} -o ${19_day_harib16c_BOOTB}
  COMMAND cat ${19_day_harib16c_HEADB} ${19_day_harib16c_BOOTB} > ${19_day_harib16c_SYS}
  DEPENDS ${19_day_harib16c_HEADB} 19_day_harib16c_ipl_build
  DEPENDS ${19_day_harib16c_FUNCO}
  DEPENDS ${19_day_harib16c_FONTO} ${19_day_harib16c_LIBGC}
)
add_custom_target(19_day_harib16c_img
  COMMAND mformat -f 1440 -l HARIBOTEOS -N 0xffffffff -C -B ${19_day_harib16c_IPLB} -i ${19_day_harib16c_OS}
  COMMAND mcopy -i ${19_day_harib16c_OS} ${19_day_harib16c_SYS} ::
  DEPENDS 19_day_harib16c_sys
)

########### next target ###############
set(19_day_harib16c_asmhead_SRCS ${root_SOURCE_DIR}/projects/19_day/harib16c/asmhead.nas)
set(19_day_harib16c_asmhead_OUTS ${root_BINARY_DIR}/projects/19_day/harib16c/asmhead.bin)

add_custom_target(19_day_harib16c_asmhead
  COMMAND ${RAKUSK_EXECUTABLE} ${19_day_harib16c_asmhead_SRCS} ${19_day_harib16c_asmhead_OUTS}
  DEPENDS ${19_day_harib16c_asmhead_SRCS}
)

set(19_day_harib16c_asmhead_WINE_OUTS ${root_BINARY_DIR}/projects/19_day/harib16c/asmhead_wine.bin)
add_custom_target(19_day_harib16c_asmhead_wine
  COMMAND ${WINE} ${WINE_NASK} ${19_day_harib16c_asmhead_SRCS} ${19_day_harib16c_asmhead_WINE_OUTS}
)
add_dependencies(wine 19_day_harib16c_asmhead_wine)

add_custom_target(19_day_harib16c_asmhead_od
  COMMAND ${OD} -t x1 ${19_day_harib16c_asmhead_OUTS}      > ${19_day_harib16c_asmhead_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${19_day_harib16c_asmhead_WINE_OUTS} > ${19_day_harib16c_asmhead_WINE_OUTS}_t.txt
  COMMAND diff -s ${19_day_harib16c_asmhead_OUTS}_f.txt ${19_day_harib16c_asmhead_WINE_OUTS}_t.txt
)

add_dependencies(19_day_harib16c_asmhead_od 19_day_harib16c_asmhead)
add_dependencies(19_day_harib16c_asmhead_od 19_day_harib16c_asmhead_wine)
add_dependencies(od 19_day_harib16c_asmhead_od)

#----------------------------------------------------------
########### next target ###############
set(19_day_harib16c_ipl10_SRCS ${root_SOURCE_DIR}/projects/19_day/harib16c/ipl10.nas)
set(19_day_harib16c_ipl10_OUTS ${root_BINARY_DIR}/projects/19_day/harib16c/ipl10.bin)

add_custom_target(19_day_harib16c_ipl10
  COMMAND ${RAKUSK_EXECUTABLE} ${19_day_harib16c_ipl10_SRCS} ${19_day_harib16c_ipl10_OUTS}
  DEPENDS ${19_day_harib16c_ipl10_SRCS}
)

set(19_day_harib16c_ipl10_WINE_OUTS ${root_BINARY_DIR}/projects/19_day/harib16c/ipl10_wine.bin)
add_custom_target(19_day_harib16c_ipl10_wine
  COMMAND ${WINE} ${WINE_NASK} ${19_day_harib16c_ipl10_SRCS} ${19_day_harib16c_ipl10_WINE_OUTS}
)
add_dependencies(wine 19_day_harib16c_ipl10_wine)

add_custom_target(19_day_harib16c_ipl10_od
  COMMAND ${OD} -t x1 ${19_day_harib16c_ipl10_OUTS}      > ${19_day_harib16c_ipl10_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${19_day_harib16c_ipl10_WINE_OUTS} > ${19_day_harib16c_ipl10_WINE_OUTS}_t.txt
  COMMAND diff -s ${19_day_harib16c_ipl10_OUTS}_f.txt ${19_day_harib16c_ipl10_WINE_OUTS}_t.txt
)

add_dependencies(19_day_harib16c_ipl10_od 19_day_harib16c_ipl10)
add_dependencies(19_day_harib16c_ipl10_od 19_day_harib16c_ipl10_wine)
add_dependencies(od 19_day_harib16c_ipl10_od)

#----------------------------------------------------------
########### next target ###############
set(19_day_harib16c_naskfunc_SRCS ${root_SOURCE_DIR}/projects/19_day/harib16c/naskfunc.nas)
set(19_day_harib16c_naskfunc_OUTS ${root_BINARY_DIR}/projects/19_day/harib16c/naskfunc.img)

add_custom_target(19_day_harib16c_naskfunc
  COMMAND ${RAKUSK_EXECUTABLE} ${19_day_harib16c_naskfunc_SRCS} ${19_day_harib16c_naskfunc_OUTS}
  DEPENDS ${19_day_harib16c_naskfunc_SRCS}
)

add_dependencies(images 19_day_harib16c_naskfunc)
set(19_day_harib16c_naskfunc_WINE_OUTS ${root_BINARY_DIR}/projects/19_day/harib16c/naskfunc_wine.img)
add_custom_target(19_day_harib16c_naskfunc_wine
  COMMAND ${WINE} ${WINE_NASK} ${19_day_harib16c_naskfunc_SRCS} ${19_day_harib16c_naskfunc_WINE_OUTS}
)
add_dependencies(wine 19_day_harib16c_naskfunc_wine)

add_custom_target(19_day_harib16c_naskfunc_od
  COMMAND ${OD} -t x1 ${19_day_harib16c_naskfunc_OUTS}      > ${19_day_harib16c_naskfunc_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${19_day_harib16c_naskfunc_WINE_OUTS} > ${19_day_harib16c_naskfunc_WINE_OUTS}_t.txt
  COMMAND diff -s ${19_day_harib16c_naskfunc_OUTS}_f.txt ${19_day_harib16c_naskfunc_WINE_OUTS}_t.txt
)

add_dependencies(19_day_harib16c_naskfunc_od 19_day_harib16c_naskfunc)
add_dependencies(19_day_harib16c_naskfunc_od 19_day_harib16c_naskfunc_wine)
add_dependencies(od 19_day_harib16c_naskfunc_od)

#----------------------------------------------------------
