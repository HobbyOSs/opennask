#----------------------------------------------------------
message(STATUS "Entering directory projects/20_day/harib17e/")

set(FONT ${root_BINARY_DIR}/src/makefont)
set(B2O  ${root_BINARY_DIR}/src/bin2obj)
set(CONV ${root_BINARY_DIR}/objconv/objconv)
set(20_day_harib17e_WILDOBJ ${root_BINARY_DIR}/projects/20_day/harib17e/*.o)

# Full OS build rules
set(20_day_harib17e_OS    ${root_BINARY_DIR}/projects/20_day/harib17e/os.img)
set(20_day_harib17e_SYS   ${root_BINARY_DIR}/projects/20_day/harib17e/os.sys)
set(20_day_harib17e_IPLB  ${root_BINARY_DIR}/projects/20_day/harib17e/ipl.bin)
set(20_day_harib17e_IPLS  ${root_SOURCE_DIR}/projects/20_day/harib17e/ipl10.nas)
set(20_day_harib17e_HEADB ${root_BINARY_DIR}/projects/20_day/harib17e/asmhead.bin)
set(20_day_harib17e_HEADS ${root_SOURCE_DIR}/projects/20_day/harib17e/asmhead.nas)
set(20_day_harib17e_BOOTB ${root_BINARY_DIR}/projects/20_day/harib17e/boot.bin)
set(20_day_harib17e_CCO   ${root_BINARY_DIR}/projects/20_day/harib17e/boot.o)
set(20_day_harib17e_CCS   ${root_SOURCE_DIR}/projects/20_day/harib17e/*.c)
set(20_day_harib17e_LDS   ${root_SOURCE_DIR}/projects/os.lds)
set(20_day_harib17e_FUNCS ${root_SOURCE_DIR}/projects/20_day/harib17e/naskfunc.nas)
set(20_day_harib17e_FUNCO ${root_BINARY_DIR}/projects/20_day/harib17e/naskfunc.o)
set(20_day_harib17e_FONTS ${root_SOURCE_DIR}/projects/20_day/harib17e/hankaku.txt)
set(20_day_harib17e_FONTB ${root_BINARY_DIR}/projects/20_day/harib17e/hankaku.bin)
set(20_day_harib17e_FONTO ${root_BINARY_DIR}/projects/20_day/harib17e/hankaku.o)
set(20_day_harib17e_LIBGE ${root_BINARY_DIR}/golibc/libgo.a)
set(20_day_harib17e_LIBGC ${root_BINARY_DIR}/projects/20_day/harib17e/libgo.o)

add_custom_target(20_day_harib17e_run
  COMMAND ${QEMU} ${QEMUOPT} ${20_day_harib17e_OS}
  DEPENDS 20_day_harib17e_img
)
add_custom_target(20_day_harib17e_debug
  COMMAND ${QEMU} -s -S ${QEMUOPT} ${20_day_harib17e_OS} ${QEMU_DEBUG_OPT}
  DEPENDS 20_day_harib17e_img
)
add_custom_target(20_day_harib17e_clean
  COMMAND rm -f ${20_day_harib17e_OS}
  COMMAND rm -f ${20_day_harib17e_SYS}
  COMMAND rm -f ${20_day_harib17e_IPLB}
  COMMAND rm -f ${20_day_harib17e_HEADB}
  COMMAND rm -f ${20_day_harib17e_BOOTB}
  COMMAND rm -f ${20_day_harib17e_WILDOBJ}
  COMMAND rm -f ${root_BINARY_DIR}/projects/20_day/harib17e/ipl10.bin
  COMMAND rm -f ${root_BINARY_DIR}/projects/20_day/harib17e/asmhead.bin
  COMMAND rm -f ${root_BINARY_DIR}/projects/20_day/harib17e/*_wine.bin
  COMMAND rm -f ${root_BINARY_DIR}/projects/20_day/harib17e/*.txt
)
add_custom_target(20_day_harib17e_ipl_build
  COMMAND ${RAKUSK_EXECUTABLE} ${20_day_harib17e_IPLS} ${20_day_harib17e_IPLB}
  DEPENDS ${20_day_harib17e_IPLS}
)
add_custom_command(
  OUTPUT ${20_day_harib17e_HEADB}
  COMMAND ${RAKUSK_EXECUTABLE} ${20_day_harib17e_HEADS} ${20_day_harib17e_HEADB}
  DEPENDS ${20_day_harib17e_HEADS}
  COMMENT "Generating asmhead.bin for 20_day_harib17e"
)

add_custom_command(
  OUTPUT ${20_day_harib17e_FUNCO}
  COMMAND ${RAKUSK_EXECUTABLE} ${20_day_harib17e_FUNCS} ${20_day_harib17e_FUNCO}
  DEPENDS ${20_day_harib17e_FUNCS}
  COMMENT "Generating naskfunc.o for 20_day_harib17e"
)
add_custom_command(
  OUTPUT ${20_day_harib17e_FONTO}
  COMMAND ${FONT} ${20_day_harib17e_FONTS} ${20_day_harib17e_FONTB}
  COMMAND ${B2O}  ${20_day_harib17e_FONTB} ${20_day_harib17e_FONTO} _hankaku
  DEPENDS ${20_day_harib17e_FONTS}
  COMMENT "Generating hankaku.o for 20_day_harib17e"
)
add_custom_command(
  OUTPUT ${20_day_harib17e_LIBGC}
  COMMAND ${CONV} -fcoff32 -nu ${20_day_harib17e_LIBGE} ${20_day_harib17e_LIBGC}
  DEPENDS ${20_day_harib17e_LIBGE}
  COMMENT "Converting golibc for 20_day_harib17e"
)
add_custom_target(20_day_harib17e_sys
  COMMAND gcc ${BINOPT} -T ${20_day_harib17e_LDS} ${20_day_harib17e_CCS} ${20_day_harib17e_WILDOBJ} -o ${20_day_harib17e_BOOTB}
  COMMAND cat ${20_day_harib17e_HEADB} ${20_day_harib17e_BOOTB} > ${20_day_harib17e_SYS}
  DEPENDS ${20_day_harib17e_HEADB} 20_day_harib17e_ipl_build
  DEPENDS ${20_day_harib17e_FUNCO}
  DEPENDS ${20_day_harib17e_FONTO} ${20_day_harib17e_LIBGC}
)
add_custom_target(20_day_harib17e_img
  COMMAND mformat -f 1440 -l HARIBOTEOS -N 0xffffffff -C -B ${20_day_harib17e_IPLB} -i ${20_day_harib17e_OS}
  COMMAND mcopy -i ${20_day_harib17e_OS} ${20_day_harib17e_SYS} ::
  DEPENDS 20_day_harib17e_sys
)

########### next target ###############
set(20_day_harib17e_asmhead_SRCS ${root_SOURCE_DIR}/projects/20_day/harib17e/asmhead.nas)
set(20_day_harib17e_asmhead_OUTS ${root_BINARY_DIR}/projects/20_day/harib17e/asmhead.bin)

add_custom_target(20_day_harib17e_asmhead
  COMMAND ${RAKUSK_EXECUTABLE} ${20_day_harib17e_asmhead_SRCS} ${20_day_harib17e_asmhead_OUTS}
  DEPENDS ${20_day_harib17e_asmhead_SRCS}
)

set(20_day_harib17e_asmhead_WINE_OUTS ${root_BINARY_DIR}/projects/20_day/harib17e/asmhead_wine.bin)
add_custom_target(20_day_harib17e_asmhead_wine
  COMMAND ${WINE} ${WINE_NASK} ${20_day_harib17e_asmhead_SRCS} ${20_day_harib17e_asmhead_WINE_OUTS}
)
add_dependencies(wine 20_day_harib17e_asmhead_wine)

add_custom_target(20_day_harib17e_asmhead_od
  COMMAND ${OD} -t x1 ${20_day_harib17e_asmhead_OUTS}      > ${20_day_harib17e_asmhead_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${20_day_harib17e_asmhead_WINE_OUTS} > ${20_day_harib17e_asmhead_WINE_OUTS}_t.txt
  COMMAND diff -s ${20_day_harib17e_asmhead_OUTS}_f.txt ${20_day_harib17e_asmhead_WINE_OUTS}_t.txt
)

add_dependencies(20_day_harib17e_asmhead_od 20_day_harib17e_asmhead)
add_dependencies(20_day_harib17e_asmhead_od 20_day_harib17e_asmhead_wine)
add_dependencies(od 20_day_harib17e_asmhead_od)

#----------------------------------------------------------
########### next target ###############
set(20_day_harib17e_hlt_SRCS ${root_SOURCE_DIR}/projects/20_day/harib17e/hlt.nas)
set(20_day_harib17e_hlt_OUTS ${root_BINARY_DIR}/projects/20_day/harib17e/hlt.img)

add_custom_target(20_day_harib17e_hlt
  COMMAND ${RAKUSK_EXECUTABLE} ${20_day_harib17e_hlt_SRCS} ${20_day_harib17e_hlt_OUTS}
  DEPENDS ${20_day_harib17e_hlt_SRCS}
)

add_dependencies(images 20_day_harib17e_hlt)
set(20_day_harib17e_hlt_WINE_OUTS ${root_BINARY_DIR}/projects/20_day/harib17e/hlt_wine.img)
add_custom_target(20_day_harib17e_hlt_wine
  COMMAND ${WINE} ${WINE_NASK} ${20_day_harib17e_hlt_SRCS} ${20_day_harib17e_hlt_WINE_OUTS}
)
add_dependencies(wine 20_day_harib17e_hlt_wine)

add_custom_target(20_day_harib17e_hlt_od
  COMMAND ${OD} -t x1 ${20_day_harib17e_hlt_OUTS}      > ${20_day_harib17e_hlt_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${20_day_harib17e_hlt_WINE_OUTS} > ${20_day_harib17e_hlt_WINE_OUTS}_t.txt
  COMMAND diff -s ${20_day_harib17e_hlt_OUTS}_f.txt ${20_day_harib17e_hlt_WINE_OUTS}_t.txt
)

add_dependencies(20_day_harib17e_hlt_od 20_day_harib17e_hlt)
add_dependencies(20_day_harib17e_hlt_od 20_day_harib17e_hlt_wine)
add_dependencies(od 20_day_harib17e_hlt_od)

set(20_day_harib17e_hlt_HRB ${root_BINARY_DIR}/projects/20_day/harib17e/hlt.hrb)
add_custom_target(20_day_harib17e_hlt_hrb
  COMMAND ${RAKUSK_EXECUTABLE} ${20_day_harib17e_hlt_SRCS} ${20_day_harib17e_hlt_HRB}
  COMMAND mcopy -i ${20_day_harib17e_OS} ${20_day_harib17e_hlt_HRB} ::hlt.hrb
  DEPENDS 20_day_harib17e_img
)

add_dependencies(20_day_harib17e_run 20_day_harib17e_hlt_hrb)
#----------------------------------------------------------
########### next target ###############
set(20_day_harib17e_ipl10_SRCS ${root_SOURCE_DIR}/projects/20_day/harib17e/ipl10.nas)
set(20_day_harib17e_ipl10_OUTS ${root_BINARY_DIR}/projects/20_day/harib17e/ipl10.bin)

add_custom_target(20_day_harib17e_ipl10
  COMMAND ${RAKUSK_EXECUTABLE} ${20_day_harib17e_ipl10_SRCS} ${20_day_harib17e_ipl10_OUTS}
  DEPENDS ${20_day_harib17e_ipl10_SRCS}
)

set(20_day_harib17e_ipl10_WINE_OUTS ${root_BINARY_DIR}/projects/20_day/harib17e/ipl10_wine.bin)
add_custom_target(20_day_harib17e_ipl10_wine
  COMMAND ${WINE} ${WINE_NASK} ${20_day_harib17e_ipl10_SRCS} ${20_day_harib17e_ipl10_WINE_OUTS}
)
add_dependencies(wine 20_day_harib17e_ipl10_wine)

add_custom_target(20_day_harib17e_ipl10_od
  COMMAND ${OD} -t x1 ${20_day_harib17e_ipl10_OUTS}      > ${20_day_harib17e_ipl10_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${20_day_harib17e_ipl10_WINE_OUTS} > ${20_day_harib17e_ipl10_WINE_OUTS}_t.txt
  COMMAND diff -s ${20_day_harib17e_ipl10_OUTS}_f.txt ${20_day_harib17e_ipl10_WINE_OUTS}_t.txt
)

add_dependencies(20_day_harib17e_ipl10_od 20_day_harib17e_ipl10)
add_dependencies(20_day_harib17e_ipl10_od 20_day_harib17e_ipl10_wine)
add_dependencies(od 20_day_harib17e_ipl10_od)

#----------------------------------------------------------
########### next target ###############
set(20_day_harib17e_naskfunc_SRCS ${root_SOURCE_DIR}/projects/20_day/harib17e/naskfunc.nas)
set(20_day_harib17e_naskfunc_OUTS ${root_BINARY_DIR}/projects/20_day/harib17e/naskfunc.img)

add_custom_target(20_day_harib17e_naskfunc
  COMMAND ${RAKUSK_EXECUTABLE} ${20_day_harib17e_naskfunc_SRCS} ${20_day_harib17e_naskfunc_OUTS}
  DEPENDS ${20_day_harib17e_naskfunc_SRCS}
)

add_dependencies(images 20_day_harib17e_naskfunc)
set(20_day_harib17e_naskfunc_WINE_OUTS ${root_BINARY_DIR}/projects/20_day/harib17e/naskfunc_wine.img)
add_custom_target(20_day_harib17e_naskfunc_wine
  COMMAND ${WINE} ${WINE_NASK} ${20_day_harib17e_naskfunc_SRCS} ${20_day_harib17e_naskfunc_WINE_OUTS}
)
add_dependencies(wine 20_day_harib17e_naskfunc_wine)

add_custom_target(20_day_harib17e_naskfunc_od
  COMMAND ${OD} -t x1 ${20_day_harib17e_naskfunc_OUTS}      > ${20_day_harib17e_naskfunc_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${20_day_harib17e_naskfunc_WINE_OUTS} > ${20_day_harib17e_naskfunc_WINE_OUTS}_t.txt
  COMMAND diff -s ${20_day_harib17e_naskfunc_OUTS}_f.txt ${20_day_harib17e_naskfunc_WINE_OUTS}_t.txt
)

add_dependencies(20_day_harib17e_naskfunc_od 20_day_harib17e_naskfunc)
add_dependencies(20_day_harib17e_naskfunc_od 20_day_harib17e_naskfunc_wine)
add_dependencies(od 20_day_harib17e_naskfunc_od)

#----------------------------------------------------------
