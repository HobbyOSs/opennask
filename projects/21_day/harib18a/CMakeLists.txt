#----------------------------------------------------------
message(STATUS "Entering directory projects/21_day/harib18a/")

set(FONT ${root_BINARY_DIR}/src/makefont)
set(B2O  ${root_BINARY_DIR}/src/bin2obj)
set(CONV ${root_BINARY_DIR}/objconv/objconv)
set(21_day_harib18a_OS    ${root_BINARY_DIR}/projects/21_day/harib18a/os.img)
set(21_day_harib18a_SYS	  ${root_BINARY_DIR}/projects/21_day/harib18a/os.sys)
set(21_day_harib18a_IPLB  ${root_BINARY_DIR}/projects/21_day/harib18a/ipl.bin)
set(21_day_harib18a_IPLS  ${root_SOURCE_DIR}/projects/21_day/harib18a/ipl10.nas)
set(21_day_harib18a_HEADB ${root_BINARY_DIR}/projects/21_day/harib18a/asmhead.bin)
set(21_day_harib18a_HEADS ${root_SOURCE_DIR}/projects/21_day/harib18a/asmhead.nas)
set(21_day_harib18a_BOOTB ${root_BINARY_DIR}/projects/21_day/harib18a/boot.bin)
set(21_day_harib18a_CCO	  ${root_BINARY_DIR}/projects/21_day/harib18a/boot.o)
set(21_day_harib18a_CCS	  ${root_SOURCE_DIR}/projects/21_day/harib18a/*.c)
set(21_day_harib18a_LDS	  ${root_SOURCE_DIR}/projects/os.lds)
set(21_day_harib18a_FUNCS ${root_SOURCE_DIR}/projects/21_day/harib18a/naskfunc.nas)
set(21_day_harib18a_FUNCO ${root_BINARY_DIR}/projects/21_day/harib18a/naskfunc.o)
set(21_day_harib18a_FONTS ${root_SOURCE_DIR}/projects/21_day/harib18a/hankaku.txt)
set(21_day_harib18a_FONTB ${root_BINARY_DIR}/projects/21_day/harib18a/hankaku.bin)
set(21_day_harib18a_FONTO ${root_BINARY_DIR}/projects/21_day/harib18a/hankaku.o)
set(21_day_harib18a_LIBGE ${root_BINARY_DIR}/golibc/libgo.a)
set(21_day_harib18a_LIBGC ${root_BINARY_DIR}/projects/21_day/harib18a/libgo.o)
set(21_day_harib18a_WILDOBJ ${root_BINARY_DIR}/projects/21_day/harib18a/*.o)


add_custom_target(21_day_harib18a_run
  COMMAND ${QEMU} ${QEMUOPT} ${21_day_harib18a_OS}
  DEPENDS 21_day_harib18a_img
)
add_custom_target(21_day_harib18a_debug
  COMMAND ${QEMU} -s -S ${QEMUOPT} ${21_day_harib18a_OS} ${QEMU_DEBUG_OPT}
  DEPENDS 21_day_harib18a_img
)
add_custom_target(21_day_harib18a_clean
  COMMAND rm -f ${21_day_harib18a_OS}
  COMMAND rm -f ${21_day_harib18a_SYS}
  COMMAND rm -f ${21_day_harib18a_IPLB}
  COMMAND rm -f ${21_day_harib18a_HEADB}
  COMMAND rm -f ${21_day_harib18a_BOOTB}
  COMMAND rm -f ${21_day_harib18a_WILDOBJ}
)
add_custom_target(21_day_harib18a_ipl
  COMMAND ${RAKUSK_EXECUTABLE} ${21_day_harib18a_IPLS} ${21_day_harib18a_IPLB}
  DEPENDS ${21_day_harib18a_IPLS}
)
add_custom_command(
  OUTPUT ${21_day_harib18a_HEADB}
  COMMAND ${RAKUSK_EXECUTABLE} ${21_day_harib18a_HEADS} ${21_day_harib18a_HEADB}
  DEPENDS ${21_day_harib18a_HEADS}
  COMMENT "Generating asmhead.bin for 21_day_harib18a"
)
add_custom_command(
  OUTPUT ${21_day_harib18a_FUNCO}
  COMMAND ${RAKUSK_EXECUTABLE} ${21_day_harib18a_FUNCS} ${21_day_harib18a_FUNCO}
  DEPENDS ${21_day_harib18a_FUNCS}
  COMMENT "Generating naskfunc.o for 21_day_harib18a"
)
set(FONT ${root_BINARY_DIR}/src/makefont)
set(B2O  ${root_BINARY_DIR}/src/bin2obj)
set(CONV ${root_BINARY_DIR}/objconv/objconv)
add_custom_command(
  OUTPUT ${21_day_harib18a_FONTO}
  COMMAND ${FONT} ${21_day_harib18a_FONTS} ${21_day_harib18a_FONTB}
  COMMAND ${B2O}  ${21_day_harib18a_FONTB} ${21_day_harib18a_FONTO} _hankaku
  DEPENDS ${21_day_harib18a_FONTS} makefont bin2obj
  COMMENT "Generating hankaku.o for 21_day_harib18a"
)
add_custom_command(
  OUTPUT ${21_day_harib18a_LIBGC}
  COMMAND ${CONV} -fcoff32 -nu ${21_day_harib18a_LIBGE} ${21_day_harib18a_LIBGC}
  DEPENDS ${21_day_harib18a_LIBGE}
  COMMENT "Converting golibc for 21_day_harib18a"
)

add_custom_target(21_day_harib18a_sys
  COMMAND gcc ${BINOPT} -T ${21_day_harib18a_LDS} ${21_day_harib18a_CCS} ${21_day_harib18a_WILDOBJ} -o ${21_day_harib18a_BOOTB}
  COMMAND cat ${21_day_harib18a_HEADB} ${21_day_harib18a_BOOTB} > ${21_day_harib18a_SYS}
  DEPENDS ${21_day_harib18a_HEADB} 21_day_harib18a_ipl
  DEPENDS ${21_day_harib18a_FUNCO}
  DEPENDS ${21_day_harib18a_FONTO} ${21_day_harib18a_LIBGC}
)
add_custom_target(21_day_harib18a_img
  COMMAND mformat -f 1440 -l HARIBOTEOS -N 0xffffffff -C -B ${21_day_harib18a_IPLB} -i ${21_day_harib18a_OS}
  COMMAND mcopy -i ${21_day_harib18a_OS} ${21_day_harib18a_SYS} ::
  DEPENDS 21_day_harib18a_sys
)
########### next target ###############
set(asmhead.img_SRCS ${root_SOURCE_DIR}/projects/21_day/harib18a/asmhead.nas)
set(asmhead.img_OUTS ${root_BINARY_DIR}/projects/21_day/harib18a/asmhead.img)
set(asmhead_wine.img_OUTS ${root_BINARY_DIR}/projects/21_day/harib18a/asmhead_wine.img)

add_custom_target(21_day_harib18a_asmhead
  COMMAND ${RAKUSK_EXECUTABLE} ${asmhead.img_SRCS} ${asmhead.img_OUTS}
  DEPENDS ${asmhead.img_SRCS}
)
add_custom_target(21_day_harib18a_asmhead_wine
  COMMAND ${WINE} ${WINE_NASK} ${asmhead.img_SRCS} ${asmhead_wine.img_OUTS}
)
add_custom_target(21_day_harib18a_asmhead_od
  COMMAND ${OD} -t x1 ${asmhead.img_OUTS}      > ${asmhead.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${asmhead_wine.img_OUTS} > ${asmhead_wine.img_OUTS}_t.txt
  COMMAND diff -s ${asmhead.img_OUTS}_f.txt ${asmhead_wine.img_OUTS}_t.txt
)

add_dependencies(21_day_harib18a_asmhead_od 21_day_harib18a_asmhead)
add_dependencies(21_day_harib18a_asmhead_od 21_day_harib18a_asmhead_wine)

add_dependencies(images 21_day_harib18a_asmhead)
add_dependencies(wine 21_day_harib18a_asmhead_wine)
add_dependencies(od 21_day_harib18a_asmhead_od)

#----------------------------------------------------------
########### next target ###############
set(hello.img_SRCS ${root_SOURCE_DIR}/projects/21_day/harib18a/hello.nas)
set(hello.img_OUTS ${root_BINARY_DIR}/projects/21_day/harib18a/hello.img)
set(hello_wine.img_OUTS ${root_BINARY_DIR}/projects/21_day/harib18a/hello_wine.img)

add_custom_target(21_day_harib18a_hello
  COMMAND ${RAKUSK_EXECUTABLE} ${hello.img_SRCS} ${hello.img_OUTS}
  DEPENDS ${hello.img_SRCS}
)
set(hello.img_HRB ${root_BINARY_DIR}/projects/21_day/harib18a/hello.hrb)
add_custom_target(21_day_harib18a_hello_hrb
  COMMAND ${RAKUSK_EXECUTABLE} ${hello.img_SRCS} ${hello.img_HRB}
  DEPENDS ${hello.img_SRCS}
  COMMAND mcopy -i ${21_day_harib18a_OS} ${hello.img_HRB} ::hello.hrb
)

add_dependencies(21_day_harib18a_run 21_day_harib18a_hello_hrb)
add_custom_target(21_day_harib18a_hello_wine
  COMMAND ${WINE} ${WINE_NASK} ${hello.img_SRCS} ${hello_wine.img_OUTS}
)
add_custom_target(21_day_harib18a_hello_od
  COMMAND ${OD} -t x1 ${hello.img_OUTS}      > ${hello.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${hello_wine.img_OUTS} > ${hello_wine.img_OUTS}_t.txt
  COMMAND diff -s ${hello.img_OUTS}_f.txt ${hello_wine.img_OUTS}_t.txt
)

add_dependencies(21_day_harib18a_hello_od 21_day_harib18a_hello)
add_dependencies(21_day_harib18a_hello_od 21_day_harib18a_hello_wine)

add_dependencies(images 21_day_harib18a_hello)
add_dependencies(wine 21_day_harib18a_hello_wine)
add_dependencies(od 21_day_harib18a_hello_od)

#----------------------------------------------------------
########### next target ###############
set(hello2.img_SRCS ${root_SOURCE_DIR}/projects/21_day/harib18a/hello2.nas)
set(hello2.img_OUTS ${root_BINARY_DIR}/projects/21_day/harib18a/hello2.img)
set(hello2_wine.img_OUTS ${root_BINARY_DIR}/projects/21_day/harib18a/hello2_wine.img)

add_custom_target(21_day_harib18a_hello2
  COMMAND ${RAKUSK_EXECUTABLE} ${hello2.img_SRCS} ${hello2.img_OUTS}
  DEPENDS ${hello2.img_SRCS}
)
set(hello2.img_HRB ${root_BINARY_DIR}/projects/21_day/harib18a/hello2.hrb)
add_custom_target(21_day_harib18a_hello2_hrb
  COMMAND ${RAKUSK_EXECUTABLE} ${hello2.img_SRCS} ${hello2.img_HRB}
  DEPENDS ${hello2.img_SRCS}
  COMMAND mcopy -i ${21_day_harib18a_OS} ${hello2.img_HRB} ::hello2.hrb
)

add_dependencies(21_day_harib18a_run 21_day_harib18a_hello2_hrb)
add_custom_target(21_day_harib18a_hello2_wine
  COMMAND ${WINE} ${WINE_NASK} ${hello2.img_SRCS} ${hello2_wine.img_OUTS}
)
add_custom_target(21_day_harib18a_hello2_od
  COMMAND ${OD} -t x1 ${hello2.img_OUTS}      > ${hello2.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${hello2_wine.img_OUTS} > ${hello2_wine.img_OUTS}_t.txt
  COMMAND diff -s ${hello2.img_OUTS}_f.txt ${hello2_wine.img_OUTS}_t.txt
)

add_dependencies(21_day_harib18a_hello2_od 21_day_harib18a_hello2)
add_dependencies(21_day_harib18a_hello2_od 21_day_harib18a_hello2_wine)

add_dependencies(images 21_day_harib18a_hello2)
add_dependencies(wine 21_day_harib18a_hello2_wine)
add_dependencies(od 21_day_harib18a_hello2_od)

#----------------------------------------------------------
########### next target ###############
set(ipl10.img_SRCS ${root_SOURCE_DIR}/projects/21_day/harib18a/ipl10.nas)
set(ipl10.img_OUTS ${root_BINARY_DIR}/projects/21_day/harib18a/ipl10.img)
set(ipl10_wine.img_OUTS ${root_BINARY_DIR}/projects/21_day/harib18a/ipl10_wine.img)

add_custom_target(21_day_harib18a_ipl10
  COMMAND ${RAKUSK_EXECUTABLE} ${ipl10.img_SRCS} ${ipl10.img_OUTS}
  DEPENDS ${ipl10.img_SRCS}
)
add_custom_target(21_day_harib18a_ipl10_wine
  COMMAND ${WINE} ${WINE_NASK} ${ipl10.img_SRCS} ${ipl10_wine.img_OUTS}
)
add_custom_target(21_day_harib18a_ipl10_od
  COMMAND ${OD} -t x1 ${ipl10.img_OUTS}      > ${ipl10.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${ipl10_wine.img_OUTS} > ${ipl10_wine.img_OUTS}_t.txt
  COMMAND diff -s ${ipl10.img_OUTS}_f.txt ${ipl10_wine.img_OUTS}_t.txt
)

add_dependencies(21_day_harib18a_ipl10_od 21_day_harib18a_ipl10)
add_dependencies(21_day_harib18a_ipl10_od 21_day_harib18a_ipl10_wine)

add_dependencies(images 21_day_harib18a_ipl10)
add_dependencies(wine 21_day_harib18a_ipl10_wine)
add_dependencies(od 21_day_harib18a_ipl10_od)

#----------------------------------------------------------
########### next target ###############
set(naskfunc.img_SRCS ${root_SOURCE_DIR}/projects/21_day/harib18a/naskfunc.nas)
set(naskfunc.img_OUTS ${root_BINARY_DIR}/projects/21_day/harib18a/naskfunc.img)
set(naskfunc_wine.img_OUTS ${root_BINARY_DIR}/projects/21_day/harib18a/naskfunc_wine.img)

add_custom_target(21_day_harib18a_naskfunc
  COMMAND ${RAKUSK_EXECUTABLE} ${naskfunc.img_SRCS} ${naskfunc.img_OUTS}
  DEPENDS ${naskfunc.img_SRCS}
)
add_custom_target(21_day_harib18a_naskfunc_wine
  COMMAND ${WINE} ${WINE_NASK} ${naskfunc.img_SRCS} ${naskfunc_wine.img_OUTS}
)
add_custom_target(21_day_harib18a_naskfunc_od
  COMMAND ${OD} -t x1 ${naskfunc.img_OUTS}      > ${naskfunc.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${naskfunc_wine.img_OUTS} > ${naskfunc_wine.img_OUTS}_t.txt
  COMMAND diff -s ${naskfunc.img_OUTS}_f.txt ${naskfunc_wine.img_OUTS}_t.txt
)

add_dependencies(21_day_harib18a_naskfunc_od 21_day_harib18a_naskfunc)
add_dependencies(21_day_harib18a_naskfunc_od 21_day_harib18a_naskfunc_wine)

add_dependencies(images 21_day_harib18a_naskfunc)
add_dependencies(wine 21_day_harib18a_naskfunc_wine)
add_dependencies(od 21_day_harib18a_naskfunc_od)

#----------------------------------------------------------
