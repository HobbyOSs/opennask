#----------------------------------------------------------
message(STATUS "Entering directory projects/12_day/harib09b/")

set(NASK ${root_BINARY_DIR}/src/opennask)
set(FONT ${root_BINARY_DIR}/src/makefont)
set(B2O  ${root_BINARY_DIR}/src/bin2obj)
set(CONV ${root_BINARY_DIR}/objconv/objconv)
set(12_day_harib09b_OS    ${root_BINARY_DIR}/projects/12_day/harib09b/os.img)
set(12_day_harib09b_SYS	  ${root_BINARY_DIR}/projects/12_day/harib09b/os.sys)
set(12_day_harib09b_IPLB  ${root_BINARY_DIR}/projects/12_day/harib09b/ipl.bin)
set(12_day_harib09b_IPLS  ${root_SOURCE_DIR}/projects/12_day/harib09b/ipl10.nas)
set(12_day_harib09b_HEADB ${root_BINARY_DIR}/projects/12_day/harib09b/asmhead.bin)
set(12_day_harib09b_HEADS ${root_SOURCE_DIR}/projects/12_day/harib09b/asmhead.nas)
set(12_day_harib09b_BOOTB ${root_BINARY_DIR}/projects/12_day/harib09b/boot.bin)
set(12_day_harib09b_CCO	  ${root_BINARY_DIR}/projects/12_day/harib09b/boot.o)
set(12_day_harib09b_CCS	  ${root_SOURCE_DIR}/projects/12_day/harib09b/*.c)
set(12_day_harib09b_LDS	  ${root_SOURCE_DIR}/projects/os.lds)
set(12_day_harib09b_FUNCS ${root_SOURCE_DIR}/projects/12_day/harib09b/naskfunc.nas)
set(12_day_harib09b_FUNCO ${root_BINARY_DIR}/projects/12_day/harib09b/naskfunc.o)
set(12_day_harib09b_FONTS ${root_SOURCE_DIR}/projects/12_day/harib09b/hankaku.txt)
set(12_day_harib09b_FONTB ${root_BINARY_DIR}/projects/12_day/harib09b/hankaku.bin)
set(12_day_harib09b_FONTO ${root_BINARY_DIR}/projects/12_day/harib09b/hankaku.o)
set(12_day_harib09b_LIBGE ${root_BINARY_DIR}/golibc/libgo.a)
set(12_day_harib09b_LIBGC ${root_BINARY_DIR}/projects/12_day/harib09b/libgo.o)
set(12_day_harib09b_WILDOBJ ${root_BINARY_DIR}/projects/12_day/harib09b/*.o)


add_custom_target(12_day_harib09b_run
  COMMAND ${QEMU} ${QEMUOPT} ${12_day_harib09b_OS}
  DEPENDS 12_day_harib09b_img
)
add_custom_target(12_day_harib09b_debug
  COMMAND ${QEMU} -s -S ${QEMUOPT} ${12_day_harib09b_OS} -redir tcp:5555:127.0.0.1:1234 &
  DEPENDS 12_day_harib09b_img
)
add_custom_target(12_day_harib09b_clean
  COMMAND rm -f ${12_day_harib09b_OS}
  COMMAND rm -f ${12_day_harib09b_SYS}
  COMMAND rm -f ${12_day_harib09b_IPLB}
  COMMAND rm -f ${12_day_harib09b_HEADB}
  COMMAND rm -f ${12_day_harib09b_BOOTB}
  COMMAND rm -f ${12_day_harib09b_WILDOBJ}
)
add_custom_target(12_day_harib09b_ipl
  COMMAND ${NASK} ${12_day_harib09b_IPLS} ${12_day_harib09b_IPLB}
)
add_custom_target(12_day_harib09b_sys
  COMMAND ${NASK} ${12_day_harib09b_HEADS} ${12_day_harib09b_HEADB}
  COMMAND ${FONT} ${12_day_harib09b_FONTS} ${12_day_harib09b_FONTB}
  COMMAND ${B2O}  ${12_day_harib09b_FONTB} ${12_day_harib09b_FONTO} _hankaku
  COMMAND ${CONV} -fcoff32 -nu ${12_day_harib09b_LIBGE} ${12_day_harib09b_LIBGC}
  COMMAND ${NASK} ${12_day_harib09b_FUNCS} ${12_day_harib09b_FUNCO}
  COMMAND gcc ${BINOPT} -T ${12_day_harib09b_LDS} ${12_day_harib09b_CCS} ${12_day_harib09b_WILDOBJ} -o ${12_day_harib09b_BOOTB}
  COMMAND cat ${12_day_harib09b_HEADB} ${12_day_harib09b_BOOTB} > ${12_day_harib09b_SYS}
  DEPENDS 12_day_harib09b_ipl
)
add_custom_target(12_day_harib09b_img
  COMMAND mformat -f 1440 -l HARIBOTEOS -N 0xffffffff -C -B ${12_day_harib09b_IPLB} -i ${12_day_harib09b_OS}
  COMMAND mcopy -i ${12_day_harib09b_OS} ${12_day_harib09b_SYS} ::
  DEPENDS 12_day_harib09b_sys
)
########### next target ###############
set(asmhead.img_SRCS ${root_SOURCE_DIR}/projects/12_day/harib09b/asmhead.nas)
set(asmhead.img_OUTS ${root_BINARY_DIR}/projects/12_day/harib09b/asmhead.img)
set(asmhead_wine.img_OUTS ${root_BINARY_DIR}/projects/12_day/harib09b/asmhead_wine.img)

add_custom_target(12_day_harib09b_asmhead
  COMMAND ${root_BINARY_DIR}/src/opennask ${asmhead.img_SRCS} ${asmhead.img_OUTS}
)
add_custom_target(12_day_harib09b_asmhead_wine
  COMMAND ${WINE} ${WINE_NASK} ${asmhead.img_SRCS} ${asmhead_wine.img_OUTS}
)
add_custom_target(12_day_harib09b_asmhead_od
  COMMAND ${OD} -t x1 ${asmhead.img_OUTS}      > ${asmhead.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${asmhead_wine.img_OUTS} > ${asmhead_wine.img_OUTS}_t.txt
  COMMAND diff -s ${asmhead.img_OUTS}_f.txt ${asmhead_wine.img_OUTS}_t.txt
)

add_dependencies(12_day_harib09b_asmhead_od 12_day_harib09b_asmhead)
add_dependencies(12_day_harib09b_asmhead_od 12_day_harib09b_asmhead_wine)

add_dependencies(images 12_day_harib09b_asmhead)
add_dependencies(wine 12_day_harib09b_asmhead_wine)
add_dependencies(od 12_day_harib09b_asmhead_od)

#----------------------------------------------------------
########### next target ###############
set(ipl10.img_SRCS ${root_SOURCE_DIR}/projects/12_day/harib09b/ipl10.nas)
set(ipl10.img_OUTS ${root_BINARY_DIR}/projects/12_day/harib09b/ipl10.img)
set(ipl10_wine.img_OUTS ${root_BINARY_DIR}/projects/12_day/harib09b/ipl10_wine.img)

add_custom_target(12_day_harib09b_ipl10
  COMMAND ${root_BINARY_DIR}/src/opennask ${ipl10.img_SRCS} ${ipl10.img_OUTS}
)
add_custom_target(12_day_harib09b_ipl10_wine
  COMMAND ${WINE} ${WINE_NASK} ${ipl10.img_SRCS} ${ipl10_wine.img_OUTS}
)
add_custom_target(12_day_harib09b_ipl10_od
  COMMAND ${OD} -t x1 ${ipl10.img_OUTS}      > ${ipl10.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${ipl10_wine.img_OUTS} > ${ipl10_wine.img_OUTS}_t.txt
  COMMAND diff -s ${ipl10.img_OUTS}_f.txt ${ipl10_wine.img_OUTS}_t.txt
)

add_dependencies(12_day_harib09b_ipl10_od 12_day_harib09b_ipl10)
add_dependencies(12_day_harib09b_ipl10_od 12_day_harib09b_ipl10_wine)

add_dependencies(images 12_day_harib09b_ipl10)
add_dependencies(wine 12_day_harib09b_ipl10_wine)
add_dependencies(od 12_day_harib09b_ipl10_od)

#----------------------------------------------------------
########### next target ###############
set(naskfunc.img_SRCS ${root_SOURCE_DIR}/projects/12_day/harib09b/naskfunc.nas)
set(naskfunc.img_OUTS ${root_BINARY_DIR}/projects/12_day/harib09b/naskfunc.img)
set(naskfunc_wine.img_OUTS ${root_BINARY_DIR}/projects/12_day/harib09b/naskfunc_wine.img)

add_custom_target(12_day_harib09b_naskfunc
  COMMAND ${root_BINARY_DIR}/src/opennask ${naskfunc.img_SRCS} ${naskfunc.img_OUTS}
)
add_custom_target(12_day_harib09b_naskfunc_wine
  COMMAND ${WINE} ${WINE_NASK} ${naskfunc.img_SRCS} ${naskfunc_wine.img_OUTS}
)
add_custom_target(12_day_harib09b_naskfunc_od
  COMMAND ${OD} -t x1 ${naskfunc.img_OUTS}      > ${naskfunc.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${naskfunc_wine.img_OUTS} > ${naskfunc_wine.img_OUTS}_t.txt
  COMMAND diff -s ${naskfunc.img_OUTS}_f.txt ${naskfunc_wine.img_OUTS}_t.txt
)

add_dependencies(12_day_harib09b_naskfunc_od 12_day_harib09b_naskfunc)
add_dependencies(12_day_harib09b_naskfunc_od 12_day_harib09b_naskfunc_wine)

add_dependencies(images 12_day_harib09b_naskfunc)
add_dependencies(wine 12_day_harib09b_naskfunc_wine)
add_dependencies(od 12_day_harib09b_naskfunc_od)

#----------------------------------------------------------
