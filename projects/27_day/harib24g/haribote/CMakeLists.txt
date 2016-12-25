#----------------------------------------------------------
message(STATUS "Entering directory projects/27_day/harib24g/haribote/")

set(NASK ${root_BINARY_DIR}/src/opennask)
set(FONT ${root_BINARY_DIR}/src/makefont)
set(B2O  ${root_BINARY_DIR}/src/bin2obj)
set(CONV ${root_BINARY_DIR}/objconv/objconv)
set(27_day_harib24g_haribote_OS    ${root_BINARY_DIR}/projects/27_day/harib24g/haribote/os.img)
set(27_day_harib24g_haribote_SYS	  ${root_BINARY_DIR}/projects/27_day/harib24g/haribote/os.sys)
set(27_day_harib24g_haribote_IPLB  ${root_BINARY_DIR}/projects/27_day/harib24g/haribote/ipl.bin)
set(27_day_harib24g_haribote_IPLS  ${root_SOURCE_DIR}/projects/27_day/harib24g/haribote/ipl10.nas)
set(27_day_harib24g_haribote_HEADB ${root_BINARY_DIR}/projects/27_day/harib24g/haribote/asmhead.bin)
set(27_day_harib24g_haribote_HEADS ${root_SOURCE_DIR}/projects/27_day/harib24g/haribote/asmhead.nas)
set(27_day_harib24g_haribote_BOOTB ${root_BINARY_DIR}/projects/27_day/harib24g/haribote/boot.bin)
set(27_day_harib24g_haribote_CCO	  ${root_BINARY_DIR}/projects/27_day/harib24g/haribote/boot.o)
set(27_day_harib24g_haribote_CCS	  ${root_SOURCE_DIR}/projects/27_day/harib24g/haribote/*.c)
set(27_day_harib24g_haribote_LDS	  ${root_SOURCE_DIR}/projects/os.lds)
set(27_day_harib24g_haribote_FUNCS ${root_SOURCE_DIR}/projects/27_day/harib24g/haribote/naskfunc.nas)
set(27_day_harib24g_haribote_FUNCO ${root_BINARY_DIR}/projects/27_day/harib24g/haribote/naskfunc.o)
set(27_day_harib24g_haribote_FONTS ${root_SOURCE_DIR}/projects/27_day/harib24g/haribote/hankaku.txt)
set(27_day_harib24g_haribote_FONTB ${root_BINARY_DIR}/projects/27_day/harib24g/haribote/hankaku.bin)
set(27_day_harib24g_haribote_FONTO ${root_BINARY_DIR}/projects/27_day/harib24g/haribote/hankaku.o)
set(27_day_harib24g_haribote_LIBGE ${root_BINARY_DIR}/golibc/libgo.a)
set(27_day_harib24g_haribote_LIBGC ${root_BINARY_DIR}/projects/27_day/harib24g/haribote/libgo.o)
set(27_day_harib24g_haribote_WILDOBJ ${root_BINARY_DIR}/projects/27_day/harib24g/haribote/*.o)


add_custom_target(27_day_harib24g_haribote_run
  COMMAND ${QEMU} ${QEMUOPT} ${27_day_harib24g_haribote_OS}
  DEPENDS 27_day_harib24g_haribote_img
)
add_custom_target(27_day_harib24g_haribote_debug
  COMMAND ${QEMU} -s -S ${QEMUOPT} ${27_day_harib24g_haribote_OS} -redir tcp:5555:127.0.0.1:1234 &
  DEPENDS 27_day_harib24g_haribote_img
)
add_custom_target(27_day_harib24g_haribote_clean
  COMMAND rm -f ${27_day_harib24g_haribote_OS}
  COMMAND rm -f ${27_day_harib24g_haribote_SYS}
  COMMAND rm -f ${27_day_harib24g_haribote_IPLB}
  COMMAND rm -f ${27_day_harib24g_haribote_HEADB}
  COMMAND rm -f ${27_day_harib24g_haribote_BOOTB}
  COMMAND rm -f ${27_day_harib24g_haribote_WILDOBJ}
)
add_custom_target(27_day_harib24g_haribote_ipl
  COMMAND ${NASK} ${27_day_harib24g_haribote_IPLS} ${27_day_harib24g_haribote_IPLB}
)
add_custom_target(27_day_harib24g_haribote_sys
  COMMAND ${NASK} ${27_day_harib24g_haribote_HEADS} ${27_day_harib24g_haribote_HEADB}
  COMMAND ${FONT} ${27_day_harib24g_haribote_FONTS} ${27_day_harib24g_haribote_FONTB}
  COMMAND ${B2O}  ${27_day_harib24g_haribote_FONTB} ${27_day_harib24g_haribote_FONTO} _hankaku
  COMMAND ${CONV} -fcoff32 -nu ${27_day_harib24g_haribote_LIBGE} ${27_day_harib24g_haribote_LIBGC}
  COMMAND ${NASK} ${27_day_harib24g_haribote_FUNCS} ${27_day_harib24g_haribote_FUNCO}
  COMMAND gcc ${BINOPT} -T ${27_day_harib24g_haribote_LDS} ${27_day_harib24g_haribote_CCS} ${27_day_harib24g_haribote_WILDOBJ} -o ${27_day_harib24g_haribote_BOOTB}
  COMMAND cat ${27_day_harib24g_haribote_HEADB} ${27_day_harib24g_haribote_BOOTB} > ${27_day_harib24g_haribote_SYS}
  DEPENDS 27_day_harib24g_haribote_ipl
)
add_custom_target(27_day_harib24g_haribote_img
  COMMAND mformat -f 1440 -l HARIBOTEOS -N 0xffffffff -C -B ${27_day_harib24g_haribote_IPLB} -i ${27_day_harib24g_haribote_OS}
  COMMAND mcopy -i ${27_day_harib24g_haribote_OS} ${27_day_harib24g_haribote_SYS} ::
  DEPENDS 27_day_harib24g_haribote_sys
)
########### next target ###############
set(asmhead.img_SRCS ${root_SOURCE_DIR}/projects/27_day/harib24g/haribote/asmhead.nas)
set(asmhead.img_OUTS ${root_BINARY_DIR}/projects/27_day/harib24g/haribote/asmhead.img)
set(asmhead_wine.img_OUTS ${root_BINARY_DIR}/projects/27_day/harib24g/haribote/asmhead_wine.img)

add_custom_target(27_day_harib24g_haribote_asmhead
  COMMAND ${root_BINARY_DIR}/src/opennask ${asmhead.img_SRCS} ${asmhead.img_OUTS}
)
add_custom_target(27_day_harib24g_haribote_asmhead_wine
  COMMAND ${WINE} ${WINE_NASK} ${asmhead.img_SRCS} ${asmhead_wine.img_OUTS}
)
add_custom_target(27_day_harib24g_haribote_asmhead_od
  COMMAND ${OD} -t x1 ${asmhead.img_OUTS}      > ${asmhead.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${asmhead_wine.img_OUTS} > ${asmhead_wine.img_OUTS}_t.txt
  COMMAND diff -s ${asmhead.img_OUTS}_f.txt ${asmhead_wine.img_OUTS}_t.txt
)

add_dependencies(27_day_harib24g_haribote_asmhead_od 27_day_harib24g_haribote_asmhead)
add_dependencies(27_day_harib24g_haribote_asmhead_od 27_day_harib24g_haribote_asmhead_wine)

add_dependencies(images 27_day_harib24g_haribote_asmhead)
add_dependencies(wine 27_day_harib24g_haribote_asmhead_wine)
add_dependencies(od 27_day_harib24g_haribote_asmhead_od)

#----------------------------------------------------------
########### next target ###############
set(ipl10.img_SRCS ${root_SOURCE_DIR}/projects/27_day/harib24g/haribote/ipl10.nas)
set(ipl10.img_OUTS ${root_BINARY_DIR}/projects/27_day/harib24g/haribote/ipl10.img)
set(ipl10_wine.img_OUTS ${root_BINARY_DIR}/projects/27_day/harib24g/haribote/ipl10_wine.img)

add_custom_target(27_day_harib24g_haribote_ipl10
  COMMAND ${root_BINARY_DIR}/src/opennask ${ipl10.img_SRCS} ${ipl10.img_OUTS}
)
add_custom_target(27_day_harib24g_haribote_ipl10_wine
  COMMAND ${WINE} ${WINE_NASK} ${ipl10.img_SRCS} ${ipl10_wine.img_OUTS}
)
add_custom_target(27_day_harib24g_haribote_ipl10_od
  COMMAND ${OD} -t x1 ${ipl10.img_OUTS}      > ${ipl10.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${ipl10_wine.img_OUTS} > ${ipl10_wine.img_OUTS}_t.txt
  COMMAND diff -s ${ipl10.img_OUTS}_f.txt ${ipl10_wine.img_OUTS}_t.txt
)

add_dependencies(27_day_harib24g_haribote_ipl10_od 27_day_harib24g_haribote_ipl10)
add_dependencies(27_day_harib24g_haribote_ipl10_od 27_day_harib24g_haribote_ipl10_wine)

add_dependencies(images 27_day_harib24g_haribote_ipl10)
add_dependencies(wine 27_day_harib24g_haribote_ipl10_wine)
add_dependencies(od 27_day_harib24g_haribote_ipl10_od)

#----------------------------------------------------------
########### next target ###############
set(naskfunc.img_SRCS ${root_SOURCE_DIR}/projects/27_day/harib24g/haribote/naskfunc.nas)
set(naskfunc.img_OUTS ${root_BINARY_DIR}/projects/27_day/harib24g/haribote/naskfunc.img)
set(naskfunc_wine.img_OUTS ${root_BINARY_DIR}/projects/27_day/harib24g/haribote/naskfunc_wine.img)

add_custom_target(27_day_harib24g_haribote_naskfunc
  COMMAND ${root_BINARY_DIR}/src/opennask ${naskfunc.img_SRCS} ${naskfunc.img_OUTS}
)
add_custom_target(27_day_harib24g_haribote_naskfunc_wine
  COMMAND ${WINE} ${WINE_NASK} ${naskfunc.img_SRCS} ${naskfunc_wine.img_OUTS}
)
add_custom_target(27_day_harib24g_haribote_naskfunc_od
  COMMAND ${OD} -t x1 ${naskfunc.img_OUTS}      > ${naskfunc.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${naskfunc_wine.img_OUTS} > ${naskfunc_wine.img_OUTS}_t.txt
  COMMAND diff -s ${naskfunc.img_OUTS}_f.txt ${naskfunc_wine.img_OUTS}_t.txt
)

add_dependencies(27_day_harib24g_haribote_naskfunc_od 27_day_harib24g_haribote_naskfunc)
add_dependencies(27_day_harib24g_haribote_naskfunc_od 27_day_harib24g_haribote_naskfunc_wine)

add_dependencies(images 27_day_harib24g_haribote_naskfunc)
add_dependencies(wine 27_day_harib24g_haribote_naskfunc_wine)
add_dependencies(od 27_day_harib24g_haribote_naskfunc_od)

#----------------------------------------------------------
