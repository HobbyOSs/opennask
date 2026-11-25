#----------------------------------------------------------
message(STATUS "Entering directory projects/26_day/harib23b/")

set(FONT ${root_BINARY_DIR}/src/makefont)
set(B2O  ${root_BINARY_DIR}/src/bin2obj)
set(CONV ${root_BINARY_DIR}/objconv/objconv)
set(26_day_harib23b_OS    ${root_BINARY_DIR}/projects/26_day/harib23b/os.img)
set(26_day_harib23b_SYS	  ${root_BINARY_DIR}/projects/26_day/harib23b/os.sys)
set(26_day_harib23b_IPLB  ${root_BINARY_DIR}/projects/26_day/harib23b/ipl.bin)
set(26_day_harib23b_IPLS  ${root_SOURCE_DIR}/projects/26_day/harib23b/ipl10.nas)
set(26_day_harib23b_HEADB ${root_BINARY_DIR}/projects/26_day/harib23b/asmhead.bin)
set(26_day_harib23b_HEADS ${root_SOURCE_DIR}/projects/26_day/harib23b/asmhead.nas)
set(26_day_harib23b_BOOTB ${root_BINARY_DIR}/projects/26_day/harib23b/boot.bin)
set(26_day_harib23b_CCO	  ${root_BINARY_DIR}/projects/26_day/harib23b/boot.o)
file(GLOB 26_day_harib23b_CCS ${root_SOURCE_DIR}/projects/26_day/harib23b/*.c)
list(REMOVE_ITEM 26_day_harib23b_CCS
  ${root_SOURCE_DIR}/projects/26_day/harib23b/a.c
  ${root_SOURCE_DIR}/projects/26_day/harib23b/hello3.c
  ${root_SOURCE_DIR}/projects/26_day/harib23b/hello4.c
  ${root_SOURCE_DIR}/projects/26_day/harib23b/winhelo.c
  ${root_SOURCE_DIR}/projects/26_day/harib23b/winhelo2.c
  ${root_SOURCE_DIR}/projects/26_day/harib23b/winhelo3.c
  ${root_SOURCE_DIR}/projects/26_day/harib23b/star1.c
  ${root_SOURCE_DIR}/projects/26_day/harib23b/stars.c
  ${root_SOURCE_DIR}/projects/26_day/harib23b/stars2.c
  ${root_SOURCE_DIR}/projects/26_day/harib23b/lines.c
  ${root_SOURCE_DIR}/projects/26_day/harib23b/walk.c
  ${root_SOURCE_DIR}/projects/26_day/harib23b/noodle.c
  ${root_SOURCE_DIR}/projects/26_day/harib23b/beepdown.c
  ${root_SOURCE_DIR}/projects/26_day/harib23b/color.c
  ${root_SOURCE_DIR}/projects/26_day/harib23b/color2.c)
set(26_day_harib23b_LDS	  ${root_SOURCE_DIR}/projects/os.lds)
set(26_day_harib23b_APPLDS  ${root_SOURCE_DIR}/projects/app.lds)
set(26_day_harib23b_FUNCS ${root_SOURCE_DIR}/projects/26_day/harib23b/naskfunc.nas)
set(26_day_harib23b_FUNCO ${root_BINARY_DIR}/projects/26_day/harib23b/naskfunc.o)
set(26_day_harib23b_FONTS ${root_SOURCE_DIR}/projects/26_day/harib23b/hankaku.txt)
set(26_day_harib23b_FONTB ${root_BINARY_DIR}/projects/26_day/harib23b/hankaku.bin)
set(26_day_harib23b_FONTO ${root_BINARY_DIR}/projects/26_day/harib23b/hankaku.o)
set(26_day_harib23b_LIBGE ${root_BINARY_DIR}/golibc/libgo.a)
set(26_day_harib23b_LIBGC ${root_BINARY_DIR}/projects/26_day/harib23b/libgo.o)
set(26_day_harib23b_WILDOBJ ${root_BINARY_DIR}/projects/26_day/harib23b/*.o)


add_custom_target(26_day_harib23b_run
  COMMAND ${QEMU} ${QEMUOPT} ${26_day_harib23b_OS}
  DEPENDS 26_day_harib23b_img
)
add_custom_target(26_day_harib23b_debug
  COMMAND ${QEMU} -s -S ${QEMUOPT} ${26_day_harib23b_OS} ${QEMU_DEBUG_OPT}
  DEPENDS 26_day_harib23b_img
)
add_custom_target(26_day_harib23b_clean
  COMMAND rm -f ${26_day_harib23b_OS}
  COMMAND rm -f ${26_day_harib23b_SYS}
  COMMAND rm -f ${26_day_harib23b_IPLB}
  COMMAND rm -f ${26_day_harib23b_HEADB}
  COMMAND rm -f ${26_day_harib23b_BOOTB}
  COMMAND rm -f ${26_day_harib23b_WILDOBJ}
)
add_custom_target(26_day_harib23b_ipl
  COMMAND ${RAKUSK_EXECUTABLE} ${26_day_harib23b_IPLS} ${26_day_harib23b_IPLB}
  DEPENDS ${26_day_harib23b_IPLS}
)
add_custom_command(
  OUTPUT ${26_day_harib23b_HEADB}
  COMMAND ${RAKUSK_EXECUTABLE} ${26_day_harib23b_HEADS} ${26_day_harib23b_HEADB}
  DEPENDS ${26_day_harib23b_HEADS}
  COMMENT "Generating asmhead.bin for 26_day_harib23b"
)
add_custom_command(
  OUTPUT ${26_day_harib23b_FUNCO}
  COMMAND ${RAKUSK_EXECUTABLE} ${26_day_harib23b_FUNCS} ${26_day_harib23b_FUNCO}
  DEPENDS ${26_day_harib23b_FUNCS}
  COMMENT "Generating naskfunc.o for 26_day_harib23b"
)
add_custom_command(
  OUTPUT ${26_day_harib23b_FONTO}
  COMMAND ${FONT} ${26_day_harib23b_FONTS} ${26_day_harib23b_FONTB}
  COMMAND ${B2O}  ${26_day_harib23b_FONTB} ${26_day_harib23b_FONTO} _hankaku
  DEPENDS ${26_day_harib23b_FONTS} makefont bin2obj
  COMMENT "Generating hankaku.o for 26_day_harib23b"
)
add_custom_command(
  OUTPUT ${26_day_harib23b_LIBGC}
  COMMAND ${CONV} -fcoff32 -nu ${26_day_harib23b_LIBGE} ${26_day_harib23b_LIBGC}
  DEPENDS ${26_day_harib23b_LIBGE}
  COMMENT "Converting golibc for 26_day_harib23b"
)

add_custom_target(26_day_harib23b_sys
  COMMAND gcc ${BINOPT} -T ${26_day_harib23b_LDS} ${26_day_harib23b_CCS} ${26_day_harib23b_WILDOBJ} -o ${26_day_harib23b_BOOTB}
  COMMAND cat ${26_day_harib23b_HEADB} ${26_day_harib23b_BOOTB} > ${26_day_harib23b_SYS}
  DEPENDS ${26_day_harib23b_HEADB} 26_day_harib23b_ipl
  DEPENDS ${26_day_harib23b_FUNCO}
  DEPENDS ${26_day_harib23b_FONTO} ${26_day_harib23b_LIBGC}
)
add_custom_target(26_day_harib23b_img
  COMMAND mformat -f 1440 -l HARIBOTEOS -N 0xffffffff -C -B ${26_day_harib23b_IPLB} -i ${26_day_harib23b_OS}
  COMMAND mcopy -i ${26_day_harib23b_OS} ${26_day_harib23b_SYS} ::
  COMMAND mcopy -i ${26_day_harib23b_OS} ${root_BINARY_DIR}/projects/26_day/harib23b/a.hrb ::a.hrb
  COMMAND mcopy -i ${26_day_harib23b_OS} ${root_BINARY_DIR}/projects/26_day/harib23b/hello3.hrb ::hello3.hrb
  COMMAND mcopy -i ${26_day_harib23b_OS} ${root_BINARY_DIR}/projects/26_day/harib23b/hello4.hrb ::hello4.hrb
  COMMAND mcopy -i ${26_day_harib23b_OS} ${root_BINARY_DIR}/projects/26_day/harib23b/winhelo.hrb ::winhelo.hrb
  COMMAND mcopy -i ${26_day_harib23b_OS} ${root_BINARY_DIR}/projects/26_day/harib23b/winhelo2.hrb ::winhelo2.hrb
  COMMAND mcopy -i ${26_day_harib23b_OS} ${root_BINARY_DIR}/projects/26_day/harib23b/winhelo3.hrb ::winhelo3.hrb
  COMMAND mcopy -i ${26_day_harib23b_OS} ${root_BINARY_DIR}/projects/26_day/harib23b/star1.hrb ::star1.hrb
  COMMAND mcopy -i ${26_day_harib23b_OS} ${root_BINARY_DIR}/projects/26_day/harib23b/stars.hrb ::stars.hrb
  COMMAND mcopy -i ${26_day_harib23b_OS} ${root_BINARY_DIR}/projects/26_day/harib23b/stars2.hrb ::stars2.hrb
  COMMAND mcopy -i ${26_day_harib23b_OS} ${root_BINARY_DIR}/projects/26_day/harib23b/lines.hrb ::lines.hrb
  COMMAND mcopy -i ${26_day_harib23b_OS} ${root_BINARY_DIR}/projects/26_day/harib23b/walk.hrb ::walk.hrb
  COMMAND mcopy -i ${26_day_harib23b_OS} ${root_BINARY_DIR}/projects/26_day/harib23b/color2.hrb ::color2.hrb
  COMMAND mcopy -i ${26_day_harib23b_OS} ${root_BINARY_DIR}/projects/26_day/harib23b/color.hrb ::color.hrb
  COMMAND mcopy -i ${26_day_harib23b_OS} ${root_BINARY_DIR}/projects/26_day/harib23b/beepdown.hrb ::beepdown.hrb
  COMMAND mcopy -i ${26_day_harib23b_OS} ${root_BINARY_DIR}/projects/26_day/harib23b/noodle.hrb ::noodle.hrb
  COMMAND mcopy -i ${26_day_harib23b_OS} ${root_BINARY_DIR}/projects/26_day/harib23b/hello.hrb ::hello.hrb
  COMMAND mcopy -i ${26_day_harib23b_OS} ${root_BINARY_DIR}/projects/26_day/harib23b/hello2.hrb ::hello2.hrb
  COMMAND mcopy -i ${26_day_harib23b_OS} ${root_BINARY_DIR}/projects/26_day/harib23b/hello5.hrb ::hello5.hrb
  DEPENDS 26_day_harib23b_sys ${root_BINARY_DIR}/projects/26_day/harib23b/a.hrb ${root_BINARY_DIR}/projects/26_day/harib23b/hello3.hrb ${root_BINARY_DIR}/projects/26_day/harib23b/hello4.hrb ${root_BINARY_DIR}/projects/26_day/harib23b/winhelo.hrb ${root_BINARY_DIR}/projects/26_day/harib23b/winhelo2.hrb ${root_BINARY_DIR}/projects/26_day/harib23b/winhelo3.hrb ${root_BINARY_DIR}/projects/26_day/harib23b/star1.hrb ${root_BINARY_DIR}/projects/26_day/harib23b/stars.hrb ${root_BINARY_DIR}/projects/26_day/harib23b/stars2.hrb ${root_BINARY_DIR}/projects/26_day/harib23b/lines.hrb ${root_BINARY_DIR}/projects/26_day/harib23b/walk.hrb ${root_BINARY_DIR}/projects/26_day/harib23b/noodle.hrb ${root_BINARY_DIR}/projects/26_day/harib23b/beepdown.hrb ${root_BINARY_DIR}/projects/26_day/harib23b/color.hrb ${root_BINARY_DIR}/projects/26_day/harib23b/color2.hrb ${root_BINARY_DIR}/projects/26_day/harib23b/hello.hrb ${root_BINARY_DIR}/projects/26_day/harib23b/hello2.hrb ${root_BINARY_DIR}/projects/26_day/harib23b/hello5.hrb
)

#----------------------------------------------------------
########### application .hrb files (copied into os.img) ###############
set(26_day_harib23b_APIO ${root_BINARY_DIR}/projects/26_day/harib23b/a_nask.o)
set(26_day_harib23b_RANDO ${root_BINARY_DIR}/projects/26_day/harib23b/rand.o)
add_custom_command(
  OUTPUT ${26_day_harib23b_RANDO}
  COMMAND gcc -fleading-underscore -m32 -nostdlib -fno-pie -x c -c ${root_SOURCE_DIR}/golibc/rand.c_ -o ${26_day_harib23b_RANDO}
  DEPENDS ${root_SOURCE_DIR}/golibc/rand.c_
  COMMENT "Generating rand.o for 26_day_harib23b"
)
add_custom_command(
  OUTPUT ${26_day_harib23b_APIO}
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/26_day/harib23b/a_nask.nas ${26_day_harib23b_APIO}
  DEPENDS ${root_SOURCE_DIR}/projects/26_day/harib23b/a_nask.nas
  COMMENT "Generating a_nask.o for 26_day_harib23b"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/26_day/harib23b/a.hrb
  COMMAND gcc ${BINOPT} -T ${26_day_harib23b_APPLDS} ${root_SOURCE_DIR}/projects/26_day/harib23b/a.c ${26_day_harib23b_APIO} ${26_day_harib23b_LIBGC} ${26_day_harib23b_RANDO} -o ${root_BINARY_DIR}/projects/26_day/harib23b/a.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/26_day/harib23b/a.c ${root_SOURCE_DIR}/projects/app.lds ${26_day_harib23b_APIO} ${26_day_harib23b_LIBGC} ${26_day_harib23b_RANDO}
  COMMENT "Generating a.hrb for 26_day_harib23b"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/26_day/harib23b/hello3.hrb
  COMMAND gcc ${BINOPT} -T ${26_day_harib23b_APPLDS} ${root_SOURCE_DIR}/projects/26_day/harib23b/hello3.c ${26_day_harib23b_APIO} ${26_day_harib23b_LIBGC} ${26_day_harib23b_RANDO} -o ${root_BINARY_DIR}/projects/26_day/harib23b/hello3.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/26_day/harib23b/hello3.c ${root_SOURCE_DIR}/projects/app.lds ${26_day_harib23b_APIO} ${26_day_harib23b_LIBGC} ${26_day_harib23b_RANDO}
  COMMENT "Generating hello3.hrb for 26_day_harib23b"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/26_day/harib23b/hello4.hrb
  COMMAND gcc ${BINOPT} -T ${26_day_harib23b_APPLDS} ${root_SOURCE_DIR}/projects/26_day/harib23b/hello4.c ${26_day_harib23b_APIO} ${26_day_harib23b_LIBGC} ${26_day_harib23b_RANDO} -o ${root_BINARY_DIR}/projects/26_day/harib23b/hello4.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/26_day/harib23b/hello4.c ${root_SOURCE_DIR}/projects/app.lds ${26_day_harib23b_APIO} ${26_day_harib23b_LIBGC} ${26_day_harib23b_RANDO}
  COMMENT "Generating hello4.hrb for 26_day_harib23b"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/26_day/harib23b/winhelo.hrb
  COMMAND gcc ${BINOPT} -T ${26_day_harib23b_APPLDS} ${root_SOURCE_DIR}/projects/26_day/harib23b/winhelo.c ${26_day_harib23b_APIO} ${26_day_harib23b_LIBGC} ${26_day_harib23b_RANDO} -o ${root_BINARY_DIR}/projects/26_day/harib23b/winhelo.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/26_day/harib23b/winhelo.c ${root_SOURCE_DIR}/projects/app.lds ${26_day_harib23b_APIO} ${26_day_harib23b_LIBGC} ${26_day_harib23b_RANDO}
  COMMENT "Generating winhelo.hrb for 26_day_harib23b"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/26_day/harib23b/winhelo2.hrb
  COMMAND gcc ${BINOPT} -T ${26_day_harib23b_APPLDS} ${root_SOURCE_DIR}/projects/26_day/harib23b/winhelo2.c ${26_day_harib23b_APIO} ${26_day_harib23b_LIBGC} ${26_day_harib23b_RANDO} -o ${root_BINARY_DIR}/projects/26_day/harib23b/winhelo2.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/26_day/harib23b/winhelo2.c ${root_SOURCE_DIR}/projects/app.lds ${26_day_harib23b_APIO} ${26_day_harib23b_LIBGC} ${26_day_harib23b_RANDO}
  COMMENT "Generating winhelo2.hrb for 26_day_harib23b"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/26_day/harib23b/winhelo3.hrb
  COMMAND gcc ${BINOPT} -T ${26_day_harib23b_APPLDS} ${root_SOURCE_DIR}/projects/26_day/harib23b/winhelo3.c ${26_day_harib23b_APIO} ${26_day_harib23b_LIBGC} ${26_day_harib23b_RANDO} -o ${root_BINARY_DIR}/projects/26_day/harib23b/winhelo3.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/26_day/harib23b/winhelo3.c ${root_SOURCE_DIR}/projects/app.lds ${26_day_harib23b_APIO} ${26_day_harib23b_LIBGC} ${26_day_harib23b_RANDO}
  COMMENT "Generating winhelo3.hrb for 26_day_harib23b"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/26_day/harib23b/star1.hrb
  COMMAND gcc ${BINOPT} -T ${26_day_harib23b_APPLDS} ${root_SOURCE_DIR}/projects/26_day/harib23b/star1.c ${26_day_harib23b_APIO} ${26_day_harib23b_LIBGC} ${26_day_harib23b_RANDO} -o ${root_BINARY_DIR}/projects/26_day/harib23b/star1.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/26_day/harib23b/star1.c ${root_SOURCE_DIR}/projects/app.lds ${26_day_harib23b_APIO} ${26_day_harib23b_LIBGC} ${26_day_harib23b_RANDO}
  COMMENT "Generating star1.hrb for 26_day_harib23b"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/26_day/harib23b/stars.hrb
  COMMAND gcc ${BINOPT} -T ${26_day_harib23b_APPLDS} ${root_SOURCE_DIR}/projects/26_day/harib23b/stars.c ${26_day_harib23b_APIO} ${26_day_harib23b_LIBGC} ${26_day_harib23b_RANDO} -o ${root_BINARY_DIR}/projects/26_day/harib23b/stars.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/26_day/harib23b/stars.c ${root_SOURCE_DIR}/projects/app.lds ${26_day_harib23b_APIO} ${26_day_harib23b_LIBGC} ${26_day_harib23b_RANDO}
  COMMENT "Generating stars.hrb for 26_day_harib23b"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/26_day/harib23b/stars2.hrb
  COMMAND gcc ${BINOPT} -T ${26_day_harib23b_APPLDS} ${root_SOURCE_DIR}/projects/26_day/harib23b/stars2.c ${26_day_harib23b_APIO} ${26_day_harib23b_LIBGC} ${26_day_harib23b_RANDO} -o ${root_BINARY_DIR}/projects/26_day/harib23b/stars2.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/26_day/harib23b/stars2.c ${root_SOURCE_DIR}/projects/app.lds ${26_day_harib23b_APIO} ${26_day_harib23b_LIBGC} ${26_day_harib23b_RANDO}
  COMMENT "Generating stars2.hrb for 26_day_harib23b"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/26_day/harib23b/lines.hrb
  COMMAND gcc ${BINOPT} -T ${26_day_harib23b_APPLDS} ${root_SOURCE_DIR}/projects/26_day/harib23b/lines.c ${26_day_harib23b_APIO} ${26_day_harib23b_LIBGC} ${26_day_harib23b_RANDO} -o ${root_BINARY_DIR}/projects/26_day/harib23b/lines.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/26_day/harib23b/lines.c ${root_SOURCE_DIR}/projects/app.lds ${26_day_harib23b_APIO} ${26_day_harib23b_LIBGC} ${26_day_harib23b_RANDO}
  COMMENT "Generating lines.hrb for 26_day_harib23b"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/26_day/harib23b/walk.hrb
  COMMAND gcc ${BINOPT} -T ${26_day_harib23b_APPLDS} ${root_SOURCE_DIR}/projects/26_day/harib23b/walk.c ${26_day_harib23b_APIO} ${26_day_harib23b_LIBGC} ${26_day_harib23b_RANDO} -o ${root_BINARY_DIR}/projects/26_day/harib23b/walk.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/26_day/harib23b/walk.c ${root_SOURCE_DIR}/projects/app.lds ${26_day_harib23b_APIO} ${26_day_harib23b_LIBGC} ${26_day_harib23b_RANDO}
  COMMENT "Generating walk.hrb for 26_day_harib23b"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/26_day/harib23b/color2.hrb
  COMMAND gcc ${BINOPT} -T ${26_day_harib23b_APPLDS} ${root_SOURCE_DIR}/projects/26_day/harib23b/color2.c ${26_day_harib23b_APIO} ${26_day_harib23b_LIBGC} ${26_day_harib23b_RANDO} -o ${root_BINARY_DIR}/projects/26_day/harib23b/color2.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/26_day/harib23b/color2.c ${root_SOURCE_DIR}/projects/app.lds ${26_day_harib23b_APIO} ${26_day_harib23b_LIBGC} ${26_day_harib23b_RANDO}
  COMMENT "Generating color2.hrb for 26_day_harib23b"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/26_day/harib23b/color.hrb
  COMMAND gcc ${BINOPT} -T ${26_day_harib23b_APPLDS} ${root_SOURCE_DIR}/projects/26_day/harib23b/color.c ${26_day_harib23b_APIO} ${26_day_harib23b_LIBGC} ${26_day_harib23b_RANDO} -o ${root_BINARY_DIR}/projects/26_day/harib23b/color.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/26_day/harib23b/color.c ${root_SOURCE_DIR}/projects/app.lds ${26_day_harib23b_APIO} ${26_day_harib23b_LIBGC} ${26_day_harib23b_RANDO}
  COMMENT "Generating color.hrb for 26_day_harib23b"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/26_day/harib23b/beepdown.hrb
  COMMAND gcc ${BINOPT} -T ${26_day_harib23b_APPLDS} ${root_SOURCE_DIR}/projects/26_day/harib23b/beepdown.c ${26_day_harib23b_APIO} ${26_day_harib23b_LIBGC} ${26_day_harib23b_RANDO} -o ${root_BINARY_DIR}/projects/26_day/harib23b/beepdown.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/26_day/harib23b/beepdown.c ${root_SOURCE_DIR}/projects/app.lds ${26_day_harib23b_APIO} ${26_day_harib23b_LIBGC} ${26_day_harib23b_RANDO}
  COMMENT "Generating beepdown.hrb for 26_day_harib23b"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/26_day/harib23b/noodle.hrb
  COMMAND gcc ${BINOPT} -T ${26_day_harib23b_APPLDS} ${root_SOURCE_DIR}/projects/26_day/harib23b/noodle.c ${26_day_harib23b_APIO} ${26_day_harib23b_LIBGC} ${26_day_harib23b_RANDO} -o ${root_BINARY_DIR}/projects/26_day/harib23b/noodle.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/26_day/harib23b/noodle.c ${root_SOURCE_DIR}/projects/app.lds ${26_day_harib23b_APIO} ${26_day_harib23b_LIBGC} ${26_day_harib23b_RANDO}
  COMMENT "Generating noodle.hrb for 26_day_harib23b"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/26_day/harib23b/hello.hrb
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/26_day/harib23b/hello.nas ${root_BINARY_DIR}/projects/26_day/harib23b/hello.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/26_day/harib23b/hello.nas
  COMMENT "Generating hello.hrb for 26_day_harib23b"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/26_day/harib23b/hello2.hrb
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/26_day/harib23b/hello2.nas ${root_BINARY_DIR}/projects/26_day/harib23b/hello2.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/26_day/harib23b/hello2.nas
  COMMENT "Generating hello2.hrb for 26_day_harib23b"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/26_day/harib23b/hello5.hrb
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/26_day/harib23b/hello5.nas ${root_BINARY_DIR}/projects/26_day/harib23b/hello5.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/26_day/harib23b/hello5.nas
  COMMENT "Generating hello5.hrb for 26_day_harib23b"
)
