#----------------------------------------------------------
message(STATUS "Entering directory projects/29_day/harib26a/haribote/")

set(NASK ${root_BINARY_DIR}/src/opennask)
set(FONT ${root_BINARY_DIR}/src/makefont)
set(B2O  ${root_BINARY_DIR}/src/bin2obj)
set(CONV ${root_BINARY_DIR}/objconv/objconv)
set(29_day_harib26a_haribote_OS    ${root_BINARY_DIR}/projects/29_day/harib26a/haribote/os.img)
set(29_day_harib26a_haribote_SYS	  ${root_BINARY_DIR}/projects/29_day/harib26a/haribote/os.sys)
set(29_day_harib26a_haribote_IPLB  ${root_BINARY_DIR}/projects/29_day/harib26a/haribote/ipl.bin)
set(29_day_harib26a_haribote_IPLS  ${root_SOURCE_DIR}/projects/29_day/harib26a/haribote/ipl10.nas)
set(29_day_harib26a_haribote_HEADB ${root_BINARY_DIR}/projects/29_day/harib26a/haribote/asmhead.bin)
set(29_day_harib26a_haribote_HEADS ${root_SOURCE_DIR}/projects/29_day/harib26a/haribote/asmhead.nas)
set(29_day_harib26a_haribote_BOOTB ${root_BINARY_DIR}/projects/29_day/harib26a/haribote/boot.bin)
set(29_day_harib26a_haribote_CCO	  ${root_BINARY_DIR}/projects/29_day/harib26a/haribote/boot.o)
set(29_day_harib26a_haribote_CCS	  ${root_SOURCE_DIR}/projects/29_day/harib26a/haribote/*.c)
set(29_day_harib26a_haribote_LDS	  ${root_SOURCE_DIR}/projects/os.lds)
set(29_day_harib26a_haribote_FUNCS ${root_SOURCE_DIR}/projects/29_day/harib26a/haribote/naskfunc.nas)
set(29_day_harib26a_haribote_FUNCO ${root_BINARY_DIR}/projects/29_day/harib26a/haribote/naskfunc.o)
set(29_day_harib26a_haribote_FONTS ${root_SOURCE_DIR}/projects/29_day/harib26a/haribote/hankaku.txt)
set(29_day_harib26a_haribote_FONTB ${root_BINARY_DIR}/projects/29_day/harib26a/haribote/hankaku.bin)
set(29_day_harib26a_haribote_FONTO ${root_BINARY_DIR}/projects/29_day/harib26a/haribote/hankaku.o)
set(29_day_harib26a_haribote_LIBGE ${root_BINARY_DIR}/golibc/libgo.a)
set(29_day_harib26a_haribote_LIBGC ${root_BINARY_DIR}/projects/29_day/harib26a/haribote/libgo.o)
set(29_day_harib26a_haribote_WILDOBJ ${root_BINARY_DIR}/projects/29_day/harib26a/haribote/*.o)


add_custom_target(29_day_harib26a_haribote_run
  COMMAND ${QEMU} ${QEMUOPT} ${29_day_harib26a_haribote_OS}
  DEPENDS 29_day_harib26a_haribote_img
)
add_custom_target(29_day_harib26a_haribote_debug
  COMMAND ${QEMU} -s -S ${QEMUOPT} ${29_day_harib26a_haribote_OS} -redir tcp:5555:127.0.0.1:1234 &
  DEPENDS 29_day_harib26a_haribote_img
)
add_custom_target(29_day_harib26a_haribote_clean
  COMMAND rm -f ${29_day_harib26a_haribote_OS}
  COMMAND rm -f ${29_day_harib26a_haribote_SYS}
  COMMAND rm -f ${29_day_harib26a_haribote_IPLB}
  COMMAND rm -f ${29_day_harib26a_haribote_HEADB}
  COMMAND rm -f ${29_day_harib26a_haribote_BOOTB}
  COMMAND rm -f ${29_day_harib26a_haribote_WILDOBJ}
)
add_custom_target(29_day_harib26a_haribote_ipl
  COMMAND ${NASK} ${29_day_harib26a_haribote_IPLS} ${29_day_harib26a_haribote_IPLB}
)
add_custom_target(29_day_harib26a_haribote_sys
  COMMAND ${NASK} ${29_day_harib26a_haribote_HEADS} ${29_day_harib26a_haribote_HEADB}
  COMMAND ${FONT} ${29_day_harib26a_haribote_FONTS} ${29_day_harib26a_haribote_FONTB}
  COMMAND ${B2O}  ${29_day_harib26a_haribote_FONTB} ${29_day_harib26a_haribote_FONTO} _hankaku
  COMMAND ${CONV} -fcoff32 -nu ${29_day_harib26a_haribote_LIBGE} ${29_day_harib26a_haribote_LIBGC}
  COMMAND ${NASK} ${29_day_harib26a_haribote_FUNCS} ${29_day_harib26a_haribote_FUNCO}
  COMMAND gcc ${BINOPT} -T ${29_day_harib26a_haribote_LDS} ${29_day_harib26a_haribote_CCS} ${29_day_harib26a_haribote_WILDOBJ} -o ${29_day_harib26a_haribote_BOOTB}
  COMMAND cat ${29_day_harib26a_haribote_HEADB} ${29_day_harib26a_haribote_BOOTB} > ${29_day_harib26a_haribote_SYS}
  DEPENDS 29_day_harib26a_haribote_ipl
)
add_custom_target(29_day_harib26a_haribote_img
  COMMAND mformat -f 1440 -l HARIBOTEOS -N 0xffffffff -C -B ${29_day_harib26a_haribote_IPLB} -i ${29_day_harib26a_haribote_OS}
  COMMAND mcopy -i ${29_day_harib26a_haribote_OS} ${29_day_harib26a_haribote_SYS} ::
  DEPENDS 29_day_harib26a_haribote_sys
)
########### next target ###############
set(asmhead.img_SRCS ${root_SOURCE_DIR}/projects/29_day/harib26a/haribote/asmhead.nas)
set(asmhead.img_OUTS ${root_BINARY_DIR}/projects/29_day/harib26a/haribote/asmhead.img)
set(asmhead_wine.img_OUTS ${root_BINARY_DIR}/projects/29_day/harib26a/haribote/asmhead_wine.img)

add_custom_target(29_day_harib26a_haribote_asmhead
  COMMAND ${root_BINARY_DIR}/src/opennask ${asmhead.img_SRCS} ${asmhead.img_OUTS}
)
add_custom_target(29_day_harib26a_haribote_asmhead_wine
  COMMAND ${WINE} ${WINE_NASK} ${asmhead.img_SRCS} ${asmhead_wine.img_OUTS}
)
add_custom_target(29_day_harib26a_haribote_asmhead_od
  COMMAND ${OD} -t x1 ${asmhead.img_OUTS}      > ${asmhead.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${asmhead_wine.img_OUTS} > ${asmhead_wine.img_OUTS}_t.txt
  COMMAND diff -s ${asmhead.img_OUTS}_f.txt ${asmhead_wine.img_OUTS}_t.txt
)

add_dependencies(29_day_harib26a_haribote_asmhead_od 29_day_harib26a_haribote_asmhead)
add_dependencies(29_day_harib26a_haribote_asmhead_od 29_day_harib26a_haribote_asmhead_wine)

add_dependencies(images 29_day_harib26a_haribote_asmhead)
add_dependencies(wine 29_day_harib26a_haribote_asmhead_wine)
add_dependencies(od 29_day_harib26a_haribote_asmhead_od)

#----------------------------------------------------------
########### next target ###############
set(ipl20.img_SRCS ${root_SOURCE_DIR}/projects/29_day/harib26a/haribote/ipl20.nas)
set(ipl20.img_OUTS ${root_BINARY_DIR}/projects/29_day/harib26a/haribote/ipl20.img)
set(ipl20_wine.img_OUTS ${root_BINARY_DIR}/projects/29_day/harib26a/haribote/ipl20_wine.img)

add_custom_target(29_day_harib26a_haribote_ipl20
  COMMAND ${root_BINARY_DIR}/src/opennask ${ipl20.img_SRCS} ${ipl20.img_OUTS}
)
set(ipl20.img_HRB ${root_BINARY_DIR}/projects/29_day/harib26a/haribote/ipl20.hrb)
add_custom_target(29_day_harib26a_haribote_ipl20_hrb
  COMMAND ${root_BINARY_DIR}/src/opennask ${ipl20.img_SRCS} ${ipl20.img_HRB}
  COMMAND mcopy -i ${29_day_harib26a_haribote_OS} ${ipl20.img_HRB} ::ipl20.hrb
  DEPENDS 29_day_harib26a_haribote_img
)

add_dependencies(29_day_harib26a_haribote_run 29_day_harib26a_haribote_ipl20_hrb)
add_custom_target(29_day_harib26a_haribote_ipl20_wine
  COMMAND ${WINE} ${WINE_NASK} ${ipl20.img_SRCS} ${ipl20_wine.img_OUTS}
)
add_custom_target(29_day_harib26a_haribote_ipl20_od
  COMMAND ${OD} -t x1 ${ipl20.img_OUTS}      > ${ipl20.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${ipl20_wine.img_OUTS} > ${ipl20_wine.img_OUTS}_t.txt
  COMMAND diff -s ${ipl20.img_OUTS}_f.txt ${ipl20_wine.img_OUTS}_t.txt
)

add_dependencies(29_day_harib26a_haribote_ipl20_od 29_day_harib26a_haribote_ipl20)
add_dependencies(29_day_harib26a_haribote_ipl20_od 29_day_harib26a_haribote_ipl20_wine)

add_dependencies(images 29_day_harib26a_haribote_ipl20)
add_dependencies(wine 29_day_harib26a_haribote_ipl20_wine)
add_dependencies(od 29_day_harib26a_haribote_ipl20_od)

#----------------------------------------------------------
########### next target ###############
set(naskfunc.img_SRCS ${root_SOURCE_DIR}/projects/29_day/harib26a/haribote/naskfunc.nas)
set(naskfunc.img_OUTS ${root_BINARY_DIR}/projects/29_day/harib26a/haribote/naskfunc.img)
set(naskfunc_wine.img_OUTS ${root_BINARY_DIR}/projects/29_day/harib26a/haribote/naskfunc_wine.img)

add_custom_target(29_day_harib26a_haribote_naskfunc
  COMMAND ${root_BINARY_DIR}/src/opennask ${naskfunc.img_SRCS} ${naskfunc.img_OUTS}
)
add_custom_target(29_day_harib26a_haribote_naskfunc_wine
  COMMAND ${WINE} ${WINE_NASK} ${naskfunc.img_SRCS} ${naskfunc_wine.img_OUTS}
)
add_custom_target(29_day_harib26a_haribote_naskfunc_od
  COMMAND ${OD} -t x1 ${naskfunc.img_OUTS}      > ${naskfunc.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${naskfunc_wine.img_OUTS} > ${naskfunc_wine.img_OUTS}_t.txt
  COMMAND diff -s ${naskfunc.img_OUTS}_f.txt ${naskfunc_wine.img_OUTS}_t.txt
)

add_dependencies(29_day_harib26a_haribote_naskfunc_od 29_day_harib26a_haribote_naskfunc)
add_dependencies(29_day_harib26a_haribote_naskfunc_od 29_day_harib26a_haribote_naskfunc_wine)

add_dependencies(images 29_day_harib26a_haribote_naskfunc)
add_dependencies(wine 29_day_harib26a_haribote_naskfunc_wine)
add_dependencies(od 29_day_harib26a_haribote_naskfunc_od)

#----------------------------------------------------------
