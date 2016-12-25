#----------------------------------------------------------
message(STATUS "Entering directory projects/22_day/harib19a/")

set(NASK ${root_BINARY_DIR}/src/opennask)
set(FONT ${root_BINARY_DIR}/src/makefont)
set(B2O  ${root_BINARY_DIR}/src/bin2obj)
set(CONV ${root_BINARY_DIR}/objconv/objconv)
set(22_day_harib19a_OS    ${root_BINARY_DIR}/projects/22_day/harib19a/os.img)
set(22_day_harib19a_SYS	  ${root_BINARY_DIR}/projects/22_day/harib19a/os.sys)
set(22_day_harib19a_IPLB  ${root_BINARY_DIR}/projects/22_day/harib19a/ipl.bin)
set(22_day_harib19a_IPLS  ${root_SOURCE_DIR}/projects/22_day/harib19a/ipl10.nas)
set(22_day_harib19a_HEADB ${root_BINARY_DIR}/projects/22_day/harib19a/asmhead.bin)
set(22_day_harib19a_HEADS ${root_SOURCE_DIR}/projects/22_day/harib19a/asmhead.nas)
set(22_day_harib19a_BOOTB ${root_BINARY_DIR}/projects/22_day/harib19a/boot.bin)
set(22_day_harib19a_CCO	  ${root_BINARY_DIR}/projects/22_day/harib19a/boot.o)
set(22_day_harib19a_CCS	  ${root_SOURCE_DIR}/projects/22_day/harib19a/*.c)
set(22_day_harib19a_LDS	  ${root_SOURCE_DIR}/projects/os.lds)
set(22_day_harib19a_FUNCS ${root_SOURCE_DIR}/projects/22_day/harib19a/naskfunc.nas)
set(22_day_harib19a_FUNCO ${root_BINARY_DIR}/projects/22_day/harib19a/naskfunc.o)
set(22_day_harib19a_FONTS ${root_SOURCE_DIR}/projects/22_day/harib19a/hankaku.txt)
set(22_day_harib19a_FONTB ${root_BINARY_DIR}/projects/22_day/harib19a/hankaku.bin)
set(22_day_harib19a_FONTO ${root_BINARY_DIR}/projects/22_day/harib19a/hankaku.o)
set(22_day_harib19a_LIBGE ${root_BINARY_DIR}/golibc/libgo.a)
set(22_day_harib19a_LIBGC ${root_BINARY_DIR}/projects/22_day/harib19a/libgo.o)
set(22_day_harib19a_WILDOBJ ${root_BINARY_DIR}/projects/22_day/harib19a/*.o)


add_custom_target(22_day_harib19a_a_nask_run
  COMMAND ${QEMU} ${QEMUOPT} ${22_day_harib19a_OS}
  DEPENDS 22_day_harib19a_img
)
add_custom_target(22_day_harib19a_a_nask_debug
  COMMAND ${QEMU} -s -S ${QEMUOPT} ${22_day_harib19a_OS} -redir tcp:5555:127.0.0.1:1234 &
  DEPENDS 22_day_harib19a_img
)
add_custom_target(22_day_harib19a_a_nask_clean
  COMMAND rm -f ${22_day_harib19a_OS}
  COMMAND rm -f ${22_day_harib19a_SYS}
  COMMAND rm -f ${22_day_harib19a_IPLB}
  COMMAND rm -f ${22_day_harib19a_HEADB}
  COMMAND rm -f ${22_day_harib19a_BOOTB}
  COMMAND rm -f ${22_day_harib19a_WILDOBJ}
)
add_custom_target(22_day_harib19a_a_nask_ipl
  COMMAND ${NASK} ${22_day_harib19a_IPLS} ${22_day_harib19a_IPLB}
)
add_custom_target(22_day_harib19a_a_nask_sys
  COMMAND ${NASK} ${22_day_harib19a_HEADS} ${22_day_harib19a_HEADB}
  COMMAND ${FONT} ${22_day_harib19a_FONTS} ${22_day_harib19a_FONTB}
  COMMAND ${B2O}  ${22_day_harib19a_FONTB} ${22_day_harib19a_FONTO} _hankaku
  COMMAND ${CONV} -fcoff32 -nu ${22_day_harib19a_LIBGE} ${22_day_harib19a_LIBGC}
  COMMAND ${NASK} ${22_day_harib19a_FUNCS} ${22_day_harib19a_FUNCO}
  COMMAND gcc ${BINOPT} -T ${22_day_harib19a_LDS} ${22_day_harib19a_CCS} ${22_day_harib19a_WILDOBJ} -o ${22_day_harib19a_BOOTB}
  COMMAND cat ${22_day_harib19a_HEADB} ${22_day_harib19a_BOOTB} > ${22_day_harib19a_SYS}
  DEPENDS 22_day_harib19a_ipl
)
add_custom_target(22_day_harib19a_a_nask_img
  COMMAND mformat -f 1440 -l HARIBOTEOS -N 0xffffffff -C -B ${22_day_harib19a_IPLB} -i ${22_day_harib19a_OS}
  COMMAND mcopy -i ${22_day_harib19a_OS} ${22_day_harib19a_SYS} ::
  DEPENDS 22_day_harib19a_sys
)
########### next target ###############
set(a.imgk.img_SRCS ${root_SOURCE_DIR}/projects/22_day/harib19a/a_nask.nas)
set(a.imgk.img_OUTS ${root_BINARY_DIR}/projects/22_day/harib19a/a_nask.img)
set(a_wine.imgk_wine.img_OUTS ${root_BINARY_DIR}/projects/22_day/harib19a/a_nask_wine.img)

add_custom_target(22_day_harib19a_a_nask
  COMMAND ${root_BINARY_DIR}/src/opennask ${a.imgk.img_SRCS} ${a.imgk.img_OUTS}
)
set(a.imgk.img_HRB ${root_BINARY_DIR}/projects/22_day/harib19a/a_nask.hrb)
add_custom_target(22_day_harib19a_a_nask_hrb
  COMMAND ${root_BINARY_DIR}/src/opennask ${a.imgk.img_SRCS} ${a.imgk.img_HRB}
  COMMAND mcopy -i ${22_day_harib19a_OS} ${a.imgk.img_HRB} ::a.hrbk.hrb
  DEPENDS 22_day_harib19a_img
)

add_dependencies(22_day_harib19a_run 22_day_harib19a_a_nask_hrb)
add_custom_target(22_day_harib19a_a_nask_wine
  COMMAND ${WINE} ${WINE_NASK} ${a.imgk.img_SRCS} ${a_wine.imgk_wine.img_OUTS}
)
add_custom_target(22_day_harib19a_a_nask_od
  COMMAND ${OD} -t x1 ${a.imgk.img_OUTS}      > ${a.imgk.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${a_wine.imgk_wine.img_OUTS} > ${a_wine.imgk_wine.img_OUTS}_t.txt
  COMMAND diff -s ${a.imgk.img_OUTS}_f.txt ${a_wine.imgk_wine.img_OUTS}_t.txt
)

add_dependencies(22_day_harib19a_a_nask_od 22_day_harib19a_a_nask)
add_dependencies(22_day_harib19a_a_nask_od 22_day_harib19a_a_nask_wine)

add_dependencies(images 22_day_harib19a_a_nask)
add_dependencies(wine 22_day_harib19a_a_nask_wine)
add_dependencies(od 22_day_harib19a_a_nask_od)

#----------------------------------------------------------
########### next target ###############
set(asmhead.img_SRCS ${root_SOURCE_DIR}/projects/22_day/harib19a/asmhead.nas)
set(asmhead.img_OUTS ${root_BINARY_DIR}/projects/22_day/harib19a/asmhead.img)
set(asmhead_wine.img_OUTS ${root_BINARY_DIR}/projects/22_day/harib19a/asmhead_wine.img)

add_custom_target(22_day_harib19a_asmhead
  COMMAND ${root_BINARY_DIR}/src/opennask ${asmhead.img_SRCS} ${asmhead.img_OUTS}
)
add_custom_target(22_day_harib19a_asmhead_wine
  COMMAND ${WINE} ${WINE_NASK} ${asmhead.img_SRCS} ${asmhead_wine.img_OUTS}
)
add_custom_target(22_day_harib19a_asmhead_od
  COMMAND ${OD} -t x1 ${asmhead.img_OUTS}      > ${asmhead.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${asmhead_wine.img_OUTS} > ${asmhead_wine.img_OUTS}_t.txt
  COMMAND diff -s ${asmhead.img_OUTS}_f.txt ${asmhead_wine.img_OUTS}_t.txt
)

add_dependencies(22_day_harib19a_asmhead_od 22_day_harib19a_asmhead)
add_dependencies(22_day_harib19a_asmhead_od 22_day_harib19a_asmhead_wine)

add_dependencies(images 22_day_harib19a_asmhead)
add_dependencies(wine 22_day_harib19a_asmhead_wine)
add_dependencies(od 22_day_harib19a_asmhead_od)

#----------------------------------------------------------
########### next target ###############
set(crack2.img_SRCS ${root_SOURCE_DIR}/projects/22_day/harib19a/crack2.nas)
set(crack2.img_OUTS ${root_BINARY_DIR}/projects/22_day/harib19a/crack2.img)
set(crack2_wine.img_OUTS ${root_BINARY_DIR}/projects/22_day/harib19a/crack2_wine.img)

add_custom_target(22_day_harib19a_crack2
  COMMAND ${root_BINARY_DIR}/src/opennask ${crack2.img_SRCS} ${crack2.img_OUTS}
)
set(crack2.img_HRB ${root_BINARY_DIR}/projects/22_day/harib19a/crack2.hrb)
add_custom_target(22_day_harib19a_crack2_hrb
  COMMAND ${root_BINARY_DIR}/src/opennask ${crack2.img_SRCS} ${crack2.img_HRB}
  COMMAND mcopy -i ${22_day_harib19a_OS} ${crack2.img_HRB} ::crack2.hrb
  DEPENDS 22_day_harib19a_img
)

add_dependencies(22_day_harib19a_run 22_day_harib19a_crack2_hrb)
add_custom_target(22_day_harib19a_crack2_wine
  COMMAND ${WINE} ${WINE_NASK} ${crack2.img_SRCS} ${crack2_wine.img_OUTS}
)
add_custom_target(22_day_harib19a_crack2_od
  COMMAND ${OD} -t x1 ${crack2.img_OUTS}      > ${crack2.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${crack2_wine.img_OUTS} > ${crack2_wine.img_OUTS}_t.txt
  COMMAND diff -s ${crack2.img_OUTS}_f.txt ${crack2_wine.img_OUTS}_t.txt
)

add_dependencies(22_day_harib19a_crack2_od 22_day_harib19a_crack2)
add_dependencies(22_day_harib19a_crack2_od 22_day_harib19a_crack2_wine)

add_dependencies(images 22_day_harib19a_crack2)
add_dependencies(wine 22_day_harib19a_crack2_wine)
add_dependencies(od 22_day_harib19a_crack2_od)

#----------------------------------------------------------
########### next target ###############
set(crack3.img_SRCS ${root_SOURCE_DIR}/projects/22_day/harib19a/crack3.nas)
set(crack3.img_OUTS ${root_BINARY_DIR}/projects/22_day/harib19a/crack3.img)
set(crack3_wine.img_OUTS ${root_BINARY_DIR}/projects/22_day/harib19a/crack3_wine.img)

add_custom_target(22_day_harib19a_crack3
  COMMAND ${root_BINARY_DIR}/src/opennask ${crack3.img_SRCS} ${crack3.img_OUTS}
)
set(crack3.img_HRB ${root_BINARY_DIR}/projects/22_day/harib19a/crack3.hrb)
add_custom_target(22_day_harib19a_crack3_hrb
  COMMAND ${root_BINARY_DIR}/src/opennask ${crack3.img_SRCS} ${crack3.img_HRB}
  COMMAND mcopy -i ${22_day_harib19a_OS} ${crack3.img_HRB} ::crack3.hrb
  DEPENDS 22_day_harib19a_img
)

add_dependencies(22_day_harib19a_run 22_day_harib19a_crack3_hrb)
add_custom_target(22_day_harib19a_crack3_wine
  COMMAND ${WINE} ${WINE_NASK} ${crack3.img_SRCS} ${crack3_wine.img_OUTS}
)
add_custom_target(22_day_harib19a_crack3_od
  COMMAND ${OD} -t x1 ${crack3.img_OUTS}      > ${crack3.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${crack3_wine.img_OUTS} > ${crack3_wine.img_OUTS}_t.txt
  COMMAND diff -s ${crack3.img_OUTS}_f.txt ${crack3_wine.img_OUTS}_t.txt
)

add_dependencies(22_day_harib19a_crack3_od 22_day_harib19a_crack3)
add_dependencies(22_day_harib19a_crack3_od 22_day_harib19a_crack3_wine)

add_dependencies(images 22_day_harib19a_crack3)
add_dependencies(wine 22_day_harib19a_crack3_wine)
add_dependencies(od 22_day_harib19a_crack3_od)

#----------------------------------------------------------
########### next target ###############
set(crack4.img_SRCS ${root_SOURCE_DIR}/projects/22_day/harib19a/crack4.nas)
set(crack4.img_OUTS ${root_BINARY_DIR}/projects/22_day/harib19a/crack4.img)
set(crack4_wine.img_OUTS ${root_BINARY_DIR}/projects/22_day/harib19a/crack4_wine.img)

add_custom_target(22_day_harib19a_crack4
  COMMAND ${root_BINARY_DIR}/src/opennask ${crack4.img_SRCS} ${crack4.img_OUTS}
)
set(crack4.img_HRB ${root_BINARY_DIR}/projects/22_day/harib19a/crack4.hrb)
add_custom_target(22_day_harib19a_crack4_hrb
  COMMAND ${root_BINARY_DIR}/src/opennask ${crack4.img_SRCS} ${crack4.img_HRB}
  COMMAND mcopy -i ${22_day_harib19a_OS} ${crack4.img_HRB} ::crack4.hrb
  DEPENDS 22_day_harib19a_img
)

add_dependencies(22_day_harib19a_run 22_day_harib19a_crack4_hrb)
add_custom_target(22_day_harib19a_crack4_wine
  COMMAND ${WINE} ${WINE_NASK} ${crack4.img_SRCS} ${crack4_wine.img_OUTS}
)
add_custom_target(22_day_harib19a_crack4_od
  COMMAND ${OD} -t x1 ${crack4.img_OUTS}      > ${crack4.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${crack4_wine.img_OUTS} > ${crack4_wine.img_OUTS}_t.txt
  COMMAND diff -s ${crack4.img_OUTS}_f.txt ${crack4_wine.img_OUTS}_t.txt
)

add_dependencies(22_day_harib19a_crack4_od 22_day_harib19a_crack4)
add_dependencies(22_day_harib19a_crack4_od 22_day_harib19a_crack4_wine)

add_dependencies(images 22_day_harib19a_crack4)
add_dependencies(wine 22_day_harib19a_crack4_wine)
add_dependencies(od 22_day_harib19a_crack4_od)

#----------------------------------------------------------
########### next target ###############
set(crack5.img_SRCS ${root_SOURCE_DIR}/projects/22_day/harib19a/crack5.nas)
set(crack5.img_OUTS ${root_BINARY_DIR}/projects/22_day/harib19a/crack5.img)
set(crack5_wine.img_OUTS ${root_BINARY_DIR}/projects/22_day/harib19a/crack5_wine.img)

add_custom_target(22_day_harib19a_crack5
  COMMAND ${root_BINARY_DIR}/src/opennask ${crack5.img_SRCS} ${crack5.img_OUTS}
)
set(crack5.img_HRB ${root_BINARY_DIR}/projects/22_day/harib19a/crack5.hrb)
add_custom_target(22_day_harib19a_crack5_hrb
  COMMAND ${root_BINARY_DIR}/src/opennask ${crack5.img_SRCS} ${crack5.img_HRB}
  COMMAND mcopy -i ${22_day_harib19a_OS} ${crack5.img_HRB} ::crack5.hrb
  DEPENDS 22_day_harib19a_img
)

add_dependencies(22_day_harib19a_run 22_day_harib19a_crack5_hrb)
add_custom_target(22_day_harib19a_crack5_wine
  COMMAND ${WINE} ${WINE_NASK} ${crack5.img_SRCS} ${crack5_wine.img_OUTS}
)
add_custom_target(22_day_harib19a_crack5_od
  COMMAND ${OD} -t x1 ${crack5.img_OUTS}      > ${crack5.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${crack5_wine.img_OUTS} > ${crack5_wine.img_OUTS}_t.txt
  COMMAND diff -s ${crack5.img_OUTS}_f.txt ${crack5_wine.img_OUTS}_t.txt
)

add_dependencies(22_day_harib19a_crack5_od 22_day_harib19a_crack5)
add_dependencies(22_day_harib19a_crack5_od 22_day_harib19a_crack5_wine)

add_dependencies(images 22_day_harib19a_crack5)
add_dependencies(wine 22_day_harib19a_crack5_wine)
add_dependencies(od 22_day_harib19a_crack5_od)

#----------------------------------------------------------
########### next target ###############
set(crack6.img_SRCS ${root_SOURCE_DIR}/projects/22_day/harib19a/crack6.nas)
set(crack6.img_OUTS ${root_BINARY_DIR}/projects/22_day/harib19a/crack6.img)
set(crack6_wine.img_OUTS ${root_BINARY_DIR}/projects/22_day/harib19a/crack6_wine.img)

add_custom_target(22_day_harib19a_crack6
  COMMAND ${root_BINARY_DIR}/src/opennask ${crack6.img_SRCS} ${crack6.img_OUTS}
)
set(crack6.img_HRB ${root_BINARY_DIR}/projects/22_day/harib19a/crack6.hrb)
add_custom_target(22_day_harib19a_crack6_hrb
  COMMAND ${root_BINARY_DIR}/src/opennask ${crack6.img_SRCS} ${crack6.img_HRB}
  COMMAND mcopy -i ${22_day_harib19a_OS} ${crack6.img_HRB} ::crack6.hrb
  DEPENDS 22_day_harib19a_img
)

add_dependencies(22_day_harib19a_run 22_day_harib19a_crack6_hrb)
add_custom_target(22_day_harib19a_crack6_wine
  COMMAND ${WINE} ${WINE_NASK} ${crack6.img_SRCS} ${crack6_wine.img_OUTS}
)
add_custom_target(22_day_harib19a_crack6_od
  COMMAND ${OD} -t x1 ${crack6.img_OUTS}      > ${crack6.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${crack6_wine.img_OUTS} > ${crack6_wine.img_OUTS}_t.txt
  COMMAND diff -s ${crack6.img_OUTS}_f.txt ${crack6_wine.img_OUTS}_t.txt
)

add_dependencies(22_day_harib19a_crack6_od 22_day_harib19a_crack6)
add_dependencies(22_day_harib19a_crack6_od 22_day_harib19a_crack6_wine)

add_dependencies(images 22_day_harib19a_crack6)
add_dependencies(wine 22_day_harib19a_crack6_wine)
add_dependencies(od 22_day_harib19a_crack6_od)

#----------------------------------------------------------
########### next target ###############
set(hello.img_SRCS ${root_SOURCE_DIR}/projects/22_day/harib19a/hello.nas)
set(hello.img_OUTS ${root_BINARY_DIR}/projects/22_day/harib19a/hello.img)
set(hello_wine.img_OUTS ${root_BINARY_DIR}/projects/22_day/harib19a/hello_wine.img)

add_custom_target(22_day_harib19a_hello
  COMMAND ${root_BINARY_DIR}/src/opennask ${hello.img_SRCS} ${hello.img_OUTS}
)
set(hello.img_HRB ${root_BINARY_DIR}/projects/22_day/harib19a/hello.hrb)
add_custom_target(22_day_harib19a_hello_hrb
  COMMAND ${root_BINARY_DIR}/src/opennask ${hello.img_SRCS} ${hello.img_HRB}
  COMMAND mcopy -i ${22_day_harib19a_OS} ${hello.img_HRB} ::hello.hrb
  DEPENDS 22_day_harib19a_img
)

add_dependencies(22_day_harib19a_run 22_day_harib19a_hello_hrb)
add_custom_target(22_day_harib19a_hello_wine
  COMMAND ${WINE} ${WINE_NASK} ${hello.img_SRCS} ${hello_wine.img_OUTS}
)
add_custom_target(22_day_harib19a_hello_od
  COMMAND ${OD} -t x1 ${hello.img_OUTS}      > ${hello.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${hello_wine.img_OUTS} > ${hello_wine.img_OUTS}_t.txt
  COMMAND diff -s ${hello.img_OUTS}_f.txt ${hello_wine.img_OUTS}_t.txt
)

add_dependencies(22_day_harib19a_hello_od 22_day_harib19a_hello)
add_dependencies(22_day_harib19a_hello_od 22_day_harib19a_hello_wine)

add_dependencies(images 22_day_harib19a_hello)
add_dependencies(wine 22_day_harib19a_hello_wine)
add_dependencies(od 22_day_harib19a_hello_od)

#----------------------------------------------------------
########### next target ###############
set(hello2.img_SRCS ${root_SOURCE_DIR}/projects/22_day/harib19a/hello2.nas)
set(hello2.img_OUTS ${root_BINARY_DIR}/projects/22_day/harib19a/hello2.img)
set(hello2_wine.img_OUTS ${root_BINARY_DIR}/projects/22_day/harib19a/hello2_wine.img)

add_custom_target(22_day_harib19a_hello2
  COMMAND ${root_BINARY_DIR}/src/opennask ${hello2.img_SRCS} ${hello2.img_OUTS}
)
set(hello2.img_HRB ${root_BINARY_DIR}/projects/22_day/harib19a/hello2.hrb)
add_custom_target(22_day_harib19a_hello2_hrb
  COMMAND ${root_BINARY_DIR}/src/opennask ${hello2.img_SRCS} ${hello2.img_HRB}
  COMMAND mcopy -i ${22_day_harib19a_OS} ${hello2.img_HRB} ::hello2.hrb
  DEPENDS 22_day_harib19a_img
)

add_dependencies(22_day_harib19a_run 22_day_harib19a_hello2_hrb)
add_custom_target(22_day_harib19a_hello2_wine
  COMMAND ${WINE} ${WINE_NASK} ${hello2.img_SRCS} ${hello2_wine.img_OUTS}
)
add_custom_target(22_day_harib19a_hello2_od
  COMMAND ${OD} -t x1 ${hello2.img_OUTS}      > ${hello2.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${hello2_wine.img_OUTS} > ${hello2_wine.img_OUTS}_t.txt
  COMMAND diff -s ${hello2.img_OUTS}_f.txt ${hello2_wine.img_OUTS}_t.txt
)

add_dependencies(22_day_harib19a_hello2_od 22_day_harib19a_hello2)
add_dependencies(22_day_harib19a_hello2_od 22_day_harib19a_hello2_wine)

add_dependencies(images 22_day_harib19a_hello2)
add_dependencies(wine 22_day_harib19a_hello2_wine)
add_dependencies(od 22_day_harib19a_hello2_od)

#----------------------------------------------------------
########### next target ###############
set(ipl10.img_SRCS ${root_SOURCE_DIR}/projects/22_day/harib19a/ipl10.nas)
set(ipl10.img_OUTS ${root_BINARY_DIR}/projects/22_day/harib19a/ipl10.img)
set(ipl10_wine.img_OUTS ${root_BINARY_DIR}/projects/22_day/harib19a/ipl10_wine.img)

add_custom_target(22_day_harib19a_ipl10
  COMMAND ${root_BINARY_DIR}/src/opennask ${ipl10.img_SRCS} ${ipl10.img_OUTS}
)
add_custom_target(22_day_harib19a_ipl10_wine
  COMMAND ${WINE} ${WINE_NASK} ${ipl10.img_SRCS} ${ipl10_wine.img_OUTS}
)
add_custom_target(22_day_harib19a_ipl10_od
  COMMAND ${OD} -t x1 ${ipl10.img_OUTS}      > ${ipl10.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${ipl10_wine.img_OUTS} > ${ipl10_wine.img_OUTS}_t.txt
  COMMAND diff -s ${ipl10.img_OUTS}_f.txt ${ipl10_wine.img_OUTS}_t.txt
)

add_dependencies(22_day_harib19a_ipl10_od 22_day_harib19a_ipl10)
add_dependencies(22_day_harib19a_ipl10_od 22_day_harib19a_ipl10_wine)

add_dependencies(images 22_day_harib19a_ipl10)
add_dependencies(wine 22_day_harib19a_ipl10_wine)
add_dependencies(od 22_day_harib19a_ipl10_od)

#----------------------------------------------------------
########### next target ###############
set(naskfunc.img_SRCS ${root_SOURCE_DIR}/projects/22_day/harib19a/naskfunc.nas)
set(naskfunc.img_OUTS ${root_BINARY_DIR}/projects/22_day/harib19a/naskfunc.img)
set(naskfunc_wine.img_OUTS ${root_BINARY_DIR}/projects/22_day/harib19a/naskfunc_wine.img)

add_custom_target(22_day_harib19a_naskfunc
  COMMAND ${root_BINARY_DIR}/src/opennask ${naskfunc.img_SRCS} ${naskfunc.img_OUTS}
)
add_custom_target(22_day_harib19a_naskfunc_wine
  COMMAND ${WINE} ${WINE_NASK} ${naskfunc.img_SRCS} ${naskfunc_wine.img_OUTS}
)
add_custom_target(22_day_harib19a_naskfunc_od
  COMMAND ${OD} -t x1 ${naskfunc.img_OUTS}      > ${naskfunc.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${naskfunc_wine.img_OUTS} > ${naskfunc_wine.img_OUTS}_t.txt
  COMMAND diff -s ${naskfunc.img_OUTS}_f.txt ${naskfunc_wine.img_OUTS}_t.txt
)

add_dependencies(22_day_harib19a_naskfunc_od 22_day_harib19a_naskfunc)
add_dependencies(22_day_harib19a_naskfunc_od 22_day_harib19a_naskfunc_wine)

add_dependencies(images 22_day_harib19a_naskfunc)
add_dependencies(wine 22_day_harib19a_naskfunc_wine)
add_dependencies(od 22_day_harib19a_naskfunc_od)

#----------------------------------------------------------
