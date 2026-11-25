#----------------------------------------------------------
message(STATUS "Entering directory projects/26_day/harib23f/")

set(FONT ${root_BINARY_DIR}/src/makefont)
set(B2O  ${root_BINARY_DIR}/src/bin2obj)
set(CONV ${root_BINARY_DIR}/objconv/objconv)
set(26_day_harib23f_OS    ${root_BINARY_DIR}/projects/26_day/harib23f/os.img)
set(26_day_harib23f_SYS	  ${root_BINARY_DIR}/projects/26_day/harib23f/os.sys)
set(26_day_harib23f_IPLB  ${root_BINARY_DIR}/projects/26_day/harib23f/ipl.bin)
set(26_day_harib23f_IPLS  ${root_SOURCE_DIR}/projects/26_day/harib23f/ipl10.nas)
set(26_day_harib23f_HEADB ${root_BINARY_DIR}/projects/26_day/harib23f/asmhead.bin)
set(26_day_harib23f_HEADS ${root_SOURCE_DIR}/projects/26_day/harib23f/asmhead.nas)
set(26_day_harib23f_BOOTB ${root_BINARY_DIR}/projects/26_day/harib23f/boot.bin)
set(26_day_harib23f_CCO	  ${root_BINARY_DIR}/projects/26_day/harib23f/boot.o)
file(GLOB 26_day_harib23f_CCS ${root_SOURCE_DIR}/projects/26_day/harib23f/*.c)
list(REMOVE_ITEM 26_day_harib23f_CCS
  ${root_SOURCE_DIR}/projects/26_day/harib23f/a.c
  ${root_SOURCE_DIR}/projects/26_day/harib23f/hello3.c
  ${root_SOURCE_DIR}/projects/26_day/harib23f/hello4.c
  ${root_SOURCE_DIR}/projects/26_day/harib23f/winhelo.c
  ${root_SOURCE_DIR}/projects/26_day/harib23f/winhelo2.c
  ${root_SOURCE_DIR}/projects/26_day/harib23f/winhelo3.c
  ${root_SOURCE_DIR}/projects/26_day/harib23f/star1.c
  ${root_SOURCE_DIR}/projects/26_day/harib23f/stars.c
  ${root_SOURCE_DIR}/projects/26_day/harib23f/stars2.c
  ${root_SOURCE_DIR}/projects/26_day/harib23f/lines.c
  ${root_SOURCE_DIR}/projects/26_day/harib23f/walk.c
  ${root_SOURCE_DIR}/projects/26_day/harib23f/noodle.c
  ${root_SOURCE_DIR}/projects/26_day/harib23f/beepdown.c
  ${root_SOURCE_DIR}/projects/26_day/harib23f/color.c
  ${root_SOURCE_DIR}/projects/26_day/harib23f/color2.c)
set(26_day_harib23f_LDS	  ${root_SOURCE_DIR}/projects/os.lds)
set(26_day_harib23f_APPLDS  ${root_SOURCE_DIR}/projects/app.lds)
set(26_day_harib23f_FUNCS ${root_SOURCE_DIR}/projects/26_day/harib23f/naskfunc.nas)
set(26_day_harib23f_FUNCO ${root_BINARY_DIR}/projects/26_day/harib23f/naskfunc.o)
set(26_day_harib23f_FONTS ${root_SOURCE_DIR}/projects/26_day/harib23f/hankaku.txt)
set(26_day_harib23f_FONTB ${root_BINARY_DIR}/projects/26_day/harib23f/hankaku.bin)
set(26_day_harib23f_FONTO ${root_BINARY_DIR}/projects/26_day/harib23f/hankaku.o)
set(26_day_harib23f_LIBGE ${root_BINARY_DIR}/golibc/libgo.a)
set(26_day_harib23f_LIBGC ${root_BINARY_DIR}/projects/26_day/harib23f/libgo.o)
set(26_day_harib23f_WILDOBJ ${root_BINARY_DIR}/projects/26_day/harib23f/*.o)


add_custom_target(26_day_harib23f_run
  COMMAND ${QEMU} ${QEMUOPT} ${26_day_harib23f_OS}
  DEPENDS 26_day_harib23f_img
)
add_custom_target(26_day_harib23f_debug
  COMMAND ${QEMU} -s -S ${QEMUOPT} ${26_day_harib23f_OS} ${QEMU_DEBUG_OPT}
  DEPENDS 26_day_harib23f_img
)
add_custom_target(26_day_harib23f_clean
  COMMAND rm -f ${26_day_harib23f_OS}
  COMMAND rm -f ${26_day_harib23f_SYS}
  COMMAND rm -f ${26_day_harib23f_IPLB}
  COMMAND rm -f ${26_day_harib23f_HEADB}
  COMMAND rm -f ${26_day_harib23f_BOOTB}
  COMMAND rm -f ${26_day_harib23f_WILDOBJ}
)
add_custom_target(26_day_harib23f_ipl
  COMMAND ${RAKUSK_EXECUTABLE} ${26_day_harib23f_IPLS} ${26_day_harib23f_IPLB}
  DEPENDS ${26_day_harib23f_IPLS}
)
add_custom_command(
  OUTPUT ${26_day_harib23f_HEADB}
  COMMAND ${RAKUSK_EXECUTABLE} ${26_day_harib23f_HEADS} ${26_day_harib23f_HEADB}
  DEPENDS ${26_day_harib23f_HEADS}
  COMMENT "Generating asmhead.bin for 26_day_harib23f"
)
add_custom_command(
  OUTPUT ${26_day_harib23f_FUNCO}
  COMMAND ${RAKUSK_EXECUTABLE} ${26_day_harib23f_FUNCS} ${26_day_harib23f_FUNCO}
  DEPENDS ${26_day_harib23f_FUNCS}
  COMMENT "Generating naskfunc.o for 26_day_harib23f"
)
add_custom_command(
  OUTPUT ${26_day_harib23f_FONTO}
  COMMAND ${FONT} ${26_day_harib23f_FONTS} ${26_day_harib23f_FONTB}
  COMMAND ${B2O}  ${26_day_harib23f_FONTB} ${26_day_harib23f_FONTO} _hankaku
  DEPENDS ${26_day_harib23f_FONTS} makefont bin2obj
  COMMENT "Generating hankaku.o for 26_day_harib23f"
)
add_custom_command(
  OUTPUT ${26_day_harib23f_LIBGC}
  COMMAND ${CONV} -fcoff32 -nu ${26_day_harib23f_LIBGE} ${26_day_harib23f_LIBGC}
  DEPENDS ${26_day_harib23f_LIBGE}
  COMMENT "Converting golibc for 26_day_harib23f"
)

add_custom_target(26_day_harib23f_sys
  COMMAND gcc ${BINOPT} -T ${26_day_harib23f_LDS} ${26_day_harib23f_CCS} ${26_day_harib23f_WILDOBJ} -o ${26_day_harib23f_BOOTB}
  COMMAND cat ${26_day_harib23f_HEADB} ${26_day_harib23f_BOOTB} > ${26_day_harib23f_SYS}
  DEPENDS ${26_day_harib23f_HEADB} 26_day_harib23f_ipl
  DEPENDS ${26_day_harib23f_FUNCO}
  DEPENDS ${26_day_harib23f_FONTO} ${26_day_harib23f_LIBGC}
)
add_custom_target(26_day_harib23f_img
  COMMAND mformat -f 1440 -l HARIBOTEOS -N 0xffffffff -C -B ${26_day_harib23f_IPLB} -i ${26_day_harib23f_OS}
  COMMAND mcopy -i ${26_day_harib23f_OS} ${26_day_harib23f_SYS} ::
  COMMAND mcopy -i ${26_day_harib23f_OS} ${root_BINARY_DIR}/projects/26_day/harib23f/a.hrb ::a.hrb
  COMMAND mcopy -i ${26_day_harib23f_OS} ${root_BINARY_DIR}/projects/26_day/harib23f/hello3.hrb ::hello3.hrb
  COMMAND mcopy -i ${26_day_harib23f_OS} ${root_BINARY_DIR}/projects/26_day/harib23f/hello4.hrb ::hello4.hrb
  COMMAND mcopy -i ${26_day_harib23f_OS} ${root_BINARY_DIR}/projects/26_day/harib23f/winhelo.hrb ::winhelo.hrb
  COMMAND mcopy -i ${26_day_harib23f_OS} ${root_BINARY_DIR}/projects/26_day/harib23f/winhelo2.hrb ::winhelo2.hrb
  COMMAND mcopy -i ${26_day_harib23f_OS} ${root_BINARY_DIR}/projects/26_day/harib23f/winhelo3.hrb ::winhelo3.hrb
  COMMAND mcopy -i ${26_day_harib23f_OS} ${root_BINARY_DIR}/projects/26_day/harib23f/star1.hrb ::star1.hrb
  COMMAND mcopy -i ${26_day_harib23f_OS} ${root_BINARY_DIR}/projects/26_day/harib23f/stars.hrb ::stars.hrb
  COMMAND mcopy -i ${26_day_harib23f_OS} ${root_BINARY_DIR}/projects/26_day/harib23f/stars2.hrb ::stars2.hrb
  COMMAND mcopy -i ${26_day_harib23f_OS} ${root_BINARY_DIR}/projects/26_day/harib23f/lines.hrb ::lines.hrb
  COMMAND mcopy -i ${26_day_harib23f_OS} ${root_BINARY_DIR}/projects/26_day/harib23f/walk.hrb ::walk.hrb
  COMMAND mcopy -i ${26_day_harib23f_OS} ${root_BINARY_DIR}/projects/26_day/harib23f/color2.hrb ::color2.hrb
  COMMAND mcopy -i ${26_day_harib23f_OS} ${root_BINARY_DIR}/projects/26_day/harib23f/color.hrb ::color.hrb
  COMMAND mcopy -i ${26_day_harib23f_OS} ${root_BINARY_DIR}/projects/26_day/harib23f/beepdown.hrb ::beepdown.hrb
  COMMAND mcopy -i ${26_day_harib23f_OS} ${root_BINARY_DIR}/projects/26_day/harib23f/noodle.hrb ::noodle.hrb
  COMMAND mcopy -i ${26_day_harib23f_OS} ${root_BINARY_DIR}/projects/26_day/harib23f/hello.hrb ::hello.hrb
  COMMAND mcopy -i ${26_day_harib23f_OS} ${root_BINARY_DIR}/projects/26_day/harib23f/hello2.hrb ::hello2.hrb
  COMMAND mcopy -i ${26_day_harib23f_OS} ${root_BINARY_DIR}/projects/26_day/harib23f/hello5.hrb ::hello5.hrb
  DEPENDS 26_day_harib23f_sys ${root_BINARY_DIR}/projects/26_day/harib23f/a.hrb ${root_BINARY_DIR}/projects/26_day/harib23f/hello3.hrb ${root_BINARY_DIR}/projects/26_day/harib23f/hello4.hrb ${root_BINARY_DIR}/projects/26_day/harib23f/winhelo.hrb ${root_BINARY_DIR}/projects/26_day/harib23f/winhelo2.hrb ${root_BINARY_DIR}/projects/26_day/harib23f/winhelo3.hrb ${root_BINARY_DIR}/projects/26_day/harib23f/star1.hrb ${root_BINARY_DIR}/projects/26_day/harib23f/stars.hrb ${root_BINARY_DIR}/projects/26_day/harib23f/stars2.hrb ${root_BINARY_DIR}/projects/26_day/harib23f/lines.hrb ${root_BINARY_DIR}/projects/26_day/harib23f/walk.hrb ${root_BINARY_DIR}/projects/26_day/harib23f/noodle.hrb ${root_BINARY_DIR}/projects/26_day/harib23f/beepdown.hrb ${root_BINARY_DIR}/projects/26_day/harib23f/color.hrb ${root_BINARY_DIR}/projects/26_day/harib23f/color2.hrb ${root_BINARY_DIR}/projects/26_day/harib23f/hello.hrb ${root_BINARY_DIR}/projects/26_day/harib23f/hello2.hrb ${root_BINARY_DIR}/projects/26_day/harib23f/hello5.hrb
)

#----------------------------------------------------------
########### application .hrb files (copied into os.img) ###############
set(26_day_harib23f_APIO ${root_BINARY_DIR}/projects/26_day/harib23f/a_nask.o)
set(26_day_harib23f_RANDO ${root_BINARY_DIR}/projects/26_day/harib23f/rand.o)
add_custom_command(
  OUTPUT ${26_day_harib23f_RANDO}
  COMMAND gcc -fleading-underscore -m32 -nostdlib -fno-pie -x c -c ${root_SOURCE_DIR}/golibc/rand.c_ -o ${26_day_harib23f_RANDO}
  DEPENDS ${root_SOURCE_DIR}/golibc/rand.c_
  COMMENT "Generating rand.o for 26_day_harib23f"
)
add_custom_command(
  OUTPUT ${26_day_harib23f_APIO}
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/26_day/harib23f/a_nask.nas ${26_day_harib23f_APIO}
  DEPENDS ${root_SOURCE_DIR}/projects/26_day/harib23f/a_nask.nas
  COMMENT "Generating a_nask.o for 26_day_harib23f"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/26_day/harib23f/a.hrb
  COMMAND gcc ${BINOPT} -T ${26_day_harib23f_APPLDS} ${root_SOURCE_DIR}/projects/26_day/harib23f/a.c ${26_day_harib23f_APIO} ${26_day_harib23f_LIBGC} ${26_day_harib23f_RANDO} -o ${root_BINARY_DIR}/projects/26_day/harib23f/a.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/26_day/harib23f/a.c ${root_SOURCE_DIR}/projects/app.lds ${26_day_harib23f_APIO} ${26_day_harib23f_LIBGC} ${26_day_harib23f_RANDO}
  COMMENT "Generating a.hrb for 26_day_harib23f"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/26_day/harib23f/hello3.hrb
  COMMAND gcc ${BINOPT} -T ${26_day_harib23f_APPLDS} ${root_SOURCE_DIR}/projects/26_day/harib23f/hello3.c ${26_day_harib23f_APIO} ${26_day_harib23f_LIBGC} ${26_day_harib23f_RANDO} -o ${root_BINARY_DIR}/projects/26_day/harib23f/hello3.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/26_day/harib23f/hello3.c ${root_SOURCE_DIR}/projects/app.lds ${26_day_harib23f_APIO} ${26_day_harib23f_LIBGC} ${26_day_harib23f_RANDO}
  COMMENT "Generating hello3.hrb for 26_day_harib23f"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/26_day/harib23f/hello4.hrb
  COMMAND gcc ${BINOPT} -T ${26_day_harib23f_APPLDS} ${root_SOURCE_DIR}/projects/26_day/harib23f/hello4.c ${26_day_harib23f_APIO} ${26_day_harib23f_LIBGC} ${26_day_harib23f_RANDO} -o ${root_BINARY_DIR}/projects/26_day/harib23f/hello4.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/26_day/harib23f/hello4.c ${root_SOURCE_DIR}/projects/app.lds ${26_day_harib23f_APIO} ${26_day_harib23f_LIBGC} ${26_day_harib23f_RANDO}
  COMMENT "Generating hello4.hrb for 26_day_harib23f"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/26_day/harib23f/winhelo.hrb
  COMMAND gcc ${BINOPT} -T ${26_day_harib23f_APPLDS} ${root_SOURCE_DIR}/projects/26_day/harib23f/winhelo.c ${26_day_harib23f_APIO} ${26_day_harib23f_LIBGC} ${26_day_harib23f_RANDO} -o ${root_BINARY_DIR}/projects/26_day/harib23f/winhelo.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/26_day/harib23f/winhelo.c ${root_SOURCE_DIR}/projects/app.lds ${26_day_harib23f_APIO} ${26_day_harib23f_LIBGC} ${26_day_harib23f_RANDO}
  COMMENT "Generating winhelo.hrb for 26_day_harib23f"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/26_day/harib23f/winhelo2.hrb
  COMMAND gcc ${BINOPT} -T ${26_day_harib23f_APPLDS} ${root_SOURCE_DIR}/projects/26_day/harib23f/winhelo2.c ${26_day_harib23f_APIO} ${26_day_harib23f_LIBGC} ${26_day_harib23f_RANDO} -o ${root_BINARY_DIR}/projects/26_day/harib23f/winhelo2.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/26_day/harib23f/winhelo2.c ${root_SOURCE_DIR}/projects/app.lds ${26_day_harib23f_APIO} ${26_day_harib23f_LIBGC} ${26_day_harib23f_RANDO}
  COMMENT "Generating winhelo2.hrb for 26_day_harib23f"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/26_day/harib23f/winhelo3.hrb
  COMMAND gcc ${BINOPT} -T ${26_day_harib23f_APPLDS} ${root_SOURCE_DIR}/projects/26_day/harib23f/winhelo3.c ${26_day_harib23f_APIO} ${26_day_harib23f_LIBGC} ${26_day_harib23f_RANDO} -o ${root_BINARY_DIR}/projects/26_day/harib23f/winhelo3.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/26_day/harib23f/winhelo3.c ${root_SOURCE_DIR}/projects/app.lds ${26_day_harib23f_APIO} ${26_day_harib23f_LIBGC} ${26_day_harib23f_RANDO}
  COMMENT "Generating winhelo3.hrb for 26_day_harib23f"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/26_day/harib23f/star1.hrb
  COMMAND gcc ${BINOPT} -T ${26_day_harib23f_APPLDS} ${root_SOURCE_DIR}/projects/26_day/harib23f/star1.c ${26_day_harib23f_APIO} ${26_day_harib23f_LIBGC} ${26_day_harib23f_RANDO} -o ${root_BINARY_DIR}/projects/26_day/harib23f/star1.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/26_day/harib23f/star1.c ${root_SOURCE_DIR}/projects/app.lds ${26_day_harib23f_APIO} ${26_day_harib23f_LIBGC} ${26_day_harib23f_RANDO}
  COMMENT "Generating star1.hrb for 26_day_harib23f"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/26_day/harib23f/stars.hrb
  COMMAND gcc ${BINOPT} -T ${26_day_harib23f_APPLDS} ${root_SOURCE_DIR}/projects/26_day/harib23f/stars.c ${26_day_harib23f_APIO} ${26_day_harib23f_LIBGC} ${26_day_harib23f_RANDO} -o ${root_BINARY_DIR}/projects/26_day/harib23f/stars.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/26_day/harib23f/stars.c ${root_SOURCE_DIR}/projects/app.lds ${26_day_harib23f_APIO} ${26_day_harib23f_LIBGC} ${26_day_harib23f_RANDO}
  COMMENT "Generating stars.hrb for 26_day_harib23f"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/26_day/harib23f/stars2.hrb
  COMMAND gcc ${BINOPT} -T ${26_day_harib23f_APPLDS} ${root_SOURCE_DIR}/projects/26_day/harib23f/stars2.c ${26_day_harib23f_APIO} ${26_day_harib23f_LIBGC} ${26_day_harib23f_RANDO} -o ${root_BINARY_DIR}/projects/26_day/harib23f/stars2.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/26_day/harib23f/stars2.c ${root_SOURCE_DIR}/projects/app.lds ${26_day_harib23f_APIO} ${26_day_harib23f_LIBGC} ${26_day_harib23f_RANDO}
  COMMENT "Generating stars2.hrb for 26_day_harib23f"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/26_day/harib23f/lines.hrb
  COMMAND gcc ${BINOPT} -T ${26_day_harib23f_APPLDS} ${root_SOURCE_DIR}/projects/26_day/harib23f/lines.c ${26_day_harib23f_APIO} ${26_day_harib23f_LIBGC} ${26_day_harib23f_RANDO} -o ${root_BINARY_DIR}/projects/26_day/harib23f/lines.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/26_day/harib23f/lines.c ${root_SOURCE_DIR}/projects/app.lds ${26_day_harib23f_APIO} ${26_day_harib23f_LIBGC} ${26_day_harib23f_RANDO}
  COMMENT "Generating lines.hrb for 26_day_harib23f"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/26_day/harib23f/walk.hrb
  COMMAND gcc ${BINOPT} -T ${26_day_harib23f_APPLDS} ${root_SOURCE_DIR}/projects/26_day/harib23f/walk.c ${26_day_harib23f_APIO} ${26_day_harib23f_LIBGC} ${26_day_harib23f_RANDO} -o ${root_BINARY_DIR}/projects/26_day/harib23f/walk.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/26_day/harib23f/walk.c ${root_SOURCE_DIR}/projects/app.lds ${26_day_harib23f_APIO} ${26_day_harib23f_LIBGC} ${26_day_harib23f_RANDO}
  COMMENT "Generating walk.hrb for 26_day_harib23f"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/26_day/harib23f/color2.hrb
  COMMAND gcc ${BINOPT} -T ${26_day_harib23f_APPLDS} ${root_SOURCE_DIR}/projects/26_day/harib23f/color2.c ${26_day_harib23f_APIO} ${26_day_harib23f_LIBGC} ${26_day_harib23f_RANDO} -o ${root_BINARY_DIR}/projects/26_day/harib23f/color2.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/26_day/harib23f/color2.c ${root_SOURCE_DIR}/projects/app.lds ${26_day_harib23f_APIO} ${26_day_harib23f_LIBGC} ${26_day_harib23f_RANDO}
  COMMENT "Generating color2.hrb for 26_day_harib23f"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/26_day/harib23f/color.hrb
  COMMAND gcc ${BINOPT} -T ${26_day_harib23f_APPLDS} ${root_SOURCE_DIR}/projects/26_day/harib23f/color.c ${26_day_harib23f_APIO} ${26_day_harib23f_LIBGC} ${26_day_harib23f_RANDO} -o ${root_BINARY_DIR}/projects/26_day/harib23f/color.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/26_day/harib23f/color.c ${root_SOURCE_DIR}/projects/app.lds ${26_day_harib23f_APIO} ${26_day_harib23f_LIBGC} ${26_day_harib23f_RANDO}
  COMMENT "Generating color.hrb for 26_day_harib23f"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/26_day/harib23f/beepdown.hrb
  COMMAND gcc ${BINOPT} -T ${26_day_harib23f_APPLDS} ${root_SOURCE_DIR}/projects/26_day/harib23f/beepdown.c ${26_day_harib23f_APIO} ${26_day_harib23f_LIBGC} ${26_day_harib23f_RANDO} -o ${root_BINARY_DIR}/projects/26_day/harib23f/beepdown.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/26_day/harib23f/beepdown.c ${root_SOURCE_DIR}/projects/app.lds ${26_day_harib23f_APIO} ${26_day_harib23f_LIBGC} ${26_day_harib23f_RANDO}
  COMMENT "Generating beepdown.hrb for 26_day_harib23f"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/26_day/harib23f/noodle.hrb
  COMMAND gcc ${BINOPT} -T ${26_day_harib23f_APPLDS} ${root_SOURCE_DIR}/projects/26_day/harib23f/noodle.c ${26_day_harib23f_APIO} ${26_day_harib23f_LIBGC} ${26_day_harib23f_RANDO} -o ${root_BINARY_DIR}/projects/26_day/harib23f/noodle.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/26_day/harib23f/noodle.c ${root_SOURCE_DIR}/projects/app.lds ${26_day_harib23f_APIO} ${26_day_harib23f_LIBGC} ${26_day_harib23f_RANDO}
  COMMENT "Generating noodle.hrb for 26_day_harib23f"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/26_day/harib23f/hello.hrb
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/26_day/harib23f/hello.nas ${root_BINARY_DIR}/projects/26_day/harib23f/hello.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/26_day/harib23f/hello.nas
  COMMENT "Generating hello.hrb for 26_day_harib23f"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/26_day/harib23f/hello2.hrb
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/26_day/harib23f/hello2.nas ${root_BINARY_DIR}/projects/26_day/harib23f/hello2.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/26_day/harib23f/hello2.nas
  COMMENT "Generating hello2.hrb for 26_day_harib23f"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/26_day/harib23f/hello5.hrb
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/26_day/harib23f/hello5.nas ${root_BINARY_DIR}/projects/26_day/harib23f/hello5.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/26_day/harib23f/hello5.nas
  COMMENT "Generating hello5.hrb for 26_day_harib23f"
)
