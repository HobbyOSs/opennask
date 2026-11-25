#----------------------------------------------------------
message(STATUS "Entering directory projects/27_day/harib24g/")

set(FONT ${root_BINARY_DIR}/src/makefont)
set(B2O  ${root_BINARY_DIR}/src/bin2obj)
set(CONV ${root_BINARY_DIR}/objconv/objconv)
set(27_day_harib24g_OS    ${root_BINARY_DIR}/projects/27_day/harib24g/os.img)
set(27_day_harib24g_SYS	  ${root_BINARY_DIR}/projects/27_day/harib24g/os.sys)
set(27_day_harib24g_IPLB  ${root_BINARY_DIR}/projects/27_day/harib24g/ipl.bin)
set(27_day_harib24g_IPLS  ${root_SOURCE_DIR}/projects/27_day/harib24g/haribote/ipl10.nas)
set(27_day_harib24g_HEADB ${root_BINARY_DIR}/projects/27_day/harib24g/asmhead.bin)
set(27_day_harib24g_HEADS ${root_SOURCE_DIR}/projects/27_day/harib24g/haribote/asmhead.nas)
set(27_day_harib24g_BOOTB ${root_BINARY_DIR}/projects/27_day/harib24g/boot.bin)
set(27_day_harib24g_CCO	  ${root_BINARY_DIR}/projects/27_day/harib24g/boot.o)
file(GLOB 27_day_harib24g_CCS ${root_SOURCE_DIR}/projects/27_day/harib24g/haribote/*.c)
set(27_day_harib24g_LDS	  ${root_SOURCE_DIR}/projects/os.lds)
set(27_day_harib24g_APPLDS  ${root_SOURCE_DIR}/projects/app.lds)
set(27_day_harib24g_FUNCS ${root_SOURCE_DIR}/projects/27_day/harib24g/haribote/naskfunc.nas)
set(27_day_harib24g_FUNCO ${root_BINARY_DIR}/projects/27_day/harib24g/naskfunc.o)
set(27_day_harib24g_FONTS ${root_SOURCE_DIR}/projects/27_day/harib24g/haribote/hankaku.txt)
set(27_day_harib24g_FONTB ${root_BINARY_DIR}/projects/27_day/harib24g/hankaku.bin)
set(27_day_harib24g_FONTO ${root_BINARY_DIR}/projects/27_day/harib24g/hankaku.o)
set(27_day_harib24g_LIBGE ${root_BINARY_DIR}/golibc/libgo.a)
set(27_day_harib24g_LIBGC ${root_BINARY_DIR}/projects/27_day/harib24g/libgo.o)
set(27_day_harib24g_WILDOBJ ${root_BINARY_DIR}/projects/27_day/harib24g/*.o)


add_custom_target(27_day_harib24g_run
  COMMAND ${QEMU} ${QEMUOPT} ${27_day_harib24g_OS}
  DEPENDS 27_day_harib24g_img
)
add_custom_target(27_day_harib24g_debug
  COMMAND ${QEMU} -s -S ${QEMUOPT} ${27_day_harib24g_OS} ${QEMU_DEBUG_OPT}
  DEPENDS 27_day_harib24g_img
)
add_custom_target(27_day_harib24g_clean
  COMMAND rm -f ${27_day_harib24g_OS}
  COMMAND rm -f ${27_day_harib24g_SYS}
  COMMAND rm -f ${27_day_harib24g_IPLB}
  COMMAND rm -f ${27_day_harib24g_HEADB}
  COMMAND rm -f ${27_day_harib24g_BOOTB}
  COMMAND rm -f ${27_day_harib24g_WILDOBJ}
)
add_custom_target(27_day_harib24g_ipl
  COMMAND ${RAKUSK_EXECUTABLE} ${27_day_harib24g_IPLS} ${27_day_harib24g_IPLB}
  DEPENDS ${27_day_harib24g_IPLS}
)
add_custom_command(
  OUTPUT ${27_day_harib24g_HEADB}
  COMMAND ${RAKUSK_EXECUTABLE} ${27_day_harib24g_HEADS} ${27_day_harib24g_HEADB}
  DEPENDS ${27_day_harib24g_HEADS}
  COMMENT "Generating asmhead.bin for 27_day_harib24g"
)
add_custom_command(
  OUTPUT ${27_day_harib24g_FUNCO}
  COMMAND ${RAKUSK_EXECUTABLE} ${27_day_harib24g_FUNCS} ${27_day_harib24g_FUNCO}
  DEPENDS ${27_day_harib24g_FUNCS}
  COMMENT "Generating naskfunc.o for 27_day_harib24g"
)
add_custom_command(
  OUTPUT ${27_day_harib24g_FONTO}
  COMMAND ${FONT} ${27_day_harib24g_FONTS} ${27_day_harib24g_FONTB}
  COMMAND ${B2O}  ${27_day_harib24g_FONTB} ${27_day_harib24g_FONTO} _hankaku
  DEPENDS ${27_day_harib24g_FONTS} makefont bin2obj
  COMMENT "Generating hankaku.o for 27_day_harib24g"
)
add_custom_command(
  OUTPUT ${27_day_harib24g_LIBGC}
  COMMAND ${CONV} -fcoff32 -nu ${27_day_harib24g_LIBGE} ${27_day_harib24g_LIBGC}
  DEPENDS ${27_day_harib24g_LIBGE}
  COMMENT "Converting golibc for 27_day_harib24g"
)

add_custom_target(27_day_harib24g_sys
  COMMAND gcc ${BINOPT} -T ${27_day_harib24g_LDS} ${27_day_harib24g_CCS} ${27_day_harib24g_WILDOBJ} -o ${27_day_harib24g_BOOTB}
  COMMAND cat ${27_day_harib24g_HEADB} ${27_day_harib24g_BOOTB} > ${27_day_harib24g_SYS}
  DEPENDS ${27_day_harib24g_HEADB} 27_day_harib24g_ipl
  DEPENDS ${27_day_harib24g_FUNCO}
  DEPENDS ${27_day_harib24g_FONTO} ${27_day_harib24g_LIBGC}
)
add_custom_target(27_day_harib24g_img
  COMMAND mformat -f 1440 -l HARIBOTEOS -N 0xffffffff -C -B ${27_day_harib24g_IPLB} -i ${27_day_harib24g_OS}
  COMMAND mcopy -i ${27_day_harib24g_OS} ${27_day_harib24g_SYS} ::
  COMMAND mcopy -i ${27_day_harib24g_OS} ${root_BINARY_DIR}/projects/27_day/harib24g/a.hrb ::a.hrb
  COMMAND mcopy -i ${27_day_harib24g_OS} ${root_BINARY_DIR}/projects/27_day/harib24g/hello3.hrb ::hello3.hrb
  COMMAND mcopy -i ${27_day_harib24g_OS} ${root_BINARY_DIR}/projects/27_day/harib24g/hello4.hrb ::hello4.hrb
  COMMAND mcopy -i ${27_day_harib24g_OS} ${root_BINARY_DIR}/projects/27_day/harib24g/winhelo.hrb ::winhelo.hrb
  COMMAND mcopy -i ${27_day_harib24g_OS} ${root_BINARY_DIR}/projects/27_day/harib24g/winhelo2.hrb ::winhelo2.hrb
  COMMAND mcopy -i ${27_day_harib24g_OS} ${root_BINARY_DIR}/projects/27_day/harib24g/winhelo3.hrb ::winhelo3.hrb
  COMMAND mcopy -i ${27_day_harib24g_OS} ${root_BINARY_DIR}/projects/27_day/harib24g/star1.hrb ::star1.hrb
  COMMAND mcopy -i ${27_day_harib24g_OS} ${root_BINARY_DIR}/projects/27_day/harib24g/stars.hrb ::stars.hrb
  COMMAND mcopy -i ${27_day_harib24g_OS} ${root_BINARY_DIR}/projects/27_day/harib24g/stars2.hrb ::stars2.hrb
  COMMAND mcopy -i ${27_day_harib24g_OS} ${root_BINARY_DIR}/projects/27_day/harib24g/lines.hrb ::lines.hrb
  COMMAND mcopy -i ${27_day_harib24g_OS} ${root_BINARY_DIR}/projects/27_day/harib24g/walk.hrb ::walk.hrb
  COMMAND mcopy -i ${27_day_harib24g_OS} ${root_BINARY_DIR}/projects/27_day/harib24g/color2.hrb ::color2.hrb
  COMMAND mcopy -i ${27_day_harib24g_OS} ${root_BINARY_DIR}/projects/27_day/harib24g/color.hrb ::color.hrb
  COMMAND mcopy -i ${27_day_harib24g_OS} ${root_BINARY_DIR}/projects/27_day/harib24g/beepdown.hrb ::beepdown.hrb
  COMMAND mcopy -i ${27_day_harib24g_OS} ${root_BINARY_DIR}/projects/27_day/harib24g/noodle.hrb ::noodle.hrb
  COMMAND mcopy -i ${27_day_harib24g_OS} ${root_BINARY_DIR}/projects/27_day/harib24g/hello5.hrb ::hello5.hrb
  DEPENDS 27_day_harib24g_sys ${root_BINARY_DIR}/projects/27_day/harib24g/a.hrb ${root_BINARY_DIR}/projects/27_day/harib24g/hello3.hrb ${root_BINARY_DIR}/projects/27_day/harib24g/hello4.hrb ${root_BINARY_DIR}/projects/27_day/harib24g/winhelo.hrb ${root_BINARY_DIR}/projects/27_day/harib24g/winhelo2.hrb ${root_BINARY_DIR}/projects/27_day/harib24g/winhelo3.hrb ${root_BINARY_DIR}/projects/27_day/harib24g/star1.hrb ${root_BINARY_DIR}/projects/27_day/harib24g/stars.hrb ${root_BINARY_DIR}/projects/27_day/harib24g/stars2.hrb ${root_BINARY_DIR}/projects/27_day/harib24g/lines.hrb ${root_BINARY_DIR}/projects/27_day/harib24g/walk.hrb ${root_BINARY_DIR}/projects/27_day/harib24g/noodle.hrb ${root_BINARY_DIR}/projects/27_day/harib24g/beepdown.hrb ${root_BINARY_DIR}/projects/27_day/harib24g/color.hrb ${root_BINARY_DIR}/projects/27_day/harib24g/color2.hrb ${root_BINARY_DIR}/projects/27_day/harib24g/hello5.hrb
)

#----------------------------------------------------------
########### application .hrb files (copied into os.img) ###############
set(27_day_harib24g_APIO
  ${root_BINARY_DIR}/projects/27_day/harib24g/api001.o
  ${root_BINARY_DIR}/projects/27_day/harib24g/api002.o
  ${root_BINARY_DIR}/projects/27_day/harib24g/api003.o
  ${root_BINARY_DIR}/projects/27_day/harib24g/api004.o
  ${root_BINARY_DIR}/projects/27_day/harib24g/api005.o
  ${root_BINARY_DIR}/projects/27_day/harib24g/api006.o
  ${root_BINARY_DIR}/projects/27_day/harib24g/api007.o
  ${root_BINARY_DIR}/projects/27_day/harib24g/api008.o
  ${root_BINARY_DIR}/projects/27_day/harib24g/api009.o
  ${root_BINARY_DIR}/projects/27_day/harib24g/api010.o
  ${root_BINARY_DIR}/projects/27_day/harib24g/api011.o
  ${root_BINARY_DIR}/projects/27_day/harib24g/api012.o
  ${root_BINARY_DIR}/projects/27_day/harib24g/api013.o
  ${root_BINARY_DIR}/projects/27_day/harib24g/api014.o
  ${root_BINARY_DIR}/projects/27_day/harib24g/api015.o
  ${root_BINARY_DIR}/projects/27_day/harib24g/api016.o
  ${root_BINARY_DIR}/projects/27_day/harib24g/api017.o
  ${root_BINARY_DIR}/projects/27_day/harib24g/api018.o
  ${root_BINARY_DIR}/projects/27_day/harib24g/api019.o
  ${root_BINARY_DIR}/projects/27_day/harib24g/api020.o)
set(27_day_harib24g_RANDO ${root_BINARY_DIR}/projects/27_day/harib24g/rand.o)
add_custom_command(
  OUTPUT ${27_day_harib24g_RANDO}
  COMMAND gcc -fleading-underscore -m32 -nostdlib -fno-pie -x c -c ${root_SOURCE_DIR}/golibc/rand.c_ -o ${27_day_harib24g_RANDO}
  DEPENDS ${root_SOURCE_DIR}/golibc/rand.c_
  COMMENT "Generating rand.o for 27_day_harib24g"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/27_day/harib24g/api001.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/27_day/harib24g/apilib/api001.nas ${root_BINARY_DIR}/projects/27_day/harib24g/api001.o
  DEPENDS ${root_SOURCE_DIR}/projects/27_day/harib24g/apilib/api001.nas
  COMMENT "Generating api001.o for 27_day_harib24g"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/27_day/harib24g/api002.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/27_day/harib24g/apilib/api002.nas ${root_BINARY_DIR}/projects/27_day/harib24g/api002.o
  DEPENDS ${root_SOURCE_DIR}/projects/27_day/harib24g/apilib/api002.nas
  COMMENT "Generating api002.o for 27_day_harib24g"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/27_day/harib24g/api003.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/27_day/harib24g/apilib/api003.nas ${root_BINARY_DIR}/projects/27_day/harib24g/api003.o
  DEPENDS ${root_SOURCE_DIR}/projects/27_day/harib24g/apilib/api003.nas
  COMMENT "Generating api003.o for 27_day_harib24g"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/27_day/harib24g/api004.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/27_day/harib24g/apilib/api004.nas ${root_BINARY_DIR}/projects/27_day/harib24g/api004.o
  DEPENDS ${root_SOURCE_DIR}/projects/27_day/harib24g/apilib/api004.nas
  COMMENT "Generating api004.o for 27_day_harib24g"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/27_day/harib24g/api005.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/27_day/harib24g/apilib/api005.nas ${root_BINARY_DIR}/projects/27_day/harib24g/api005.o
  DEPENDS ${root_SOURCE_DIR}/projects/27_day/harib24g/apilib/api005.nas
  COMMENT "Generating api005.o for 27_day_harib24g"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/27_day/harib24g/api006.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/27_day/harib24g/apilib/api006.nas ${root_BINARY_DIR}/projects/27_day/harib24g/api006.o
  DEPENDS ${root_SOURCE_DIR}/projects/27_day/harib24g/apilib/api006.nas
  COMMENT "Generating api006.o for 27_day_harib24g"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/27_day/harib24g/api007.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/27_day/harib24g/apilib/api007.nas ${root_BINARY_DIR}/projects/27_day/harib24g/api007.o
  DEPENDS ${root_SOURCE_DIR}/projects/27_day/harib24g/apilib/api007.nas
  COMMENT "Generating api007.o for 27_day_harib24g"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/27_day/harib24g/api008.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/27_day/harib24g/apilib/api008.nas ${root_BINARY_DIR}/projects/27_day/harib24g/api008.o
  DEPENDS ${root_SOURCE_DIR}/projects/27_day/harib24g/apilib/api008.nas
  COMMENT "Generating api008.o for 27_day_harib24g"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/27_day/harib24g/api009.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/27_day/harib24g/apilib/api009.nas ${root_BINARY_DIR}/projects/27_day/harib24g/api009.o
  DEPENDS ${root_SOURCE_DIR}/projects/27_day/harib24g/apilib/api009.nas
  COMMENT "Generating api009.o for 27_day_harib24g"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/27_day/harib24g/api010.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/27_day/harib24g/apilib/api010.nas ${root_BINARY_DIR}/projects/27_day/harib24g/api010.o
  DEPENDS ${root_SOURCE_DIR}/projects/27_day/harib24g/apilib/api010.nas
  COMMENT "Generating api010.o for 27_day_harib24g"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/27_day/harib24g/api011.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/27_day/harib24g/apilib/api011.nas ${root_BINARY_DIR}/projects/27_day/harib24g/api011.o
  DEPENDS ${root_SOURCE_DIR}/projects/27_day/harib24g/apilib/api011.nas
  COMMENT "Generating api011.o for 27_day_harib24g"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/27_day/harib24g/api012.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/27_day/harib24g/apilib/api012.nas ${root_BINARY_DIR}/projects/27_day/harib24g/api012.o
  DEPENDS ${root_SOURCE_DIR}/projects/27_day/harib24g/apilib/api012.nas
  COMMENT "Generating api012.o for 27_day_harib24g"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/27_day/harib24g/api013.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/27_day/harib24g/apilib/api013.nas ${root_BINARY_DIR}/projects/27_day/harib24g/api013.o
  DEPENDS ${root_SOURCE_DIR}/projects/27_day/harib24g/apilib/api013.nas
  COMMENT "Generating api013.o for 27_day_harib24g"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/27_day/harib24g/api014.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/27_day/harib24g/apilib/api014.nas ${root_BINARY_DIR}/projects/27_day/harib24g/api014.o
  DEPENDS ${root_SOURCE_DIR}/projects/27_day/harib24g/apilib/api014.nas
  COMMENT "Generating api014.o for 27_day_harib24g"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/27_day/harib24g/api015.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/27_day/harib24g/apilib/api015.nas ${root_BINARY_DIR}/projects/27_day/harib24g/api015.o
  DEPENDS ${root_SOURCE_DIR}/projects/27_day/harib24g/apilib/api015.nas
  COMMENT "Generating api015.o for 27_day_harib24g"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/27_day/harib24g/api016.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/27_day/harib24g/apilib/api016.nas ${root_BINARY_DIR}/projects/27_day/harib24g/api016.o
  DEPENDS ${root_SOURCE_DIR}/projects/27_day/harib24g/apilib/api016.nas
  COMMENT "Generating api016.o for 27_day_harib24g"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/27_day/harib24g/api017.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/27_day/harib24g/apilib/api017.nas ${root_BINARY_DIR}/projects/27_day/harib24g/api017.o
  DEPENDS ${root_SOURCE_DIR}/projects/27_day/harib24g/apilib/api017.nas
  COMMENT "Generating api017.o for 27_day_harib24g"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/27_day/harib24g/api018.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/27_day/harib24g/apilib/api018.nas ${root_BINARY_DIR}/projects/27_day/harib24g/api018.o
  DEPENDS ${root_SOURCE_DIR}/projects/27_day/harib24g/apilib/api018.nas
  COMMENT "Generating api018.o for 27_day_harib24g"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/27_day/harib24g/api019.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/27_day/harib24g/apilib/api019.nas ${root_BINARY_DIR}/projects/27_day/harib24g/api019.o
  DEPENDS ${root_SOURCE_DIR}/projects/27_day/harib24g/apilib/api019.nas
  COMMENT "Generating api019.o for 27_day_harib24g"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/27_day/harib24g/api020.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/27_day/harib24g/apilib/api020.nas ${root_BINARY_DIR}/projects/27_day/harib24g/api020.o
  DEPENDS ${root_SOURCE_DIR}/projects/27_day/harib24g/apilib/api020.nas
  COMMENT "Generating api020.o for 27_day_harib24g"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/27_day/harib24g/a.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/projects/27_day/harib24g -T ${27_day_harib24g_APPLDS} ${root_SOURCE_DIR}/projects/27_day/harib24g/a/a.c ${27_day_harib24g_APIO} ${27_day_harib24g_LIBGC} ${27_day_harib24g_RANDO} -o ${root_BINARY_DIR}/projects/27_day/harib24g/a.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/27_day/harib24g/a/a.c ${root_SOURCE_DIR}/projects/app.lds ${27_day_harib24g_APIO} ${27_day_harib24g_LIBGC} ${27_day_harib24g_RANDO}
  COMMENT "Generating a.hrb for 27_day_harib24g"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/27_day/harib24g/hello3.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/projects/27_day/harib24g -T ${27_day_harib24g_APPLDS} ${root_SOURCE_DIR}/projects/27_day/harib24g/hello3/hello3.c ${27_day_harib24g_APIO} ${27_day_harib24g_LIBGC} ${27_day_harib24g_RANDO} -o ${root_BINARY_DIR}/projects/27_day/harib24g/hello3.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/27_day/harib24g/hello3/hello3.c ${root_SOURCE_DIR}/projects/app.lds ${27_day_harib24g_APIO} ${27_day_harib24g_LIBGC} ${27_day_harib24g_RANDO}
  COMMENT "Generating hello3.hrb for 27_day_harib24g"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/27_day/harib24g/hello4.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/projects/27_day/harib24g -T ${27_day_harib24g_APPLDS} ${root_SOURCE_DIR}/projects/27_day/harib24g/hello4/hello4.c ${27_day_harib24g_APIO} ${27_day_harib24g_LIBGC} ${27_day_harib24g_RANDO} -o ${root_BINARY_DIR}/projects/27_day/harib24g/hello4.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/27_day/harib24g/hello4/hello4.c ${root_SOURCE_DIR}/projects/app.lds ${27_day_harib24g_APIO} ${27_day_harib24g_LIBGC} ${27_day_harib24g_RANDO}
  COMMENT "Generating hello4.hrb for 27_day_harib24g"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/27_day/harib24g/winhelo.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/projects/27_day/harib24g -T ${27_day_harib24g_APPLDS} ${root_SOURCE_DIR}/projects/27_day/harib24g/winhelo/winhelo.c ${27_day_harib24g_APIO} ${27_day_harib24g_LIBGC} ${27_day_harib24g_RANDO} -o ${root_BINARY_DIR}/projects/27_day/harib24g/winhelo.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/27_day/harib24g/winhelo/winhelo.c ${root_SOURCE_DIR}/projects/app.lds ${27_day_harib24g_APIO} ${27_day_harib24g_LIBGC} ${27_day_harib24g_RANDO}
  COMMENT "Generating winhelo.hrb for 27_day_harib24g"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/27_day/harib24g/winhelo2.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/projects/27_day/harib24g -T ${27_day_harib24g_APPLDS} ${root_SOURCE_DIR}/projects/27_day/harib24g/winhelo2/winhelo2.c ${27_day_harib24g_APIO} ${27_day_harib24g_LIBGC} ${27_day_harib24g_RANDO} -o ${root_BINARY_DIR}/projects/27_day/harib24g/winhelo2.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/27_day/harib24g/winhelo2/winhelo2.c ${root_SOURCE_DIR}/projects/app.lds ${27_day_harib24g_APIO} ${27_day_harib24g_LIBGC} ${27_day_harib24g_RANDO}
  COMMENT "Generating winhelo2.hrb for 27_day_harib24g"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/27_day/harib24g/winhelo3.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/projects/27_day/harib24g -T ${27_day_harib24g_APPLDS} ${root_SOURCE_DIR}/projects/27_day/harib24g/winhelo3/winhelo3.c ${27_day_harib24g_APIO} ${27_day_harib24g_LIBGC} ${27_day_harib24g_RANDO} -o ${root_BINARY_DIR}/projects/27_day/harib24g/winhelo3.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/27_day/harib24g/winhelo3/winhelo3.c ${root_SOURCE_DIR}/projects/app.lds ${27_day_harib24g_APIO} ${27_day_harib24g_LIBGC} ${27_day_harib24g_RANDO}
  COMMENT "Generating winhelo3.hrb for 27_day_harib24g"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/27_day/harib24g/star1.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/projects/27_day/harib24g -T ${27_day_harib24g_APPLDS} ${root_SOURCE_DIR}/projects/27_day/harib24g/star1/star1.c ${27_day_harib24g_APIO} ${27_day_harib24g_LIBGC} ${27_day_harib24g_RANDO} -o ${root_BINARY_DIR}/projects/27_day/harib24g/star1.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/27_day/harib24g/star1/star1.c ${root_SOURCE_DIR}/projects/app.lds ${27_day_harib24g_APIO} ${27_day_harib24g_LIBGC} ${27_day_harib24g_RANDO}
  COMMENT "Generating star1.hrb for 27_day_harib24g"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/27_day/harib24g/stars.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/projects/27_day/harib24g -T ${27_day_harib24g_APPLDS} ${root_SOURCE_DIR}/projects/27_day/harib24g/stars/stars.c ${27_day_harib24g_APIO} ${27_day_harib24g_LIBGC} ${27_day_harib24g_RANDO} -o ${root_BINARY_DIR}/projects/27_day/harib24g/stars.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/27_day/harib24g/stars/stars.c ${root_SOURCE_DIR}/projects/app.lds ${27_day_harib24g_APIO} ${27_day_harib24g_LIBGC} ${27_day_harib24g_RANDO}
  COMMENT "Generating stars.hrb for 27_day_harib24g"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/27_day/harib24g/stars2.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/projects/27_day/harib24g -T ${27_day_harib24g_APPLDS} ${root_SOURCE_DIR}/projects/27_day/harib24g/stars2/stars2.c ${27_day_harib24g_APIO} ${27_day_harib24g_LIBGC} ${27_day_harib24g_RANDO} -o ${root_BINARY_DIR}/projects/27_day/harib24g/stars2.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/27_day/harib24g/stars2/stars2.c ${root_SOURCE_DIR}/projects/app.lds ${27_day_harib24g_APIO} ${27_day_harib24g_LIBGC} ${27_day_harib24g_RANDO}
  COMMENT "Generating stars2.hrb for 27_day_harib24g"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/27_day/harib24g/lines.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/projects/27_day/harib24g -T ${27_day_harib24g_APPLDS} ${root_SOURCE_DIR}/projects/27_day/harib24g/lines/lines.c ${27_day_harib24g_APIO} ${27_day_harib24g_LIBGC} ${27_day_harib24g_RANDO} -o ${root_BINARY_DIR}/projects/27_day/harib24g/lines.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/27_day/harib24g/lines/lines.c ${root_SOURCE_DIR}/projects/app.lds ${27_day_harib24g_APIO} ${27_day_harib24g_LIBGC} ${27_day_harib24g_RANDO}
  COMMENT "Generating lines.hrb for 27_day_harib24g"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/27_day/harib24g/walk.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/projects/27_day/harib24g -T ${27_day_harib24g_APPLDS} ${root_SOURCE_DIR}/projects/27_day/harib24g/walk/walk.c ${27_day_harib24g_APIO} ${27_day_harib24g_LIBGC} ${27_day_harib24g_RANDO} -o ${root_BINARY_DIR}/projects/27_day/harib24g/walk.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/27_day/harib24g/walk/walk.c ${root_SOURCE_DIR}/projects/app.lds ${27_day_harib24g_APIO} ${27_day_harib24g_LIBGC} ${27_day_harib24g_RANDO}
  COMMENT "Generating walk.hrb for 27_day_harib24g"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/27_day/harib24g/color2.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/projects/27_day/harib24g -T ${27_day_harib24g_APPLDS} ${root_SOURCE_DIR}/projects/27_day/harib24g/color2/color2.c ${27_day_harib24g_APIO} ${27_day_harib24g_LIBGC} ${27_day_harib24g_RANDO} -o ${root_BINARY_DIR}/projects/27_day/harib24g/color2.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/27_day/harib24g/color2/color2.c ${root_SOURCE_DIR}/projects/app.lds ${27_day_harib24g_APIO} ${27_day_harib24g_LIBGC} ${27_day_harib24g_RANDO}
  COMMENT "Generating color2.hrb for 27_day_harib24g"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/27_day/harib24g/color.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/projects/27_day/harib24g -T ${27_day_harib24g_APPLDS} ${root_SOURCE_DIR}/projects/27_day/harib24g/color/color.c ${27_day_harib24g_APIO} ${27_day_harib24g_LIBGC} ${27_day_harib24g_RANDO} -o ${root_BINARY_DIR}/projects/27_day/harib24g/color.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/27_day/harib24g/color/color.c ${root_SOURCE_DIR}/projects/app.lds ${27_day_harib24g_APIO} ${27_day_harib24g_LIBGC} ${27_day_harib24g_RANDO}
  COMMENT "Generating color.hrb for 27_day_harib24g"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/27_day/harib24g/beepdown.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/projects/27_day/harib24g -T ${27_day_harib24g_APPLDS} ${root_SOURCE_DIR}/projects/27_day/harib24g/beepdown/beepdown.c ${27_day_harib24g_APIO} ${27_day_harib24g_LIBGC} ${27_day_harib24g_RANDO} -o ${root_BINARY_DIR}/projects/27_day/harib24g/beepdown.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/27_day/harib24g/beepdown/beepdown.c ${root_SOURCE_DIR}/projects/app.lds ${27_day_harib24g_APIO} ${27_day_harib24g_LIBGC} ${27_day_harib24g_RANDO}
  COMMENT "Generating beepdown.hrb for 27_day_harib24g"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/27_day/harib24g/noodle.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/projects/27_day/harib24g -T ${27_day_harib24g_APPLDS} ${root_SOURCE_DIR}/projects/27_day/harib24g/noodle/noodle.c ${27_day_harib24g_APIO} ${27_day_harib24g_LIBGC} ${27_day_harib24g_RANDO} -o ${root_BINARY_DIR}/projects/27_day/harib24g/noodle.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/27_day/harib24g/noodle/noodle.c ${root_SOURCE_DIR}/projects/app.lds ${27_day_harib24g_APIO} ${27_day_harib24g_LIBGC} ${27_day_harib24g_RANDO}
  COMMENT "Generating noodle.hrb for 27_day_harib24g"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/27_day/harib24g/hello5.hrb
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/27_day/harib24g/hello5/hello5.nas ${root_BINARY_DIR}/projects/27_day/harib24g/hello5.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/27_day/harib24g/hello5/hello5.nas
  COMMENT "Generating hello5.hrb for 27_day_harib24g"
)
