#----------------------------------------------------------
message(STATUS "Entering directory projects/20_day/harib17a/")

set(FONT ${RAKUSK_EXECUTABLE} --makefont)
set(B2O  ${RAKUSK_EXECUTABLE} --bin2obj)
set(CONV ${root_BINARY_DIR}/objconv/objconv)
set(20_day_harib17a_WILDOBJ ${root_BINARY_DIR}/projects/20_day/harib17a/*.o)

# Full OS build rules
set(20_day_harib17a_OS    ${root_BINARY_DIR}/projects/20_day/harib17a/os.img)
set(20_day_harib17a_SYS   ${root_BINARY_DIR}/projects/20_day/harib17a/os.sys)
set(20_day_harib17a_IPLB  ${root_BINARY_DIR}/projects/20_day/harib17a/ipl.bin)
set(20_day_harib17a_IPLS  ${root_SOURCE_DIR}/projects/20_day/harib17a/ipl10.nas)
set(20_day_harib17a_HEADB ${root_BINARY_DIR}/projects/20_day/harib17a/asmhead.bin)
set(20_day_harib17a_HEADS ${root_SOURCE_DIR}/projects/20_day/harib17a/asmhead.nas)
set(20_day_harib17a_BOOTB ${root_BINARY_DIR}/projects/20_day/harib17a/boot.bin)
set(20_day_harib17a_CCO   ${root_BINARY_DIR}/projects/20_day/harib17a/boot.o)
set(20_day_harib17a_CCS   ${root_SOURCE_DIR}/projects/20_day/harib17a/*.c)
set(20_day_harib17a_LDS   ${root_SOURCE_DIR}/projects/os.lds)
set(20_day_harib17a_FUNCS ${root_SOURCE_DIR}/projects/20_day/harib17a/naskfunc.nas)
set(20_day_harib17a_FUNCO ${root_BINARY_DIR}/projects/20_day/harib17a/naskfunc.o)
set(20_day_harib17a_FONTS ${root_SOURCE_DIR}/projects/20_day/harib17a/hankaku.txt)
set(20_day_harib17a_FONTB ${root_BINARY_DIR}/projects/20_day/harib17a/hankaku.bin)
set(20_day_harib17a_FONTO ${root_BINARY_DIR}/projects/20_day/harib17a/hankaku.o)
set(20_day_harib17a_LIBGE ${root_BINARY_DIR}/golibc/libgo.a)
set(20_day_harib17a_LIBGC ${root_BINARY_DIR}/projects/20_day/harib17a/libgo.o)

add_custom_target(20_day_harib17a_run
  COMMAND ${QEMU} ${QEMUOPT} ${20_day_harib17a_OS}
  DEPENDS 20_day_harib17a_img
)
add_custom_target(20_day_harib17a_debug
  COMMAND ${QEMU} -s -S ${QEMUOPT} ${20_day_harib17a_OS} ${QEMU_DEBUG_OPT}
  DEPENDS 20_day_harib17a_img
)
add_custom_target(20_day_harib17a_clean
  COMMAND rm -f ${20_day_harib17a_OS}
  COMMAND rm -f ${20_day_harib17a_SYS}
  COMMAND rm -f ${20_day_harib17a_IPLB}
  COMMAND rm -f ${20_day_harib17a_HEADB}
  COMMAND rm -f ${20_day_harib17a_BOOTB}
  COMMAND rm -f ${20_day_harib17a_WILDOBJ}
  COMMAND rm -f ${root_BINARY_DIR}/projects/20_day/harib17a/ipl10.bin
  COMMAND rm -f ${root_BINARY_DIR}/projects/20_day/harib17a/asmhead.bin
  COMMAND rm -f ${root_BINARY_DIR}/projects/20_day/harib17a/*_wine.bin
  COMMAND rm -f ${root_BINARY_DIR}/projects/20_day/harib17a/*.txt
)
add_custom_target(20_day_harib17a_ipl_build
  COMMAND ${RAKUSK_EXECUTABLE} ${20_day_harib17a_IPLS} ${20_day_harib17a_IPLB}
  DEPENDS ${20_day_harib17a_IPLS}
)
add_custom_command(
  OUTPUT ${20_day_harib17a_HEADB}
  COMMAND ${RAKUSK_EXECUTABLE} ${20_day_harib17a_HEADS} ${20_day_harib17a_HEADB}
  DEPENDS ${20_day_harib17a_HEADS}
  COMMENT "Generating asmhead.bin for 20_day_harib17a"
)

add_custom_target(20_day_harib17a_sys
  COMMAND ${FONT} ${20_day_harib17a_FONTS} ${20_day_harib17a_FONTB}
  COMMAND ${B2O}  ${20_day_harib17a_FONTB} ${20_day_harib17a_FONTO} _hankaku
  COMMAND ${CONV} -fcoff32 -nu ${20_day_harib17a_LIBGE} ${20_day_harib17a_LIBGC}
  COMMAND ${RAKUSK_EXECUTABLE} ${20_day_harib17a_FUNCS} ${20_day_harib17a_FUNCO}
  COMMAND gcc ${BINOPT} -T ${20_day_harib17a_LDS} ${20_day_harib17a_CCS} ${20_day_harib17a_WILDOBJ} -o ${20_day_harib17a_BOOTB}
  COMMAND cat ${20_day_harib17a_HEADB} ${20_day_harib17a_BOOTB} > ${20_day_harib17a_SYS}
  DEPENDS ${20_day_harib17a_HEADB} 20_day_harib17a_ipl_build
  DEPENDS ${20_day_harib17a_FUNCO}
  DEPENDS ${20_day_harib17a_FONTO}
)
add_custom_target(20_day_harib17a_img
  COMMAND mformat -f 1440 -l HARIBOTEOS -N 0xffffffff -C -B ${20_day_harib17a_IPLB} -i ${20_day_harib17a_OS}
  COMMAND mcopy -i ${20_day_harib17a_OS} ${20_day_harib17a_SYS} ::
  DEPENDS 20_day_harib17a_sys
)

########### next target ###############
set(20_day_harib17a_asmhead_SRCS ${root_SOURCE_DIR}/projects/20_day/harib17a/asmhead.nas)
set(20_day_harib17a_asmhead_OUTS ${root_BINARY_DIR}/projects/20_day/harib17a/asmhead.bin)

add_custom_target(20_day_harib17a_asmhead
  COMMAND ${RAKUSK_EXECUTABLE} ${20_day_harib17a_asmhead_SRCS} ${20_day_harib17a_asmhead_OUTS}
  DEPENDS ${20_day_harib17a_asmhead_SRCS}
)

set(20_day_harib17a_asmhead_WINE_OUTS ${root_BINARY_DIR}/projects/20_day/harib17a/asmhead_wine.bin)
add_custom_target(20_day_harib17a_asmhead_wine
  COMMAND ${WINE} ${WINE_NASK} ${20_day_harib17a_asmhead_SRCS} ${20_day_harib17a_asmhead_WINE_OUTS}
)
add_dependencies(wine 20_day_harib17a_asmhead_wine)

add_custom_target(20_day_harib17a_asmhead_od
  COMMAND ${OD} -t x1 ${20_day_harib17a_asmhead_OUTS}      > ${20_day_harib17a_asmhead_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${20_day_harib17a_asmhead_WINE_OUTS} > ${20_day_harib17a_asmhead_WINE_OUTS}_t.txt
  COMMAND diff -s ${20_day_harib17a_asmhead_OUTS}_f.txt ${20_day_harib17a_asmhead_WINE_OUTS}_t.txt
)

add_dependencies(20_day_harib17a_asmhead_od 20_day_harib17a_asmhead)
add_dependencies(20_day_harib17a_asmhead_od 20_day_harib17a_asmhead_wine)
add_dependencies(od 20_day_harib17a_asmhead_od)

#----------------------------------------------------------
########### next target ###############
set(20_day_harib17a_hlt_SRCS ${root_SOURCE_DIR}/projects/20_day/harib17a/hlt.nas)
set(20_day_harib17a_hlt_OUTS ${root_BINARY_DIR}/projects/20_day/harib17a/hlt.img)

add_custom_target(20_day_harib17a_hlt
  COMMAND ${RAKUSK_EXECUTABLE} ${20_day_harib17a_hlt_SRCS} ${20_day_harib17a_hlt_OUTS}
  DEPENDS ${20_day_harib17a_hlt_SRCS}
)

add_dependencies(images 20_day_harib17a_hlt)
set(20_day_harib17a_hlt_WINE_OUTS ${root_BINARY_DIR}/projects/20_day/harib17a/hlt_wine.img)
add_custom_target(20_day_harib17a_hlt_wine
  COMMAND ${WINE} ${WINE_NASK} ${20_day_harib17a_hlt_SRCS} ${20_day_harib17a_hlt_WINE_OUTS}
)
add_dependencies(wine 20_day_harib17a_hlt_wine)

add_custom_target(20_day_harib17a_hlt_od
  COMMAND ${OD} -t x1 ${20_day_harib17a_hlt_OUTS}      > ${20_day_harib17a_hlt_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${20_day_harib17a_hlt_WINE_OUTS} > ${20_day_harib17a_hlt_WINE_OUTS}_t.txt
  COMMAND diff -s ${20_day_harib17a_hlt_OUTS}_f.txt ${20_day_harib17a_hlt_WINE_OUTS}_t.txt
)

add_dependencies(20_day_harib17a_hlt_od 20_day_harib17a_hlt)
add_dependencies(20_day_harib17a_hlt_od 20_day_harib17a_hlt_wine)
add_dependencies(od 20_day_harib17a_hlt_od)

set(20_day_harib17a_hlt_HRB ${root_BINARY_DIR}/projects/20_day/harib17a/hlt.hrb)
add_custom_target(20_day_harib17a_hlt_hrb
  COMMAND ${RAKUSK_EXECUTABLE} ${20_day_harib17a_hlt_SRCS} ${20_day_harib17a_hlt_HRB}
  COMMAND mcopy -i ${20_day_harib17a_OS} ${20_day_harib17a_hlt_HRB} ::hlt.hrb
  DEPENDS 20_day_harib17a_img
)

add_dependencies(20_day_harib17a_run 20_day_harib17a_hlt_hrb)
#----------------------------------------------------------
########### next target ###############
set(20_day_harib17a_ipl10_SRCS ${root_SOURCE_DIR}/projects/20_day/harib17a/ipl10.nas)
set(20_day_harib17a_ipl10_OUTS ${root_BINARY_DIR}/projects/20_day/harib17a/ipl10.bin)

add_custom_target(20_day_harib17a_ipl10
  COMMAND ${RAKUSK_EXECUTABLE} ${20_day_harib17a_ipl10_SRCS} ${20_day_harib17a_ipl10_OUTS}
  DEPENDS ${20_day_harib17a_ipl10_SRCS}
)

set(20_day_harib17a_ipl10_WINE_OUTS ${root_BINARY_DIR}/projects/20_day/harib17a/ipl10_wine.bin)
add_custom_target(20_day_harib17a_ipl10_wine
  COMMAND ${WINE} ${WINE_NASK} ${20_day_harib17a_ipl10_SRCS} ${20_day_harib17a_ipl10_WINE_OUTS}
)
add_dependencies(wine 20_day_harib17a_ipl10_wine)

add_custom_target(20_day_harib17a_ipl10_od
  COMMAND ${OD} -t x1 ${20_day_harib17a_ipl10_OUTS}      > ${20_day_harib17a_ipl10_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${20_day_harib17a_ipl10_WINE_OUTS} > ${20_day_harib17a_ipl10_WINE_OUTS}_t.txt
  COMMAND diff -s ${20_day_harib17a_ipl10_OUTS}_f.txt ${20_day_harib17a_ipl10_WINE_OUTS}_t.txt
)

add_dependencies(20_day_harib17a_ipl10_od 20_day_harib17a_ipl10)
add_dependencies(20_day_harib17a_ipl10_od 20_day_harib17a_ipl10_wine)
add_dependencies(od 20_day_harib17a_ipl10_od)

#----------------------------------------------------------
########### next target ###############
set(20_day_harib17a_naskfunc_SRCS ${root_SOURCE_DIR}/projects/20_day/harib17a/naskfunc.nas)
set(20_day_harib17a_naskfunc_OUTS ${root_BINARY_DIR}/projects/20_day/harib17a/naskfunc.img)

add_custom_target(20_day_harib17a_naskfunc
  COMMAND ${RAKUSK_EXECUTABLE} ${20_day_harib17a_naskfunc_SRCS} ${20_day_harib17a_naskfunc_OUTS}
  DEPENDS ${20_day_harib17a_naskfunc_SRCS}
)

add_dependencies(images 20_day_harib17a_naskfunc)
set(20_day_harib17a_naskfunc_WINE_OUTS ${root_BINARY_DIR}/projects/20_day/harib17a/naskfunc_wine.img)
add_custom_target(20_day_harib17a_naskfunc_wine
  COMMAND ${WINE} ${WINE_NASK} ${20_day_harib17a_naskfunc_SRCS} ${20_day_harib17a_naskfunc_WINE_OUTS}
)
add_dependencies(wine 20_day_harib17a_naskfunc_wine)

add_custom_target(20_day_harib17a_naskfunc_od
  COMMAND ${OD} -t x1 ${20_day_harib17a_naskfunc_OUTS}      > ${20_day_harib17a_naskfunc_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${20_day_harib17a_naskfunc_WINE_OUTS} > ${20_day_harib17a_naskfunc_WINE_OUTS}_t.txt
  COMMAND diff -s ${20_day_harib17a_naskfunc_OUTS}_f.txt ${20_day_harib17a_naskfunc_WINE_OUTS}_t.txt
)

add_dependencies(20_day_harib17a_naskfunc_od 20_day_harib17a_naskfunc)
add_dependencies(20_day_harib17a_naskfunc_od 20_day_harib17a_naskfunc_wine)
add_dependencies(od 20_day_harib17a_naskfunc_od)

#----------------------------------------------------------
