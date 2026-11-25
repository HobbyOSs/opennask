#----------------------------------------------------------
message(STATUS "Entering directory projects/22_day/harib19e/")

set(FONT ${root_BINARY_DIR}/src/makefont)
set(B2O  ${root_BINARY_DIR}/src/bin2obj)
set(CONV ${root_BINARY_DIR}/objconv/objconv)
set(22_day_harib19e_OS    ${root_BINARY_DIR}/projects/22_day/harib19e/os.img)
set(22_day_harib19e_SYS	  ${root_BINARY_DIR}/projects/22_day/harib19e/os.sys)
set(22_day_harib19e_IPLB  ${root_BINARY_DIR}/projects/22_day/harib19e/ipl.bin)
set(22_day_harib19e_IPLS  ${root_SOURCE_DIR}/projects/22_day/harib19e/ipl10.nas)
set(22_day_harib19e_HEADB ${root_BINARY_DIR}/projects/22_day/harib19e/asmhead.bin)
set(22_day_harib19e_HEADS ${root_SOURCE_DIR}/projects/22_day/harib19e/asmhead.nas)
set(22_day_harib19e_BOOTB ${root_BINARY_DIR}/projects/22_day/harib19e/boot.bin)
set(22_day_harib19e_CCO	  ${root_BINARY_DIR}/projects/22_day/harib19e/boot.o)
file(GLOB 22_day_harib19e_CCS ${root_SOURCE_DIR}/projects/22_day/harib19e/*.c)
list(REMOVE_ITEM 22_day_harib19e_CCS
  ${root_SOURCE_DIR}/projects/22_day/harib19e/a.c
  ${root_SOURCE_DIR}/projects/22_day/harib19e/hello3.c
  ${root_SOURCE_DIR}/projects/22_day/harib19e/bug1.c
  ${root_SOURCE_DIR}/projects/22_day/harib19e/bug2.c
  ${root_SOURCE_DIR}/projects/22_day/harib19e/bug3.c
  ${root_SOURCE_DIR}/projects/22_day/harib19e/hello4.c)
set(22_day_harib19e_LDS	  ${root_SOURCE_DIR}/projects/os.lds)
set(22_day_harib19e_APPLDS  ${root_SOURCE_DIR}/projects/app.lds)
set(22_day_harib19e_FUNCS ${root_SOURCE_DIR}/projects/22_day/harib19e/naskfunc.nas)
set(22_day_harib19e_FUNCO ${root_BINARY_DIR}/projects/22_day/harib19e/naskfunc.o)
set(22_day_harib19e_FONTS ${root_SOURCE_DIR}/projects/22_day/harib19e/hankaku.txt)
set(22_day_harib19e_FONTB ${root_BINARY_DIR}/projects/22_day/harib19e/hankaku.bin)
set(22_day_harib19e_FONTO ${root_BINARY_DIR}/projects/22_day/harib19e/hankaku.o)
set(22_day_harib19e_LIBGE ${root_BINARY_DIR}/golibc/libgo.a)
set(22_day_harib19e_LIBGC ${root_BINARY_DIR}/projects/22_day/harib19e/libgo.o)
set(22_day_harib19e_WILDOBJ ${root_BINARY_DIR}/projects/22_day/harib19e/*.o)


add_custom_target(22_day_harib19e_run
  COMMAND ${QEMU} ${QEMUOPT} ${22_day_harib19e_OS}
  DEPENDS 22_day_harib19e_img
)
add_custom_target(22_day_harib19e_debug
  COMMAND ${QEMU} -s -S ${QEMUOPT} ${22_day_harib19e_OS} ${QEMU_DEBUG_OPT}
  DEPENDS 22_day_harib19e_img
)
add_custom_target(22_day_harib19e_clean
  COMMAND rm -f ${22_day_harib19e_OS}
  COMMAND rm -f ${22_day_harib19e_SYS}
  COMMAND rm -f ${22_day_harib19e_IPLB}
  COMMAND rm -f ${22_day_harib19e_HEADB}
  COMMAND rm -f ${22_day_harib19e_BOOTB}
  COMMAND rm -f ${22_day_harib19e_WILDOBJ}
)
add_custom_target(22_day_harib19e_ipl
  COMMAND ${RAKUSK_EXECUTABLE} ${22_day_harib19e_IPLS} ${22_day_harib19e_IPLB}
  DEPENDS ${22_day_harib19e_IPLS}
)
add_custom_command(
  OUTPUT ${22_day_harib19e_HEADB}
  COMMAND ${RAKUSK_EXECUTABLE} ${22_day_harib19e_HEADS} ${22_day_harib19e_HEADB}
  DEPENDS ${22_day_harib19e_HEADS}
  COMMENT "Generating asmhead.bin for 22_day_harib19e"
)
add_custom_command(
  OUTPUT ${22_day_harib19e_FUNCO}
  COMMAND ${RAKUSK_EXECUTABLE} ${22_day_harib19e_FUNCS} ${22_day_harib19e_FUNCO}
  DEPENDS ${22_day_harib19e_FUNCS}
  COMMENT "Generating naskfunc.o for 22_day_harib19e"
)
add_custom_command(
  OUTPUT ${22_day_harib19e_FONTO}
  COMMAND ${FONT} ${22_day_harib19e_FONTS} ${22_day_harib19e_FONTB}
  COMMAND ${B2O}  ${22_day_harib19e_FONTB} ${22_day_harib19e_FONTO} _hankaku
  DEPENDS ${22_day_harib19e_FONTS} makefont bin2obj
  COMMENT "Generating hankaku.o for 22_day_harib19e"
)
add_custom_command(
  OUTPUT ${22_day_harib19e_LIBGC}
  COMMAND ${CONV} -fcoff32 -nu ${22_day_harib19e_LIBGE} ${22_day_harib19e_LIBGC}
  DEPENDS ${22_day_harib19e_LIBGE}
  COMMENT "Converting golibc for 22_day_harib19e"
)

add_custom_target(22_day_harib19e_sys
  COMMAND gcc ${BINOPT} -T ${22_day_harib19e_LDS} ${22_day_harib19e_CCS} ${22_day_harib19e_WILDOBJ} -o ${22_day_harib19e_BOOTB}
  COMMAND cat ${22_day_harib19e_HEADB} ${22_day_harib19e_BOOTB} > ${22_day_harib19e_SYS}
  DEPENDS ${22_day_harib19e_HEADB} 22_day_harib19e_ipl
  DEPENDS ${22_day_harib19e_FUNCO}
  DEPENDS ${22_day_harib19e_FONTO} ${22_day_harib19e_LIBGC}
)
add_custom_target(22_day_harib19e_img
  COMMAND mformat -f 1440 -l HARIBOTEOS -N 0xffffffff -C -B ${22_day_harib19e_IPLB} -i ${22_day_harib19e_OS}
  COMMAND mcopy -i ${22_day_harib19e_OS} ${22_day_harib19e_SYS} ::
  COMMAND mcopy -i ${22_day_harib19e_OS} ${root_BINARY_DIR}/projects/22_day/harib19e/a.hrb ::a.hrb
  COMMAND mcopy -i ${22_day_harib19e_OS} ${root_BINARY_DIR}/projects/22_day/harib19e/hello3.hrb ::hello3.hrb
  COMMAND mcopy -i ${22_day_harib19e_OS} ${root_BINARY_DIR}/projects/22_day/harib19e/bug1.hrb ::bug1.hrb
  COMMAND mcopy -i ${22_day_harib19e_OS} ${root_BINARY_DIR}/projects/22_day/harib19e/bug2.hrb ::bug2.hrb
  COMMAND mcopy -i ${22_day_harib19e_OS} ${root_BINARY_DIR}/projects/22_day/harib19e/bug3.hrb ::bug3.hrb
  COMMAND mcopy -i ${22_day_harib19e_OS} ${root_BINARY_DIR}/projects/22_day/harib19e/hello4.hrb ::hello4.hrb
  COMMAND mcopy -i ${22_day_harib19e_OS} ${root_BINARY_DIR}/projects/22_day/harib19e/hello.hrb ::hello.hrb
  COMMAND mcopy -i ${22_day_harib19e_OS} ${root_BINARY_DIR}/projects/22_day/harib19e/hello2.hrb ::hello2.hrb
  COMMAND mcopy -i ${22_day_harib19e_OS} ${root_BINARY_DIR}/projects/22_day/harib19e/hello5.hrb ::hello5.hrb
  DEPENDS 22_day_harib19e_sys ${root_BINARY_DIR}/projects/22_day/harib19e/a.hrb ${root_BINARY_DIR}/projects/22_day/harib19e/hello3.hrb ${root_BINARY_DIR}/projects/22_day/harib19e/bug1.hrb ${root_BINARY_DIR}/projects/22_day/harib19e/bug2.hrb ${root_BINARY_DIR}/projects/22_day/harib19e/bug3.hrb ${root_BINARY_DIR}/projects/22_day/harib19e/hello4.hrb ${root_BINARY_DIR}/projects/22_day/harib19e/hello.hrb ${root_BINARY_DIR}/projects/22_day/harib19e/hello2.hrb ${root_BINARY_DIR}/projects/22_day/harib19e/hello5.hrb
)

#----------------------------------------------------------
########### application .hrb files (copied into os.img) ###############
set(22_day_harib19e_APIO ${root_BINARY_DIR}/projects/22_day/harib19e/a_nask.o)
add_custom_command(
  OUTPUT ${22_day_harib19e_APIO}
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/22_day/harib19e/a_nask.nas ${22_day_harib19e_APIO}
  DEPENDS ${root_SOURCE_DIR}/projects/22_day/harib19e/a_nask.nas
  COMMENT "Generating a_nask.o for 22_day_harib19e"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/22_day/harib19e/a.hrb
  COMMAND gcc ${BINOPT} -T ${22_day_harib19e_APPLDS} ${root_SOURCE_DIR}/projects/22_day/harib19e/a.c ${22_day_harib19e_APIO} -o ${root_BINARY_DIR}/projects/22_day/harib19e/a.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/22_day/harib19e/a.c ${22_day_harib19e_APIO}
  COMMENT "Generating a.hrb for 22_day_harib19e"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/22_day/harib19e/hello3.hrb
  COMMAND gcc ${BINOPT} -T ${22_day_harib19e_APPLDS} ${root_SOURCE_DIR}/projects/22_day/harib19e/hello3.c ${22_day_harib19e_APIO} -o ${root_BINARY_DIR}/projects/22_day/harib19e/hello3.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/22_day/harib19e/hello3.c ${22_day_harib19e_APIO}
  COMMENT "Generating hello3.hrb for 22_day_harib19e"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/22_day/harib19e/bug1.hrb
  COMMAND gcc ${BINOPT} -T ${22_day_harib19e_APPLDS} ${root_SOURCE_DIR}/projects/22_day/harib19e/bug1.c ${22_day_harib19e_APIO} -o ${root_BINARY_DIR}/projects/22_day/harib19e/bug1.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/22_day/harib19e/bug1.c ${22_day_harib19e_APIO}
  COMMENT "Generating bug1.hrb for 22_day_harib19e"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/22_day/harib19e/bug2.hrb
  COMMAND gcc ${BINOPT} -T ${22_day_harib19e_APPLDS} ${root_SOURCE_DIR}/projects/22_day/harib19e/bug2.c ${22_day_harib19e_APIO} -o ${root_BINARY_DIR}/projects/22_day/harib19e/bug2.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/22_day/harib19e/bug2.c ${22_day_harib19e_APIO}
  COMMENT "Generating bug2.hrb for 22_day_harib19e"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/22_day/harib19e/bug3.hrb
  COMMAND gcc ${BINOPT} -T ${22_day_harib19e_APPLDS} ${root_SOURCE_DIR}/projects/22_day/harib19e/bug3.c ${22_day_harib19e_APIO} -o ${root_BINARY_DIR}/projects/22_day/harib19e/bug3.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/22_day/harib19e/bug3.c ${22_day_harib19e_APIO}
  COMMENT "Generating bug3.hrb for 22_day_harib19e"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/22_day/harib19e/hello4.hrb
  COMMAND gcc ${BINOPT} -T ${22_day_harib19e_APPLDS} ${root_SOURCE_DIR}/projects/22_day/harib19e/hello4.c ${22_day_harib19e_APIO} -o ${root_BINARY_DIR}/projects/22_day/harib19e/hello4.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/22_day/harib19e/hello4.c ${22_day_harib19e_APIO}
  COMMENT "Generating hello4.hrb for 22_day_harib19e"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/22_day/harib19e/hello.hrb
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/22_day/harib19e/hello.nas ${root_BINARY_DIR}/projects/22_day/harib19e/hello.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/22_day/harib19e/hello.nas
  COMMENT "Generating hello.hrb for 22_day_harib19e"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/22_day/harib19e/hello2.hrb
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/22_day/harib19e/hello2.nas ${root_BINARY_DIR}/projects/22_day/harib19e/hello2.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/22_day/harib19e/hello2.nas
  COMMENT "Generating hello2.hrb for 22_day_harib19e"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/22_day/harib19e/hello5.hrb
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/22_day/harib19e/hello5.nas ${root_BINARY_DIR}/projects/22_day/harib19e/hello5.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/22_day/harib19e/hello5.nas
  COMMENT "Generating hello5.hrb for 22_day_harib19e"
)
