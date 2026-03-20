#----------------------------------------------------------
message(STATUS "Entering directory projects/20_day/harib17d/")

set(FONT ${RAKUSK_EXECUTABLE} --makefont)
set(B2O  ${RAKUSK_EXECUTABLE} --bin2obj)
set(CONV ${root_BINARY_DIR}/objconv/objconv)
set(20_day_harib17d_WILDOBJ ${root_BINARY_DIR}/projects/20_day/harib17d/*.o)

# Full OS build rules
set(20_day_harib17d_OS    ${root_BINARY_DIR}/projects/20_day/harib17d/os.img)
set(20_day_harib17d_SYS   ${root_BINARY_DIR}/projects/20_day/harib17d/os.sys)
set(20_day_harib17d_IPLB  ${root_BINARY_DIR}/projects/20_day/harib17d/ipl.bin)
set(20_day_harib17d_IPLS  ${root_SOURCE_DIR}/projects/20_day/harib17d/ipl10.nas)
set(20_day_harib17d_HEADB ${root_BINARY_DIR}/projects/20_day/harib17d/asmhead.bin)
set(20_day_harib17d_HEADS ${root_SOURCE_DIR}/projects/20_day/harib17d/asmhead.nas)
set(20_day_harib17d_BOOTB ${root_BINARY_DIR}/projects/20_day/harib17d/boot.bin)
set(20_day_harib17d_CCO   ${root_BINARY_DIR}/projects/20_day/harib17d/boot.o)
set(20_day_harib17d_CCS   ${root_SOURCE_DIR}/projects/20_day/harib17d/*.c)
set(20_day_harib17d_LDS   ${root_SOURCE_DIR}/projects/os.lds)
set(20_day_harib17d_FUNCS ${root_SOURCE_DIR}/projects/20_day/harib17d/naskfunc.nas)
set(20_day_harib17d_FUNCO ${root_BINARY_DIR}/projects/20_day/harib17d/naskfunc.o)
set(20_day_harib17d_FONTS ${root_SOURCE_DIR}/projects/20_day/harib17d/hankaku.txt)
set(20_day_harib17d_FONTB ${root_BINARY_DIR}/projects/20_day/harib17d/hankaku.bin)
set(20_day_harib17d_FONTO ${root_BINARY_DIR}/projects/20_day/harib17d/hankaku.o)
set(20_day_harib17d_LIBGE ${root_BINARY_DIR}/golibc/libgo.a)
set(20_day_harib17d_LIBGC ${root_BINARY_DIR}/projects/20_day/harib17d/libgo.o)

add_custom_target(20_day_harib17d_run
  COMMAND ${QEMU} ${QEMUOPT} ${20_day_harib17d_OS}
  DEPENDS 20_day_harib17d_img
)
add_custom_target(20_day_harib17d_debug
  COMMAND ${QEMU} -s -S ${QEMUOPT} ${20_day_harib17d_OS} ${QEMU_DEBUG_OPT}
  DEPENDS 20_day_harib17d_img
)
add_custom_target(20_day_harib17d_clean
  COMMAND rm -f ${20_day_harib17d_OS}
  COMMAND rm -f ${20_day_harib17d_SYS}
  COMMAND rm -f ${20_day_harib17d_IPLB}
  COMMAND rm -f ${20_day_harib17d_HEADB}
  COMMAND rm -f ${20_day_harib17d_BOOTB}
  COMMAND rm -f ${20_day_harib17d_WILDOBJ}
  COMMAND rm -f ${root_BINARY_DIR}/projects/20_day/harib17d/ipl10.bin
  COMMAND rm -f ${root_BINARY_DIR}/projects/20_day/harib17d/asmhead.bin
  COMMAND rm -f ${root_BINARY_DIR}/projects/20_day/harib17d/*_wine.bin
  COMMAND rm -f ${root_BINARY_DIR}/projects/20_day/harib17d/*.txt
)
add_custom_target(20_day_harib17d_ipl_build
  COMMAND ${RAKUSK_EXECUTABLE} ${20_day_harib17d_IPLS} ${20_day_harib17d_IPLB}
  DEPENDS ${20_day_harib17d_IPLS}
)
add_custom_command(
  OUTPUT ${20_day_harib17d_HEADB}
  COMMAND ${RAKUSK_EXECUTABLE} ${20_day_harib17d_HEADS} ${20_day_harib17d_HEADB}
  DEPENDS ${20_day_harib17d_HEADS}
  COMMENT "Generating asmhead.bin for 20_day_harib17d"
)

add_custom_target(20_day_harib17d_sys
  COMMAND ${FONT} ${20_day_harib17d_FONTS} ${20_day_harib17d_FONTB}
  COMMAND ${B2O}  ${20_day_harib17d_FONTB} ${20_day_harib17d_FONTO} _hankaku
  COMMAND ${CONV} -fcoff32 -nu ${20_day_harib17d_LIBGE} ${20_day_harib17d_LIBGC}
  COMMAND ${RAKUSK_EXECUTABLE} ${20_day_harib17d_FUNCS} ${20_day_harib17d_FUNCO}
  COMMAND gcc ${BINOPT} -T ${20_day_harib17d_LDS} ${20_day_harib17d_CCS} ${20_day_harib17d_WILDOBJ} -o ${20_day_harib17d_BOOTB}
  COMMAND cat ${20_day_harib17d_HEADB} ${20_day_harib17d_BOOTB} > ${20_day_harib17d_SYS}
  DEPENDS ${20_day_harib17d_HEADB} 20_day_harib17d_ipl_build
  DEPENDS ${20_day_harib17d_FUNCO}
  DEPENDS ${20_day_harib17d_FONTO}
)
add_custom_target(20_day_harib17d_img
  COMMAND mformat -f 1440 -l HARIBOTEOS -N 0xffffffff -C -B ${20_day_harib17d_IPLB} -i ${20_day_harib17d_OS}
  COMMAND mcopy -i ${20_day_harib17d_OS} ${20_day_harib17d_SYS} ::
  DEPENDS 20_day_harib17d_sys
)

########### next target ###############
set(20_day_harib17d_asmhead_SRCS ${root_SOURCE_DIR}/projects/20_day/harib17d/asmhead.nas)
set(20_day_harib17d_asmhead_OUTS ${root_BINARY_DIR}/projects/20_day/harib17d/asmhead.bin)

add_custom_target(20_day_harib17d_asmhead
  COMMAND ${RAKUSK_EXECUTABLE} ${20_day_harib17d_asmhead_SRCS} ${20_day_harib17d_asmhead_OUTS}
  DEPENDS ${20_day_harib17d_asmhead_SRCS}
)

set(20_day_harib17d_asmhead_WINE_OUTS ${root_BINARY_DIR}/projects/20_day/harib17d/asmhead_wine.bin)
add_custom_target(20_day_harib17d_asmhead_wine
  COMMAND ${WINE} ${WINE_NASK} ${20_day_harib17d_asmhead_SRCS} ${20_day_harib17d_asmhead_WINE_OUTS}
)
add_dependencies(wine 20_day_harib17d_asmhead_wine)

add_custom_target(20_day_harib17d_asmhead_od
  COMMAND ${OD} -t x1 ${20_day_harib17d_asmhead_OUTS}      > ${20_day_harib17d_asmhead_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${20_day_harib17d_asmhead_WINE_OUTS} > ${20_day_harib17d_asmhead_WINE_OUTS}_t.txt
  COMMAND diff -s ${20_day_harib17d_asmhead_OUTS}_f.txt ${20_day_harib17d_asmhead_WINE_OUTS}_t.txt
)

add_dependencies(20_day_harib17d_asmhead_od 20_day_harib17d_asmhead)
add_dependencies(20_day_harib17d_asmhead_od 20_day_harib17d_asmhead_wine)
add_dependencies(od 20_day_harib17d_asmhead_od)

#----------------------------------------------------------
########### next target ###############
set(20_day_harib17d_hlt_SRCS ${root_SOURCE_DIR}/projects/20_day/harib17d/hlt.nas)
set(20_day_harib17d_hlt_OUTS ${root_BINARY_DIR}/projects/20_day/harib17d/hlt.img)

add_custom_target(20_day_harib17d_hlt
  COMMAND ${RAKUSK_EXECUTABLE} ${20_day_harib17d_hlt_SRCS} ${20_day_harib17d_hlt_OUTS}
  DEPENDS ${20_day_harib17d_hlt_SRCS}
)

add_dependencies(images 20_day_harib17d_hlt)
set(20_day_harib17d_hlt_WINE_OUTS ${root_BINARY_DIR}/projects/20_day/harib17d/hlt_wine.img)
add_custom_target(20_day_harib17d_hlt_wine
  COMMAND ${WINE} ${WINE_NASK} ${20_day_harib17d_hlt_SRCS} ${20_day_harib17d_hlt_WINE_OUTS}
)
add_dependencies(wine 20_day_harib17d_hlt_wine)

add_custom_target(20_day_harib17d_hlt_od
  COMMAND ${OD} -t x1 ${20_day_harib17d_hlt_OUTS}      > ${20_day_harib17d_hlt_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${20_day_harib17d_hlt_WINE_OUTS} > ${20_day_harib17d_hlt_WINE_OUTS}_t.txt
  COMMAND diff -s ${20_day_harib17d_hlt_OUTS}_f.txt ${20_day_harib17d_hlt_WINE_OUTS}_t.txt
)

add_dependencies(20_day_harib17d_hlt_od 20_day_harib17d_hlt)
add_dependencies(20_day_harib17d_hlt_od 20_day_harib17d_hlt_wine)
add_dependencies(od 20_day_harib17d_hlt_od)

set(20_day_harib17d_hlt_HRB ${root_BINARY_DIR}/projects/20_day/harib17d/hlt.hrb)
add_custom_target(20_day_harib17d_hlt_hrb
  COMMAND ${RAKUSK_EXECUTABLE} ${20_day_harib17d_hlt_SRCS} ${20_day_harib17d_hlt_HRB}
  COMMAND mcopy -i ${20_day_harib17d_OS} ${20_day_harib17d_hlt_HRB} ::hlt.hrb
  DEPENDS 20_day_harib17d_img
)

add_dependencies(20_day_harib17d_run 20_day_harib17d_hlt_hrb)
#----------------------------------------------------------
########### next target ###############
set(20_day_harib17d_ipl10_SRCS ${root_SOURCE_DIR}/projects/20_day/harib17d/ipl10.nas)
set(20_day_harib17d_ipl10_OUTS ${root_BINARY_DIR}/projects/20_day/harib17d/ipl10.bin)

add_custom_target(20_day_harib17d_ipl10
  COMMAND ${RAKUSK_EXECUTABLE} ${20_day_harib17d_ipl10_SRCS} ${20_day_harib17d_ipl10_OUTS}
  DEPENDS ${20_day_harib17d_ipl10_SRCS}
)

set(20_day_harib17d_ipl10_WINE_OUTS ${root_BINARY_DIR}/projects/20_day/harib17d/ipl10_wine.bin)
add_custom_target(20_day_harib17d_ipl10_wine
  COMMAND ${WINE} ${WINE_NASK} ${20_day_harib17d_ipl10_SRCS} ${20_day_harib17d_ipl10_WINE_OUTS}
)
add_dependencies(wine 20_day_harib17d_ipl10_wine)

add_custom_target(20_day_harib17d_ipl10_od
  COMMAND ${OD} -t x1 ${20_day_harib17d_ipl10_OUTS}      > ${20_day_harib17d_ipl10_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${20_day_harib17d_ipl10_WINE_OUTS} > ${20_day_harib17d_ipl10_WINE_OUTS}_t.txt
  COMMAND diff -s ${20_day_harib17d_ipl10_OUTS}_f.txt ${20_day_harib17d_ipl10_WINE_OUTS}_t.txt
)

add_dependencies(20_day_harib17d_ipl10_od 20_day_harib17d_ipl10)
add_dependencies(20_day_harib17d_ipl10_od 20_day_harib17d_ipl10_wine)
add_dependencies(od 20_day_harib17d_ipl10_od)

#----------------------------------------------------------
########### next target ###############
set(20_day_harib17d_naskfunc_SRCS ${root_SOURCE_DIR}/projects/20_day/harib17d/naskfunc.nas)
set(20_day_harib17d_naskfunc_OUTS ${root_BINARY_DIR}/projects/20_day/harib17d/naskfunc.img)

add_custom_target(20_day_harib17d_naskfunc
  COMMAND ${RAKUSK_EXECUTABLE} ${20_day_harib17d_naskfunc_SRCS} ${20_day_harib17d_naskfunc_OUTS}
  DEPENDS ${20_day_harib17d_naskfunc_SRCS}
)

add_dependencies(images 20_day_harib17d_naskfunc)
set(20_day_harib17d_naskfunc_WINE_OUTS ${root_BINARY_DIR}/projects/20_day/harib17d/naskfunc_wine.img)
add_custom_target(20_day_harib17d_naskfunc_wine
  COMMAND ${WINE} ${WINE_NASK} ${20_day_harib17d_naskfunc_SRCS} ${20_day_harib17d_naskfunc_WINE_OUTS}
)
add_dependencies(wine 20_day_harib17d_naskfunc_wine)

add_custom_target(20_day_harib17d_naskfunc_od
  COMMAND ${OD} -t x1 ${20_day_harib17d_naskfunc_OUTS}      > ${20_day_harib17d_naskfunc_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${20_day_harib17d_naskfunc_WINE_OUTS} > ${20_day_harib17d_naskfunc_WINE_OUTS}_t.txt
  COMMAND diff -s ${20_day_harib17d_naskfunc_OUTS}_f.txt ${20_day_harib17d_naskfunc_WINE_OUTS}_t.txt
)

add_dependencies(20_day_harib17d_naskfunc_od 20_day_harib17d_naskfunc)
add_dependencies(20_day_harib17d_naskfunc_od 20_day_harib17d_naskfunc_wine)
add_dependencies(od 20_day_harib17d_naskfunc_od)

#----------------------------------------------------------
