#----------------------------------------------------------
message(STATUS "Entering directory projects/19_day/harib16b/")

set(NASK ${root_BINARY_DIR}/src/opennask)
set(FONT ${root_BINARY_DIR}/src/makefont)
set(B2O  ${root_BINARY_DIR}/src/bin2obj)
set(CONV ${root_BINARY_DIR}/objconv/objconv)
set(19_day_harib16b_OS    ${root_BINARY_DIR}/projects/19_day/harib16b/os.img)
set(19_day_harib16b_SYS	  ${root_BINARY_DIR}/projects/19_day/harib16b/os.sys)
set(19_day_harib16b_IPLB  ${root_BINARY_DIR}/projects/19_day/harib16b/ipl.bin)
set(19_day_harib16b_IPLS  ${root_SOURCE_DIR}/projects/19_day/harib16b/ipl10.nas)
set(19_day_harib16b_HEADB ${root_BINARY_DIR}/projects/19_day/harib16b/asmhead.bin)
set(19_day_harib16b_HEADS ${root_SOURCE_DIR}/projects/19_day/harib16b/asmhead.nas)
set(19_day_harib16b_BOOTB ${root_BINARY_DIR}/projects/19_day/harib16b/boot.bin)
set(19_day_harib16b_CCO	  ${root_BINARY_DIR}/projects/19_day/harib16b/boot.o)
set(19_day_harib16b_CCS	  ${root_SOURCE_DIR}/projects/19_day/harib16b/*.c)
set(19_day_harib16b_LDS	  ${root_SOURCE_DIR}/projects/os.lds)
set(19_day_harib16b_FUNCS ${root_SOURCE_DIR}/projects/19_day/harib16b/naskfunc.nas)
set(19_day_harib16b_FUNCO ${root_BINARY_DIR}/projects/19_day/harib16b/naskfunc.o)
set(19_day_harib16b_FONTS ${root_SOURCE_DIR}/projects/19_day/harib16b/hankaku.txt)
set(19_day_harib16b_FONTB ${root_BINARY_DIR}/projects/19_day/harib16b/hankaku.bin)
set(19_day_harib16b_FONTO ${root_BINARY_DIR}/projects/19_day/harib16b/hankaku.o)
set(19_day_harib16b_LIBGE ${root_BINARY_DIR}/golibc/libgo.a)
set(19_day_harib16b_LIBGC ${root_BINARY_DIR}/projects/19_day/harib16b/libgo.o)
set(19_day_harib16b_WILDOBJ ${root_BINARY_DIR}/projects/19_day/harib16b/*.o)


add_custom_target(19_day_harib16b_run
  COMMAND ${QEMU} ${QEMUOPT} ${19_day_harib16b_OS}
  DEPENDS 19_day_harib16b_img
)
add_custom_target(19_day_harib16b_debug
  COMMAND ${QEMU} -s -S ${QEMUOPT} ${19_day_harib16b_OS} -redir tcp:5555:127.0.0.1:1234 &
  DEPENDS 19_day_harib16b_img
)
add_custom_target(19_day_harib16b_clean
  COMMAND rm -f ${19_day_harib16b_OS}
  COMMAND rm -f ${19_day_harib16b_SYS}
  COMMAND rm -f ${19_day_harib16b_IPLB}
  COMMAND rm -f ${19_day_harib16b_HEADB}
  COMMAND rm -f ${19_day_harib16b_BOOTB}
  COMMAND rm -f ${19_day_harib16b_WILDOBJ}
)
add_custom_target(19_day_harib16b_ipl
  COMMAND ${NASK} ${19_day_harib16b_IPLS} ${19_day_harib16b_IPLB}
)
add_custom_target(19_day_harib16b_sys
  COMMAND ${NASK} ${19_day_harib16b_HEADS} ${19_day_harib16b_HEADB}
  COMMAND ${FONT} ${19_day_harib16b_FONTS} ${19_day_harib16b_FONTB}
  COMMAND ${B2O}  ${19_day_harib16b_FONTB} ${19_day_harib16b_FONTO} _hankaku
  COMMAND ${CONV} -fcoff32 -nu ${19_day_harib16b_LIBGE} ${19_day_harib16b_LIBGC}
  COMMAND ${NASK} ${19_day_harib16b_FUNCS} ${19_day_harib16b_FUNCO}
  COMMAND gcc ${BINOPT} -T ${19_day_harib16b_LDS} ${19_day_harib16b_CCS} ${19_day_harib16b_WILDOBJ} -o ${19_day_harib16b_BOOTB}
  COMMAND cat ${19_day_harib16b_HEADB} ${19_day_harib16b_BOOTB} > ${19_day_harib16b_SYS}
  DEPENDS 19_day_harib16b_ipl
)
add_custom_target(19_day_harib16b_img
  COMMAND mformat -f 1440 -l HARIBOTEOS -N 0xffffffff -C -B ${19_day_harib16b_IPLB} -i ${19_day_harib16b_OS}
  COMMAND mcopy -i ${19_day_harib16b_OS} ${19_day_harib16b_SYS} ::
  DEPENDS 19_day_harib16b_sys
)
########### next target ###############
set(asmhead.img_SRCS ${root_SOURCE_DIR}/projects/19_day/harib16b/asmhead.nas)
set(asmhead.img_OUTS ${root_BINARY_DIR}/projects/19_day/harib16b/asmhead.img)
set(asmhead_wine.img_OUTS ${root_BINARY_DIR}/projects/19_day/harib16b/asmhead_wine.img)

add_custom_target(19_day_harib16b_asmhead
  COMMAND ${root_BINARY_DIR}/src/opennask ${asmhead.img_SRCS} ${asmhead.img_OUTS}
)
add_custom_target(19_day_harib16b_asmhead_wine
  COMMAND ${WINE} ${WINE_NASK} ${asmhead.img_SRCS} ${asmhead_wine.img_OUTS}
)
add_custom_target(19_day_harib16b_asmhead_od
  COMMAND ${OD} -t x1 ${asmhead.img_OUTS}      > ${asmhead.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${asmhead_wine.img_OUTS} > ${asmhead_wine.img_OUTS}_t.txt
  COMMAND diff -s ${asmhead.img_OUTS}_f.txt ${asmhead_wine.img_OUTS}_t.txt
)

add_dependencies(19_day_harib16b_asmhead_od 19_day_harib16b_asmhead)
add_dependencies(19_day_harib16b_asmhead_od 19_day_harib16b_asmhead_wine)

add_dependencies(images 19_day_harib16b_asmhead)
add_dependencies(wine 19_day_harib16b_asmhead_wine)
add_dependencies(od 19_day_harib16b_asmhead_od)

#----------------------------------------------------------
########### next target ###############
set(ipl10.img_SRCS ${root_SOURCE_DIR}/projects/19_day/harib16b/ipl10.nas)
set(ipl10.img_OUTS ${root_BINARY_DIR}/projects/19_day/harib16b/ipl10.img)
set(ipl10_wine.img_OUTS ${root_BINARY_DIR}/projects/19_day/harib16b/ipl10_wine.img)

add_custom_target(19_day_harib16b_ipl10
  COMMAND ${root_BINARY_DIR}/src/opennask ${ipl10.img_SRCS} ${ipl10.img_OUTS}
)
add_custom_target(19_day_harib16b_ipl10_wine
  COMMAND ${WINE} ${WINE_NASK} ${ipl10.img_SRCS} ${ipl10_wine.img_OUTS}
)
add_custom_target(19_day_harib16b_ipl10_od
  COMMAND ${OD} -t x1 ${ipl10.img_OUTS}      > ${ipl10.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${ipl10_wine.img_OUTS} > ${ipl10_wine.img_OUTS}_t.txt
  COMMAND diff -s ${ipl10.img_OUTS}_f.txt ${ipl10_wine.img_OUTS}_t.txt
)

add_dependencies(19_day_harib16b_ipl10_od 19_day_harib16b_ipl10)
add_dependencies(19_day_harib16b_ipl10_od 19_day_harib16b_ipl10_wine)

add_dependencies(images 19_day_harib16b_ipl10)
add_dependencies(wine 19_day_harib16b_ipl10_wine)
add_dependencies(od 19_day_harib16b_ipl10_od)

#----------------------------------------------------------
########### next target ###############
set(naskfunc.img_SRCS ${root_SOURCE_DIR}/projects/19_day/harib16b/naskfunc.nas)
set(naskfunc.img_OUTS ${root_BINARY_DIR}/projects/19_day/harib16b/naskfunc.img)
set(naskfunc_wine.img_OUTS ${root_BINARY_DIR}/projects/19_day/harib16b/naskfunc_wine.img)

add_custom_target(19_day_harib16b_naskfunc
  COMMAND ${root_BINARY_DIR}/src/opennask ${naskfunc.img_SRCS} ${naskfunc.img_OUTS}
)
add_custom_target(19_day_harib16b_naskfunc_wine
  COMMAND ${WINE} ${WINE_NASK} ${naskfunc.img_SRCS} ${naskfunc_wine.img_OUTS}
)
add_custom_target(19_day_harib16b_naskfunc_od
  COMMAND ${OD} -t x1 ${naskfunc.img_OUTS}      > ${naskfunc.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${naskfunc_wine.img_OUTS} > ${naskfunc_wine.img_OUTS}_t.txt
  COMMAND diff -s ${naskfunc.img_OUTS}_f.txt ${naskfunc_wine.img_OUTS}_t.txt
)

add_dependencies(19_day_harib16b_naskfunc_od 19_day_harib16b_naskfunc)
add_dependencies(19_day_harib16b_naskfunc_od 19_day_harib16b_naskfunc_wine)

add_dependencies(images 19_day_harib16b_naskfunc)
add_dependencies(wine 19_day_harib16b_naskfunc_wine)
add_dependencies(od 19_day_harib16b_naskfunc_od)

#----------------------------------------------------------
