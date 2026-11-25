#----------------------------------------------------------
message(STATUS "Entering directory projects/27_day/harib24e/")

set(FONT ${root_BINARY_DIR}/src/makefont)
set(B2O  ${root_BINARY_DIR}/src/bin2obj)
set(CONV ${root_BINARY_DIR}/objconv/objconv)
set(27_day_harib24e_OS    ${root_BINARY_DIR}/projects/27_day/harib24e/os.img)
set(27_day_harib24e_SYS	  ${root_BINARY_DIR}/projects/27_day/harib24e/os.sys)
set(27_day_harib24e_IPLB  ${root_BINARY_DIR}/projects/27_day/harib24e/ipl.bin)
set(27_day_harib24e_IPLS  ${root_SOURCE_DIR}/projects/27_day/harib24e/ipl10.nas)
set(27_day_harib24e_HEADB ${root_BINARY_DIR}/projects/27_day/harib24e/asmhead.bin)
set(27_day_harib24e_HEADS ${root_SOURCE_DIR}/projects/27_day/harib24e/asmhead.nas)
set(27_day_harib24e_BOOTB ${root_BINARY_DIR}/projects/27_day/harib24e/boot.bin)
set(27_day_harib24e_CCO	  ${root_BINARY_DIR}/projects/27_day/harib24e/boot.o)
file(GLOB 27_day_harib24e_CCS ${root_SOURCE_DIR}/projects/27_day/harib24e/*.c)
list(REMOVE_ITEM 27_day_harib24e_CCS
  ${root_SOURCE_DIR}/projects/27_day/harib24e/a.c
  ${root_SOURCE_DIR}/projects/27_day/harib24e/hello3.c
  ${root_SOURCE_DIR}/projects/27_day/harib24e/hello4.c
  ${root_SOURCE_DIR}/projects/27_day/harib24e/winhelo.c
  ${root_SOURCE_DIR}/projects/27_day/harib24e/winhelo2.c
  ${root_SOURCE_DIR}/projects/27_day/harib24e/winhelo3.c
  ${root_SOURCE_DIR}/projects/27_day/harib24e/star1.c
  ${root_SOURCE_DIR}/projects/27_day/harib24e/stars.c
  ${root_SOURCE_DIR}/projects/27_day/harib24e/stars2.c
  ${root_SOURCE_DIR}/projects/27_day/harib24e/lines.c
  ${root_SOURCE_DIR}/projects/27_day/harib24e/walk.c
  ${root_SOURCE_DIR}/projects/27_day/harib24e/noodle.c
  ${root_SOURCE_DIR}/projects/27_day/harib24e/beepdown.c
  ${root_SOURCE_DIR}/projects/27_day/harib24e/color.c
  ${root_SOURCE_DIR}/projects/27_day/harib24e/color2.c)
set(27_day_harib24e_LDS	  ${root_SOURCE_DIR}/projects/os.lds)
set(27_day_harib24e_APPLDS  ${root_SOURCE_DIR}/projects/app.lds)
set(27_day_harib24e_FUNCS ${root_SOURCE_DIR}/projects/27_day/harib24e/naskfunc.nas)
set(27_day_harib24e_FUNCO ${root_BINARY_DIR}/projects/27_day/harib24e/naskfunc.o)
set(27_day_harib24e_FONTS ${root_SOURCE_DIR}/projects/27_day/harib24e/hankaku.txt)
set(27_day_harib24e_FONTB ${root_BINARY_DIR}/projects/27_day/harib24e/hankaku.bin)
set(27_day_harib24e_FONTO ${root_BINARY_DIR}/projects/27_day/harib24e/hankaku.o)
set(27_day_harib24e_LIBGE ${root_BINARY_DIR}/golibc/libgo.a)
set(27_day_harib24e_LIBGC ${root_BINARY_DIR}/projects/27_day/harib24e/libgo.o)
set(27_day_harib24e_WILDOBJ ${root_BINARY_DIR}/projects/27_day/harib24e/*.o)


add_custom_target(27_day_harib24e_run
  COMMAND ${QEMU} ${QEMUOPT} ${27_day_harib24e_OS}
  DEPENDS 27_day_harib24e_img
)
add_custom_target(27_day_harib24e_debug
  COMMAND ${QEMU} -s -S ${QEMUOPT} ${27_day_harib24e_OS} ${QEMU_DEBUG_OPT}
  DEPENDS 27_day_harib24e_img
)
add_custom_target(27_day_harib24e_clean
  COMMAND rm -f ${27_day_harib24e_OS}
  COMMAND rm -f ${27_day_harib24e_SYS}
  COMMAND rm -f ${27_day_harib24e_IPLB}
  COMMAND rm -f ${27_day_harib24e_HEADB}
  COMMAND rm -f ${27_day_harib24e_BOOTB}
  COMMAND rm -f ${27_day_harib24e_WILDOBJ}
)
add_custom_target(27_day_harib24e_ipl
  COMMAND ${RAKUSK_EXECUTABLE} ${27_day_harib24e_IPLS} ${27_day_harib24e_IPLB}
  DEPENDS ${27_day_harib24e_IPLS}
)
add_custom_command(
  OUTPUT ${27_day_harib24e_HEADB}
  COMMAND ${RAKUSK_EXECUTABLE} ${27_day_harib24e_HEADS} ${27_day_harib24e_HEADB}
  DEPENDS ${27_day_harib24e_HEADS}
  COMMENT "Generating asmhead.bin for 27_day_harib24e"
)
add_custom_command(
  OUTPUT ${27_day_harib24e_FUNCO}
  COMMAND ${RAKUSK_EXECUTABLE} ${27_day_harib24e_FUNCS} ${27_day_harib24e_FUNCO}
  DEPENDS ${27_day_harib24e_FUNCS}
  COMMENT "Generating naskfunc.o for 27_day_harib24e"
)
add_custom_command(
  OUTPUT ${27_day_harib24e_FONTO}
  COMMAND ${FONT} ${27_day_harib24e_FONTS} ${27_day_harib24e_FONTB}
  COMMAND ${B2O}  ${27_day_harib24e_FONTB} ${27_day_harib24e_FONTO} _hankaku
  DEPENDS ${27_day_harib24e_FONTS} makefont bin2obj
  COMMENT "Generating hankaku.o for 27_day_harib24e"
)
add_custom_command(
  OUTPUT ${27_day_harib24e_LIBGC}
  COMMAND ${CONV} -fcoff32 -nu ${27_day_harib24e_LIBGE} ${27_day_harib24e_LIBGC}
  DEPENDS ${27_day_harib24e_LIBGE}
  COMMENT "Converting golibc for 27_day_harib24e"
)

add_custom_target(27_day_harib24e_sys
  COMMAND gcc ${BINOPT} -T ${27_day_harib24e_LDS} ${27_day_harib24e_CCS} ${27_day_harib24e_WILDOBJ} -o ${27_day_harib24e_BOOTB}
  COMMAND cat ${27_day_harib24e_HEADB} ${27_day_harib24e_BOOTB} > ${27_day_harib24e_SYS}
  DEPENDS ${27_day_harib24e_HEADB} 27_day_harib24e_ipl
  DEPENDS ${27_day_harib24e_FUNCO}
  DEPENDS ${27_day_harib24e_FONTO} ${27_day_harib24e_LIBGC}
)
add_custom_target(27_day_harib24e_img
  COMMAND mformat -f 1440 -l HARIBOTEOS -N 0xffffffff -C -B ${27_day_harib24e_IPLB} -i ${27_day_harib24e_OS}
  COMMAND mcopy -i ${27_day_harib24e_OS} ${27_day_harib24e_SYS} ::
  COMMAND mcopy -i ${27_day_harib24e_OS} ${root_BINARY_DIR}/projects/27_day/harib24e/a.hrb ::a.hrb
  COMMAND mcopy -i ${27_day_harib24e_OS} ${root_BINARY_DIR}/projects/27_day/harib24e/hello3.hrb ::hello3.hrb
  COMMAND mcopy -i ${27_day_harib24e_OS} ${root_BINARY_DIR}/projects/27_day/harib24e/hello4.hrb ::hello4.hrb
  COMMAND mcopy -i ${27_day_harib24e_OS} ${root_BINARY_DIR}/projects/27_day/harib24e/winhelo.hrb ::winhelo.hrb
  COMMAND mcopy -i ${27_day_harib24e_OS} ${root_BINARY_DIR}/projects/27_day/harib24e/winhelo2.hrb ::winhelo2.hrb
  COMMAND mcopy -i ${27_day_harib24e_OS} ${root_BINARY_DIR}/projects/27_day/harib24e/winhelo3.hrb ::winhelo3.hrb
  COMMAND mcopy -i ${27_day_harib24e_OS} ${root_BINARY_DIR}/projects/27_day/harib24e/star1.hrb ::star1.hrb
  COMMAND mcopy -i ${27_day_harib24e_OS} ${root_BINARY_DIR}/projects/27_day/harib24e/stars.hrb ::stars.hrb
  COMMAND mcopy -i ${27_day_harib24e_OS} ${root_BINARY_DIR}/projects/27_day/harib24e/stars2.hrb ::stars2.hrb
  COMMAND mcopy -i ${27_day_harib24e_OS} ${root_BINARY_DIR}/projects/27_day/harib24e/lines.hrb ::lines.hrb
  COMMAND mcopy -i ${27_day_harib24e_OS} ${root_BINARY_DIR}/projects/27_day/harib24e/walk.hrb ::walk.hrb
  COMMAND mcopy -i ${27_day_harib24e_OS} ${root_BINARY_DIR}/projects/27_day/harib24e/color2.hrb ::color2.hrb
  COMMAND mcopy -i ${27_day_harib24e_OS} ${root_BINARY_DIR}/projects/27_day/harib24e/color.hrb ::color.hrb
  COMMAND mcopy -i ${27_day_harib24e_OS} ${root_BINARY_DIR}/projects/27_day/harib24e/beepdown.hrb ::beepdown.hrb
  COMMAND mcopy -i ${27_day_harib24e_OS} ${root_BINARY_DIR}/projects/27_day/harib24e/noodle.hrb ::noodle.hrb
  COMMAND mcopy -i ${27_day_harib24e_OS} ${root_BINARY_DIR}/projects/27_day/harib24e/hello.hrb ::hello.hrb
  COMMAND mcopy -i ${27_day_harib24e_OS} ${root_BINARY_DIR}/projects/27_day/harib24e/hello2.hrb ::hello2.hrb
  COMMAND mcopy -i ${27_day_harib24e_OS} ${root_BINARY_DIR}/projects/27_day/harib24e/hello5.hrb ::hello5.hrb
  COMMAND mcopy -i ${27_day_harib24e_OS} ${root_BINARY_DIR}/projects/27_day/harib24e/crack7.hrb ::crack7.hrb
  DEPENDS 27_day_harib24e_sys ${root_BINARY_DIR}/projects/27_day/harib24e/a.hrb ${root_BINARY_DIR}/projects/27_day/harib24e/hello3.hrb ${root_BINARY_DIR}/projects/27_day/harib24e/hello4.hrb ${root_BINARY_DIR}/projects/27_day/harib24e/winhelo.hrb ${root_BINARY_DIR}/projects/27_day/harib24e/winhelo2.hrb ${root_BINARY_DIR}/projects/27_day/harib24e/winhelo3.hrb ${root_BINARY_DIR}/projects/27_day/harib24e/star1.hrb ${root_BINARY_DIR}/projects/27_day/harib24e/stars.hrb ${root_BINARY_DIR}/projects/27_day/harib24e/stars2.hrb ${root_BINARY_DIR}/projects/27_day/harib24e/lines.hrb ${root_BINARY_DIR}/projects/27_day/harib24e/walk.hrb ${root_BINARY_DIR}/projects/27_day/harib24e/noodle.hrb ${root_BINARY_DIR}/projects/27_day/harib24e/beepdown.hrb ${root_BINARY_DIR}/projects/27_day/harib24e/color.hrb ${root_BINARY_DIR}/projects/27_day/harib24e/color2.hrb ${root_BINARY_DIR}/projects/27_day/harib24e/hello.hrb ${root_BINARY_DIR}/projects/27_day/harib24e/hello2.hrb ${root_BINARY_DIR}/projects/27_day/harib24e/hello5.hrb ${root_BINARY_DIR}/projects/27_day/harib24e/crack7.hrb
)

#----------------------------------------------------------
########### application .hrb files (copied into os.img) ###############
set(27_day_harib24e_APIO
  ${root_BINARY_DIR}/projects/27_day/harib24e/api001.o
  ${root_BINARY_DIR}/projects/27_day/harib24e/api002.o
  ${root_BINARY_DIR}/projects/27_day/harib24e/api003.o
  ${root_BINARY_DIR}/projects/27_day/harib24e/api004.o
  ${root_BINARY_DIR}/projects/27_day/harib24e/api005.o
  ${root_BINARY_DIR}/projects/27_day/harib24e/api006.o
  ${root_BINARY_DIR}/projects/27_day/harib24e/api007.o
  ${root_BINARY_DIR}/projects/27_day/harib24e/api008.o
  ${root_BINARY_DIR}/projects/27_day/harib24e/api009.o
  ${root_BINARY_DIR}/projects/27_day/harib24e/api010.o
  ${root_BINARY_DIR}/projects/27_day/harib24e/api011.o
  ${root_BINARY_DIR}/projects/27_day/harib24e/api012.o
  ${root_BINARY_DIR}/projects/27_day/harib24e/api013.o
  ${root_BINARY_DIR}/projects/27_day/harib24e/api014.o
  ${root_BINARY_DIR}/projects/27_day/harib24e/api015.o
  ${root_BINARY_DIR}/projects/27_day/harib24e/api016.o
  ${root_BINARY_DIR}/projects/27_day/harib24e/api017.o
  ${root_BINARY_DIR}/projects/27_day/harib24e/api018.o
  ${root_BINARY_DIR}/projects/27_day/harib24e/api019.o
  ${root_BINARY_DIR}/projects/27_day/harib24e/api020.o)
set(27_day_harib24e_RANDO ${root_BINARY_DIR}/projects/27_day/harib24e/rand.o)
add_custom_command(
  OUTPUT ${27_day_harib24e_RANDO}
  COMMAND gcc -fleading-underscore -m32 -nostdlib -fno-pie -x c -c ${root_SOURCE_DIR}/golibc/rand.c_ -o ${27_day_harib24e_RANDO}
  DEPENDS ${root_SOURCE_DIR}/golibc/rand.c_
  COMMENT "Generating rand.o for 27_day_harib24e"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/27_day/harib24e/api001.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/27_day/harib24e/api001.nas ${root_BINARY_DIR}/projects/27_day/harib24e/api001.o
  DEPENDS ${root_SOURCE_DIR}/projects/27_day/harib24e/api001.nas
  COMMENT "Generating api001.o for 27_day_harib24e"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/27_day/harib24e/api002.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/27_day/harib24e/api002.nas ${root_BINARY_DIR}/projects/27_day/harib24e/api002.o
  DEPENDS ${root_SOURCE_DIR}/projects/27_day/harib24e/api002.nas
  COMMENT "Generating api002.o for 27_day_harib24e"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/27_day/harib24e/api003.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/27_day/harib24e/api003.nas ${root_BINARY_DIR}/projects/27_day/harib24e/api003.o
  DEPENDS ${root_SOURCE_DIR}/projects/27_day/harib24e/api003.nas
  COMMENT "Generating api003.o for 27_day_harib24e"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/27_day/harib24e/api004.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/27_day/harib24e/api004.nas ${root_BINARY_DIR}/projects/27_day/harib24e/api004.o
  DEPENDS ${root_SOURCE_DIR}/projects/27_day/harib24e/api004.nas
  COMMENT "Generating api004.o for 27_day_harib24e"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/27_day/harib24e/api005.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/27_day/harib24e/api005.nas ${root_BINARY_DIR}/projects/27_day/harib24e/api005.o
  DEPENDS ${root_SOURCE_DIR}/projects/27_day/harib24e/api005.nas
  COMMENT "Generating api005.o for 27_day_harib24e"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/27_day/harib24e/api006.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/27_day/harib24e/api006.nas ${root_BINARY_DIR}/projects/27_day/harib24e/api006.o
  DEPENDS ${root_SOURCE_DIR}/projects/27_day/harib24e/api006.nas
  COMMENT "Generating api006.o for 27_day_harib24e"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/27_day/harib24e/api007.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/27_day/harib24e/api007.nas ${root_BINARY_DIR}/projects/27_day/harib24e/api007.o
  DEPENDS ${root_SOURCE_DIR}/projects/27_day/harib24e/api007.nas
  COMMENT "Generating api007.o for 27_day_harib24e"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/27_day/harib24e/api008.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/27_day/harib24e/api008.nas ${root_BINARY_DIR}/projects/27_day/harib24e/api008.o
  DEPENDS ${root_SOURCE_DIR}/projects/27_day/harib24e/api008.nas
  COMMENT "Generating api008.o for 27_day_harib24e"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/27_day/harib24e/api009.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/27_day/harib24e/api009.nas ${root_BINARY_DIR}/projects/27_day/harib24e/api009.o
  DEPENDS ${root_SOURCE_DIR}/projects/27_day/harib24e/api009.nas
  COMMENT "Generating api009.o for 27_day_harib24e"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/27_day/harib24e/api010.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/27_day/harib24e/api010.nas ${root_BINARY_DIR}/projects/27_day/harib24e/api010.o
  DEPENDS ${root_SOURCE_DIR}/projects/27_day/harib24e/api010.nas
  COMMENT "Generating api010.o for 27_day_harib24e"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/27_day/harib24e/api011.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/27_day/harib24e/api011.nas ${root_BINARY_DIR}/projects/27_day/harib24e/api011.o
  DEPENDS ${root_SOURCE_DIR}/projects/27_day/harib24e/api011.nas
  COMMENT "Generating api011.o for 27_day_harib24e"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/27_day/harib24e/api012.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/27_day/harib24e/api012.nas ${root_BINARY_DIR}/projects/27_day/harib24e/api012.o
  DEPENDS ${root_SOURCE_DIR}/projects/27_day/harib24e/api012.nas
  COMMENT "Generating api012.o for 27_day_harib24e"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/27_day/harib24e/api013.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/27_day/harib24e/api013.nas ${root_BINARY_DIR}/projects/27_day/harib24e/api013.o
  DEPENDS ${root_SOURCE_DIR}/projects/27_day/harib24e/api013.nas
  COMMENT "Generating api013.o for 27_day_harib24e"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/27_day/harib24e/api014.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/27_day/harib24e/api014.nas ${root_BINARY_DIR}/projects/27_day/harib24e/api014.o
  DEPENDS ${root_SOURCE_DIR}/projects/27_day/harib24e/api014.nas
  COMMENT "Generating api014.o for 27_day_harib24e"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/27_day/harib24e/api015.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/27_day/harib24e/api015.nas ${root_BINARY_DIR}/projects/27_day/harib24e/api015.o
  DEPENDS ${root_SOURCE_DIR}/projects/27_day/harib24e/api015.nas
  COMMENT "Generating api015.o for 27_day_harib24e"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/27_day/harib24e/api016.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/27_day/harib24e/api016.nas ${root_BINARY_DIR}/projects/27_day/harib24e/api016.o
  DEPENDS ${root_SOURCE_DIR}/projects/27_day/harib24e/api016.nas
  COMMENT "Generating api016.o for 27_day_harib24e"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/27_day/harib24e/api017.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/27_day/harib24e/api017.nas ${root_BINARY_DIR}/projects/27_day/harib24e/api017.o
  DEPENDS ${root_SOURCE_DIR}/projects/27_day/harib24e/api017.nas
  COMMENT "Generating api017.o for 27_day_harib24e"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/27_day/harib24e/api018.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/27_day/harib24e/api018.nas ${root_BINARY_DIR}/projects/27_day/harib24e/api018.o
  DEPENDS ${root_SOURCE_DIR}/projects/27_day/harib24e/api018.nas
  COMMENT "Generating api018.o for 27_day_harib24e"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/27_day/harib24e/api019.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/27_day/harib24e/api019.nas ${root_BINARY_DIR}/projects/27_day/harib24e/api019.o
  DEPENDS ${root_SOURCE_DIR}/projects/27_day/harib24e/api019.nas
  COMMENT "Generating api019.o for 27_day_harib24e"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/27_day/harib24e/api020.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/27_day/harib24e/api020.nas ${root_BINARY_DIR}/projects/27_day/harib24e/api020.o
  DEPENDS ${root_SOURCE_DIR}/projects/27_day/harib24e/api020.nas
  COMMENT "Generating api020.o for 27_day_harib24e"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/27_day/harib24e/a.hrb
  COMMAND gcc ${BINOPT} -T ${27_day_harib24e_APPLDS} ${root_SOURCE_DIR}/projects/27_day/harib24e/a.c ${27_day_harib24e_APIO} ${27_day_harib24e_LIBGC} ${27_day_harib24e_RANDO} -o ${root_BINARY_DIR}/projects/27_day/harib24e/a.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/27_day/harib24e/a.c ${root_SOURCE_DIR}/projects/app.lds ${27_day_harib24e_APIO} ${27_day_harib24e_LIBGC} ${27_day_harib24e_RANDO}
  COMMENT "Generating a.hrb for 27_day_harib24e"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/27_day/harib24e/hello3.hrb
  COMMAND gcc ${BINOPT} -T ${27_day_harib24e_APPLDS} ${root_SOURCE_DIR}/projects/27_day/harib24e/hello3.c ${27_day_harib24e_APIO} ${27_day_harib24e_LIBGC} ${27_day_harib24e_RANDO} -o ${root_BINARY_DIR}/projects/27_day/harib24e/hello3.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/27_day/harib24e/hello3.c ${root_SOURCE_DIR}/projects/app.lds ${27_day_harib24e_APIO} ${27_day_harib24e_LIBGC} ${27_day_harib24e_RANDO}
  COMMENT "Generating hello3.hrb for 27_day_harib24e"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/27_day/harib24e/hello4.hrb
  COMMAND gcc ${BINOPT} -T ${27_day_harib24e_APPLDS} ${root_SOURCE_DIR}/projects/27_day/harib24e/hello4.c ${27_day_harib24e_APIO} ${27_day_harib24e_LIBGC} ${27_day_harib24e_RANDO} -o ${root_BINARY_DIR}/projects/27_day/harib24e/hello4.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/27_day/harib24e/hello4.c ${root_SOURCE_DIR}/projects/app.lds ${27_day_harib24e_APIO} ${27_day_harib24e_LIBGC} ${27_day_harib24e_RANDO}
  COMMENT "Generating hello4.hrb for 27_day_harib24e"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/27_day/harib24e/winhelo.hrb
  COMMAND gcc ${BINOPT} -T ${27_day_harib24e_APPLDS} ${root_SOURCE_DIR}/projects/27_day/harib24e/winhelo.c ${27_day_harib24e_APIO} ${27_day_harib24e_LIBGC} ${27_day_harib24e_RANDO} -o ${root_BINARY_DIR}/projects/27_day/harib24e/winhelo.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/27_day/harib24e/winhelo.c ${root_SOURCE_DIR}/projects/app.lds ${27_day_harib24e_APIO} ${27_day_harib24e_LIBGC} ${27_day_harib24e_RANDO}
  COMMENT "Generating winhelo.hrb for 27_day_harib24e"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/27_day/harib24e/winhelo2.hrb
  COMMAND gcc ${BINOPT} -T ${27_day_harib24e_APPLDS} ${root_SOURCE_DIR}/projects/27_day/harib24e/winhelo2.c ${27_day_harib24e_APIO} ${27_day_harib24e_LIBGC} ${27_day_harib24e_RANDO} -o ${root_BINARY_DIR}/projects/27_day/harib24e/winhelo2.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/27_day/harib24e/winhelo2.c ${root_SOURCE_DIR}/projects/app.lds ${27_day_harib24e_APIO} ${27_day_harib24e_LIBGC} ${27_day_harib24e_RANDO}
  COMMENT "Generating winhelo2.hrb for 27_day_harib24e"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/27_day/harib24e/winhelo3.hrb
  COMMAND gcc ${BINOPT} -T ${27_day_harib24e_APPLDS} ${root_SOURCE_DIR}/projects/27_day/harib24e/winhelo3.c ${27_day_harib24e_APIO} ${27_day_harib24e_LIBGC} ${27_day_harib24e_RANDO} -o ${root_BINARY_DIR}/projects/27_day/harib24e/winhelo3.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/27_day/harib24e/winhelo3.c ${root_SOURCE_DIR}/projects/app.lds ${27_day_harib24e_APIO} ${27_day_harib24e_LIBGC} ${27_day_harib24e_RANDO}
  COMMENT "Generating winhelo3.hrb for 27_day_harib24e"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/27_day/harib24e/star1.hrb
  COMMAND gcc ${BINOPT} -T ${27_day_harib24e_APPLDS} ${root_SOURCE_DIR}/projects/27_day/harib24e/star1.c ${27_day_harib24e_APIO} ${27_day_harib24e_LIBGC} ${27_day_harib24e_RANDO} -o ${root_BINARY_DIR}/projects/27_day/harib24e/star1.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/27_day/harib24e/star1.c ${root_SOURCE_DIR}/projects/app.lds ${27_day_harib24e_APIO} ${27_day_harib24e_LIBGC} ${27_day_harib24e_RANDO}
  COMMENT "Generating star1.hrb for 27_day_harib24e"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/27_day/harib24e/stars.hrb
  COMMAND gcc ${BINOPT} -T ${27_day_harib24e_APPLDS} ${root_SOURCE_DIR}/projects/27_day/harib24e/stars.c ${27_day_harib24e_APIO} ${27_day_harib24e_LIBGC} ${27_day_harib24e_RANDO} -o ${root_BINARY_DIR}/projects/27_day/harib24e/stars.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/27_day/harib24e/stars.c ${root_SOURCE_DIR}/projects/app.lds ${27_day_harib24e_APIO} ${27_day_harib24e_LIBGC} ${27_day_harib24e_RANDO}
  COMMENT "Generating stars.hrb for 27_day_harib24e"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/27_day/harib24e/stars2.hrb
  COMMAND gcc ${BINOPT} -T ${27_day_harib24e_APPLDS} ${root_SOURCE_DIR}/projects/27_day/harib24e/stars2.c ${27_day_harib24e_APIO} ${27_day_harib24e_LIBGC} ${27_day_harib24e_RANDO} -o ${root_BINARY_DIR}/projects/27_day/harib24e/stars2.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/27_day/harib24e/stars2.c ${root_SOURCE_DIR}/projects/app.lds ${27_day_harib24e_APIO} ${27_day_harib24e_LIBGC} ${27_day_harib24e_RANDO}
  COMMENT "Generating stars2.hrb for 27_day_harib24e"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/27_day/harib24e/lines.hrb
  COMMAND gcc ${BINOPT} -T ${27_day_harib24e_APPLDS} ${root_SOURCE_DIR}/projects/27_day/harib24e/lines.c ${27_day_harib24e_APIO} ${27_day_harib24e_LIBGC} ${27_day_harib24e_RANDO} -o ${root_BINARY_DIR}/projects/27_day/harib24e/lines.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/27_day/harib24e/lines.c ${root_SOURCE_DIR}/projects/app.lds ${27_day_harib24e_APIO} ${27_day_harib24e_LIBGC} ${27_day_harib24e_RANDO}
  COMMENT "Generating lines.hrb for 27_day_harib24e"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/27_day/harib24e/walk.hrb
  COMMAND gcc ${BINOPT} -T ${27_day_harib24e_APPLDS} ${root_SOURCE_DIR}/projects/27_day/harib24e/walk.c ${27_day_harib24e_APIO} ${27_day_harib24e_LIBGC} ${27_day_harib24e_RANDO} -o ${root_BINARY_DIR}/projects/27_day/harib24e/walk.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/27_day/harib24e/walk.c ${root_SOURCE_DIR}/projects/app.lds ${27_day_harib24e_APIO} ${27_day_harib24e_LIBGC} ${27_day_harib24e_RANDO}
  COMMENT "Generating walk.hrb for 27_day_harib24e"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/27_day/harib24e/color2.hrb
  COMMAND gcc ${BINOPT} -T ${27_day_harib24e_APPLDS} ${root_SOURCE_DIR}/projects/27_day/harib24e/color2.c ${27_day_harib24e_APIO} ${27_day_harib24e_LIBGC} ${27_day_harib24e_RANDO} -o ${root_BINARY_DIR}/projects/27_day/harib24e/color2.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/27_day/harib24e/color2.c ${root_SOURCE_DIR}/projects/app.lds ${27_day_harib24e_APIO} ${27_day_harib24e_LIBGC} ${27_day_harib24e_RANDO}
  COMMENT "Generating color2.hrb for 27_day_harib24e"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/27_day/harib24e/color.hrb
  COMMAND gcc ${BINOPT} -T ${27_day_harib24e_APPLDS} ${root_SOURCE_DIR}/projects/27_day/harib24e/color.c ${27_day_harib24e_APIO} ${27_day_harib24e_LIBGC} ${27_day_harib24e_RANDO} -o ${root_BINARY_DIR}/projects/27_day/harib24e/color.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/27_day/harib24e/color.c ${root_SOURCE_DIR}/projects/app.lds ${27_day_harib24e_APIO} ${27_day_harib24e_LIBGC} ${27_day_harib24e_RANDO}
  COMMENT "Generating color.hrb for 27_day_harib24e"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/27_day/harib24e/beepdown.hrb
  COMMAND gcc ${BINOPT} -T ${27_day_harib24e_APPLDS} ${root_SOURCE_DIR}/projects/27_day/harib24e/beepdown.c ${27_day_harib24e_APIO} ${27_day_harib24e_LIBGC} ${27_day_harib24e_RANDO} -o ${root_BINARY_DIR}/projects/27_day/harib24e/beepdown.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/27_day/harib24e/beepdown.c ${root_SOURCE_DIR}/projects/app.lds ${27_day_harib24e_APIO} ${27_day_harib24e_LIBGC} ${27_day_harib24e_RANDO}
  COMMENT "Generating beepdown.hrb for 27_day_harib24e"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/27_day/harib24e/noodle.hrb
  COMMAND gcc ${BINOPT} -T ${27_day_harib24e_APPLDS} ${root_SOURCE_DIR}/projects/27_day/harib24e/noodle.c ${27_day_harib24e_APIO} ${27_day_harib24e_LIBGC} ${27_day_harib24e_RANDO} -o ${root_BINARY_DIR}/projects/27_day/harib24e/noodle.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/27_day/harib24e/noodle.c ${root_SOURCE_DIR}/projects/app.lds ${27_day_harib24e_APIO} ${27_day_harib24e_LIBGC} ${27_day_harib24e_RANDO}
  COMMENT "Generating noodle.hrb for 27_day_harib24e"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/27_day/harib24e/hello.hrb
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/27_day/harib24e/hello.nas ${root_BINARY_DIR}/projects/27_day/harib24e/hello.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/27_day/harib24e/hello.nas
  COMMENT "Generating hello.hrb for 27_day_harib24e"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/27_day/harib24e/crack7.hrb
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/27_day/harib24e/crack7.nas ${root_BINARY_DIR}/projects/27_day/harib24e/crack7.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/27_day/harib24e/crack7.nas
  COMMENT "Generating crack7.hrb for 27_day_harib24e"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/27_day/harib24e/hello2.hrb
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/27_day/harib24e/hello2.nas ${root_BINARY_DIR}/projects/27_day/harib24e/hello2.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/27_day/harib24e/hello2.nas
  COMMENT "Generating hello2.hrb for 27_day_harib24e"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/27_day/harib24e/hello5.hrb
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/27_day/harib24e/hello5.nas ${root_BINARY_DIR}/projects/27_day/harib24e/hello5.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/27_day/harib24e/hello5.nas
  COMMENT "Generating hello5.hrb for 27_day_harib24e"
)
