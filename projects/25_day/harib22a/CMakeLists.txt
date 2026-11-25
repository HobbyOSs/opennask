#----------------------------------------------------------
message(STATUS "Entering directory projects/25_day/harib22a/")

set(FONT ${root_BINARY_DIR}/src/makefont)
set(B2O  ${root_BINARY_DIR}/src/bin2obj)
set(CONV ${root_BINARY_DIR}/objconv/objconv)
set(25_day_harib22a_OS    ${root_BINARY_DIR}/projects/25_day/harib22a/os.img)
set(25_day_harib22a_SYS	  ${root_BINARY_DIR}/projects/25_day/harib22a/os.sys)
set(25_day_harib22a_IPLB  ${root_BINARY_DIR}/projects/25_day/harib22a/ipl.bin)
set(25_day_harib22a_IPLS  ${root_SOURCE_DIR}/projects/25_day/harib22a/ipl10.nas)
set(25_day_harib22a_HEADB ${root_BINARY_DIR}/projects/25_day/harib22a/asmhead.bin)
set(25_day_harib22a_HEADS ${root_SOURCE_DIR}/projects/25_day/harib22a/asmhead.nas)
set(25_day_harib22a_BOOTB ${root_BINARY_DIR}/projects/25_day/harib22a/boot.bin)
set(25_day_harib22a_CCO	  ${root_BINARY_DIR}/projects/25_day/harib22a/boot.o)
file(GLOB 25_day_harib22a_CCS ${root_SOURCE_DIR}/projects/25_day/harib22a/*.c)
list(REMOVE_ITEM 25_day_harib22a_CCS
  ${root_SOURCE_DIR}/projects/25_day/harib22a/a.c
  ${root_SOURCE_DIR}/projects/25_day/harib22a/hello3.c
  ${root_SOURCE_DIR}/projects/25_day/harib22a/hello4.c
  ${root_SOURCE_DIR}/projects/25_day/harib22a/winhelo.c
  ${root_SOURCE_DIR}/projects/25_day/harib22a/winhelo2.c
  ${root_SOURCE_DIR}/projects/25_day/harib22a/winhelo3.c
  ${root_SOURCE_DIR}/projects/25_day/harib22a/star1.c
  ${root_SOURCE_DIR}/projects/25_day/harib22a/stars.c
  ${root_SOURCE_DIR}/projects/25_day/harib22a/stars2.c
  ${root_SOURCE_DIR}/projects/25_day/harib22a/lines.c
  ${root_SOURCE_DIR}/projects/25_day/harib22a/walk.c
  ${root_SOURCE_DIR}/projects/25_day/harib22a/noodle.c
  ${root_SOURCE_DIR}/projects/25_day/harib22a/beepdown.c)
set(25_day_harib22a_LDS	  ${root_SOURCE_DIR}/projects/os.lds)
set(25_day_harib22a_APPLDS  ${root_SOURCE_DIR}/projects/app.lds)
set(25_day_harib22a_FUNCS ${root_SOURCE_DIR}/projects/25_day/harib22a/naskfunc.nas)
set(25_day_harib22a_FUNCO ${root_BINARY_DIR}/projects/25_day/harib22a/naskfunc.o)
set(25_day_harib22a_FONTS ${root_SOURCE_DIR}/projects/25_day/harib22a/hankaku.txt)
set(25_day_harib22a_FONTB ${root_BINARY_DIR}/projects/25_day/harib22a/hankaku.bin)
set(25_day_harib22a_FONTO ${root_BINARY_DIR}/projects/25_day/harib22a/hankaku.o)
set(25_day_harib22a_LIBGE ${root_BINARY_DIR}/golibc/libgo.a)
set(25_day_harib22a_LIBGC ${root_BINARY_DIR}/projects/25_day/harib22a/libgo.o)
set(25_day_harib22a_WILDOBJ ${root_BINARY_DIR}/projects/25_day/harib22a/*.o)


add_custom_target(25_day_harib22a_run
  COMMAND ${QEMU} ${QEMUOPT} ${25_day_harib22a_OS}
  DEPENDS 25_day_harib22a_img
)
add_custom_target(25_day_harib22a_debug
  COMMAND ${QEMU} -s -S ${QEMUOPT} ${25_day_harib22a_OS} ${QEMU_DEBUG_OPT}
  DEPENDS 25_day_harib22a_img
)
add_custom_target(25_day_harib22a_clean
  COMMAND rm -f ${25_day_harib22a_OS}
  COMMAND rm -f ${25_day_harib22a_SYS}
  COMMAND rm -f ${25_day_harib22a_IPLB}
  COMMAND rm -f ${25_day_harib22a_HEADB}
  COMMAND rm -f ${25_day_harib22a_BOOTB}
  COMMAND rm -f ${25_day_harib22a_WILDOBJ}
)
add_custom_target(25_day_harib22a_ipl
  COMMAND ${RAKUSK_EXECUTABLE} ${25_day_harib22a_IPLS} ${25_day_harib22a_IPLB}
  DEPENDS ${25_day_harib22a_IPLS}
)
add_custom_command(
  OUTPUT ${25_day_harib22a_HEADB}
  COMMAND ${RAKUSK_EXECUTABLE} ${25_day_harib22a_HEADS} ${25_day_harib22a_HEADB}
  DEPENDS ${25_day_harib22a_HEADS}
  COMMENT "Generating asmhead.bin for 25_day_harib22a"
)
add_custom_command(
  OUTPUT ${25_day_harib22a_FUNCO}
  COMMAND ${RAKUSK_EXECUTABLE} ${25_day_harib22a_FUNCS} ${25_day_harib22a_FUNCO}
  DEPENDS ${25_day_harib22a_FUNCS}
  COMMENT "Generating naskfunc.o for 25_day_harib22a"
)
add_custom_command(
  OUTPUT ${25_day_harib22a_FONTO}
  COMMAND ${FONT} ${25_day_harib22a_FONTS} ${25_day_harib22a_FONTB}
  COMMAND ${B2O}  ${25_day_harib22a_FONTB} ${25_day_harib22a_FONTO} _hankaku
  DEPENDS ${25_day_harib22a_FONTS} makefont bin2obj
  COMMENT "Generating hankaku.o for 25_day_harib22a"
)
add_custom_command(
  OUTPUT ${25_day_harib22a_LIBGC}
  COMMAND ${CONV} -fcoff32 -nu ${25_day_harib22a_LIBGE} ${25_day_harib22a_LIBGC}
  DEPENDS ${25_day_harib22a_LIBGE}
  COMMENT "Converting golibc for 25_day_harib22a"
)

add_custom_target(25_day_harib22a_sys
  COMMAND gcc ${BINOPT} -T ${25_day_harib22a_LDS} ${25_day_harib22a_CCS} ${25_day_harib22a_WILDOBJ} -o ${25_day_harib22a_BOOTB}
  COMMAND cat ${25_day_harib22a_HEADB} ${25_day_harib22a_BOOTB} > ${25_day_harib22a_SYS}
  DEPENDS ${25_day_harib22a_HEADB} 25_day_harib22a_ipl
  DEPENDS ${25_day_harib22a_FUNCO}
  DEPENDS ${25_day_harib22a_FONTO} ${25_day_harib22a_LIBGC}
)
add_custom_target(25_day_harib22a_img
  COMMAND mformat -f 1440 -l HARIBOTEOS -N 0xffffffff -C -B ${25_day_harib22a_IPLB} -i ${25_day_harib22a_OS}
  COMMAND mcopy -i ${25_day_harib22a_OS} ${25_day_harib22a_SYS} ::
  COMMAND mcopy -i ${25_day_harib22a_OS} ${root_BINARY_DIR}/projects/25_day/harib22a/a.hrb ::a.hrb
  COMMAND mcopy -i ${25_day_harib22a_OS} ${root_BINARY_DIR}/projects/25_day/harib22a/hello3.hrb ::hello3.hrb
  COMMAND mcopy -i ${25_day_harib22a_OS} ${root_BINARY_DIR}/projects/25_day/harib22a/hello4.hrb ::hello4.hrb
  COMMAND mcopy -i ${25_day_harib22a_OS} ${root_BINARY_DIR}/projects/25_day/harib22a/winhelo.hrb ::winhelo.hrb
  COMMAND mcopy -i ${25_day_harib22a_OS} ${root_BINARY_DIR}/projects/25_day/harib22a/winhelo2.hrb ::winhelo2.hrb
  COMMAND mcopy -i ${25_day_harib22a_OS} ${root_BINARY_DIR}/projects/25_day/harib22a/winhelo3.hrb ::winhelo3.hrb
  COMMAND mcopy -i ${25_day_harib22a_OS} ${root_BINARY_DIR}/projects/25_day/harib22a/star1.hrb ::star1.hrb
  COMMAND mcopy -i ${25_day_harib22a_OS} ${root_BINARY_DIR}/projects/25_day/harib22a/stars.hrb ::stars.hrb
  COMMAND mcopy -i ${25_day_harib22a_OS} ${root_BINARY_DIR}/projects/25_day/harib22a/stars2.hrb ::stars2.hrb
  COMMAND mcopy -i ${25_day_harib22a_OS} ${root_BINARY_DIR}/projects/25_day/harib22a/lines.hrb ::lines.hrb
  COMMAND mcopy -i ${25_day_harib22a_OS} ${root_BINARY_DIR}/projects/25_day/harib22a/walk.hrb ::walk.hrb
  COMMAND mcopy -i ${25_day_harib22a_OS} ${root_BINARY_DIR}/projects/25_day/harib22a/beepdown.hrb ::beepdown.hrb
  COMMAND mcopy -i ${25_day_harib22a_OS} ${root_BINARY_DIR}/projects/25_day/harib22a/noodle.hrb ::noodle.hrb
  COMMAND mcopy -i ${25_day_harib22a_OS} ${root_BINARY_DIR}/projects/25_day/harib22a/hello.hrb ::hello.hrb
  COMMAND mcopy -i ${25_day_harib22a_OS} ${root_BINARY_DIR}/projects/25_day/harib22a/hello2.hrb ::hello2.hrb
  COMMAND mcopy -i ${25_day_harib22a_OS} ${root_BINARY_DIR}/projects/25_day/harib22a/hello5.hrb ::hello5.hrb
  DEPENDS 25_day_harib22a_sys ${root_BINARY_DIR}/projects/25_day/harib22a/a.hrb ${root_BINARY_DIR}/projects/25_day/harib22a/hello3.hrb ${root_BINARY_DIR}/projects/25_day/harib22a/hello4.hrb ${root_BINARY_DIR}/projects/25_day/harib22a/winhelo.hrb ${root_BINARY_DIR}/projects/25_day/harib22a/winhelo2.hrb ${root_BINARY_DIR}/projects/25_day/harib22a/winhelo3.hrb ${root_BINARY_DIR}/projects/25_day/harib22a/star1.hrb ${root_BINARY_DIR}/projects/25_day/harib22a/stars.hrb ${root_BINARY_DIR}/projects/25_day/harib22a/stars2.hrb ${root_BINARY_DIR}/projects/25_day/harib22a/lines.hrb ${root_BINARY_DIR}/projects/25_day/harib22a/walk.hrb ${root_BINARY_DIR}/projects/25_day/harib22a/noodle.hrb ${root_BINARY_DIR}/projects/25_day/harib22a/beepdown.hrb ${root_BINARY_DIR}/projects/25_day/harib22a/hello.hrb ${root_BINARY_DIR}/projects/25_day/harib22a/hello2.hrb ${root_BINARY_DIR}/projects/25_day/harib22a/hello5.hrb
)

#----------------------------------------------------------
########### application .hrb files (copied into os.img) ###############
set(25_day_harib22a_APIO ${root_BINARY_DIR}/projects/25_day/harib22a/a_nask.o)
set(25_day_harib22a_RANDO ${root_BINARY_DIR}/projects/25_day/harib22a/rand.o)
add_custom_command(
  OUTPUT ${25_day_harib22a_RANDO}
  COMMAND gcc -fleading-underscore -m32 -nostdlib -fno-pie -x c -c ${root_SOURCE_DIR}/golibc/rand.c_ -o ${25_day_harib22a_RANDO}
  DEPENDS ${root_SOURCE_DIR}/golibc/rand.c_
  COMMENT "Generating rand.o for 25_day_harib22a"
)
add_custom_command(
  OUTPUT ${25_day_harib22a_APIO}
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/25_day/harib22a/a_nask.nas ${25_day_harib22a_APIO}
  DEPENDS ${root_SOURCE_DIR}/projects/25_day/harib22a/a_nask.nas
  COMMENT "Generating a_nask.o for 25_day_harib22a"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/25_day/harib22a/a.hrb
  COMMAND gcc ${BINOPT} -T ${25_day_harib22a_APPLDS} ${root_SOURCE_DIR}/projects/25_day/harib22a/a.c ${25_day_harib22a_APIO} ${25_day_harib22a_LIBGC} ${25_day_harib22a_RANDO} -o ${root_BINARY_DIR}/projects/25_day/harib22a/a.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/25_day/harib22a/a.c ${root_SOURCE_DIR}/projects/app.lds ${25_day_harib22a_APIO} ${25_day_harib22a_LIBGC} ${25_day_harib22a_RANDO}
  COMMENT "Generating a.hrb for 25_day_harib22a"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/25_day/harib22a/hello3.hrb
  COMMAND gcc ${BINOPT} -T ${25_day_harib22a_APPLDS} ${root_SOURCE_DIR}/projects/25_day/harib22a/hello3.c ${25_day_harib22a_APIO} ${25_day_harib22a_LIBGC} ${25_day_harib22a_RANDO} -o ${root_BINARY_DIR}/projects/25_day/harib22a/hello3.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/25_day/harib22a/hello3.c ${root_SOURCE_DIR}/projects/app.lds ${25_day_harib22a_APIO} ${25_day_harib22a_LIBGC} ${25_day_harib22a_RANDO}
  COMMENT "Generating hello3.hrb for 25_day_harib22a"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/25_day/harib22a/hello4.hrb
  COMMAND gcc ${BINOPT} -T ${25_day_harib22a_APPLDS} ${root_SOURCE_DIR}/projects/25_day/harib22a/hello4.c ${25_day_harib22a_APIO} ${25_day_harib22a_LIBGC} ${25_day_harib22a_RANDO} -o ${root_BINARY_DIR}/projects/25_day/harib22a/hello4.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/25_day/harib22a/hello4.c ${root_SOURCE_DIR}/projects/app.lds ${25_day_harib22a_APIO} ${25_day_harib22a_LIBGC} ${25_day_harib22a_RANDO}
  COMMENT "Generating hello4.hrb for 25_day_harib22a"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/25_day/harib22a/winhelo.hrb
  COMMAND gcc ${BINOPT} -T ${25_day_harib22a_APPLDS} ${root_SOURCE_DIR}/projects/25_day/harib22a/winhelo.c ${25_day_harib22a_APIO} ${25_day_harib22a_LIBGC} ${25_day_harib22a_RANDO} -o ${root_BINARY_DIR}/projects/25_day/harib22a/winhelo.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/25_day/harib22a/winhelo.c ${root_SOURCE_DIR}/projects/app.lds ${25_day_harib22a_APIO} ${25_day_harib22a_LIBGC} ${25_day_harib22a_RANDO}
  COMMENT "Generating winhelo.hrb for 25_day_harib22a"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/25_day/harib22a/winhelo2.hrb
  COMMAND gcc ${BINOPT} -T ${25_day_harib22a_APPLDS} ${root_SOURCE_DIR}/projects/25_day/harib22a/winhelo2.c ${25_day_harib22a_APIO} ${25_day_harib22a_LIBGC} ${25_day_harib22a_RANDO} -o ${root_BINARY_DIR}/projects/25_day/harib22a/winhelo2.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/25_day/harib22a/winhelo2.c ${root_SOURCE_DIR}/projects/app.lds ${25_day_harib22a_APIO} ${25_day_harib22a_LIBGC} ${25_day_harib22a_RANDO}
  COMMENT "Generating winhelo2.hrb for 25_day_harib22a"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/25_day/harib22a/winhelo3.hrb
  COMMAND gcc ${BINOPT} -T ${25_day_harib22a_APPLDS} ${root_SOURCE_DIR}/projects/25_day/harib22a/winhelo3.c ${25_day_harib22a_APIO} ${25_day_harib22a_LIBGC} ${25_day_harib22a_RANDO} -o ${root_BINARY_DIR}/projects/25_day/harib22a/winhelo3.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/25_day/harib22a/winhelo3.c ${root_SOURCE_DIR}/projects/app.lds ${25_day_harib22a_APIO} ${25_day_harib22a_LIBGC} ${25_day_harib22a_RANDO}
  COMMENT "Generating winhelo3.hrb for 25_day_harib22a"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/25_day/harib22a/star1.hrb
  COMMAND gcc ${BINOPT} -T ${25_day_harib22a_APPLDS} ${root_SOURCE_DIR}/projects/25_day/harib22a/star1.c ${25_day_harib22a_APIO} ${25_day_harib22a_LIBGC} ${25_day_harib22a_RANDO} -o ${root_BINARY_DIR}/projects/25_day/harib22a/star1.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/25_day/harib22a/star1.c ${root_SOURCE_DIR}/projects/app.lds ${25_day_harib22a_APIO} ${25_day_harib22a_LIBGC} ${25_day_harib22a_RANDO}
  COMMENT "Generating star1.hrb for 25_day_harib22a"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/25_day/harib22a/stars.hrb
  COMMAND gcc ${BINOPT} -T ${25_day_harib22a_APPLDS} ${root_SOURCE_DIR}/projects/25_day/harib22a/stars.c ${25_day_harib22a_APIO} ${25_day_harib22a_LIBGC} ${25_day_harib22a_RANDO} -o ${root_BINARY_DIR}/projects/25_day/harib22a/stars.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/25_day/harib22a/stars.c ${root_SOURCE_DIR}/projects/app.lds ${25_day_harib22a_APIO} ${25_day_harib22a_LIBGC} ${25_day_harib22a_RANDO}
  COMMENT "Generating stars.hrb for 25_day_harib22a"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/25_day/harib22a/stars2.hrb
  COMMAND gcc ${BINOPT} -T ${25_day_harib22a_APPLDS} ${root_SOURCE_DIR}/projects/25_day/harib22a/stars2.c ${25_day_harib22a_APIO} ${25_day_harib22a_LIBGC} ${25_day_harib22a_RANDO} -o ${root_BINARY_DIR}/projects/25_day/harib22a/stars2.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/25_day/harib22a/stars2.c ${root_SOURCE_DIR}/projects/app.lds ${25_day_harib22a_APIO} ${25_day_harib22a_LIBGC} ${25_day_harib22a_RANDO}
  COMMENT "Generating stars2.hrb for 25_day_harib22a"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/25_day/harib22a/lines.hrb
  COMMAND gcc ${BINOPT} -T ${25_day_harib22a_APPLDS} ${root_SOURCE_DIR}/projects/25_day/harib22a/lines.c ${25_day_harib22a_APIO} ${25_day_harib22a_LIBGC} ${25_day_harib22a_RANDO} -o ${root_BINARY_DIR}/projects/25_day/harib22a/lines.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/25_day/harib22a/lines.c ${root_SOURCE_DIR}/projects/app.lds ${25_day_harib22a_APIO} ${25_day_harib22a_LIBGC} ${25_day_harib22a_RANDO}
  COMMENT "Generating lines.hrb for 25_day_harib22a"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/25_day/harib22a/walk.hrb
  COMMAND gcc ${BINOPT} -T ${25_day_harib22a_APPLDS} ${root_SOURCE_DIR}/projects/25_day/harib22a/walk.c ${25_day_harib22a_APIO} ${25_day_harib22a_LIBGC} ${25_day_harib22a_RANDO} -o ${root_BINARY_DIR}/projects/25_day/harib22a/walk.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/25_day/harib22a/walk.c ${root_SOURCE_DIR}/projects/app.lds ${25_day_harib22a_APIO} ${25_day_harib22a_LIBGC} ${25_day_harib22a_RANDO}
  COMMENT "Generating walk.hrb for 25_day_harib22a"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/25_day/harib22a/beepdown.hrb
  COMMAND gcc ${BINOPT} -T ${25_day_harib22a_APPLDS} ${root_SOURCE_DIR}/projects/25_day/harib22a/beepdown.c ${25_day_harib22a_APIO} ${25_day_harib22a_LIBGC} ${25_day_harib22a_RANDO} -o ${root_BINARY_DIR}/projects/25_day/harib22a/beepdown.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/25_day/harib22a/beepdown.c ${root_SOURCE_DIR}/projects/app.lds ${25_day_harib22a_APIO} ${25_day_harib22a_LIBGC} ${25_day_harib22a_RANDO}
  COMMENT "Generating beepdown.hrb for 25_day_harib22a"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/25_day/harib22a/noodle.hrb
  COMMAND gcc ${BINOPT} -T ${25_day_harib22a_APPLDS} ${root_SOURCE_DIR}/projects/25_day/harib22a/noodle.c ${25_day_harib22a_APIO} ${25_day_harib22a_LIBGC} ${25_day_harib22a_RANDO} -o ${root_BINARY_DIR}/projects/25_day/harib22a/noodle.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/25_day/harib22a/noodle.c ${root_SOURCE_DIR}/projects/app.lds ${25_day_harib22a_APIO} ${25_day_harib22a_LIBGC} ${25_day_harib22a_RANDO}
  COMMENT "Generating noodle.hrb for 25_day_harib22a"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/25_day/harib22a/hello.hrb
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/25_day/harib22a/hello.nas ${root_BINARY_DIR}/projects/25_day/harib22a/hello.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/25_day/harib22a/hello.nas
  COMMENT "Generating hello.hrb for 25_day_harib22a"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/25_day/harib22a/hello2.hrb
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/25_day/harib22a/hello2.nas ${root_BINARY_DIR}/projects/25_day/harib22a/hello2.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/25_day/harib22a/hello2.nas
  COMMENT "Generating hello2.hrb for 25_day_harib22a"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/25_day/harib22a/hello5.hrb
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/25_day/harib22a/hello5.nas ${root_BINARY_DIR}/projects/25_day/harib22a/hello5.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/25_day/harib22a/hello5.nas
  COMMENT "Generating hello5.hrb for 25_day_harib22a"
)
