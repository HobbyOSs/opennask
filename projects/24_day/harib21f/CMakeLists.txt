#----------------------------------------------------------
message(STATUS "Entering directory projects/24_day/harib21f/")

set(FONT ${root_BINARY_DIR}/src/makefont)
set(B2O  ${root_BINARY_DIR}/src/bin2obj)
set(CONV ${root_BINARY_DIR}/objconv/objconv)
set(24_day_harib21f_OS    ${root_BINARY_DIR}/projects/24_day/harib21f/os.img)
set(24_day_harib21f_SYS	  ${root_BINARY_DIR}/projects/24_day/harib21f/os.sys)
set(24_day_harib21f_IPLB  ${root_BINARY_DIR}/projects/24_day/harib21f/ipl.bin)
set(24_day_harib21f_IPLS  ${root_SOURCE_DIR}/projects/24_day/harib21f/ipl10.nas)
set(24_day_harib21f_HEADB ${root_BINARY_DIR}/projects/24_day/harib21f/asmhead.bin)
set(24_day_harib21f_HEADS ${root_SOURCE_DIR}/projects/24_day/harib21f/asmhead.nas)
set(24_day_harib21f_BOOTB ${root_BINARY_DIR}/projects/24_day/harib21f/boot.bin)
set(24_day_harib21f_CCO	  ${root_BINARY_DIR}/projects/24_day/harib21f/boot.o)
file(GLOB 24_day_harib21f_CCS ${root_SOURCE_DIR}/projects/24_day/harib21f/*.c)
list(REMOVE_ITEM 24_day_harib21f_CCS
  ${root_SOURCE_DIR}/projects/24_day/harib21f/a.c
  ${root_SOURCE_DIR}/projects/24_day/harib21f/hello3.c
  ${root_SOURCE_DIR}/projects/24_day/harib21f/hello4.c
  ${root_SOURCE_DIR}/projects/24_day/harib21f/winhelo.c
  ${root_SOURCE_DIR}/projects/24_day/harib21f/winhelo2.c
  ${root_SOURCE_DIR}/projects/24_day/harib21f/winhelo3.c
  ${root_SOURCE_DIR}/projects/24_day/harib21f/star1.c
  ${root_SOURCE_DIR}/projects/24_day/harib21f/stars.c
  ${root_SOURCE_DIR}/projects/24_day/harib21f/stars2.c
  ${root_SOURCE_DIR}/projects/24_day/harib21f/lines.c
  ${root_SOURCE_DIR}/projects/24_day/harib21f/walk.c)
set(24_day_harib21f_LDS	  ${root_SOURCE_DIR}/projects/os.lds)
set(24_day_harib21f_APPLDS  ${root_SOURCE_DIR}/projects/app.lds)
set(24_day_harib21f_FUNCS ${root_SOURCE_DIR}/projects/24_day/harib21f/naskfunc.nas)
set(24_day_harib21f_FUNCO ${root_BINARY_DIR}/projects/24_day/harib21f/naskfunc.o)
set(24_day_harib21f_FONTS ${root_SOURCE_DIR}/projects/24_day/harib21f/hankaku.txt)
set(24_day_harib21f_FONTB ${root_BINARY_DIR}/projects/24_day/harib21f/hankaku.bin)
set(24_day_harib21f_FONTO ${root_BINARY_DIR}/projects/24_day/harib21f/hankaku.o)
set(24_day_harib21f_LIBGE ${root_BINARY_DIR}/golibc/libgo.a)
set(24_day_harib21f_LIBGC ${root_BINARY_DIR}/projects/24_day/harib21f/libgo.o)
set(24_day_harib21f_WILDOBJ ${root_BINARY_DIR}/projects/24_day/harib21f/*.o)


add_custom_target(24_day_harib21f_run
  COMMAND ${QEMU} ${QEMUOPT} ${24_day_harib21f_OS}
  DEPENDS 24_day_harib21f_img
)
add_custom_target(24_day_harib21f_debug
  COMMAND ${QEMU} -s -S ${QEMUOPT} ${24_day_harib21f_OS} ${QEMU_DEBUG_OPT}
  DEPENDS 24_day_harib21f_img
)
add_custom_target(24_day_harib21f_clean
  COMMAND rm -f ${24_day_harib21f_OS}
  COMMAND rm -f ${24_day_harib21f_SYS}
  COMMAND rm -f ${24_day_harib21f_IPLB}
  COMMAND rm -f ${24_day_harib21f_HEADB}
  COMMAND rm -f ${24_day_harib21f_BOOTB}
  COMMAND rm -f ${24_day_harib21f_WILDOBJ}
)
add_custom_target(24_day_harib21f_ipl
  COMMAND ${RAKUSK_EXECUTABLE} ${24_day_harib21f_IPLS} ${24_day_harib21f_IPLB}
  DEPENDS ${24_day_harib21f_IPLS}
)
add_custom_command(
  OUTPUT ${24_day_harib21f_HEADB}
  COMMAND ${RAKUSK_EXECUTABLE} ${24_day_harib21f_HEADS} ${24_day_harib21f_HEADB}
  DEPENDS ${24_day_harib21f_HEADS}
  COMMENT "Generating asmhead.bin for 24_day_harib21f"
)
add_custom_command(
  OUTPUT ${24_day_harib21f_FUNCO}
  COMMAND ${RAKUSK_EXECUTABLE} ${24_day_harib21f_FUNCS} ${24_day_harib21f_FUNCO}
  DEPENDS ${24_day_harib21f_FUNCS}
  COMMENT "Generating naskfunc.o for 24_day_harib21f"
)
add_custom_command(
  OUTPUT ${24_day_harib21f_FONTO}
  COMMAND ${FONT} ${24_day_harib21f_FONTS} ${24_day_harib21f_FONTB}
  COMMAND ${B2O}  ${24_day_harib21f_FONTB} ${24_day_harib21f_FONTO} _hankaku
  DEPENDS ${24_day_harib21f_FONTS} makefont bin2obj
  COMMENT "Generating hankaku.o for 24_day_harib21f"
)
add_custom_command(
  OUTPUT ${24_day_harib21f_LIBGC}
  COMMAND ${CONV} -fcoff32 -nu ${24_day_harib21f_LIBGE} ${24_day_harib21f_LIBGC}
  DEPENDS ${24_day_harib21f_LIBGE}
  COMMENT "Converting golibc for 24_day_harib21f"
)

add_custom_target(24_day_harib21f_sys
  COMMAND gcc ${BINOPT} -T ${24_day_harib21f_LDS} ${24_day_harib21f_CCS} ${24_day_harib21f_WILDOBJ} -o ${24_day_harib21f_BOOTB}
  COMMAND cat ${24_day_harib21f_HEADB} ${24_day_harib21f_BOOTB} > ${24_day_harib21f_SYS}
  DEPENDS ${24_day_harib21f_HEADB} 24_day_harib21f_ipl
  DEPENDS ${24_day_harib21f_FUNCO}
  DEPENDS ${24_day_harib21f_FONTO} ${24_day_harib21f_LIBGC}
)
add_custom_target(24_day_harib21f_img
  COMMAND mformat -f 1440 -l HARIBOTEOS -N 0xffffffff -C -B ${24_day_harib21f_IPLB} -i ${24_day_harib21f_OS}
  COMMAND mcopy -i ${24_day_harib21f_OS} ${24_day_harib21f_SYS} ::
  COMMAND mcopy -i ${24_day_harib21f_OS} ${root_BINARY_DIR}/projects/24_day/harib21f/a.hrb ::a.hrb
  COMMAND mcopy -i ${24_day_harib21f_OS} ${root_BINARY_DIR}/projects/24_day/harib21f/hello3.hrb ::hello3.hrb
  COMMAND mcopy -i ${24_day_harib21f_OS} ${root_BINARY_DIR}/projects/24_day/harib21f/hello4.hrb ::hello4.hrb
  COMMAND mcopy -i ${24_day_harib21f_OS} ${root_BINARY_DIR}/projects/24_day/harib21f/winhelo.hrb ::winhelo.hrb
  COMMAND mcopy -i ${24_day_harib21f_OS} ${root_BINARY_DIR}/projects/24_day/harib21f/winhelo2.hrb ::winhelo2.hrb
  COMMAND mcopy -i ${24_day_harib21f_OS} ${root_BINARY_DIR}/projects/24_day/harib21f/winhelo3.hrb ::winhelo3.hrb
  COMMAND mcopy -i ${24_day_harib21f_OS} ${root_BINARY_DIR}/projects/24_day/harib21f/star1.hrb ::star1.hrb
  COMMAND mcopy -i ${24_day_harib21f_OS} ${root_BINARY_DIR}/projects/24_day/harib21f/stars.hrb ::stars.hrb
  COMMAND mcopy -i ${24_day_harib21f_OS} ${root_BINARY_DIR}/projects/24_day/harib21f/stars2.hrb ::stars2.hrb
  COMMAND mcopy -i ${24_day_harib21f_OS} ${root_BINARY_DIR}/projects/24_day/harib21f/lines.hrb ::lines.hrb
  COMMAND mcopy -i ${24_day_harib21f_OS} ${root_BINARY_DIR}/projects/24_day/harib21f/walk.hrb ::walk.hrb
  COMMAND mcopy -i ${24_day_harib21f_OS} ${root_BINARY_DIR}/projects/24_day/harib21f/hello.hrb ::hello.hrb
  COMMAND mcopy -i ${24_day_harib21f_OS} ${root_BINARY_DIR}/projects/24_day/harib21f/hello2.hrb ::hello2.hrb
  COMMAND mcopy -i ${24_day_harib21f_OS} ${root_BINARY_DIR}/projects/24_day/harib21f/hello5.hrb ::hello5.hrb
  DEPENDS 24_day_harib21f_sys ${root_BINARY_DIR}/projects/24_day/harib21f/a.hrb ${root_BINARY_DIR}/projects/24_day/harib21f/hello3.hrb ${root_BINARY_DIR}/projects/24_day/harib21f/hello4.hrb ${root_BINARY_DIR}/projects/24_day/harib21f/winhelo.hrb ${root_BINARY_DIR}/projects/24_day/harib21f/winhelo2.hrb ${root_BINARY_DIR}/projects/24_day/harib21f/winhelo3.hrb ${root_BINARY_DIR}/projects/24_day/harib21f/star1.hrb ${root_BINARY_DIR}/projects/24_day/harib21f/stars.hrb ${root_BINARY_DIR}/projects/24_day/harib21f/stars2.hrb ${root_BINARY_DIR}/projects/24_day/harib21f/lines.hrb ${root_BINARY_DIR}/projects/24_day/harib21f/walk.hrb ${root_BINARY_DIR}/projects/24_day/harib21f/hello.hrb ${root_BINARY_DIR}/projects/24_day/harib21f/hello2.hrb ${root_BINARY_DIR}/projects/24_day/harib21f/hello5.hrb
)

#----------------------------------------------------------
########### application .hrb files (copied into os.img) ###############
set(24_day_harib21f_APIO ${root_BINARY_DIR}/projects/24_day/harib21f/a_nask.o)
set(24_day_harib21f_RANDO ${root_BINARY_DIR}/projects/24_day/harib21f/rand.o)
add_custom_command(
  OUTPUT ${24_day_harib21f_RANDO}
  COMMAND gcc -fleading-underscore -m32 -nostdlib -fno-pie -x c -c ${root_SOURCE_DIR}/golibc/rand.c_ -o ${24_day_harib21f_RANDO}
  DEPENDS ${root_SOURCE_DIR}/golibc/rand.c_
  COMMENT "Generating rand.o for 24_day_harib21f"
)
add_custom_command(
  OUTPUT ${24_day_harib21f_APIO}
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/24_day/harib21f/a_nask.nas ${24_day_harib21f_APIO}
  DEPENDS ${root_SOURCE_DIR}/projects/24_day/harib21f/a_nask.nas
  COMMENT "Generating a_nask.o for 24_day_harib21f"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/24_day/harib21f/a.hrb
  COMMAND gcc ${BINOPT} -T ${24_day_harib21f_APPLDS} ${root_SOURCE_DIR}/projects/24_day/harib21f/a.c ${24_day_harib21f_APIO} ${24_day_harib21f_LIBGC} ${24_day_harib21f_RANDO} -o ${root_BINARY_DIR}/projects/24_day/harib21f/a.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/24_day/harib21f/a.c ${root_SOURCE_DIR}/projects/app.lds ${24_day_harib21f_APIO} ${24_day_harib21f_LIBGC} ${24_day_harib21f_RANDO}
  COMMENT "Generating a.hrb for 24_day_harib21f"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/24_day/harib21f/hello3.hrb
  COMMAND gcc ${BINOPT} -T ${24_day_harib21f_APPLDS} ${root_SOURCE_DIR}/projects/24_day/harib21f/hello3.c ${24_day_harib21f_APIO} ${24_day_harib21f_LIBGC} ${24_day_harib21f_RANDO} -o ${root_BINARY_DIR}/projects/24_day/harib21f/hello3.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/24_day/harib21f/hello3.c ${root_SOURCE_DIR}/projects/app.lds ${24_day_harib21f_APIO} ${24_day_harib21f_LIBGC} ${24_day_harib21f_RANDO}
  COMMENT "Generating hello3.hrb for 24_day_harib21f"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/24_day/harib21f/hello4.hrb
  COMMAND gcc ${BINOPT} -T ${24_day_harib21f_APPLDS} ${root_SOURCE_DIR}/projects/24_day/harib21f/hello4.c ${24_day_harib21f_APIO} ${24_day_harib21f_LIBGC} ${24_day_harib21f_RANDO} -o ${root_BINARY_DIR}/projects/24_day/harib21f/hello4.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/24_day/harib21f/hello4.c ${root_SOURCE_DIR}/projects/app.lds ${24_day_harib21f_APIO} ${24_day_harib21f_LIBGC} ${24_day_harib21f_RANDO}
  COMMENT "Generating hello4.hrb for 24_day_harib21f"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/24_day/harib21f/winhelo.hrb
  COMMAND gcc ${BINOPT} -T ${24_day_harib21f_APPLDS} ${root_SOURCE_DIR}/projects/24_day/harib21f/winhelo.c ${24_day_harib21f_APIO} ${24_day_harib21f_LIBGC} ${24_day_harib21f_RANDO} -o ${root_BINARY_DIR}/projects/24_day/harib21f/winhelo.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/24_day/harib21f/winhelo.c ${root_SOURCE_DIR}/projects/app.lds ${24_day_harib21f_APIO} ${24_day_harib21f_LIBGC} ${24_day_harib21f_RANDO}
  COMMENT "Generating winhelo.hrb for 24_day_harib21f"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/24_day/harib21f/winhelo2.hrb
  COMMAND gcc ${BINOPT} -T ${24_day_harib21f_APPLDS} ${root_SOURCE_DIR}/projects/24_day/harib21f/winhelo2.c ${24_day_harib21f_APIO} ${24_day_harib21f_LIBGC} ${24_day_harib21f_RANDO} -o ${root_BINARY_DIR}/projects/24_day/harib21f/winhelo2.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/24_day/harib21f/winhelo2.c ${root_SOURCE_DIR}/projects/app.lds ${24_day_harib21f_APIO} ${24_day_harib21f_LIBGC} ${24_day_harib21f_RANDO}
  COMMENT "Generating winhelo2.hrb for 24_day_harib21f"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/24_day/harib21f/winhelo3.hrb
  COMMAND gcc ${BINOPT} -T ${24_day_harib21f_APPLDS} ${root_SOURCE_DIR}/projects/24_day/harib21f/winhelo3.c ${24_day_harib21f_APIO} ${24_day_harib21f_LIBGC} ${24_day_harib21f_RANDO} -o ${root_BINARY_DIR}/projects/24_day/harib21f/winhelo3.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/24_day/harib21f/winhelo3.c ${root_SOURCE_DIR}/projects/app.lds ${24_day_harib21f_APIO} ${24_day_harib21f_LIBGC} ${24_day_harib21f_RANDO}
  COMMENT "Generating winhelo3.hrb for 24_day_harib21f"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/24_day/harib21f/star1.hrb
  COMMAND gcc ${BINOPT} -T ${24_day_harib21f_APPLDS} ${root_SOURCE_DIR}/projects/24_day/harib21f/star1.c ${24_day_harib21f_APIO} ${24_day_harib21f_LIBGC} ${24_day_harib21f_RANDO} -o ${root_BINARY_DIR}/projects/24_day/harib21f/star1.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/24_day/harib21f/star1.c ${root_SOURCE_DIR}/projects/app.lds ${24_day_harib21f_APIO} ${24_day_harib21f_LIBGC} ${24_day_harib21f_RANDO}
  COMMENT "Generating star1.hrb for 24_day_harib21f"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/24_day/harib21f/stars.hrb
  COMMAND gcc ${BINOPT} -T ${24_day_harib21f_APPLDS} ${root_SOURCE_DIR}/projects/24_day/harib21f/stars.c ${24_day_harib21f_APIO} ${24_day_harib21f_LIBGC} ${24_day_harib21f_RANDO} -o ${root_BINARY_DIR}/projects/24_day/harib21f/stars.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/24_day/harib21f/stars.c ${root_SOURCE_DIR}/projects/app.lds ${24_day_harib21f_APIO} ${24_day_harib21f_LIBGC} ${24_day_harib21f_RANDO}
  COMMENT "Generating stars.hrb for 24_day_harib21f"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/24_day/harib21f/stars2.hrb
  COMMAND gcc ${BINOPT} -T ${24_day_harib21f_APPLDS} ${root_SOURCE_DIR}/projects/24_day/harib21f/stars2.c ${24_day_harib21f_APIO} ${24_day_harib21f_LIBGC} ${24_day_harib21f_RANDO} -o ${root_BINARY_DIR}/projects/24_day/harib21f/stars2.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/24_day/harib21f/stars2.c ${root_SOURCE_DIR}/projects/app.lds ${24_day_harib21f_APIO} ${24_day_harib21f_LIBGC} ${24_day_harib21f_RANDO}
  COMMENT "Generating stars2.hrb for 24_day_harib21f"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/24_day/harib21f/lines.hrb
  COMMAND gcc ${BINOPT} -T ${24_day_harib21f_APPLDS} ${root_SOURCE_DIR}/projects/24_day/harib21f/lines.c ${24_day_harib21f_APIO} ${24_day_harib21f_LIBGC} ${24_day_harib21f_RANDO} -o ${root_BINARY_DIR}/projects/24_day/harib21f/lines.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/24_day/harib21f/lines.c ${root_SOURCE_DIR}/projects/app.lds ${24_day_harib21f_APIO} ${24_day_harib21f_LIBGC} ${24_day_harib21f_RANDO}
  COMMENT "Generating lines.hrb for 24_day_harib21f"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/24_day/harib21f/walk.hrb
  COMMAND gcc ${BINOPT} -T ${24_day_harib21f_APPLDS} ${root_SOURCE_DIR}/projects/24_day/harib21f/walk.c ${24_day_harib21f_APIO} ${24_day_harib21f_LIBGC} ${24_day_harib21f_RANDO} -o ${root_BINARY_DIR}/projects/24_day/harib21f/walk.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/24_day/harib21f/walk.c ${root_SOURCE_DIR}/projects/app.lds ${24_day_harib21f_APIO} ${24_day_harib21f_LIBGC} ${24_day_harib21f_RANDO}
  COMMENT "Generating walk.hrb for 24_day_harib21f"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/24_day/harib21f/hello.hrb
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/24_day/harib21f/hello.nas ${root_BINARY_DIR}/projects/24_day/harib21f/hello.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/24_day/harib21f/hello.nas
  COMMENT "Generating hello.hrb for 24_day_harib21f"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/24_day/harib21f/hello2.hrb
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/24_day/harib21f/hello2.nas ${root_BINARY_DIR}/projects/24_day/harib21f/hello2.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/24_day/harib21f/hello2.nas
  COMMENT "Generating hello2.hrb for 24_day_harib21f"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/24_day/harib21f/hello5.hrb
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/24_day/harib21f/hello5.nas ${root_BINARY_DIR}/projects/24_day/harib21f/hello5.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/24_day/harib21f/hello5.nas
  COMMENT "Generating hello5.hrb for 24_day_harib21f"
)
