#----------------------------------------------------------
message(STATUS "Entering directory projects/25_day/harib22b/")

set(FONT ${root_BINARY_DIR}/src/makefont)
set(B2O  ${root_BINARY_DIR}/src/bin2obj)
set(CONV ${root_BINARY_DIR}/objconv/objconv)
set(25_day_harib22b_OS    ${root_BINARY_DIR}/projects/25_day/harib22b/os.img)
set(25_day_harib22b_SYS	  ${root_BINARY_DIR}/projects/25_day/harib22b/os.sys)
set(25_day_harib22b_IPLB  ${root_BINARY_DIR}/projects/25_day/harib22b/ipl.bin)
set(25_day_harib22b_IPLS  ${root_SOURCE_DIR}/projects/25_day/harib22b/ipl10.nas)
set(25_day_harib22b_HEADB ${root_BINARY_DIR}/projects/25_day/harib22b/asmhead.bin)
set(25_day_harib22b_HEADS ${root_SOURCE_DIR}/projects/25_day/harib22b/asmhead.nas)
set(25_day_harib22b_BOOTB ${root_BINARY_DIR}/projects/25_day/harib22b/boot.bin)
set(25_day_harib22b_CCO	  ${root_BINARY_DIR}/projects/25_day/harib22b/boot.o)
file(GLOB 25_day_harib22b_CCS ${root_SOURCE_DIR}/projects/25_day/harib22b/*.c)
list(REMOVE_ITEM 25_day_harib22b_CCS
  ${root_SOURCE_DIR}/projects/25_day/harib22b/a.c
  ${root_SOURCE_DIR}/projects/25_day/harib22b/hello3.c
  ${root_SOURCE_DIR}/projects/25_day/harib22b/hello4.c
  ${root_SOURCE_DIR}/projects/25_day/harib22b/winhelo.c
  ${root_SOURCE_DIR}/projects/25_day/harib22b/winhelo2.c
  ${root_SOURCE_DIR}/projects/25_day/harib22b/winhelo3.c
  ${root_SOURCE_DIR}/projects/25_day/harib22b/star1.c
  ${root_SOURCE_DIR}/projects/25_day/harib22b/stars.c
  ${root_SOURCE_DIR}/projects/25_day/harib22b/stars2.c
  ${root_SOURCE_DIR}/projects/25_day/harib22b/lines.c
  ${root_SOURCE_DIR}/projects/25_day/harib22b/walk.c
  ${root_SOURCE_DIR}/projects/25_day/harib22b/noodle.c
  ${root_SOURCE_DIR}/projects/25_day/harib22b/beepdown.c
  ${root_SOURCE_DIR}/projects/25_day/harib22b/color.c)
set(25_day_harib22b_LDS	  ${root_SOURCE_DIR}/projects/os.lds)
set(25_day_harib22b_APPLDS  ${root_SOURCE_DIR}/projects/app.lds)
set(25_day_harib22b_FUNCS ${root_SOURCE_DIR}/projects/25_day/harib22b/naskfunc.nas)
set(25_day_harib22b_FUNCO ${root_BINARY_DIR}/projects/25_day/harib22b/naskfunc.o)
set(25_day_harib22b_FONTS ${root_SOURCE_DIR}/projects/25_day/harib22b/hankaku.txt)
set(25_day_harib22b_FONTB ${root_BINARY_DIR}/projects/25_day/harib22b/hankaku.bin)
set(25_day_harib22b_FONTO ${root_BINARY_DIR}/projects/25_day/harib22b/hankaku.o)
set(25_day_harib22b_LIBGE ${root_BINARY_DIR}/golibc/libgo.a)
set(25_day_harib22b_LIBGC ${root_BINARY_DIR}/projects/25_day/harib22b/libgo.o)
set(25_day_harib22b_WILDOBJ ${root_BINARY_DIR}/projects/25_day/harib22b/*.o)


add_custom_target(25_day_harib22b_run
  COMMAND ${QEMU} ${QEMUOPT} ${25_day_harib22b_OS}
  DEPENDS 25_day_harib22b_img
)
add_custom_target(25_day_harib22b_debug
  COMMAND ${QEMU} -s -S ${QEMUOPT} ${25_day_harib22b_OS} ${QEMU_DEBUG_OPT}
  DEPENDS 25_day_harib22b_img
)
add_custom_target(25_day_harib22b_clean
  COMMAND rm -f ${25_day_harib22b_OS}
  COMMAND rm -f ${25_day_harib22b_SYS}
  COMMAND rm -f ${25_day_harib22b_IPLB}
  COMMAND rm -f ${25_day_harib22b_HEADB}
  COMMAND rm -f ${25_day_harib22b_BOOTB}
  COMMAND rm -f ${25_day_harib22b_WILDOBJ}
)
add_custom_target(25_day_harib22b_ipl
  COMMAND ${RAKUSK_EXECUTABLE} ${25_day_harib22b_IPLS} ${25_day_harib22b_IPLB}
  DEPENDS ${25_day_harib22b_IPLS}
)
add_custom_command(
  OUTPUT ${25_day_harib22b_HEADB}
  COMMAND ${RAKUSK_EXECUTABLE} ${25_day_harib22b_HEADS} ${25_day_harib22b_HEADB}
  DEPENDS ${25_day_harib22b_HEADS}
  COMMENT "Generating asmhead.bin for 25_day_harib22b"
)
add_custom_command(
  OUTPUT ${25_day_harib22b_FUNCO}
  COMMAND ${RAKUSK_EXECUTABLE} ${25_day_harib22b_FUNCS} ${25_day_harib22b_FUNCO}
  DEPENDS ${25_day_harib22b_FUNCS}
  COMMENT "Generating naskfunc.o for 25_day_harib22b"
)
add_custom_command(
  OUTPUT ${25_day_harib22b_FONTO}
  COMMAND ${FONT} ${25_day_harib22b_FONTS} ${25_day_harib22b_FONTB}
  COMMAND ${B2O}  ${25_day_harib22b_FONTB} ${25_day_harib22b_FONTO} _hankaku
  DEPENDS ${25_day_harib22b_FONTS} makefont bin2obj
  COMMENT "Generating hankaku.o for 25_day_harib22b"
)
add_custom_command(
  OUTPUT ${25_day_harib22b_LIBGC}
  COMMAND ${CONV} -fcoff32 -nu ${25_day_harib22b_LIBGE} ${25_day_harib22b_LIBGC}
  DEPENDS ${25_day_harib22b_LIBGE}
  COMMENT "Converting golibc for 25_day_harib22b"
)

add_custom_target(25_day_harib22b_sys
  COMMAND gcc ${BINOPT} -T ${25_day_harib22b_LDS} ${25_day_harib22b_CCS} ${25_day_harib22b_WILDOBJ} -o ${25_day_harib22b_BOOTB}
  COMMAND cat ${25_day_harib22b_HEADB} ${25_day_harib22b_BOOTB} > ${25_day_harib22b_SYS}
  DEPENDS ${25_day_harib22b_HEADB} 25_day_harib22b_ipl
  DEPENDS ${25_day_harib22b_FUNCO}
  DEPENDS ${25_day_harib22b_FONTO} ${25_day_harib22b_LIBGC}
)
add_custom_target(25_day_harib22b_img
  COMMAND mformat -f 1440 -l HARIBOTEOS -N 0xffffffff -C -B ${25_day_harib22b_IPLB} -i ${25_day_harib22b_OS}
  COMMAND mcopy -i ${25_day_harib22b_OS} ${25_day_harib22b_SYS} ::
  COMMAND mcopy -i ${25_day_harib22b_OS} ${root_BINARY_DIR}/projects/25_day/harib22b/a.hrb ::a.hrb
  COMMAND mcopy -i ${25_day_harib22b_OS} ${root_BINARY_DIR}/projects/25_day/harib22b/hello3.hrb ::hello3.hrb
  COMMAND mcopy -i ${25_day_harib22b_OS} ${root_BINARY_DIR}/projects/25_day/harib22b/hello4.hrb ::hello4.hrb
  COMMAND mcopy -i ${25_day_harib22b_OS} ${root_BINARY_DIR}/projects/25_day/harib22b/winhelo.hrb ::winhelo.hrb
  COMMAND mcopy -i ${25_day_harib22b_OS} ${root_BINARY_DIR}/projects/25_day/harib22b/winhelo2.hrb ::winhelo2.hrb
  COMMAND mcopy -i ${25_day_harib22b_OS} ${root_BINARY_DIR}/projects/25_day/harib22b/winhelo3.hrb ::winhelo3.hrb
  COMMAND mcopy -i ${25_day_harib22b_OS} ${root_BINARY_DIR}/projects/25_day/harib22b/star1.hrb ::star1.hrb
  COMMAND mcopy -i ${25_day_harib22b_OS} ${root_BINARY_DIR}/projects/25_day/harib22b/stars.hrb ::stars.hrb
  COMMAND mcopy -i ${25_day_harib22b_OS} ${root_BINARY_DIR}/projects/25_day/harib22b/stars2.hrb ::stars2.hrb
  COMMAND mcopy -i ${25_day_harib22b_OS} ${root_BINARY_DIR}/projects/25_day/harib22b/lines.hrb ::lines.hrb
  COMMAND mcopy -i ${25_day_harib22b_OS} ${root_BINARY_DIR}/projects/25_day/harib22b/walk.hrb ::walk.hrb
  COMMAND mcopy -i ${25_day_harib22b_OS} ${root_BINARY_DIR}/projects/25_day/harib22b/color.hrb ::color.hrb
  COMMAND mcopy -i ${25_day_harib22b_OS} ${root_BINARY_DIR}/projects/25_day/harib22b/beepdown.hrb ::beepdown.hrb
  COMMAND mcopy -i ${25_day_harib22b_OS} ${root_BINARY_DIR}/projects/25_day/harib22b/noodle.hrb ::noodle.hrb
  COMMAND mcopy -i ${25_day_harib22b_OS} ${root_BINARY_DIR}/projects/25_day/harib22b/hello.hrb ::hello.hrb
  COMMAND mcopy -i ${25_day_harib22b_OS} ${root_BINARY_DIR}/projects/25_day/harib22b/hello2.hrb ::hello2.hrb
  COMMAND mcopy -i ${25_day_harib22b_OS} ${root_BINARY_DIR}/projects/25_day/harib22b/hello5.hrb ::hello5.hrb
  DEPENDS 25_day_harib22b_sys ${root_BINARY_DIR}/projects/25_day/harib22b/a.hrb ${root_BINARY_DIR}/projects/25_day/harib22b/hello3.hrb ${root_BINARY_DIR}/projects/25_day/harib22b/hello4.hrb ${root_BINARY_DIR}/projects/25_day/harib22b/winhelo.hrb ${root_BINARY_DIR}/projects/25_day/harib22b/winhelo2.hrb ${root_BINARY_DIR}/projects/25_day/harib22b/winhelo3.hrb ${root_BINARY_DIR}/projects/25_day/harib22b/star1.hrb ${root_BINARY_DIR}/projects/25_day/harib22b/stars.hrb ${root_BINARY_DIR}/projects/25_day/harib22b/stars2.hrb ${root_BINARY_DIR}/projects/25_day/harib22b/lines.hrb ${root_BINARY_DIR}/projects/25_day/harib22b/walk.hrb ${root_BINARY_DIR}/projects/25_day/harib22b/noodle.hrb ${root_BINARY_DIR}/projects/25_day/harib22b/beepdown.hrb ${root_BINARY_DIR}/projects/25_day/harib22b/color.hrb ${root_BINARY_DIR}/projects/25_day/harib22b/hello.hrb ${root_BINARY_DIR}/projects/25_day/harib22b/hello2.hrb ${root_BINARY_DIR}/projects/25_day/harib22b/hello5.hrb
)

#----------------------------------------------------------
########### application .hrb files (copied into os.img) ###############
set(25_day_harib22b_APIO ${root_BINARY_DIR}/projects/25_day/harib22b/a_nask.o)
set(25_day_harib22b_RANDO ${root_BINARY_DIR}/projects/25_day/harib22b/rand.o)
add_custom_command(
  OUTPUT ${25_day_harib22b_RANDO}
  COMMAND gcc -fleading-underscore -m32 -nostdlib -fno-pie -x c -c ${root_SOURCE_DIR}/golibc/rand.c_ -o ${25_day_harib22b_RANDO}
  DEPENDS ${root_SOURCE_DIR}/golibc/rand.c_
  COMMENT "Generating rand.o for 25_day_harib22b"
)
add_custom_command(
  OUTPUT ${25_day_harib22b_APIO}
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/25_day/harib22b/a_nask.nas ${25_day_harib22b_APIO}
  DEPENDS ${root_SOURCE_DIR}/projects/25_day/harib22b/a_nask.nas
  COMMENT "Generating a_nask.o for 25_day_harib22b"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/25_day/harib22b/a.hrb
  COMMAND gcc ${BINOPT} -T ${25_day_harib22b_APPLDS} ${root_SOURCE_DIR}/projects/25_day/harib22b/a.c ${25_day_harib22b_APIO} ${25_day_harib22b_LIBGC} ${25_day_harib22b_RANDO} -o ${root_BINARY_DIR}/projects/25_day/harib22b/a.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/25_day/harib22b/a.c ${root_SOURCE_DIR}/projects/app.lds ${25_day_harib22b_APIO} ${25_day_harib22b_LIBGC} ${25_day_harib22b_RANDO}
  COMMENT "Generating a.hrb for 25_day_harib22b"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/25_day/harib22b/hello3.hrb
  COMMAND gcc ${BINOPT} -T ${25_day_harib22b_APPLDS} ${root_SOURCE_DIR}/projects/25_day/harib22b/hello3.c ${25_day_harib22b_APIO} ${25_day_harib22b_LIBGC} ${25_day_harib22b_RANDO} -o ${root_BINARY_DIR}/projects/25_day/harib22b/hello3.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/25_day/harib22b/hello3.c ${root_SOURCE_DIR}/projects/app.lds ${25_day_harib22b_APIO} ${25_day_harib22b_LIBGC} ${25_day_harib22b_RANDO}
  COMMENT "Generating hello3.hrb for 25_day_harib22b"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/25_day/harib22b/hello4.hrb
  COMMAND gcc ${BINOPT} -T ${25_day_harib22b_APPLDS} ${root_SOURCE_DIR}/projects/25_day/harib22b/hello4.c ${25_day_harib22b_APIO} ${25_day_harib22b_LIBGC} ${25_day_harib22b_RANDO} -o ${root_BINARY_DIR}/projects/25_day/harib22b/hello4.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/25_day/harib22b/hello4.c ${root_SOURCE_DIR}/projects/app.lds ${25_day_harib22b_APIO} ${25_day_harib22b_LIBGC} ${25_day_harib22b_RANDO}
  COMMENT "Generating hello4.hrb for 25_day_harib22b"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/25_day/harib22b/winhelo.hrb
  COMMAND gcc ${BINOPT} -T ${25_day_harib22b_APPLDS} ${root_SOURCE_DIR}/projects/25_day/harib22b/winhelo.c ${25_day_harib22b_APIO} ${25_day_harib22b_LIBGC} ${25_day_harib22b_RANDO} -o ${root_BINARY_DIR}/projects/25_day/harib22b/winhelo.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/25_day/harib22b/winhelo.c ${root_SOURCE_DIR}/projects/app.lds ${25_day_harib22b_APIO} ${25_day_harib22b_LIBGC} ${25_day_harib22b_RANDO}
  COMMENT "Generating winhelo.hrb for 25_day_harib22b"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/25_day/harib22b/winhelo2.hrb
  COMMAND gcc ${BINOPT} -T ${25_day_harib22b_APPLDS} ${root_SOURCE_DIR}/projects/25_day/harib22b/winhelo2.c ${25_day_harib22b_APIO} ${25_day_harib22b_LIBGC} ${25_day_harib22b_RANDO} -o ${root_BINARY_DIR}/projects/25_day/harib22b/winhelo2.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/25_day/harib22b/winhelo2.c ${root_SOURCE_DIR}/projects/app.lds ${25_day_harib22b_APIO} ${25_day_harib22b_LIBGC} ${25_day_harib22b_RANDO}
  COMMENT "Generating winhelo2.hrb for 25_day_harib22b"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/25_day/harib22b/winhelo3.hrb
  COMMAND gcc ${BINOPT} -T ${25_day_harib22b_APPLDS} ${root_SOURCE_DIR}/projects/25_day/harib22b/winhelo3.c ${25_day_harib22b_APIO} ${25_day_harib22b_LIBGC} ${25_day_harib22b_RANDO} -o ${root_BINARY_DIR}/projects/25_day/harib22b/winhelo3.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/25_day/harib22b/winhelo3.c ${root_SOURCE_DIR}/projects/app.lds ${25_day_harib22b_APIO} ${25_day_harib22b_LIBGC} ${25_day_harib22b_RANDO}
  COMMENT "Generating winhelo3.hrb for 25_day_harib22b"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/25_day/harib22b/star1.hrb
  COMMAND gcc ${BINOPT} -T ${25_day_harib22b_APPLDS} ${root_SOURCE_DIR}/projects/25_day/harib22b/star1.c ${25_day_harib22b_APIO} ${25_day_harib22b_LIBGC} ${25_day_harib22b_RANDO} -o ${root_BINARY_DIR}/projects/25_day/harib22b/star1.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/25_day/harib22b/star1.c ${root_SOURCE_DIR}/projects/app.lds ${25_day_harib22b_APIO} ${25_day_harib22b_LIBGC} ${25_day_harib22b_RANDO}
  COMMENT "Generating star1.hrb for 25_day_harib22b"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/25_day/harib22b/stars.hrb
  COMMAND gcc ${BINOPT} -T ${25_day_harib22b_APPLDS} ${root_SOURCE_DIR}/projects/25_day/harib22b/stars.c ${25_day_harib22b_APIO} ${25_day_harib22b_LIBGC} ${25_day_harib22b_RANDO} -o ${root_BINARY_DIR}/projects/25_day/harib22b/stars.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/25_day/harib22b/stars.c ${root_SOURCE_DIR}/projects/app.lds ${25_day_harib22b_APIO} ${25_day_harib22b_LIBGC} ${25_day_harib22b_RANDO}
  COMMENT "Generating stars.hrb for 25_day_harib22b"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/25_day/harib22b/stars2.hrb
  COMMAND gcc ${BINOPT} -T ${25_day_harib22b_APPLDS} ${root_SOURCE_DIR}/projects/25_day/harib22b/stars2.c ${25_day_harib22b_APIO} ${25_day_harib22b_LIBGC} ${25_day_harib22b_RANDO} -o ${root_BINARY_DIR}/projects/25_day/harib22b/stars2.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/25_day/harib22b/stars2.c ${root_SOURCE_DIR}/projects/app.lds ${25_day_harib22b_APIO} ${25_day_harib22b_LIBGC} ${25_day_harib22b_RANDO}
  COMMENT "Generating stars2.hrb for 25_day_harib22b"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/25_day/harib22b/lines.hrb
  COMMAND gcc ${BINOPT} -T ${25_day_harib22b_APPLDS} ${root_SOURCE_DIR}/projects/25_day/harib22b/lines.c ${25_day_harib22b_APIO} ${25_day_harib22b_LIBGC} ${25_day_harib22b_RANDO} -o ${root_BINARY_DIR}/projects/25_day/harib22b/lines.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/25_day/harib22b/lines.c ${root_SOURCE_DIR}/projects/app.lds ${25_day_harib22b_APIO} ${25_day_harib22b_LIBGC} ${25_day_harib22b_RANDO}
  COMMENT "Generating lines.hrb for 25_day_harib22b"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/25_day/harib22b/walk.hrb
  COMMAND gcc ${BINOPT} -T ${25_day_harib22b_APPLDS} ${root_SOURCE_DIR}/projects/25_day/harib22b/walk.c ${25_day_harib22b_APIO} ${25_day_harib22b_LIBGC} ${25_day_harib22b_RANDO} -o ${root_BINARY_DIR}/projects/25_day/harib22b/walk.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/25_day/harib22b/walk.c ${root_SOURCE_DIR}/projects/app.lds ${25_day_harib22b_APIO} ${25_day_harib22b_LIBGC} ${25_day_harib22b_RANDO}
  COMMENT "Generating walk.hrb for 25_day_harib22b"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/25_day/harib22b/color.hrb
  COMMAND gcc ${BINOPT} -T ${25_day_harib22b_APPLDS} ${root_SOURCE_DIR}/projects/25_day/harib22b/color.c ${25_day_harib22b_APIO} ${25_day_harib22b_LIBGC} ${25_day_harib22b_RANDO} -o ${root_BINARY_DIR}/projects/25_day/harib22b/color.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/25_day/harib22b/color.c ${root_SOURCE_DIR}/projects/app.lds ${25_day_harib22b_APIO} ${25_day_harib22b_LIBGC} ${25_day_harib22b_RANDO}
  COMMENT "Generating color.hrb for 25_day_harib22b"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/25_day/harib22b/beepdown.hrb
  COMMAND gcc ${BINOPT} -T ${25_day_harib22b_APPLDS} ${root_SOURCE_DIR}/projects/25_day/harib22b/beepdown.c ${25_day_harib22b_APIO} ${25_day_harib22b_LIBGC} ${25_day_harib22b_RANDO} -o ${root_BINARY_DIR}/projects/25_day/harib22b/beepdown.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/25_day/harib22b/beepdown.c ${root_SOURCE_DIR}/projects/app.lds ${25_day_harib22b_APIO} ${25_day_harib22b_LIBGC} ${25_day_harib22b_RANDO}
  COMMENT "Generating beepdown.hrb for 25_day_harib22b"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/25_day/harib22b/noodle.hrb
  COMMAND gcc ${BINOPT} -T ${25_day_harib22b_APPLDS} ${root_SOURCE_DIR}/projects/25_day/harib22b/noodle.c ${25_day_harib22b_APIO} ${25_day_harib22b_LIBGC} ${25_day_harib22b_RANDO} -o ${root_BINARY_DIR}/projects/25_day/harib22b/noodle.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/25_day/harib22b/noodle.c ${root_SOURCE_DIR}/projects/app.lds ${25_day_harib22b_APIO} ${25_day_harib22b_LIBGC} ${25_day_harib22b_RANDO}
  COMMENT "Generating noodle.hrb for 25_day_harib22b"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/25_day/harib22b/hello.hrb
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/25_day/harib22b/hello.nas ${root_BINARY_DIR}/projects/25_day/harib22b/hello.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/25_day/harib22b/hello.nas
  COMMENT "Generating hello.hrb for 25_day_harib22b"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/25_day/harib22b/hello2.hrb
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/25_day/harib22b/hello2.nas ${root_BINARY_DIR}/projects/25_day/harib22b/hello2.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/25_day/harib22b/hello2.nas
  COMMENT "Generating hello2.hrb for 25_day_harib22b"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/25_day/harib22b/hello5.hrb
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/25_day/harib22b/hello5.nas ${root_BINARY_DIR}/projects/25_day/harib22b/hello5.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/25_day/harib22b/hello5.nas
  COMMENT "Generating hello5.hrb for 25_day_harib22b"
)
