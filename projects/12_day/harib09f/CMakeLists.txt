#----------------------------------------------------------
message(STATUS "Entering directory projects/12_day/harib09f/")

set(NASK ${root_BINARY_DIR}/src/opennask)
set(FONT ${root_BINARY_DIR}/src/makefont)
set(B2O  ${root_BINARY_DIR}/src/bin2obj)
set(CONV ${root_BINARY_DIR}/objconv/objconv)
set(12_day_harib09f_OS    ${root_BINARY_DIR}/projects/12_day/harib09f/os.img)
set(12_day_harib09f_SYS	  ${root_BINARY_DIR}/projects/12_day/harib09f/os.sys)
set(12_day_harib09f_IPLB  ${root_BINARY_DIR}/projects/12_day/harib09f/ipl.bin)
set(12_day_harib09f_IPLS  ${root_SOURCE_DIR}/projects/12_day/harib09f/ipl10.nas)
set(12_day_harib09f_HEADB ${root_BINARY_DIR}/projects/12_day/harib09f/asmhead.bin)
set(12_day_harib09f_HEADS ${root_SOURCE_DIR}/projects/12_day/harib09f/asmhead.nas)
set(12_day_harib09f_BOOTB ${root_BINARY_DIR}/projects/12_day/harib09f/boot.bin)
set(12_day_harib09f_CCO	  ${root_BINARY_DIR}/projects/12_day/harib09f/boot.o)
set(12_day_harib09f_CCS	  ${root_SOURCE_DIR}/projects/12_day/harib09f/*.c)
set(12_day_harib09f_LDS	  ${root_SOURCE_DIR}/projects/os.lds)
set(12_day_harib09f_FUNCS ${root_SOURCE_DIR}/projects/12_day/harib09f/naskfunc.nas)
set(12_day_harib09f_FUNCO ${root_BINARY_DIR}/projects/12_day/harib09f/naskfunc.o)
set(12_day_harib09f_FONTS ${root_SOURCE_DIR}/projects/12_day/harib09f/hankaku.txt)
set(12_day_harib09f_FONTB ${root_BINARY_DIR}/projects/12_day/harib09f/hankaku.bin)
set(12_day_harib09f_FONTO ${root_BINARY_DIR}/projects/12_day/harib09f/hankaku.o)
set(12_day_harib09f_LIBGE ${root_BINARY_DIR}/golibc/libgo.a)
set(12_day_harib09f_LIBGC ${root_BINARY_DIR}/projects/12_day/harib09f/libgo.o)
set(12_day_harib09f_WILDOBJ ${root_BINARY_DIR}/projects/12_day/harib09f/*.o)


add_custom_target(12_day_harib09f_run
  COMMAND ${QEMU} ${QEMUOPT} ${12_day_harib09f_OS}
  DEPENDS 12_day_harib09f_img
)
add_custom_target(12_day_harib09f_debug
  COMMAND ${QEMU} -s -S ${QEMUOPT} ${12_day_harib09f_OS} -redir tcp:5555:127.0.0.1:1234 &
  DEPENDS 12_day_harib09f_img
)
add_custom_target(12_day_harib09f_clean
  COMMAND rm -f ${12_day_harib09f_OS}
  COMMAND rm -f ${12_day_harib09f_SYS}
  COMMAND rm -f ${12_day_harib09f_IPLB}
  COMMAND rm -f ${12_day_harib09f_HEADB}
  COMMAND rm -f ${12_day_harib09f_BOOTB}
  COMMAND rm -f ${12_day_harib09f_WILDOBJ}
)
add_custom_target(12_day_harib09f_ipl
  COMMAND ${NASK} ${12_day_harib09f_IPLS} ${12_day_harib09f_IPLB}
)
add_custom_target(12_day_harib09f_sys
  COMMAND ${NASK} ${12_day_harib09f_HEADS} ${12_day_harib09f_HEADB}
  COMMAND ${FONT} ${12_day_harib09f_FONTS} ${12_day_harib09f_FONTB}
  COMMAND ${B2O}  ${12_day_harib09f_FONTB} ${12_day_harib09f_FONTO} _hankaku
  COMMAND ${CONV} -fcoff32 -nu ${12_day_harib09f_LIBGE} ${12_day_harib09f_LIBGC}
  COMMAND ${NASK} ${12_day_harib09f_FUNCS} ${12_day_harib09f_FUNCO}
  COMMAND gcc ${BINOPT} -T ${12_day_harib09f_LDS} ${12_day_harib09f_CCS} ${12_day_harib09f_WILDOBJ} -o ${12_day_harib09f_BOOTB}
  COMMAND cat ${12_day_harib09f_HEADB} ${12_day_harib09f_BOOTB} > ${12_day_harib09f_SYS}
  DEPENDS 12_day_harib09f_ipl
)
add_custom_target(12_day_harib09f_img
  COMMAND mformat -f 1440 -l HARIBOTEOS -N 0xffffffff -C -B ${12_day_harib09f_IPLB} -i ${12_day_harib09f_OS}
  COMMAND mcopy -i ${12_day_harib09f_OS} ${12_day_harib09f_SYS} ::
  DEPENDS 12_day_harib09f_sys
)
########### next target ###############
set(asmhead.img_SRCS ${root_SOURCE_DIR}/projects/12_day/harib09f/asmhead.nas)
set(asmhead.img_OUTS ${root_BINARY_DIR}/projects/12_day/harib09f/asmhead.img)
set(asmhead_wine.img_OUTS ${root_BINARY_DIR}/projects/12_day/harib09f/asmhead_wine.img)

add_custom_target(12_day_harib09f_asmhead
  COMMAND ${root_BINARY_DIR}/src/opennask ${asmhead.img_SRCS} ${asmhead.img_OUTS}
)
add_custom_target(12_day_harib09f_asmhead_wine
  COMMAND ${WINE} ${WINE_NASK} ${asmhead.img_SRCS} ${asmhead_wine.img_OUTS}
)
add_custom_target(12_day_harib09f_asmhead_od
  COMMAND ${OD} -t x1 ${asmhead.img_OUTS}      > ${asmhead.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${asmhead_wine.img_OUTS} > ${asmhead_wine.img_OUTS}_t.txt
  COMMAND diff -s ${asmhead.img_OUTS}_f.txt ${asmhead_wine.img_OUTS}_t.txt
)

add_dependencies(12_day_harib09f_asmhead_od 12_day_harib09f_asmhead)
add_dependencies(12_day_harib09f_asmhead_od 12_day_harib09f_asmhead_wine)

add_dependencies(images 12_day_harib09f_asmhead)
add_dependencies(wine 12_day_harib09f_asmhead_wine)
add_dependencies(od 12_day_harib09f_asmhead_od)

#----------------------------------------------------------
########### next target ###############
set(ipl10.img_SRCS ${root_SOURCE_DIR}/projects/12_day/harib09f/ipl10.nas)
set(ipl10.img_OUTS ${root_BINARY_DIR}/projects/12_day/harib09f/ipl10.img)
set(ipl10_wine.img_OUTS ${root_BINARY_DIR}/projects/12_day/harib09f/ipl10_wine.img)

add_custom_target(12_day_harib09f_ipl10
  COMMAND ${root_BINARY_DIR}/src/opennask ${ipl10.img_SRCS} ${ipl10.img_OUTS}
)
add_custom_target(12_day_harib09f_ipl10_wine
  COMMAND ${WINE} ${WINE_NASK} ${ipl10.img_SRCS} ${ipl10_wine.img_OUTS}
)
add_custom_target(12_day_harib09f_ipl10_od
  COMMAND ${OD} -t x1 ${ipl10.img_OUTS}      > ${ipl10.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${ipl10_wine.img_OUTS} > ${ipl10_wine.img_OUTS}_t.txt
  COMMAND diff -s ${ipl10.img_OUTS}_f.txt ${ipl10_wine.img_OUTS}_t.txt
)

add_dependencies(12_day_harib09f_ipl10_od 12_day_harib09f_ipl10)
add_dependencies(12_day_harib09f_ipl10_od 12_day_harib09f_ipl10_wine)

add_dependencies(images 12_day_harib09f_ipl10)
add_dependencies(wine 12_day_harib09f_ipl10_wine)
add_dependencies(od 12_day_harib09f_ipl10_od)

#----------------------------------------------------------
########### next target ###############
set(naskfunc.img_SRCS ${root_SOURCE_DIR}/projects/12_day/harib09f/naskfunc.nas)
set(naskfunc.img_OUTS ${root_BINARY_DIR}/projects/12_day/harib09f/naskfunc.img)
set(naskfunc_wine.img_OUTS ${root_BINARY_DIR}/projects/12_day/harib09f/naskfunc_wine.img)

add_custom_target(12_day_harib09f_naskfunc
  COMMAND ${root_BINARY_DIR}/src/opennask ${naskfunc.img_SRCS} ${naskfunc.img_OUTS}
)
add_custom_target(12_day_harib09f_naskfunc_wine
  COMMAND ${WINE} ${WINE_NASK} ${naskfunc.img_SRCS} ${naskfunc_wine.img_OUTS}
)
add_custom_target(12_day_harib09f_naskfunc_od
  COMMAND ${OD} -t x1 ${naskfunc.img_OUTS}      > ${naskfunc.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${naskfunc_wine.img_OUTS} > ${naskfunc_wine.img_OUTS}_t.txt
  COMMAND diff -s ${naskfunc.img_OUTS}_f.txt ${naskfunc_wine.img_OUTS}_t.txt
)

add_dependencies(12_day_harib09f_naskfunc_od 12_day_harib09f_naskfunc)
add_dependencies(12_day_harib09f_naskfunc_od 12_day_harib09f_naskfunc_wine)

add_dependencies(images 12_day_harib09f_naskfunc)
add_dependencies(wine 12_day_harib09f_naskfunc_wine)
add_dependencies(od 12_day_harib09f_naskfunc_od)

#----------------------------------------------------------
