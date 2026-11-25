#----------------------------------------------------------
message(STATUS "Entering directory projects/26_day/harib23a/")

set(FONT ${root_BINARY_DIR}/src/makefont)
set(B2O  ${root_BINARY_DIR}/src/bin2obj)
set(CONV ${root_BINARY_DIR}/objconv/objconv)
set(26_day_harib23a_OS    ${root_BINARY_DIR}/projects/26_day/harib23a/os.img)
set(26_day_harib23a_SYS	  ${root_BINARY_DIR}/projects/26_day/harib23a/os.sys)
set(26_day_harib23a_IPLB  ${root_BINARY_DIR}/projects/26_day/harib23a/ipl.bin)
set(26_day_harib23a_IPLS  ${root_SOURCE_DIR}/projects/26_day/harib23a/ipl10.nas)
set(26_day_harib23a_HEADB ${root_BINARY_DIR}/projects/26_day/harib23a/asmhead.bin)
set(26_day_harib23a_HEADS ${root_SOURCE_DIR}/projects/26_day/harib23a/asmhead.nas)
set(26_day_harib23a_BOOTB ${root_BINARY_DIR}/projects/26_day/harib23a/boot.bin)
set(26_day_harib23a_CCO	  ${root_BINARY_DIR}/projects/26_day/harib23a/boot.o)
file(GLOB 26_day_harib23a_CCS ${root_SOURCE_DIR}/projects/26_day/harib23a/*.c)
list(REMOVE_ITEM 26_day_harib23a_CCS
  ${root_SOURCE_DIR}/projects/26_day/harib23a/a.c
  ${root_SOURCE_DIR}/projects/26_day/harib23a/hello3.c
  ${root_SOURCE_DIR}/projects/26_day/harib23a/hello4.c
  ${root_SOURCE_DIR}/projects/26_day/harib23a/winhelo.c
  ${root_SOURCE_DIR}/projects/26_day/harib23a/winhelo2.c
  ${root_SOURCE_DIR}/projects/26_day/harib23a/winhelo3.c
  ${root_SOURCE_DIR}/projects/26_day/harib23a/star1.c
  ${root_SOURCE_DIR}/projects/26_day/harib23a/stars.c
  ${root_SOURCE_DIR}/projects/26_day/harib23a/stars2.c
  ${root_SOURCE_DIR}/projects/26_day/harib23a/lines.c
  ${root_SOURCE_DIR}/projects/26_day/harib23a/walk.c
  ${root_SOURCE_DIR}/projects/26_day/harib23a/noodle.c
  ${root_SOURCE_DIR}/projects/26_day/harib23a/beepdown.c
  ${root_SOURCE_DIR}/projects/26_day/harib23a/color.c
  ${root_SOURCE_DIR}/projects/26_day/harib23a/color2.c)
set(26_day_harib23a_LDS	  ${root_SOURCE_DIR}/projects/os.lds)
set(26_day_harib23a_APPLDS  ${root_SOURCE_DIR}/projects/app.lds)
set(26_day_harib23a_FUNCS ${root_SOURCE_DIR}/projects/26_day/harib23a/naskfunc.nas)
set(26_day_harib23a_FUNCO ${root_BINARY_DIR}/projects/26_day/harib23a/naskfunc.o)
set(26_day_harib23a_FONTS ${root_SOURCE_DIR}/projects/26_day/harib23a/hankaku.txt)
set(26_day_harib23a_FONTB ${root_BINARY_DIR}/projects/26_day/harib23a/hankaku.bin)
set(26_day_harib23a_FONTO ${root_BINARY_DIR}/projects/26_day/harib23a/hankaku.o)
set(26_day_harib23a_LIBGE ${root_BINARY_DIR}/golibc/libgo.a)
set(26_day_harib23a_LIBGC ${root_BINARY_DIR}/projects/26_day/harib23a/libgo.o)
set(26_day_harib23a_WILDOBJ ${root_BINARY_DIR}/projects/26_day/harib23a/*.o)


add_custom_target(26_day_harib23a_run
  COMMAND ${QEMU} ${QEMUOPT} ${26_day_harib23a_OS}
  DEPENDS 26_day_harib23a_img
)
add_custom_target(26_day_harib23a_debug
  COMMAND ${QEMU} -s -S ${QEMUOPT} ${26_day_harib23a_OS} ${QEMU_DEBUG_OPT}
  DEPENDS 26_day_harib23a_img
)
add_custom_target(26_day_harib23a_clean
  COMMAND rm -f ${26_day_harib23a_OS}
  COMMAND rm -f ${26_day_harib23a_SYS}
  COMMAND rm -f ${26_day_harib23a_IPLB}
  COMMAND rm -f ${26_day_harib23a_HEADB}
  COMMAND rm -f ${26_day_harib23a_BOOTB}
  COMMAND rm -f ${26_day_harib23a_WILDOBJ}
)
add_custom_target(26_day_harib23a_ipl
  COMMAND ${RAKUSK_EXECUTABLE} ${26_day_harib23a_IPLS} ${26_day_harib23a_IPLB}
  DEPENDS ${26_day_harib23a_IPLS}
)
add_custom_command(
  OUTPUT ${26_day_harib23a_HEADB}
  COMMAND ${RAKUSK_EXECUTABLE} ${26_day_harib23a_HEADS} ${26_day_harib23a_HEADB}
  DEPENDS ${26_day_harib23a_HEADS}
  COMMENT "Generating asmhead.bin for 26_day_harib23a"
)
add_custom_command(
  OUTPUT ${26_day_harib23a_FUNCO}
  COMMAND ${RAKUSK_EXECUTABLE} ${26_day_harib23a_FUNCS} ${26_day_harib23a_FUNCO}
  DEPENDS ${26_day_harib23a_FUNCS}
  COMMENT "Generating naskfunc.o for 26_day_harib23a"
)
add_custom_command(
  OUTPUT ${26_day_harib23a_FONTO}
  COMMAND ${FONT} ${26_day_harib23a_FONTS} ${26_day_harib23a_FONTB}
  COMMAND ${B2O}  ${26_day_harib23a_FONTB} ${26_day_harib23a_FONTO} _hankaku
  DEPENDS ${26_day_harib23a_FONTS} makefont bin2obj
  COMMENT "Generating hankaku.o for 26_day_harib23a"
)
add_custom_command(
  OUTPUT ${26_day_harib23a_LIBGC}
  COMMAND ${CONV} -fcoff32 -nu ${26_day_harib23a_LIBGE} ${26_day_harib23a_LIBGC}
  DEPENDS ${26_day_harib23a_LIBGE}
  COMMENT "Converting golibc for 26_day_harib23a"
)

add_custom_target(26_day_harib23a_sys
  COMMAND gcc ${BINOPT} -T ${26_day_harib23a_LDS} ${26_day_harib23a_CCS} ${26_day_harib23a_WILDOBJ} -o ${26_day_harib23a_BOOTB}
  COMMAND cat ${26_day_harib23a_HEADB} ${26_day_harib23a_BOOTB} > ${26_day_harib23a_SYS}
  DEPENDS ${26_day_harib23a_HEADB} 26_day_harib23a_ipl
  DEPENDS ${26_day_harib23a_FUNCO}
  DEPENDS ${26_day_harib23a_FONTO} ${26_day_harib23a_LIBGC}
)
add_custom_target(26_day_harib23a_img
  COMMAND mformat -f 1440 -l HARIBOTEOS -N 0xffffffff -C -B ${26_day_harib23a_IPLB} -i ${26_day_harib23a_OS}
  COMMAND mcopy -i ${26_day_harib23a_OS} ${26_day_harib23a_SYS} ::
  COMMAND mcopy -i ${26_day_harib23a_OS} ${root_BINARY_DIR}/projects/26_day/harib23a/a.hrb ::a.hrb
  COMMAND mcopy -i ${26_day_harib23a_OS} ${root_BINARY_DIR}/projects/26_day/harib23a/hello3.hrb ::hello3.hrb
  COMMAND mcopy -i ${26_day_harib23a_OS} ${root_BINARY_DIR}/projects/26_day/harib23a/hello4.hrb ::hello4.hrb
  COMMAND mcopy -i ${26_day_harib23a_OS} ${root_BINARY_DIR}/projects/26_day/harib23a/winhelo.hrb ::winhelo.hrb
  COMMAND mcopy -i ${26_day_harib23a_OS} ${root_BINARY_DIR}/projects/26_day/harib23a/winhelo2.hrb ::winhelo2.hrb
  COMMAND mcopy -i ${26_day_harib23a_OS} ${root_BINARY_DIR}/projects/26_day/harib23a/winhelo3.hrb ::winhelo3.hrb
  COMMAND mcopy -i ${26_day_harib23a_OS} ${root_BINARY_DIR}/projects/26_day/harib23a/star1.hrb ::star1.hrb
  COMMAND mcopy -i ${26_day_harib23a_OS} ${root_BINARY_DIR}/projects/26_day/harib23a/stars.hrb ::stars.hrb
  COMMAND mcopy -i ${26_day_harib23a_OS} ${root_BINARY_DIR}/projects/26_day/harib23a/stars2.hrb ::stars2.hrb
  COMMAND mcopy -i ${26_day_harib23a_OS} ${root_BINARY_DIR}/projects/26_day/harib23a/lines.hrb ::lines.hrb
  COMMAND mcopy -i ${26_day_harib23a_OS} ${root_BINARY_DIR}/projects/26_day/harib23a/walk.hrb ::walk.hrb
  COMMAND mcopy -i ${26_day_harib23a_OS} ${root_BINARY_DIR}/projects/26_day/harib23a/color2.hrb ::color2.hrb
  COMMAND mcopy -i ${26_day_harib23a_OS} ${root_BINARY_DIR}/projects/26_day/harib23a/color.hrb ::color.hrb
  COMMAND mcopy -i ${26_day_harib23a_OS} ${root_BINARY_DIR}/projects/26_day/harib23a/beepdown.hrb ::beepdown.hrb
  COMMAND mcopy -i ${26_day_harib23a_OS} ${root_BINARY_DIR}/projects/26_day/harib23a/noodle.hrb ::noodle.hrb
  COMMAND mcopy -i ${26_day_harib23a_OS} ${root_BINARY_DIR}/projects/26_day/harib23a/hello.hrb ::hello.hrb
  COMMAND mcopy -i ${26_day_harib23a_OS} ${root_BINARY_DIR}/projects/26_day/harib23a/hello2.hrb ::hello2.hrb
  COMMAND mcopy -i ${26_day_harib23a_OS} ${root_BINARY_DIR}/projects/26_day/harib23a/hello5.hrb ::hello5.hrb
  DEPENDS 26_day_harib23a_sys ${root_BINARY_DIR}/projects/26_day/harib23a/a.hrb ${root_BINARY_DIR}/projects/26_day/harib23a/hello3.hrb ${root_BINARY_DIR}/projects/26_day/harib23a/hello4.hrb ${root_BINARY_DIR}/projects/26_day/harib23a/winhelo.hrb ${root_BINARY_DIR}/projects/26_day/harib23a/winhelo2.hrb ${root_BINARY_DIR}/projects/26_day/harib23a/winhelo3.hrb ${root_BINARY_DIR}/projects/26_day/harib23a/star1.hrb ${root_BINARY_DIR}/projects/26_day/harib23a/stars.hrb ${root_BINARY_DIR}/projects/26_day/harib23a/stars2.hrb ${root_BINARY_DIR}/projects/26_day/harib23a/lines.hrb ${root_BINARY_DIR}/projects/26_day/harib23a/walk.hrb ${root_BINARY_DIR}/projects/26_day/harib23a/noodle.hrb ${root_BINARY_DIR}/projects/26_day/harib23a/beepdown.hrb ${root_BINARY_DIR}/projects/26_day/harib23a/color.hrb ${root_BINARY_DIR}/projects/26_day/harib23a/color2.hrb ${root_BINARY_DIR}/projects/26_day/harib23a/hello.hrb ${root_BINARY_DIR}/projects/26_day/harib23a/hello2.hrb ${root_BINARY_DIR}/projects/26_day/harib23a/hello5.hrb
)

#----------------------------------------------------------
########### application .hrb files (copied into os.img) ###############
set(26_day_harib23a_APIO ${root_BINARY_DIR}/projects/26_day/harib23a/a_nask.o)
set(26_day_harib23a_RANDO ${root_BINARY_DIR}/projects/26_day/harib23a/rand.o)
add_custom_command(
  OUTPUT ${26_day_harib23a_RANDO}
  COMMAND gcc -fleading-underscore -m32 -nostdlib -fno-pie -x c -c ${root_SOURCE_DIR}/golibc/rand.c_ -o ${26_day_harib23a_RANDO}
  DEPENDS ${root_SOURCE_DIR}/golibc/rand.c_
  COMMENT "Generating rand.o for 26_day_harib23a"
)
add_custom_command(
  OUTPUT ${26_day_harib23a_APIO}
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/26_day/harib23a/a_nask.nas ${26_day_harib23a_APIO}
  DEPENDS ${root_SOURCE_DIR}/projects/26_day/harib23a/a_nask.nas
  COMMENT "Generating a_nask.o for 26_day_harib23a"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/26_day/harib23a/a.hrb
  COMMAND gcc ${BINOPT} -T ${26_day_harib23a_APPLDS} ${root_SOURCE_DIR}/projects/26_day/harib23a/a.c ${26_day_harib23a_APIO} ${26_day_harib23a_LIBGC} ${26_day_harib23a_RANDO} -o ${root_BINARY_DIR}/projects/26_day/harib23a/a.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/26_day/harib23a/a.c ${root_SOURCE_DIR}/projects/app.lds ${26_day_harib23a_APIO} ${26_day_harib23a_LIBGC} ${26_day_harib23a_RANDO}
  COMMENT "Generating a.hrb for 26_day_harib23a"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/26_day/harib23a/hello3.hrb
  COMMAND gcc ${BINOPT} -T ${26_day_harib23a_APPLDS} ${root_SOURCE_DIR}/projects/26_day/harib23a/hello3.c ${26_day_harib23a_APIO} ${26_day_harib23a_LIBGC} ${26_day_harib23a_RANDO} -o ${root_BINARY_DIR}/projects/26_day/harib23a/hello3.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/26_day/harib23a/hello3.c ${root_SOURCE_DIR}/projects/app.lds ${26_day_harib23a_APIO} ${26_day_harib23a_LIBGC} ${26_day_harib23a_RANDO}
  COMMENT "Generating hello3.hrb for 26_day_harib23a"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/26_day/harib23a/hello4.hrb
  COMMAND gcc ${BINOPT} -T ${26_day_harib23a_APPLDS} ${root_SOURCE_DIR}/projects/26_day/harib23a/hello4.c ${26_day_harib23a_APIO} ${26_day_harib23a_LIBGC} ${26_day_harib23a_RANDO} -o ${root_BINARY_DIR}/projects/26_day/harib23a/hello4.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/26_day/harib23a/hello4.c ${root_SOURCE_DIR}/projects/app.lds ${26_day_harib23a_APIO} ${26_day_harib23a_LIBGC} ${26_day_harib23a_RANDO}
  COMMENT "Generating hello4.hrb for 26_day_harib23a"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/26_day/harib23a/winhelo.hrb
  COMMAND gcc ${BINOPT} -T ${26_day_harib23a_APPLDS} ${root_SOURCE_DIR}/projects/26_day/harib23a/winhelo.c ${26_day_harib23a_APIO} ${26_day_harib23a_LIBGC} ${26_day_harib23a_RANDO} -o ${root_BINARY_DIR}/projects/26_day/harib23a/winhelo.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/26_day/harib23a/winhelo.c ${root_SOURCE_DIR}/projects/app.lds ${26_day_harib23a_APIO} ${26_day_harib23a_LIBGC} ${26_day_harib23a_RANDO}
  COMMENT "Generating winhelo.hrb for 26_day_harib23a"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/26_day/harib23a/winhelo2.hrb
  COMMAND gcc ${BINOPT} -T ${26_day_harib23a_APPLDS} ${root_SOURCE_DIR}/projects/26_day/harib23a/winhelo2.c ${26_day_harib23a_APIO} ${26_day_harib23a_LIBGC} ${26_day_harib23a_RANDO} -o ${root_BINARY_DIR}/projects/26_day/harib23a/winhelo2.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/26_day/harib23a/winhelo2.c ${root_SOURCE_DIR}/projects/app.lds ${26_day_harib23a_APIO} ${26_day_harib23a_LIBGC} ${26_day_harib23a_RANDO}
  COMMENT "Generating winhelo2.hrb for 26_day_harib23a"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/26_day/harib23a/winhelo3.hrb
  COMMAND gcc ${BINOPT} -T ${26_day_harib23a_APPLDS} ${root_SOURCE_DIR}/projects/26_day/harib23a/winhelo3.c ${26_day_harib23a_APIO} ${26_day_harib23a_LIBGC} ${26_day_harib23a_RANDO} -o ${root_BINARY_DIR}/projects/26_day/harib23a/winhelo3.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/26_day/harib23a/winhelo3.c ${root_SOURCE_DIR}/projects/app.lds ${26_day_harib23a_APIO} ${26_day_harib23a_LIBGC} ${26_day_harib23a_RANDO}
  COMMENT "Generating winhelo3.hrb for 26_day_harib23a"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/26_day/harib23a/star1.hrb
  COMMAND gcc ${BINOPT} -T ${26_day_harib23a_APPLDS} ${root_SOURCE_DIR}/projects/26_day/harib23a/star1.c ${26_day_harib23a_APIO} ${26_day_harib23a_LIBGC} ${26_day_harib23a_RANDO} -o ${root_BINARY_DIR}/projects/26_day/harib23a/star1.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/26_day/harib23a/star1.c ${root_SOURCE_DIR}/projects/app.lds ${26_day_harib23a_APIO} ${26_day_harib23a_LIBGC} ${26_day_harib23a_RANDO}
  COMMENT "Generating star1.hrb for 26_day_harib23a"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/26_day/harib23a/stars.hrb
  COMMAND gcc ${BINOPT} -T ${26_day_harib23a_APPLDS} ${root_SOURCE_DIR}/projects/26_day/harib23a/stars.c ${26_day_harib23a_APIO} ${26_day_harib23a_LIBGC} ${26_day_harib23a_RANDO} -o ${root_BINARY_DIR}/projects/26_day/harib23a/stars.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/26_day/harib23a/stars.c ${root_SOURCE_DIR}/projects/app.lds ${26_day_harib23a_APIO} ${26_day_harib23a_LIBGC} ${26_day_harib23a_RANDO}
  COMMENT "Generating stars.hrb for 26_day_harib23a"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/26_day/harib23a/stars2.hrb
  COMMAND gcc ${BINOPT} -T ${26_day_harib23a_APPLDS} ${root_SOURCE_DIR}/projects/26_day/harib23a/stars2.c ${26_day_harib23a_APIO} ${26_day_harib23a_LIBGC} ${26_day_harib23a_RANDO} -o ${root_BINARY_DIR}/projects/26_day/harib23a/stars2.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/26_day/harib23a/stars2.c ${root_SOURCE_DIR}/projects/app.lds ${26_day_harib23a_APIO} ${26_day_harib23a_LIBGC} ${26_day_harib23a_RANDO}
  COMMENT "Generating stars2.hrb for 26_day_harib23a"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/26_day/harib23a/lines.hrb
  COMMAND gcc ${BINOPT} -T ${26_day_harib23a_APPLDS} ${root_SOURCE_DIR}/projects/26_day/harib23a/lines.c ${26_day_harib23a_APIO} ${26_day_harib23a_LIBGC} ${26_day_harib23a_RANDO} -o ${root_BINARY_DIR}/projects/26_day/harib23a/lines.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/26_day/harib23a/lines.c ${root_SOURCE_DIR}/projects/app.lds ${26_day_harib23a_APIO} ${26_day_harib23a_LIBGC} ${26_day_harib23a_RANDO}
  COMMENT "Generating lines.hrb for 26_day_harib23a"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/26_day/harib23a/walk.hrb
  COMMAND gcc ${BINOPT} -T ${26_day_harib23a_APPLDS} ${root_SOURCE_DIR}/projects/26_day/harib23a/walk.c ${26_day_harib23a_APIO} ${26_day_harib23a_LIBGC} ${26_day_harib23a_RANDO} -o ${root_BINARY_DIR}/projects/26_day/harib23a/walk.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/26_day/harib23a/walk.c ${root_SOURCE_DIR}/projects/app.lds ${26_day_harib23a_APIO} ${26_day_harib23a_LIBGC} ${26_day_harib23a_RANDO}
  COMMENT "Generating walk.hrb for 26_day_harib23a"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/26_day/harib23a/color2.hrb
  COMMAND gcc ${BINOPT} -T ${26_day_harib23a_APPLDS} ${root_SOURCE_DIR}/projects/26_day/harib23a/color2.c ${26_day_harib23a_APIO} ${26_day_harib23a_LIBGC} ${26_day_harib23a_RANDO} -o ${root_BINARY_DIR}/projects/26_day/harib23a/color2.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/26_day/harib23a/color2.c ${root_SOURCE_DIR}/projects/app.lds ${26_day_harib23a_APIO} ${26_day_harib23a_LIBGC} ${26_day_harib23a_RANDO}
  COMMENT "Generating color2.hrb for 26_day_harib23a"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/26_day/harib23a/color.hrb
  COMMAND gcc ${BINOPT} -T ${26_day_harib23a_APPLDS} ${root_SOURCE_DIR}/projects/26_day/harib23a/color.c ${26_day_harib23a_APIO} ${26_day_harib23a_LIBGC} ${26_day_harib23a_RANDO} -o ${root_BINARY_DIR}/projects/26_day/harib23a/color.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/26_day/harib23a/color.c ${root_SOURCE_DIR}/projects/app.lds ${26_day_harib23a_APIO} ${26_day_harib23a_LIBGC} ${26_day_harib23a_RANDO}
  COMMENT "Generating color.hrb for 26_day_harib23a"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/26_day/harib23a/beepdown.hrb
  COMMAND gcc ${BINOPT} -T ${26_day_harib23a_APPLDS} ${root_SOURCE_DIR}/projects/26_day/harib23a/beepdown.c ${26_day_harib23a_APIO} ${26_day_harib23a_LIBGC} ${26_day_harib23a_RANDO} -o ${root_BINARY_DIR}/projects/26_day/harib23a/beepdown.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/26_day/harib23a/beepdown.c ${root_SOURCE_DIR}/projects/app.lds ${26_day_harib23a_APIO} ${26_day_harib23a_LIBGC} ${26_day_harib23a_RANDO}
  COMMENT "Generating beepdown.hrb for 26_day_harib23a"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/26_day/harib23a/noodle.hrb
  COMMAND gcc ${BINOPT} -T ${26_day_harib23a_APPLDS} ${root_SOURCE_DIR}/projects/26_day/harib23a/noodle.c ${26_day_harib23a_APIO} ${26_day_harib23a_LIBGC} ${26_day_harib23a_RANDO} -o ${root_BINARY_DIR}/projects/26_day/harib23a/noodle.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/26_day/harib23a/noodle.c ${root_SOURCE_DIR}/projects/app.lds ${26_day_harib23a_APIO} ${26_day_harib23a_LIBGC} ${26_day_harib23a_RANDO}
  COMMENT "Generating noodle.hrb for 26_day_harib23a"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/26_day/harib23a/hello.hrb
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/26_day/harib23a/hello.nas ${root_BINARY_DIR}/projects/26_day/harib23a/hello.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/26_day/harib23a/hello.nas
  COMMENT "Generating hello.hrb for 26_day_harib23a"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/26_day/harib23a/hello2.hrb
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/26_day/harib23a/hello2.nas ${root_BINARY_DIR}/projects/26_day/harib23a/hello2.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/26_day/harib23a/hello2.nas
  COMMENT "Generating hello2.hrb for 26_day_harib23a"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/26_day/harib23a/hello5.hrb
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/26_day/harib23a/hello5.nas ${root_BINARY_DIR}/projects/26_day/harib23a/hello5.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/26_day/harib23a/hello5.nas
  COMMENT "Generating hello5.hrb for 26_day_harib23a"
)
