#----------------------------------------------------------
message(STATUS "Entering directory projects/22_day/harib19d/")

set(FONT ${root_BINARY_DIR}/src/makefont)
set(B2O  ${root_BINARY_DIR}/src/bin2obj)
set(CONV ${root_BINARY_DIR}/objconv/objconv)
set(22_day_harib19d_OS    ${root_BINARY_DIR}/projects/22_day/harib19d/os.img)
set(22_day_harib19d_SYS	  ${root_BINARY_DIR}/projects/22_day/harib19d/os.sys)
set(22_day_harib19d_IPLB  ${root_BINARY_DIR}/projects/22_day/harib19d/ipl.bin)
set(22_day_harib19d_IPLS  ${root_SOURCE_DIR}/projects/22_day/harib19d/ipl10.nas)
set(22_day_harib19d_HEADB ${root_BINARY_DIR}/projects/22_day/harib19d/asmhead.bin)
set(22_day_harib19d_HEADS ${root_SOURCE_DIR}/projects/22_day/harib19d/asmhead.nas)
set(22_day_harib19d_BOOTB ${root_BINARY_DIR}/projects/22_day/harib19d/boot.bin)
set(22_day_harib19d_CCO	  ${root_BINARY_DIR}/projects/22_day/harib19d/boot.o)
file(GLOB 22_day_harib19d_CCS ${root_SOURCE_DIR}/projects/22_day/harib19d/*.c)
list(REMOVE_ITEM 22_day_harib19d_CCS
  ${root_SOURCE_DIR}/projects/22_day/harib19d/a.c
  ${root_SOURCE_DIR}/projects/22_day/harib19d/hello3.c
  ${root_SOURCE_DIR}/projects/22_day/harib19d/bug1.c
  ${root_SOURCE_DIR}/projects/22_day/harib19d/bug2.c
  ${root_SOURCE_DIR}/projects/22_day/harib19d/bug3.c
  ${root_SOURCE_DIR}/projects/22_day/harib19d/hello4.c)
set(22_day_harib19d_LDS	  ${root_SOURCE_DIR}/projects/os.lds)
set(22_day_harib19d_APPLDS  ${root_SOURCE_DIR}/projects/app.lds)
set(22_day_harib19d_FUNCS ${root_SOURCE_DIR}/projects/22_day/harib19d/naskfunc.nas)
set(22_day_harib19d_FUNCO ${root_BINARY_DIR}/projects/22_day/harib19d/naskfunc.o)
set(22_day_harib19d_FONTS ${root_SOURCE_DIR}/projects/22_day/harib19d/hankaku.txt)
set(22_day_harib19d_FONTB ${root_BINARY_DIR}/projects/22_day/harib19d/hankaku.bin)
set(22_day_harib19d_FONTO ${root_BINARY_DIR}/projects/22_day/harib19d/hankaku.o)
set(22_day_harib19d_LIBGE ${root_BINARY_DIR}/golibc/libgo.a)
set(22_day_harib19d_LIBGC ${root_BINARY_DIR}/projects/22_day/harib19d/libgo.o)
set(22_day_harib19d_WILDOBJ ${root_BINARY_DIR}/projects/22_day/harib19d/*.o)


add_custom_target(22_day_harib19d_run
  COMMAND ${QEMU} ${QEMUOPT} ${22_day_harib19d_OS}
  DEPENDS 22_day_harib19d_img
)
add_custom_target(22_day_harib19d_debug
  COMMAND ${QEMU} -s -S ${QEMUOPT} ${22_day_harib19d_OS} ${QEMU_DEBUG_OPT}
  DEPENDS 22_day_harib19d_img
)
add_custom_target(22_day_harib19d_clean
  COMMAND rm -f ${22_day_harib19d_OS}
  COMMAND rm -f ${22_day_harib19d_SYS}
  COMMAND rm -f ${22_day_harib19d_IPLB}
  COMMAND rm -f ${22_day_harib19d_HEADB}
  COMMAND rm -f ${22_day_harib19d_BOOTB}
  COMMAND rm -f ${22_day_harib19d_WILDOBJ}
)
add_custom_target(22_day_harib19d_ipl
  COMMAND ${RAKUSK_EXECUTABLE} ${22_day_harib19d_IPLS} ${22_day_harib19d_IPLB}
  DEPENDS ${22_day_harib19d_IPLS}
)
add_custom_command(
  OUTPUT ${22_day_harib19d_HEADB}
  COMMAND ${RAKUSK_EXECUTABLE} ${22_day_harib19d_HEADS} ${22_day_harib19d_HEADB}
  DEPENDS ${22_day_harib19d_HEADS}
  COMMENT "Generating asmhead.bin for 22_day_harib19d"
)
add_custom_command(
  OUTPUT ${22_day_harib19d_FUNCO}
  COMMAND ${RAKUSK_EXECUTABLE} ${22_day_harib19d_FUNCS} ${22_day_harib19d_FUNCO}
  DEPENDS ${22_day_harib19d_FUNCS}
  COMMENT "Generating naskfunc.o for 22_day_harib19d"
)
add_custom_command(
  OUTPUT ${22_day_harib19d_FONTO}
  COMMAND ${FONT} ${22_day_harib19d_FONTS} ${22_day_harib19d_FONTB}
  COMMAND ${B2O}  ${22_day_harib19d_FONTB} ${22_day_harib19d_FONTO} _hankaku
  DEPENDS ${22_day_harib19d_FONTS} makefont bin2obj
  COMMENT "Generating hankaku.o for 22_day_harib19d"
)
add_custom_command(
  OUTPUT ${22_day_harib19d_LIBGC}
  COMMAND ${CONV} -fcoff32 -nu ${22_day_harib19d_LIBGE} ${22_day_harib19d_LIBGC}
  DEPENDS ${22_day_harib19d_LIBGE}
  COMMENT "Converting golibc for 22_day_harib19d"
)

add_custom_target(22_day_harib19d_sys
  COMMAND gcc ${BINOPT} -T ${22_day_harib19d_LDS} ${22_day_harib19d_CCS} ${22_day_harib19d_WILDOBJ} -o ${22_day_harib19d_BOOTB}
  COMMAND cat ${22_day_harib19d_HEADB} ${22_day_harib19d_BOOTB} > ${22_day_harib19d_SYS}
  DEPENDS ${22_day_harib19d_HEADB} 22_day_harib19d_ipl
  DEPENDS ${22_day_harib19d_FUNCO}
  DEPENDS ${22_day_harib19d_FONTO} ${22_day_harib19d_LIBGC}
)
add_custom_target(22_day_harib19d_img
  COMMAND mformat -f 1440 -l HARIBOTEOS -N 0xffffffff -C -B ${22_day_harib19d_IPLB} -i ${22_day_harib19d_OS}
  COMMAND mcopy -i ${22_day_harib19d_OS} ${22_day_harib19d_SYS} ::
  COMMAND mcopy -i ${22_day_harib19d_OS} ${root_BINARY_DIR}/projects/22_day/harib19d/a.hrb ::a.hrb
  COMMAND mcopy -i ${22_day_harib19d_OS} ${root_BINARY_DIR}/projects/22_day/harib19d/hello3.hrb ::hello3.hrb
  COMMAND mcopy -i ${22_day_harib19d_OS} ${root_BINARY_DIR}/projects/22_day/harib19d/bug1.hrb ::bug1.hrb
  COMMAND mcopy -i ${22_day_harib19d_OS} ${root_BINARY_DIR}/projects/22_day/harib19d/bug2.hrb ::bug2.hrb
  COMMAND mcopy -i ${22_day_harib19d_OS} ${root_BINARY_DIR}/projects/22_day/harib19d/bug3.hrb ::bug3.hrb
  COMMAND mcopy -i ${22_day_harib19d_OS} ${root_BINARY_DIR}/projects/22_day/harib19d/hello4.hrb ::hello4.hrb
  COMMAND mcopy -i ${22_day_harib19d_OS} ${root_BINARY_DIR}/projects/22_day/harib19d/hello.hrb ::hello.hrb
  COMMAND mcopy -i ${22_day_harib19d_OS} ${root_BINARY_DIR}/projects/22_day/harib19d/hello2.hrb ::hello2.hrb
  DEPENDS 22_day_harib19d_sys ${root_BINARY_DIR}/projects/22_day/harib19d/a.hrb ${root_BINARY_DIR}/projects/22_day/harib19d/hello3.hrb ${root_BINARY_DIR}/projects/22_day/harib19d/bug1.hrb ${root_BINARY_DIR}/projects/22_day/harib19d/bug2.hrb ${root_BINARY_DIR}/projects/22_day/harib19d/bug3.hrb ${root_BINARY_DIR}/projects/22_day/harib19d/hello4.hrb ${root_BINARY_DIR}/projects/22_day/harib19d/hello.hrb ${root_BINARY_DIR}/projects/22_day/harib19d/hello2.hrb
)

#----------------------------------------------------------
########### application .hrb files (copied into os.img) ###############
set(22_day_harib19d_APIO ${root_BINARY_DIR}/projects/22_day/harib19d/a_nask.o)
add_custom_command(
  OUTPUT ${22_day_harib19d_APIO}
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/22_day/harib19d/a_nask.nas ${22_day_harib19d_APIO}
  DEPENDS ${root_SOURCE_DIR}/projects/22_day/harib19d/a_nask.nas
  COMMENT "Generating a_nask.o for 22_day_harib19d"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/22_day/harib19d/a.hrb
  COMMAND gcc ${BINOPT} -T ${22_day_harib19d_APPLDS} ${root_SOURCE_DIR}/projects/22_day/harib19d/a.c ${22_day_harib19d_APIO} -o ${root_BINARY_DIR}/projects/22_day/harib19d/a.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/22_day/harib19d/a.c ${22_day_harib19d_APIO}
  COMMENT "Generating a.hrb for 22_day_harib19d"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/22_day/harib19d/hello3.hrb
  COMMAND gcc ${BINOPT} -T ${22_day_harib19d_APPLDS} ${root_SOURCE_DIR}/projects/22_day/harib19d/hello3.c ${22_day_harib19d_APIO} -o ${root_BINARY_DIR}/projects/22_day/harib19d/hello3.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/22_day/harib19d/hello3.c ${22_day_harib19d_APIO}
  COMMENT "Generating hello3.hrb for 22_day_harib19d"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/22_day/harib19d/bug1.hrb
  COMMAND gcc ${BINOPT} -T ${22_day_harib19d_APPLDS} ${root_SOURCE_DIR}/projects/22_day/harib19d/bug1.c ${22_day_harib19d_APIO} -o ${root_BINARY_DIR}/projects/22_day/harib19d/bug1.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/22_day/harib19d/bug1.c ${22_day_harib19d_APIO}
  COMMENT "Generating bug1.hrb for 22_day_harib19d"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/22_day/harib19d/bug2.hrb
  COMMAND gcc ${BINOPT} -T ${22_day_harib19d_APPLDS} ${root_SOURCE_DIR}/projects/22_day/harib19d/bug2.c ${22_day_harib19d_APIO} -o ${root_BINARY_DIR}/projects/22_day/harib19d/bug2.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/22_day/harib19d/bug2.c ${22_day_harib19d_APIO}
  COMMENT "Generating bug2.hrb for 22_day_harib19d"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/22_day/harib19d/bug3.hrb
  COMMAND gcc ${BINOPT} -T ${22_day_harib19d_APPLDS} ${root_SOURCE_DIR}/projects/22_day/harib19d/bug3.c ${22_day_harib19d_APIO} -o ${root_BINARY_DIR}/projects/22_day/harib19d/bug3.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/22_day/harib19d/bug3.c ${22_day_harib19d_APIO}
  COMMENT "Generating bug3.hrb for 22_day_harib19d"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/22_day/harib19d/hello4.hrb
  COMMAND gcc ${BINOPT} -T ${22_day_harib19d_APPLDS} ${root_SOURCE_DIR}/projects/22_day/harib19d/hello4.c ${22_day_harib19d_APIO} -o ${root_BINARY_DIR}/projects/22_day/harib19d/hello4.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/22_day/harib19d/hello4.c ${22_day_harib19d_APIO}
  COMMENT "Generating hello4.hrb for 22_day_harib19d"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/22_day/harib19d/hello.hrb
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/22_day/harib19d/hello.nas ${root_BINARY_DIR}/projects/22_day/harib19d/hello.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/22_day/harib19d/hello.nas
  COMMENT "Generating hello.hrb for 22_day_harib19d"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/22_day/harib19d/hello2.hrb
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/22_day/harib19d/hello2.nas ${root_BINARY_DIR}/projects/22_day/harib19d/hello2.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/22_day/harib19d/hello2.nas
  COMMENT "Generating hello2.hrb for 22_day_harib19d"
)
