#----------------------------------------------------------
message(STATUS "Entering directory projects/14_day/harib11c/")

set(NASK ${root_BINARY_DIR}/src/opennask)
set(FONT ${root_BINARY_DIR}/src/makefont)
set(B2O  ${root_BINARY_DIR}/src/bin2obj)
set(CONV ${root_BINARY_DIR}/objconv/objconv)
set(14_day_harib11c_OS    ${root_BINARY_DIR}/projects/14_day/harib11c/os.img)
set(14_day_harib11c_SYS	  ${root_BINARY_DIR}/projects/14_day/harib11c/os.sys)
set(14_day_harib11c_IPLB  ${root_BINARY_DIR}/projects/14_day/harib11c/ipl.bin)
set(14_day_harib11c_IPLS  ${root_SOURCE_DIR}/projects/14_day/harib11c/ipl10.nas)
set(14_day_harib11c_HEADB ${root_BINARY_DIR}/projects/14_day/harib11c/asmhead.bin)
set(14_day_harib11c_HEADS ${root_SOURCE_DIR}/projects/14_day/harib11c/asmhead.nas)
set(14_day_harib11c_BOOTB ${root_BINARY_DIR}/projects/14_day/harib11c/boot.bin)
set(14_day_harib11c_CCO	  ${root_BINARY_DIR}/projects/14_day/harib11c/boot.o)
set(14_day_harib11c_CCS	  ${root_SOURCE_DIR}/projects/14_day/harib11c/*.c)
set(14_day_harib11c_LDS	  ${root_SOURCE_DIR}/projects/os.lds)
set(14_day_harib11c_FUNCS ${root_SOURCE_DIR}/projects/14_day/harib11c/naskfunc.nas)
set(14_day_harib11c_FUNCO ${root_BINARY_DIR}/projects/14_day/harib11c/naskfunc.o)
set(14_day_harib11c_FONTS ${root_SOURCE_DIR}/projects/14_day/harib11c/hankaku.txt)
set(14_day_harib11c_FONTB ${root_BINARY_DIR}/projects/14_day/harib11c/hankaku.bin)
set(14_day_harib11c_FONTO ${root_BINARY_DIR}/projects/14_day/harib11c/hankaku.o)
set(14_day_harib11c_LIBGE ${root_BINARY_DIR}/golibc/libgo.a)
set(14_day_harib11c_LIBGC ${root_BINARY_DIR}/projects/14_day/harib11c/libgo.o)
set(14_day_harib11c_WILDOBJ ${root_BINARY_DIR}/projects/14_day/harib11c/*.o)


add_custom_target(14_day_harib11c_run
  COMMAND ${QEMU} ${QEMUOPT} ${14_day_harib11c_OS}
  DEPENDS 14_day_harib11c_img
)
add_custom_target(14_day_harib11c_debug
  COMMAND ${QEMU} -s -S ${QEMUOPT} ${14_day_harib11c_OS} -redir tcp:5555:127.0.0.1:1234 &
  DEPENDS 14_day_harib11c_img
)
add_custom_target(14_day_harib11c_clean
  COMMAND rm -f ${14_day_harib11c_OS}
  COMMAND rm -f ${14_day_harib11c_SYS}
  COMMAND rm -f ${14_day_harib11c_IPLB}
  COMMAND rm -f ${14_day_harib11c_HEADB}
  COMMAND rm -f ${14_day_harib11c_BOOTB}
  COMMAND rm -f ${14_day_harib11c_WILDOBJ}
)
add_custom_target(14_day_harib11c_ipl
  COMMAND ${NASK} ${14_day_harib11c_IPLS} ${14_day_harib11c_IPLB}
)
add_custom_target(14_day_harib11c_sys
  COMMAND ${NASK} ${14_day_harib11c_HEADS} ${14_day_harib11c_HEADB}
  COMMAND ${FONT} ${14_day_harib11c_FONTS} ${14_day_harib11c_FONTB}
  COMMAND ${B2O}  ${14_day_harib11c_FONTB} ${14_day_harib11c_FONTO} _hankaku
  COMMAND ${CONV} -fcoff32 -nu ${14_day_harib11c_LIBGE} ${14_day_harib11c_LIBGC}
  COMMAND ${NASK} ${14_day_harib11c_FUNCS} ${14_day_harib11c_FUNCO}
  COMMAND gcc ${BINOPT} -T ${14_day_harib11c_LDS} ${14_day_harib11c_CCS} ${14_day_harib11c_WILDOBJ} -o ${14_day_harib11c_BOOTB}
  COMMAND cat ${14_day_harib11c_HEADB} ${14_day_harib11c_BOOTB} > ${14_day_harib11c_SYS}
  DEPENDS 14_day_harib11c_ipl
)
add_custom_target(14_day_harib11c_img
  COMMAND mformat -f 1440 -l HARIBOTEOS -N 0xffffffff -C -B ${14_day_harib11c_IPLB} -i ${14_day_harib11c_OS}
  COMMAND mcopy -i ${14_day_harib11c_OS} ${14_day_harib11c_SYS} ::
  DEPENDS 14_day_harib11c_sys
)
########### next target ###############
set(asmhead.img_SRCS ${root_SOURCE_DIR}/projects/14_day/harib11c/asmhead.nas)
set(asmhead.img_OUTS ${root_BINARY_DIR}/projects/14_day/harib11c/asmhead.img)
set(asmhead_wine.img_OUTS ${root_BINARY_DIR}/projects/14_day/harib11c/asmhead_wine.img)

add_custom_target(14_day_harib11c_asmhead
  COMMAND ${root_BINARY_DIR}/src/opennask ${asmhead.img_SRCS} ${asmhead.img_OUTS}
)
add_custom_target(14_day_harib11c_asmhead_wine
  COMMAND ${WINE} ${WINE_NASK} ${asmhead.img_SRCS} ${asmhead_wine.img_OUTS}
)
add_custom_target(14_day_harib11c_asmhead_od
  COMMAND ${OD} -t x1 ${asmhead.img_OUTS}      > ${asmhead.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${asmhead_wine.img_OUTS} > ${asmhead_wine.img_OUTS}_t.txt
  COMMAND diff -s ${asmhead.img_OUTS}_f.txt ${asmhead_wine.img_OUTS}_t.txt
)

add_dependencies(14_day_harib11c_asmhead_od 14_day_harib11c_asmhead)
add_dependencies(14_day_harib11c_asmhead_od 14_day_harib11c_asmhead_wine)

add_dependencies(images 14_day_harib11c_asmhead)
add_dependencies(wine 14_day_harib11c_asmhead_wine)
add_dependencies(od 14_day_harib11c_asmhead_od)

#----------------------------------------------------------
########### next target ###############
set(ipl10.img_SRCS ${root_SOURCE_DIR}/projects/14_day/harib11c/ipl10.nas)
set(ipl10.img_OUTS ${root_BINARY_DIR}/projects/14_day/harib11c/ipl10.img)
set(ipl10_wine.img_OUTS ${root_BINARY_DIR}/projects/14_day/harib11c/ipl10_wine.img)

add_custom_target(14_day_harib11c_ipl10
  COMMAND ${root_BINARY_DIR}/src/opennask ${ipl10.img_SRCS} ${ipl10.img_OUTS}
)
add_custom_target(14_day_harib11c_ipl10_wine
  COMMAND ${WINE} ${WINE_NASK} ${ipl10.img_SRCS} ${ipl10_wine.img_OUTS}
)
add_custom_target(14_day_harib11c_ipl10_od
  COMMAND ${OD} -t x1 ${ipl10.img_OUTS}      > ${ipl10.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${ipl10_wine.img_OUTS} > ${ipl10_wine.img_OUTS}_t.txt
  COMMAND diff -s ${ipl10.img_OUTS}_f.txt ${ipl10_wine.img_OUTS}_t.txt
)

add_dependencies(14_day_harib11c_ipl10_od 14_day_harib11c_ipl10)
add_dependencies(14_day_harib11c_ipl10_od 14_day_harib11c_ipl10_wine)

add_dependencies(images 14_day_harib11c_ipl10)
add_dependencies(wine 14_day_harib11c_ipl10_wine)
add_dependencies(od 14_day_harib11c_ipl10_od)

#----------------------------------------------------------
########### next target ###############
set(naskfunc.img_SRCS ${root_SOURCE_DIR}/projects/14_day/harib11c/naskfunc.nas)
set(naskfunc.img_OUTS ${root_BINARY_DIR}/projects/14_day/harib11c/naskfunc.img)
set(naskfunc_wine.img_OUTS ${root_BINARY_DIR}/projects/14_day/harib11c/naskfunc_wine.img)

add_custom_target(14_day_harib11c_naskfunc
  COMMAND ${root_BINARY_DIR}/src/opennask ${naskfunc.img_SRCS} ${naskfunc.img_OUTS}
)
add_custom_target(14_day_harib11c_naskfunc_wine
  COMMAND ${WINE} ${WINE_NASK} ${naskfunc.img_SRCS} ${naskfunc_wine.img_OUTS}
)
add_custom_target(14_day_harib11c_naskfunc_od
  COMMAND ${OD} -t x1 ${naskfunc.img_OUTS}      > ${naskfunc.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${naskfunc_wine.img_OUTS} > ${naskfunc_wine.img_OUTS}_t.txt
  COMMAND diff -s ${naskfunc.img_OUTS}_f.txt ${naskfunc_wine.img_OUTS}_t.txt
)

add_dependencies(14_day_harib11c_naskfunc_od 14_day_harib11c_naskfunc)
add_dependencies(14_day_harib11c_naskfunc_od 14_day_harib11c_naskfunc_wine)

add_dependencies(images 14_day_harib11c_naskfunc)
add_dependencies(wine 14_day_harib11c_naskfunc_wine)
add_dependencies(od 14_day_harib11c_naskfunc_od)

#----------------------------------------------------------
