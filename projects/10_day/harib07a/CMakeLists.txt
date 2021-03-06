#----------------------------------------------------------
message(STATUS "Entering directory projects/10_day/harib07a/")

set(NASK ${root_BINARY_DIR}/src/opennask)
set(FONT ${root_BINARY_DIR}/src/makefont)
set(B2O  ${root_BINARY_DIR}/src/bin2obj)
set(CONV ${root_BINARY_DIR}/objconv/objconv)
set(10_day_harib07a_OS    ${root_BINARY_DIR}/projects/10_day/harib07a/os.img)
set(10_day_harib07a_SYS	  ${root_BINARY_DIR}/projects/10_day/harib07a/os.sys)
set(10_day_harib07a_IPLB  ${root_BINARY_DIR}/projects/10_day/harib07a/ipl.bin)
set(10_day_harib07a_IPLS  ${root_SOURCE_DIR}/projects/10_day/harib07a/ipl10.nas)
set(10_day_harib07a_HEADB ${root_BINARY_DIR}/projects/10_day/harib07a/asmhead.bin)
set(10_day_harib07a_HEADS ${root_SOURCE_DIR}/projects/10_day/harib07a/asmhead.nas)
set(10_day_harib07a_BOOTB ${root_BINARY_DIR}/projects/10_day/harib07a/boot.bin)
set(10_day_harib07a_CCO	  ${root_BINARY_DIR}/projects/10_day/harib07a/boot.o)
set(10_day_harib07a_CCS	  ${root_SOURCE_DIR}/projects/10_day/harib07a/*.c)
set(10_day_harib07a_LDS	  ${root_SOURCE_DIR}/projects/os.lds)
set(10_day_harib07a_FUNCS ${root_SOURCE_DIR}/projects/10_day/harib07a/naskfunc.nas)
set(10_day_harib07a_FUNCO ${root_BINARY_DIR}/projects/10_day/harib07a/naskfunc.o)
set(10_day_harib07a_FONTS ${root_SOURCE_DIR}/projects/10_day/harib07a/hankaku.txt)
set(10_day_harib07a_FONTB ${root_BINARY_DIR}/projects/10_day/harib07a/hankaku.bin)
set(10_day_harib07a_FONTO ${root_BINARY_DIR}/projects/10_day/harib07a/hankaku.o)
set(10_day_harib07a_LIBGE ${root_BINARY_DIR}/golibc/libgo.a)
set(10_day_harib07a_LIBGC ${root_BINARY_DIR}/projects/10_day/harib07a/libgo.o)
set(10_day_harib07a_WILDOBJ ${root_BINARY_DIR}/projects/10_day/harib07a/*.o)


add_custom_target(10_day_harib07a_run
  COMMAND ${QEMU} ${QEMUOPT} ${10_day_harib07a_OS}
  DEPENDS 10_day_harib07a_img
)
add_custom_target(10_day_harib07a_debug
  COMMAND ${QEMU} -s -S ${QEMUOPT} ${10_day_harib07a_OS} ${QEMU_DEBUG_OPT}
  DEPENDS 10_day_harib07a_img
)
add_custom_target(10_day_harib07a_clean
  COMMAND rm -f ${10_day_harib07a_OS}
  COMMAND rm -f ${10_day_harib07a_SYS}
  COMMAND rm -f ${10_day_harib07a_IPLB}
  COMMAND rm -f ${10_day_harib07a_HEADB}
  COMMAND rm -f ${10_day_harib07a_BOOTB}
  COMMAND rm -f ${10_day_harib07a_WILDOBJ}
)
add_custom_target(10_day_harib07a_ipl
  COMMAND ${NASK} ${10_day_harib07a_IPLS} ${10_day_harib07a_IPLB}
)
add_custom_target(10_day_harib07a_sys
  COMMAND ${NASK} ${10_day_harib07a_HEADS} ${10_day_harib07a_HEADB}
  COMMAND ${FONT} ${10_day_harib07a_FONTS} ${10_day_harib07a_FONTB}
  COMMAND ${B2O}  ${10_day_harib07a_FONTB} ${10_day_harib07a_FONTO} _hankaku
  COMMAND ${CONV} -fcoff32 -nu ${10_day_harib07a_LIBGE} ${10_day_harib07a_LIBGC}
  COMMAND ${NASK} ${10_day_harib07a_FUNCS} ${10_day_harib07a_FUNCO}
  COMMAND gcc ${BINOPT} -T ${10_day_harib07a_LDS} ${10_day_harib07a_CCS} ${10_day_harib07a_WILDOBJ} -o ${10_day_harib07a_BOOTB}
  COMMAND cat ${10_day_harib07a_HEADB} ${10_day_harib07a_BOOTB} > ${10_day_harib07a_SYS}
  DEPENDS 10_day_harib07a_ipl
)
add_custom_target(10_day_harib07a_img
  COMMAND mformat -f 1440 -l HARIBOTEOS -N 0xffffffff -C -B ${10_day_harib07a_IPLB} -i ${10_day_harib07a_OS}
  COMMAND mcopy -i ${10_day_harib07a_OS} ${10_day_harib07a_SYS} ::
  DEPENDS 10_day_harib07a_sys
)
########### next target ###############
set(asmhead.img_SRCS ${root_SOURCE_DIR}/projects/10_day/harib07a/asmhead.nas)
set(asmhead.img_OUTS ${root_BINARY_DIR}/projects/10_day/harib07a/asmhead.img)
set(asmhead_wine.img_OUTS ${root_BINARY_DIR}/projects/10_day/harib07a/asmhead_wine.img)

add_custom_target(10_day_harib07a_asmhead
  COMMAND ${root_BINARY_DIR}/src/opennask ${asmhead.img_SRCS} ${asmhead.img_OUTS}
)
add_custom_target(10_day_harib07a_asmhead_wine
  COMMAND ${WINE} ${WINE_NASK} ${asmhead.img_SRCS} ${asmhead_wine.img_OUTS}
)
add_custom_target(10_day_harib07a_asmhead_od
  COMMAND ${OD} -t x1 ${asmhead.img_OUTS}      > ${asmhead.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${asmhead_wine.img_OUTS} > ${asmhead_wine.img_OUTS}_t.txt
  COMMAND diff -s ${asmhead.img_OUTS}_f.txt ${asmhead_wine.img_OUTS}_t.txt
)

add_dependencies(10_day_harib07a_asmhead_od 10_day_harib07a_asmhead)
add_dependencies(10_day_harib07a_asmhead_od 10_day_harib07a_asmhead_wine)

add_dependencies(images 10_day_harib07a_asmhead)
add_dependencies(wine 10_day_harib07a_asmhead_wine)
add_dependencies(od 10_day_harib07a_asmhead_od)

#----------------------------------------------------------
########### next target ###############
set(ipl10.img_SRCS ${root_SOURCE_DIR}/projects/10_day/harib07a/ipl10.nas)
set(ipl10.img_OUTS ${root_BINARY_DIR}/projects/10_day/harib07a/ipl10.img)
set(ipl10_wine.img_OUTS ${root_BINARY_DIR}/projects/10_day/harib07a/ipl10_wine.img)

add_custom_target(10_day_harib07a_ipl10
  COMMAND ${root_BINARY_DIR}/src/opennask ${ipl10.img_SRCS} ${ipl10.img_OUTS}
)
add_custom_target(10_day_harib07a_ipl10_wine
  COMMAND ${WINE} ${WINE_NASK} ${ipl10.img_SRCS} ${ipl10_wine.img_OUTS}
)
add_custom_target(10_day_harib07a_ipl10_od
  COMMAND ${OD} -t x1 ${ipl10.img_OUTS}      > ${ipl10.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${ipl10_wine.img_OUTS} > ${ipl10_wine.img_OUTS}_t.txt
  COMMAND diff -s ${ipl10.img_OUTS}_f.txt ${ipl10_wine.img_OUTS}_t.txt
)

add_dependencies(10_day_harib07a_ipl10_od 10_day_harib07a_ipl10)
add_dependencies(10_day_harib07a_ipl10_od 10_day_harib07a_ipl10_wine)

add_dependencies(images 10_day_harib07a_ipl10)
add_dependencies(wine 10_day_harib07a_ipl10_wine)
add_dependencies(od 10_day_harib07a_ipl10_od)

#----------------------------------------------------------
########### next target ###############
set(naskfunc.img_SRCS ${root_SOURCE_DIR}/projects/10_day/harib07a/naskfunc.nas)
set(naskfunc.img_OUTS ${root_BINARY_DIR}/projects/10_day/harib07a/naskfunc.img)
set(naskfunc_wine.img_OUTS ${root_BINARY_DIR}/projects/10_day/harib07a/naskfunc_wine.img)

add_custom_target(10_day_harib07a_naskfunc
  COMMAND ${root_BINARY_DIR}/src/opennask ${naskfunc.img_SRCS} ${naskfunc.img_OUTS}
)
add_custom_target(10_day_harib07a_naskfunc_wine
  COMMAND ${WINE} ${WINE_NASK} ${naskfunc.img_SRCS} ${naskfunc_wine.img_OUTS}
)
add_custom_target(10_day_harib07a_naskfunc_od
  COMMAND ${OD} -t x1 ${naskfunc.img_OUTS}      > ${naskfunc.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${naskfunc_wine.img_OUTS} > ${naskfunc_wine.img_OUTS}_t.txt
  COMMAND diff -s ${naskfunc.img_OUTS}_f.txt ${naskfunc_wine.img_OUTS}_t.txt
)

add_dependencies(10_day_harib07a_naskfunc_od 10_day_harib07a_naskfunc)
add_dependencies(10_day_harib07a_naskfunc_od 10_day_harib07a_naskfunc_wine)

add_dependencies(images 10_day_harib07a_naskfunc)
add_dependencies(wine 10_day_harib07a_naskfunc_wine)
add_dependencies(od 10_day_harib07a_naskfunc_od)

#----------------------------------------------------------
