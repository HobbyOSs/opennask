#----------------------------------------------------------
message(STATUS "Entering directory projects/19_day/harib16d/")

set(NASK ${root_BINARY_DIR}/src/opennask)
set(FONT ${root_BINARY_DIR}/src/makefont)
set(B2O  ${root_BINARY_DIR}/src/bin2obj)
set(CONV ${root_BINARY_DIR}/objconv/objconv)
set(19_day_harib16d_OS    ${root_BINARY_DIR}/projects/19_day/harib16d/os.img)
set(19_day_harib16d_SYS	  ${root_BINARY_DIR}/projects/19_day/harib16d/os.sys)
set(19_day_harib16d_IPLB  ${root_BINARY_DIR}/projects/19_day/harib16d/ipl.bin)
set(19_day_harib16d_IPLS  ${root_SOURCE_DIR}/projects/19_day/harib16d/ipl10.nas)
set(19_day_harib16d_HEADB ${root_BINARY_DIR}/projects/19_day/harib16d/asmhead.bin)
set(19_day_harib16d_HEADS ${root_SOURCE_DIR}/projects/19_day/harib16d/asmhead.nas)
set(19_day_harib16d_BOOTB ${root_BINARY_DIR}/projects/19_day/harib16d/boot.bin)
set(19_day_harib16d_CCO	  ${root_BINARY_DIR}/projects/19_day/harib16d/boot.o)
set(19_day_harib16d_CCS	  ${root_SOURCE_DIR}/projects/19_day/harib16d/*.c)
set(19_day_harib16d_LDS	  ${root_SOURCE_DIR}/projects/os.lds)
set(19_day_harib16d_FUNCS ${root_SOURCE_DIR}/projects/19_day/harib16d/naskfunc.nas)
set(19_day_harib16d_FUNCO ${root_BINARY_DIR}/projects/19_day/harib16d/naskfunc.o)
set(19_day_harib16d_FONTS ${root_SOURCE_DIR}/projects/19_day/harib16d/hankaku.txt)
set(19_day_harib16d_FONTB ${root_BINARY_DIR}/projects/19_day/harib16d/hankaku.bin)
set(19_day_harib16d_FONTO ${root_BINARY_DIR}/projects/19_day/harib16d/hankaku.o)
set(19_day_harib16d_LIBGE ${root_BINARY_DIR}/golibc/libgo.a)
set(19_day_harib16d_LIBGC ${root_BINARY_DIR}/projects/19_day/harib16d/libgo.o)
set(19_day_harib16d_WILDOBJ ${root_BINARY_DIR}/projects/19_day/harib16d/*.o)


add_custom_target(19_day_harib16d_run
  COMMAND ${QEMU} ${QEMUOPT} ${19_day_harib16d_OS}
  DEPENDS 19_day_harib16d_img
)
add_custom_target(19_day_harib16d_debug
  COMMAND ${QEMU} -s -S ${QEMUOPT} ${19_day_harib16d_OS} -redir tcp:5555:127.0.0.1:1234 &
  DEPENDS 19_day_harib16d_img
)
add_custom_target(19_day_harib16d_clean
  COMMAND rm -f ${19_day_harib16d_OS}
  COMMAND rm -f ${19_day_harib16d_SYS}
  COMMAND rm -f ${19_day_harib16d_IPLB}
  COMMAND rm -f ${19_day_harib16d_HEADB}
  COMMAND rm -f ${19_day_harib16d_BOOTB}
  COMMAND rm -f ${19_day_harib16d_WILDOBJ}
)
add_custom_target(19_day_harib16d_ipl
  COMMAND ${NASK} ${19_day_harib16d_IPLS} ${19_day_harib16d_IPLB}
)
add_custom_target(19_day_harib16d_sys
  COMMAND ${NASK} ${19_day_harib16d_HEADS} ${19_day_harib16d_HEADB}
  COMMAND ${FONT} ${19_day_harib16d_FONTS} ${19_day_harib16d_FONTB}
  COMMAND ${B2O}  ${19_day_harib16d_FONTB} ${19_day_harib16d_FONTO} _hankaku
  COMMAND ${CONV} -fcoff32 -nu ${19_day_harib16d_LIBGE} ${19_day_harib16d_LIBGC}
  COMMAND ${NASK} ${19_day_harib16d_FUNCS} ${19_day_harib16d_FUNCO}
  COMMAND gcc ${BINOPT} -T ${19_day_harib16d_LDS} ${19_day_harib16d_CCS} ${19_day_harib16d_WILDOBJ} -o ${19_day_harib16d_BOOTB}
  COMMAND cat ${19_day_harib16d_HEADB} ${19_day_harib16d_BOOTB} > ${19_day_harib16d_SYS}
  DEPENDS 19_day_harib16d_ipl
)
add_custom_target(19_day_harib16d_img
  COMMAND mformat -f 1440 -l HARIBOTEOS -N 0xffffffff -C -B ${19_day_harib16d_IPLB} -i ${19_day_harib16d_OS}
  COMMAND mcopy -i ${19_day_harib16d_OS} ${19_day_harib16d_SYS} ::
  DEPENDS 19_day_harib16d_sys
)
########### next target ###############
set(asmhead.img_SRCS ${root_SOURCE_DIR}/projects/19_day/harib16d/asmhead.nas)
set(asmhead.img_OUTS ${root_BINARY_DIR}/projects/19_day/harib16d/asmhead.img)
set(asmhead_wine.img_OUTS ${root_BINARY_DIR}/projects/19_day/harib16d/asmhead_wine.img)

add_custom_target(19_day_harib16d_asmhead
  COMMAND ${root_BINARY_DIR}/src/opennask ${asmhead.img_SRCS} ${asmhead.img_OUTS}
)
add_custom_target(19_day_harib16d_asmhead_wine
  COMMAND ${WINE} ${WINE_NASK} ${asmhead.img_SRCS} ${asmhead_wine.img_OUTS}
)
add_custom_target(19_day_harib16d_asmhead_od
  COMMAND ${OD} -t x1 ${asmhead.img_OUTS}      > ${asmhead.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${asmhead_wine.img_OUTS} > ${asmhead_wine.img_OUTS}_t.txt
  COMMAND diff -s ${asmhead.img_OUTS}_f.txt ${asmhead_wine.img_OUTS}_t.txt
)

add_dependencies(19_day_harib16d_asmhead_od 19_day_harib16d_asmhead)
add_dependencies(19_day_harib16d_asmhead_od 19_day_harib16d_asmhead_wine)

add_dependencies(images 19_day_harib16d_asmhead)
add_dependencies(wine 19_day_harib16d_asmhead_wine)
add_dependencies(od 19_day_harib16d_asmhead_od)

#----------------------------------------------------------
########### next target ###############
set(ipl10.img_SRCS ${root_SOURCE_DIR}/projects/19_day/harib16d/ipl10.nas)
set(ipl10.img_OUTS ${root_BINARY_DIR}/projects/19_day/harib16d/ipl10.img)
set(ipl10_wine.img_OUTS ${root_BINARY_DIR}/projects/19_day/harib16d/ipl10_wine.img)

add_custom_target(19_day_harib16d_ipl10
  COMMAND ${root_BINARY_DIR}/src/opennask ${ipl10.img_SRCS} ${ipl10.img_OUTS}
)
add_custom_target(19_day_harib16d_ipl10_wine
  COMMAND ${WINE} ${WINE_NASK} ${ipl10.img_SRCS} ${ipl10_wine.img_OUTS}
)
add_custom_target(19_day_harib16d_ipl10_od
  COMMAND ${OD} -t x1 ${ipl10.img_OUTS}      > ${ipl10.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${ipl10_wine.img_OUTS} > ${ipl10_wine.img_OUTS}_t.txt
  COMMAND diff -s ${ipl10.img_OUTS}_f.txt ${ipl10_wine.img_OUTS}_t.txt
)

add_dependencies(19_day_harib16d_ipl10_od 19_day_harib16d_ipl10)
add_dependencies(19_day_harib16d_ipl10_od 19_day_harib16d_ipl10_wine)

add_dependencies(images 19_day_harib16d_ipl10)
add_dependencies(wine 19_day_harib16d_ipl10_wine)
add_dependencies(od 19_day_harib16d_ipl10_od)

#----------------------------------------------------------
########### next target ###############
set(naskfunc.img_SRCS ${root_SOURCE_DIR}/projects/19_day/harib16d/naskfunc.nas)
set(naskfunc.img_OUTS ${root_BINARY_DIR}/projects/19_day/harib16d/naskfunc.img)
set(naskfunc_wine.img_OUTS ${root_BINARY_DIR}/projects/19_day/harib16d/naskfunc_wine.img)

add_custom_target(19_day_harib16d_naskfunc
  COMMAND ${root_BINARY_DIR}/src/opennask ${naskfunc.img_SRCS} ${naskfunc.img_OUTS}
)
add_custom_target(19_day_harib16d_naskfunc_wine
  COMMAND ${WINE} ${WINE_NASK} ${naskfunc.img_SRCS} ${naskfunc_wine.img_OUTS}
)
add_custom_target(19_day_harib16d_naskfunc_od
  COMMAND ${OD} -t x1 ${naskfunc.img_OUTS}      > ${naskfunc.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${naskfunc_wine.img_OUTS} > ${naskfunc_wine.img_OUTS}_t.txt
  COMMAND diff -s ${naskfunc.img_OUTS}_f.txt ${naskfunc_wine.img_OUTS}_t.txt
)

add_dependencies(19_day_harib16d_naskfunc_od 19_day_harib16d_naskfunc)
add_dependencies(19_day_harib16d_naskfunc_od 19_day_harib16d_naskfunc_wine)

add_dependencies(images 19_day_harib16d_naskfunc)
add_dependencies(wine 19_day_harib16d_naskfunc_wine)
add_dependencies(od 19_day_harib16d_naskfunc_od)

#----------------------------------------------------------
