#----------------------------------------------------------
message(STATUS "Entering directory projects/13_day/harib10b/")

set(NASK ${root_BINARY_DIR}/src/opennask)
set(FONT ${root_BINARY_DIR}/src/makefont)
set(B2O  ${root_BINARY_DIR}/src/bin2obj)
set(CONV ${root_BINARY_DIR}/objconv/objconv)
set(13_day_harib10b_OS    ${root_BINARY_DIR}/projects/13_day/harib10b/os.img)
set(13_day_harib10b_SYS	  ${root_BINARY_DIR}/projects/13_day/harib10b/os.sys)
set(13_day_harib10b_IPLB  ${root_BINARY_DIR}/projects/13_day/harib10b/ipl.bin)
set(13_day_harib10b_IPLS  ${root_SOURCE_DIR}/projects/13_day/harib10b/ipl10.nas)
set(13_day_harib10b_HEADB ${root_BINARY_DIR}/projects/13_day/harib10b/asmhead.bin)
set(13_day_harib10b_HEADS ${root_SOURCE_DIR}/projects/13_day/harib10b/asmhead.nas)
set(13_day_harib10b_BOOTB ${root_BINARY_DIR}/projects/13_day/harib10b/boot.bin)
set(13_day_harib10b_CCO	  ${root_BINARY_DIR}/projects/13_day/harib10b/boot.o)
set(13_day_harib10b_CCS	  ${root_SOURCE_DIR}/projects/13_day/harib10b/*.c)
set(13_day_harib10b_LDS	  ${root_SOURCE_DIR}/projects/os.lds)
set(13_day_harib10b_FUNCS ${root_SOURCE_DIR}/projects/13_day/harib10b/naskfunc.nas)
set(13_day_harib10b_FUNCO ${root_BINARY_DIR}/projects/13_day/harib10b/naskfunc.o)
set(13_day_harib10b_FONTS ${root_SOURCE_DIR}/projects/13_day/harib10b/hankaku.txt)
set(13_day_harib10b_FONTB ${root_BINARY_DIR}/projects/13_day/harib10b/hankaku.bin)
set(13_day_harib10b_FONTO ${root_BINARY_DIR}/projects/13_day/harib10b/hankaku.o)
set(13_day_harib10b_LIBGE ${root_BINARY_DIR}/golibc/libgo.a)
set(13_day_harib10b_LIBGC ${root_BINARY_DIR}/projects/13_day/harib10b/libgo.o)
set(13_day_harib10b_WILDOBJ ${root_BINARY_DIR}/projects/13_day/harib10b/*.o)


add_custom_target(13_day_harib10b_run
  COMMAND ${QEMU} ${QEMUOPT} ${13_day_harib10b_OS}
  DEPENDS 13_day_harib10b_img
)
add_custom_target(13_day_harib10b_debug
  COMMAND ${QEMU} -s -S ${QEMUOPT} ${13_day_harib10b_OS} -redir tcp:5555:127.0.0.1:1234 &
  DEPENDS 13_day_harib10b_img
)
add_custom_target(13_day_harib10b_clean
  COMMAND rm -f ${13_day_harib10b_OS}
  COMMAND rm -f ${13_day_harib10b_SYS}
  COMMAND rm -f ${13_day_harib10b_IPLB}
  COMMAND rm -f ${13_day_harib10b_HEADB}
  COMMAND rm -f ${13_day_harib10b_BOOTB}
  COMMAND rm -f ${13_day_harib10b_WILDOBJ}
)
add_custom_target(13_day_harib10b_ipl
  COMMAND ${NASK} ${13_day_harib10b_IPLS} ${13_day_harib10b_IPLB}
)
add_custom_target(13_day_harib10b_sys
  COMMAND ${NASK} ${13_day_harib10b_HEADS} ${13_day_harib10b_HEADB}
  COMMAND ${FONT} ${13_day_harib10b_FONTS} ${13_day_harib10b_FONTB}
  COMMAND ${B2O}  ${13_day_harib10b_FONTB} ${13_day_harib10b_FONTO} _hankaku
  COMMAND ${CONV} -fcoff32 -nu ${13_day_harib10b_LIBGE} ${13_day_harib10b_LIBGC}
  COMMAND ${NASK} ${13_day_harib10b_FUNCS} ${13_day_harib10b_FUNCO}
  COMMAND gcc ${BINOPT} -T ${13_day_harib10b_LDS} ${13_day_harib10b_CCS} ${13_day_harib10b_WILDOBJ} -o ${13_day_harib10b_BOOTB}
  COMMAND cat ${13_day_harib10b_HEADB} ${13_day_harib10b_BOOTB} > ${13_day_harib10b_SYS}
  DEPENDS 13_day_harib10b_ipl
)
add_custom_target(13_day_harib10b_img
  COMMAND mformat -f 1440 -l HARIBOTEOS -N 0xffffffff -C -B ${13_day_harib10b_IPLB} -i ${13_day_harib10b_OS}
  COMMAND mcopy -i ${13_day_harib10b_OS} ${13_day_harib10b_SYS} ::
  DEPENDS 13_day_harib10b_sys
)
########### next target ###############
set(asmhead.img_SRCS ${root_SOURCE_DIR}/projects/13_day/harib10b/asmhead.nas)
set(asmhead.img_OUTS ${root_BINARY_DIR}/projects/13_day/harib10b/asmhead.img)
set(asmhead_wine.img_OUTS ${root_BINARY_DIR}/projects/13_day/harib10b/asmhead_wine.img)

add_custom_target(13_day_harib10b_asmhead
  COMMAND ${root_BINARY_DIR}/src/opennask ${asmhead.img_SRCS} ${asmhead.img_OUTS}
)
add_custom_target(13_day_harib10b_asmhead_wine
  COMMAND ${WINE} ${WINE_NASK} ${asmhead.img_SRCS} ${asmhead_wine.img_OUTS}
)
add_custom_target(13_day_harib10b_asmhead_od
  COMMAND ${OD} -t x1 ${asmhead.img_OUTS}      > ${asmhead.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${asmhead_wine.img_OUTS} > ${asmhead_wine.img_OUTS}_t.txt
  COMMAND diff -s ${asmhead.img_OUTS}_f.txt ${asmhead_wine.img_OUTS}_t.txt
)

add_dependencies(13_day_harib10b_asmhead_od 13_day_harib10b_asmhead)
add_dependencies(13_day_harib10b_asmhead_od 13_day_harib10b_asmhead_wine)

add_dependencies(images 13_day_harib10b_asmhead)
add_dependencies(wine 13_day_harib10b_asmhead_wine)
add_dependencies(od 13_day_harib10b_asmhead_od)

#----------------------------------------------------------
########### next target ###############
set(ipl10.img_SRCS ${root_SOURCE_DIR}/projects/13_day/harib10b/ipl10.nas)
set(ipl10.img_OUTS ${root_BINARY_DIR}/projects/13_day/harib10b/ipl10.img)
set(ipl10_wine.img_OUTS ${root_BINARY_DIR}/projects/13_day/harib10b/ipl10_wine.img)

add_custom_target(13_day_harib10b_ipl10
  COMMAND ${root_BINARY_DIR}/src/opennask ${ipl10.img_SRCS} ${ipl10.img_OUTS}
)
add_custom_target(13_day_harib10b_ipl10_wine
  COMMAND ${WINE} ${WINE_NASK} ${ipl10.img_SRCS} ${ipl10_wine.img_OUTS}
)
add_custom_target(13_day_harib10b_ipl10_od
  COMMAND ${OD} -t x1 ${ipl10.img_OUTS}      > ${ipl10.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${ipl10_wine.img_OUTS} > ${ipl10_wine.img_OUTS}_t.txt
  COMMAND diff -s ${ipl10.img_OUTS}_f.txt ${ipl10_wine.img_OUTS}_t.txt
)

add_dependencies(13_day_harib10b_ipl10_od 13_day_harib10b_ipl10)
add_dependencies(13_day_harib10b_ipl10_od 13_day_harib10b_ipl10_wine)

add_dependencies(images 13_day_harib10b_ipl10)
add_dependencies(wine 13_day_harib10b_ipl10_wine)
add_dependencies(od 13_day_harib10b_ipl10_od)

#----------------------------------------------------------
########### next target ###############
set(naskfunc.img_SRCS ${root_SOURCE_DIR}/projects/13_day/harib10b/naskfunc.nas)
set(naskfunc.img_OUTS ${root_BINARY_DIR}/projects/13_day/harib10b/naskfunc.img)
set(naskfunc_wine.img_OUTS ${root_BINARY_DIR}/projects/13_day/harib10b/naskfunc_wine.img)

add_custom_target(13_day_harib10b_naskfunc
  COMMAND ${root_BINARY_DIR}/src/opennask ${naskfunc.img_SRCS} ${naskfunc.img_OUTS}
)
add_custom_target(13_day_harib10b_naskfunc_wine
  COMMAND ${WINE} ${WINE_NASK} ${naskfunc.img_SRCS} ${naskfunc_wine.img_OUTS}
)
add_custom_target(13_day_harib10b_naskfunc_od
  COMMAND ${OD} -t x1 ${naskfunc.img_OUTS}      > ${naskfunc.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${naskfunc_wine.img_OUTS} > ${naskfunc_wine.img_OUTS}_t.txt
  COMMAND diff -s ${naskfunc.img_OUTS}_f.txt ${naskfunc_wine.img_OUTS}_t.txt
)

add_dependencies(13_day_harib10b_naskfunc_od 13_day_harib10b_naskfunc)
add_dependencies(13_day_harib10b_naskfunc_od 13_day_harib10b_naskfunc_wine)

add_dependencies(images 13_day_harib10b_naskfunc)
add_dependencies(wine 13_day_harib10b_naskfunc_wine)
add_dependencies(od 13_day_harib10b_naskfunc_od)

#----------------------------------------------------------
