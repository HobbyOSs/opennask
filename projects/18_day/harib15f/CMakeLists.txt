#----------------------------------------------------------
message(STATUS "Entering directory projects/18_day/harib15f/")

set(NASK ${root_BINARY_DIR}/src/opennask)
set(FONT ${root_BINARY_DIR}/src/makefont)
set(B2O  ${root_BINARY_DIR}/src/bin2obj)
set(CONV ${root_BINARY_DIR}/objconv/objconv)
set(18_day_harib15f_OS    ${root_BINARY_DIR}/projects/18_day/harib15f/os.img)
set(18_day_harib15f_SYS	  ${root_BINARY_DIR}/projects/18_day/harib15f/os.sys)
set(18_day_harib15f_IPLB  ${root_BINARY_DIR}/projects/18_day/harib15f/ipl.bin)
set(18_day_harib15f_IPLS  ${root_SOURCE_DIR}/projects/18_day/harib15f/ipl10.nas)
set(18_day_harib15f_HEADB ${root_BINARY_DIR}/projects/18_day/harib15f/asmhead.bin)
set(18_day_harib15f_HEADS ${root_SOURCE_DIR}/projects/18_day/harib15f/asmhead.nas)
set(18_day_harib15f_BOOTB ${root_BINARY_DIR}/projects/18_day/harib15f/boot.bin)
set(18_day_harib15f_CCO	  ${root_BINARY_DIR}/projects/18_day/harib15f/boot.o)
set(18_day_harib15f_CCS	  ${root_SOURCE_DIR}/projects/18_day/harib15f/*.c)
set(18_day_harib15f_LDS	  ${root_SOURCE_DIR}/projects/os.lds)
set(18_day_harib15f_FUNCS ${root_SOURCE_DIR}/projects/18_day/harib15f/naskfunc.nas)
set(18_day_harib15f_FUNCO ${root_BINARY_DIR}/projects/18_day/harib15f/naskfunc.o)
set(18_day_harib15f_FONTS ${root_SOURCE_DIR}/projects/18_day/harib15f/hankaku.txt)
set(18_day_harib15f_FONTB ${root_BINARY_DIR}/projects/18_day/harib15f/hankaku.bin)
set(18_day_harib15f_FONTO ${root_BINARY_DIR}/projects/18_day/harib15f/hankaku.o)
set(18_day_harib15f_LIBGE ${root_BINARY_DIR}/golibc/libgo.a)
set(18_day_harib15f_LIBGC ${root_BINARY_DIR}/projects/18_day/harib15f/libgo.o)
set(18_day_harib15f_WILDOBJ ${root_BINARY_DIR}/projects/18_day/harib15f/*.o)


add_custom_target(18_day_harib15f_run
  COMMAND ${QEMU} ${QEMUOPT} ${18_day_harib15f_OS}
  DEPENDS 18_day_harib15f_img
)
add_custom_target(18_day_harib15f_debug
  COMMAND ${QEMU} -s -S ${QEMUOPT} ${18_day_harib15f_OS} -redir tcp:5555:127.0.0.1:1234 &
  DEPENDS 18_day_harib15f_img
)
add_custom_target(18_day_harib15f_clean
  COMMAND rm -f ${18_day_harib15f_OS}
  COMMAND rm -f ${18_day_harib15f_SYS}
  COMMAND rm -f ${18_day_harib15f_IPLB}
  COMMAND rm -f ${18_day_harib15f_HEADB}
  COMMAND rm -f ${18_day_harib15f_BOOTB}
  COMMAND rm -f ${18_day_harib15f_WILDOBJ}
)
add_custom_target(18_day_harib15f_ipl
  COMMAND ${NASK} ${18_day_harib15f_IPLS} ${18_day_harib15f_IPLB}
)
add_custom_target(18_day_harib15f_sys
  COMMAND ${NASK} ${18_day_harib15f_HEADS} ${18_day_harib15f_HEADB}
  COMMAND ${FONT} ${18_day_harib15f_FONTS} ${18_day_harib15f_FONTB}
  COMMAND ${B2O}  ${18_day_harib15f_FONTB} ${18_day_harib15f_FONTO} _hankaku
  COMMAND ${CONV} -fcoff32 -nu ${18_day_harib15f_LIBGE} ${18_day_harib15f_LIBGC}
  COMMAND ${NASK} ${18_day_harib15f_FUNCS} ${18_day_harib15f_FUNCO}
  COMMAND gcc ${BINOPT} -T ${18_day_harib15f_LDS} ${18_day_harib15f_CCS} ${18_day_harib15f_WILDOBJ} -o ${18_day_harib15f_BOOTB}
  COMMAND cat ${18_day_harib15f_HEADB} ${18_day_harib15f_BOOTB} > ${18_day_harib15f_SYS}
  DEPENDS 18_day_harib15f_ipl
)
add_custom_target(18_day_harib15f_img
  COMMAND mformat -f 1440 -l HARIBOTEOS -N 0xffffffff -C -B ${18_day_harib15f_IPLB} -i ${18_day_harib15f_OS}
  COMMAND mcopy -i ${18_day_harib15f_OS} ${18_day_harib15f_SYS} ::
  DEPENDS 18_day_harib15f_sys
)
########### next target ###############
set(asmhead.img_SRCS ${root_SOURCE_DIR}/projects/18_day/harib15f/asmhead.nas)
set(asmhead.img_OUTS ${root_BINARY_DIR}/projects/18_day/harib15f/asmhead.img)
set(asmhead_wine.img_OUTS ${root_BINARY_DIR}/projects/18_day/harib15f/asmhead_wine.img)

add_custom_target(18_day_harib15f_asmhead
  COMMAND ${root_BINARY_DIR}/src/opennask ${asmhead.img_SRCS} ${asmhead.img_OUTS}
)
add_custom_target(18_day_harib15f_asmhead_wine
  COMMAND ${WINE} ${WINE_NASK} ${asmhead.img_SRCS} ${asmhead_wine.img_OUTS}
)
add_custom_target(18_day_harib15f_asmhead_od
  COMMAND ${OD} -t x1 ${asmhead.img_OUTS}      > ${asmhead.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${asmhead_wine.img_OUTS} > ${asmhead_wine.img_OUTS}_t.txt
  COMMAND diff -s ${asmhead.img_OUTS}_f.txt ${asmhead_wine.img_OUTS}_t.txt
)

add_dependencies(18_day_harib15f_asmhead_od 18_day_harib15f_asmhead)
add_dependencies(18_day_harib15f_asmhead_od 18_day_harib15f_asmhead_wine)

add_dependencies(images 18_day_harib15f_asmhead)
add_dependencies(wine 18_day_harib15f_asmhead_wine)
add_dependencies(od 18_day_harib15f_asmhead_od)

#----------------------------------------------------------
########### next target ###############
set(ipl10.img_SRCS ${root_SOURCE_DIR}/projects/18_day/harib15f/ipl10.nas)
set(ipl10.img_OUTS ${root_BINARY_DIR}/projects/18_day/harib15f/ipl10.img)
set(ipl10_wine.img_OUTS ${root_BINARY_DIR}/projects/18_day/harib15f/ipl10_wine.img)

add_custom_target(18_day_harib15f_ipl10
  COMMAND ${root_BINARY_DIR}/src/opennask ${ipl10.img_SRCS} ${ipl10.img_OUTS}
)
add_custom_target(18_day_harib15f_ipl10_wine
  COMMAND ${WINE} ${WINE_NASK} ${ipl10.img_SRCS} ${ipl10_wine.img_OUTS}
)
add_custom_target(18_day_harib15f_ipl10_od
  COMMAND ${OD} -t x1 ${ipl10.img_OUTS}      > ${ipl10.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${ipl10_wine.img_OUTS} > ${ipl10_wine.img_OUTS}_t.txt
  COMMAND diff -s ${ipl10.img_OUTS}_f.txt ${ipl10_wine.img_OUTS}_t.txt
)

add_dependencies(18_day_harib15f_ipl10_od 18_day_harib15f_ipl10)
add_dependencies(18_day_harib15f_ipl10_od 18_day_harib15f_ipl10_wine)

add_dependencies(images 18_day_harib15f_ipl10)
add_dependencies(wine 18_day_harib15f_ipl10_wine)
add_dependencies(od 18_day_harib15f_ipl10_od)

#----------------------------------------------------------
########### next target ###############
set(naskfunc.img_SRCS ${root_SOURCE_DIR}/projects/18_day/harib15f/naskfunc.nas)
set(naskfunc.img_OUTS ${root_BINARY_DIR}/projects/18_day/harib15f/naskfunc.img)
set(naskfunc_wine.img_OUTS ${root_BINARY_DIR}/projects/18_day/harib15f/naskfunc_wine.img)

add_custom_target(18_day_harib15f_naskfunc
  COMMAND ${root_BINARY_DIR}/src/opennask ${naskfunc.img_SRCS} ${naskfunc.img_OUTS}
)
add_custom_target(18_day_harib15f_naskfunc_wine
  COMMAND ${WINE} ${WINE_NASK} ${naskfunc.img_SRCS} ${naskfunc_wine.img_OUTS}
)
add_custom_target(18_day_harib15f_naskfunc_od
  COMMAND ${OD} -t x1 ${naskfunc.img_OUTS}      > ${naskfunc.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${naskfunc_wine.img_OUTS} > ${naskfunc_wine.img_OUTS}_t.txt
  COMMAND diff -s ${naskfunc.img_OUTS}_f.txt ${naskfunc_wine.img_OUTS}_t.txt
)

add_dependencies(18_day_harib15f_naskfunc_od 18_day_harib15f_naskfunc)
add_dependencies(18_day_harib15f_naskfunc_od 18_day_harib15f_naskfunc_wine)

add_dependencies(images 18_day_harib15f_naskfunc)
add_dependencies(wine 18_day_harib15f_naskfunc_wine)
add_dependencies(od 18_day_harib15f_naskfunc_od)

#----------------------------------------------------------
