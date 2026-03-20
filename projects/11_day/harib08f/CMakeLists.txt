#----------------------------------------------------------
message(STATUS "Entering directory projects/11_day/harib08f/")

set(FONT ${RAKUSK_EXECUTABLE} --makefont)
set(B2O  ${RAKUSK_EXECUTABLE} --bin2obj)
set(CONV ${root_BINARY_DIR}/objconv/objconv)
set(11_day_harib08f_WILDOBJ ${root_BINARY_DIR}/projects/11_day/harib08f/*.o)

# Full OS build rules
set(11_day_harib08f_OS    ${root_BINARY_DIR}/projects/11_day/harib08f/os.img)
set(11_day_harib08f_SYS   ${root_BINARY_DIR}/projects/11_day/harib08f/os.sys)
set(11_day_harib08f_IPLB  ${root_BINARY_DIR}/projects/11_day/harib08f/ipl.bin)
set(11_day_harib08f_IPLS  ${root_SOURCE_DIR}/projects/11_day/harib08f/ipl10.nas)
set(11_day_harib08f_HEADB ${root_BINARY_DIR}/projects/11_day/harib08f/asmhead.bin)
set(11_day_harib08f_HEADS ${root_SOURCE_DIR}/projects/11_day/harib08f/asmhead.nas)
set(11_day_harib08f_BOOTB ${root_BINARY_DIR}/projects/11_day/harib08f/boot.bin)
set(11_day_harib08f_CCO   ${root_BINARY_DIR}/projects/11_day/harib08f/boot.o)
set(11_day_harib08f_CCS   ${root_SOURCE_DIR}/projects/11_day/harib08f/*.c)
set(11_day_harib08f_LDS   ${root_SOURCE_DIR}/projects/os.lds)
set(11_day_harib08f_FUNCS ${root_SOURCE_DIR}/projects/11_day/harib08f/naskfunc.nas)
set(11_day_harib08f_FUNCO ${root_BINARY_DIR}/projects/11_day/harib08f/naskfunc.o)
set(11_day_harib08f_FONTS ${root_SOURCE_DIR}/projects/11_day/harib08f/hankaku.txt)
set(11_day_harib08f_FONTB ${root_BINARY_DIR}/projects/11_day/harib08f/hankaku.bin)
set(11_day_harib08f_FONTO ${root_BINARY_DIR}/projects/11_day/harib08f/hankaku.o)
set(11_day_harib08f_LIBGE ${root_BINARY_DIR}/golibc/libgo.a)
set(11_day_harib08f_LIBGC ${root_BINARY_DIR}/projects/11_day/harib08f/libgo.o)

add_custom_target(11_day_harib08f_run
  COMMAND ${QEMU} ${QEMUOPT} ${11_day_harib08f_OS}
  DEPENDS 11_day_harib08f_img
)
add_custom_target(11_day_harib08f_debug
  COMMAND ${QEMU} -s -S ${QEMUOPT} ${11_day_harib08f_OS} ${QEMU_DEBUG_OPT}
  DEPENDS 11_day_harib08f_img
)
add_custom_target(11_day_harib08f_clean
  COMMAND rm -f ${11_day_harib08f_OS}
  COMMAND rm -f ${11_day_harib08f_SYS}
  COMMAND rm -f ${11_day_harib08f_IPLB}
  COMMAND rm -f ${11_day_harib08f_HEADB}
  COMMAND rm -f ${11_day_harib08f_BOOTB}
  COMMAND rm -f ${11_day_harib08f_WILDOBJ}
  COMMAND rm -f ${root_BINARY_DIR}/projects/11_day/harib08f/ipl10.bin
  COMMAND rm -f ${root_BINARY_DIR}/projects/11_day/harib08f/asmhead.bin
  COMMAND rm -f ${root_BINARY_DIR}/projects/11_day/harib08f/*_wine.bin
  COMMAND rm -f ${root_BINARY_DIR}/projects/11_day/harib08f/*.txt
)
add_custom_target(11_day_harib08f_ipl_build
  COMMAND ${RAKUSK_EXECUTABLE} ${11_day_harib08f_IPLS} ${11_day_harib08f_IPLB}
  DEPENDS ${11_day_harib08f_IPLS}
)
add_custom_command(
  OUTPUT ${11_day_harib08f_HEADB}
  COMMAND ${RAKUSK_EXECUTABLE} ${11_day_harib08f_HEADS} ${11_day_harib08f_HEADB}
  DEPENDS ${11_day_harib08f_HEADS}
  COMMENT "Generating asmhead.bin for 11_day_harib08f"
)

add_custom_target(11_day_harib08f_sys
  COMMAND ${FONT} ${11_day_harib08f_FONTS} ${11_day_harib08f_FONTB}
  COMMAND ${B2O}  ${11_day_harib08f_FONTB} ${11_day_harib08f_FONTO} _hankaku
  COMMAND ${CONV} -fcoff32 -nu ${11_day_harib08f_LIBGE} ${11_day_harib08f_LIBGC}
  COMMAND ${RAKUSK_EXECUTABLE} ${11_day_harib08f_FUNCS} ${11_day_harib08f_FUNCO}
  COMMAND gcc ${BINOPT} -T ${11_day_harib08f_LDS} ${11_day_harib08f_CCS} ${11_day_harib08f_WILDOBJ} -o ${11_day_harib08f_BOOTB}
  COMMAND cat ${11_day_harib08f_HEADB} ${11_day_harib08f_BOOTB} > ${11_day_harib08f_SYS}
  DEPENDS ${11_day_harib08f_HEADB} 11_day_harib08f_ipl_build
  DEPENDS ${11_day_harib08f_FUNCO}
  DEPENDS ${11_day_harib08f_FONTO}
)
add_custom_target(11_day_harib08f_img
  COMMAND mformat -f 1440 -l HARIBOTEOS -N 0xffffffff -C -B ${11_day_harib08f_IPLB} -i ${11_day_harib08f_OS}
  COMMAND mcopy -i ${11_day_harib08f_OS} ${11_day_harib08f_SYS} ::
  DEPENDS 11_day_harib08f_sys
)

########### next target ###############
set(11_day_harib08f_asmhead_SRCS ${root_SOURCE_DIR}/projects/11_day/harib08f/asmhead.nas)
set(11_day_harib08f_asmhead_OUTS ${root_BINARY_DIR}/projects/11_day/harib08f/asmhead.bin)

add_custom_target(11_day_harib08f_asmhead
  COMMAND ${RAKUSK_EXECUTABLE} ${11_day_harib08f_asmhead_SRCS} ${11_day_harib08f_asmhead_OUTS}
  DEPENDS ${11_day_harib08f_asmhead_SRCS}
)

set(11_day_harib08f_asmhead_WINE_OUTS ${root_BINARY_DIR}/projects/11_day/harib08f/asmhead_wine.bin)
add_custom_target(11_day_harib08f_asmhead_wine
  COMMAND ${WINE} ${WINE_NASK} ${11_day_harib08f_asmhead_SRCS} ${11_day_harib08f_asmhead_WINE_OUTS}
)
add_dependencies(wine 11_day_harib08f_asmhead_wine)

add_custom_target(11_day_harib08f_asmhead_od
  COMMAND ${OD} -t x1 ${11_day_harib08f_asmhead_OUTS}      > ${11_day_harib08f_asmhead_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${11_day_harib08f_asmhead_WINE_OUTS} > ${11_day_harib08f_asmhead_WINE_OUTS}_t.txt
  COMMAND diff -s ${11_day_harib08f_asmhead_OUTS}_f.txt ${11_day_harib08f_asmhead_WINE_OUTS}_t.txt
)

add_dependencies(11_day_harib08f_asmhead_od 11_day_harib08f_asmhead)
add_dependencies(11_day_harib08f_asmhead_od 11_day_harib08f_asmhead_wine)
add_dependencies(od 11_day_harib08f_asmhead_od)

#----------------------------------------------------------
########### next target ###############
set(11_day_harib08f_ipl10_SRCS ${root_SOURCE_DIR}/projects/11_day/harib08f/ipl10.nas)
set(11_day_harib08f_ipl10_OUTS ${root_BINARY_DIR}/projects/11_day/harib08f/ipl10.bin)

add_custom_target(11_day_harib08f_ipl10
  COMMAND ${RAKUSK_EXECUTABLE} ${11_day_harib08f_ipl10_SRCS} ${11_day_harib08f_ipl10_OUTS}
  DEPENDS ${11_day_harib08f_ipl10_SRCS}
)

set(11_day_harib08f_ipl10_WINE_OUTS ${root_BINARY_DIR}/projects/11_day/harib08f/ipl10_wine.bin)
add_custom_target(11_day_harib08f_ipl10_wine
  COMMAND ${WINE} ${WINE_NASK} ${11_day_harib08f_ipl10_SRCS} ${11_day_harib08f_ipl10_WINE_OUTS}
)
add_dependencies(wine 11_day_harib08f_ipl10_wine)

add_custom_target(11_day_harib08f_ipl10_od
  COMMAND ${OD} -t x1 ${11_day_harib08f_ipl10_OUTS}      > ${11_day_harib08f_ipl10_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${11_day_harib08f_ipl10_WINE_OUTS} > ${11_day_harib08f_ipl10_WINE_OUTS}_t.txt
  COMMAND diff -s ${11_day_harib08f_ipl10_OUTS}_f.txt ${11_day_harib08f_ipl10_WINE_OUTS}_t.txt
)

add_dependencies(11_day_harib08f_ipl10_od 11_day_harib08f_ipl10)
add_dependencies(11_day_harib08f_ipl10_od 11_day_harib08f_ipl10_wine)
add_dependencies(od 11_day_harib08f_ipl10_od)

#----------------------------------------------------------
########### next target ###############
set(11_day_harib08f_naskfunc_SRCS ${root_SOURCE_DIR}/projects/11_day/harib08f/naskfunc.nas)
set(11_day_harib08f_naskfunc_OUTS ${root_BINARY_DIR}/projects/11_day/harib08f/naskfunc.img)

add_custom_target(11_day_harib08f_naskfunc
  COMMAND ${RAKUSK_EXECUTABLE} ${11_day_harib08f_naskfunc_SRCS} ${11_day_harib08f_naskfunc_OUTS}
  DEPENDS ${11_day_harib08f_naskfunc_SRCS}
)

add_dependencies(images 11_day_harib08f_naskfunc)
set(11_day_harib08f_naskfunc_WINE_OUTS ${root_BINARY_DIR}/projects/11_day/harib08f/naskfunc_wine.img)
add_custom_target(11_day_harib08f_naskfunc_wine
  COMMAND ${WINE} ${WINE_NASK} ${11_day_harib08f_naskfunc_SRCS} ${11_day_harib08f_naskfunc_WINE_OUTS}
)
add_dependencies(wine 11_day_harib08f_naskfunc_wine)

add_custom_target(11_day_harib08f_naskfunc_od
  COMMAND ${OD} -t x1 ${11_day_harib08f_naskfunc_OUTS}      > ${11_day_harib08f_naskfunc_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${11_day_harib08f_naskfunc_WINE_OUTS} > ${11_day_harib08f_naskfunc_WINE_OUTS}_t.txt
  COMMAND diff -s ${11_day_harib08f_naskfunc_OUTS}_f.txt ${11_day_harib08f_naskfunc_WINE_OUTS}_t.txt
)

add_dependencies(11_day_harib08f_naskfunc_od 11_day_harib08f_naskfunc)
add_dependencies(11_day_harib08f_naskfunc_od 11_day_harib08f_naskfunc_wine)
add_dependencies(od 11_day_harib08f_naskfunc_od)

#----------------------------------------------------------
