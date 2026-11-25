#----------------------------------------------------------
message(STATUS "Entering directory projects/26_day/harib23c/")

set(FONT ${root_BINARY_DIR}/src/makefont)
set(B2O  ${root_BINARY_DIR}/src/bin2obj)
set(CONV ${root_BINARY_DIR}/objconv/objconv)
set(26_day_harib23c_OS    ${root_BINARY_DIR}/projects/26_day/harib23c/os.img)
set(26_day_harib23c_SYS	  ${root_BINARY_DIR}/projects/26_day/harib23c/os.sys)
set(26_day_harib23c_IPLB  ${root_BINARY_DIR}/projects/26_day/harib23c/ipl.bin)
set(26_day_harib23c_IPLS  ${root_SOURCE_DIR}/projects/26_day/harib23c/ipl10.nas)
set(26_day_harib23c_HEADB ${root_BINARY_DIR}/projects/26_day/harib23c/asmhead.bin)
set(26_day_harib23c_HEADS ${root_SOURCE_DIR}/projects/26_day/harib23c/asmhead.nas)
set(26_day_harib23c_BOOTB ${root_BINARY_DIR}/projects/26_day/harib23c/boot.bin)
set(26_day_harib23c_CCO	  ${root_BINARY_DIR}/projects/26_day/harib23c/boot.o)
file(GLOB 26_day_harib23c_CCS ${root_SOURCE_DIR}/projects/26_day/harib23c/*.c)
list(REMOVE_ITEM 26_day_harib23c_CCS
  ${root_SOURCE_DIR}/projects/26_day/harib23c/a.c
  ${root_SOURCE_DIR}/projects/26_day/harib23c/hello3.c
  ${root_SOURCE_DIR}/projects/26_day/harib23c/hello4.c
  ${root_SOURCE_DIR}/projects/26_day/harib23c/winhelo.c
  ${root_SOURCE_DIR}/projects/26_day/harib23c/winhelo2.c
  ${root_SOURCE_DIR}/projects/26_day/harib23c/winhelo3.c
  ${root_SOURCE_DIR}/projects/26_day/harib23c/star1.c
  ${root_SOURCE_DIR}/projects/26_day/harib23c/stars.c
  ${root_SOURCE_DIR}/projects/26_day/harib23c/stars2.c
  ${root_SOURCE_DIR}/projects/26_day/harib23c/lines.c
  ${root_SOURCE_DIR}/projects/26_day/harib23c/walk.c
  ${root_SOURCE_DIR}/projects/26_day/harib23c/noodle.c
  ${root_SOURCE_DIR}/projects/26_day/harib23c/beepdown.c
  ${root_SOURCE_DIR}/projects/26_day/harib23c/color.c
  ${root_SOURCE_DIR}/projects/26_day/harib23c/color2.c)
set(26_day_harib23c_LDS	  ${root_SOURCE_DIR}/projects/os.lds)
set(26_day_harib23c_APPLDS  ${root_SOURCE_DIR}/projects/app.lds)
set(26_day_harib23c_FUNCS ${root_SOURCE_DIR}/projects/26_day/harib23c/naskfunc.nas)
set(26_day_harib23c_FUNCO ${root_BINARY_DIR}/projects/26_day/harib23c/naskfunc.o)
set(26_day_harib23c_FONTS ${root_SOURCE_DIR}/projects/26_day/harib23c/hankaku.txt)
set(26_day_harib23c_FONTB ${root_BINARY_DIR}/projects/26_day/harib23c/hankaku.bin)
set(26_day_harib23c_FONTO ${root_BINARY_DIR}/projects/26_day/harib23c/hankaku.o)
set(26_day_harib23c_LIBGE ${root_BINARY_DIR}/golibc/libgo.a)
set(26_day_harib23c_LIBGC ${root_BINARY_DIR}/projects/26_day/harib23c/libgo.o)
set(26_day_harib23c_WILDOBJ ${root_BINARY_DIR}/projects/26_day/harib23c/*.o)


add_custom_target(26_day_harib23c_run
  COMMAND ${QEMU} ${QEMUOPT} ${26_day_harib23c_OS}
  DEPENDS 26_day_harib23c_img
)
add_custom_target(26_day_harib23c_debug
  COMMAND ${QEMU} -s -S ${QEMUOPT} ${26_day_harib23c_OS} ${QEMU_DEBUG_OPT}
  DEPENDS 26_day_harib23c_img
)
add_custom_target(26_day_harib23c_clean
  COMMAND rm -f ${26_day_harib23c_OS}
  COMMAND rm -f ${26_day_harib23c_SYS}
  COMMAND rm -f ${26_day_harib23c_IPLB}
  COMMAND rm -f ${26_day_harib23c_HEADB}
  COMMAND rm -f ${26_day_harib23c_BOOTB}
  COMMAND rm -f ${26_day_harib23c_WILDOBJ}
)
add_custom_target(26_day_harib23c_ipl
  COMMAND ${RAKUSK_EXECUTABLE} ${26_day_harib23c_IPLS} ${26_day_harib23c_IPLB}
  DEPENDS ${26_day_harib23c_IPLS}
)
add_custom_command(
  OUTPUT ${26_day_harib23c_HEADB}
  COMMAND ${RAKUSK_EXECUTABLE} ${26_day_harib23c_HEADS} ${26_day_harib23c_HEADB}
  DEPENDS ${26_day_harib23c_HEADS}
  COMMENT "Generating asmhead.bin for 26_day_harib23c"
)
add_custom_command(
  OUTPUT ${26_day_harib23c_FUNCO}
  COMMAND ${RAKUSK_EXECUTABLE} ${26_day_harib23c_FUNCS} ${26_day_harib23c_FUNCO}
  DEPENDS ${26_day_harib23c_FUNCS}
  COMMENT "Generating naskfunc.o for 26_day_harib23c"
)
add_custom_command(
  OUTPUT ${26_day_harib23c_FONTO}
  COMMAND ${FONT} ${26_day_harib23c_FONTS} ${26_day_harib23c_FONTB}
  COMMAND ${B2O}  ${26_day_harib23c_FONTB} ${26_day_harib23c_FONTO} _hankaku
  DEPENDS ${26_day_harib23c_FONTS} makefont bin2obj
  COMMENT "Generating hankaku.o for 26_day_harib23c"
)
add_custom_command(
  OUTPUT ${26_day_harib23c_LIBGC}
  COMMAND ${CONV} -fcoff32 -nu ${26_day_harib23c_LIBGE} ${26_day_harib23c_LIBGC}
  DEPENDS ${26_day_harib23c_LIBGE}
  COMMENT "Converting golibc for 26_day_harib23c"
)

add_custom_target(26_day_harib23c_sys
  COMMAND gcc ${BINOPT} -T ${26_day_harib23c_LDS} ${26_day_harib23c_CCS} ${26_day_harib23c_WILDOBJ} -o ${26_day_harib23c_BOOTB}
  COMMAND cat ${26_day_harib23c_HEADB} ${26_day_harib23c_BOOTB} > ${26_day_harib23c_SYS}
  DEPENDS ${26_day_harib23c_HEADB} 26_day_harib23c_ipl
  DEPENDS ${26_day_harib23c_FUNCO}
  DEPENDS ${26_day_harib23c_FONTO} ${26_day_harib23c_LIBGC}
)
add_custom_target(26_day_harib23c_img
  COMMAND mformat -f 1440 -l HARIBOTEOS -N 0xffffffff -C -B ${26_day_harib23c_IPLB} -i ${26_day_harib23c_OS}
  COMMAND mcopy -i ${26_day_harib23c_OS} ${26_day_harib23c_SYS} ::
  COMMAND mcopy -i ${26_day_harib23c_OS} ${root_BINARY_DIR}/projects/26_day/harib23c/a.hrb ::a.hrb
  COMMAND mcopy -i ${26_day_harib23c_OS} ${root_BINARY_DIR}/projects/26_day/harib23c/hello3.hrb ::hello3.hrb
  COMMAND mcopy -i ${26_day_harib23c_OS} ${root_BINARY_DIR}/projects/26_day/harib23c/hello4.hrb ::hello4.hrb
  COMMAND mcopy -i ${26_day_harib23c_OS} ${root_BINARY_DIR}/projects/26_day/harib23c/winhelo.hrb ::winhelo.hrb
  COMMAND mcopy -i ${26_day_harib23c_OS} ${root_BINARY_DIR}/projects/26_day/harib23c/winhelo2.hrb ::winhelo2.hrb
  COMMAND mcopy -i ${26_day_harib23c_OS} ${root_BINARY_DIR}/projects/26_day/harib23c/winhelo3.hrb ::winhelo3.hrb
  COMMAND mcopy -i ${26_day_harib23c_OS} ${root_BINARY_DIR}/projects/26_day/harib23c/star1.hrb ::star1.hrb
  COMMAND mcopy -i ${26_day_harib23c_OS} ${root_BINARY_DIR}/projects/26_day/harib23c/stars.hrb ::stars.hrb
  COMMAND mcopy -i ${26_day_harib23c_OS} ${root_BINARY_DIR}/projects/26_day/harib23c/stars2.hrb ::stars2.hrb
  COMMAND mcopy -i ${26_day_harib23c_OS} ${root_BINARY_DIR}/projects/26_day/harib23c/lines.hrb ::lines.hrb
  COMMAND mcopy -i ${26_day_harib23c_OS} ${root_BINARY_DIR}/projects/26_day/harib23c/walk.hrb ::walk.hrb
  COMMAND mcopy -i ${26_day_harib23c_OS} ${root_BINARY_DIR}/projects/26_day/harib23c/color2.hrb ::color2.hrb
  COMMAND mcopy -i ${26_day_harib23c_OS} ${root_BINARY_DIR}/projects/26_day/harib23c/color.hrb ::color.hrb
  COMMAND mcopy -i ${26_day_harib23c_OS} ${root_BINARY_DIR}/projects/26_day/harib23c/beepdown.hrb ::beepdown.hrb
  COMMAND mcopy -i ${26_day_harib23c_OS} ${root_BINARY_DIR}/projects/26_day/harib23c/noodle.hrb ::noodle.hrb
  COMMAND mcopy -i ${26_day_harib23c_OS} ${root_BINARY_DIR}/projects/26_day/harib23c/hello.hrb ::hello.hrb
  COMMAND mcopy -i ${26_day_harib23c_OS} ${root_BINARY_DIR}/projects/26_day/harib23c/hello2.hrb ::hello2.hrb
  COMMAND mcopy -i ${26_day_harib23c_OS} ${root_BINARY_DIR}/projects/26_day/harib23c/hello5.hrb ::hello5.hrb
  DEPENDS 26_day_harib23c_sys ${root_BINARY_DIR}/projects/26_day/harib23c/a.hrb ${root_BINARY_DIR}/projects/26_day/harib23c/hello3.hrb ${root_BINARY_DIR}/projects/26_day/harib23c/hello4.hrb ${root_BINARY_DIR}/projects/26_day/harib23c/winhelo.hrb ${root_BINARY_DIR}/projects/26_day/harib23c/winhelo2.hrb ${root_BINARY_DIR}/projects/26_day/harib23c/winhelo3.hrb ${root_BINARY_DIR}/projects/26_day/harib23c/star1.hrb ${root_BINARY_DIR}/projects/26_day/harib23c/stars.hrb ${root_BINARY_DIR}/projects/26_day/harib23c/stars2.hrb ${root_BINARY_DIR}/projects/26_day/harib23c/lines.hrb ${root_BINARY_DIR}/projects/26_day/harib23c/walk.hrb ${root_BINARY_DIR}/projects/26_day/harib23c/noodle.hrb ${root_BINARY_DIR}/projects/26_day/harib23c/beepdown.hrb ${root_BINARY_DIR}/projects/26_day/harib23c/color.hrb ${root_BINARY_DIR}/projects/26_day/harib23c/color2.hrb ${root_BINARY_DIR}/projects/26_day/harib23c/hello.hrb ${root_BINARY_DIR}/projects/26_day/harib23c/hello2.hrb ${root_BINARY_DIR}/projects/26_day/harib23c/hello5.hrb
)

#----------------------------------------------------------
########### application .hrb files (copied into os.img) ###############
set(26_day_harib23c_APIO ${root_BINARY_DIR}/projects/26_day/harib23c/a_nask.o)
set(26_day_harib23c_RANDO ${root_BINARY_DIR}/projects/26_day/harib23c/rand.o)
add_custom_command(
  OUTPUT ${26_day_harib23c_RANDO}
  COMMAND gcc -fleading-underscore -m32 -nostdlib -fno-pie -x c -c ${root_SOURCE_DIR}/golibc/rand.c_ -o ${26_day_harib23c_RANDO}
  DEPENDS ${root_SOURCE_DIR}/golibc/rand.c_
  COMMENT "Generating rand.o for 26_day_harib23c"
)
add_custom_command(
  OUTPUT ${26_day_harib23c_APIO}
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/26_day/harib23c/a_nask.nas ${26_day_harib23c_APIO}
  DEPENDS ${root_SOURCE_DIR}/projects/26_day/harib23c/a_nask.nas
  COMMENT "Generating a_nask.o for 26_day_harib23c"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/26_day/harib23c/a.hrb
  COMMAND gcc ${BINOPT} -T ${26_day_harib23c_APPLDS} ${root_SOURCE_DIR}/projects/26_day/harib23c/a.c ${26_day_harib23c_APIO} ${26_day_harib23c_LIBGC} ${26_day_harib23c_RANDO} -o ${root_BINARY_DIR}/projects/26_day/harib23c/a.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/26_day/harib23c/a.c ${root_SOURCE_DIR}/projects/app.lds ${26_day_harib23c_APIO} ${26_day_harib23c_LIBGC} ${26_day_harib23c_RANDO}
  COMMENT "Generating a.hrb for 26_day_harib23c"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/26_day/harib23c/hello3.hrb
  COMMAND gcc ${BINOPT} -T ${26_day_harib23c_APPLDS} ${root_SOURCE_DIR}/projects/26_day/harib23c/hello3.c ${26_day_harib23c_APIO} ${26_day_harib23c_LIBGC} ${26_day_harib23c_RANDO} -o ${root_BINARY_DIR}/projects/26_day/harib23c/hello3.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/26_day/harib23c/hello3.c ${root_SOURCE_DIR}/projects/app.lds ${26_day_harib23c_APIO} ${26_day_harib23c_LIBGC} ${26_day_harib23c_RANDO}
  COMMENT "Generating hello3.hrb for 26_day_harib23c"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/26_day/harib23c/hello4.hrb
  COMMAND gcc ${BINOPT} -T ${26_day_harib23c_APPLDS} ${root_SOURCE_DIR}/projects/26_day/harib23c/hello4.c ${26_day_harib23c_APIO} ${26_day_harib23c_LIBGC} ${26_day_harib23c_RANDO} -o ${root_BINARY_DIR}/projects/26_day/harib23c/hello4.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/26_day/harib23c/hello4.c ${root_SOURCE_DIR}/projects/app.lds ${26_day_harib23c_APIO} ${26_day_harib23c_LIBGC} ${26_day_harib23c_RANDO}
  COMMENT "Generating hello4.hrb for 26_day_harib23c"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/26_day/harib23c/winhelo.hrb
  COMMAND gcc ${BINOPT} -T ${26_day_harib23c_APPLDS} ${root_SOURCE_DIR}/projects/26_day/harib23c/winhelo.c ${26_day_harib23c_APIO} ${26_day_harib23c_LIBGC} ${26_day_harib23c_RANDO} -o ${root_BINARY_DIR}/projects/26_day/harib23c/winhelo.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/26_day/harib23c/winhelo.c ${root_SOURCE_DIR}/projects/app.lds ${26_day_harib23c_APIO} ${26_day_harib23c_LIBGC} ${26_day_harib23c_RANDO}
  COMMENT "Generating winhelo.hrb for 26_day_harib23c"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/26_day/harib23c/winhelo2.hrb
  COMMAND gcc ${BINOPT} -T ${26_day_harib23c_APPLDS} ${root_SOURCE_DIR}/projects/26_day/harib23c/winhelo2.c ${26_day_harib23c_APIO} ${26_day_harib23c_LIBGC} ${26_day_harib23c_RANDO} -o ${root_BINARY_DIR}/projects/26_day/harib23c/winhelo2.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/26_day/harib23c/winhelo2.c ${root_SOURCE_DIR}/projects/app.lds ${26_day_harib23c_APIO} ${26_day_harib23c_LIBGC} ${26_day_harib23c_RANDO}
  COMMENT "Generating winhelo2.hrb for 26_day_harib23c"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/26_day/harib23c/winhelo3.hrb
  COMMAND gcc ${BINOPT} -T ${26_day_harib23c_APPLDS} ${root_SOURCE_DIR}/projects/26_day/harib23c/winhelo3.c ${26_day_harib23c_APIO} ${26_day_harib23c_LIBGC} ${26_day_harib23c_RANDO} -o ${root_BINARY_DIR}/projects/26_day/harib23c/winhelo3.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/26_day/harib23c/winhelo3.c ${root_SOURCE_DIR}/projects/app.lds ${26_day_harib23c_APIO} ${26_day_harib23c_LIBGC} ${26_day_harib23c_RANDO}
  COMMENT "Generating winhelo3.hrb for 26_day_harib23c"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/26_day/harib23c/star1.hrb
  COMMAND gcc ${BINOPT} -T ${26_day_harib23c_APPLDS} ${root_SOURCE_DIR}/projects/26_day/harib23c/star1.c ${26_day_harib23c_APIO} ${26_day_harib23c_LIBGC} ${26_day_harib23c_RANDO} -o ${root_BINARY_DIR}/projects/26_day/harib23c/star1.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/26_day/harib23c/star1.c ${root_SOURCE_DIR}/projects/app.lds ${26_day_harib23c_APIO} ${26_day_harib23c_LIBGC} ${26_day_harib23c_RANDO}
  COMMENT "Generating star1.hrb for 26_day_harib23c"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/26_day/harib23c/stars.hrb
  COMMAND gcc ${BINOPT} -T ${26_day_harib23c_APPLDS} ${root_SOURCE_DIR}/projects/26_day/harib23c/stars.c ${26_day_harib23c_APIO} ${26_day_harib23c_LIBGC} ${26_day_harib23c_RANDO} -o ${root_BINARY_DIR}/projects/26_day/harib23c/stars.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/26_day/harib23c/stars.c ${root_SOURCE_DIR}/projects/app.lds ${26_day_harib23c_APIO} ${26_day_harib23c_LIBGC} ${26_day_harib23c_RANDO}
  COMMENT "Generating stars.hrb for 26_day_harib23c"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/26_day/harib23c/stars2.hrb
  COMMAND gcc ${BINOPT} -T ${26_day_harib23c_APPLDS} ${root_SOURCE_DIR}/projects/26_day/harib23c/stars2.c ${26_day_harib23c_APIO} ${26_day_harib23c_LIBGC} ${26_day_harib23c_RANDO} -o ${root_BINARY_DIR}/projects/26_day/harib23c/stars2.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/26_day/harib23c/stars2.c ${root_SOURCE_DIR}/projects/app.lds ${26_day_harib23c_APIO} ${26_day_harib23c_LIBGC} ${26_day_harib23c_RANDO}
  COMMENT "Generating stars2.hrb for 26_day_harib23c"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/26_day/harib23c/lines.hrb
  COMMAND gcc ${BINOPT} -T ${26_day_harib23c_APPLDS} ${root_SOURCE_DIR}/projects/26_day/harib23c/lines.c ${26_day_harib23c_APIO} ${26_day_harib23c_LIBGC} ${26_day_harib23c_RANDO} -o ${root_BINARY_DIR}/projects/26_day/harib23c/lines.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/26_day/harib23c/lines.c ${root_SOURCE_DIR}/projects/app.lds ${26_day_harib23c_APIO} ${26_day_harib23c_LIBGC} ${26_day_harib23c_RANDO}
  COMMENT "Generating lines.hrb for 26_day_harib23c"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/26_day/harib23c/walk.hrb
  COMMAND gcc ${BINOPT} -T ${26_day_harib23c_APPLDS} ${root_SOURCE_DIR}/projects/26_day/harib23c/walk.c ${26_day_harib23c_APIO} ${26_day_harib23c_LIBGC} ${26_day_harib23c_RANDO} -o ${root_BINARY_DIR}/projects/26_day/harib23c/walk.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/26_day/harib23c/walk.c ${root_SOURCE_DIR}/projects/app.lds ${26_day_harib23c_APIO} ${26_day_harib23c_LIBGC} ${26_day_harib23c_RANDO}
  COMMENT "Generating walk.hrb for 26_day_harib23c"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/26_day/harib23c/color2.hrb
  COMMAND gcc ${BINOPT} -T ${26_day_harib23c_APPLDS} ${root_SOURCE_DIR}/projects/26_day/harib23c/color2.c ${26_day_harib23c_APIO} ${26_day_harib23c_LIBGC} ${26_day_harib23c_RANDO} -o ${root_BINARY_DIR}/projects/26_day/harib23c/color2.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/26_day/harib23c/color2.c ${root_SOURCE_DIR}/projects/app.lds ${26_day_harib23c_APIO} ${26_day_harib23c_LIBGC} ${26_day_harib23c_RANDO}
  COMMENT "Generating color2.hrb for 26_day_harib23c"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/26_day/harib23c/color.hrb
  COMMAND gcc ${BINOPT} -T ${26_day_harib23c_APPLDS} ${root_SOURCE_DIR}/projects/26_day/harib23c/color.c ${26_day_harib23c_APIO} ${26_day_harib23c_LIBGC} ${26_day_harib23c_RANDO} -o ${root_BINARY_DIR}/projects/26_day/harib23c/color.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/26_day/harib23c/color.c ${root_SOURCE_DIR}/projects/app.lds ${26_day_harib23c_APIO} ${26_day_harib23c_LIBGC} ${26_day_harib23c_RANDO}
  COMMENT "Generating color.hrb for 26_day_harib23c"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/26_day/harib23c/beepdown.hrb
  COMMAND gcc ${BINOPT} -T ${26_day_harib23c_APPLDS} ${root_SOURCE_DIR}/projects/26_day/harib23c/beepdown.c ${26_day_harib23c_APIO} ${26_day_harib23c_LIBGC} ${26_day_harib23c_RANDO} -o ${root_BINARY_DIR}/projects/26_day/harib23c/beepdown.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/26_day/harib23c/beepdown.c ${root_SOURCE_DIR}/projects/app.lds ${26_day_harib23c_APIO} ${26_day_harib23c_LIBGC} ${26_day_harib23c_RANDO}
  COMMENT "Generating beepdown.hrb for 26_day_harib23c"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/26_day/harib23c/noodle.hrb
  COMMAND gcc ${BINOPT} -T ${26_day_harib23c_APPLDS} ${root_SOURCE_DIR}/projects/26_day/harib23c/noodle.c ${26_day_harib23c_APIO} ${26_day_harib23c_LIBGC} ${26_day_harib23c_RANDO} -o ${root_BINARY_DIR}/projects/26_day/harib23c/noodle.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/26_day/harib23c/noodle.c ${root_SOURCE_DIR}/projects/app.lds ${26_day_harib23c_APIO} ${26_day_harib23c_LIBGC} ${26_day_harib23c_RANDO}
  COMMENT "Generating noodle.hrb for 26_day_harib23c"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/26_day/harib23c/hello.hrb
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/26_day/harib23c/hello.nas ${root_BINARY_DIR}/projects/26_day/harib23c/hello.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/26_day/harib23c/hello.nas
  COMMENT "Generating hello.hrb for 26_day_harib23c"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/26_day/harib23c/hello2.hrb
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/26_day/harib23c/hello2.nas ${root_BINARY_DIR}/projects/26_day/harib23c/hello2.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/26_day/harib23c/hello2.nas
  COMMENT "Generating hello2.hrb for 26_day_harib23c"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/26_day/harib23c/hello5.hrb
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/26_day/harib23c/hello5.nas ${root_BINARY_DIR}/projects/26_day/harib23c/hello5.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/26_day/harib23c/hello5.nas
  COMMENT "Generating hello5.hrb for 26_day_harib23c"
)
