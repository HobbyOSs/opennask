#----------------------------------------------------------
message(STATUS "Entering directory projects/10_day/harib07b/")

set(NASK ${root_BINARY_DIR}/src/opennask)
set(FONT ${root_BINARY_DIR}/src/makefont)
set(B2O  ${root_BINARY_DIR}/src/bin2obj)
set(CONV ${root_BINARY_DIR}/objconv/objconv)
set(10_day_harib07b_OS    ${root_BINARY_DIR}/projects/10_day/harib07b/os.img)
set(10_day_harib07b_SYS	  ${root_BINARY_DIR}/projects/10_day/harib07b/os.sys)
set(10_day_harib07b_IPLB  ${root_BINARY_DIR}/projects/10_day/harib07b/ipl.bin)
set(10_day_harib07b_IPLS  ${root_SOURCE_DIR}/projects/10_day/harib07b/ipl10.nas)
set(10_day_harib07b_HEADB ${root_BINARY_DIR}/projects/10_day/harib07b/asmhead.bin)
set(10_day_harib07b_HEADS ${root_SOURCE_DIR}/projects/10_day/harib07b/asmhead.nas)
set(10_day_harib07b_BOOTB ${root_BINARY_DIR}/projects/10_day/harib07b/boot.bin)
set(10_day_harib07b_CCO	  ${root_BINARY_DIR}/projects/10_day/harib07b/boot.o)
set(10_day_harib07b_CCS	  ${root_SOURCE_DIR}/projects/10_day/harib07b/*.c)
set(10_day_harib07b_LDS	  ${root_SOURCE_DIR}/projects/os.lds)
set(10_day_harib07b_FUNCS ${root_SOURCE_DIR}/projects/10_day/harib07b/naskfunc.nas)
set(10_day_harib07b_FUNCO ${root_BINARY_DIR}/projects/10_day/harib07b/naskfunc.o)
set(10_day_harib07b_FONTS ${root_SOURCE_DIR}/projects/10_day/harib07b/hankaku.txt)
set(10_day_harib07b_FONTB ${root_BINARY_DIR}/projects/10_day/harib07b/hankaku.bin)
set(10_day_harib07b_FONTO ${root_BINARY_DIR}/projects/10_day/harib07b/hankaku.o)
set(10_day_harib07b_LIBGE ${root_BINARY_DIR}/golibc/libgo.a)
set(10_day_harib07b_LIBGC ${root_BINARY_DIR}/projects/10_day/harib07b/libgo.o)
set(10_day_harib07b_WILDOBJ ${root_BINARY_DIR}/projects/10_day/harib07b/*.o)


add_custom_target(10_day_harib07b_run
  COMMAND ${QEMU} ${QEMUOPT} ${10_day_harib07b_OS}
  DEPENDS 10_day_harib07b_img
)
add_custom_target(10_day_harib07b_debug
  COMMAND ${QEMU} -s -S ${QEMUOPT} ${10_day_harib07b_OS} ${QEMU_DEBUG_OPT}
  DEPENDS 10_day_harib07b_img
)
add_custom_target(10_day_harib07b_clean
  COMMAND rm -f ${10_day_harib07b_OS}
  COMMAND rm -f ${10_day_harib07b_SYS}
  COMMAND rm -f ${10_day_harib07b_IPLB}
  COMMAND rm -f ${10_day_harib07b_HEADB}
  COMMAND rm -f ${10_day_harib07b_BOOTB}
  COMMAND rm -f ${10_day_harib07b_WILDOBJ}
)
add_custom_target(10_day_harib07b_ipl
  COMMAND ${NASK} ${10_day_harib07b_IPLS} ${10_day_harib07b_IPLB}
)
add_custom_target(10_day_harib07b_sys
  COMMAND ${NASK} ${10_day_harib07b_HEADS} ${10_day_harib07b_HEADB}
  COMMAND ${FONT} ${10_day_harib07b_FONTS} ${10_day_harib07b_FONTB}
  COMMAND ${B2O}  ${10_day_harib07b_FONTB} ${10_day_harib07b_FONTO} _hankaku
  COMMAND ${CONV} -fcoff32 -nu ${10_day_harib07b_LIBGE} ${10_day_harib07b_LIBGC}
  COMMAND ${NASK} ${10_day_harib07b_FUNCS} ${10_day_harib07b_FUNCO}
  COMMAND gcc ${BINOPT} -T ${10_day_harib07b_LDS} ${10_day_harib07b_CCS} ${10_day_harib07b_WILDOBJ} -o ${10_day_harib07b_BOOTB}
  COMMAND cat ${10_day_harib07b_HEADB} ${10_day_harib07b_BOOTB} > ${10_day_harib07b_SYS}
  DEPENDS 10_day_harib07b_ipl
)
add_custom_target(10_day_harib07b_img
  COMMAND mformat -f 1440 -l HARIBOTEOS -N 0xffffffff -C -B ${10_day_harib07b_IPLB} -i ${10_day_harib07b_OS}
  COMMAND mcopy -i ${10_day_harib07b_OS} ${10_day_harib07b_SYS} ::
  DEPENDS 10_day_harib07b_sys
)
########### next target ###############
set(asmhead.img_SRCS ${root_SOURCE_DIR}/projects/10_day/harib07b/asmhead.nas)
set(asmhead.img_OUTS ${root_BINARY_DIR}/projects/10_day/harib07b/asmhead.img)
set(asmhead_wine.img_OUTS ${root_BINARY_DIR}/projects/10_day/harib07b/asmhead_wine.img)

add_custom_target(10_day_harib07b_asmhead
  COMMAND ${root_BINARY_DIR}/src/opennask ${asmhead.img_SRCS} ${asmhead.img_OUTS}
)
add_custom_target(10_day_harib07b_asmhead_wine
  COMMAND ${WINE} ${WINE_NASK} ${asmhead.img_SRCS} ${asmhead_wine.img_OUTS}
)
add_custom_target(10_day_harib07b_asmhead_od
  COMMAND ${OD} -t x1 ${asmhead.img_OUTS}      > ${asmhead.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${asmhead_wine.img_OUTS} > ${asmhead_wine.img_OUTS}_t.txt
  COMMAND diff -s ${asmhead.img_OUTS}_f.txt ${asmhead_wine.img_OUTS}_t.txt
)

add_dependencies(10_day_harib07b_asmhead_od 10_day_harib07b_asmhead)
add_dependencies(10_day_harib07b_asmhead_od 10_day_harib07b_asmhead_wine)

add_dependencies(images 10_day_harib07b_asmhead)
add_dependencies(wine 10_day_harib07b_asmhead_wine)
add_dependencies(od 10_day_harib07b_asmhead_od)

#----------------------------------------------------------
########### next target ###############
set(ipl10.img_SRCS ${root_SOURCE_DIR}/projects/10_day/harib07b/ipl10.nas)
set(ipl10.img_OUTS ${root_BINARY_DIR}/projects/10_day/harib07b/ipl10.img)
set(ipl10_wine.img_OUTS ${root_BINARY_DIR}/projects/10_day/harib07b/ipl10_wine.img)

add_custom_target(10_day_harib07b_ipl10
  COMMAND ${root_BINARY_DIR}/src/opennask ${ipl10.img_SRCS} ${ipl10.img_OUTS}
)
add_custom_target(10_day_harib07b_ipl10_wine
  COMMAND ${WINE} ${WINE_NASK} ${ipl10.img_SRCS} ${ipl10_wine.img_OUTS}
)
add_custom_target(10_day_harib07b_ipl10_od
  COMMAND ${OD} -t x1 ${ipl10.img_OUTS}      > ${ipl10.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${ipl10_wine.img_OUTS} > ${ipl10_wine.img_OUTS}_t.txt
  COMMAND diff -s ${ipl10.img_OUTS}_f.txt ${ipl10_wine.img_OUTS}_t.txt
)

add_dependencies(10_day_harib07b_ipl10_od 10_day_harib07b_ipl10)
add_dependencies(10_day_harib07b_ipl10_od 10_day_harib07b_ipl10_wine)

add_dependencies(images 10_day_harib07b_ipl10)
add_dependencies(wine 10_day_harib07b_ipl10_wine)
add_dependencies(od 10_day_harib07b_ipl10_od)

#----------------------------------------------------------
########### next target ###############
set(naskfunc.img_SRCS ${root_SOURCE_DIR}/projects/10_day/harib07b/naskfunc.nas)
set(naskfunc.img_OUTS ${root_BINARY_DIR}/projects/10_day/harib07b/naskfunc.img)
set(naskfunc_wine.img_OUTS ${root_BINARY_DIR}/projects/10_day/harib07b/naskfunc_wine.img)

add_custom_target(10_day_harib07b_naskfunc
  COMMAND ${root_BINARY_DIR}/src/opennask ${naskfunc.img_SRCS} ${naskfunc.img_OUTS}
)
add_custom_target(10_day_harib07b_naskfunc_wine
  COMMAND ${WINE} ${WINE_NASK} ${naskfunc.img_SRCS} ${naskfunc_wine.img_OUTS}
)
add_custom_target(10_day_harib07b_naskfunc_od
  COMMAND ${OD} -t x1 ${naskfunc.img_OUTS}      > ${naskfunc.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${naskfunc_wine.img_OUTS} > ${naskfunc_wine.img_OUTS}_t.txt
  COMMAND diff -s ${naskfunc.img_OUTS}_f.txt ${naskfunc_wine.img_OUTS}_t.txt
)

add_dependencies(10_day_harib07b_naskfunc_od 10_day_harib07b_naskfunc)
add_dependencies(10_day_harib07b_naskfunc_od 10_day_harib07b_naskfunc_wine)

add_dependencies(images 10_day_harib07b_naskfunc)
add_dependencies(wine 10_day_harib07b_naskfunc_wine)
add_dependencies(od 10_day_harib07b_naskfunc_od)

#----------------------------------------------------------
