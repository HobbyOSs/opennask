#----------------------------------------------------------
message(STATUS "Entering directory projects/24_day/harib21c/")

set(FONT ${root_BINARY_DIR}/src/makefont)
set(B2O  ${root_BINARY_DIR}/src/bin2obj)
set(CONV ${root_BINARY_DIR}/objconv/objconv)
set(24_day_harib21c_OS    ${root_BINARY_DIR}/projects/24_day/harib21c/os.img)
set(24_day_harib21c_SYS	  ${root_BINARY_DIR}/projects/24_day/harib21c/os.sys)
set(24_day_harib21c_IPLB  ${root_BINARY_DIR}/projects/24_day/harib21c/ipl.bin)
set(24_day_harib21c_IPLS  ${root_SOURCE_DIR}/projects/24_day/harib21c/ipl10.nas)
set(24_day_harib21c_HEADB ${root_BINARY_DIR}/projects/24_day/harib21c/asmhead.bin)
set(24_day_harib21c_HEADS ${root_SOURCE_DIR}/projects/24_day/harib21c/asmhead.nas)
set(24_day_harib21c_BOOTB ${root_BINARY_DIR}/projects/24_day/harib21c/boot.bin)
set(24_day_harib21c_CCO	  ${root_BINARY_DIR}/projects/24_day/harib21c/boot.o)
file(GLOB 24_day_harib21c_CCS ${root_SOURCE_DIR}/projects/24_day/harib21c/*.c)
list(REMOVE_ITEM 24_day_harib21c_CCS
  ${root_SOURCE_DIR}/projects/24_day/harib21c/a.c
  ${root_SOURCE_DIR}/projects/24_day/harib21c/hello3.c
  ${root_SOURCE_DIR}/projects/24_day/harib21c/hello4.c
  ${root_SOURCE_DIR}/projects/24_day/harib21c/winhelo.c
  ${root_SOURCE_DIR}/projects/24_day/harib21c/winhelo2.c
  ${root_SOURCE_DIR}/projects/24_day/harib21c/winhelo3.c
  ${root_SOURCE_DIR}/projects/24_day/harib21c/star1.c
  ${root_SOURCE_DIR}/projects/24_day/harib21c/stars.c
  ${root_SOURCE_DIR}/projects/24_day/harib21c/stars2.c
  ${root_SOURCE_DIR}/projects/24_day/harib21c/lines.c
  ${root_SOURCE_DIR}/projects/24_day/harib21c/walk.c)
set(24_day_harib21c_LDS	  ${root_SOURCE_DIR}/projects/os.lds)
set(24_day_harib21c_APPLDS  ${root_SOURCE_DIR}/projects/app.lds)
set(24_day_harib21c_FUNCS ${root_SOURCE_DIR}/projects/24_day/harib21c/naskfunc.nas)
set(24_day_harib21c_FUNCO ${root_BINARY_DIR}/projects/24_day/harib21c/naskfunc.o)
set(24_day_harib21c_FONTS ${root_SOURCE_DIR}/projects/24_day/harib21c/hankaku.txt)
set(24_day_harib21c_FONTB ${root_BINARY_DIR}/projects/24_day/harib21c/hankaku.bin)
set(24_day_harib21c_FONTO ${root_BINARY_DIR}/projects/24_day/harib21c/hankaku.o)
set(24_day_harib21c_LIBGE ${root_BINARY_DIR}/golibc/libgo.a)
set(24_day_harib21c_LIBGC ${root_BINARY_DIR}/projects/24_day/harib21c/libgo.o)
set(24_day_harib21c_WILDOBJ ${root_BINARY_DIR}/projects/24_day/harib21c/*.o)


add_custom_target(24_day_harib21c_run
  COMMAND ${QEMU} ${QEMUOPT} ${24_day_harib21c_OS}
  DEPENDS 24_day_harib21c_img
)
add_custom_target(24_day_harib21c_debug
  COMMAND ${QEMU} -s -S ${QEMUOPT} ${24_day_harib21c_OS} ${QEMU_DEBUG_OPT}
  DEPENDS 24_day_harib21c_img
)
add_custom_target(24_day_harib21c_clean
  COMMAND rm -f ${24_day_harib21c_OS}
  COMMAND rm -f ${24_day_harib21c_SYS}
  COMMAND rm -f ${24_day_harib21c_IPLB}
  COMMAND rm -f ${24_day_harib21c_HEADB}
  COMMAND rm -f ${24_day_harib21c_BOOTB}
  COMMAND rm -f ${24_day_harib21c_WILDOBJ}
)
add_custom_target(24_day_harib21c_ipl
  COMMAND ${RAKUSK_EXECUTABLE} ${24_day_harib21c_IPLS} ${24_day_harib21c_IPLB}
  DEPENDS ${24_day_harib21c_IPLS}
)
add_custom_command(
  OUTPUT ${24_day_harib21c_HEADB}
  COMMAND ${RAKUSK_EXECUTABLE} ${24_day_harib21c_HEADS} ${24_day_harib21c_HEADB}
  DEPENDS ${24_day_harib21c_HEADS}
  COMMENT "Generating asmhead.bin for 24_day_harib21c"
)
add_custom_command(
  OUTPUT ${24_day_harib21c_FUNCO}
  COMMAND ${RAKUSK_EXECUTABLE} ${24_day_harib21c_FUNCS} ${24_day_harib21c_FUNCO}
  DEPENDS ${24_day_harib21c_FUNCS}
  COMMENT "Generating naskfunc.o for 24_day_harib21c"
)
add_custom_command(
  OUTPUT ${24_day_harib21c_FONTO}
  COMMAND ${FONT} ${24_day_harib21c_FONTS} ${24_day_harib21c_FONTB}
  COMMAND ${B2O}  ${24_day_harib21c_FONTB} ${24_day_harib21c_FONTO} _hankaku
  DEPENDS ${24_day_harib21c_FONTS} makefont bin2obj
  COMMENT "Generating hankaku.o for 24_day_harib21c"
)
add_custom_command(
  OUTPUT ${24_day_harib21c_LIBGC}
  COMMAND ${CONV} -fcoff32 -nu ${24_day_harib21c_LIBGE} ${24_day_harib21c_LIBGC}
  DEPENDS ${24_day_harib21c_LIBGE}
  COMMENT "Converting golibc for 24_day_harib21c"
)

add_custom_target(24_day_harib21c_sys
  COMMAND gcc ${BINOPT} -T ${24_day_harib21c_LDS} ${24_day_harib21c_CCS} ${24_day_harib21c_WILDOBJ} -o ${24_day_harib21c_BOOTB}
  COMMAND cat ${24_day_harib21c_HEADB} ${24_day_harib21c_BOOTB} > ${24_day_harib21c_SYS}
  DEPENDS ${24_day_harib21c_HEADB} 24_day_harib21c_ipl
  DEPENDS ${24_day_harib21c_FUNCO}
  DEPENDS ${24_day_harib21c_FONTO} ${24_day_harib21c_LIBGC}
)
add_custom_target(24_day_harib21c_img
  COMMAND mformat -f 1440 -l HARIBOTEOS -N 0xffffffff -C -B ${24_day_harib21c_IPLB} -i ${24_day_harib21c_OS}
  COMMAND mcopy -i ${24_day_harib21c_OS} ${24_day_harib21c_SYS} ::
  COMMAND mcopy -i ${24_day_harib21c_OS} ${root_BINARY_DIR}/projects/24_day/harib21c/a.hrb ::a.hrb
  COMMAND mcopy -i ${24_day_harib21c_OS} ${root_BINARY_DIR}/projects/24_day/harib21c/hello3.hrb ::hello3.hrb
  COMMAND mcopy -i ${24_day_harib21c_OS} ${root_BINARY_DIR}/projects/24_day/harib21c/hello4.hrb ::hello4.hrb
  COMMAND mcopy -i ${24_day_harib21c_OS} ${root_BINARY_DIR}/projects/24_day/harib21c/winhelo.hrb ::winhelo.hrb
  COMMAND mcopy -i ${24_day_harib21c_OS} ${root_BINARY_DIR}/projects/24_day/harib21c/winhelo2.hrb ::winhelo2.hrb
  COMMAND mcopy -i ${24_day_harib21c_OS} ${root_BINARY_DIR}/projects/24_day/harib21c/winhelo3.hrb ::winhelo3.hrb
  COMMAND mcopy -i ${24_day_harib21c_OS} ${root_BINARY_DIR}/projects/24_day/harib21c/star1.hrb ::star1.hrb
  COMMAND mcopy -i ${24_day_harib21c_OS} ${root_BINARY_DIR}/projects/24_day/harib21c/stars.hrb ::stars.hrb
  COMMAND mcopy -i ${24_day_harib21c_OS} ${root_BINARY_DIR}/projects/24_day/harib21c/stars2.hrb ::stars2.hrb
  COMMAND mcopy -i ${24_day_harib21c_OS} ${root_BINARY_DIR}/projects/24_day/harib21c/lines.hrb ::lines.hrb
  COMMAND mcopy -i ${24_day_harib21c_OS} ${root_BINARY_DIR}/projects/24_day/harib21c/walk.hrb ::walk.hrb
  COMMAND mcopy -i ${24_day_harib21c_OS} ${root_BINARY_DIR}/projects/24_day/harib21c/hello.hrb ::hello.hrb
  COMMAND mcopy -i ${24_day_harib21c_OS} ${root_BINARY_DIR}/projects/24_day/harib21c/hello2.hrb ::hello2.hrb
  COMMAND mcopy -i ${24_day_harib21c_OS} ${root_BINARY_DIR}/projects/24_day/harib21c/hello5.hrb ::hello5.hrb
  DEPENDS 24_day_harib21c_sys ${root_BINARY_DIR}/projects/24_day/harib21c/a.hrb ${root_BINARY_DIR}/projects/24_day/harib21c/hello3.hrb ${root_BINARY_DIR}/projects/24_day/harib21c/hello4.hrb ${root_BINARY_DIR}/projects/24_day/harib21c/winhelo.hrb ${root_BINARY_DIR}/projects/24_day/harib21c/winhelo2.hrb ${root_BINARY_DIR}/projects/24_day/harib21c/winhelo3.hrb ${root_BINARY_DIR}/projects/24_day/harib21c/star1.hrb ${root_BINARY_DIR}/projects/24_day/harib21c/stars.hrb ${root_BINARY_DIR}/projects/24_day/harib21c/stars2.hrb ${root_BINARY_DIR}/projects/24_day/harib21c/lines.hrb ${root_BINARY_DIR}/projects/24_day/harib21c/walk.hrb ${root_BINARY_DIR}/projects/24_day/harib21c/hello.hrb ${root_BINARY_DIR}/projects/24_day/harib21c/hello2.hrb ${root_BINARY_DIR}/projects/24_day/harib21c/hello5.hrb
)

#----------------------------------------------------------
########### application .hrb files (copied into os.img) ###############
set(24_day_harib21c_APIO ${root_BINARY_DIR}/projects/24_day/harib21c/a_nask.o)
set(24_day_harib21c_RANDO ${root_BINARY_DIR}/projects/24_day/harib21c/rand.o)
add_custom_command(
  OUTPUT ${24_day_harib21c_RANDO}
  COMMAND gcc -fleading-underscore -m32 -nostdlib -fno-pie -x c -c ${root_SOURCE_DIR}/golibc/rand.c_ -o ${24_day_harib21c_RANDO}
  DEPENDS ${root_SOURCE_DIR}/golibc/rand.c_
  COMMENT "Generating rand.o for 24_day_harib21c"
)
add_custom_command(
  OUTPUT ${24_day_harib21c_APIO}
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/24_day/harib21c/a_nask.nas ${24_day_harib21c_APIO}
  DEPENDS ${root_SOURCE_DIR}/projects/24_day/harib21c/a_nask.nas
  COMMENT "Generating a_nask.o for 24_day_harib21c"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/24_day/harib21c/a.hrb
  COMMAND gcc ${BINOPT} -T ${24_day_harib21c_APPLDS} ${root_SOURCE_DIR}/projects/24_day/harib21c/a.c ${24_day_harib21c_APIO} ${24_day_harib21c_LIBGC} ${24_day_harib21c_RANDO} -o ${root_BINARY_DIR}/projects/24_day/harib21c/a.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/24_day/harib21c/a.c ${root_SOURCE_DIR}/projects/app.lds ${24_day_harib21c_APIO} ${24_day_harib21c_LIBGC} ${24_day_harib21c_RANDO}
  COMMENT "Generating a.hrb for 24_day_harib21c"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/24_day/harib21c/hello3.hrb
  COMMAND gcc ${BINOPT} -T ${24_day_harib21c_APPLDS} ${root_SOURCE_DIR}/projects/24_day/harib21c/hello3.c ${24_day_harib21c_APIO} ${24_day_harib21c_LIBGC} ${24_day_harib21c_RANDO} -o ${root_BINARY_DIR}/projects/24_day/harib21c/hello3.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/24_day/harib21c/hello3.c ${root_SOURCE_DIR}/projects/app.lds ${24_day_harib21c_APIO} ${24_day_harib21c_LIBGC} ${24_day_harib21c_RANDO}
  COMMENT "Generating hello3.hrb for 24_day_harib21c"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/24_day/harib21c/hello4.hrb
  COMMAND gcc ${BINOPT} -T ${24_day_harib21c_APPLDS} ${root_SOURCE_DIR}/projects/24_day/harib21c/hello4.c ${24_day_harib21c_APIO} ${24_day_harib21c_LIBGC} ${24_day_harib21c_RANDO} -o ${root_BINARY_DIR}/projects/24_day/harib21c/hello4.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/24_day/harib21c/hello4.c ${root_SOURCE_DIR}/projects/app.lds ${24_day_harib21c_APIO} ${24_day_harib21c_LIBGC} ${24_day_harib21c_RANDO}
  COMMENT "Generating hello4.hrb for 24_day_harib21c"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/24_day/harib21c/winhelo.hrb
  COMMAND gcc ${BINOPT} -T ${24_day_harib21c_APPLDS} ${root_SOURCE_DIR}/projects/24_day/harib21c/winhelo.c ${24_day_harib21c_APIO} ${24_day_harib21c_LIBGC} ${24_day_harib21c_RANDO} -o ${root_BINARY_DIR}/projects/24_day/harib21c/winhelo.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/24_day/harib21c/winhelo.c ${root_SOURCE_DIR}/projects/app.lds ${24_day_harib21c_APIO} ${24_day_harib21c_LIBGC} ${24_day_harib21c_RANDO}
  COMMENT "Generating winhelo.hrb for 24_day_harib21c"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/24_day/harib21c/winhelo2.hrb
  COMMAND gcc ${BINOPT} -T ${24_day_harib21c_APPLDS} ${root_SOURCE_DIR}/projects/24_day/harib21c/winhelo2.c ${24_day_harib21c_APIO} ${24_day_harib21c_LIBGC} ${24_day_harib21c_RANDO} -o ${root_BINARY_DIR}/projects/24_day/harib21c/winhelo2.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/24_day/harib21c/winhelo2.c ${root_SOURCE_DIR}/projects/app.lds ${24_day_harib21c_APIO} ${24_day_harib21c_LIBGC} ${24_day_harib21c_RANDO}
  COMMENT "Generating winhelo2.hrb for 24_day_harib21c"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/24_day/harib21c/winhelo3.hrb
  COMMAND gcc ${BINOPT} -T ${24_day_harib21c_APPLDS} ${root_SOURCE_DIR}/projects/24_day/harib21c/winhelo3.c ${24_day_harib21c_APIO} ${24_day_harib21c_LIBGC} ${24_day_harib21c_RANDO} -o ${root_BINARY_DIR}/projects/24_day/harib21c/winhelo3.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/24_day/harib21c/winhelo3.c ${root_SOURCE_DIR}/projects/app.lds ${24_day_harib21c_APIO} ${24_day_harib21c_LIBGC} ${24_day_harib21c_RANDO}
  COMMENT "Generating winhelo3.hrb for 24_day_harib21c"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/24_day/harib21c/star1.hrb
  COMMAND gcc ${BINOPT} -T ${24_day_harib21c_APPLDS} ${root_SOURCE_DIR}/projects/24_day/harib21c/star1.c ${24_day_harib21c_APIO} ${24_day_harib21c_LIBGC} ${24_day_harib21c_RANDO} -o ${root_BINARY_DIR}/projects/24_day/harib21c/star1.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/24_day/harib21c/star1.c ${root_SOURCE_DIR}/projects/app.lds ${24_day_harib21c_APIO} ${24_day_harib21c_LIBGC} ${24_day_harib21c_RANDO}
  COMMENT "Generating star1.hrb for 24_day_harib21c"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/24_day/harib21c/stars.hrb
  COMMAND gcc ${BINOPT} -T ${24_day_harib21c_APPLDS} ${root_SOURCE_DIR}/projects/24_day/harib21c/stars.c ${24_day_harib21c_APIO} ${24_day_harib21c_LIBGC} ${24_day_harib21c_RANDO} -o ${root_BINARY_DIR}/projects/24_day/harib21c/stars.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/24_day/harib21c/stars.c ${root_SOURCE_DIR}/projects/app.lds ${24_day_harib21c_APIO} ${24_day_harib21c_LIBGC} ${24_day_harib21c_RANDO}
  COMMENT "Generating stars.hrb for 24_day_harib21c"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/24_day/harib21c/stars2.hrb
  COMMAND gcc ${BINOPT} -T ${24_day_harib21c_APPLDS} ${root_SOURCE_DIR}/projects/24_day/harib21c/stars2.c ${24_day_harib21c_APIO} ${24_day_harib21c_LIBGC} ${24_day_harib21c_RANDO} -o ${root_BINARY_DIR}/projects/24_day/harib21c/stars2.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/24_day/harib21c/stars2.c ${root_SOURCE_DIR}/projects/app.lds ${24_day_harib21c_APIO} ${24_day_harib21c_LIBGC} ${24_day_harib21c_RANDO}
  COMMENT "Generating stars2.hrb for 24_day_harib21c"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/24_day/harib21c/lines.hrb
  COMMAND gcc ${BINOPT} -T ${24_day_harib21c_APPLDS} ${root_SOURCE_DIR}/projects/24_day/harib21c/lines.c ${24_day_harib21c_APIO} ${24_day_harib21c_LIBGC} ${24_day_harib21c_RANDO} -o ${root_BINARY_DIR}/projects/24_day/harib21c/lines.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/24_day/harib21c/lines.c ${root_SOURCE_DIR}/projects/app.lds ${24_day_harib21c_APIO} ${24_day_harib21c_LIBGC} ${24_day_harib21c_RANDO}
  COMMENT "Generating lines.hrb for 24_day_harib21c"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/24_day/harib21c/walk.hrb
  COMMAND gcc ${BINOPT} -T ${24_day_harib21c_APPLDS} ${root_SOURCE_DIR}/projects/24_day/harib21c/walk.c ${24_day_harib21c_APIO} ${24_day_harib21c_LIBGC} ${24_day_harib21c_RANDO} -o ${root_BINARY_DIR}/projects/24_day/harib21c/walk.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/24_day/harib21c/walk.c ${root_SOURCE_DIR}/projects/app.lds ${24_day_harib21c_APIO} ${24_day_harib21c_LIBGC} ${24_day_harib21c_RANDO}
  COMMENT "Generating walk.hrb for 24_day_harib21c"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/24_day/harib21c/hello.hrb
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/24_day/harib21c/hello.nas ${root_BINARY_DIR}/projects/24_day/harib21c/hello.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/24_day/harib21c/hello.nas
  COMMENT "Generating hello.hrb for 24_day_harib21c"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/24_day/harib21c/hello2.hrb
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/24_day/harib21c/hello2.nas ${root_BINARY_DIR}/projects/24_day/harib21c/hello2.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/24_day/harib21c/hello2.nas
  COMMENT "Generating hello2.hrb for 24_day_harib21c"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/24_day/harib21c/hello5.hrb
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/24_day/harib21c/hello5.nas ${root_BINARY_DIR}/projects/24_day/harib21c/hello5.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/24_day/harib21c/hello5.nas
  COMMENT "Generating hello5.hrb for 24_day_harib21c"
)
