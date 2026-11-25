#----------------------------------------------------------
message(STATUS "Entering directory projects/27_day/harib24a/")

set(FONT ${root_BINARY_DIR}/src/makefont)
set(B2O  ${root_BINARY_DIR}/src/bin2obj)
set(CONV ${root_BINARY_DIR}/objconv/objconv)
set(27_day_harib24a_OS    ${root_BINARY_DIR}/projects/27_day/harib24a/os.img)
set(27_day_harib24a_SYS	  ${root_BINARY_DIR}/projects/27_day/harib24a/os.sys)
set(27_day_harib24a_IPLB  ${root_BINARY_DIR}/projects/27_day/harib24a/ipl.bin)
set(27_day_harib24a_IPLS  ${root_SOURCE_DIR}/projects/27_day/harib24a/ipl10.nas)
set(27_day_harib24a_HEADB ${root_BINARY_DIR}/projects/27_day/harib24a/asmhead.bin)
set(27_day_harib24a_HEADS ${root_SOURCE_DIR}/projects/27_day/harib24a/asmhead.nas)
set(27_day_harib24a_BOOTB ${root_BINARY_DIR}/projects/27_day/harib24a/boot.bin)
set(27_day_harib24a_CCO	  ${root_BINARY_DIR}/projects/27_day/harib24a/boot.o)
file(GLOB 27_day_harib24a_CCS ${root_SOURCE_DIR}/projects/27_day/harib24a/*.c)
list(REMOVE_ITEM 27_day_harib24a_CCS
  ${root_SOURCE_DIR}/projects/27_day/harib24a/a.c
  ${root_SOURCE_DIR}/projects/27_day/harib24a/hello3.c
  ${root_SOURCE_DIR}/projects/27_day/harib24a/hello4.c
  ${root_SOURCE_DIR}/projects/27_day/harib24a/winhelo.c
  ${root_SOURCE_DIR}/projects/27_day/harib24a/winhelo2.c
  ${root_SOURCE_DIR}/projects/27_day/harib24a/winhelo3.c
  ${root_SOURCE_DIR}/projects/27_day/harib24a/star1.c
  ${root_SOURCE_DIR}/projects/27_day/harib24a/stars.c
  ${root_SOURCE_DIR}/projects/27_day/harib24a/stars2.c
  ${root_SOURCE_DIR}/projects/27_day/harib24a/lines.c
  ${root_SOURCE_DIR}/projects/27_day/harib24a/walk.c
  ${root_SOURCE_DIR}/projects/27_day/harib24a/noodle.c
  ${root_SOURCE_DIR}/projects/27_day/harib24a/beepdown.c
  ${root_SOURCE_DIR}/projects/27_day/harib24a/color.c
  ${root_SOURCE_DIR}/projects/27_day/harib24a/color2.c)
set(27_day_harib24a_LDS	  ${root_SOURCE_DIR}/projects/os.lds)
set(27_day_harib24a_APPLDS  ${root_SOURCE_DIR}/projects/app.lds)
set(27_day_harib24a_FUNCS ${root_SOURCE_DIR}/projects/27_day/harib24a/naskfunc.nas)
set(27_day_harib24a_FUNCO ${root_BINARY_DIR}/projects/27_day/harib24a/naskfunc.o)
set(27_day_harib24a_FONTS ${root_SOURCE_DIR}/projects/27_day/harib24a/hankaku.txt)
set(27_day_harib24a_FONTB ${root_BINARY_DIR}/projects/27_day/harib24a/hankaku.bin)
set(27_day_harib24a_FONTO ${root_BINARY_DIR}/projects/27_day/harib24a/hankaku.o)
set(27_day_harib24a_LIBGE ${root_BINARY_DIR}/golibc/libgo.a)
set(27_day_harib24a_LIBGC ${root_BINARY_DIR}/projects/27_day/harib24a/libgo.o)
set(27_day_harib24a_WILDOBJ ${root_BINARY_DIR}/projects/27_day/harib24a/*.o)


add_custom_target(27_day_harib24a_run
  COMMAND ${QEMU} ${QEMUOPT} ${27_day_harib24a_OS}
  DEPENDS 27_day_harib24a_img
)
add_custom_target(27_day_harib24a_debug
  COMMAND ${QEMU} -s -S ${QEMUOPT} ${27_day_harib24a_OS} ${QEMU_DEBUG_OPT}
  DEPENDS 27_day_harib24a_img
)
add_custom_target(27_day_harib24a_clean
  COMMAND rm -f ${27_day_harib24a_OS}
  COMMAND rm -f ${27_day_harib24a_SYS}
  COMMAND rm -f ${27_day_harib24a_IPLB}
  COMMAND rm -f ${27_day_harib24a_HEADB}
  COMMAND rm -f ${27_day_harib24a_BOOTB}
  COMMAND rm -f ${27_day_harib24a_WILDOBJ}
)
add_custom_target(27_day_harib24a_ipl
  COMMAND ${RAKUSK_EXECUTABLE} ${27_day_harib24a_IPLS} ${27_day_harib24a_IPLB}
  DEPENDS ${27_day_harib24a_IPLS}
)
add_custom_command(
  OUTPUT ${27_day_harib24a_HEADB}
  COMMAND ${RAKUSK_EXECUTABLE} ${27_day_harib24a_HEADS} ${27_day_harib24a_HEADB}
  DEPENDS ${27_day_harib24a_HEADS}
  COMMENT "Generating asmhead.bin for 27_day_harib24a"
)
add_custom_command(
  OUTPUT ${27_day_harib24a_FUNCO}
  COMMAND ${RAKUSK_EXECUTABLE} ${27_day_harib24a_FUNCS} ${27_day_harib24a_FUNCO}
  DEPENDS ${27_day_harib24a_FUNCS}
  COMMENT "Generating naskfunc.o for 27_day_harib24a"
)
add_custom_command(
  OUTPUT ${27_day_harib24a_FONTO}
  COMMAND ${FONT} ${27_day_harib24a_FONTS} ${27_day_harib24a_FONTB}
  COMMAND ${B2O}  ${27_day_harib24a_FONTB} ${27_day_harib24a_FONTO} _hankaku
  DEPENDS ${27_day_harib24a_FONTS} makefont bin2obj
  COMMENT "Generating hankaku.o for 27_day_harib24a"
)
add_custom_command(
  OUTPUT ${27_day_harib24a_LIBGC}
  COMMAND ${CONV} -fcoff32 -nu ${27_day_harib24a_LIBGE} ${27_day_harib24a_LIBGC}
  DEPENDS ${27_day_harib24a_LIBGE}
  COMMENT "Converting golibc for 27_day_harib24a"
)

add_custom_target(27_day_harib24a_sys
  COMMAND gcc ${BINOPT} -T ${27_day_harib24a_LDS} ${27_day_harib24a_CCS} ${27_day_harib24a_WILDOBJ} -o ${27_day_harib24a_BOOTB}
  COMMAND cat ${27_day_harib24a_HEADB} ${27_day_harib24a_BOOTB} > ${27_day_harib24a_SYS}
  DEPENDS ${27_day_harib24a_HEADB} 27_day_harib24a_ipl
  DEPENDS ${27_day_harib24a_FUNCO}
  DEPENDS ${27_day_harib24a_FONTO} ${27_day_harib24a_LIBGC}
)
add_custom_target(27_day_harib24a_img
  COMMAND mformat -f 1440 -l HARIBOTEOS -N 0xffffffff -C -B ${27_day_harib24a_IPLB} -i ${27_day_harib24a_OS}
  COMMAND mcopy -i ${27_day_harib24a_OS} ${27_day_harib24a_SYS} ::
  COMMAND mcopy -i ${27_day_harib24a_OS} ${root_BINARY_DIR}/projects/27_day/harib24a/a.hrb ::a.hrb
  COMMAND mcopy -i ${27_day_harib24a_OS} ${root_BINARY_DIR}/projects/27_day/harib24a/hello3.hrb ::hello3.hrb
  COMMAND mcopy -i ${27_day_harib24a_OS} ${root_BINARY_DIR}/projects/27_day/harib24a/hello4.hrb ::hello4.hrb
  COMMAND mcopy -i ${27_day_harib24a_OS} ${root_BINARY_DIR}/projects/27_day/harib24a/winhelo.hrb ::winhelo.hrb
  COMMAND mcopy -i ${27_day_harib24a_OS} ${root_BINARY_DIR}/projects/27_day/harib24a/winhelo2.hrb ::winhelo2.hrb
  COMMAND mcopy -i ${27_day_harib24a_OS} ${root_BINARY_DIR}/projects/27_day/harib24a/winhelo3.hrb ::winhelo3.hrb
  COMMAND mcopy -i ${27_day_harib24a_OS} ${root_BINARY_DIR}/projects/27_day/harib24a/star1.hrb ::star1.hrb
  COMMAND mcopy -i ${27_day_harib24a_OS} ${root_BINARY_DIR}/projects/27_day/harib24a/stars.hrb ::stars.hrb
  COMMAND mcopy -i ${27_day_harib24a_OS} ${root_BINARY_DIR}/projects/27_day/harib24a/stars2.hrb ::stars2.hrb
  COMMAND mcopy -i ${27_day_harib24a_OS} ${root_BINARY_DIR}/projects/27_day/harib24a/lines.hrb ::lines.hrb
  COMMAND mcopy -i ${27_day_harib24a_OS} ${root_BINARY_DIR}/projects/27_day/harib24a/walk.hrb ::walk.hrb
  COMMAND mcopy -i ${27_day_harib24a_OS} ${root_BINARY_DIR}/projects/27_day/harib24a/color2.hrb ::color2.hrb
  COMMAND mcopy -i ${27_day_harib24a_OS} ${root_BINARY_DIR}/projects/27_day/harib24a/color.hrb ::color.hrb
  COMMAND mcopy -i ${27_day_harib24a_OS} ${root_BINARY_DIR}/projects/27_day/harib24a/beepdown.hrb ::beepdown.hrb
  COMMAND mcopy -i ${27_day_harib24a_OS} ${root_BINARY_DIR}/projects/27_day/harib24a/noodle.hrb ::noodle.hrb
  COMMAND mcopy -i ${27_day_harib24a_OS} ${root_BINARY_DIR}/projects/27_day/harib24a/hello.hrb ::hello.hrb
  COMMAND mcopy -i ${27_day_harib24a_OS} ${root_BINARY_DIR}/projects/27_day/harib24a/hello2.hrb ::hello2.hrb
  COMMAND mcopy -i ${27_day_harib24a_OS} ${root_BINARY_DIR}/projects/27_day/harib24a/hello5.hrb ::hello5.hrb
  DEPENDS 27_day_harib24a_sys ${root_BINARY_DIR}/projects/27_day/harib24a/a.hrb ${root_BINARY_DIR}/projects/27_day/harib24a/hello3.hrb ${root_BINARY_DIR}/projects/27_day/harib24a/hello4.hrb ${root_BINARY_DIR}/projects/27_day/harib24a/winhelo.hrb ${root_BINARY_DIR}/projects/27_day/harib24a/winhelo2.hrb ${root_BINARY_DIR}/projects/27_day/harib24a/winhelo3.hrb ${root_BINARY_DIR}/projects/27_day/harib24a/star1.hrb ${root_BINARY_DIR}/projects/27_day/harib24a/stars.hrb ${root_BINARY_DIR}/projects/27_day/harib24a/stars2.hrb ${root_BINARY_DIR}/projects/27_day/harib24a/lines.hrb ${root_BINARY_DIR}/projects/27_day/harib24a/walk.hrb ${root_BINARY_DIR}/projects/27_day/harib24a/noodle.hrb ${root_BINARY_DIR}/projects/27_day/harib24a/beepdown.hrb ${root_BINARY_DIR}/projects/27_day/harib24a/color.hrb ${root_BINARY_DIR}/projects/27_day/harib24a/color2.hrb ${root_BINARY_DIR}/projects/27_day/harib24a/hello.hrb ${root_BINARY_DIR}/projects/27_day/harib24a/hello2.hrb ${root_BINARY_DIR}/projects/27_day/harib24a/hello5.hrb
)

#----------------------------------------------------------
########### application .hrb files (copied into os.img) ###############
set(27_day_harib24a_APIO ${root_BINARY_DIR}/projects/27_day/harib24a/a_nask.o)
set(27_day_harib24a_RANDO ${root_BINARY_DIR}/projects/27_day/harib24a/rand.o)
add_custom_command(
  OUTPUT ${27_day_harib24a_RANDO}
  COMMAND gcc -fleading-underscore -m32 -nostdlib -fno-pie -x c -c ${root_SOURCE_DIR}/golibc/rand.c_ -o ${27_day_harib24a_RANDO}
  DEPENDS ${root_SOURCE_DIR}/golibc/rand.c_
  COMMENT "Generating rand.o for 27_day_harib24a"
)
add_custom_command(
  OUTPUT ${27_day_harib24a_APIO}
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/27_day/harib24a/a_nask.nas ${27_day_harib24a_APIO}
  DEPENDS ${root_SOURCE_DIR}/projects/27_day/harib24a/a_nask.nas
  COMMENT "Generating a_nask.o for 27_day_harib24a"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/27_day/harib24a/a.hrb
  COMMAND gcc ${BINOPT} -T ${27_day_harib24a_APPLDS} ${root_SOURCE_DIR}/projects/27_day/harib24a/a.c ${27_day_harib24a_APIO} ${27_day_harib24a_LIBGC} ${27_day_harib24a_RANDO} -o ${root_BINARY_DIR}/projects/27_day/harib24a/a.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/27_day/harib24a/a.c ${root_SOURCE_DIR}/projects/app.lds ${27_day_harib24a_APIO} ${27_day_harib24a_LIBGC} ${27_day_harib24a_RANDO}
  COMMENT "Generating a.hrb for 27_day_harib24a"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/27_day/harib24a/hello3.hrb
  COMMAND gcc ${BINOPT} -T ${27_day_harib24a_APPLDS} ${root_SOURCE_DIR}/projects/27_day/harib24a/hello3.c ${27_day_harib24a_APIO} ${27_day_harib24a_LIBGC} ${27_day_harib24a_RANDO} -o ${root_BINARY_DIR}/projects/27_day/harib24a/hello3.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/27_day/harib24a/hello3.c ${root_SOURCE_DIR}/projects/app.lds ${27_day_harib24a_APIO} ${27_day_harib24a_LIBGC} ${27_day_harib24a_RANDO}
  COMMENT "Generating hello3.hrb for 27_day_harib24a"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/27_day/harib24a/hello4.hrb
  COMMAND gcc ${BINOPT} -T ${27_day_harib24a_APPLDS} ${root_SOURCE_DIR}/projects/27_day/harib24a/hello4.c ${27_day_harib24a_APIO} ${27_day_harib24a_LIBGC} ${27_day_harib24a_RANDO} -o ${root_BINARY_DIR}/projects/27_day/harib24a/hello4.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/27_day/harib24a/hello4.c ${root_SOURCE_DIR}/projects/app.lds ${27_day_harib24a_APIO} ${27_day_harib24a_LIBGC} ${27_day_harib24a_RANDO}
  COMMENT "Generating hello4.hrb for 27_day_harib24a"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/27_day/harib24a/winhelo.hrb
  COMMAND gcc ${BINOPT} -T ${27_day_harib24a_APPLDS} ${root_SOURCE_DIR}/projects/27_day/harib24a/winhelo.c ${27_day_harib24a_APIO} ${27_day_harib24a_LIBGC} ${27_day_harib24a_RANDO} -o ${root_BINARY_DIR}/projects/27_day/harib24a/winhelo.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/27_day/harib24a/winhelo.c ${root_SOURCE_DIR}/projects/app.lds ${27_day_harib24a_APIO} ${27_day_harib24a_LIBGC} ${27_day_harib24a_RANDO}
  COMMENT "Generating winhelo.hrb for 27_day_harib24a"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/27_day/harib24a/winhelo2.hrb
  COMMAND gcc ${BINOPT} -T ${27_day_harib24a_APPLDS} ${root_SOURCE_DIR}/projects/27_day/harib24a/winhelo2.c ${27_day_harib24a_APIO} ${27_day_harib24a_LIBGC} ${27_day_harib24a_RANDO} -o ${root_BINARY_DIR}/projects/27_day/harib24a/winhelo2.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/27_day/harib24a/winhelo2.c ${root_SOURCE_DIR}/projects/app.lds ${27_day_harib24a_APIO} ${27_day_harib24a_LIBGC} ${27_day_harib24a_RANDO}
  COMMENT "Generating winhelo2.hrb for 27_day_harib24a"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/27_day/harib24a/winhelo3.hrb
  COMMAND gcc ${BINOPT} -T ${27_day_harib24a_APPLDS} ${root_SOURCE_DIR}/projects/27_day/harib24a/winhelo3.c ${27_day_harib24a_APIO} ${27_day_harib24a_LIBGC} ${27_day_harib24a_RANDO} -o ${root_BINARY_DIR}/projects/27_day/harib24a/winhelo3.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/27_day/harib24a/winhelo3.c ${root_SOURCE_DIR}/projects/app.lds ${27_day_harib24a_APIO} ${27_day_harib24a_LIBGC} ${27_day_harib24a_RANDO}
  COMMENT "Generating winhelo3.hrb for 27_day_harib24a"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/27_day/harib24a/star1.hrb
  COMMAND gcc ${BINOPT} -T ${27_day_harib24a_APPLDS} ${root_SOURCE_DIR}/projects/27_day/harib24a/star1.c ${27_day_harib24a_APIO} ${27_day_harib24a_LIBGC} ${27_day_harib24a_RANDO} -o ${root_BINARY_DIR}/projects/27_day/harib24a/star1.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/27_day/harib24a/star1.c ${root_SOURCE_DIR}/projects/app.lds ${27_day_harib24a_APIO} ${27_day_harib24a_LIBGC} ${27_day_harib24a_RANDO}
  COMMENT "Generating star1.hrb for 27_day_harib24a"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/27_day/harib24a/stars.hrb
  COMMAND gcc ${BINOPT} -T ${27_day_harib24a_APPLDS} ${root_SOURCE_DIR}/projects/27_day/harib24a/stars.c ${27_day_harib24a_APIO} ${27_day_harib24a_LIBGC} ${27_day_harib24a_RANDO} -o ${root_BINARY_DIR}/projects/27_day/harib24a/stars.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/27_day/harib24a/stars.c ${root_SOURCE_DIR}/projects/app.lds ${27_day_harib24a_APIO} ${27_day_harib24a_LIBGC} ${27_day_harib24a_RANDO}
  COMMENT "Generating stars.hrb for 27_day_harib24a"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/27_day/harib24a/stars2.hrb
  COMMAND gcc ${BINOPT} -T ${27_day_harib24a_APPLDS} ${root_SOURCE_DIR}/projects/27_day/harib24a/stars2.c ${27_day_harib24a_APIO} ${27_day_harib24a_LIBGC} ${27_day_harib24a_RANDO} -o ${root_BINARY_DIR}/projects/27_day/harib24a/stars2.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/27_day/harib24a/stars2.c ${root_SOURCE_DIR}/projects/app.lds ${27_day_harib24a_APIO} ${27_day_harib24a_LIBGC} ${27_day_harib24a_RANDO}
  COMMENT "Generating stars2.hrb for 27_day_harib24a"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/27_day/harib24a/lines.hrb
  COMMAND gcc ${BINOPT} -T ${27_day_harib24a_APPLDS} ${root_SOURCE_DIR}/projects/27_day/harib24a/lines.c ${27_day_harib24a_APIO} ${27_day_harib24a_LIBGC} ${27_day_harib24a_RANDO} -o ${root_BINARY_DIR}/projects/27_day/harib24a/lines.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/27_day/harib24a/lines.c ${root_SOURCE_DIR}/projects/app.lds ${27_day_harib24a_APIO} ${27_day_harib24a_LIBGC} ${27_day_harib24a_RANDO}
  COMMENT "Generating lines.hrb for 27_day_harib24a"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/27_day/harib24a/walk.hrb
  COMMAND gcc ${BINOPT} -T ${27_day_harib24a_APPLDS} ${root_SOURCE_DIR}/projects/27_day/harib24a/walk.c ${27_day_harib24a_APIO} ${27_day_harib24a_LIBGC} ${27_day_harib24a_RANDO} -o ${root_BINARY_DIR}/projects/27_day/harib24a/walk.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/27_day/harib24a/walk.c ${root_SOURCE_DIR}/projects/app.lds ${27_day_harib24a_APIO} ${27_day_harib24a_LIBGC} ${27_day_harib24a_RANDO}
  COMMENT "Generating walk.hrb for 27_day_harib24a"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/27_day/harib24a/color2.hrb
  COMMAND gcc ${BINOPT} -T ${27_day_harib24a_APPLDS} ${root_SOURCE_DIR}/projects/27_day/harib24a/color2.c ${27_day_harib24a_APIO} ${27_day_harib24a_LIBGC} ${27_day_harib24a_RANDO} -o ${root_BINARY_DIR}/projects/27_day/harib24a/color2.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/27_day/harib24a/color2.c ${root_SOURCE_DIR}/projects/app.lds ${27_day_harib24a_APIO} ${27_day_harib24a_LIBGC} ${27_day_harib24a_RANDO}
  COMMENT "Generating color2.hrb for 27_day_harib24a"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/27_day/harib24a/color.hrb
  COMMAND gcc ${BINOPT} -T ${27_day_harib24a_APPLDS} ${root_SOURCE_DIR}/projects/27_day/harib24a/color.c ${27_day_harib24a_APIO} ${27_day_harib24a_LIBGC} ${27_day_harib24a_RANDO} -o ${root_BINARY_DIR}/projects/27_day/harib24a/color.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/27_day/harib24a/color.c ${root_SOURCE_DIR}/projects/app.lds ${27_day_harib24a_APIO} ${27_day_harib24a_LIBGC} ${27_day_harib24a_RANDO}
  COMMENT "Generating color.hrb for 27_day_harib24a"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/27_day/harib24a/beepdown.hrb
  COMMAND gcc ${BINOPT} -T ${27_day_harib24a_APPLDS} ${root_SOURCE_DIR}/projects/27_day/harib24a/beepdown.c ${27_day_harib24a_APIO} ${27_day_harib24a_LIBGC} ${27_day_harib24a_RANDO} -o ${root_BINARY_DIR}/projects/27_day/harib24a/beepdown.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/27_day/harib24a/beepdown.c ${root_SOURCE_DIR}/projects/app.lds ${27_day_harib24a_APIO} ${27_day_harib24a_LIBGC} ${27_day_harib24a_RANDO}
  COMMENT "Generating beepdown.hrb for 27_day_harib24a"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/27_day/harib24a/noodle.hrb
  COMMAND gcc ${BINOPT} -T ${27_day_harib24a_APPLDS} ${root_SOURCE_DIR}/projects/27_day/harib24a/noodle.c ${27_day_harib24a_APIO} ${27_day_harib24a_LIBGC} ${27_day_harib24a_RANDO} -o ${root_BINARY_DIR}/projects/27_day/harib24a/noodle.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/27_day/harib24a/noodle.c ${root_SOURCE_DIR}/projects/app.lds ${27_day_harib24a_APIO} ${27_day_harib24a_LIBGC} ${27_day_harib24a_RANDO}
  COMMENT "Generating noodle.hrb for 27_day_harib24a"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/27_day/harib24a/hello.hrb
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/27_day/harib24a/hello.nas ${root_BINARY_DIR}/projects/27_day/harib24a/hello.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/27_day/harib24a/hello.nas
  COMMENT "Generating hello.hrb for 27_day_harib24a"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/27_day/harib24a/hello2.hrb
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/27_day/harib24a/hello2.nas ${root_BINARY_DIR}/projects/27_day/harib24a/hello2.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/27_day/harib24a/hello2.nas
  COMMENT "Generating hello2.hrb for 27_day_harib24a"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/27_day/harib24a/hello5.hrb
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/27_day/harib24a/hello5.nas ${root_BINARY_DIR}/projects/27_day/harib24a/hello5.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/27_day/harib24a/hello5.nas
  COMMENT "Generating hello5.hrb for 27_day_harib24a"
)
