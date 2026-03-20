#----------------------------------------------------------
message(STATUS "Entering directory projects/20_day/harib17b/")

set(FONT ${RAKUSK_EXECUTABLE} --makefont)
set(B2O  ${RAKUSK_EXECUTABLE} --bin2obj)
set(CONV ${root_BINARY_DIR}/objconv/objconv)
set(20_day_harib17b_WILDOBJ ${root_BINARY_DIR}/projects/20_day/harib17b/*.o)

# Full OS build rules
set(20_day_harib17b_OS    ${root_BINARY_DIR}/projects/20_day/harib17b/os.img)
set(20_day_harib17b_SYS   ${root_BINARY_DIR}/projects/20_day/harib17b/os.sys)
set(20_day_harib17b_IPLB  ${root_BINARY_DIR}/projects/20_day/harib17b/ipl.bin)
set(20_day_harib17b_IPLS  ${root_SOURCE_DIR}/projects/20_day/harib17b/ipl10.nas)
set(20_day_harib17b_HEADB ${root_BINARY_DIR}/projects/20_day/harib17b/asmhead.bin)
set(20_day_harib17b_HEADS ${root_SOURCE_DIR}/projects/20_day/harib17b/asmhead.nas)
set(20_day_harib17b_BOOTB ${root_BINARY_DIR}/projects/20_day/harib17b/boot.bin)
set(20_day_harib17b_CCO   ${root_BINARY_DIR}/projects/20_day/harib17b/boot.o)
set(20_day_harib17b_CCS   ${root_SOURCE_DIR}/projects/20_day/harib17b/*.c)
set(20_day_harib17b_LDS   ${root_SOURCE_DIR}/projects/os.lds)
set(20_day_harib17b_FUNCS ${root_SOURCE_DIR}/projects/20_day/harib17b/naskfunc.nas)
set(20_day_harib17b_FUNCO ${root_BINARY_DIR}/projects/20_day/harib17b/naskfunc.o)
set(20_day_harib17b_FONTS ${root_SOURCE_DIR}/projects/20_day/harib17b/hankaku.txt)
set(20_day_harib17b_FONTB ${root_BINARY_DIR}/projects/20_day/harib17b/hankaku.bin)
set(20_day_harib17b_FONTO ${root_BINARY_DIR}/projects/20_day/harib17b/hankaku.o)
set(20_day_harib17b_LIBGE ${root_BINARY_DIR}/golibc/libgo.a)
set(20_day_harib17b_LIBGC ${root_BINARY_DIR}/projects/20_day/harib17b/libgo.o)

add_custom_target(20_day_harib17b_run
  COMMAND ${QEMU} ${QEMUOPT} ${20_day_harib17b_OS}
  DEPENDS 20_day_harib17b_img
)
add_custom_target(20_day_harib17b_debug
  COMMAND ${QEMU} -s -S ${QEMUOPT} ${20_day_harib17b_OS} ${QEMU_DEBUG_OPT}
  DEPENDS 20_day_harib17b_img
)
add_custom_target(20_day_harib17b_clean
  COMMAND rm -f ${20_day_harib17b_OS}
  COMMAND rm -f ${20_day_harib17b_SYS}
  COMMAND rm -f ${20_day_harib17b_IPLB}
  COMMAND rm -f ${20_day_harib17b_HEADB}
  COMMAND rm -f ${20_day_harib17b_BOOTB}
  COMMAND rm -f ${20_day_harib17b_WILDOBJ}
  COMMAND rm -f ${root_BINARY_DIR}/projects/20_day/harib17b/ipl10.bin
  COMMAND rm -f ${root_BINARY_DIR}/projects/20_day/harib17b/asmhead.bin
  COMMAND rm -f ${root_BINARY_DIR}/projects/20_day/harib17b/*_wine.bin
  COMMAND rm -f ${root_BINARY_DIR}/projects/20_day/harib17b/*.txt
)
add_custom_target(20_day_harib17b_ipl_build
  COMMAND ${RAKUSK_EXECUTABLE} ${20_day_harib17b_IPLS} ${20_day_harib17b_IPLB}
  DEPENDS ${20_day_harib17b_IPLS}
)
add_custom_command(
  OUTPUT ${20_day_harib17b_HEADB}
  COMMAND ${RAKUSK_EXECUTABLE} ${20_day_harib17b_HEADS} ${20_day_harib17b_HEADB}
  DEPENDS ${20_day_harib17b_HEADS}
  COMMENT "Generating asmhead.bin for 20_day_harib17b"
)

add_custom_target(20_day_harib17b_sys
  COMMAND ${FONT} ${20_day_harib17b_FONTS} ${20_day_harib17b_FONTB}
  COMMAND ${B2O}  ${20_day_harib17b_FONTB} ${20_day_harib17b_FONTO} _hankaku
  COMMAND ${CONV} -fcoff32 -nu ${20_day_harib17b_LIBGE} ${20_day_harib17b_LIBGC}
  COMMAND ${RAKUSK_EXECUTABLE} ${20_day_harib17b_FUNCS} ${20_day_harib17b_FUNCO}
  COMMAND gcc ${BINOPT} -T ${20_day_harib17b_LDS} ${20_day_harib17b_CCS} ${20_day_harib17b_WILDOBJ} -o ${20_day_harib17b_BOOTB}
  COMMAND cat ${20_day_harib17b_HEADB} ${20_day_harib17b_BOOTB} > ${20_day_harib17b_SYS}
  DEPENDS ${20_day_harib17b_HEADB} 20_day_harib17b_ipl_build
  DEPENDS ${20_day_harib17b_FUNCO}
  DEPENDS ${20_day_harib17b_FONTO}
)
add_custom_target(20_day_harib17b_img
  COMMAND mformat -f 1440 -l HARIBOTEOS -N 0xffffffff -C -B ${20_day_harib17b_IPLB} -i ${20_day_harib17b_OS}
  COMMAND mcopy -i ${20_day_harib17b_OS} ${20_day_harib17b_SYS} ::
  DEPENDS 20_day_harib17b_sys
)

########### next target ###############
set(20_day_harib17b_asmhead_SRCS ${root_SOURCE_DIR}/projects/20_day/harib17b/asmhead.nas)
set(20_day_harib17b_asmhead_OUTS ${root_BINARY_DIR}/projects/20_day/harib17b/asmhead.bin)

add_custom_target(20_day_harib17b_asmhead
  COMMAND ${RAKUSK_EXECUTABLE} ${20_day_harib17b_asmhead_SRCS} ${20_day_harib17b_asmhead_OUTS}
  DEPENDS ${20_day_harib17b_asmhead_SRCS}
)

set(20_day_harib17b_asmhead_WINE_OUTS ${root_BINARY_DIR}/projects/20_day/harib17b/asmhead_wine.bin)
add_custom_target(20_day_harib17b_asmhead_wine
  COMMAND ${WINE} ${WINE_NASK} ${20_day_harib17b_asmhead_SRCS} ${20_day_harib17b_asmhead_WINE_OUTS}
)
add_dependencies(wine 20_day_harib17b_asmhead_wine)

add_custom_target(20_day_harib17b_asmhead_od
  COMMAND ${OD} -t x1 ${20_day_harib17b_asmhead_OUTS}      > ${20_day_harib17b_asmhead_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${20_day_harib17b_asmhead_WINE_OUTS} > ${20_day_harib17b_asmhead_WINE_OUTS}_t.txt
  COMMAND diff -s ${20_day_harib17b_asmhead_OUTS}_f.txt ${20_day_harib17b_asmhead_WINE_OUTS}_t.txt
)

add_dependencies(20_day_harib17b_asmhead_od 20_day_harib17b_asmhead)
add_dependencies(20_day_harib17b_asmhead_od 20_day_harib17b_asmhead_wine)
add_dependencies(od 20_day_harib17b_asmhead_od)

#----------------------------------------------------------
########### next target ###############
set(20_day_harib17b_hlt_SRCS ${root_SOURCE_DIR}/projects/20_day/harib17b/hlt.nas)
set(20_day_harib17b_hlt_OUTS ${root_BINARY_DIR}/projects/20_day/harib17b/hlt.img)

add_custom_target(20_day_harib17b_hlt
  COMMAND ${RAKUSK_EXECUTABLE} ${20_day_harib17b_hlt_SRCS} ${20_day_harib17b_hlt_OUTS}
  DEPENDS ${20_day_harib17b_hlt_SRCS}
)

add_dependencies(images 20_day_harib17b_hlt)
set(20_day_harib17b_hlt_WINE_OUTS ${root_BINARY_DIR}/projects/20_day/harib17b/hlt_wine.img)
add_custom_target(20_day_harib17b_hlt_wine
  COMMAND ${WINE} ${WINE_NASK} ${20_day_harib17b_hlt_SRCS} ${20_day_harib17b_hlt_WINE_OUTS}
)
add_dependencies(wine 20_day_harib17b_hlt_wine)

add_custom_target(20_day_harib17b_hlt_od
  COMMAND ${OD} -t x1 ${20_day_harib17b_hlt_OUTS}      > ${20_day_harib17b_hlt_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${20_day_harib17b_hlt_WINE_OUTS} > ${20_day_harib17b_hlt_WINE_OUTS}_t.txt
  COMMAND diff -s ${20_day_harib17b_hlt_OUTS}_f.txt ${20_day_harib17b_hlt_WINE_OUTS}_t.txt
)

add_dependencies(20_day_harib17b_hlt_od 20_day_harib17b_hlt)
add_dependencies(20_day_harib17b_hlt_od 20_day_harib17b_hlt_wine)
add_dependencies(od 20_day_harib17b_hlt_od)

set(20_day_harib17b_hlt_HRB ${root_BINARY_DIR}/projects/20_day/harib17b/hlt.hrb)
add_custom_target(20_day_harib17b_hlt_hrb
  COMMAND ${RAKUSK_EXECUTABLE} ${20_day_harib17b_hlt_SRCS} ${20_day_harib17b_hlt_HRB}
  COMMAND mcopy -i ${20_day_harib17b_OS} ${20_day_harib17b_hlt_HRB} ::hlt.hrb
  DEPENDS 20_day_harib17b_img
)

add_dependencies(20_day_harib17b_run 20_day_harib17b_hlt_hrb)
#----------------------------------------------------------
########### next target ###############
set(20_day_harib17b_ipl10_SRCS ${root_SOURCE_DIR}/projects/20_day/harib17b/ipl10.nas)
set(20_day_harib17b_ipl10_OUTS ${root_BINARY_DIR}/projects/20_day/harib17b/ipl10.bin)

add_custom_target(20_day_harib17b_ipl10
  COMMAND ${RAKUSK_EXECUTABLE} ${20_day_harib17b_ipl10_SRCS} ${20_day_harib17b_ipl10_OUTS}
  DEPENDS ${20_day_harib17b_ipl10_SRCS}
)

set(20_day_harib17b_ipl10_WINE_OUTS ${root_BINARY_DIR}/projects/20_day/harib17b/ipl10_wine.bin)
add_custom_target(20_day_harib17b_ipl10_wine
  COMMAND ${WINE} ${WINE_NASK} ${20_day_harib17b_ipl10_SRCS} ${20_day_harib17b_ipl10_WINE_OUTS}
)
add_dependencies(wine 20_day_harib17b_ipl10_wine)

add_custom_target(20_day_harib17b_ipl10_od
  COMMAND ${OD} -t x1 ${20_day_harib17b_ipl10_OUTS}      > ${20_day_harib17b_ipl10_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${20_day_harib17b_ipl10_WINE_OUTS} > ${20_day_harib17b_ipl10_WINE_OUTS}_t.txt
  COMMAND diff -s ${20_day_harib17b_ipl10_OUTS}_f.txt ${20_day_harib17b_ipl10_WINE_OUTS}_t.txt
)

add_dependencies(20_day_harib17b_ipl10_od 20_day_harib17b_ipl10)
add_dependencies(20_day_harib17b_ipl10_od 20_day_harib17b_ipl10_wine)
add_dependencies(od 20_day_harib17b_ipl10_od)

#----------------------------------------------------------
########### next target ###############
set(20_day_harib17b_naskfunc_SRCS ${root_SOURCE_DIR}/projects/20_day/harib17b/naskfunc.nas)
set(20_day_harib17b_naskfunc_OUTS ${root_BINARY_DIR}/projects/20_day/harib17b/naskfunc.img)

add_custom_target(20_day_harib17b_naskfunc
  COMMAND ${RAKUSK_EXECUTABLE} ${20_day_harib17b_naskfunc_SRCS} ${20_day_harib17b_naskfunc_OUTS}
  DEPENDS ${20_day_harib17b_naskfunc_SRCS}
)

add_dependencies(images 20_day_harib17b_naskfunc)
set(20_day_harib17b_naskfunc_WINE_OUTS ${root_BINARY_DIR}/projects/20_day/harib17b/naskfunc_wine.img)
add_custom_target(20_day_harib17b_naskfunc_wine
  COMMAND ${WINE} ${WINE_NASK} ${20_day_harib17b_naskfunc_SRCS} ${20_day_harib17b_naskfunc_WINE_OUTS}
)
add_dependencies(wine 20_day_harib17b_naskfunc_wine)

add_custom_target(20_day_harib17b_naskfunc_od
  COMMAND ${OD} -t x1 ${20_day_harib17b_naskfunc_OUTS}      > ${20_day_harib17b_naskfunc_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${20_day_harib17b_naskfunc_WINE_OUTS} > ${20_day_harib17b_naskfunc_WINE_OUTS}_t.txt
  COMMAND diff -s ${20_day_harib17b_naskfunc_OUTS}_f.txt ${20_day_harib17b_naskfunc_WINE_OUTS}_t.txt
)

add_dependencies(20_day_harib17b_naskfunc_od 20_day_harib17b_naskfunc)
add_dependencies(20_day_harib17b_naskfunc_od 20_day_harib17b_naskfunc_wine)
add_dependencies(od 20_day_harib17b_naskfunc_od)

#----------------------------------------------------------
