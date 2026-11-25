#----------------------------------------------------------
message(STATUS "Entering directory projects/27_day/harib24b/")

set(FONT ${root_BINARY_DIR}/src/makefont)
set(B2O  ${root_BINARY_DIR}/src/bin2obj)
set(CONV ${root_BINARY_DIR}/objconv/objconv)
set(27_day_harib24b_OS    ${root_BINARY_DIR}/projects/27_day/harib24b/os.img)
set(27_day_harib24b_SYS	  ${root_BINARY_DIR}/projects/27_day/harib24b/os.sys)
set(27_day_harib24b_IPLB  ${root_BINARY_DIR}/projects/27_day/harib24b/ipl.bin)
set(27_day_harib24b_IPLS  ${root_SOURCE_DIR}/projects/27_day/harib24b/ipl10.nas)
set(27_day_harib24b_HEADB ${root_BINARY_DIR}/projects/27_day/harib24b/asmhead.bin)
set(27_day_harib24b_HEADS ${root_SOURCE_DIR}/projects/27_day/harib24b/asmhead.nas)
set(27_day_harib24b_BOOTB ${root_BINARY_DIR}/projects/27_day/harib24b/boot.bin)
set(27_day_harib24b_CCO	  ${root_BINARY_DIR}/projects/27_day/harib24b/boot.o)
file(GLOB 27_day_harib24b_CCS ${root_SOURCE_DIR}/projects/27_day/harib24b/*.c)
list(REMOVE_ITEM 27_day_harib24b_CCS
  ${root_SOURCE_DIR}/projects/27_day/harib24b/a.c
  ${root_SOURCE_DIR}/projects/27_day/harib24b/hello3.c
  ${root_SOURCE_DIR}/projects/27_day/harib24b/hello4.c
  ${root_SOURCE_DIR}/projects/27_day/harib24b/winhelo.c
  ${root_SOURCE_DIR}/projects/27_day/harib24b/winhelo2.c
  ${root_SOURCE_DIR}/projects/27_day/harib24b/winhelo3.c
  ${root_SOURCE_DIR}/projects/27_day/harib24b/star1.c
  ${root_SOURCE_DIR}/projects/27_day/harib24b/stars.c
  ${root_SOURCE_DIR}/projects/27_day/harib24b/stars2.c
  ${root_SOURCE_DIR}/projects/27_day/harib24b/lines.c
  ${root_SOURCE_DIR}/projects/27_day/harib24b/walk.c
  ${root_SOURCE_DIR}/projects/27_day/harib24b/noodle.c
  ${root_SOURCE_DIR}/projects/27_day/harib24b/beepdown.c
  ${root_SOURCE_DIR}/projects/27_day/harib24b/color.c
  ${root_SOURCE_DIR}/projects/27_day/harib24b/color2.c)
set(27_day_harib24b_LDS	  ${root_SOURCE_DIR}/projects/os.lds)
set(27_day_harib24b_APPLDS  ${root_SOURCE_DIR}/projects/app.lds)
set(27_day_harib24b_FUNCS ${root_SOURCE_DIR}/projects/27_day/harib24b/naskfunc.nas)
set(27_day_harib24b_FUNCO ${root_BINARY_DIR}/projects/27_day/harib24b/naskfunc.o)
set(27_day_harib24b_FONTS ${root_SOURCE_DIR}/projects/27_day/harib24b/hankaku.txt)
set(27_day_harib24b_FONTB ${root_BINARY_DIR}/projects/27_day/harib24b/hankaku.bin)
set(27_day_harib24b_FONTO ${root_BINARY_DIR}/projects/27_day/harib24b/hankaku.o)
set(27_day_harib24b_LIBGE ${root_BINARY_DIR}/golibc/libgo.a)
set(27_day_harib24b_LIBGC ${root_BINARY_DIR}/projects/27_day/harib24b/libgo.o)
set(27_day_harib24b_WILDOBJ ${root_BINARY_DIR}/projects/27_day/harib24b/*.o)


add_custom_target(27_day_harib24b_run
  COMMAND ${QEMU} ${QEMUOPT} ${27_day_harib24b_OS}
  DEPENDS 27_day_harib24b_img
)
add_custom_target(27_day_harib24b_debug
  COMMAND ${QEMU} -s -S ${QEMUOPT} ${27_day_harib24b_OS} ${QEMU_DEBUG_OPT}
  DEPENDS 27_day_harib24b_img
)
add_custom_target(27_day_harib24b_clean
  COMMAND rm -f ${27_day_harib24b_OS}
  COMMAND rm -f ${27_day_harib24b_SYS}
  COMMAND rm -f ${27_day_harib24b_IPLB}
  COMMAND rm -f ${27_day_harib24b_HEADB}
  COMMAND rm -f ${27_day_harib24b_BOOTB}
  COMMAND rm -f ${27_day_harib24b_WILDOBJ}
)
add_custom_target(27_day_harib24b_ipl
  COMMAND ${RAKUSK_EXECUTABLE} ${27_day_harib24b_IPLS} ${27_day_harib24b_IPLB}
  DEPENDS ${27_day_harib24b_IPLS}
)
add_custom_command(
  OUTPUT ${27_day_harib24b_HEADB}
  COMMAND ${RAKUSK_EXECUTABLE} ${27_day_harib24b_HEADS} ${27_day_harib24b_HEADB}
  DEPENDS ${27_day_harib24b_HEADS}
  COMMENT "Generating asmhead.bin for 27_day_harib24b"
)
add_custom_command(
  OUTPUT ${27_day_harib24b_FUNCO}
  COMMAND ${RAKUSK_EXECUTABLE} ${27_day_harib24b_FUNCS} ${27_day_harib24b_FUNCO}
  DEPENDS ${27_day_harib24b_FUNCS}
  COMMENT "Generating naskfunc.o for 27_day_harib24b"
)
add_custom_command(
  OUTPUT ${27_day_harib24b_FONTO}
  COMMAND ${FONT} ${27_day_harib24b_FONTS} ${27_day_harib24b_FONTB}
  COMMAND ${B2O}  ${27_day_harib24b_FONTB} ${27_day_harib24b_FONTO} _hankaku
  DEPENDS ${27_day_harib24b_FONTS} makefont bin2obj
  COMMENT "Generating hankaku.o for 27_day_harib24b"
)
add_custom_command(
  OUTPUT ${27_day_harib24b_LIBGC}
  COMMAND ${CONV} -fcoff32 -nu ${27_day_harib24b_LIBGE} ${27_day_harib24b_LIBGC}
  DEPENDS ${27_day_harib24b_LIBGE}
  COMMENT "Converting golibc for 27_day_harib24b"
)

add_custom_target(27_day_harib24b_sys
  COMMAND gcc ${BINOPT} -T ${27_day_harib24b_LDS} ${27_day_harib24b_CCS} ${27_day_harib24b_WILDOBJ} -o ${27_day_harib24b_BOOTB}
  COMMAND cat ${27_day_harib24b_HEADB} ${27_day_harib24b_BOOTB} > ${27_day_harib24b_SYS}
  DEPENDS ${27_day_harib24b_HEADB} 27_day_harib24b_ipl
  DEPENDS ${27_day_harib24b_FUNCO}
  DEPENDS ${27_day_harib24b_FONTO} ${27_day_harib24b_LIBGC}
)
add_custom_target(27_day_harib24b_img
  COMMAND mformat -f 1440 -l HARIBOTEOS -N 0xffffffff -C -B ${27_day_harib24b_IPLB} -i ${27_day_harib24b_OS}
  COMMAND mcopy -i ${27_day_harib24b_OS} ${27_day_harib24b_SYS} ::
  COMMAND mcopy -i ${27_day_harib24b_OS} ${root_BINARY_DIR}/projects/27_day/harib24b/a.hrb ::a.hrb
  COMMAND mcopy -i ${27_day_harib24b_OS} ${root_BINARY_DIR}/projects/27_day/harib24b/hello3.hrb ::hello3.hrb
  COMMAND mcopy -i ${27_day_harib24b_OS} ${root_BINARY_DIR}/projects/27_day/harib24b/hello4.hrb ::hello4.hrb
  COMMAND mcopy -i ${27_day_harib24b_OS} ${root_BINARY_DIR}/projects/27_day/harib24b/winhelo.hrb ::winhelo.hrb
  COMMAND mcopy -i ${27_day_harib24b_OS} ${root_BINARY_DIR}/projects/27_day/harib24b/winhelo2.hrb ::winhelo2.hrb
  COMMAND mcopy -i ${27_day_harib24b_OS} ${root_BINARY_DIR}/projects/27_day/harib24b/winhelo3.hrb ::winhelo3.hrb
  COMMAND mcopy -i ${27_day_harib24b_OS} ${root_BINARY_DIR}/projects/27_day/harib24b/star1.hrb ::star1.hrb
  COMMAND mcopy -i ${27_day_harib24b_OS} ${root_BINARY_DIR}/projects/27_day/harib24b/stars.hrb ::stars.hrb
  COMMAND mcopy -i ${27_day_harib24b_OS} ${root_BINARY_DIR}/projects/27_day/harib24b/stars2.hrb ::stars2.hrb
  COMMAND mcopy -i ${27_day_harib24b_OS} ${root_BINARY_DIR}/projects/27_day/harib24b/lines.hrb ::lines.hrb
  COMMAND mcopy -i ${27_day_harib24b_OS} ${root_BINARY_DIR}/projects/27_day/harib24b/walk.hrb ::walk.hrb
  COMMAND mcopy -i ${27_day_harib24b_OS} ${root_BINARY_DIR}/projects/27_day/harib24b/color2.hrb ::color2.hrb
  COMMAND mcopy -i ${27_day_harib24b_OS} ${root_BINARY_DIR}/projects/27_day/harib24b/color.hrb ::color.hrb
  COMMAND mcopy -i ${27_day_harib24b_OS} ${root_BINARY_DIR}/projects/27_day/harib24b/beepdown.hrb ::beepdown.hrb
  COMMAND mcopy -i ${27_day_harib24b_OS} ${root_BINARY_DIR}/projects/27_day/harib24b/noodle.hrb ::noodle.hrb
  COMMAND mcopy -i ${27_day_harib24b_OS} ${root_BINARY_DIR}/projects/27_day/harib24b/hello.hrb ::hello.hrb
  COMMAND mcopy -i ${27_day_harib24b_OS} ${root_BINARY_DIR}/projects/27_day/harib24b/hello2.hrb ::hello2.hrb
  COMMAND mcopy -i ${27_day_harib24b_OS} ${root_BINARY_DIR}/projects/27_day/harib24b/hello5.hrb ::hello5.hrb
  DEPENDS 27_day_harib24b_sys ${root_BINARY_DIR}/projects/27_day/harib24b/a.hrb ${root_BINARY_DIR}/projects/27_day/harib24b/hello3.hrb ${root_BINARY_DIR}/projects/27_day/harib24b/hello4.hrb ${root_BINARY_DIR}/projects/27_day/harib24b/winhelo.hrb ${root_BINARY_DIR}/projects/27_day/harib24b/winhelo2.hrb ${root_BINARY_DIR}/projects/27_day/harib24b/winhelo3.hrb ${root_BINARY_DIR}/projects/27_day/harib24b/star1.hrb ${root_BINARY_DIR}/projects/27_day/harib24b/stars.hrb ${root_BINARY_DIR}/projects/27_day/harib24b/stars2.hrb ${root_BINARY_DIR}/projects/27_day/harib24b/lines.hrb ${root_BINARY_DIR}/projects/27_day/harib24b/walk.hrb ${root_BINARY_DIR}/projects/27_day/harib24b/noodle.hrb ${root_BINARY_DIR}/projects/27_day/harib24b/beepdown.hrb ${root_BINARY_DIR}/projects/27_day/harib24b/color.hrb ${root_BINARY_DIR}/projects/27_day/harib24b/color2.hrb ${root_BINARY_DIR}/projects/27_day/harib24b/hello.hrb ${root_BINARY_DIR}/projects/27_day/harib24b/hello2.hrb ${root_BINARY_DIR}/projects/27_day/harib24b/hello5.hrb
)

#----------------------------------------------------------
########### application .hrb files (copied into os.img) ###############
set(27_day_harib24b_APIO ${root_BINARY_DIR}/projects/27_day/harib24b/a_nask.o)
set(27_day_harib24b_RANDO ${root_BINARY_DIR}/projects/27_day/harib24b/rand.o)
add_custom_command(
  OUTPUT ${27_day_harib24b_RANDO}
  COMMAND gcc -fleading-underscore -m32 -nostdlib -fno-pie -x c -c ${root_SOURCE_DIR}/golibc/rand.c_ -o ${27_day_harib24b_RANDO}
  DEPENDS ${root_SOURCE_DIR}/golibc/rand.c_
  COMMENT "Generating rand.o for 27_day_harib24b"
)
add_custom_command(
  OUTPUT ${27_day_harib24b_APIO}
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/27_day/harib24b/a_nask.nas ${27_day_harib24b_APIO}
  DEPENDS ${root_SOURCE_DIR}/projects/27_day/harib24b/a_nask.nas
  COMMENT "Generating a_nask.o for 27_day_harib24b"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/27_day/harib24b/a.hrb
  COMMAND gcc ${BINOPT} -T ${27_day_harib24b_APPLDS} ${root_SOURCE_DIR}/projects/27_day/harib24b/a.c ${27_day_harib24b_APIO} ${27_day_harib24b_LIBGC} ${27_day_harib24b_RANDO} -o ${root_BINARY_DIR}/projects/27_day/harib24b/a.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/27_day/harib24b/a.c ${root_SOURCE_DIR}/projects/app.lds ${27_day_harib24b_APIO} ${27_day_harib24b_LIBGC} ${27_day_harib24b_RANDO}
  COMMENT "Generating a.hrb for 27_day_harib24b"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/27_day/harib24b/hello3.hrb
  COMMAND gcc ${BINOPT} -T ${27_day_harib24b_APPLDS} ${root_SOURCE_DIR}/projects/27_day/harib24b/hello3.c ${27_day_harib24b_APIO} ${27_day_harib24b_LIBGC} ${27_day_harib24b_RANDO} -o ${root_BINARY_DIR}/projects/27_day/harib24b/hello3.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/27_day/harib24b/hello3.c ${root_SOURCE_DIR}/projects/app.lds ${27_day_harib24b_APIO} ${27_day_harib24b_LIBGC} ${27_day_harib24b_RANDO}
  COMMENT "Generating hello3.hrb for 27_day_harib24b"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/27_day/harib24b/hello4.hrb
  COMMAND gcc ${BINOPT} -T ${27_day_harib24b_APPLDS} ${root_SOURCE_DIR}/projects/27_day/harib24b/hello4.c ${27_day_harib24b_APIO} ${27_day_harib24b_LIBGC} ${27_day_harib24b_RANDO} -o ${root_BINARY_DIR}/projects/27_day/harib24b/hello4.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/27_day/harib24b/hello4.c ${root_SOURCE_DIR}/projects/app.lds ${27_day_harib24b_APIO} ${27_day_harib24b_LIBGC} ${27_day_harib24b_RANDO}
  COMMENT "Generating hello4.hrb for 27_day_harib24b"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/27_day/harib24b/winhelo.hrb
  COMMAND gcc ${BINOPT} -T ${27_day_harib24b_APPLDS} ${root_SOURCE_DIR}/projects/27_day/harib24b/winhelo.c ${27_day_harib24b_APIO} ${27_day_harib24b_LIBGC} ${27_day_harib24b_RANDO} -o ${root_BINARY_DIR}/projects/27_day/harib24b/winhelo.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/27_day/harib24b/winhelo.c ${root_SOURCE_DIR}/projects/app.lds ${27_day_harib24b_APIO} ${27_day_harib24b_LIBGC} ${27_day_harib24b_RANDO}
  COMMENT "Generating winhelo.hrb for 27_day_harib24b"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/27_day/harib24b/winhelo2.hrb
  COMMAND gcc ${BINOPT} -T ${27_day_harib24b_APPLDS} ${root_SOURCE_DIR}/projects/27_day/harib24b/winhelo2.c ${27_day_harib24b_APIO} ${27_day_harib24b_LIBGC} ${27_day_harib24b_RANDO} -o ${root_BINARY_DIR}/projects/27_day/harib24b/winhelo2.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/27_day/harib24b/winhelo2.c ${root_SOURCE_DIR}/projects/app.lds ${27_day_harib24b_APIO} ${27_day_harib24b_LIBGC} ${27_day_harib24b_RANDO}
  COMMENT "Generating winhelo2.hrb for 27_day_harib24b"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/27_day/harib24b/winhelo3.hrb
  COMMAND gcc ${BINOPT} -T ${27_day_harib24b_APPLDS} ${root_SOURCE_DIR}/projects/27_day/harib24b/winhelo3.c ${27_day_harib24b_APIO} ${27_day_harib24b_LIBGC} ${27_day_harib24b_RANDO} -o ${root_BINARY_DIR}/projects/27_day/harib24b/winhelo3.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/27_day/harib24b/winhelo3.c ${root_SOURCE_DIR}/projects/app.lds ${27_day_harib24b_APIO} ${27_day_harib24b_LIBGC} ${27_day_harib24b_RANDO}
  COMMENT "Generating winhelo3.hrb for 27_day_harib24b"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/27_day/harib24b/star1.hrb
  COMMAND gcc ${BINOPT} -T ${27_day_harib24b_APPLDS} ${root_SOURCE_DIR}/projects/27_day/harib24b/star1.c ${27_day_harib24b_APIO} ${27_day_harib24b_LIBGC} ${27_day_harib24b_RANDO} -o ${root_BINARY_DIR}/projects/27_day/harib24b/star1.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/27_day/harib24b/star1.c ${root_SOURCE_DIR}/projects/app.lds ${27_day_harib24b_APIO} ${27_day_harib24b_LIBGC} ${27_day_harib24b_RANDO}
  COMMENT "Generating star1.hrb for 27_day_harib24b"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/27_day/harib24b/stars.hrb
  COMMAND gcc ${BINOPT} -T ${27_day_harib24b_APPLDS} ${root_SOURCE_DIR}/projects/27_day/harib24b/stars.c ${27_day_harib24b_APIO} ${27_day_harib24b_LIBGC} ${27_day_harib24b_RANDO} -o ${root_BINARY_DIR}/projects/27_day/harib24b/stars.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/27_day/harib24b/stars.c ${root_SOURCE_DIR}/projects/app.lds ${27_day_harib24b_APIO} ${27_day_harib24b_LIBGC} ${27_day_harib24b_RANDO}
  COMMENT "Generating stars.hrb for 27_day_harib24b"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/27_day/harib24b/stars2.hrb
  COMMAND gcc ${BINOPT} -T ${27_day_harib24b_APPLDS} ${root_SOURCE_DIR}/projects/27_day/harib24b/stars2.c ${27_day_harib24b_APIO} ${27_day_harib24b_LIBGC} ${27_day_harib24b_RANDO} -o ${root_BINARY_DIR}/projects/27_day/harib24b/stars2.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/27_day/harib24b/stars2.c ${root_SOURCE_DIR}/projects/app.lds ${27_day_harib24b_APIO} ${27_day_harib24b_LIBGC} ${27_day_harib24b_RANDO}
  COMMENT "Generating stars2.hrb for 27_day_harib24b"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/27_day/harib24b/lines.hrb
  COMMAND gcc ${BINOPT} -T ${27_day_harib24b_APPLDS} ${root_SOURCE_DIR}/projects/27_day/harib24b/lines.c ${27_day_harib24b_APIO} ${27_day_harib24b_LIBGC} ${27_day_harib24b_RANDO} -o ${root_BINARY_DIR}/projects/27_day/harib24b/lines.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/27_day/harib24b/lines.c ${root_SOURCE_DIR}/projects/app.lds ${27_day_harib24b_APIO} ${27_day_harib24b_LIBGC} ${27_day_harib24b_RANDO}
  COMMENT "Generating lines.hrb for 27_day_harib24b"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/27_day/harib24b/walk.hrb
  COMMAND gcc ${BINOPT} -T ${27_day_harib24b_APPLDS} ${root_SOURCE_DIR}/projects/27_day/harib24b/walk.c ${27_day_harib24b_APIO} ${27_day_harib24b_LIBGC} ${27_day_harib24b_RANDO} -o ${root_BINARY_DIR}/projects/27_day/harib24b/walk.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/27_day/harib24b/walk.c ${root_SOURCE_DIR}/projects/app.lds ${27_day_harib24b_APIO} ${27_day_harib24b_LIBGC} ${27_day_harib24b_RANDO}
  COMMENT "Generating walk.hrb for 27_day_harib24b"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/27_day/harib24b/color2.hrb
  COMMAND gcc ${BINOPT} -T ${27_day_harib24b_APPLDS} ${root_SOURCE_DIR}/projects/27_day/harib24b/color2.c ${27_day_harib24b_APIO} ${27_day_harib24b_LIBGC} ${27_day_harib24b_RANDO} -o ${root_BINARY_DIR}/projects/27_day/harib24b/color2.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/27_day/harib24b/color2.c ${root_SOURCE_DIR}/projects/app.lds ${27_day_harib24b_APIO} ${27_day_harib24b_LIBGC} ${27_day_harib24b_RANDO}
  COMMENT "Generating color2.hrb for 27_day_harib24b"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/27_day/harib24b/color.hrb
  COMMAND gcc ${BINOPT} -T ${27_day_harib24b_APPLDS} ${root_SOURCE_DIR}/projects/27_day/harib24b/color.c ${27_day_harib24b_APIO} ${27_day_harib24b_LIBGC} ${27_day_harib24b_RANDO} -o ${root_BINARY_DIR}/projects/27_day/harib24b/color.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/27_day/harib24b/color.c ${root_SOURCE_DIR}/projects/app.lds ${27_day_harib24b_APIO} ${27_day_harib24b_LIBGC} ${27_day_harib24b_RANDO}
  COMMENT "Generating color.hrb for 27_day_harib24b"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/27_day/harib24b/beepdown.hrb
  COMMAND gcc ${BINOPT} -T ${27_day_harib24b_APPLDS} ${root_SOURCE_DIR}/projects/27_day/harib24b/beepdown.c ${27_day_harib24b_APIO} ${27_day_harib24b_LIBGC} ${27_day_harib24b_RANDO} -o ${root_BINARY_DIR}/projects/27_day/harib24b/beepdown.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/27_day/harib24b/beepdown.c ${root_SOURCE_DIR}/projects/app.lds ${27_day_harib24b_APIO} ${27_day_harib24b_LIBGC} ${27_day_harib24b_RANDO}
  COMMENT "Generating beepdown.hrb for 27_day_harib24b"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/27_day/harib24b/noodle.hrb
  COMMAND gcc ${BINOPT} -T ${27_day_harib24b_APPLDS} ${root_SOURCE_DIR}/projects/27_day/harib24b/noodle.c ${27_day_harib24b_APIO} ${27_day_harib24b_LIBGC} ${27_day_harib24b_RANDO} -o ${root_BINARY_DIR}/projects/27_day/harib24b/noodle.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/27_day/harib24b/noodle.c ${root_SOURCE_DIR}/projects/app.lds ${27_day_harib24b_APIO} ${27_day_harib24b_LIBGC} ${27_day_harib24b_RANDO}
  COMMENT "Generating noodle.hrb for 27_day_harib24b"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/27_day/harib24b/hello.hrb
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/27_day/harib24b/hello.nas ${root_BINARY_DIR}/projects/27_day/harib24b/hello.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/27_day/harib24b/hello.nas
  COMMENT "Generating hello.hrb for 27_day_harib24b"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/27_day/harib24b/hello2.hrb
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/27_day/harib24b/hello2.nas ${root_BINARY_DIR}/projects/27_day/harib24b/hello2.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/27_day/harib24b/hello2.nas
  COMMENT "Generating hello2.hrb for 27_day_harib24b"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/27_day/harib24b/hello5.hrb
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/27_day/harib24b/hello5.nas ${root_BINARY_DIR}/projects/27_day/harib24b/hello5.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/27_day/harib24b/hello5.nas
  COMMENT "Generating hello5.hrb for 27_day_harib24b"
)
