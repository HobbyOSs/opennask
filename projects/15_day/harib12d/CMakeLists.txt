#----------------------------------------------------------
message(STATUS "Entering directory projects/15_day/harib12d/")

set(NASK ${root_BINARY_DIR}/src/opennask)
set(FONT ${root_BINARY_DIR}/src/makefont)
set(B2O  ${root_BINARY_DIR}/src/bin2obj)
set(CONV ${root_BINARY_DIR}/objconv/objconv)
set(15_day_harib12d_OS    ${root_BINARY_DIR}/projects/15_day/harib12d/os.img)
set(15_day_harib12d_SYS	  ${root_BINARY_DIR}/projects/15_day/harib12d/os.sys)
set(15_day_harib12d_IPLB  ${root_BINARY_DIR}/projects/15_day/harib12d/ipl.bin)
set(15_day_harib12d_IPLS  ${root_SOURCE_DIR}/projects/15_day/harib12d/ipl10.nas)
set(15_day_harib12d_HEADB ${root_BINARY_DIR}/projects/15_day/harib12d/asmhead.bin)
set(15_day_harib12d_HEADS ${root_SOURCE_DIR}/projects/15_day/harib12d/asmhead.nas)
set(15_day_harib12d_BOOTB ${root_BINARY_DIR}/projects/15_day/harib12d/boot.bin)
set(15_day_harib12d_CCO	  ${root_BINARY_DIR}/projects/15_day/harib12d/boot.o)
set(15_day_harib12d_CCS	  ${root_SOURCE_DIR}/projects/15_day/harib12d/*.c)
set(15_day_harib12d_LDS	  ${root_SOURCE_DIR}/projects/os.lds)
set(15_day_harib12d_FUNCS ${root_SOURCE_DIR}/projects/15_day/harib12d/naskfunc.nas)
set(15_day_harib12d_FUNCO ${root_BINARY_DIR}/projects/15_day/harib12d/naskfunc.o)
set(15_day_harib12d_FONTS ${root_SOURCE_DIR}/projects/15_day/harib12d/hankaku.txt)
set(15_day_harib12d_FONTB ${root_BINARY_DIR}/projects/15_day/harib12d/hankaku.bin)
set(15_day_harib12d_FONTO ${root_BINARY_DIR}/projects/15_day/harib12d/hankaku.o)
set(15_day_harib12d_LIBGE ${root_BINARY_DIR}/golibc/libgo.a)
set(15_day_harib12d_LIBGC ${root_BINARY_DIR}/projects/15_day/harib12d/libgo.o)
set(15_day_harib12d_WILDOBJ ${root_BINARY_DIR}/projects/15_day/harib12d/*.o)


add_custom_target(15_day_harib12d_run
  COMMAND ${QEMU} ${QEMUOPT} ${15_day_harib12d_OS}
  DEPENDS 15_day_harib12d_img
)
add_custom_target(15_day_harib12d_debug
  COMMAND ${QEMU} -s -S ${QEMUOPT} ${15_day_harib12d_OS} -redir tcp:5555:127.0.0.1:1234 &
  DEPENDS 15_day_harib12d_img
)
add_custom_target(15_day_harib12d_clean
  COMMAND rm -f ${15_day_harib12d_OS}
  COMMAND rm -f ${15_day_harib12d_SYS}
  COMMAND rm -f ${15_day_harib12d_IPLB}
  COMMAND rm -f ${15_day_harib12d_HEADB}
  COMMAND rm -f ${15_day_harib12d_BOOTB}
  COMMAND rm -f ${15_day_harib12d_WILDOBJ}
)
add_custom_target(15_day_harib12d_ipl
  COMMAND ${NASK} ${15_day_harib12d_IPLS} ${15_day_harib12d_IPLB}
)
add_custom_target(15_day_harib12d_sys
  COMMAND ${NASK} ${15_day_harib12d_HEADS} ${15_day_harib12d_HEADB}
  COMMAND ${FONT} ${15_day_harib12d_FONTS} ${15_day_harib12d_FONTB}
  COMMAND ${B2O}  ${15_day_harib12d_FONTB} ${15_day_harib12d_FONTO} _hankaku
  COMMAND ${CONV} -fcoff32 -nu ${15_day_harib12d_LIBGE} ${15_day_harib12d_LIBGC}
  COMMAND ${NASK} ${15_day_harib12d_FUNCS} ${15_day_harib12d_FUNCO}
  COMMAND gcc ${BINOPT} -T ${15_day_harib12d_LDS} ${15_day_harib12d_CCS} ${15_day_harib12d_WILDOBJ} -o ${15_day_harib12d_BOOTB}
  COMMAND cat ${15_day_harib12d_HEADB} ${15_day_harib12d_BOOTB} > ${15_day_harib12d_SYS}
  DEPENDS 15_day_harib12d_ipl
)
add_custom_target(15_day_harib12d_img
  COMMAND mformat -f 1440 -l HARIBOTEOS -N 0xffffffff -C -B ${15_day_harib12d_IPLB} -i ${15_day_harib12d_OS}
  COMMAND mcopy -i ${15_day_harib12d_OS} ${15_day_harib12d_SYS} ::
  DEPENDS 15_day_harib12d_sys
)
########### next target ###############
set(asmhead.img_SRCS ${root_SOURCE_DIR}/projects/15_day/harib12d/asmhead.nas)
set(asmhead.img_OUTS ${root_BINARY_DIR}/projects/15_day/harib12d/asmhead.img)
set(asmhead_wine.img_OUTS ${root_BINARY_DIR}/projects/15_day/harib12d/asmhead_wine.img)

add_custom_target(15_day_harib12d_asmhead
  COMMAND ${root_BINARY_DIR}/src/opennask ${asmhead.img_SRCS} ${asmhead.img_OUTS}
)
add_custom_target(15_day_harib12d_asmhead_wine
  COMMAND ${WINE} ${WINE_NASK} ${asmhead.img_SRCS} ${asmhead_wine.img_OUTS}
)
add_custom_target(15_day_harib12d_asmhead_od
  COMMAND ${OD} -t x1 ${asmhead.img_OUTS}      > ${asmhead.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${asmhead_wine.img_OUTS} > ${asmhead_wine.img_OUTS}_t.txt
  COMMAND diff -s ${asmhead.img_OUTS}_f.txt ${asmhead_wine.img_OUTS}_t.txt
)

add_dependencies(15_day_harib12d_asmhead_od 15_day_harib12d_asmhead)
add_dependencies(15_day_harib12d_asmhead_od 15_day_harib12d_asmhead_wine)

add_dependencies(images 15_day_harib12d_asmhead)
add_dependencies(wine 15_day_harib12d_asmhead_wine)
add_dependencies(od 15_day_harib12d_asmhead_od)

#----------------------------------------------------------
########### next target ###############
set(ipl10.img_SRCS ${root_SOURCE_DIR}/projects/15_day/harib12d/ipl10.nas)
set(ipl10.img_OUTS ${root_BINARY_DIR}/projects/15_day/harib12d/ipl10.img)
set(ipl10_wine.img_OUTS ${root_BINARY_DIR}/projects/15_day/harib12d/ipl10_wine.img)

add_custom_target(15_day_harib12d_ipl10
  COMMAND ${root_BINARY_DIR}/src/opennask ${ipl10.img_SRCS} ${ipl10.img_OUTS}
)
add_custom_target(15_day_harib12d_ipl10_wine
  COMMAND ${WINE} ${WINE_NASK} ${ipl10.img_SRCS} ${ipl10_wine.img_OUTS}
)
add_custom_target(15_day_harib12d_ipl10_od
  COMMAND ${OD} -t x1 ${ipl10.img_OUTS}      > ${ipl10.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${ipl10_wine.img_OUTS} > ${ipl10_wine.img_OUTS}_t.txt
  COMMAND diff -s ${ipl10.img_OUTS}_f.txt ${ipl10_wine.img_OUTS}_t.txt
)

add_dependencies(15_day_harib12d_ipl10_od 15_day_harib12d_ipl10)
add_dependencies(15_day_harib12d_ipl10_od 15_day_harib12d_ipl10_wine)

add_dependencies(images 15_day_harib12d_ipl10)
add_dependencies(wine 15_day_harib12d_ipl10_wine)
add_dependencies(od 15_day_harib12d_ipl10_od)

#----------------------------------------------------------
########### next target ###############
set(naskfunc.img_SRCS ${root_SOURCE_DIR}/projects/15_day/harib12d/naskfunc.nas)
set(naskfunc.img_OUTS ${root_BINARY_DIR}/projects/15_day/harib12d/naskfunc.img)
set(naskfunc_wine.img_OUTS ${root_BINARY_DIR}/projects/15_day/harib12d/naskfunc_wine.img)

add_custom_target(15_day_harib12d_naskfunc
  COMMAND ${root_BINARY_DIR}/src/opennask ${naskfunc.img_SRCS} ${naskfunc.img_OUTS}
)
add_custom_target(15_day_harib12d_naskfunc_wine
  COMMAND ${WINE} ${WINE_NASK} ${naskfunc.img_SRCS} ${naskfunc_wine.img_OUTS}
)
add_custom_target(15_day_harib12d_naskfunc_od
  COMMAND ${OD} -t x1 ${naskfunc.img_OUTS}      > ${naskfunc.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${naskfunc_wine.img_OUTS} > ${naskfunc_wine.img_OUTS}_t.txt
  COMMAND diff -s ${naskfunc.img_OUTS}_f.txt ${naskfunc_wine.img_OUTS}_t.txt
)

add_dependencies(15_day_harib12d_naskfunc_od 15_day_harib12d_naskfunc)
add_dependencies(15_day_harib12d_naskfunc_od 15_day_harib12d_naskfunc_wine)

add_dependencies(images 15_day_harib12d_naskfunc)
add_dependencies(wine 15_day_harib12d_naskfunc_wine)
add_dependencies(od 15_day_harib12d_naskfunc_od)

#----------------------------------------------------------
