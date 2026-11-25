#----------------------------------------------------------
message(STATUS "Entering directory projects/27_day/harib24c/")

set(FONT ${root_BINARY_DIR}/src/makefont)
set(B2O  ${root_BINARY_DIR}/src/bin2obj)
set(CONV ${root_BINARY_DIR}/objconv/objconv)
set(27_day_harib24c_OS    ${root_BINARY_DIR}/projects/27_day/harib24c/os.img)
set(27_day_harib24c_SYS	  ${root_BINARY_DIR}/projects/27_day/harib24c/os.sys)
set(27_day_harib24c_IPLB  ${root_BINARY_DIR}/projects/27_day/harib24c/ipl.bin)
set(27_day_harib24c_IPLS  ${root_SOURCE_DIR}/projects/27_day/harib24c/ipl10.nas)
set(27_day_harib24c_HEADB ${root_BINARY_DIR}/projects/27_day/harib24c/asmhead.bin)
set(27_day_harib24c_HEADS ${root_SOURCE_DIR}/projects/27_day/harib24c/asmhead.nas)
set(27_day_harib24c_BOOTB ${root_BINARY_DIR}/projects/27_day/harib24c/boot.bin)
set(27_day_harib24c_CCO	  ${root_BINARY_DIR}/projects/27_day/harib24c/boot.o)
file(GLOB 27_day_harib24c_CCS ${root_SOURCE_DIR}/projects/27_day/harib24c/*.c)
list(REMOVE_ITEM 27_day_harib24c_CCS
  ${root_SOURCE_DIR}/projects/27_day/harib24c/a.c
  ${root_SOURCE_DIR}/projects/27_day/harib24c/hello3.c
  ${root_SOURCE_DIR}/projects/27_day/harib24c/hello4.c
  ${root_SOURCE_DIR}/projects/27_day/harib24c/winhelo.c
  ${root_SOURCE_DIR}/projects/27_day/harib24c/winhelo2.c
  ${root_SOURCE_DIR}/projects/27_day/harib24c/winhelo3.c
  ${root_SOURCE_DIR}/projects/27_day/harib24c/star1.c
  ${root_SOURCE_DIR}/projects/27_day/harib24c/stars.c
  ${root_SOURCE_DIR}/projects/27_day/harib24c/stars2.c
  ${root_SOURCE_DIR}/projects/27_day/harib24c/lines.c
  ${root_SOURCE_DIR}/projects/27_day/harib24c/walk.c
  ${root_SOURCE_DIR}/projects/27_day/harib24c/noodle.c
  ${root_SOURCE_DIR}/projects/27_day/harib24c/beepdown.c
  ${root_SOURCE_DIR}/projects/27_day/harib24c/color.c
  ${root_SOURCE_DIR}/projects/27_day/harib24c/color2.c)
set(27_day_harib24c_LDS	  ${root_SOURCE_DIR}/projects/os.lds)
set(27_day_harib24c_APPLDS  ${root_SOURCE_DIR}/projects/app.lds)
set(27_day_harib24c_FUNCS ${root_SOURCE_DIR}/projects/27_day/harib24c/naskfunc.nas)
set(27_day_harib24c_FUNCO ${root_BINARY_DIR}/projects/27_day/harib24c/naskfunc.o)
set(27_day_harib24c_FONTS ${root_SOURCE_DIR}/projects/27_day/harib24c/hankaku.txt)
set(27_day_harib24c_FONTB ${root_BINARY_DIR}/projects/27_day/harib24c/hankaku.bin)
set(27_day_harib24c_FONTO ${root_BINARY_DIR}/projects/27_day/harib24c/hankaku.o)
set(27_day_harib24c_LIBGE ${root_BINARY_DIR}/golibc/libgo.a)
set(27_day_harib24c_LIBGC ${root_BINARY_DIR}/projects/27_day/harib24c/libgo.o)
set(27_day_harib24c_WILDOBJ ${root_BINARY_DIR}/projects/27_day/harib24c/*.o)


add_custom_target(27_day_harib24c_run
  COMMAND ${QEMU} ${QEMUOPT} ${27_day_harib24c_OS}
  DEPENDS 27_day_harib24c_img
)
add_custom_target(27_day_harib24c_debug
  COMMAND ${QEMU} -s -S ${QEMUOPT} ${27_day_harib24c_OS} ${QEMU_DEBUG_OPT}
  DEPENDS 27_day_harib24c_img
)
add_custom_target(27_day_harib24c_clean
  COMMAND rm -f ${27_day_harib24c_OS}
  COMMAND rm -f ${27_day_harib24c_SYS}
  COMMAND rm -f ${27_day_harib24c_IPLB}
  COMMAND rm -f ${27_day_harib24c_HEADB}
  COMMAND rm -f ${27_day_harib24c_BOOTB}
  COMMAND rm -f ${27_day_harib24c_WILDOBJ}
)
add_custom_target(27_day_harib24c_ipl
  COMMAND ${RAKUSK_EXECUTABLE} ${27_day_harib24c_IPLS} ${27_day_harib24c_IPLB}
  DEPENDS ${27_day_harib24c_IPLS}
)
add_custom_command(
  OUTPUT ${27_day_harib24c_HEADB}
  COMMAND ${RAKUSK_EXECUTABLE} ${27_day_harib24c_HEADS} ${27_day_harib24c_HEADB}
  DEPENDS ${27_day_harib24c_HEADS}
  COMMENT "Generating asmhead.bin for 27_day_harib24c"
)
add_custom_command(
  OUTPUT ${27_day_harib24c_FUNCO}
  COMMAND ${RAKUSK_EXECUTABLE} ${27_day_harib24c_FUNCS} ${27_day_harib24c_FUNCO}
  DEPENDS ${27_day_harib24c_FUNCS}
  COMMENT "Generating naskfunc.o for 27_day_harib24c"
)
add_custom_command(
  OUTPUT ${27_day_harib24c_FONTO}
  COMMAND ${FONT} ${27_day_harib24c_FONTS} ${27_day_harib24c_FONTB}
  COMMAND ${B2O}  ${27_day_harib24c_FONTB} ${27_day_harib24c_FONTO} _hankaku
  DEPENDS ${27_day_harib24c_FONTS} makefont bin2obj
  COMMENT "Generating hankaku.o for 27_day_harib24c"
)
add_custom_command(
  OUTPUT ${27_day_harib24c_LIBGC}
  COMMAND ${CONV} -fcoff32 -nu ${27_day_harib24c_LIBGE} ${27_day_harib24c_LIBGC}
  DEPENDS ${27_day_harib24c_LIBGE}
  COMMENT "Converting golibc for 27_day_harib24c"
)

add_custom_target(27_day_harib24c_sys
  COMMAND gcc ${BINOPT} -T ${27_day_harib24c_LDS} ${27_day_harib24c_CCS} ${27_day_harib24c_WILDOBJ} -o ${27_day_harib24c_BOOTB}
  COMMAND cat ${27_day_harib24c_HEADB} ${27_day_harib24c_BOOTB} > ${27_day_harib24c_SYS}
  DEPENDS ${27_day_harib24c_HEADB} 27_day_harib24c_ipl
  DEPENDS ${27_day_harib24c_FUNCO}
  DEPENDS ${27_day_harib24c_FONTO} ${27_day_harib24c_LIBGC}
)
add_custom_target(27_day_harib24c_img
  COMMAND mformat -f 1440 -l HARIBOTEOS -N 0xffffffff -C -B ${27_day_harib24c_IPLB} -i ${27_day_harib24c_OS}
  COMMAND mcopy -i ${27_day_harib24c_OS} ${27_day_harib24c_SYS} ::
  COMMAND mcopy -i ${27_day_harib24c_OS} ${root_BINARY_DIR}/projects/27_day/harib24c/a.hrb ::a.hrb
  COMMAND mcopy -i ${27_day_harib24c_OS} ${root_BINARY_DIR}/projects/27_day/harib24c/hello3.hrb ::hello3.hrb
  COMMAND mcopy -i ${27_day_harib24c_OS} ${root_BINARY_DIR}/projects/27_day/harib24c/hello4.hrb ::hello4.hrb
  COMMAND mcopy -i ${27_day_harib24c_OS} ${root_BINARY_DIR}/projects/27_day/harib24c/winhelo.hrb ::winhelo.hrb
  COMMAND mcopy -i ${27_day_harib24c_OS} ${root_BINARY_DIR}/projects/27_day/harib24c/winhelo2.hrb ::winhelo2.hrb
  COMMAND mcopy -i ${27_day_harib24c_OS} ${root_BINARY_DIR}/projects/27_day/harib24c/winhelo3.hrb ::winhelo3.hrb
  COMMAND mcopy -i ${27_day_harib24c_OS} ${root_BINARY_DIR}/projects/27_day/harib24c/star1.hrb ::star1.hrb
  COMMAND mcopy -i ${27_day_harib24c_OS} ${root_BINARY_DIR}/projects/27_day/harib24c/stars.hrb ::stars.hrb
  COMMAND mcopy -i ${27_day_harib24c_OS} ${root_BINARY_DIR}/projects/27_day/harib24c/stars2.hrb ::stars2.hrb
  COMMAND mcopy -i ${27_day_harib24c_OS} ${root_BINARY_DIR}/projects/27_day/harib24c/lines.hrb ::lines.hrb
  COMMAND mcopy -i ${27_day_harib24c_OS} ${root_BINARY_DIR}/projects/27_day/harib24c/walk.hrb ::walk.hrb
  COMMAND mcopy -i ${27_day_harib24c_OS} ${root_BINARY_DIR}/projects/27_day/harib24c/color2.hrb ::color2.hrb
  COMMAND mcopy -i ${27_day_harib24c_OS} ${root_BINARY_DIR}/projects/27_day/harib24c/color.hrb ::color.hrb
  COMMAND mcopy -i ${27_day_harib24c_OS} ${root_BINARY_DIR}/projects/27_day/harib24c/beepdown.hrb ::beepdown.hrb
  COMMAND mcopy -i ${27_day_harib24c_OS} ${root_BINARY_DIR}/projects/27_day/harib24c/noodle.hrb ::noodle.hrb
  COMMAND mcopy -i ${27_day_harib24c_OS} ${root_BINARY_DIR}/projects/27_day/harib24c/hello.hrb ::hello.hrb
  COMMAND mcopy -i ${27_day_harib24c_OS} ${root_BINARY_DIR}/projects/27_day/harib24c/hello2.hrb ::hello2.hrb
  COMMAND mcopy -i ${27_day_harib24c_OS} ${root_BINARY_DIR}/projects/27_day/harib24c/hello5.hrb ::hello5.hrb
  COMMAND mcopy -i ${27_day_harib24c_OS} ${root_BINARY_DIR}/projects/27_day/harib24c/crack7.hrb ::crack7.hrb
  DEPENDS 27_day_harib24c_sys ${root_BINARY_DIR}/projects/27_day/harib24c/a.hrb ${root_BINARY_DIR}/projects/27_day/harib24c/hello3.hrb ${root_BINARY_DIR}/projects/27_day/harib24c/hello4.hrb ${root_BINARY_DIR}/projects/27_day/harib24c/winhelo.hrb ${root_BINARY_DIR}/projects/27_day/harib24c/winhelo2.hrb ${root_BINARY_DIR}/projects/27_day/harib24c/winhelo3.hrb ${root_BINARY_DIR}/projects/27_day/harib24c/star1.hrb ${root_BINARY_DIR}/projects/27_day/harib24c/stars.hrb ${root_BINARY_DIR}/projects/27_day/harib24c/stars2.hrb ${root_BINARY_DIR}/projects/27_day/harib24c/lines.hrb ${root_BINARY_DIR}/projects/27_day/harib24c/walk.hrb ${root_BINARY_DIR}/projects/27_day/harib24c/noodle.hrb ${root_BINARY_DIR}/projects/27_day/harib24c/beepdown.hrb ${root_BINARY_DIR}/projects/27_day/harib24c/color.hrb ${root_BINARY_DIR}/projects/27_day/harib24c/color2.hrb ${root_BINARY_DIR}/projects/27_day/harib24c/hello.hrb ${root_BINARY_DIR}/projects/27_day/harib24c/hello2.hrb ${root_BINARY_DIR}/projects/27_day/harib24c/hello5.hrb ${root_BINARY_DIR}/projects/27_day/harib24c/crack7.hrb
)

#----------------------------------------------------------
########### application .hrb files (copied into os.img) ###############
set(27_day_harib24c_APIO ${root_BINARY_DIR}/projects/27_day/harib24c/a_nask.o)
set(27_day_harib24c_RANDO ${root_BINARY_DIR}/projects/27_day/harib24c/rand.o)
add_custom_command(
  OUTPUT ${27_day_harib24c_RANDO}
  COMMAND gcc -fleading-underscore -m32 -nostdlib -fno-pie -x c -c ${root_SOURCE_DIR}/golibc/rand.c_ -o ${27_day_harib24c_RANDO}
  DEPENDS ${root_SOURCE_DIR}/golibc/rand.c_
  COMMENT "Generating rand.o for 27_day_harib24c"
)
add_custom_command(
  OUTPUT ${27_day_harib24c_APIO}
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/27_day/harib24c/a_nask.nas ${27_day_harib24c_APIO}
  DEPENDS ${root_SOURCE_DIR}/projects/27_day/harib24c/a_nask.nas
  COMMENT "Generating a_nask.o for 27_day_harib24c"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/27_day/harib24c/a.hrb
  COMMAND gcc ${BINOPT} -T ${27_day_harib24c_APPLDS} ${root_SOURCE_DIR}/projects/27_day/harib24c/a.c ${27_day_harib24c_APIO} ${27_day_harib24c_LIBGC} ${27_day_harib24c_RANDO} -o ${root_BINARY_DIR}/projects/27_day/harib24c/a.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/27_day/harib24c/a.c ${root_SOURCE_DIR}/projects/app.lds ${27_day_harib24c_APIO} ${27_day_harib24c_LIBGC} ${27_day_harib24c_RANDO}
  COMMENT "Generating a.hrb for 27_day_harib24c"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/27_day/harib24c/hello3.hrb
  COMMAND gcc ${BINOPT} -T ${27_day_harib24c_APPLDS} ${root_SOURCE_DIR}/projects/27_day/harib24c/hello3.c ${27_day_harib24c_APIO} ${27_day_harib24c_LIBGC} ${27_day_harib24c_RANDO} -o ${root_BINARY_DIR}/projects/27_day/harib24c/hello3.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/27_day/harib24c/hello3.c ${root_SOURCE_DIR}/projects/app.lds ${27_day_harib24c_APIO} ${27_day_harib24c_LIBGC} ${27_day_harib24c_RANDO}
  COMMENT "Generating hello3.hrb for 27_day_harib24c"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/27_day/harib24c/hello4.hrb
  COMMAND gcc ${BINOPT} -T ${27_day_harib24c_APPLDS} ${root_SOURCE_DIR}/projects/27_day/harib24c/hello4.c ${27_day_harib24c_APIO} ${27_day_harib24c_LIBGC} ${27_day_harib24c_RANDO} -o ${root_BINARY_DIR}/projects/27_day/harib24c/hello4.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/27_day/harib24c/hello4.c ${root_SOURCE_DIR}/projects/app.lds ${27_day_harib24c_APIO} ${27_day_harib24c_LIBGC} ${27_day_harib24c_RANDO}
  COMMENT "Generating hello4.hrb for 27_day_harib24c"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/27_day/harib24c/winhelo.hrb
  COMMAND gcc ${BINOPT} -T ${27_day_harib24c_APPLDS} ${root_SOURCE_DIR}/projects/27_day/harib24c/winhelo.c ${27_day_harib24c_APIO} ${27_day_harib24c_LIBGC} ${27_day_harib24c_RANDO} -o ${root_BINARY_DIR}/projects/27_day/harib24c/winhelo.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/27_day/harib24c/winhelo.c ${root_SOURCE_DIR}/projects/app.lds ${27_day_harib24c_APIO} ${27_day_harib24c_LIBGC} ${27_day_harib24c_RANDO}
  COMMENT "Generating winhelo.hrb for 27_day_harib24c"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/27_day/harib24c/winhelo2.hrb
  COMMAND gcc ${BINOPT} -T ${27_day_harib24c_APPLDS} ${root_SOURCE_DIR}/projects/27_day/harib24c/winhelo2.c ${27_day_harib24c_APIO} ${27_day_harib24c_LIBGC} ${27_day_harib24c_RANDO} -o ${root_BINARY_DIR}/projects/27_day/harib24c/winhelo2.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/27_day/harib24c/winhelo2.c ${root_SOURCE_DIR}/projects/app.lds ${27_day_harib24c_APIO} ${27_day_harib24c_LIBGC} ${27_day_harib24c_RANDO}
  COMMENT "Generating winhelo2.hrb for 27_day_harib24c"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/27_day/harib24c/winhelo3.hrb
  COMMAND gcc ${BINOPT} -T ${27_day_harib24c_APPLDS} ${root_SOURCE_DIR}/projects/27_day/harib24c/winhelo3.c ${27_day_harib24c_APIO} ${27_day_harib24c_LIBGC} ${27_day_harib24c_RANDO} -o ${root_BINARY_DIR}/projects/27_day/harib24c/winhelo3.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/27_day/harib24c/winhelo3.c ${root_SOURCE_DIR}/projects/app.lds ${27_day_harib24c_APIO} ${27_day_harib24c_LIBGC} ${27_day_harib24c_RANDO}
  COMMENT "Generating winhelo3.hrb for 27_day_harib24c"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/27_day/harib24c/star1.hrb
  COMMAND gcc ${BINOPT} -T ${27_day_harib24c_APPLDS} ${root_SOURCE_DIR}/projects/27_day/harib24c/star1.c ${27_day_harib24c_APIO} ${27_day_harib24c_LIBGC} ${27_day_harib24c_RANDO} -o ${root_BINARY_DIR}/projects/27_day/harib24c/star1.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/27_day/harib24c/star1.c ${root_SOURCE_DIR}/projects/app.lds ${27_day_harib24c_APIO} ${27_day_harib24c_LIBGC} ${27_day_harib24c_RANDO}
  COMMENT "Generating star1.hrb for 27_day_harib24c"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/27_day/harib24c/stars.hrb
  COMMAND gcc ${BINOPT} -T ${27_day_harib24c_APPLDS} ${root_SOURCE_DIR}/projects/27_day/harib24c/stars.c ${27_day_harib24c_APIO} ${27_day_harib24c_LIBGC} ${27_day_harib24c_RANDO} -o ${root_BINARY_DIR}/projects/27_day/harib24c/stars.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/27_day/harib24c/stars.c ${root_SOURCE_DIR}/projects/app.lds ${27_day_harib24c_APIO} ${27_day_harib24c_LIBGC} ${27_day_harib24c_RANDO}
  COMMENT "Generating stars.hrb for 27_day_harib24c"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/27_day/harib24c/stars2.hrb
  COMMAND gcc ${BINOPT} -T ${27_day_harib24c_APPLDS} ${root_SOURCE_DIR}/projects/27_day/harib24c/stars2.c ${27_day_harib24c_APIO} ${27_day_harib24c_LIBGC} ${27_day_harib24c_RANDO} -o ${root_BINARY_DIR}/projects/27_day/harib24c/stars2.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/27_day/harib24c/stars2.c ${root_SOURCE_DIR}/projects/app.lds ${27_day_harib24c_APIO} ${27_day_harib24c_LIBGC} ${27_day_harib24c_RANDO}
  COMMENT "Generating stars2.hrb for 27_day_harib24c"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/27_day/harib24c/lines.hrb
  COMMAND gcc ${BINOPT} -T ${27_day_harib24c_APPLDS} ${root_SOURCE_DIR}/projects/27_day/harib24c/lines.c ${27_day_harib24c_APIO} ${27_day_harib24c_LIBGC} ${27_day_harib24c_RANDO} -o ${root_BINARY_DIR}/projects/27_day/harib24c/lines.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/27_day/harib24c/lines.c ${root_SOURCE_DIR}/projects/app.lds ${27_day_harib24c_APIO} ${27_day_harib24c_LIBGC} ${27_day_harib24c_RANDO}
  COMMENT "Generating lines.hrb for 27_day_harib24c"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/27_day/harib24c/walk.hrb
  COMMAND gcc ${BINOPT} -T ${27_day_harib24c_APPLDS} ${root_SOURCE_DIR}/projects/27_day/harib24c/walk.c ${27_day_harib24c_APIO} ${27_day_harib24c_LIBGC} ${27_day_harib24c_RANDO} -o ${root_BINARY_DIR}/projects/27_day/harib24c/walk.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/27_day/harib24c/walk.c ${root_SOURCE_DIR}/projects/app.lds ${27_day_harib24c_APIO} ${27_day_harib24c_LIBGC} ${27_day_harib24c_RANDO}
  COMMENT "Generating walk.hrb for 27_day_harib24c"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/27_day/harib24c/color2.hrb
  COMMAND gcc ${BINOPT} -T ${27_day_harib24c_APPLDS} ${root_SOURCE_DIR}/projects/27_day/harib24c/color2.c ${27_day_harib24c_APIO} ${27_day_harib24c_LIBGC} ${27_day_harib24c_RANDO} -o ${root_BINARY_DIR}/projects/27_day/harib24c/color2.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/27_day/harib24c/color2.c ${root_SOURCE_DIR}/projects/app.lds ${27_day_harib24c_APIO} ${27_day_harib24c_LIBGC} ${27_day_harib24c_RANDO}
  COMMENT "Generating color2.hrb for 27_day_harib24c"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/27_day/harib24c/color.hrb
  COMMAND gcc ${BINOPT} -T ${27_day_harib24c_APPLDS} ${root_SOURCE_DIR}/projects/27_day/harib24c/color.c ${27_day_harib24c_APIO} ${27_day_harib24c_LIBGC} ${27_day_harib24c_RANDO} -o ${root_BINARY_DIR}/projects/27_day/harib24c/color.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/27_day/harib24c/color.c ${root_SOURCE_DIR}/projects/app.lds ${27_day_harib24c_APIO} ${27_day_harib24c_LIBGC} ${27_day_harib24c_RANDO}
  COMMENT "Generating color.hrb for 27_day_harib24c"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/27_day/harib24c/beepdown.hrb
  COMMAND gcc ${BINOPT} -T ${27_day_harib24c_APPLDS} ${root_SOURCE_DIR}/projects/27_day/harib24c/beepdown.c ${27_day_harib24c_APIO} ${27_day_harib24c_LIBGC} ${27_day_harib24c_RANDO} -o ${root_BINARY_DIR}/projects/27_day/harib24c/beepdown.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/27_day/harib24c/beepdown.c ${root_SOURCE_DIR}/projects/app.lds ${27_day_harib24c_APIO} ${27_day_harib24c_LIBGC} ${27_day_harib24c_RANDO}
  COMMENT "Generating beepdown.hrb for 27_day_harib24c"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/27_day/harib24c/noodle.hrb
  COMMAND gcc ${BINOPT} -T ${27_day_harib24c_APPLDS} ${root_SOURCE_DIR}/projects/27_day/harib24c/noodle.c ${27_day_harib24c_APIO} ${27_day_harib24c_LIBGC} ${27_day_harib24c_RANDO} -o ${root_BINARY_DIR}/projects/27_day/harib24c/noodle.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/27_day/harib24c/noodle.c ${root_SOURCE_DIR}/projects/app.lds ${27_day_harib24c_APIO} ${27_day_harib24c_LIBGC} ${27_day_harib24c_RANDO}
  COMMENT "Generating noodle.hrb for 27_day_harib24c"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/27_day/harib24c/hello.hrb
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/27_day/harib24c/hello.nas ${root_BINARY_DIR}/projects/27_day/harib24c/hello.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/27_day/harib24c/hello.nas
  COMMENT "Generating hello.hrb for 27_day_harib24c"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/27_day/harib24c/crack7.hrb
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/27_day/harib24c/crack7.nas ${root_BINARY_DIR}/projects/27_day/harib24c/crack7.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/27_day/harib24c/crack7.nas
  COMMENT "Generating crack7.hrb for 27_day_harib24c"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/27_day/harib24c/hello2.hrb
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/27_day/harib24c/hello2.nas ${root_BINARY_DIR}/projects/27_day/harib24c/hello2.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/27_day/harib24c/hello2.nas
  COMMENT "Generating hello2.hrb for 27_day_harib24c"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/27_day/harib24c/hello5.hrb
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/27_day/harib24c/hello5.nas ${root_BINARY_DIR}/projects/27_day/harib24c/hello5.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/27_day/harib24c/hello5.nas
  COMMENT "Generating hello5.hrb for 27_day_harib24c"
)
