#----------------------------------------------------------
message(STATUS "Entering directory projects/12_day/harib09a/")

set(NASK ${root_BINARY_DIR}/src/opennask)
set(FONT ${root_BINARY_DIR}/src/makefont)
set(B2O  ${root_BINARY_DIR}/src/bin2obj)
set(CONV ${root_BINARY_DIR}/objconv/objconv)
set(12_day_harib09a_OS    ${root_BINARY_DIR}/projects/12_day/harib09a/os.img)
set(12_day_harib09a_SYS	  ${root_BINARY_DIR}/projects/12_day/harib09a/os.sys)
set(12_day_harib09a_IPLB  ${root_BINARY_DIR}/projects/12_day/harib09a/ipl.bin)
set(12_day_harib09a_IPLS  ${root_SOURCE_DIR}/projects/12_day/harib09a/ipl10.nas)
set(12_day_harib09a_HEADB ${root_BINARY_DIR}/projects/12_day/harib09a/asmhead.bin)
set(12_day_harib09a_HEADS ${root_SOURCE_DIR}/projects/12_day/harib09a/asmhead.nas)
set(12_day_harib09a_BOOTB ${root_BINARY_DIR}/projects/12_day/harib09a/boot.bin)
set(12_day_harib09a_CCO	  ${root_BINARY_DIR}/projects/12_day/harib09a/boot.o)
set(12_day_harib09a_CCS	  ${root_SOURCE_DIR}/projects/12_day/harib09a/*.c)
set(12_day_harib09a_LDS	  ${root_SOURCE_DIR}/projects/os.lds)
set(12_day_harib09a_FUNCS ${root_SOURCE_DIR}/projects/12_day/harib09a/naskfunc.nas)
set(12_day_harib09a_FUNCO ${root_BINARY_DIR}/projects/12_day/harib09a/naskfunc.o)
set(12_day_harib09a_FONTS ${root_SOURCE_DIR}/projects/12_day/harib09a/hankaku.txt)
set(12_day_harib09a_FONTB ${root_BINARY_DIR}/projects/12_day/harib09a/hankaku.bin)
set(12_day_harib09a_FONTO ${root_BINARY_DIR}/projects/12_day/harib09a/hankaku.o)
set(12_day_harib09a_LIBGE ${root_BINARY_DIR}/golibc/libgo.a)
set(12_day_harib09a_LIBGC ${root_BINARY_DIR}/projects/12_day/harib09a/libgo.o)
set(12_day_harib09a_WILDOBJ ${root_BINARY_DIR}/projects/12_day/harib09a/*.o)


add_custom_target(12_day_harib09a_run
  COMMAND ${QEMU} ${QEMUOPT} ${12_day_harib09a_OS}
  DEPENDS 12_day_harib09a_img
)
add_custom_target(12_day_harib09a_debug
  COMMAND ${QEMU} -s -S ${QEMUOPT} ${12_day_harib09a_OS} ${QEMU_DEBUG_OPT}
  DEPENDS 12_day_harib09a_img
)
add_custom_target(12_day_harib09a_clean
  COMMAND rm -f ${12_day_harib09a_OS}
  COMMAND rm -f ${12_day_harib09a_SYS}
  COMMAND rm -f ${12_day_harib09a_IPLB}
  COMMAND rm -f ${12_day_harib09a_HEADB}
  COMMAND rm -f ${12_day_harib09a_BOOTB}
  COMMAND rm -f ${12_day_harib09a_WILDOBJ}
)
add_custom_target(12_day_harib09a_ipl
  COMMAND ${NASK} ${12_day_harib09a_IPLS} ${12_day_harib09a_IPLB}
)
add_custom_target(12_day_harib09a_sys
  COMMAND ${NASK} ${12_day_harib09a_HEADS} ${12_day_harib09a_HEADB}
  COMMAND ${FONT} ${12_day_harib09a_FONTS} ${12_day_harib09a_FONTB}
  COMMAND ${B2O}  ${12_day_harib09a_FONTB} ${12_day_harib09a_FONTO} _hankaku
  COMMAND ${CONV} -fcoff32 -nu ${12_day_harib09a_LIBGE} ${12_day_harib09a_LIBGC}
  COMMAND ${NASK} ${12_day_harib09a_FUNCS} ${12_day_harib09a_FUNCO}
  COMMAND gcc ${BINOPT} -T ${12_day_harib09a_LDS} ${12_day_harib09a_CCS} ${12_day_harib09a_WILDOBJ} -o ${12_day_harib09a_BOOTB}
  COMMAND cat ${12_day_harib09a_HEADB} ${12_day_harib09a_BOOTB} > ${12_day_harib09a_SYS}
  DEPENDS 12_day_harib09a_ipl
)
add_custom_target(12_day_harib09a_img
  COMMAND mformat -f 1440 -l HARIBOTEOS -N 0xffffffff -C -B ${12_day_harib09a_IPLB} -i ${12_day_harib09a_OS}
  COMMAND mcopy -i ${12_day_harib09a_OS} ${12_day_harib09a_SYS} ::
  DEPENDS 12_day_harib09a_sys
)
########### next target ###############
set(asmhead.img_SRCS ${root_SOURCE_DIR}/projects/12_day/harib09a/asmhead.nas)
set(asmhead.img_OUTS ${root_BINARY_DIR}/projects/12_day/harib09a/asmhead.img)
set(asmhead_wine.img_OUTS ${root_BINARY_DIR}/projects/12_day/harib09a/asmhead_wine.img)

add_custom_target(12_day_harib09a_asmhead
  COMMAND ${root_BINARY_DIR}/src/opennask ${asmhead.img_SRCS} ${asmhead.img_OUTS}
)
add_custom_target(12_day_harib09a_asmhead_wine
  COMMAND ${WINE} ${WINE_NASK} ${asmhead.img_SRCS} ${asmhead_wine.img_OUTS}
)
add_custom_target(12_day_harib09a_asmhead_od
  COMMAND ${OD} -t x1 ${asmhead.img_OUTS}      > ${asmhead.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${asmhead_wine.img_OUTS} > ${asmhead_wine.img_OUTS}_t.txt
  COMMAND diff -s ${asmhead.img_OUTS}_f.txt ${asmhead_wine.img_OUTS}_t.txt
)

add_dependencies(12_day_harib09a_asmhead_od 12_day_harib09a_asmhead)
add_dependencies(12_day_harib09a_asmhead_od 12_day_harib09a_asmhead_wine)

add_dependencies(images 12_day_harib09a_asmhead)
add_dependencies(wine 12_day_harib09a_asmhead_wine)
add_dependencies(od 12_day_harib09a_asmhead_od)

#----------------------------------------------------------
########### next target ###############
set(ipl10.img_SRCS ${root_SOURCE_DIR}/projects/12_day/harib09a/ipl10.nas)
set(ipl10.img_OUTS ${root_BINARY_DIR}/projects/12_day/harib09a/ipl10.img)
set(ipl10_wine.img_OUTS ${root_BINARY_DIR}/projects/12_day/harib09a/ipl10_wine.img)

add_custom_target(12_day_harib09a_ipl10
  COMMAND ${root_BINARY_DIR}/src/opennask ${ipl10.img_SRCS} ${ipl10.img_OUTS}
)
add_custom_target(12_day_harib09a_ipl10_wine
  COMMAND ${WINE} ${WINE_NASK} ${ipl10.img_SRCS} ${ipl10_wine.img_OUTS}
)
add_custom_target(12_day_harib09a_ipl10_od
  COMMAND ${OD} -t x1 ${ipl10.img_OUTS}      > ${ipl10.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${ipl10_wine.img_OUTS} > ${ipl10_wine.img_OUTS}_t.txt
  COMMAND diff -s ${ipl10.img_OUTS}_f.txt ${ipl10_wine.img_OUTS}_t.txt
)

add_dependencies(12_day_harib09a_ipl10_od 12_day_harib09a_ipl10)
add_dependencies(12_day_harib09a_ipl10_od 12_day_harib09a_ipl10_wine)

add_dependencies(images 12_day_harib09a_ipl10)
add_dependencies(wine 12_day_harib09a_ipl10_wine)
add_dependencies(od 12_day_harib09a_ipl10_od)

#----------------------------------------------------------
########### next target ###############
set(naskfunc.img_SRCS ${root_SOURCE_DIR}/projects/12_day/harib09a/naskfunc.nas)
set(naskfunc.img_OUTS ${root_BINARY_DIR}/projects/12_day/harib09a/naskfunc.img)
set(naskfunc_wine.img_OUTS ${root_BINARY_DIR}/projects/12_day/harib09a/naskfunc_wine.img)

add_custom_target(12_day_harib09a_naskfunc
  COMMAND ${root_BINARY_DIR}/src/opennask ${naskfunc.img_SRCS} ${naskfunc.img_OUTS}
)
add_custom_target(12_day_harib09a_naskfunc_wine
  COMMAND ${WINE} ${WINE_NASK} ${naskfunc.img_SRCS} ${naskfunc_wine.img_OUTS}
)
add_custom_target(12_day_harib09a_naskfunc_od
  COMMAND ${OD} -t x1 ${naskfunc.img_OUTS}      > ${naskfunc.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${naskfunc_wine.img_OUTS} > ${naskfunc_wine.img_OUTS}_t.txt
  COMMAND diff -s ${naskfunc.img_OUTS}_f.txt ${naskfunc_wine.img_OUTS}_t.txt
)

add_dependencies(12_day_harib09a_naskfunc_od 12_day_harib09a_naskfunc)
add_dependencies(12_day_harib09a_naskfunc_od 12_day_harib09a_naskfunc_wine)

add_dependencies(images 12_day_harib09a_naskfunc)
add_dependencies(wine 12_day_harib09a_naskfunc_wine)
add_dependencies(od 12_day_harib09a_naskfunc_od)

#----------------------------------------------------------
