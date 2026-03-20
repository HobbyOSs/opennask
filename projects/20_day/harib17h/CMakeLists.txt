#----------------------------------------------------------
message(STATUS "Entering directory projects/20_day/harib17h/")

set(FONT ${root_BINARY_DIR}/src/makefont)
set(B2O  ${root_BINARY_DIR}/src/bin2obj)
set(CONV ${root_BINARY_DIR}/objconv/objconv)
set(20_day_harib17h_WILDOBJ ${root_BINARY_DIR}/projects/20_day/harib17h/*.o)

# Full OS build rules
set(20_day_harib17h_OS    ${root_BINARY_DIR}/projects/20_day/harib17h/os.img)
set(20_day_harib17h_SYS   ${root_BINARY_DIR}/projects/20_day/harib17h/os.sys)
set(20_day_harib17h_IPLB  ${root_BINARY_DIR}/projects/20_day/harib17h/ipl.bin)
set(20_day_harib17h_IPLS  ${root_SOURCE_DIR}/projects/20_day/harib17h/ipl10.nas)
set(20_day_harib17h_HEADB ${root_BINARY_DIR}/projects/20_day/harib17h/asmhead.bin)
set(20_day_harib17h_HEADS ${root_SOURCE_DIR}/projects/20_day/harib17h/asmhead.nas)
set(20_day_harib17h_BOOTB ${root_BINARY_DIR}/projects/20_day/harib17h/boot.bin)
set(20_day_harib17h_CCO   ${root_BINARY_DIR}/projects/20_day/harib17h/boot.o)
set(20_day_harib17h_CCS   ${root_SOURCE_DIR}/projects/20_day/harib17h/*.c)
set(20_day_harib17h_LDS   ${root_SOURCE_DIR}/projects/os.lds)
set(20_day_harib17h_FUNCS ${root_SOURCE_DIR}/projects/20_day/harib17h/naskfunc.nas)
set(20_day_harib17h_FUNCO ${root_BINARY_DIR}/projects/20_day/harib17h/naskfunc.o)
set(20_day_harib17h_FONTS ${root_SOURCE_DIR}/projects/20_day/harib17h/hankaku.txt)
set(20_day_harib17h_FONTB ${root_BINARY_DIR}/projects/20_day/harib17h/hankaku.bin)
set(20_day_harib17h_FONTO ${root_BINARY_DIR}/projects/20_day/harib17h/hankaku.o)
set(20_day_harib17h_LIBGE ${root_BINARY_DIR}/golibc/libgo.a)
set(20_day_harib17h_LIBGC ${root_BINARY_DIR}/projects/20_day/harib17h/libgo.o)

add_custom_target(20_day_harib17h_run
  COMMAND ${QEMU} ${QEMUOPT} ${20_day_harib17h_OS}
  DEPENDS 20_day_harib17h_img
)
add_custom_target(20_day_harib17h_debug
  COMMAND ${QEMU} -s -S ${QEMUOPT} ${20_day_harib17h_OS} ${QEMU_DEBUG_OPT}
  DEPENDS 20_day_harib17h_img
)
add_custom_target(20_day_harib17h_clean
  COMMAND rm -f ${20_day_harib17h_OS}
  COMMAND rm -f ${20_day_harib17h_SYS}
  COMMAND rm -f ${20_day_harib17h_IPLB}
  COMMAND rm -f ${20_day_harib17h_HEADB}
  COMMAND rm -f ${20_day_harib17h_BOOTB}
  COMMAND rm -f ${20_day_harib17h_WILDOBJ}
  COMMAND rm -f ${root_BINARY_DIR}/projects/20_day/harib17h/ipl10.bin
  COMMAND rm -f ${root_BINARY_DIR}/projects/20_day/harib17h/asmhead.bin
  COMMAND rm -f ${root_BINARY_DIR}/projects/20_day/harib17h/*_wine.bin
  COMMAND rm -f ${root_BINARY_DIR}/projects/20_day/harib17h/*.txt
)
add_custom_target(20_day_harib17h_ipl_build
  COMMAND ${RAKUSK_EXECUTABLE} ${20_day_harib17h_IPLS} ${20_day_harib17h_IPLB}
  DEPENDS ${20_day_harib17h_IPLS}
)
add_custom_command(
  OUTPUT ${20_day_harib17h_HEADB}
  COMMAND ${RAKUSK_EXECUTABLE} ${20_day_harib17h_HEADS} ${20_day_harib17h_HEADB}
  DEPENDS ${20_day_harib17h_HEADS}
  COMMENT "Generating asmhead.bin for 20_day_harib17h"
)

add_custom_command(
  OUTPUT ${20_day_harib17h_FUNCO}
  COMMAND ${RAKUSK_EXECUTABLE} ${20_day_harib17h_FUNCS} ${20_day_harib17h_FUNCO}
  DEPENDS ${20_day_harib17h_FUNCS}
  COMMENT "Generating naskfunc.o for 20_day_harib17h"
)
add_custom_command(
  OUTPUT ${20_day_harib17h_FONTO}
  COMMAND ${FONT} ${20_day_harib17h_FONTS} ${20_day_harib17h_FONTB}
  COMMAND ${B2O}  ${20_day_harib17h_FONTB} ${20_day_harib17h_FONTO} _hankaku
  DEPENDS ${20_day_harib17h_FONTS}
  COMMENT "Generating hankaku.o for 20_day_harib17h"
)
add_custom_command(
  OUTPUT ${20_day_harib17h_LIBGC}
  COMMAND ${CONV} -fcoff32 -nu ${20_day_harib17h_LIBGE} ${20_day_harib17h_LIBGC}
  DEPENDS ${20_day_harib17h_LIBGE}
  COMMENT "Converting golibc for 20_day_harib17h"
)
add_custom_target(20_day_harib17h_sys
  COMMAND gcc ${BINOPT} -T ${20_day_harib17h_LDS} ${20_day_harib17h_CCS} ${20_day_harib17h_WILDOBJ} -o ${20_day_harib17h_BOOTB}
  COMMAND cat ${20_day_harib17h_HEADB} ${20_day_harib17h_BOOTB} > ${20_day_harib17h_SYS}
  DEPENDS ${20_day_harib17h_HEADB} 20_day_harib17h_ipl_build
  DEPENDS ${20_day_harib17h_FUNCO}
  DEPENDS ${20_day_harib17h_FONTO} ${20_day_harib17h_LIBGC}
)
add_custom_target(20_day_harib17h_img
  COMMAND mformat -f 1440 -l HARIBOTEOS -N 0xffffffff -C -B ${20_day_harib17h_IPLB} -i ${20_day_harib17h_OS}
  COMMAND mcopy -i ${20_day_harib17h_OS} ${20_day_harib17h_SYS} ::
  DEPENDS 20_day_harib17h_sys
)

########### next target ###############
set(20_day_harib17h_asmhead_SRCS ${root_SOURCE_DIR}/projects/20_day/harib17h/asmhead.nas)
set(20_day_harib17h_asmhead_OUTS ${root_BINARY_DIR}/projects/20_day/harib17h/asmhead.bin)

add_custom_target(20_day_harib17h_asmhead
  COMMAND ${RAKUSK_EXECUTABLE} ${20_day_harib17h_asmhead_SRCS} ${20_day_harib17h_asmhead_OUTS}
  DEPENDS ${20_day_harib17h_asmhead_SRCS}
)

set(20_day_harib17h_asmhead_WINE_OUTS ${root_BINARY_DIR}/projects/20_day/harib17h/asmhead_wine.bin)
add_custom_target(20_day_harib17h_asmhead_wine
  COMMAND ${WINE} ${WINE_NASK} ${20_day_harib17h_asmhead_SRCS} ${20_day_harib17h_asmhead_WINE_OUTS}
)
add_dependencies(wine 20_day_harib17h_asmhead_wine)

add_custom_target(20_day_harib17h_asmhead_od
  COMMAND ${OD} -t x1 ${20_day_harib17h_asmhead_OUTS}      > ${20_day_harib17h_asmhead_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${20_day_harib17h_asmhead_WINE_OUTS} > ${20_day_harib17h_asmhead_WINE_OUTS}_t.txt
  COMMAND diff -s ${20_day_harib17h_asmhead_OUTS}_f.txt ${20_day_harib17h_asmhead_WINE_OUTS}_t.txt
)

add_dependencies(20_day_harib17h_asmhead_od 20_day_harib17h_asmhead)
add_dependencies(20_day_harib17h_asmhead_od 20_day_harib17h_asmhead_wine)
add_dependencies(od 20_day_harib17h_asmhead_od)

#----------------------------------------------------------
########### next target ###############
set(20_day_harib17h_hello_SRCS ${root_SOURCE_DIR}/projects/20_day/harib17h/hello.nas)
set(20_day_harib17h_hello_OUTS ${root_BINARY_DIR}/projects/20_day/harib17h/hello.img)

add_custom_target(20_day_harib17h_hello
  COMMAND ${RAKUSK_EXECUTABLE} ${20_day_harib17h_hello_SRCS} ${20_day_harib17h_hello_OUTS}
  DEPENDS ${20_day_harib17h_hello_SRCS}
)

add_dependencies(images 20_day_harib17h_hello)
set(20_day_harib17h_hello_WINE_OUTS ${root_BINARY_DIR}/projects/20_day/harib17h/hello_wine.img)
add_custom_target(20_day_harib17h_hello_wine
  COMMAND ${WINE} ${WINE_NASK} ${20_day_harib17h_hello_SRCS} ${20_day_harib17h_hello_WINE_OUTS}
)
add_dependencies(wine 20_day_harib17h_hello_wine)

add_custom_target(20_day_harib17h_hello_od
  COMMAND ${OD} -t x1 ${20_day_harib17h_hello_OUTS}      > ${20_day_harib17h_hello_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${20_day_harib17h_hello_WINE_OUTS} > ${20_day_harib17h_hello_WINE_OUTS}_t.txt
  COMMAND diff -s ${20_day_harib17h_hello_OUTS}_f.txt ${20_day_harib17h_hello_WINE_OUTS}_t.txt
)

add_dependencies(20_day_harib17h_hello_od 20_day_harib17h_hello)
add_dependencies(20_day_harib17h_hello_od 20_day_harib17h_hello_wine)
add_dependencies(od 20_day_harib17h_hello_od)

set(20_day_harib17h_hello_HRB ${root_BINARY_DIR}/projects/20_day/harib17h/hello.hrb)
add_custom_target(20_day_harib17h_hello_hrb
  COMMAND ${RAKUSK_EXECUTABLE} ${20_day_harib17h_hello_SRCS} ${20_day_harib17h_hello_HRB}
  COMMAND mcopy -i ${20_day_harib17h_OS} ${20_day_harib17h_hello_HRB} ::hello.hrb
  DEPENDS 20_day_harib17h_img
)

add_dependencies(20_day_harib17h_run 20_day_harib17h_hello_hrb)
#----------------------------------------------------------
########### next target ###############
set(20_day_harib17h_hello2_SRCS ${root_SOURCE_DIR}/projects/20_day/harib17h/hello2.nas)
set(20_day_harib17h_hello2_OUTS ${root_BINARY_DIR}/projects/20_day/harib17h/hello2.img)

add_custom_target(20_day_harib17h_hello2
  COMMAND ${RAKUSK_EXECUTABLE} ${20_day_harib17h_hello2_SRCS} ${20_day_harib17h_hello2_OUTS}
  DEPENDS ${20_day_harib17h_hello2_SRCS}
)

add_dependencies(images 20_day_harib17h_hello2)
set(20_day_harib17h_hello2_WINE_OUTS ${root_BINARY_DIR}/projects/20_day/harib17h/hello2_wine.img)
add_custom_target(20_day_harib17h_hello2_wine
  COMMAND ${WINE} ${WINE_NASK} ${20_day_harib17h_hello2_SRCS} ${20_day_harib17h_hello2_WINE_OUTS}
)
add_dependencies(wine 20_day_harib17h_hello2_wine)

add_custom_target(20_day_harib17h_hello2_od
  COMMAND ${OD} -t x1 ${20_day_harib17h_hello2_OUTS}      > ${20_day_harib17h_hello2_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${20_day_harib17h_hello2_WINE_OUTS} > ${20_day_harib17h_hello2_WINE_OUTS}_t.txt
  COMMAND diff -s ${20_day_harib17h_hello2_OUTS}_f.txt ${20_day_harib17h_hello2_WINE_OUTS}_t.txt
)

add_dependencies(20_day_harib17h_hello2_od 20_day_harib17h_hello2)
add_dependencies(20_day_harib17h_hello2_od 20_day_harib17h_hello2_wine)
add_dependencies(od 20_day_harib17h_hello2_od)

set(20_day_harib17h_hello2_HRB ${root_BINARY_DIR}/projects/20_day/harib17h/hello2.hrb)
add_custom_target(20_day_harib17h_hello2_hrb
  COMMAND ${RAKUSK_EXECUTABLE} ${20_day_harib17h_hello2_SRCS} ${20_day_harib17h_hello2_HRB}
  COMMAND mcopy -i ${20_day_harib17h_OS} ${20_day_harib17h_hello2_HRB} ::hello2.hrb
  DEPENDS 20_day_harib17h_img
)

add_dependencies(20_day_harib17h_run 20_day_harib17h_hello2_hrb)
#----------------------------------------------------------
########### next target ###############
set(20_day_harib17h_ipl10_SRCS ${root_SOURCE_DIR}/projects/20_day/harib17h/ipl10.nas)
set(20_day_harib17h_ipl10_OUTS ${root_BINARY_DIR}/projects/20_day/harib17h/ipl10.bin)

add_custom_target(20_day_harib17h_ipl10
  COMMAND ${RAKUSK_EXECUTABLE} ${20_day_harib17h_ipl10_SRCS} ${20_day_harib17h_ipl10_OUTS}
  DEPENDS ${20_day_harib17h_ipl10_SRCS}
)

set(20_day_harib17h_ipl10_WINE_OUTS ${root_BINARY_DIR}/projects/20_day/harib17h/ipl10_wine.bin)
add_custom_target(20_day_harib17h_ipl10_wine
  COMMAND ${WINE} ${WINE_NASK} ${20_day_harib17h_ipl10_SRCS} ${20_day_harib17h_ipl10_WINE_OUTS}
)
add_dependencies(wine 20_day_harib17h_ipl10_wine)

add_custom_target(20_day_harib17h_ipl10_od
  COMMAND ${OD} -t x1 ${20_day_harib17h_ipl10_OUTS}      > ${20_day_harib17h_ipl10_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${20_day_harib17h_ipl10_WINE_OUTS} > ${20_day_harib17h_ipl10_WINE_OUTS}_t.txt
  COMMAND diff -s ${20_day_harib17h_ipl10_OUTS}_f.txt ${20_day_harib17h_ipl10_WINE_OUTS}_t.txt
)

add_dependencies(20_day_harib17h_ipl10_od 20_day_harib17h_ipl10)
add_dependencies(20_day_harib17h_ipl10_od 20_day_harib17h_ipl10_wine)
add_dependencies(od 20_day_harib17h_ipl10_od)

#----------------------------------------------------------
########### next target ###############
set(20_day_harib17h_naskfunc_SRCS ${root_SOURCE_DIR}/projects/20_day/harib17h/naskfunc.nas)
set(20_day_harib17h_naskfunc_OUTS ${root_BINARY_DIR}/projects/20_day/harib17h/naskfunc.img)

add_custom_target(20_day_harib17h_naskfunc
  COMMAND ${RAKUSK_EXECUTABLE} ${20_day_harib17h_naskfunc_SRCS} ${20_day_harib17h_naskfunc_OUTS}
  DEPENDS ${20_day_harib17h_naskfunc_SRCS}
)

add_dependencies(images 20_day_harib17h_naskfunc)
set(20_day_harib17h_naskfunc_WINE_OUTS ${root_BINARY_DIR}/projects/20_day/harib17h/naskfunc_wine.img)
add_custom_target(20_day_harib17h_naskfunc_wine
  COMMAND ${WINE} ${WINE_NASK} ${20_day_harib17h_naskfunc_SRCS} ${20_day_harib17h_naskfunc_WINE_OUTS}
)
add_dependencies(wine 20_day_harib17h_naskfunc_wine)

add_custom_target(20_day_harib17h_naskfunc_od
  COMMAND ${OD} -t x1 ${20_day_harib17h_naskfunc_OUTS}      > ${20_day_harib17h_naskfunc_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${20_day_harib17h_naskfunc_WINE_OUTS} > ${20_day_harib17h_naskfunc_WINE_OUTS}_t.txt
  COMMAND diff -s ${20_day_harib17h_naskfunc_OUTS}_f.txt ${20_day_harib17h_naskfunc_WINE_OUTS}_t.txt
)

add_dependencies(20_day_harib17h_naskfunc_od 20_day_harib17h_naskfunc)
add_dependencies(20_day_harib17h_naskfunc_od 20_day_harib17h_naskfunc_wine)
add_dependencies(od 20_day_harib17h_naskfunc_od)

#----------------------------------------------------------
