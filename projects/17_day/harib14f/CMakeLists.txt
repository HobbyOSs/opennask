#----------------------------------------------------------
message(STATUS "Entering directory projects/17_day/harib14f/")

set(NASK ${root_BINARY_DIR}/src/opennask)
set(FONT ${root_BINARY_DIR}/src/makefont)
set(B2O  ${root_BINARY_DIR}/src/bin2obj)
set(CONV ${root_BINARY_DIR}/objconv/objconv)
set(17_day_harib14f_OS    ${root_BINARY_DIR}/projects/17_day/harib14f/os.img)
set(17_day_harib14f_SYS	  ${root_BINARY_DIR}/projects/17_day/harib14f/os.sys)
set(17_day_harib14f_IPLB  ${root_BINARY_DIR}/projects/17_day/harib14f/ipl.bin)
set(17_day_harib14f_IPLS  ${root_SOURCE_DIR}/projects/17_day/harib14f/ipl10.nas)
set(17_day_harib14f_HEADB ${root_BINARY_DIR}/projects/17_day/harib14f/asmhead.bin)
set(17_day_harib14f_HEADS ${root_SOURCE_DIR}/projects/17_day/harib14f/asmhead.nas)
set(17_day_harib14f_BOOTB ${root_BINARY_DIR}/projects/17_day/harib14f/boot.bin)
set(17_day_harib14f_CCO	  ${root_BINARY_DIR}/projects/17_day/harib14f/boot.o)
set(17_day_harib14f_CCS	  ${root_SOURCE_DIR}/projects/17_day/harib14f/*.c)
set(17_day_harib14f_LDS	  ${root_SOURCE_DIR}/projects/os.lds)
set(17_day_harib14f_FUNCS ${root_SOURCE_DIR}/projects/17_day/harib14f/naskfunc.nas)
set(17_day_harib14f_FUNCO ${root_BINARY_DIR}/projects/17_day/harib14f/naskfunc.o)
set(17_day_harib14f_FONTS ${root_SOURCE_DIR}/projects/17_day/harib14f/hankaku.txt)
set(17_day_harib14f_FONTB ${root_BINARY_DIR}/projects/17_day/harib14f/hankaku.bin)
set(17_day_harib14f_FONTO ${root_BINARY_DIR}/projects/17_day/harib14f/hankaku.o)
set(17_day_harib14f_LIBGE ${root_BINARY_DIR}/golibc/libgo.a)
set(17_day_harib14f_LIBGC ${root_BINARY_DIR}/projects/17_day/harib14f/libgo.o)
set(17_day_harib14f_WILDOBJ ${root_BINARY_DIR}/projects/17_day/harib14f/*.o)


add_custom_target(17_day_harib14f_run
  COMMAND ${QEMU} ${QEMUOPT} ${17_day_harib14f_OS}
  DEPENDS 17_day_harib14f_img
)
add_custom_target(17_day_harib14f_debug
  COMMAND ${QEMU} -s -S ${QEMUOPT} ${17_day_harib14f_OS} -redir tcp:5555:127.0.0.1:1234 &
  DEPENDS 17_day_harib14f_img
)
add_custom_target(17_day_harib14f_clean
  COMMAND rm -f ${17_day_harib14f_OS}
  COMMAND rm -f ${17_day_harib14f_SYS}
  COMMAND rm -f ${17_day_harib14f_IPLB}
  COMMAND rm -f ${17_day_harib14f_HEADB}
  COMMAND rm -f ${17_day_harib14f_BOOTB}
  COMMAND rm -f ${17_day_harib14f_WILDOBJ}
)
add_custom_target(17_day_harib14f_ipl
  COMMAND ${NASK} ${17_day_harib14f_IPLS} ${17_day_harib14f_IPLB}
)
add_custom_target(17_day_harib14f_sys
  COMMAND ${NASK} ${17_day_harib14f_HEADS} ${17_day_harib14f_HEADB}
  COMMAND ${FONT} ${17_day_harib14f_FONTS} ${17_day_harib14f_FONTB}
  COMMAND ${B2O}  ${17_day_harib14f_FONTB} ${17_day_harib14f_FONTO} _hankaku
  COMMAND ${CONV} -fcoff32 -nu ${17_day_harib14f_LIBGE} ${17_day_harib14f_LIBGC}
  COMMAND ${NASK} ${17_day_harib14f_FUNCS} ${17_day_harib14f_FUNCO}
  COMMAND gcc ${BINOPT} -T ${17_day_harib14f_LDS} ${17_day_harib14f_CCS} ${17_day_harib14f_WILDOBJ} -o ${17_day_harib14f_BOOTB}
  COMMAND cat ${17_day_harib14f_HEADB} ${17_day_harib14f_BOOTB} > ${17_day_harib14f_SYS}
  DEPENDS 17_day_harib14f_ipl
)
add_custom_target(17_day_harib14f_img
  COMMAND mformat -f 1440 -l HARIBOTEOS -N 0xffffffff -C -B ${17_day_harib14f_IPLB} -i ${17_day_harib14f_OS}
  COMMAND mcopy -i ${17_day_harib14f_OS} ${17_day_harib14f_SYS} ::
  DEPENDS 17_day_harib14f_sys
)
########### next target ###############
set(asmhead.img_SRCS ${root_SOURCE_DIR}/projects/17_day/harib14f/asmhead.nas)
set(asmhead.img_OUTS ${root_BINARY_DIR}/projects/17_day/harib14f/asmhead.img)
set(asmhead_wine.img_OUTS ${root_BINARY_DIR}/projects/17_day/harib14f/asmhead_wine.img)

add_custom_target(17_day_harib14f_asmhead
  COMMAND ${root_BINARY_DIR}/src/opennask ${asmhead.img_SRCS} ${asmhead.img_OUTS}
)
add_custom_target(17_day_harib14f_asmhead_wine
  COMMAND ${WINE} ${WINE_NASK} ${asmhead.img_SRCS} ${asmhead_wine.img_OUTS}
)
add_custom_target(17_day_harib14f_asmhead_od
  COMMAND ${OD} -t x1 ${asmhead.img_OUTS}      > ${asmhead.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${asmhead_wine.img_OUTS} > ${asmhead_wine.img_OUTS}_t.txt
  COMMAND diff -s ${asmhead.img_OUTS}_f.txt ${asmhead_wine.img_OUTS}_t.txt
)

add_dependencies(17_day_harib14f_asmhead_od 17_day_harib14f_asmhead)
add_dependencies(17_day_harib14f_asmhead_od 17_day_harib14f_asmhead_wine)

add_dependencies(images 17_day_harib14f_asmhead)
add_dependencies(wine 17_day_harib14f_asmhead_wine)
add_dependencies(od 17_day_harib14f_asmhead_od)

#----------------------------------------------------------
########### next target ###############
set(ipl10.img_SRCS ${root_SOURCE_DIR}/projects/17_day/harib14f/ipl10.nas)
set(ipl10.img_OUTS ${root_BINARY_DIR}/projects/17_day/harib14f/ipl10.img)
set(ipl10_wine.img_OUTS ${root_BINARY_DIR}/projects/17_day/harib14f/ipl10_wine.img)

add_custom_target(17_day_harib14f_ipl10
  COMMAND ${root_BINARY_DIR}/src/opennask ${ipl10.img_SRCS} ${ipl10.img_OUTS}
)
add_custom_target(17_day_harib14f_ipl10_wine
  COMMAND ${WINE} ${WINE_NASK} ${ipl10.img_SRCS} ${ipl10_wine.img_OUTS}
)
add_custom_target(17_day_harib14f_ipl10_od
  COMMAND ${OD} -t x1 ${ipl10.img_OUTS}      > ${ipl10.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${ipl10_wine.img_OUTS} > ${ipl10_wine.img_OUTS}_t.txt
  COMMAND diff -s ${ipl10.img_OUTS}_f.txt ${ipl10_wine.img_OUTS}_t.txt
)

add_dependencies(17_day_harib14f_ipl10_od 17_day_harib14f_ipl10)
add_dependencies(17_day_harib14f_ipl10_od 17_day_harib14f_ipl10_wine)

add_dependencies(images 17_day_harib14f_ipl10)
add_dependencies(wine 17_day_harib14f_ipl10_wine)
add_dependencies(od 17_day_harib14f_ipl10_od)

#----------------------------------------------------------
########### next target ###############
set(naskfunc.img_SRCS ${root_SOURCE_DIR}/projects/17_day/harib14f/naskfunc.nas)
set(naskfunc.img_OUTS ${root_BINARY_DIR}/projects/17_day/harib14f/naskfunc.img)
set(naskfunc_wine.img_OUTS ${root_BINARY_DIR}/projects/17_day/harib14f/naskfunc_wine.img)

add_custom_target(17_day_harib14f_naskfunc
  COMMAND ${root_BINARY_DIR}/src/opennask ${naskfunc.img_SRCS} ${naskfunc.img_OUTS}
)
add_custom_target(17_day_harib14f_naskfunc_wine
  COMMAND ${WINE} ${WINE_NASK} ${naskfunc.img_SRCS} ${naskfunc_wine.img_OUTS}
)
add_custom_target(17_day_harib14f_naskfunc_od
  COMMAND ${OD} -t x1 ${naskfunc.img_OUTS}      > ${naskfunc.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${naskfunc_wine.img_OUTS} > ${naskfunc_wine.img_OUTS}_t.txt
  COMMAND diff -s ${naskfunc.img_OUTS}_f.txt ${naskfunc_wine.img_OUTS}_t.txt
)

add_dependencies(17_day_harib14f_naskfunc_od 17_day_harib14f_naskfunc)
add_dependencies(17_day_harib14f_naskfunc_od 17_day_harib14f_naskfunc_wine)

add_dependencies(images 17_day_harib14f_naskfunc)
add_dependencies(wine 17_day_harib14f_naskfunc_wine)
add_dependencies(od 17_day_harib14f_naskfunc_od)

#----------------------------------------------------------
