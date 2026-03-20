#----------------------------------------------------------
message(STATUS "Entering directory projects/19_day/harib16a/")

set(FONT ${root_BINARY_DIR}/src/makefont)
set(B2O  ${root_BINARY_DIR}/src/bin2obj)
set(CONV ${root_BINARY_DIR}/objconv/objconv)
set(19_day_harib16a_WILDOBJ ${root_BINARY_DIR}/projects/19_day/harib16a/*.o)

# Full OS build rules
set(19_day_harib16a_OS    ${root_BINARY_DIR}/projects/19_day/harib16a/os.img)
set(19_day_harib16a_SYS   ${root_BINARY_DIR}/projects/19_day/harib16a/os.sys)
set(19_day_harib16a_IPLB  ${root_BINARY_DIR}/projects/19_day/harib16a/ipl.bin)
set(19_day_harib16a_IPLS  ${root_SOURCE_DIR}/projects/19_day/harib16a/ipl10.nas)
set(19_day_harib16a_HEADB ${root_BINARY_DIR}/projects/19_day/harib16a/asmhead.bin)
set(19_day_harib16a_HEADS ${root_SOURCE_DIR}/projects/19_day/harib16a/asmhead.nas)
set(19_day_harib16a_BOOTB ${root_BINARY_DIR}/projects/19_day/harib16a/boot.bin)
set(19_day_harib16a_CCO   ${root_BINARY_DIR}/projects/19_day/harib16a/boot.o)
set(19_day_harib16a_CCS   ${root_SOURCE_DIR}/projects/19_day/harib16a/*.c)
set(19_day_harib16a_LDS   ${root_SOURCE_DIR}/projects/os.lds)
set(19_day_harib16a_FUNCS ${root_SOURCE_DIR}/projects/19_day/harib16a/naskfunc.nas)
set(19_day_harib16a_FUNCO ${root_BINARY_DIR}/projects/19_day/harib16a/naskfunc.o)
set(19_day_harib16a_FONTS ${root_SOURCE_DIR}/projects/19_day/harib16a/hankaku.txt)
set(19_day_harib16a_FONTB ${root_BINARY_DIR}/projects/19_day/harib16a/hankaku.bin)
set(19_day_harib16a_FONTO ${root_BINARY_DIR}/projects/19_day/harib16a/hankaku.o)
set(19_day_harib16a_LIBGE ${root_BINARY_DIR}/golibc/libgo.a)
set(19_day_harib16a_LIBGC ${root_BINARY_DIR}/projects/19_day/harib16a/libgo.o)

add_custom_target(19_day_harib16a_run
  COMMAND ${QEMU} ${QEMUOPT} ${19_day_harib16a_OS}
  DEPENDS 19_day_harib16a_img
)
add_custom_target(19_day_harib16a_debug
  COMMAND ${QEMU} -s -S ${QEMUOPT} ${19_day_harib16a_OS} ${QEMU_DEBUG_OPT}
  DEPENDS 19_day_harib16a_img
)
add_custom_target(19_day_harib16a_clean
  COMMAND rm -f ${19_day_harib16a_OS}
  COMMAND rm -f ${19_day_harib16a_SYS}
  COMMAND rm -f ${19_day_harib16a_IPLB}
  COMMAND rm -f ${19_day_harib16a_HEADB}
  COMMAND rm -f ${19_day_harib16a_BOOTB}
  COMMAND rm -f ${19_day_harib16a_WILDOBJ}
  COMMAND rm -f ${root_BINARY_DIR}/projects/19_day/harib16a/ipl10.bin
  COMMAND rm -f ${root_BINARY_DIR}/projects/19_day/harib16a/asmhead.bin
  COMMAND rm -f ${root_BINARY_DIR}/projects/19_day/harib16a/*_wine.bin
  COMMAND rm -f ${root_BINARY_DIR}/projects/19_day/harib16a/*.txt
)
add_custom_target(19_day_harib16a_ipl_build
  COMMAND ${RAKUSK_EXECUTABLE} ${19_day_harib16a_IPLS} ${19_day_harib16a_IPLB}
  DEPENDS ${19_day_harib16a_IPLS}
)
add_custom_command(
  OUTPUT ${19_day_harib16a_HEADB}
  COMMAND ${RAKUSK_EXECUTABLE} ${19_day_harib16a_HEADS} ${19_day_harib16a_HEADB}
  DEPENDS ${19_day_harib16a_HEADS}
  COMMENT "Generating asmhead.bin for 19_day_harib16a"
)

add_custom_command(
  OUTPUT ${19_day_harib16a_FUNCO}
  COMMAND ${RAKUSK_EXECUTABLE} ${19_day_harib16a_FUNCS} ${19_day_harib16a_FUNCO}
  DEPENDS ${19_day_harib16a_FUNCS}
  COMMENT "Generating naskfunc.o for 19_day_harib16a"
)
add_custom_command(
  OUTPUT ${19_day_harib16a_FONTO}
  COMMAND ${FONT} ${19_day_harib16a_FONTS} ${19_day_harib16a_FONTB}
  COMMAND ${B2O}  ${19_day_harib16a_FONTB} ${19_day_harib16a_FONTO} _hankaku
  DEPENDS ${19_day_harib16a_FONTS}
  COMMENT "Generating hankaku.o for 19_day_harib16a"
)
add_custom_command(
  OUTPUT ${19_day_harib16a_LIBGC}
  COMMAND ${CONV} -fcoff32 -nu ${19_day_harib16a_LIBGE} ${19_day_harib16a_LIBGC}
  DEPENDS ${19_day_harib16a_LIBGE}
  COMMENT "Converting golibc for 19_day_harib16a"
)
add_custom_target(19_day_harib16a_sys
  COMMAND gcc ${BINOPT} -T ${19_day_harib16a_LDS} ${19_day_harib16a_CCS} ${19_day_harib16a_WILDOBJ} -o ${19_day_harib16a_BOOTB}
  COMMAND cat ${19_day_harib16a_HEADB} ${19_day_harib16a_BOOTB} > ${19_day_harib16a_SYS}
  DEPENDS ${19_day_harib16a_HEADB} 19_day_harib16a_ipl_build
  DEPENDS ${19_day_harib16a_FUNCO}
  DEPENDS ${19_day_harib16a_FONTO} ${19_day_harib16a_LIBGC}
)
add_custom_target(19_day_harib16a_img
  COMMAND mformat -f 1440 -l HARIBOTEOS -N 0xffffffff -C -B ${19_day_harib16a_IPLB} -i ${19_day_harib16a_OS}
  COMMAND mcopy -i ${19_day_harib16a_OS} ${19_day_harib16a_SYS} ::
  DEPENDS 19_day_harib16a_sys
)

########### next target ###############
set(19_day_harib16a_asmhead_SRCS ${root_SOURCE_DIR}/projects/19_day/harib16a/asmhead.nas)
set(19_day_harib16a_asmhead_OUTS ${root_BINARY_DIR}/projects/19_day/harib16a/asmhead.bin)

add_custom_target(19_day_harib16a_asmhead
  COMMAND ${RAKUSK_EXECUTABLE} ${19_day_harib16a_asmhead_SRCS} ${19_day_harib16a_asmhead_OUTS}
  DEPENDS ${19_day_harib16a_asmhead_SRCS}
)

set(19_day_harib16a_asmhead_WINE_OUTS ${root_BINARY_DIR}/projects/19_day/harib16a/asmhead_wine.bin)
add_custom_target(19_day_harib16a_asmhead_wine
  COMMAND ${WINE} ${WINE_NASK} ${19_day_harib16a_asmhead_SRCS} ${19_day_harib16a_asmhead_WINE_OUTS}
)
add_dependencies(wine 19_day_harib16a_asmhead_wine)

add_custom_target(19_day_harib16a_asmhead_od
  COMMAND ${OD} -t x1 ${19_day_harib16a_asmhead_OUTS}      > ${19_day_harib16a_asmhead_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${19_day_harib16a_asmhead_WINE_OUTS} > ${19_day_harib16a_asmhead_WINE_OUTS}_t.txt
  COMMAND diff -s ${19_day_harib16a_asmhead_OUTS}_f.txt ${19_day_harib16a_asmhead_WINE_OUTS}_t.txt
)

add_dependencies(19_day_harib16a_asmhead_od 19_day_harib16a_asmhead)
add_dependencies(19_day_harib16a_asmhead_od 19_day_harib16a_asmhead_wine)
add_dependencies(od 19_day_harib16a_asmhead_od)

#----------------------------------------------------------
########### next target ###############
set(19_day_harib16a_ipl10_SRCS ${root_SOURCE_DIR}/projects/19_day/harib16a/ipl10.nas)
set(19_day_harib16a_ipl10_OUTS ${root_BINARY_DIR}/projects/19_day/harib16a/ipl10.bin)

add_custom_target(19_day_harib16a_ipl10
  COMMAND ${RAKUSK_EXECUTABLE} ${19_day_harib16a_ipl10_SRCS} ${19_day_harib16a_ipl10_OUTS}
  DEPENDS ${19_day_harib16a_ipl10_SRCS}
)

set(19_day_harib16a_ipl10_WINE_OUTS ${root_BINARY_DIR}/projects/19_day/harib16a/ipl10_wine.bin)
add_custom_target(19_day_harib16a_ipl10_wine
  COMMAND ${WINE} ${WINE_NASK} ${19_day_harib16a_ipl10_SRCS} ${19_day_harib16a_ipl10_WINE_OUTS}
)
add_dependencies(wine 19_day_harib16a_ipl10_wine)

add_custom_target(19_day_harib16a_ipl10_od
  COMMAND ${OD} -t x1 ${19_day_harib16a_ipl10_OUTS}      > ${19_day_harib16a_ipl10_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${19_day_harib16a_ipl10_WINE_OUTS} > ${19_day_harib16a_ipl10_WINE_OUTS}_t.txt
  COMMAND diff -s ${19_day_harib16a_ipl10_OUTS}_f.txt ${19_day_harib16a_ipl10_WINE_OUTS}_t.txt
)

add_dependencies(19_day_harib16a_ipl10_od 19_day_harib16a_ipl10)
add_dependencies(19_day_harib16a_ipl10_od 19_day_harib16a_ipl10_wine)
add_dependencies(od 19_day_harib16a_ipl10_od)

#----------------------------------------------------------
########### next target ###############
set(19_day_harib16a_naskfunc_SRCS ${root_SOURCE_DIR}/projects/19_day/harib16a/naskfunc.nas)
set(19_day_harib16a_naskfunc_OUTS ${root_BINARY_DIR}/projects/19_day/harib16a/naskfunc.img)

add_custom_target(19_day_harib16a_naskfunc
  COMMAND ${RAKUSK_EXECUTABLE} ${19_day_harib16a_naskfunc_SRCS} ${19_day_harib16a_naskfunc_OUTS}
  DEPENDS ${19_day_harib16a_naskfunc_SRCS}
)

add_dependencies(images 19_day_harib16a_naskfunc)
set(19_day_harib16a_naskfunc_WINE_OUTS ${root_BINARY_DIR}/projects/19_day/harib16a/naskfunc_wine.img)
add_custom_target(19_day_harib16a_naskfunc_wine
  COMMAND ${WINE} ${WINE_NASK} ${19_day_harib16a_naskfunc_SRCS} ${19_day_harib16a_naskfunc_WINE_OUTS}
)
add_dependencies(wine 19_day_harib16a_naskfunc_wine)

add_custom_target(19_day_harib16a_naskfunc_od
  COMMAND ${OD} -t x1 ${19_day_harib16a_naskfunc_OUTS}      > ${19_day_harib16a_naskfunc_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${19_day_harib16a_naskfunc_WINE_OUTS} > ${19_day_harib16a_naskfunc_WINE_OUTS}_t.txt
  COMMAND diff -s ${19_day_harib16a_naskfunc_OUTS}_f.txt ${19_day_harib16a_naskfunc_WINE_OUTS}_t.txt
)

add_dependencies(19_day_harib16a_naskfunc_od 19_day_harib16a_naskfunc)
add_dependencies(19_day_harib16a_naskfunc_od 19_day_harib16a_naskfunc_wine)
add_dependencies(od 19_day_harib16a_naskfunc_od)

#----------------------------------------------------------
