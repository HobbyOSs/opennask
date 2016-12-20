#----------------------------------------------------------
message(STATUS "Entering directory projects/18_day/harib15e/")

set(NASK ${root_BINARY_DIR}/src/opennask)
set(FONT ${root_BINARY_DIR}/src/makefont)
set(B2O  ${root_BINARY_DIR}/src/bin2obj)
set(CONV ${root_BINARY_DIR}/objconv/objconv)
set(18_day_harib15e_OS    ${root_BINARY_DIR}/projects/18_day/harib15e/os.img)
set(18_day_harib15e_SYS	  ${root_BINARY_DIR}/projects/18_day/harib15e/os.sys)
set(18_day_harib15e_IPLB  ${root_BINARY_DIR}/projects/18_day/harib15e/ipl.bin)
set(18_day_harib15e_IPLS  ${root_SOURCE_DIR}/projects/18_day/harib15e/ipl10.nas)
set(18_day_harib15e_HEADB ${root_BINARY_DIR}/projects/18_day/harib15e/asmhead.bin)
set(18_day_harib15e_HEADS ${root_SOURCE_DIR}/projects/18_day/harib15e/asmhead.nas)
set(18_day_harib15e_BOOTB ${root_BINARY_DIR}/projects/18_day/harib15e/boot.bin)
set(18_day_harib15e_CCO	  ${root_BINARY_DIR}/projects/18_day/harib15e/boot.o)
set(18_day_harib15e_CCS	  ${root_SOURCE_DIR}/projects/18_day/harib15e/*.c)
set(18_day_harib15e_LDS	  ${root_SOURCE_DIR}/projects/os.lds)
set(18_day_harib15e_FUNCS ${root_SOURCE_DIR}/projects/18_day/harib15e/naskfunc.nas)
set(18_day_harib15e_FUNCO ${root_BINARY_DIR}/projects/18_day/harib15e/naskfunc.o)
set(18_day_harib15e_FONTS ${root_SOURCE_DIR}/projects/18_day/harib15e/hankaku.txt)
set(18_day_harib15e_FONTB ${root_BINARY_DIR}/projects/18_day/harib15e/hankaku.bin)
set(18_day_harib15e_FONTO ${root_BINARY_DIR}/projects/18_day/harib15e/hankaku.o)
set(18_day_harib15e_LIBGE ${root_BINARY_DIR}/golibc/libgo.a)
set(18_day_harib15e_LIBGC ${root_BINARY_DIR}/projects/18_day/harib15e/libgo.o)
set(18_day_harib15e_WILDOBJ ${root_BINARY_DIR}/projects/18_day/harib15e/*.o)


add_custom_target(18_day_harib15e_run
  COMMAND ${QEMU} ${QEMUOPT} ${18_day_harib15e_OS}
  DEPENDS 18_day_harib15e_img
)
add_custom_target(18_day_harib15e_debug
  COMMAND ${QEMU} -s -S ${QEMUOPT} ${18_day_harib15e_OS} -redir tcp:5555:127.0.0.1:1234 &
  DEPENDS 18_day_harib15e_img
)
add_custom_target(18_day_harib15e_clean
  COMMAND rm -f ${18_day_harib15e_OS}
  COMMAND rm -f ${18_day_harib15e_SYS}
  COMMAND rm -f ${18_day_harib15e_IPLB}
  COMMAND rm -f ${18_day_harib15e_HEADB}
  COMMAND rm -f ${18_day_harib15e_BOOTB}
  COMMAND rm -f ${18_day_harib15e_WILDOBJ}
)
add_custom_target(18_day_harib15e_ipl
  COMMAND ${NASK} ${18_day_harib15e_IPLS} ${18_day_harib15e_IPLB}
)
add_custom_target(18_day_harib15e_sys
  COMMAND ${NASK} ${18_day_harib15e_HEADS} ${18_day_harib15e_HEADB}
  COMMAND ${FONT} ${18_day_harib15e_FONTS} ${18_day_harib15e_FONTB}
  COMMAND ${B2O}  ${18_day_harib15e_FONTB} ${18_day_harib15e_FONTO} _hankaku
  COMMAND ${CONV} -fcoff32 -nu ${18_day_harib15e_LIBGE} ${18_day_harib15e_LIBGC}
  COMMAND ${NASK} ${18_day_harib15e_FUNCS} ${18_day_harib15e_FUNCO}
  COMMAND gcc ${BINOPT} -T ${18_day_harib15e_LDS} ${18_day_harib15e_CCS} ${18_day_harib15e_WILDOBJ} -o ${18_day_harib15e_BOOTB}
  COMMAND cat ${18_day_harib15e_HEADB} ${18_day_harib15e_BOOTB} > ${18_day_harib15e_SYS}
  DEPENDS 18_day_harib15e_ipl
)
add_custom_target(18_day_harib15e_img
  COMMAND mformat -f 1440 -l HARIBOTEOS -N 0xffffffff -C -B ${18_day_harib15e_IPLB} -i ${18_day_harib15e_OS}
  COMMAND mcopy -i ${18_day_harib15e_OS} ${18_day_harib15e_SYS} ::
  DEPENDS 18_day_harib15e_sys
)
########### next target ###############
set(asmhead.img_SRCS ${root_SOURCE_DIR}/projects/18_day/harib15e/asmhead.nas)
set(asmhead.img_OUTS ${root_BINARY_DIR}/projects/18_day/harib15e/asmhead.img)
set(asmhead_wine.img_OUTS ${root_BINARY_DIR}/projects/18_day/harib15e/asmhead_wine.img)

add_custom_target(18_day_harib15e_asmhead
  COMMAND ${root_BINARY_DIR}/src/opennask ${asmhead.img_SRCS} ${asmhead.img_OUTS}
)
add_custom_target(18_day_harib15e_asmhead_wine
  COMMAND ${WINE} ${WINE_NASK} ${asmhead.img_SRCS} ${asmhead_wine.img_OUTS}
)
add_custom_target(18_day_harib15e_asmhead_od
  COMMAND ${OD} -t x1 ${asmhead.img_OUTS}      > ${asmhead.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${asmhead_wine.img_OUTS} > ${asmhead_wine.img_OUTS}_t.txt
  COMMAND diff -s ${asmhead.img_OUTS}_f.txt ${asmhead_wine.img_OUTS}_t.txt
)

add_dependencies(18_day_harib15e_asmhead_od 18_day_harib15e_asmhead)
add_dependencies(18_day_harib15e_asmhead_od 18_day_harib15e_asmhead_wine)

add_dependencies(images 18_day_harib15e_asmhead)
add_dependencies(wine 18_day_harib15e_asmhead_wine)
add_dependencies(od 18_day_harib15e_asmhead_od)

#----------------------------------------------------------
########### next target ###############
set(ipl10.img_SRCS ${root_SOURCE_DIR}/projects/18_day/harib15e/ipl10.nas)
set(ipl10.img_OUTS ${root_BINARY_DIR}/projects/18_day/harib15e/ipl10.img)
set(ipl10_wine.img_OUTS ${root_BINARY_DIR}/projects/18_day/harib15e/ipl10_wine.img)

add_custom_target(18_day_harib15e_ipl10
  COMMAND ${root_BINARY_DIR}/src/opennask ${ipl10.img_SRCS} ${ipl10.img_OUTS}
)
add_custom_target(18_day_harib15e_ipl10_wine
  COMMAND ${WINE} ${WINE_NASK} ${ipl10.img_SRCS} ${ipl10_wine.img_OUTS}
)
add_custom_target(18_day_harib15e_ipl10_od
  COMMAND ${OD} -t x1 ${ipl10.img_OUTS}      > ${ipl10.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${ipl10_wine.img_OUTS} > ${ipl10_wine.img_OUTS}_t.txt
  COMMAND diff -s ${ipl10.img_OUTS}_f.txt ${ipl10_wine.img_OUTS}_t.txt
)

add_dependencies(18_day_harib15e_ipl10_od 18_day_harib15e_ipl10)
add_dependencies(18_day_harib15e_ipl10_od 18_day_harib15e_ipl10_wine)

add_dependencies(images 18_day_harib15e_ipl10)
add_dependencies(wine 18_day_harib15e_ipl10_wine)
add_dependencies(od 18_day_harib15e_ipl10_od)

#----------------------------------------------------------
########### next target ###############
set(naskfunc.img_SRCS ${root_SOURCE_DIR}/projects/18_day/harib15e/naskfunc.nas)
set(naskfunc.img_OUTS ${root_BINARY_DIR}/projects/18_day/harib15e/naskfunc.img)
set(naskfunc_wine.img_OUTS ${root_BINARY_DIR}/projects/18_day/harib15e/naskfunc_wine.img)

add_custom_target(18_day_harib15e_naskfunc
  COMMAND ${root_BINARY_DIR}/src/opennask ${naskfunc.img_SRCS} ${naskfunc.img_OUTS}
)
add_custom_target(18_day_harib15e_naskfunc_wine
  COMMAND ${WINE} ${WINE_NASK} ${naskfunc.img_SRCS} ${naskfunc_wine.img_OUTS}
)
add_custom_target(18_day_harib15e_naskfunc_od
  COMMAND ${OD} -t x1 ${naskfunc.img_OUTS}      > ${naskfunc.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${naskfunc_wine.img_OUTS} > ${naskfunc_wine.img_OUTS}_t.txt
  COMMAND diff -s ${naskfunc.img_OUTS}_f.txt ${naskfunc_wine.img_OUTS}_t.txt
)

add_dependencies(18_day_harib15e_naskfunc_od 18_day_harib15e_naskfunc)
add_dependencies(18_day_harib15e_naskfunc_od 18_day_harib15e_naskfunc_wine)

add_dependencies(images 18_day_harib15e_naskfunc)
add_dependencies(wine 18_day_harib15e_naskfunc_wine)
add_dependencies(od 18_day_harib15e_naskfunc_od)

#----------------------------------------------------------
