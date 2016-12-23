#----------------------------------------------------------
message(STATUS "Entering directory projects/20_day/harib17b/")

set(NASK ${root_BINARY_DIR}/src/opennask)
set(FONT ${root_BINARY_DIR}/src/makefont)
set(B2O  ${root_BINARY_DIR}/src/bin2obj)
set(CONV ${root_BINARY_DIR}/objconv/objconv)
set(20_day_harib17b_OS    ${root_BINARY_DIR}/projects/20_day/harib17b/os.img)
set(20_day_harib17b_SYS	  ${root_BINARY_DIR}/projects/20_day/harib17b/os.sys)
set(20_day_harib17b_IPLB  ${root_BINARY_DIR}/projects/20_day/harib17b/ipl.bin)
set(20_day_harib17b_IPLS  ${root_SOURCE_DIR}/projects/20_day/harib17b/ipl10.nas)
set(20_day_harib17b_HEADB ${root_BINARY_DIR}/projects/20_day/harib17b/asmhead.bin)
set(20_day_harib17b_HEADS ${root_SOURCE_DIR}/projects/20_day/harib17b/asmhead.nas)
set(20_day_harib17b_BOOTB ${root_BINARY_DIR}/projects/20_day/harib17b/boot.bin)
set(20_day_harib17b_CCO	  ${root_BINARY_DIR}/projects/20_day/harib17b/boot.o)
set(20_day_harib17b_CCS	  ${root_SOURCE_DIR}/projects/20_day/harib17b/*.c)
set(20_day_harib17b_LDS	  ${root_SOURCE_DIR}/projects/os.lds)
set(20_day_harib17b_FUNCS ${root_SOURCE_DIR}/projects/20_day/harib17b/naskfunc.nas)
set(20_day_harib17b_FUNCO ${root_BINARY_DIR}/projects/20_day/harib17b/naskfunc.o)
set(20_day_harib17b_FONTS ${root_SOURCE_DIR}/projects/20_day/harib17b/hankaku.txt)
set(20_day_harib17b_FONTB ${root_BINARY_DIR}/projects/20_day/harib17b/hankaku.bin)
set(20_day_harib17b_FONTO ${root_BINARY_DIR}/projects/20_day/harib17b/hankaku.o)
set(20_day_harib17b_LIBGE ${root_BINARY_DIR}/golibc/libgo.a)
set(20_day_harib17b_LIBGC ${root_BINARY_DIR}/projects/20_day/harib17b/libgo.o)
set(20_day_harib17b_WILDOBJ ${root_BINARY_DIR}/projects/20_day/harib17b/*.o)


add_custom_target(20_day_harib17b_run
  COMMAND ${QEMU} ${QEMUOPT} ${20_day_harib17b_OS}
  DEPENDS 20_day_harib17b_img
)
add_custom_target(20_day_harib17b_debug
  COMMAND ${QEMU} -s -S ${QEMUOPT} ${20_day_harib17b_OS} -redir tcp:5555:127.0.0.1:1234 &
  DEPENDS 20_day_harib17b_img
)
add_custom_target(20_day_harib17b_clean
  COMMAND rm -f ${20_day_harib17b_OS}
  COMMAND rm -f ${20_day_harib17b_SYS}
  COMMAND rm -f ${20_day_harib17b_IPLB}
  COMMAND rm -f ${20_day_harib17b_HEADB}
  COMMAND rm -f ${20_day_harib17b_BOOTB}
  COMMAND rm -f ${20_day_harib17b_WILDOBJ}
)
add_custom_target(20_day_harib17b_ipl
  COMMAND ${NASK} ${20_day_harib17b_IPLS} ${20_day_harib17b_IPLB}
)
add_custom_target(20_day_harib17b_sys
  COMMAND ${NASK} ${20_day_harib17b_HEADS} ${20_day_harib17b_HEADB}
  COMMAND ${FONT} ${20_day_harib17b_FONTS} ${20_day_harib17b_FONTB}
  COMMAND ${B2O}  ${20_day_harib17b_FONTB} ${20_day_harib17b_FONTO} _hankaku
  COMMAND ${CONV} -fcoff32 -nu ${20_day_harib17b_LIBGE} ${20_day_harib17b_LIBGC}
  COMMAND ${NASK} ${20_day_harib17b_FUNCS} ${20_day_harib17b_FUNCO}
  COMMAND gcc ${BINOPT} -T ${20_day_harib17b_LDS} ${20_day_harib17b_CCS} ${20_day_harib17b_WILDOBJ} -o ${20_day_harib17b_BOOTB}
  COMMAND cat ${20_day_harib17b_HEADB} ${20_day_harib17b_BOOTB} > ${20_day_harib17b_SYS}
  DEPENDS 20_day_harib17b_ipl
)
add_custom_target(20_day_harib17b_img
  COMMAND mformat -f 1440 -l HARIBOTEOS -N 0xffffffff -C -B ${20_day_harib17b_IPLB} -i ${20_day_harib17b_OS}
  COMMAND mcopy -i ${20_day_harib17b_OS} ${20_day_harib17b_SYS} ::
  DEPENDS 20_day_harib17b_sys
)
########### next target ###############
set(asmhead.img_SRCS ${root_SOURCE_DIR}/projects/20_day/harib17b/asmhead.nas)
set(asmhead.img_OUTS ${root_BINARY_DIR}/projects/20_day/harib17b/asmhead.img)
set(asmhead_wine.img_OUTS ${root_BINARY_DIR}/projects/20_day/harib17b/asmhead_wine.img)

add_custom_target(20_day_harib17b_asmhead
  COMMAND ${root_BINARY_DIR}/src/opennask ${asmhead.img_SRCS} ${asmhead.img_OUTS}
)
add_custom_target(20_day_harib17b_asmhead_wine
  COMMAND ${WINE} ${WINE_NASK} ${asmhead.img_SRCS} ${asmhead_wine.img_OUTS}
)
add_custom_target(20_day_harib17b_asmhead_od
  COMMAND ${OD} -t x1 ${asmhead.img_OUTS}      > ${asmhead.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${asmhead_wine.img_OUTS} > ${asmhead_wine.img_OUTS}_t.txt
  COMMAND diff -s ${asmhead.img_OUTS}_f.txt ${asmhead_wine.img_OUTS}_t.txt
)

add_dependencies(20_day_harib17b_asmhead_od 20_day_harib17b_asmhead)
add_dependencies(20_day_harib17b_asmhead_od 20_day_harib17b_asmhead_wine)

add_dependencies(images 20_day_harib17b_asmhead)
add_dependencies(wine 20_day_harib17b_asmhead_wine)
add_dependencies(od 20_day_harib17b_asmhead_od)

#----------------------------------------------------------
########### next target ###############
set(hlt.img_SRCS ${root_SOURCE_DIR}/projects/20_day/harib17b/hlt.nas)
set(hlt.img_OUTS ${root_BINARY_DIR}/projects/20_day/harib17b/hlt.img)
set(hlt_wine.img_OUTS ${root_BINARY_DIR}/projects/20_day/harib17b/hlt_wine.img)

add_custom_target(20_day_harib17b_hlt
  COMMAND ${root_BINARY_DIR}/src/opennask ${hlt.img_SRCS} ${hlt.img_OUTS}
)
set(hlt.img_OBJ  ${root_BINARY_DIR}/projects/20_day/harib17b/hlt.o)
add_custom_target(20_day_harib17b_hlt_obj
  COMMAND ${root_BINARY_DIR}/src/opennask ${hlt.img_SRCS} ${hlt.img_OBJ}
)

add_dependencies(20_day_harib17b_sys 20_day_harib17b_hlt_obj)

add_custom_target(20_day_harib17b_hlt_wine
  COMMAND ${WINE} ${WINE_NASK} ${hlt.img_SRCS} ${hlt_wine.img_OUTS}
)
add_custom_target(20_day_harib17b_hlt_od
  COMMAND ${OD} -t x1 ${hlt.img_OUTS}      > ${hlt.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${hlt_wine.img_OUTS} > ${hlt_wine.img_OUTS}_t.txt
  COMMAND diff -s ${hlt.img_OUTS}_f.txt ${hlt_wine.img_OUTS}_t.txt
)

add_dependencies(20_day_harib17b_hlt_od 20_day_harib17b_hlt)
add_dependencies(20_day_harib17b_hlt_od 20_day_harib17b_hlt_wine)

add_dependencies(images 20_day_harib17b_hlt)
add_dependencies(wine 20_day_harib17b_hlt_wine)
add_dependencies(od 20_day_harib17b_hlt_od)

#----------------------------------------------------------
########### next target ###############
set(ipl10.img_SRCS ${root_SOURCE_DIR}/projects/20_day/harib17b/ipl10.nas)
set(ipl10.img_OUTS ${root_BINARY_DIR}/projects/20_day/harib17b/ipl10.img)
set(ipl10_wine.img_OUTS ${root_BINARY_DIR}/projects/20_day/harib17b/ipl10_wine.img)

add_custom_target(20_day_harib17b_ipl10
  COMMAND ${root_BINARY_DIR}/src/opennask ${ipl10.img_SRCS} ${ipl10.img_OUTS}
)
add_custom_target(20_day_harib17b_ipl10_wine
  COMMAND ${WINE} ${WINE_NASK} ${ipl10.img_SRCS} ${ipl10_wine.img_OUTS}
)
add_custom_target(20_day_harib17b_ipl10_od
  COMMAND ${OD} -t x1 ${ipl10.img_OUTS}      > ${ipl10.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${ipl10_wine.img_OUTS} > ${ipl10_wine.img_OUTS}_t.txt
  COMMAND diff -s ${ipl10.img_OUTS}_f.txt ${ipl10_wine.img_OUTS}_t.txt
)

add_dependencies(20_day_harib17b_ipl10_od 20_day_harib17b_ipl10)
add_dependencies(20_day_harib17b_ipl10_od 20_day_harib17b_ipl10_wine)

add_dependencies(images 20_day_harib17b_ipl10)
add_dependencies(wine 20_day_harib17b_ipl10_wine)
add_dependencies(od 20_day_harib17b_ipl10_od)

#----------------------------------------------------------
########### next target ###############
set(naskfunc.img_SRCS ${root_SOURCE_DIR}/projects/20_day/harib17b/naskfunc.nas)
set(naskfunc.img_OUTS ${root_BINARY_DIR}/projects/20_day/harib17b/naskfunc.img)
set(naskfunc_wine.img_OUTS ${root_BINARY_DIR}/projects/20_day/harib17b/naskfunc_wine.img)

add_custom_target(20_day_harib17b_naskfunc
  COMMAND ${root_BINARY_DIR}/src/opennask ${naskfunc.img_SRCS} ${naskfunc.img_OUTS}
)
add_custom_target(20_day_harib17b_naskfunc_wine
  COMMAND ${WINE} ${WINE_NASK} ${naskfunc.img_SRCS} ${naskfunc_wine.img_OUTS}
)
add_custom_target(20_day_harib17b_naskfunc_od
  COMMAND ${OD} -t x1 ${naskfunc.img_OUTS}      > ${naskfunc.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${naskfunc_wine.img_OUTS} > ${naskfunc_wine.img_OUTS}_t.txt
  COMMAND diff -s ${naskfunc.img_OUTS}_f.txt ${naskfunc_wine.img_OUTS}_t.txt
)

add_dependencies(20_day_harib17b_naskfunc_od 20_day_harib17b_naskfunc)
add_dependencies(20_day_harib17b_naskfunc_od 20_day_harib17b_naskfunc_wine)

add_dependencies(images 20_day_harib17b_naskfunc)
add_dependencies(wine 20_day_harib17b_naskfunc_wine)
add_dependencies(od 20_day_harib17b_naskfunc_od)

#----------------------------------------------------------
