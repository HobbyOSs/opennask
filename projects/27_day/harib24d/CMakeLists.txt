#----------------------------------------------------------
message(STATUS "Entering directory projects/27_day/harib24d/")

set(FONT ${root_BINARY_DIR}/src/makefont)
set(B2O  ${root_BINARY_DIR}/src/bin2obj)
set(CONV ${root_BINARY_DIR}/objconv/objconv)
set(27_day_harib24d_OS    ${root_BINARY_DIR}/projects/27_day/harib24d/os.img)
set(27_day_harib24d_SYS	  ${root_BINARY_DIR}/projects/27_day/harib24d/os.sys)
set(27_day_harib24d_IPLB  ${root_BINARY_DIR}/projects/27_day/harib24d/ipl.bin)
set(27_day_harib24d_IPLS  ${root_SOURCE_DIR}/projects/27_day/harib24d/ipl10.nas)
set(27_day_harib24d_HEADB ${root_BINARY_DIR}/projects/27_day/harib24d/asmhead.bin)
set(27_day_harib24d_HEADS ${root_SOURCE_DIR}/projects/27_day/harib24d/asmhead.nas)
set(27_day_harib24d_BOOTB ${root_BINARY_DIR}/projects/27_day/harib24d/boot.bin)
set(27_day_harib24d_CCO	  ${root_BINARY_DIR}/projects/27_day/harib24d/boot.o)
file(GLOB 27_day_harib24d_CCS ${root_SOURCE_DIR}/projects/27_day/harib24d/*.c)
list(REMOVE_ITEM 27_day_harib24d_CCS
  ${root_SOURCE_DIR}/projects/27_day/harib24d/a.c
  ${root_SOURCE_DIR}/projects/27_day/harib24d/hello3.c
  ${root_SOURCE_DIR}/projects/27_day/harib24d/hello4.c
  ${root_SOURCE_DIR}/projects/27_day/harib24d/winhelo.c
  ${root_SOURCE_DIR}/projects/27_day/harib24d/winhelo2.c
  ${root_SOURCE_DIR}/projects/27_day/harib24d/winhelo3.c
  ${root_SOURCE_DIR}/projects/27_day/harib24d/star1.c
  ${root_SOURCE_DIR}/projects/27_day/harib24d/stars.c
  ${root_SOURCE_DIR}/projects/27_day/harib24d/stars2.c
  ${root_SOURCE_DIR}/projects/27_day/harib24d/lines.c
  ${root_SOURCE_DIR}/projects/27_day/harib24d/walk.c
  ${root_SOURCE_DIR}/projects/27_day/harib24d/noodle.c
  ${root_SOURCE_DIR}/projects/27_day/harib24d/beepdown.c
  ${root_SOURCE_DIR}/projects/27_day/harib24d/color.c
  ${root_SOURCE_DIR}/projects/27_day/harib24d/color2.c)
set(27_day_harib24d_LDS	  ${root_SOURCE_DIR}/projects/os.lds)
set(27_day_harib24d_APPLDS  ${root_SOURCE_DIR}/projects/app.lds)
set(27_day_harib24d_FUNCS ${root_SOURCE_DIR}/projects/27_day/harib24d/naskfunc.nas)
set(27_day_harib24d_FUNCO ${root_BINARY_DIR}/projects/27_day/harib24d/naskfunc.o)
set(27_day_harib24d_FONTS ${root_SOURCE_DIR}/projects/27_day/harib24d/hankaku.txt)
set(27_day_harib24d_FONTB ${root_BINARY_DIR}/projects/27_day/harib24d/hankaku.bin)
set(27_day_harib24d_FONTO ${root_BINARY_DIR}/projects/27_day/harib24d/hankaku.o)
set(27_day_harib24d_LIBGE ${root_BINARY_DIR}/golibc/libgo.a)
set(27_day_harib24d_LIBGC ${root_BINARY_DIR}/projects/27_day/harib24d/libgo.o)
set(27_day_harib24d_WILDOBJ ${root_BINARY_DIR}/projects/27_day/harib24d/*.o)


add_custom_target(27_day_harib24d_run
  COMMAND ${QEMU} ${QEMUOPT} ${27_day_harib24d_OS}
  DEPENDS 27_day_harib24d_img
)
add_custom_target(27_day_harib24d_debug
  COMMAND ${QEMU} -s -S ${QEMUOPT} ${27_day_harib24d_OS} ${QEMU_DEBUG_OPT}
  DEPENDS 27_day_harib24d_img
)
add_custom_target(27_day_harib24d_clean
  COMMAND rm -f ${27_day_harib24d_OS}
  COMMAND rm -f ${27_day_harib24d_SYS}
  COMMAND rm -f ${27_day_harib24d_IPLB}
  COMMAND rm -f ${27_day_harib24d_HEADB}
  COMMAND rm -f ${27_day_harib24d_BOOTB}
  COMMAND rm -f ${27_day_harib24d_WILDOBJ}
)
add_custom_target(27_day_harib24d_ipl
  COMMAND ${RAKUSK_EXECUTABLE} ${27_day_harib24d_IPLS} ${27_day_harib24d_IPLB}
  DEPENDS ${27_day_harib24d_IPLS}
)
add_custom_command(
  OUTPUT ${27_day_harib24d_HEADB}
  COMMAND ${RAKUSK_EXECUTABLE} ${27_day_harib24d_HEADS} ${27_day_harib24d_HEADB}
  DEPENDS ${27_day_harib24d_HEADS}
  COMMENT "Generating asmhead.bin for 27_day_harib24d"
)
add_custom_command(
  OUTPUT ${27_day_harib24d_FUNCO}
  COMMAND ${RAKUSK_EXECUTABLE} ${27_day_harib24d_FUNCS} ${27_day_harib24d_FUNCO}
  DEPENDS ${27_day_harib24d_FUNCS}
  COMMENT "Generating naskfunc.o for 27_day_harib24d"
)
add_custom_command(
  OUTPUT ${27_day_harib24d_FONTO}
  COMMAND ${FONT} ${27_day_harib24d_FONTS} ${27_day_harib24d_FONTB}
  COMMAND ${B2O}  ${27_day_harib24d_FONTB} ${27_day_harib24d_FONTO} _hankaku
  DEPENDS ${27_day_harib24d_FONTS} makefont bin2obj
  COMMENT "Generating hankaku.o for 27_day_harib24d"
)
add_custom_command(
  OUTPUT ${27_day_harib24d_LIBGC}
  COMMAND ${CONV} -fcoff32 -nu ${27_day_harib24d_LIBGE} ${27_day_harib24d_LIBGC}
  DEPENDS ${27_day_harib24d_LIBGE}
  COMMENT "Converting golibc for 27_day_harib24d"
)

add_custom_target(27_day_harib24d_sys
  COMMAND gcc ${BINOPT} -T ${27_day_harib24d_LDS} ${27_day_harib24d_CCS} ${27_day_harib24d_WILDOBJ} -o ${27_day_harib24d_BOOTB}
  COMMAND cat ${27_day_harib24d_HEADB} ${27_day_harib24d_BOOTB} > ${27_day_harib24d_SYS}
  DEPENDS ${27_day_harib24d_HEADB} 27_day_harib24d_ipl
  DEPENDS ${27_day_harib24d_FUNCO}
  DEPENDS ${27_day_harib24d_FONTO} ${27_day_harib24d_LIBGC}
)
add_custom_target(27_day_harib24d_img
  COMMAND mformat -f 1440 -l HARIBOTEOS -N 0xffffffff -C -B ${27_day_harib24d_IPLB} -i ${27_day_harib24d_OS}
  COMMAND mcopy -i ${27_day_harib24d_OS} ${27_day_harib24d_SYS} ::
  COMMAND mcopy -i ${27_day_harib24d_OS} ${root_BINARY_DIR}/projects/27_day/harib24d/a.hrb ::a.hrb
  COMMAND mcopy -i ${27_day_harib24d_OS} ${root_BINARY_DIR}/projects/27_day/harib24d/hello3.hrb ::hello3.hrb
  COMMAND mcopy -i ${27_day_harib24d_OS} ${root_BINARY_DIR}/projects/27_day/harib24d/hello4.hrb ::hello4.hrb
  COMMAND mcopy -i ${27_day_harib24d_OS} ${root_BINARY_DIR}/projects/27_day/harib24d/winhelo.hrb ::winhelo.hrb
  COMMAND mcopy -i ${27_day_harib24d_OS} ${root_BINARY_DIR}/projects/27_day/harib24d/winhelo2.hrb ::winhelo2.hrb
  COMMAND mcopy -i ${27_day_harib24d_OS} ${root_BINARY_DIR}/projects/27_day/harib24d/winhelo3.hrb ::winhelo3.hrb
  COMMAND mcopy -i ${27_day_harib24d_OS} ${root_BINARY_DIR}/projects/27_day/harib24d/star1.hrb ::star1.hrb
  COMMAND mcopy -i ${27_day_harib24d_OS} ${root_BINARY_DIR}/projects/27_day/harib24d/stars.hrb ::stars.hrb
  COMMAND mcopy -i ${27_day_harib24d_OS} ${root_BINARY_DIR}/projects/27_day/harib24d/stars2.hrb ::stars2.hrb
  COMMAND mcopy -i ${27_day_harib24d_OS} ${root_BINARY_DIR}/projects/27_day/harib24d/lines.hrb ::lines.hrb
  COMMAND mcopy -i ${27_day_harib24d_OS} ${root_BINARY_DIR}/projects/27_day/harib24d/walk.hrb ::walk.hrb
  COMMAND mcopy -i ${27_day_harib24d_OS} ${root_BINARY_DIR}/projects/27_day/harib24d/color2.hrb ::color2.hrb
  COMMAND mcopy -i ${27_day_harib24d_OS} ${root_BINARY_DIR}/projects/27_day/harib24d/color.hrb ::color.hrb
  COMMAND mcopy -i ${27_day_harib24d_OS} ${root_BINARY_DIR}/projects/27_day/harib24d/beepdown.hrb ::beepdown.hrb
  COMMAND mcopy -i ${27_day_harib24d_OS} ${root_BINARY_DIR}/projects/27_day/harib24d/noodle.hrb ::noodle.hrb
  COMMAND mcopy -i ${27_day_harib24d_OS} ${root_BINARY_DIR}/projects/27_day/harib24d/hello.hrb ::hello.hrb
  COMMAND mcopy -i ${27_day_harib24d_OS} ${root_BINARY_DIR}/projects/27_day/harib24d/hello2.hrb ::hello2.hrb
  COMMAND mcopy -i ${27_day_harib24d_OS} ${root_BINARY_DIR}/projects/27_day/harib24d/hello5.hrb ::hello5.hrb
  COMMAND mcopy -i ${27_day_harib24d_OS} ${root_BINARY_DIR}/projects/27_day/harib24d/crack7.hrb ::crack7.hrb
  DEPENDS 27_day_harib24d_sys ${root_BINARY_DIR}/projects/27_day/harib24d/a.hrb ${root_BINARY_DIR}/projects/27_day/harib24d/hello3.hrb ${root_BINARY_DIR}/projects/27_day/harib24d/hello4.hrb ${root_BINARY_DIR}/projects/27_day/harib24d/winhelo.hrb ${root_BINARY_DIR}/projects/27_day/harib24d/winhelo2.hrb ${root_BINARY_DIR}/projects/27_day/harib24d/winhelo3.hrb ${root_BINARY_DIR}/projects/27_day/harib24d/star1.hrb ${root_BINARY_DIR}/projects/27_day/harib24d/stars.hrb ${root_BINARY_DIR}/projects/27_day/harib24d/stars2.hrb ${root_BINARY_DIR}/projects/27_day/harib24d/lines.hrb ${root_BINARY_DIR}/projects/27_day/harib24d/walk.hrb ${root_BINARY_DIR}/projects/27_day/harib24d/noodle.hrb ${root_BINARY_DIR}/projects/27_day/harib24d/beepdown.hrb ${root_BINARY_DIR}/projects/27_day/harib24d/color.hrb ${root_BINARY_DIR}/projects/27_day/harib24d/color2.hrb ${root_BINARY_DIR}/projects/27_day/harib24d/hello.hrb ${root_BINARY_DIR}/projects/27_day/harib24d/hello2.hrb ${root_BINARY_DIR}/projects/27_day/harib24d/hello5.hrb ${root_BINARY_DIR}/projects/27_day/harib24d/crack7.hrb
)

#----------------------------------------------------------
########### application .hrb files (copied into os.img) ###############
set(27_day_harib24d_APIO ${root_BINARY_DIR}/projects/27_day/harib24d/a_nask.o)
set(27_day_harib24d_RANDO ${root_BINARY_DIR}/projects/27_day/harib24d/rand.o)
add_custom_command(
  OUTPUT ${27_day_harib24d_RANDO}
  COMMAND gcc -fleading-underscore -m32 -nostdlib -fno-pie -x c -c ${root_SOURCE_DIR}/golibc/rand.c_ -o ${27_day_harib24d_RANDO}
  DEPENDS ${root_SOURCE_DIR}/golibc/rand.c_
  COMMENT "Generating rand.o for 27_day_harib24d"
)
add_custom_command(
  OUTPUT ${27_day_harib24d_APIO}
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/27_day/harib24d/a_nask.nas ${27_day_harib24d_APIO}
  DEPENDS ${root_SOURCE_DIR}/projects/27_day/harib24d/a_nask.nas
  COMMENT "Generating a_nask.o for 27_day_harib24d"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/27_day/harib24d/a.hrb
  COMMAND gcc ${BINOPT} -T ${27_day_harib24d_APPLDS} ${root_SOURCE_DIR}/projects/27_day/harib24d/a.c ${27_day_harib24d_APIO} ${27_day_harib24d_LIBGC} ${27_day_harib24d_RANDO} -o ${root_BINARY_DIR}/projects/27_day/harib24d/a.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/27_day/harib24d/a.c ${root_SOURCE_DIR}/projects/app.lds ${27_day_harib24d_APIO} ${27_day_harib24d_LIBGC} ${27_day_harib24d_RANDO}
  COMMENT "Generating a.hrb for 27_day_harib24d"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/27_day/harib24d/hello3.hrb
  COMMAND gcc ${BINOPT} -T ${27_day_harib24d_APPLDS} ${root_SOURCE_DIR}/projects/27_day/harib24d/hello3.c ${27_day_harib24d_APIO} ${27_day_harib24d_LIBGC} ${27_day_harib24d_RANDO} -o ${root_BINARY_DIR}/projects/27_day/harib24d/hello3.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/27_day/harib24d/hello3.c ${root_SOURCE_DIR}/projects/app.lds ${27_day_harib24d_APIO} ${27_day_harib24d_LIBGC} ${27_day_harib24d_RANDO}
  COMMENT "Generating hello3.hrb for 27_day_harib24d"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/27_day/harib24d/hello4.hrb
  COMMAND gcc ${BINOPT} -T ${27_day_harib24d_APPLDS} ${root_SOURCE_DIR}/projects/27_day/harib24d/hello4.c ${27_day_harib24d_APIO} ${27_day_harib24d_LIBGC} ${27_day_harib24d_RANDO} -o ${root_BINARY_DIR}/projects/27_day/harib24d/hello4.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/27_day/harib24d/hello4.c ${root_SOURCE_DIR}/projects/app.lds ${27_day_harib24d_APIO} ${27_day_harib24d_LIBGC} ${27_day_harib24d_RANDO}
  COMMENT "Generating hello4.hrb for 27_day_harib24d"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/27_day/harib24d/winhelo.hrb
  COMMAND gcc ${BINOPT} -T ${27_day_harib24d_APPLDS} ${root_SOURCE_DIR}/projects/27_day/harib24d/winhelo.c ${27_day_harib24d_APIO} ${27_day_harib24d_LIBGC} ${27_day_harib24d_RANDO} -o ${root_BINARY_DIR}/projects/27_day/harib24d/winhelo.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/27_day/harib24d/winhelo.c ${root_SOURCE_DIR}/projects/app.lds ${27_day_harib24d_APIO} ${27_day_harib24d_LIBGC} ${27_day_harib24d_RANDO}
  COMMENT "Generating winhelo.hrb for 27_day_harib24d"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/27_day/harib24d/winhelo2.hrb
  COMMAND gcc ${BINOPT} -T ${27_day_harib24d_APPLDS} ${root_SOURCE_DIR}/projects/27_day/harib24d/winhelo2.c ${27_day_harib24d_APIO} ${27_day_harib24d_LIBGC} ${27_day_harib24d_RANDO} -o ${root_BINARY_DIR}/projects/27_day/harib24d/winhelo2.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/27_day/harib24d/winhelo2.c ${root_SOURCE_DIR}/projects/app.lds ${27_day_harib24d_APIO} ${27_day_harib24d_LIBGC} ${27_day_harib24d_RANDO}
  COMMENT "Generating winhelo2.hrb for 27_day_harib24d"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/27_day/harib24d/winhelo3.hrb
  COMMAND gcc ${BINOPT} -T ${27_day_harib24d_APPLDS} ${root_SOURCE_DIR}/projects/27_day/harib24d/winhelo3.c ${27_day_harib24d_APIO} ${27_day_harib24d_LIBGC} ${27_day_harib24d_RANDO} -o ${root_BINARY_DIR}/projects/27_day/harib24d/winhelo3.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/27_day/harib24d/winhelo3.c ${root_SOURCE_DIR}/projects/app.lds ${27_day_harib24d_APIO} ${27_day_harib24d_LIBGC} ${27_day_harib24d_RANDO}
  COMMENT "Generating winhelo3.hrb for 27_day_harib24d"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/27_day/harib24d/star1.hrb
  COMMAND gcc ${BINOPT} -T ${27_day_harib24d_APPLDS} ${root_SOURCE_DIR}/projects/27_day/harib24d/star1.c ${27_day_harib24d_APIO} ${27_day_harib24d_LIBGC} ${27_day_harib24d_RANDO} -o ${root_BINARY_DIR}/projects/27_day/harib24d/star1.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/27_day/harib24d/star1.c ${root_SOURCE_DIR}/projects/app.lds ${27_day_harib24d_APIO} ${27_day_harib24d_LIBGC} ${27_day_harib24d_RANDO}
  COMMENT "Generating star1.hrb for 27_day_harib24d"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/27_day/harib24d/stars.hrb
  COMMAND gcc ${BINOPT} -T ${27_day_harib24d_APPLDS} ${root_SOURCE_DIR}/projects/27_day/harib24d/stars.c ${27_day_harib24d_APIO} ${27_day_harib24d_LIBGC} ${27_day_harib24d_RANDO} -o ${root_BINARY_DIR}/projects/27_day/harib24d/stars.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/27_day/harib24d/stars.c ${root_SOURCE_DIR}/projects/app.lds ${27_day_harib24d_APIO} ${27_day_harib24d_LIBGC} ${27_day_harib24d_RANDO}
  COMMENT "Generating stars.hrb for 27_day_harib24d"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/27_day/harib24d/stars2.hrb
  COMMAND gcc ${BINOPT} -T ${27_day_harib24d_APPLDS} ${root_SOURCE_DIR}/projects/27_day/harib24d/stars2.c ${27_day_harib24d_APIO} ${27_day_harib24d_LIBGC} ${27_day_harib24d_RANDO} -o ${root_BINARY_DIR}/projects/27_day/harib24d/stars2.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/27_day/harib24d/stars2.c ${root_SOURCE_DIR}/projects/app.lds ${27_day_harib24d_APIO} ${27_day_harib24d_LIBGC} ${27_day_harib24d_RANDO}
  COMMENT "Generating stars2.hrb for 27_day_harib24d"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/27_day/harib24d/lines.hrb
  COMMAND gcc ${BINOPT} -T ${27_day_harib24d_APPLDS} ${root_SOURCE_DIR}/projects/27_day/harib24d/lines.c ${27_day_harib24d_APIO} ${27_day_harib24d_LIBGC} ${27_day_harib24d_RANDO} -o ${root_BINARY_DIR}/projects/27_day/harib24d/lines.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/27_day/harib24d/lines.c ${root_SOURCE_DIR}/projects/app.lds ${27_day_harib24d_APIO} ${27_day_harib24d_LIBGC} ${27_day_harib24d_RANDO}
  COMMENT "Generating lines.hrb for 27_day_harib24d"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/27_day/harib24d/walk.hrb
  COMMAND gcc ${BINOPT} -T ${27_day_harib24d_APPLDS} ${root_SOURCE_DIR}/projects/27_day/harib24d/walk.c ${27_day_harib24d_APIO} ${27_day_harib24d_LIBGC} ${27_day_harib24d_RANDO} -o ${root_BINARY_DIR}/projects/27_day/harib24d/walk.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/27_day/harib24d/walk.c ${root_SOURCE_DIR}/projects/app.lds ${27_day_harib24d_APIO} ${27_day_harib24d_LIBGC} ${27_day_harib24d_RANDO}
  COMMENT "Generating walk.hrb for 27_day_harib24d"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/27_day/harib24d/color2.hrb
  COMMAND gcc ${BINOPT} -T ${27_day_harib24d_APPLDS} ${root_SOURCE_DIR}/projects/27_day/harib24d/color2.c ${27_day_harib24d_APIO} ${27_day_harib24d_LIBGC} ${27_day_harib24d_RANDO} -o ${root_BINARY_DIR}/projects/27_day/harib24d/color2.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/27_day/harib24d/color2.c ${root_SOURCE_DIR}/projects/app.lds ${27_day_harib24d_APIO} ${27_day_harib24d_LIBGC} ${27_day_harib24d_RANDO}
  COMMENT "Generating color2.hrb for 27_day_harib24d"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/27_day/harib24d/color.hrb
  COMMAND gcc ${BINOPT} -T ${27_day_harib24d_APPLDS} ${root_SOURCE_DIR}/projects/27_day/harib24d/color.c ${27_day_harib24d_APIO} ${27_day_harib24d_LIBGC} ${27_day_harib24d_RANDO} -o ${root_BINARY_DIR}/projects/27_day/harib24d/color.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/27_day/harib24d/color.c ${root_SOURCE_DIR}/projects/app.lds ${27_day_harib24d_APIO} ${27_day_harib24d_LIBGC} ${27_day_harib24d_RANDO}
  COMMENT "Generating color.hrb for 27_day_harib24d"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/27_day/harib24d/beepdown.hrb
  COMMAND gcc ${BINOPT} -T ${27_day_harib24d_APPLDS} ${root_SOURCE_DIR}/projects/27_day/harib24d/beepdown.c ${27_day_harib24d_APIO} ${27_day_harib24d_LIBGC} ${27_day_harib24d_RANDO} -o ${root_BINARY_DIR}/projects/27_day/harib24d/beepdown.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/27_day/harib24d/beepdown.c ${root_SOURCE_DIR}/projects/app.lds ${27_day_harib24d_APIO} ${27_day_harib24d_LIBGC} ${27_day_harib24d_RANDO}
  COMMENT "Generating beepdown.hrb for 27_day_harib24d"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/27_day/harib24d/noodle.hrb
  COMMAND gcc ${BINOPT} -T ${27_day_harib24d_APPLDS} ${root_SOURCE_DIR}/projects/27_day/harib24d/noodle.c ${27_day_harib24d_APIO} ${27_day_harib24d_LIBGC} ${27_day_harib24d_RANDO} -o ${root_BINARY_DIR}/projects/27_day/harib24d/noodle.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/27_day/harib24d/noodle.c ${root_SOURCE_DIR}/projects/app.lds ${27_day_harib24d_APIO} ${27_day_harib24d_LIBGC} ${27_day_harib24d_RANDO}
  COMMENT "Generating noodle.hrb for 27_day_harib24d"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/27_day/harib24d/hello.hrb
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/27_day/harib24d/hello.nas ${root_BINARY_DIR}/projects/27_day/harib24d/hello.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/27_day/harib24d/hello.nas
  COMMENT "Generating hello.hrb for 27_day_harib24d"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/27_day/harib24d/crack7.hrb
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/27_day/harib24d/crack7.nas ${root_BINARY_DIR}/projects/27_day/harib24d/crack7.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/27_day/harib24d/crack7.nas
  COMMENT "Generating crack7.hrb for 27_day_harib24d"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/27_day/harib24d/hello2.hrb
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/27_day/harib24d/hello2.nas ${root_BINARY_DIR}/projects/27_day/harib24d/hello2.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/27_day/harib24d/hello2.nas
  COMMENT "Generating hello2.hrb for 27_day_harib24d"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/27_day/harib24d/hello5.hrb
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/27_day/harib24d/hello5.nas ${root_BINARY_DIR}/projects/27_day/harib24d/hello5.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/27_day/harib24d/hello5.nas
  COMMENT "Generating hello5.hrb for 27_day_harib24d"
)
