#----------------------------------------------------------
message(STATUS "Entering directory projects/05_day/harib02f/")

set(FONT ${root_BINARY_DIR}/src/makefont)
set(B2O  ${root_BINARY_DIR}/src/bin2obj)
set(CONV ${root_BINARY_DIR}/objconv/objconv)
set(05_day_harib02f_WILDOBJ ${root_BINARY_DIR}/projects/05_day/harib02f/*.o)

# Full OS build rules
set(05_day_harib02f_OS    ${root_BINARY_DIR}/projects/05_day/harib02f/os.img)
set(05_day_harib02f_SYS   ${root_BINARY_DIR}/projects/05_day/harib02f/os.sys)
set(05_day_harib02f_IPLB  ${root_BINARY_DIR}/projects/05_day/harib02f/ipl.bin)
set(05_day_harib02f_IPLS  ${root_SOURCE_DIR}/projects/05_day/harib02f/ipl10.nas)
set(05_day_harib02f_HEADB ${root_BINARY_DIR}/projects/05_day/harib02f/asmhead.bin)
set(05_day_harib02f_HEADS ${root_SOURCE_DIR}/projects/05_day/harib02f/asmhead.nas)
set(05_day_harib02f_BOOTB ${root_BINARY_DIR}/projects/05_day/harib02f/boot.bin)
set(05_day_harib02f_CCO   ${root_BINARY_DIR}/projects/05_day/harib02f/boot.o)
set(05_day_harib02f_CCS   ${root_SOURCE_DIR}/projects/05_day/harib02f/*.c)
set(05_day_harib02f_LDS   ${root_SOURCE_DIR}/projects/os.lds)
set(05_day_harib02f_FUNCS ${root_SOURCE_DIR}/projects/05_day/harib02f/naskfunc.nas)
set(05_day_harib02f_FUNCO ${root_BINARY_DIR}/projects/05_day/harib02f/naskfunc.o)
set(05_day_harib02f_FONTS ${root_SOURCE_DIR}/projects/05_day/harib02f/hankaku.txt)
set(05_day_harib02f_FONTB ${root_BINARY_DIR}/projects/05_day/harib02f/hankaku.bin)
set(05_day_harib02f_FONTO ${root_BINARY_DIR}/projects/05_day/harib02f/hankaku.o)
set(05_day_harib02f_LIBGE ${root_BINARY_DIR}/golibc/libgo.a)
set(05_day_harib02f_LIBGC ${root_BINARY_DIR}/projects/05_day/harib02f/libgo.o)

add_custom_target(05_day_harib02f_run
  COMMAND ${QEMU} ${QEMUOPT} ${05_day_harib02f_OS}
  DEPENDS 05_day_harib02f_img
)
add_custom_target(05_day_harib02f_debug
  COMMAND ${QEMU} -s -S ${QEMUOPT} ${05_day_harib02f_OS} ${QEMU_DEBUG_OPT}
  DEPENDS 05_day_harib02f_img
)
add_custom_target(05_day_harib02f_clean
  COMMAND rm -f ${05_day_harib02f_OS}
  COMMAND rm -f ${05_day_harib02f_SYS}
  COMMAND rm -f ${05_day_harib02f_IPLB}
  COMMAND rm -f ${05_day_harib02f_HEADB}
  COMMAND rm -f ${05_day_harib02f_BOOTB}
  COMMAND rm -f ${05_day_harib02f_WILDOBJ}
  COMMAND rm -f ${root_BINARY_DIR}/projects/05_day/harib02f/ipl10.bin
  COMMAND rm -f ${root_BINARY_DIR}/projects/05_day/harib02f/asmhead.bin
  COMMAND rm -f ${root_BINARY_DIR}/projects/05_day/harib02f/*_wine.bin
  COMMAND rm -f ${root_BINARY_DIR}/projects/05_day/harib02f/*.txt
)
add_custom_target(05_day_harib02f_ipl_build
  COMMAND ${RAKUSK_EXECUTABLE} ${05_day_harib02f_IPLS} ${05_day_harib02f_IPLB}
  DEPENDS ${05_day_harib02f_IPLS}
)
add_custom_command(
  OUTPUT ${05_day_harib02f_HEADB}
  COMMAND ${RAKUSK_EXECUTABLE} ${05_day_harib02f_HEADS} ${05_day_harib02f_HEADB}
  DEPENDS ${05_day_harib02f_HEADS}
  COMMENT "Generating asmhead.bin for 05_day_harib02f"
)

add_custom_command(
  OUTPUT ${05_day_harib02f_FUNCO}
  COMMAND ${RAKUSK_EXECUTABLE} ${05_day_harib02f_FUNCS} ${05_day_harib02f_FUNCO}
  DEPENDS ${05_day_harib02f_FUNCS}
  COMMENT "Generating naskfunc.o for 05_day_harib02f"
)
add_custom_command(
  OUTPUT ${05_day_harib02f_FONTO}
  COMMAND ${FONT} ${05_day_harib02f_FONTS} ${05_day_harib02f_FONTB}
  COMMAND ${B2O}  ${05_day_harib02f_FONTB} ${05_day_harib02f_FONTO} _hankaku
  DEPENDS ${05_day_harib02f_FONTS}
  COMMENT "Generating hankaku.o for 05_day_harib02f"
)
add_custom_command(
  OUTPUT ${05_day_harib02f_LIBGC}
  COMMAND ${CONV} -fcoff32 -nu ${05_day_harib02f_LIBGE} ${05_day_harib02f_LIBGC}
  DEPENDS ${05_day_harib02f_LIBGE}
  COMMENT "Converting golibc for 05_day_harib02f"
)
add_custom_target(05_day_harib02f_sys
  COMMAND gcc ${BINOPT} -T ${05_day_harib02f_LDS} ${05_day_harib02f_CCS} ${05_day_harib02f_WILDOBJ} -o ${05_day_harib02f_BOOTB}
  COMMAND cat ${05_day_harib02f_HEADB} ${05_day_harib02f_BOOTB} > ${05_day_harib02f_SYS}
  DEPENDS ${05_day_harib02f_HEADB} 05_day_harib02f_ipl_build
  DEPENDS ${05_day_harib02f_FUNCO}
  DEPENDS ${05_day_harib02f_FONTO} ${05_day_harib02f_LIBGC}
)
add_custom_target(05_day_harib02f_img
  COMMAND mformat -f 1440 -l HARIBOTEOS -N 0xffffffff -C -B ${05_day_harib02f_IPLB} -i ${05_day_harib02f_OS}
  COMMAND mcopy -i ${05_day_harib02f_OS} ${05_day_harib02f_SYS} ::
  DEPENDS 05_day_harib02f_sys
)

########### next target ###############
set(05_day_harib02f_asmhead_SRCS ${root_SOURCE_DIR}/projects/05_day/harib02f/asmhead.nas)
set(05_day_harib02f_asmhead_OUTS ${root_BINARY_DIR}/projects/05_day/harib02f/asmhead.bin)

add_custom_target(05_day_harib02f_asmhead
  COMMAND ${RAKUSK_EXECUTABLE} ${05_day_harib02f_asmhead_SRCS} ${05_day_harib02f_asmhead_OUTS}
  DEPENDS ${05_day_harib02f_asmhead_SRCS}
)

set(05_day_harib02f_asmhead_WINE_OUTS ${root_BINARY_DIR}/projects/05_day/harib02f/asmhead_wine.bin)
add_custom_target(05_day_harib02f_asmhead_wine
  COMMAND ${WINE} ${WINE_NASK} ${05_day_harib02f_asmhead_SRCS} ${05_day_harib02f_asmhead_WINE_OUTS}
)
add_dependencies(wine 05_day_harib02f_asmhead_wine)

add_custom_target(05_day_harib02f_asmhead_od
  COMMAND ${OD} -t x1 ${05_day_harib02f_asmhead_OUTS}      > ${05_day_harib02f_asmhead_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${05_day_harib02f_asmhead_WINE_OUTS} > ${05_day_harib02f_asmhead_WINE_OUTS}_t.txt
  COMMAND diff -s ${05_day_harib02f_asmhead_OUTS}_f.txt ${05_day_harib02f_asmhead_WINE_OUTS}_t.txt
)

add_dependencies(05_day_harib02f_asmhead_od 05_day_harib02f_asmhead)
add_dependencies(05_day_harib02f_asmhead_od 05_day_harib02f_asmhead_wine)
add_dependencies(od 05_day_harib02f_asmhead_od)

#----------------------------------------------------------
########### next target ###############
set(05_day_harib02f_ipl10_SRCS ${root_SOURCE_DIR}/projects/05_day/harib02f/ipl10.nas)
set(05_day_harib02f_ipl10_OUTS ${root_BINARY_DIR}/projects/05_day/harib02f/ipl10.bin)

add_custom_target(05_day_harib02f_ipl10
  COMMAND ${RAKUSK_EXECUTABLE} ${05_day_harib02f_ipl10_SRCS} ${05_day_harib02f_ipl10_OUTS}
  DEPENDS ${05_day_harib02f_ipl10_SRCS}
)

set(05_day_harib02f_ipl10_WINE_OUTS ${root_BINARY_DIR}/projects/05_day/harib02f/ipl10_wine.bin)
add_custom_target(05_day_harib02f_ipl10_wine
  COMMAND ${WINE} ${WINE_NASK} ${05_day_harib02f_ipl10_SRCS} ${05_day_harib02f_ipl10_WINE_OUTS}
)
add_dependencies(wine 05_day_harib02f_ipl10_wine)

add_custom_target(05_day_harib02f_ipl10_od
  COMMAND ${OD} -t x1 ${05_day_harib02f_ipl10_OUTS}      > ${05_day_harib02f_ipl10_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${05_day_harib02f_ipl10_WINE_OUTS} > ${05_day_harib02f_ipl10_WINE_OUTS}_t.txt
  COMMAND diff -s ${05_day_harib02f_ipl10_OUTS}_f.txt ${05_day_harib02f_ipl10_WINE_OUTS}_t.txt
)

add_dependencies(05_day_harib02f_ipl10_od 05_day_harib02f_ipl10)
add_dependencies(05_day_harib02f_ipl10_od 05_day_harib02f_ipl10_wine)
add_dependencies(od 05_day_harib02f_ipl10_od)

#----------------------------------------------------------
########### next target ###############
set(05_day_harib02f_naskfunc_SRCS ${root_SOURCE_DIR}/projects/05_day/harib02f/naskfunc.nas)
set(05_day_harib02f_naskfunc_OUTS ${root_BINARY_DIR}/projects/05_day/harib02f/naskfunc.img)

add_custom_target(05_day_harib02f_naskfunc
  COMMAND ${RAKUSK_EXECUTABLE} ${05_day_harib02f_naskfunc_SRCS} ${05_day_harib02f_naskfunc_OUTS}
  DEPENDS ${05_day_harib02f_naskfunc_SRCS}
)

add_dependencies(images 05_day_harib02f_naskfunc)
set(05_day_harib02f_naskfunc_WINE_OUTS ${root_BINARY_DIR}/projects/05_day/harib02f/naskfunc_wine.img)
add_custom_target(05_day_harib02f_naskfunc_wine
  COMMAND ${WINE} ${WINE_NASK} ${05_day_harib02f_naskfunc_SRCS} ${05_day_harib02f_naskfunc_WINE_OUTS}
)
add_dependencies(wine 05_day_harib02f_naskfunc_wine)

add_custom_target(05_day_harib02f_naskfunc_od
  COMMAND ${OD} -t x1 ${05_day_harib02f_naskfunc_OUTS}      > ${05_day_harib02f_naskfunc_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${05_day_harib02f_naskfunc_WINE_OUTS} > ${05_day_harib02f_naskfunc_WINE_OUTS}_t.txt
  COMMAND diff -s ${05_day_harib02f_naskfunc_OUTS}_f.txt ${05_day_harib02f_naskfunc_WINE_OUTS}_t.txt
)

add_dependencies(05_day_harib02f_naskfunc_od 05_day_harib02f_naskfunc)
add_dependencies(05_day_harib02f_naskfunc_od 05_day_harib02f_naskfunc_wine)
add_dependencies(od 05_day_harib02f_naskfunc_od)

#----------------------------------------------------------
