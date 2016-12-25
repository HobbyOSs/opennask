#----------------------------------------------------------
message(STATUS "Entering directory projects/28_day/harib25a/haribote/")

set(NASK ${root_BINARY_DIR}/src/opennask)
set(FONT ${root_BINARY_DIR}/src/makefont)
set(B2O  ${root_BINARY_DIR}/src/bin2obj)
set(CONV ${root_BINARY_DIR}/objconv/objconv)
set(28_day_harib25a_haribote_OS    ${root_BINARY_DIR}/projects/28_day/harib25a/haribote/os.img)
set(28_day_harib25a_haribote_SYS	  ${root_BINARY_DIR}/projects/28_day/harib25a/haribote/os.sys)
set(28_day_harib25a_haribote_IPLB  ${root_BINARY_DIR}/projects/28_day/harib25a/haribote/ipl.bin)
set(28_day_harib25a_haribote_IPLS  ${root_SOURCE_DIR}/projects/28_day/harib25a/haribote/ipl10.nas)
set(28_day_harib25a_haribote_HEADB ${root_BINARY_DIR}/projects/28_day/harib25a/haribote/asmhead.bin)
set(28_day_harib25a_haribote_HEADS ${root_SOURCE_DIR}/projects/28_day/harib25a/haribote/asmhead.nas)
set(28_day_harib25a_haribote_BOOTB ${root_BINARY_DIR}/projects/28_day/harib25a/haribote/boot.bin)
set(28_day_harib25a_haribote_CCO	  ${root_BINARY_DIR}/projects/28_day/harib25a/haribote/boot.o)
set(28_day_harib25a_haribote_CCS	  ${root_SOURCE_DIR}/projects/28_day/harib25a/haribote/*.c)
set(28_day_harib25a_haribote_LDS	  ${root_SOURCE_DIR}/projects/os.lds)
set(28_day_harib25a_haribote_FUNCS ${root_SOURCE_DIR}/projects/28_day/harib25a/haribote/naskfunc.nas)
set(28_day_harib25a_haribote_FUNCO ${root_BINARY_DIR}/projects/28_day/harib25a/haribote/naskfunc.o)
set(28_day_harib25a_haribote_FONTS ${root_SOURCE_DIR}/projects/28_day/harib25a/haribote/hankaku.txt)
set(28_day_harib25a_haribote_FONTB ${root_BINARY_DIR}/projects/28_day/harib25a/haribote/hankaku.bin)
set(28_day_harib25a_haribote_FONTO ${root_BINARY_DIR}/projects/28_day/harib25a/haribote/hankaku.o)
set(28_day_harib25a_haribote_LIBGE ${root_BINARY_DIR}/golibc/libgo.a)
set(28_day_harib25a_haribote_LIBGC ${root_BINARY_DIR}/projects/28_day/harib25a/haribote/libgo.o)
set(28_day_harib25a_haribote_WILDOBJ ${root_BINARY_DIR}/projects/28_day/harib25a/haribote/*.o)


add_custom_target(28_day_harib25a_haribote_run
  COMMAND ${QEMU} ${QEMUOPT} ${28_day_harib25a_haribote_OS}
  DEPENDS 28_day_harib25a_haribote_img
)
add_custom_target(28_day_harib25a_haribote_debug
  COMMAND ${QEMU} -s -S ${QEMUOPT} ${28_day_harib25a_haribote_OS} -redir tcp:5555:127.0.0.1:1234 &
  DEPENDS 28_day_harib25a_haribote_img
)
add_custom_target(28_day_harib25a_haribote_clean
  COMMAND rm -f ${28_day_harib25a_haribote_OS}
  COMMAND rm -f ${28_day_harib25a_haribote_SYS}
  COMMAND rm -f ${28_day_harib25a_haribote_IPLB}
  COMMAND rm -f ${28_day_harib25a_haribote_HEADB}
  COMMAND rm -f ${28_day_harib25a_haribote_BOOTB}
  COMMAND rm -f ${28_day_harib25a_haribote_WILDOBJ}
)
add_custom_target(28_day_harib25a_haribote_ipl
  COMMAND ${NASK} ${28_day_harib25a_haribote_IPLS} ${28_day_harib25a_haribote_IPLB}
)
add_custom_target(28_day_harib25a_haribote_sys
  COMMAND ${NASK} ${28_day_harib25a_haribote_HEADS} ${28_day_harib25a_haribote_HEADB}
  COMMAND ${FONT} ${28_day_harib25a_haribote_FONTS} ${28_day_harib25a_haribote_FONTB}
  COMMAND ${B2O}  ${28_day_harib25a_haribote_FONTB} ${28_day_harib25a_haribote_FONTO} _hankaku
  COMMAND ${CONV} -fcoff32 -nu ${28_day_harib25a_haribote_LIBGE} ${28_day_harib25a_haribote_LIBGC}
  COMMAND ${NASK} ${28_day_harib25a_haribote_FUNCS} ${28_day_harib25a_haribote_FUNCO}
  COMMAND gcc ${BINOPT} -T ${28_day_harib25a_haribote_LDS} ${28_day_harib25a_haribote_CCS} ${28_day_harib25a_haribote_WILDOBJ} -o ${28_day_harib25a_haribote_BOOTB}
  COMMAND cat ${28_day_harib25a_haribote_HEADB} ${28_day_harib25a_haribote_BOOTB} > ${28_day_harib25a_haribote_SYS}
  DEPENDS 28_day_harib25a_haribote_ipl
)
add_custom_target(28_day_harib25a_haribote_img
  COMMAND mformat -f 1440 -l HARIBOTEOS -N 0xffffffff -C -B ${28_day_harib25a_haribote_IPLB} -i ${28_day_harib25a_haribote_OS}
  COMMAND mcopy -i ${28_day_harib25a_haribote_OS} ${28_day_harib25a_haribote_SYS} ::
  DEPENDS 28_day_harib25a_haribote_sys
)
########### next target ###############
set(asmhead.img_SRCS ${root_SOURCE_DIR}/projects/28_day/harib25a/haribote/asmhead.nas)
set(asmhead.img_OUTS ${root_BINARY_DIR}/projects/28_day/harib25a/haribote/asmhead.img)
set(asmhead_wine.img_OUTS ${root_BINARY_DIR}/projects/28_day/harib25a/haribote/asmhead_wine.img)

add_custom_target(28_day_harib25a_haribote_asmhead
  COMMAND ${root_BINARY_DIR}/src/opennask ${asmhead.img_SRCS} ${asmhead.img_OUTS}
)
add_custom_target(28_day_harib25a_haribote_asmhead_wine
  COMMAND ${WINE} ${WINE_NASK} ${asmhead.img_SRCS} ${asmhead_wine.img_OUTS}
)
add_custom_target(28_day_harib25a_haribote_asmhead_od
  COMMAND ${OD} -t x1 ${asmhead.img_OUTS}      > ${asmhead.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${asmhead_wine.img_OUTS} > ${asmhead_wine.img_OUTS}_t.txt
  COMMAND diff -s ${asmhead.img_OUTS}_f.txt ${asmhead_wine.img_OUTS}_t.txt
)

add_dependencies(28_day_harib25a_haribote_asmhead_od 28_day_harib25a_haribote_asmhead)
add_dependencies(28_day_harib25a_haribote_asmhead_od 28_day_harib25a_haribote_asmhead_wine)

add_dependencies(images 28_day_harib25a_haribote_asmhead)
add_dependencies(wine 28_day_harib25a_haribote_asmhead_wine)
add_dependencies(od 28_day_harib25a_haribote_asmhead_od)

#----------------------------------------------------------
########### next target ###############
set(ipl10.img_SRCS ${root_SOURCE_DIR}/projects/28_day/harib25a/haribote/ipl10.nas)
set(ipl10.img_OUTS ${root_BINARY_DIR}/projects/28_day/harib25a/haribote/ipl10.img)
set(ipl10_wine.img_OUTS ${root_BINARY_DIR}/projects/28_day/harib25a/haribote/ipl10_wine.img)

add_custom_target(28_day_harib25a_haribote_ipl10
  COMMAND ${root_BINARY_DIR}/src/opennask ${ipl10.img_SRCS} ${ipl10.img_OUTS}
)
add_custom_target(28_day_harib25a_haribote_ipl10_wine
  COMMAND ${WINE} ${WINE_NASK} ${ipl10.img_SRCS} ${ipl10_wine.img_OUTS}
)
add_custom_target(28_day_harib25a_haribote_ipl10_od
  COMMAND ${OD} -t x1 ${ipl10.img_OUTS}      > ${ipl10.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${ipl10_wine.img_OUTS} > ${ipl10_wine.img_OUTS}_t.txt
  COMMAND diff -s ${ipl10.img_OUTS}_f.txt ${ipl10_wine.img_OUTS}_t.txt
)

add_dependencies(28_day_harib25a_haribote_ipl10_od 28_day_harib25a_haribote_ipl10)
add_dependencies(28_day_harib25a_haribote_ipl10_od 28_day_harib25a_haribote_ipl10_wine)

add_dependencies(images 28_day_harib25a_haribote_ipl10)
add_dependencies(wine 28_day_harib25a_haribote_ipl10_wine)
add_dependencies(od 28_day_harib25a_haribote_ipl10_od)

#----------------------------------------------------------
########### next target ###############
set(naskfunc.img_SRCS ${root_SOURCE_DIR}/projects/28_day/harib25a/haribote/naskfunc.nas)
set(naskfunc.img_OUTS ${root_BINARY_DIR}/projects/28_day/harib25a/haribote/naskfunc.img)
set(naskfunc_wine.img_OUTS ${root_BINARY_DIR}/projects/28_day/harib25a/haribote/naskfunc_wine.img)

add_custom_target(28_day_harib25a_haribote_naskfunc
  COMMAND ${root_BINARY_DIR}/src/opennask ${naskfunc.img_SRCS} ${naskfunc.img_OUTS}
)
add_custom_target(28_day_harib25a_haribote_naskfunc_wine
  COMMAND ${WINE} ${WINE_NASK} ${naskfunc.img_SRCS} ${naskfunc_wine.img_OUTS}
)
add_custom_target(28_day_harib25a_haribote_naskfunc_od
  COMMAND ${OD} -t x1 ${naskfunc.img_OUTS}      > ${naskfunc.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${naskfunc_wine.img_OUTS} > ${naskfunc_wine.img_OUTS}_t.txt
  COMMAND diff -s ${naskfunc.img_OUTS}_f.txt ${naskfunc_wine.img_OUTS}_t.txt
)

add_dependencies(28_day_harib25a_haribote_naskfunc_od 28_day_harib25a_haribote_naskfunc)
add_dependencies(28_day_harib25a_haribote_naskfunc_od 28_day_harib25a_haribote_naskfunc_wine)

add_dependencies(images 28_day_harib25a_haribote_naskfunc)
add_dependencies(wine 28_day_harib25a_haribote_naskfunc_wine)
add_dependencies(od 28_day_harib25a_haribote_naskfunc_od)

#----------------------------------------------------------
