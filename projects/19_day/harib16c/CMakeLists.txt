#----------------------------------------------------------
message(STATUS "Entering directory projects/19_day/harib16c/")

set(NASK ${root_BINARY_DIR}/src/opennask)
set(FONT ${root_BINARY_DIR}/src/makefont)
set(B2O  ${root_BINARY_DIR}/src/bin2obj)
set(CONV ${root_BINARY_DIR}/objconv/objconv)
set(19_day_harib16c_OS    ${root_BINARY_DIR}/projects/19_day/harib16c/os.img)
set(19_day_harib16c_SYS	  ${root_BINARY_DIR}/projects/19_day/harib16c/os.sys)
set(19_day_harib16c_IPLB  ${root_BINARY_DIR}/projects/19_day/harib16c/ipl.bin)
set(19_day_harib16c_IPLS  ${root_SOURCE_DIR}/projects/19_day/harib16c/ipl10.nas)
set(19_day_harib16c_HEADB ${root_BINARY_DIR}/projects/19_day/harib16c/asmhead.bin)
set(19_day_harib16c_HEADS ${root_SOURCE_DIR}/projects/19_day/harib16c/asmhead.nas)
set(19_day_harib16c_BOOTB ${root_BINARY_DIR}/projects/19_day/harib16c/boot.bin)
set(19_day_harib16c_CCO	  ${root_BINARY_DIR}/projects/19_day/harib16c/boot.o)
set(19_day_harib16c_CCS	  ${root_SOURCE_DIR}/projects/19_day/harib16c/*.c)
set(19_day_harib16c_LDS	  ${root_SOURCE_DIR}/projects/os.lds)
set(19_day_harib16c_FUNCS ${root_SOURCE_DIR}/projects/19_day/harib16c/naskfunc.nas)
set(19_day_harib16c_FUNCO ${root_BINARY_DIR}/projects/19_day/harib16c/naskfunc.o)
set(19_day_harib16c_FONTS ${root_SOURCE_DIR}/projects/19_day/harib16c/hankaku.txt)
set(19_day_harib16c_FONTB ${root_BINARY_DIR}/projects/19_day/harib16c/hankaku.bin)
set(19_day_harib16c_FONTO ${root_BINARY_DIR}/projects/19_day/harib16c/hankaku.o)
set(19_day_harib16c_LIBGE ${root_BINARY_DIR}/golibc/libgo.a)
set(19_day_harib16c_LIBGC ${root_BINARY_DIR}/projects/19_day/harib16c/libgo.o)
set(19_day_harib16c_WILDOBJ ${root_BINARY_DIR}/projects/19_day/harib16c/*.o)


add_custom_target(19_day_harib16c_run
  COMMAND ${QEMU} ${QEMUOPT} ${19_day_harib16c_OS}
  DEPENDS 19_day_harib16c_img
)
add_custom_target(19_day_harib16c_debug
  COMMAND ${QEMU} -s -S ${QEMUOPT} ${19_day_harib16c_OS} ${QEMU_DEBUG_OPT}
  DEPENDS 19_day_harib16c_img
)
add_custom_target(19_day_harib16c_clean
  COMMAND rm -f ${19_day_harib16c_OS}
  COMMAND rm -f ${19_day_harib16c_SYS}
  COMMAND rm -f ${19_day_harib16c_IPLB}
  COMMAND rm -f ${19_day_harib16c_HEADB}
  COMMAND rm -f ${19_day_harib16c_BOOTB}
  COMMAND rm -f ${19_day_harib16c_WILDOBJ}
)
add_custom_target(19_day_harib16c_ipl
  COMMAND ${NASK} ${19_day_harib16c_IPLS} ${19_day_harib16c_IPLB}
)
add_custom_target(19_day_harib16c_sys
  COMMAND ${NASK} ${19_day_harib16c_HEADS} ${19_day_harib16c_HEADB}
  COMMAND ${FONT} ${19_day_harib16c_FONTS} ${19_day_harib16c_FONTB}
  COMMAND ${B2O}  ${19_day_harib16c_FONTB} ${19_day_harib16c_FONTO} _hankaku
  COMMAND ${CONV} -fcoff32 -nu ${19_day_harib16c_LIBGE} ${19_day_harib16c_LIBGC}
  COMMAND ${NASK} ${19_day_harib16c_FUNCS} ${19_day_harib16c_FUNCO}
  COMMAND gcc ${BINOPT} -T ${19_day_harib16c_LDS} ${19_day_harib16c_CCS} ${19_day_harib16c_WILDOBJ} -o ${19_day_harib16c_BOOTB}
  COMMAND cat ${19_day_harib16c_HEADB} ${19_day_harib16c_BOOTB} > ${19_day_harib16c_SYS}
  DEPENDS 19_day_harib16c_ipl
)
add_custom_target(19_day_harib16c_img
  COMMAND mformat -f 1440 -l HARIBOTEOS -N 0xffffffff -C -B ${19_day_harib16c_IPLB} -i ${19_day_harib16c_OS}
  COMMAND mcopy -i ${19_day_harib16c_OS} ${19_day_harib16c_SYS} ::
  DEPENDS 19_day_harib16c_sys
)
########### next target ###############
set(asmhead.img_SRCS ${root_SOURCE_DIR}/projects/19_day/harib16c/asmhead.nas)
set(asmhead.img_OUTS ${root_BINARY_DIR}/projects/19_day/harib16c/asmhead.img)
set(asmhead_wine.img_OUTS ${root_BINARY_DIR}/projects/19_day/harib16c/asmhead_wine.img)

add_custom_target(19_day_harib16c_asmhead
  COMMAND ${root_BINARY_DIR}/src/opennask ${asmhead.img_SRCS} ${asmhead.img_OUTS}
)
add_custom_target(19_day_harib16c_asmhead_wine
  COMMAND ${WINE} ${WINE_NASK} ${asmhead.img_SRCS} ${asmhead_wine.img_OUTS}
)
add_custom_target(19_day_harib16c_asmhead_od
  COMMAND ${OD} -t x1 ${asmhead.img_OUTS}      > ${asmhead.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${asmhead_wine.img_OUTS} > ${asmhead_wine.img_OUTS}_t.txt
  COMMAND diff -s ${asmhead.img_OUTS}_f.txt ${asmhead_wine.img_OUTS}_t.txt
)

add_dependencies(19_day_harib16c_asmhead_od 19_day_harib16c_asmhead)
add_dependencies(19_day_harib16c_asmhead_od 19_day_harib16c_asmhead_wine)

add_dependencies(images 19_day_harib16c_asmhead)
add_dependencies(wine 19_day_harib16c_asmhead_wine)
add_dependencies(od 19_day_harib16c_asmhead_od)

#----------------------------------------------------------
########### next target ###############
set(ipl10.img_SRCS ${root_SOURCE_DIR}/projects/19_day/harib16c/ipl10.nas)
set(ipl10.img_OUTS ${root_BINARY_DIR}/projects/19_day/harib16c/ipl10.img)
set(ipl10_wine.img_OUTS ${root_BINARY_DIR}/projects/19_day/harib16c/ipl10_wine.img)

add_custom_target(19_day_harib16c_ipl10
  COMMAND ${root_BINARY_DIR}/src/opennask ${ipl10.img_SRCS} ${ipl10.img_OUTS}
)
add_custom_target(19_day_harib16c_ipl10_wine
  COMMAND ${WINE} ${WINE_NASK} ${ipl10.img_SRCS} ${ipl10_wine.img_OUTS}
)
add_custom_target(19_day_harib16c_ipl10_od
  COMMAND ${OD} -t x1 ${ipl10.img_OUTS}      > ${ipl10.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${ipl10_wine.img_OUTS} > ${ipl10_wine.img_OUTS}_t.txt
  COMMAND diff -s ${ipl10.img_OUTS}_f.txt ${ipl10_wine.img_OUTS}_t.txt
)

add_dependencies(19_day_harib16c_ipl10_od 19_day_harib16c_ipl10)
add_dependencies(19_day_harib16c_ipl10_od 19_day_harib16c_ipl10_wine)

add_dependencies(images 19_day_harib16c_ipl10)
add_dependencies(wine 19_day_harib16c_ipl10_wine)
add_dependencies(od 19_day_harib16c_ipl10_od)

#----------------------------------------------------------
########### next target ###############
set(naskfunc.img_SRCS ${root_SOURCE_DIR}/projects/19_day/harib16c/naskfunc.nas)
set(naskfunc.img_OUTS ${root_BINARY_DIR}/projects/19_day/harib16c/naskfunc.img)
set(naskfunc_wine.img_OUTS ${root_BINARY_DIR}/projects/19_day/harib16c/naskfunc_wine.img)

add_custom_target(19_day_harib16c_naskfunc
  COMMAND ${root_BINARY_DIR}/src/opennask ${naskfunc.img_SRCS} ${naskfunc.img_OUTS}
)
add_custom_target(19_day_harib16c_naskfunc_wine
  COMMAND ${WINE} ${WINE_NASK} ${naskfunc.img_SRCS} ${naskfunc_wine.img_OUTS}
)
add_custom_target(19_day_harib16c_naskfunc_od
  COMMAND ${OD} -t x1 ${naskfunc.img_OUTS}      > ${naskfunc.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${naskfunc_wine.img_OUTS} > ${naskfunc_wine.img_OUTS}_t.txt
  COMMAND diff -s ${naskfunc.img_OUTS}_f.txt ${naskfunc_wine.img_OUTS}_t.txt
)

add_dependencies(19_day_harib16c_naskfunc_od 19_day_harib16c_naskfunc)
add_dependencies(19_day_harib16c_naskfunc_od 19_day_harib16c_naskfunc_wine)

add_dependencies(images 19_day_harib16c_naskfunc)
add_dependencies(wine 19_day_harib16c_naskfunc_wine)
add_dependencies(od 19_day_harib16c_naskfunc_od)

#----------------------------------------------------------
