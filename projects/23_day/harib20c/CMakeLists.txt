#----------------------------------------------------------
message(STATUS "Entering directory projects/23_day/harib20c/")

set(FONT ${root_BINARY_DIR}/src/makefont)
set(B2O  ${root_BINARY_DIR}/src/bin2obj)
set(CONV ${root_BINARY_DIR}/objconv/objconv)
set(23_day_harib20c_OS    ${root_BINARY_DIR}/projects/23_day/harib20c/os.img)
set(23_day_harib20c_SYS	  ${root_BINARY_DIR}/projects/23_day/harib20c/os.sys)
set(23_day_harib20c_IPLB  ${root_BINARY_DIR}/projects/23_day/harib20c/ipl.bin)
set(23_day_harib20c_IPLS  ${root_SOURCE_DIR}/projects/23_day/harib20c/ipl10.nas)
set(23_day_harib20c_HEADB ${root_BINARY_DIR}/projects/23_day/harib20c/asmhead.bin)
set(23_day_harib20c_HEADS ${root_SOURCE_DIR}/projects/23_day/harib20c/asmhead.nas)
set(23_day_harib20c_BOOTB ${root_BINARY_DIR}/projects/23_day/harib20c/boot.bin)
set(23_day_harib20c_CCO	  ${root_BINARY_DIR}/projects/23_day/harib20c/boot.o)
file(GLOB 23_day_harib20c_CCS ${root_SOURCE_DIR}/projects/23_day/harib20c/*.c)
list(REMOVE_ITEM 23_day_harib20c_CCS
  ${root_SOURCE_DIR}/projects/23_day/harib20c/a.c
  ${root_SOURCE_DIR}/projects/23_day/harib20c/hello3.c
  ${root_SOURCE_DIR}/projects/23_day/harib20c/hello4.c
  ${root_SOURCE_DIR}/projects/23_day/harib20c/winhelo.c
  ${root_SOURCE_DIR}/projects/23_day/harib20c/winhelo2.c
  ${root_SOURCE_DIR}/projects/23_day/harib20c/winhelo3.c
  ${root_SOURCE_DIR}/projects/23_day/harib20c/star1.c
  ${root_SOURCE_DIR}/projects/23_day/harib20c/stars.c
  ${root_SOURCE_DIR}/projects/23_day/harib20c/stars2.c)
set(23_day_harib20c_LDS	  ${root_SOURCE_DIR}/projects/os.lds)
set(23_day_harib20c_APPLDS  ${root_SOURCE_DIR}/projects/app.lds)
set(23_day_harib20c_FUNCS ${root_SOURCE_DIR}/projects/23_day/harib20c/naskfunc.nas)
set(23_day_harib20c_FUNCO ${root_BINARY_DIR}/projects/23_day/harib20c/naskfunc.o)
set(23_day_harib20c_FONTS ${root_SOURCE_DIR}/projects/23_day/harib20c/hankaku.txt)
set(23_day_harib20c_FONTB ${root_BINARY_DIR}/projects/23_day/harib20c/hankaku.bin)
set(23_day_harib20c_FONTO ${root_BINARY_DIR}/projects/23_day/harib20c/hankaku.o)
set(23_day_harib20c_LIBGE ${root_BINARY_DIR}/golibc/libgo.a)
set(23_day_harib20c_LIBGC ${root_BINARY_DIR}/projects/23_day/harib20c/libgo.o)
set(23_day_harib20c_WILDOBJ ${root_BINARY_DIR}/projects/23_day/harib20c/*.o)


add_custom_target(23_day_harib20c_run
  COMMAND ${QEMU} ${QEMUOPT} ${23_day_harib20c_OS}
  DEPENDS 23_day_harib20c_img
)
add_custom_target(23_day_harib20c_debug
  COMMAND ${QEMU} -s -S ${QEMUOPT} ${23_day_harib20c_OS} ${QEMU_DEBUG_OPT}
  DEPENDS 23_day_harib20c_img
)
add_custom_target(23_day_harib20c_clean
  COMMAND rm -f ${23_day_harib20c_OS}
  COMMAND rm -f ${23_day_harib20c_SYS}
  COMMAND rm -f ${23_day_harib20c_IPLB}
  COMMAND rm -f ${23_day_harib20c_HEADB}
  COMMAND rm -f ${23_day_harib20c_BOOTB}
  COMMAND rm -f ${23_day_harib20c_WILDOBJ}
)
add_custom_target(23_day_harib20c_ipl
  COMMAND ${RAKUSK_EXECUTABLE} ${23_day_harib20c_IPLS} ${23_day_harib20c_IPLB}
  DEPENDS ${23_day_harib20c_IPLS}
)
add_custom_command(
  OUTPUT ${23_day_harib20c_HEADB}
  COMMAND ${RAKUSK_EXECUTABLE} ${23_day_harib20c_HEADS} ${23_day_harib20c_HEADB}
  DEPENDS ${23_day_harib20c_HEADS}
  COMMENT "Generating asmhead.bin for 23_day_harib20c"
)
add_custom_command(
  OUTPUT ${23_day_harib20c_FUNCO}
  COMMAND ${RAKUSK_EXECUTABLE} ${23_day_harib20c_FUNCS} ${23_day_harib20c_FUNCO}
  DEPENDS ${23_day_harib20c_FUNCS}
  COMMENT "Generating naskfunc.o for 23_day_harib20c"
)
add_custom_command(
  OUTPUT ${23_day_harib20c_FONTO}
  COMMAND ${FONT} ${23_day_harib20c_FONTS} ${23_day_harib20c_FONTB}
  COMMAND ${B2O}  ${23_day_harib20c_FONTB} ${23_day_harib20c_FONTO} _hankaku
  DEPENDS ${23_day_harib20c_FONTS} makefont bin2obj
  COMMENT "Generating hankaku.o for 23_day_harib20c"
)
add_custom_command(
  OUTPUT ${23_day_harib20c_LIBGC}
  COMMAND ${CONV} -fcoff32 -nu ${23_day_harib20c_LIBGE} ${23_day_harib20c_LIBGC}
  DEPENDS ${23_day_harib20c_LIBGE}
  COMMENT "Converting golibc for 23_day_harib20c"
)

add_custom_target(23_day_harib20c_sys
  COMMAND gcc ${BINOPT} -T ${23_day_harib20c_LDS} ${23_day_harib20c_CCS} ${23_day_harib20c_WILDOBJ} -o ${23_day_harib20c_BOOTB}
  COMMAND cat ${23_day_harib20c_HEADB} ${23_day_harib20c_BOOTB} > ${23_day_harib20c_SYS}
  DEPENDS ${23_day_harib20c_HEADB} 23_day_harib20c_ipl
  DEPENDS ${23_day_harib20c_FUNCO}
  DEPENDS ${23_day_harib20c_FONTO} ${23_day_harib20c_LIBGC}
)
add_custom_target(23_day_harib20c_img
  COMMAND mformat -f 1440 -l HARIBOTEOS -N 0xffffffff -C -B ${23_day_harib20c_IPLB} -i ${23_day_harib20c_OS}
  COMMAND mcopy -i ${23_day_harib20c_OS} ${23_day_harib20c_SYS} ::
  COMMAND mcopy -i ${23_day_harib20c_OS} ${root_BINARY_DIR}/projects/23_day/harib20c/a.hrb ::a.hrb
  COMMAND mcopy -i ${23_day_harib20c_OS} ${root_BINARY_DIR}/projects/23_day/harib20c/hello3.hrb ::hello3.hrb
  COMMAND mcopy -i ${23_day_harib20c_OS} ${root_BINARY_DIR}/projects/23_day/harib20c/hello4.hrb ::hello4.hrb
  COMMAND mcopy -i ${23_day_harib20c_OS} ${root_BINARY_DIR}/projects/23_day/harib20c/winhelo.hrb ::winhelo.hrb
  COMMAND mcopy -i ${23_day_harib20c_OS} ${root_BINARY_DIR}/projects/23_day/harib20c/winhelo2.hrb ::winhelo2.hrb
  COMMAND mcopy -i ${23_day_harib20c_OS} ${root_BINARY_DIR}/projects/23_day/harib20c/winhelo3.hrb ::winhelo3.hrb
  COMMAND mcopy -i ${23_day_harib20c_OS} ${root_BINARY_DIR}/projects/23_day/harib20c/star1.hrb ::star1.hrb
  COMMAND mcopy -i ${23_day_harib20c_OS} ${root_BINARY_DIR}/projects/23_day/harib20c/stars.hrb ::stars.hrb
  COMMAND mcopy -i ${23_day_harib20c_OS} ${root_BINARY_DIR}/projects/23_day/harib20c/stars2.hrb ::stars2.hrb
  COMMAND mcopy -i ${23_day_harib20c_OS} ${root_BINARY_DIR}/projects/23_day/harib20c/hello.hrb ::hello.hrb
  COMMAND mcopy -i ${23_day_harib20c_OS} ${root_BINARY_DIR}/projects/23_day/harib20c/hello2.hrb ::hello2.hrb
  COMMAND mcopy -i ${23_day_harib20c_OS} ${root_BINARY_DIR}/projects/23_day/harib20c/hello5.hrb ::hello5.hrb
  DEPENDS 23_day_harib20c_sys ${root_BINARY_DIR}/projects/23_day/harib20c/a.hrb ${root_BINARY_DIR}/projects/23_day/harib20c/hello3.hrb ${root_BINARY_DIR}/projects/23_day/harib20c/hello4.hrb ${root_BINARY_DIR}/projects/23_day/harib20c/winhelo.hrb ${root_BINARY_DIR}/projects/23_day/harib20c/winhelo2.hrb ${root_BINARY_DIR}/projects/23_day/harib20c/winhelo3.hrb ${root_BINARY_DIR}/projects/23_day/harib20c/star1.hrb ${root_BINARY_DIR}/projects/23_day/harib20c/stars.hrb ${root_BINARY_DIR}/projects/23_day/harib20c/stars2.hrb ${root_BINARY_DIR}/projects/23_day/harib20c/hello.hrb ${root_BINARY_DIR}/projects/23_day/harib20c/hello2.hrb ${root_BINARY_DIR}/projects/23_day/harib20c/hello5.hrb
)

#----------------------------------------------------------
########### application .hrb files (copied into os.img) ###############
set(23_day_harib20c_APIO ${root_BINARY_DIR}/projects/23_day/harib20c/a_nask.o)
set(23_day_harib20c_RANDO ${root_BINARY_DIR}/projects/23_day/harib20c/rand.o)
add_custom_command(
  OUTPUT ${23_day_harib20c_RANDO}
  COMMAND gcc -fleading-underscore -m32 -nostdlib -fno-pie -x c -c ${root_SOURCE_DIR}/golibc/rand.c_ -o ${23_day_harib20c_RANDO}
  DEPENDS ${root_SOURCE_DIR}/golibc/rand.c_
  COMMENT "Generating rand.o for 23_day_harib20c"
)
add_custom_command(
  OUTPUT ${23_day_harib20c_APIO}
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/23_day/harib20c/a_nask.nas ${23_day_harib20c_APIO}
  DEPENDS ${root_SOURCE_DIR}/projects/23_day/harib20c/a_nask.nas
  COMMENT "Generating a_nask.o for 23_day_harib20c"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/23_day/harib20c/a.hrb
  COMMAND gcc ${BINOPT} -T ${23_day_harib20c_APPLDS} ${root_SOURCE_DIR}/projects/23_day/harib20c/a.c ${23_day_harib20c_APIO} ${23_day_harib20c_LIBGC} ${23_day_harib20c_RANDO} -o ${root_BINARY_DIR}/projects/23_day/harib20c/a.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/23_day/harib20c/a.c ${root_SOURCE_DIR}/projects/app.lds ${23_day_harib20c_APIO} ${23_day_harib20c_LIBGC} ${23_day_harib20c_RANDO}
  COMMENT "Generating a.hrb for 23_day_harib20c"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/23_day/harib20c/hello3.hrb
  COMMAND gcc ${BINOPT} -T ${23_day_harib20c_APPLDS} ${root_SOURCE_DIR}/projects/23_day/harib20c/hello3.c ${23_day_harib20c_APIO} ${23_day_harib20c_LIBGC} ${23_day_harib20c_RANDO} -o ${root_BINARY_DIR}/projects/23_day/harib20c/hello3.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/23_day/harib20c/hello3.c ${root_SOURCE_DIR}/projects/app.lds ${23_day_harib20c_APIO} ${23_day_harib20c_LIBGC} ${23_day_harib20c_RANDO}
  COMMENT "Generating hello3.hrb for 23_day_harib20c"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/23_day/harib20c/hello4.hrb
  COMMAND gcc ${BINOPT} -T ${23_day_harib20c_APPLDS} ${root_SOURCE_DIR}/projects/23_day/harib20c/hello4.c ${23_day_harib20c_APIO} ${23_day_harib20c_LIBGC} ${23_day_harib20c_RANDO} -o ${root_BINARY_DIR}/projects/23_day/harib20c/hello4.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/23_day/harib20c/hello4.c ${root_SOURCE_DIR}/projects/app.lds ${23_day_harib20c_APIO} ${23_day_harib20c_LIBGC} ${23_day_harib20c_RANDO}
  COMMENT "Generating hello4.hrb for 23_day_harib20c"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/23_day/harib20c/winhelo.hrb
  COMMAND gcc ${BINOPT} -T ${23_day_harib20c_APPLDS} ${root_SOURCE_DIR}/projects/23_day/harib20c/winhelo.c ${23_day_harib20c_APIO} ${23_day_harib20c_LIBGC} ${23_day_harib20c_RANDO} -o ${root_BINARY_DIR}/projects/23_day/harib20c/winhelo.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/23_day/harib20c/winhelo.c ${root_SOURCE_DIR}/projects/app.lds ${23_day_harib20c_APIO} ${23_day_harib20c_LIBGC} ${23_day_harib20c_RANDO}
  COMMENT "Generating winhelo.hrb for 23_day_harib20c"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/23_day/harib20c/winhelo2.hrb
  COMMAND gcc ${BINOPT} -T ${23_day_harib20c_APPLDS} ${root_SOURCE_DIR}/projects/23_day/harib20c/winhelo2.c ${23_day_harib20c_APIO} ${23_day_harib20c_LIBGC} ${23_day_harib20c_RANDO} -o ${root_BINARY_DIR}/projects/23_day/harib20c/winhelo2.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/23_day/harib20c/winhelo2.c ${root_SOURCE_DIR}/projects/app.lds ${23_day_harib20c_APIO} ${23_day_harib20c_LIBGC} ${23_day_harib20c_RANDO}
  COMMENT "Generating winhelo2.hrb for 23_day_harib20c"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/23_day/harib20c/winhelo3.hrb
  COMMAND gcc ${BINOPT} -T ${23_day_harib20c_APPLDS} ${root_SOURCE_DIR}/projects/23_day/harib20c/winhelo3.c ${23_day_harib20c_APIO} ${23_day_harib20c_LIBGC} ${23_day_harib20c_RANDO} -o ${root_BINARY_DIR}/projects/23_day/harib20c/winhelo3.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/23_day/harib20c/winhelo3.c ${root_SOURCE_DIR}/projects/app.lds ${23_day_harib20c_APIO} ${23_day_harib20c_LIBGC} ${23_day_harib20c_RANDO}
  COMMENT "Generating winhelo3.hrb for 23_day_harib20c"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/23_day/harib20c/star1.hrb
  COMMAND gcc ${BINOPT} -T ${23_day_harib20c_APPLDS} ${root_SOURCE_DIR}/projects/23_day/harib20c/star1.c ${23_day_harib20c_APIO} ${23_day_harib20c_LIBGC} ${23_day_harib20c_RANDO} -o ${root_BINARY_DIR}/projects/23_day/harib20c/star1.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/23_day/harib20c/star1.c ${root_SOURCE_DIR}/projects/app.lds ${23_day_harib20c_APIO} ${23_day_harib20c_LIBGC} ${23_day_harib20c_RANDO}
  COMMENT "Generating star1.hrb for 23_day_harib20c"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/23_day/harib20c/stars.hrb
  COMMAND gcc ${BINOPT} -T ${23_day_harib20c_APPLDS} ${root_SOURCE_DIR}/projects/23_day/harib20c/stars.c ${23_day_harib20c_APIO} ${23_day_harib20c_LIBGC} ${23_day_harib20c_RANDO} -o ${root_BINARY_DIR}/projects/23_day/harib20c/stars.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/23_day/harib20c/stars.c ${root_SOURCE_DIR}/projects/app.lds ${23_day_harib20c_APIO} ${23_day_harib20c_LIBGC} ${23_day_harib20c_RANDO}
  COMMENT "Generating stars.hrb for 23_day_harib20c"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/23_day/harib20c/stars2.hrb
  COMMAND gcc ${BINOPT} -T ${23_day_harib20c_APPLDS} ${root_SOURCE_DIR}/projects/23_day/harib20c/stars2.c ${23_day_harib20c_APIO} ${23_day_harib20c_LIBGC} ${23_day_harib20c_RANDO} -o ${root_BINARY_DIR}/projects/23_day/harib20c/stars2.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/23_day/harib20c/stars2.c ${root_SOURCE_DIR}/projects/app.lds ${23_day_harib20c_APIO} ${23_day_harib20c_LIBGC} ${23_day_harib20c_RANDO}
  COMMENT "Generating stars2.hrb for 23_day_harib20c"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/23_day/harib20c/hello.hrb
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/23_day/harib20c/hello.nas ${root_BINARY_DIR}/projects/23_day/harib20c/hello.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/23_day/harib20c/hello.nas
  COMMENT "Generating hello.hrb for 23_day_harib20c"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/23_day/harib20c/hello2.hrb
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/23_day/harib20c/hello2.nas ${root_BINARY_DIR}/projects/23_day/harib20c/hello2.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/23_day/harib20c/hello2.nas
  COMMENT "Generating hello2.hrb for 23_day_harib20c"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/23_day/harib20c/hello5.hrb
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/23_day/harib20c/hello5.nas ${root_BINARY_DIR}/projects/23_day/harib20c/hello5.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/23_day/harib20c/hello5.nas
  COMMENT "Generating hello5.hrb for 23_day_harib20c"
)
