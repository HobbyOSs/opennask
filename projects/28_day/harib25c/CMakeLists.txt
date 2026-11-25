#----------------------------------------------------------
message(STATUS "Entering directory projects/28_day/harib25c/")

set(FONT ${root_BINARY_DIR}/src/makefont)
set(B2O  ${root_BINARY_DIR}/src/bin2obj)
set(CONV ${root_BINARY_DIR}/objconv/objconv)
set(28_day_harib25c_OS    ${root_BINARY_DIR}/projects/28_day/harib25c/os.img)
set(28_day_harib25c_SYS	  ${root_BINARY_DIR}/projects/28_day/harib25c/os.sys)
set(28_day_harib25c_IPLB  ${root_BINARY_DIR}/projects/28_day/harib25c/ipl.bin)
set(28_day_harib25c_IPLS  ${root_SOURCE_DIR}/projects/28_day/harib25c/haribote/ipl10.nas)
set(28_day_harib25c_HEADB ${root_BINARY_DIR}/projects/28_day/harib25c/asmhead.bin)
set(28_day_harib25c_HEADS ${root_SOURCE_DIR}/projects/28_day/harib25c/haribote/asmhead.nas)
set(28_day_harib25c_BOOTB ${root_BINARY_DIR}/projects/28_day/harib25c/boot.bin)
set(28_day_harib25c_CCO	  ${root_BINARY_DIR}/projects/28_day/harib25c/boot.o)
file(GLOB 28_day_harib25c_CCS ${root_SOURCE_DIR}/projects/28_day/harib25c/haribote/*.c)
set(28_day_harib25c_LDS	  ${root_SOURCE_DIR}/projects/os.lds)
set(28_day_harib25c_APPLDS  ${root_SOURCE_DIR}/projects/app.lds)
set(28_day_harib25c_FUNCS ${root_SOURCE_DIR}/projects/28_day/harib25c/haribote/naskfunc.nas)
set(28_day_harib25c_FUNCO ${root_BINARY_DIR}/projects/28_day/harib25c/naskfunc.o)
set(28_day_harib25c_FONTS ${root_SOURCE_DIR}/projects/28_day/harib25c/haribote/hankaku.txt)
set(28_day_harib25c_FONTB ${root_BINARY_DIR}/projects/28_day/harib25c/hankaku.bin)
set(28_day_harib25c_FONTO ${root_BINARY_DIR}/projects/28_day/harib25c/hankaku.o)
set(28_day_harib25c_LIBGE ${root_BINARY_DIR}/golibc/libgo.a)
set(28_day_harib25c_LIBGC ${root_BINARY_DIR}/projects/28_day/harib25c/libgo.o)
set(28_day_harib25c_WILDOBJ ${root_BINARY_DIR}/projects/28_day/harib25c/*.o)


add_custom_target(28_day_harib25c_run
  COMMAND ${QEMU} ${QEMUOPT} ${28_day_harib25c_OS}
  DEPENDS 28_day_harib25c_img
)
add_custom_target(28_day_harib25c_debug
  COMMAND ${QEMU} -s -S ${QEMUOPT} ${28_day_harib25c_OS} ${QEMU_DEBUG_OPT}
  DEPENDS 28_day_harib25c_img
)
add_custom_target(28_day_harib25c_clean
  COMMAND rm -f ${28_day_harib25c_OS}
  COMMAND rm -f ${28_day_harib25c_SYS}
  COMMAND rm -f ${28_day_harib25c_IPLB}
  COMMAND rm -f ${28_day_harib25c_HEADB}
  COMMAND rm -f ${28_day_harib25c_BOOTB}
  COMMAND rm -f ${28_day_harib25c_WILDOBJ}
)
add_custom_target(28_day_harib25c_ipl
  COMMAND ${RAKUSK_EXECUTABLE} ${28_day_harib25c_IPLS} ${28_day_harib25c_IPLB}
  DEPENDS ${28_day_harib25c_IPLS}
)
add_custom_command(
  OUTPUT ${28_day_harib25c_HEADB}
  COMMAND ${RAKUSK_EXECUTABLE} ${28_day_harib25c_HEADS} ${28_day_harib25c_HEADB}
  DEPENDS ${28_day_harib25c_HEADS}
  COMMENT "Generating asmhead.bin for 28_day_harib25c"
)
add_custom_command(
  OUTPUT ${28_day_harib25c_FUNCO}
  COMMAND ${RAKUSK_EXECUTABLE} ${28_day_harib25c_FUNCS} ${28_day_harib25c_FUNCO}
  DEPENDS ${28_day_harib25c_FUNCS}
  COMMENT "Generating naskfunc.o for 28_day_harib25c"
)
add_custom_command(
  OUTPUT ${28_day_harib25c_FONTO}
  COMMAND ${FONT} ${28_day_harib25c_FONTS} ${28_day_harib25c_FONTB}
  COMMAND ${B2O}  ${28_day_harib25c_FONTB} ${28_day_harib25c_FONTO} _hankaku
  DEPENDS ${28_day_harib25c_FONTS} makefont bin2obj
  COMMENT "Generating hankaku.o for 28_day_harib25c"
)
add_custom_command(
  OUTPUT ${28_day_harib25c_LIBGC}
  COMMAND ${CONV} -fcoff32 -nu ${28_day_harib25c_LIBGE} ${28_day_harib25c_LIBGC}
  DEPENDS ${28_day_harib25c_LIBGE}
  COMMENT "Converting golibc for 28_day_harib25c"
)

add_custom_target(28_day_harib25c_sys
  COMMAND gcc ${BINOPT} -T ${28_day_harib25c_LDS} ${28_day_harib25c_CCS} ${28_day_harib25c_WILDOBJ} -o ${28_day_harib25c_BOOTB}
  COMMAND cat ${28_day_harib25c_HEADB} ${28_day_harib25c_BOOTB} > ${28_day_harib25c_SYS}
  DEPENDS ${28_day_harib25c_HEADB} 28_day_harib25c_ipl
  DEPENDS ${28_day_harib25c_FUNCO}
  DEPENDS ${28_day_harib25c_FONTO} ${28_day_harib25c_LIBGC}
)
add_custom_target(28_day_harib25c_img
  COMMAND mformat -f 1440 -l HARIBOTEOS -N 0xffffffff -C -B ${28_day_harib25c_IPLB} -i ${28_day_harib25c_OS}
  COMMAND mcopy -i ${28_day_harib25c_OS} ${28_day_harib25c_SYS} ::
  COMMAND mcopy -i ${28_day_harib25c_OS} ${root_BINARY_DIR}/projects/28_day/harib25c/a.hrb ::a.hrb
  COMMAND mcopy -i ${28_day_harib25c_OS} ${root_BINARY_DIR}/projects/28_day/harib25c/hello3.hrb ::hello3.hrb
  COMMAND mcopy -i ${28_day_harib25c_OS} ${root_BINARY_DIR}/projects/28_day/harib25c/hello4.hrb ::hello4.hrb
  COMMAND mcopy -i ${28_day_harib25c_OS} ${root_BINARY_DIR}/projects/28_day/harib25c/winhelo.hrb ::winhelo.hrb
  COMMAND mcopy -i ${28_day_harib25c_OS} ${root_BINARY_DIR}/projects/28_day/harib25c/winhelo2.hrb ::winhelo2.hrb
  COMMAND mcopy -i ${28_day_harib25c_OS} ${root_BINARY_DIR}/projects/28_day/harib25c/winhelo3.hrb ::winhelo3.hrb
  COMMAND mcopy -i ${28_day_harib25c_OS} ${root_BINARY_DIR}/projects/28_day/harib25c/star1.hrb ::star1.hrb
  COMMAND mcopy -i ${28_day_harib25c_OS} ${root_BINARY_DIR}/projects/28_day/harib25c/stars.hrb ::stars.hrb
  COMMAND mcopy -i ${28_day_harib25c_OS} ${root_BINARY_DIR}/projects/28_day/harib25c/stars2.hrb ::stars2.hrb
  COMMAND mcopy -i ${28_day_harib25c_OS} ${root_BINARY_DIR}/projects/28_day/harib25c/lines.hrb ::lines.hrb
  COMMAND mcopy -i ${28_day_harib25c_OS} ${root_BINARY_DIR}/projects/28_day/harib25c/walk.hrb ::walk.hrb
  COMMAND mcopy -i ${28_day_harib25c_OS} ${root_BINARY_DIR}/projects/28_day/harib25c/typeipl.hrb ::typeipl.hrb
  COMMAND mcopy -i ${28_day_harib25c_OS} ${root_BINARY_DIR}/projects/28_day/harib25c/sosu3.hrb ::sosu3.hrb
  COMMAND mcopy -i ${28_day_harib25c_OS} ${root_BINARY_DIR}/projects/28_day/harib25c/sosu2.hrb ::sosu2.hrb
  COMMAND mcopy -i ${28_day_harib25c_OS} ${root_BINARY_DIR}/projects/28_day/harib25c/sosu.hrb ::sosu.hrb
  COMMAND mcopy -i ${28_day_harib25c_OS} ${root_BINARY_DIR}/projects/28_day/harib25c/color2.hrb ::color2.hrb
  COMMAND mcopy -i ${28_day_harib25c_OS} ${root_BINARY_DIR}/projects/28_day/harib25c/color.hrb ::color.hrb
  COMMAND mcopy -i ${28_day_harib25c_OS} ${root_BINARY_DIR}/projects/28_day/harib25c/beepdown.hrb ::beepdown.hrb
  COMMAND mcopy -i ${28_day_harib25c_OS} ${root_BINARY_DIR}/projects/28_day/harib25c/noodle.hrb ::noodle.hrb
  COMMAND mcopy -i ${28_day_harib25c_OS} ${root_BINARY_DIR}/projects/28_day/harib25c/hello5.hrb ::hello5.hrb
  DEPENDS 28_day_harib25c_sys ${root_BINARY_DIR}/projects/28_day/harib25c/a.hrb ${root_BINARY_DIR}/projects/28_day/harib25c/hello3.hrb ${root_BINARY_DIR}/projects/28_day/harib25c/hello4.hrb ${root_BINARY_DIR}/projects/28_day/harib25c/winhelo.hrb ${root_BINARY_DIR}/projects/28_day/harib25c/winhelo2.hrb ${root_BINARY_DIR}/projects/28_day/harib25c/winhelo3.hrb ${root_BINARY_DIR}/projects/28_day/harib25c/star1.hrb ${root_BINARY_DIR}/projects/28_day/harib25c/stars.hrb ${root_BINARY_DIR}/projects/28_day/harib25c/stars2.hrb ${root_BINARY_DIR}/projects/28_day/harib25c/lines.hrb ${root_BINARY_DIR}/projects/28_day/harib25c/walk.hrb ${root_BINARY_DIR}/projects/28_day/harib25c/typeipl.hrb ${root_BINARY_DIR}/projects/28_day/harib25c/sosu3.hrb ${root_BINARY_DIR}/projects/28_day/harib25c/sosu2.hrb ${root_BINARY_DIR}/projects/28_day/harib25c/sosu.hrb ${root_BINARY_DIR}/projects/28_day/harib25c/noodle.hrb ${root_BINARY_DIR}/projects/28_day/harib25c/beepdown.hrb ${root_BINARY_DIR}/projects/28_day/harib25c/color.hrb ${root_BINARY_DIR}/projects/28_day/harib25c/color2.hrb ${root_BINARY_DIR}/projects/28_day/harib25c/hello5.hrb
)

#----------------------------------------------------------
########### application .hrb files (copied into os.img) ###############
set(28_day_harib25c_APIO
  ${root_BINARY_DIR}/projects/28_day/harib25c/api001.o
  ${root_BINARY_DIR}/projects/28_day/harib25c/api002.o
  ${root_BINARY_DIR}/projects/28_day/harib25c/api003.o
  ${root_BINARY_DIR}/projects/28_day/harib25c/api004.o
  ${root_BINARY_DIR}/projects/28_day/harib25c/api005.o
  ${root_BINARY_DIR}/projects/28_day/harib25c/api006.o
  ${root_BINARY_DIR}/projects/28_day/harib25c/api007.o
  ${root_BINARY_DIR}/projects/28_day/harib25c/api008.o
  ${root_BINARY_DIR}/projects/28_day/harib25c/api009.o
  ${root_BINARY_DIR}/projects/28_day/harib25c/api010.o
  ${root_BINARY_DIR}/projects/28_day/harib25c/api011.o
  ${root_BINARY_DIR}/projects/28_day/harib25c/api012.o
  ${root_BINARY_DIR}/projects/28_day/harib25c/api013.o
  ${root_BINARY_DIR}/projects/28_day/harib25c/api014.o
  ${root_BINARY_DIR}/projects/28_day/harib25c/api015.o
  ${root_BINARY_DIR}/projects/28_day/harib25c/api016.o
  ${root_BINARY_DIR}/projects/28_day/harib25c/api017.o
  ${root_BINARY_DIR}/projects/28_day/harib25c/api018.o
  ${root_BINARY_DIR}/projects/28_day/harib25c/api019.o
  ${root_BINARY_DIR}/projects/28_day/harib25c/api020.o
  ${root_BINARY_DIR}/projects/28_day/harib25c/api021.o
  ${root_BINARY_DIR}/projects/28_day/harib25c/api022.o
  ${root_BINARY_DIR}/projects/28_day/harib25c/api023.o
  ${root_BINARY_DIR}/projects/28_day/harib25c/api024.o
  ${root_BINARY_DIR}/projects/28_day/harib25c/api025.o)
set(28_day_harib25c_RANDO ${root_BINARY_DIR}/projects/28_day/harib25c/rand.o)
add_custom_command(
  OUTPUT ${28_day_harib25c_RANDO}
  COMMAND gcc -fleading-underscore -m32 -nostdlib -fno-pie -x c -c ${root_SOURCE_DIR}/golibc/rand.c_ -o ${28_day_harib25c_RANDO}
  DEPENDS ${root_SOURCE_DIR}/golibc/rand.c_
  COMMENT "Generating rand.o for 28_day_harib25c"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/28_day/harib25c/api001.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/28_day/harib25c/apilib/api001.nas ${root_BINARY_DIR}/projects/28_day/harib25c/api001.o
  DEPENDS ${root_SOURCE_DIR}/projects/28_day/harib25c/apilib/api001.nas
  COMMENT "Generating api001.o for 28_day_harib25c"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/28_day/harib25c/api002.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/28_day/harib25c/apilib/api002.nas ${root_BINARY_DIR}/projects/28_day/harib25c/api002.o
  DEPENDS ${root_SOURCE_DIR}/projects/28_day/harib25c/apilib/api002.nas
  COMMENT "Generating api002.o for 28_day_harib25c"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/28_day/harib25c/api003.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/28_day/harib25c/apilib/api003.nas ${root_BINARY_DIR}/projects/28_day/harib25c/api003.o
  DEPENDS ${root_SOURCE_DIR}/projects/28_day/harib25c/apilib/api003.nas
  COMMENT "Generating api003.o for 28_day_harib25c"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/28_day/harib25c/api004.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/28_day/harib25c/apilib/api004.nas ${root_BINARY_DIR}/projects/28_day/harib25c/api004.o
  DEPENDS ${root_SOURCE_DIR}/projects/28_day/harib25c/apilib/api004.nas
  COMMENT "Generating api004.o for 28_day_harib25c"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/28_day/harib25c/api005.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/28_day/harib25c/apilib/api005.nas ${root_BINARY_DIR}/projects/28_day/harib25c/api005.o
  DEPENDS ${root_SOURCE_DIR}/projects/28_day/harib25c/apilib/api005.nas
  COMMENT "Generating api005.o for 28_day_harib25c"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/28_day/harib25c/api006.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/28_day/harib25c/apilib/api006.nas ${root_BINARY_DIR}/projects/28_day/harib25c/api006.o
  DEPENDS ${root_SOURCE_DIR}/projects/28_day/harib25c/apilib/api006.nas
  COMMENT "Generating api006.o for 28_day_harib25c"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/28_day/harib25c/api007.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/28_day/harib25c/apilib/api007.nas ${root_BINARY_DIR}/projects/28_day/harib25c/api007.o
  DEPENDS ${root_SOURCE_DIR}/projects/28_day/harib25c/apilib/api007.nas
  COMMENT "Generating api007.o for 28_day_harib25c"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/28_day/harib25c/api008.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/28_day/harib25c/apilib/api008.nas ${root_BINARY_DIR}/projects/28_day/harib25c/api008.o
  DEPENDS ${root_SOURCE_DIR}/projects/28_day/harib25c/apilib/api008.nas
  COMMENT "Generating api008.o for 28_day_harib25c"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/28_day/harib25c/api009.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/28_day/harib25c/apilib/api009.nas ${root_BINARY_DIR}/projects/28_day/harib25c/api009.o
  DEPENDS ${root_SOURCE_DIR}/projects/28_day/harib25c/apilib/api009.nas
  COMMENT "Generating api009.o for 28_day_harib25c"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/28_day/harib25c/api010.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/28_day/harib25c/apilib/api010.nas ${root_BINARY_DIR}/projects/28_day/harib25c/api010.o
  DEPENDS ${root_SOURCE_DIR}/projects/28_day/harib25c/apilib/api010.nas
  COMMENT "Generating api010.o for 28_day_harib25c"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/28_day/harib25c/api011.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/28_day/harib25c/apilib/api011.nas ${root_BINARY_DIR}/projects/28_day/harib25c/api011.o
  DEPENDS ${root_SOURCE_DIR}/projects/28_day/harib25c/apilib/api011.nas
  COMMENT "Generating api011.o for 28_day_harib25c"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/28_day/harib25c/api012.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/28_day/harib25c/apilib/api012.nas ${root_BINARY_DIR}/projects/28_day/harib25c/api012.o
  DEPENDS ${root_SOURCE_DIR}/projects/28_day/harib25c/apilib/api012.nas
  COMMENT "Generating api012.o for 28_day_harib25c"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/28_day/harib25c/api013.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/28_day/harib25c/apilib/api013.nas ${root_BINARY_DIR}/projects/28_day/harib25c/api013.o
  DEPENDS ${root_SOURCE_DIR}/projects/28_day/harib25c/apilib/api013.nas
  COMMENT "Generating api013.o for 28_day_harib25c"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/28_day/harib25c/api014.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/28_day/harib25c/apilib/api014.nas ${root_BINARY_DIR}/projects/28_day/harib25c/api014.o
  DEPENDS ${root_SOURCE_DIR}/projects/28_day/harib25c/apilib/api014.nas
  COMMENT "Generating api014.o for 28_day_harib25c"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/28_day/harib25c/api015.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/28_day/harib25c/apilib/api015.nas ${root_BINARY_DIR}/projects/28_day/harib25c/api015.o
  DEPENDS ${root_SOURCE_DIR}/projects/28_day/harib25c/apilib/api015.nas
  COMMENT "Generating api015.o for 28_day_harib25c"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/28_day/harib25c/api016.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/28_day/harib25c/apilib/api016.nas ${root_BINARY_DIR}/projects/28_day/harib25c/api016.o
  DEPENDS ${root_SOURCE_DIR}/projects/28_day/harib25c/apilib/api016.nas
  COMMENT "Generating api016.o for 28_day_harib25c"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/28_day/harib25c/api017.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/28_day/harib25c/apilib/api017.nas ${root_BINARY_DIR}/projects/28_day/harib25c/api017.o
  DEPENDS ${root_SOURCE_DIR}/projects/28_day/harib25c/apilib/api017.nas
  COMMENT "Generating api017.o for 28_day_harib25c"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/28_day/harib25c/api018.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/28_day/harib25c/apilib/api018.nas ${root_BINARY_DIR}/projects/28_day/harib25c/api018.o
  DEPENDS ${root_SOURCE_DIR}/projects/28_day/harib25c/apilib/api018.nas
  COMMENT "Generating api018.o for 28_day_harib25c"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/28_day/harib25c/api019.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/28_day/harib25c/apilib/api019.nas ${root_BINARY_DIR}/projects/28_day/harib25c/api019.o
  DEPENDS ${root_SOURCE_DIR}/projects/28_day/harib25c/apilib/api019.nas
  COMMENT "Generating api019.o for 28_day_harib25c"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/28_day/harib25c/api020.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/28_day/harib25c/apilib/api020.nas ${root_BINARY_DIR}/projects/28_day/harib25c/api020.o
  DEPENDS ${root_SOURCE_DIR}/projects/28_day/harib25c/apilib/api020.nas
  COMMENT "Generating api020.o for 28_day_harib25c"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/28_day/harib25c/api021.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/28_day/harib25c/apilib/api021.nas ${root_BINARY_DIR}/projects/28_day/harib25c/api021.o
  DEPENDS ${root_SOURCE_DIR}/projects/28_day/harib25c/apilib/api021.nas
  COMMENT "Generating api021.o for 28_day_harib25c"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/28_day/harib25c/api022.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/28_day/harib25c/apilib/api022.nas ${root_BINARY_DIR}/projects/28_day/harib25c/api022.o
  DEPENDS ${root_SOURCE_DIR}/projects/28_day/harib25c/apilib/api022.nas
  COMMENT "Generating api022.o for 28_day_harib25c"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/28_day/harib25c/api023.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/28_day/harib25c/apilib/api023.nas ${root_BINARY_DIR}/projects/28_day/harib25c/api023.o
  DEPENDS ${root_SOURCE_DIR}/projects/28_day/harib25c/apilib/api023.nas
  COMMENT "Generating api023.o for 28_day_harib25c"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/28_day/harib25c/api024.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/28_day/harib25c/apilib/api024.nas ${root_BINARY_DIR}/projects/28_day/harib25c/api024.o
  DEPENDS ${root_SOURCE_DIR}/projects/28_day/harib25c/apilib/api024.nas
  COMMENT "Generating api024.o for 28_day_harib25c"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/28_day/harib25c/api025.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/28_day/harib25c/apilib/api025.nas ${root_BINARY_DIR}/projects/28_day/harib25c/api025.o
  DEPENDS ${root_SOURCE_DIR}/projects/28_day/harib25c/apilib/api025.nas
  COMMENT "Generating api025.o for 28_day_harib25c"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/28_day/harib25c/a.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/projects/28_day/harib25c -T ${28_day_harib25c_APPLDS} ${root_SOURCE_DIR}/projects/28_day/harib25c/a/a.c ${28_day_harib25c_APIO} ${28_day_harib25c_LIBGC} ${28_day_harib25c_RANDO} -o ${root_BINARY_DIR}/projects/28_day/harib25c/a.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/28_day/harib25c/a/a.c ${root_SOURCE_DIR}/projects/app.lds ${28_day_harib25c_APIO} ${28_day_harib25c_LIBGC} ${28_day_harib25c_RANDO}
  COMMENT "Generating a.hrb for 28_day_harib25c"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/28_day/harib25c/hello3.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/projects/28_day/harib25c -T ${28_day_harib25c_APPLDS} ${root_SOURCE_DIR}/projects/28_day/harib25c/hello3/hello3.c ${28_day_harib25c_APIO} ${28_day_harib25c_LIBGC} ${28_day_harib25c_RANDO} -o ${root_BINARY_DIR}/projects/28_day/harib25c/hello3.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/28_day/harib25c/hello3/hello3.c ${root_SOURCE_DIR}/projects/app.lds ${28_day_harib25c_APIO} ${28_day_harib25c_LIBGC} ${28_day_harib25c_RANDO}
  COMMENT "Generating hello3.hrb for 28_day_harib25c"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/28_day/harib25c/hello4.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/projects/28_day/harib25c -T ${28_day_harib25c_APPLDS} ${root_SOURCE_DIR}/projects/28_day/harib25c/hello4/hello4.c ${28_day_harib25c_APIO} ${28_day_harib25c_LIBGC} ${28_day_harib25c_RANDO} -o ${root_BINARY_DIR}/projects/28_day/harib25c/hello4.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/28_day/harib25c/hello4/hello4.c ${root_SOURCE_DIR}/projects/app.lds ${28_day_harib25c_APIO} ${28_day_harib25c_LIBGC} ${28_day_harib25c_RANDO}
  COMMENT "Generating hello4.hrb for 28_day_harib25c"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/28_day/harib25c/winhelo.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/projects/28_day/harib25c -T ${28_day_harib25c_APPLDS} ${root_SOURCE_DIR}/projects/28_day/harib25c/winhelo/winhelo.c ${28_day_harib25c_APIO} ${28_day_harib25c_LIBGC} ${28_day_harib25c_RANDO} -o ${root_BINARY_DIR}/projects/28_day/harib25c/winhelo.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/28_day/harib25c/winhelo/winhelo.c ${root_SOURCE_DIR}/projects/app.lds ${28_day_harib25c_APIO} ${28_day_harib25c_LIBGC} ${28_day_harib25c_RANDO}
  COMMENT "Generating winhelo.hrb for 28_day_harib25c"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/28_day/harib25c/winhelo2.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/projects/28_day/harib25c -T ${28_day_harib25c_APPLDS} ${root_SOURCE_DIR}/projects/28_day/harib25c/winhelo2/winhelo2.c ${28_day_harib25c_APIO} ${28_day_harib25c_LIBGC} ${28_day_harib25c_RANDO} -o ${root_BINARY_DIR}/projects/28_day/harib25c/winhelo2.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/28_day/harib25c/winhelo2/winhelo2.c ${root_SOURCE_DIR}/projects/app.lds ${28_day_harib25c_APIO} ${28_day_harib25c_LIBGC} ${28_day_harib25c_RANDO}
  COMMENT "Generating winhelo2.hrb for 28_day_harib25c"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/28_day/harib25c/winhelo3.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/projects/28_day/harib25c -T ${28_day_harib25c_APPLDS} ${root_SOURCE_DIR}/projects/28_day/harib25c/winhelo3/winhelo3.c ${28_day_harib25c_APIO} ${28_day_harib25c_LIBGC} ${28_day_harib25c_RANDO} -o ${root_BINARY_DIR}/projects/28_day/harib25c/winhelo3.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/28_day/harib25c/winhelo3/winhelo3.c ${root_SOURCE_DIR}/projects/app.lds ${28_day_harib25c_APIO} ${28_day_harib25c_LIBGC} ${28_day_harib25c_RANDO}
  COMMENT "Generating winhelo3.hrb for 28_day_harib25c"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/28_day/harib25c/star1.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/projects/28_day/harib25c -T ${28_day_harib25c_APPLDS} ${root_SOURCE_DIR}/projects/28_day/harib25c/star1/star1.c ${28_day_harib25c_APIO} ${28_day_harib25c_LIBGC} ${28_day_harib25c_RANDO} -o ${root_BINARY_DIR}/projects/28_day/harib25c/star1.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/28_day/harib25c/star1/star1.c ${root_SOURCE_DIR}/projects/app.lds ${28_day_harib25c_APIO} ${28_day_harib25c_LIBGC} ${28_day_harib25c_RANDO}
  COMMENT "Generating star1.hrb for 28_day_harib25c"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/28_day/harib25c/stars.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/projects/28_day/harib25c -T ${28_day_harib25c_APPLDS} ${root_SOURCE_DIR}/projects/28_day/harib25c/stars/stars.c ${28_day_harib25c_APIO} ${28_day_harib25c_LIBGC} ${28_day_harib25c_RANDO} -o ${root_BINARY_DIR}/projects/28_day/harib25c/stars.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/28_day/harib25c/stars/stars.c ${root_SOURCE_DIR}/projects/app.lds ${28_day_harib25c_APIO} ${28_day_harib25c_LIBGC} ${28_day_harib25c_RANDO}
  COMMENT "Generating stars.hrb for 28_day_harib25c"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/28_day/harib25c/stars2.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/projects/28_day/harib25c -T ${28_day_harib25c_APPLDS} ${root_SOURCE_DIR}/projects/28_day/harib25c/stars2/stars2.c ${28_day_harib25c_APIO} ${28_day_harib25c_LIBGC} ${28_day_harib25c_RANDO} -o ${root_BINARY_DIR}/projects/28_day/harib25c/stars2.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/28_day/harib25c/stars2/stars2.c ${root_SOURCE_DIR}/projects/app.lds ${28_day_harib25c_APIO} ${28_day_harib25c_LIBGC} ${28_day_harib25c_RANDO}
  COMMENT "Generating stars2.hrb for 28_day_harib25c"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/28_day/harib25c/lines.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/projects/28_day/harib25c -T ${28_day_harib25c_APPLDS} ${root_SOURCE_DIR}/projects/28_day/harib25c/lines/lines.c ${28_day_harib25c_APIO} ${28_day_harib25c_LIBGC} ${28_day_harib25c_RANDO} -o ${root_BINARY_DIR}/projects/28_day/harib25c/lines.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/28_day/harib25c/lines/lines.c ${root_SOURCE_DIR}/projects/app.lds ${28_day_harib25c_APIO} ${28_day_harib25c_LIBGC} ${28_day_harib25c_RANDO}
  COMMENT "Generating lines.hrb for 28_day_harib25c"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/28_day/harib25c/walk.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/projects/28_day/harib25c -T ${28_day_harib25c_APPLDS} ${root_SOURCE_DIR}/projects/28_day/harib25c/walk/walk.c ${28_day_harib25c_APIO} ${28_day_harib25c_LIBGC} ${28_day_harib25c_RANDO} -o ${root_BINARY_DIR}/projects/28_day/harib25c/walk.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/28_day/harib25c/walk/walk.c ${root_SOURCE_DIR}/projects/app.lds ${28_day_harib25c_APIO} ${28_day_harib25c_LIBGC} ${28_day_harib25c_RANDO}
  COMMENT "Generating walk.hrb for 28_day_harib25c"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/28_day/harib25c/typeipl.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/projects/28_day/harib25c -T ${28_day_harib25c_APPLDS} ${root_SOURCE_DIR}/projects/28_day/harib25c/typeipl/typeipl.c ${28_day_harib25c_APIO} ${28_day_harib25c_LIBGC} ${28_day_harib25c_RANDO} -o ${root_BINARY_DIR}/projects/28_day/harib25c/typeipl.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/28_day/harib25c/typeipl/typeipl.c ${root_SOURCE_DIR}/projects/app.lds ${28_day_harib25c_APIO} ${28_day_harib25c_LIBGC} ${28_day_harib25c_RANDO}
  COMMENT "Generating typeipl.hrb for 28_day_harib25c"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/28_day/harib25c/sosu3.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/projects/28_day/harib25c -T ${28_day_harib25c_APPLDS} ${root_SOURCE_DIR}/projects/28_day/harib25c/sosu3/sosu3.c ${28_day_harib25c_APIO} ${28_day_harib25c_LIBGC} ${28_day_harib25c_RANDO} -o ${root_BINARY_DIR}/projects/28_day/harib25c/sosu3.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/28_day/harib25c/sosu3/sosu3.c ${root_SOURCE_DIR}/projects/app.lds ${28_day_harib25c_APIO} ${28_day_harib25c_LIBGC} ${28_day_harib25c_RANDO}
  COMMENT "Generating sosu3.hrb for 28_day_harib25c"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/28_day/harib25c/sosu2.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/projects/28_day/harib25c -T ${28_day_harib25c_APPLDS} ${root_SOURCE_DIR}/projects/28_day/harib25c/sosu2/sosu2.c ${28_day_harib25c_APIO} ${28_day_harib25c_LIBGC} ${28_day_harib25c_RANDO} -o ${root_BINARY_DIR}/projects/28_day/harib25c/sosu2.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/28_day/harib25c/sosu2/sosu2.c ${root_SOURCE_DIR}/projects/app.lds ${28_day_harib25c_APIO} ${28_day_harib25c_LIBGC} ${28_day_harib25c_RANDO}
  COMMENT "Generating sosu2.hrb for 28_day_harib25c"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/28_day/harib25c/sosu.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/projects/28_day/harib25c -T ${28_day_harib25c_APPLDS} ${root_SOURCE_DIR}/projects/28_day/harib25c/sosu/sosu.c ${28_day_harib25c_APIO} ${28_day_harib25c_LIBGC} ${28_day_harib25c_RANDO} -o ${root_BINARY_DIR}/projects/28_day/harib25c/sosu.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/28_day/harib25c/sosu/sosu.c ${root_SOURCE_DIR}/projects/app.lds ${28_day_harib25c_APIO} ${28_day_harib25c_LIBGC} ${28_day_harib25c_RANDO}
  COMMENT "Generating sosu.hrb for 28_day_harib25c"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/28_day/harib25c/color2.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/projects/28_day/harib25c -T ${28_day_harib25c_APPLDS} ${root_SOURCE_DIR}/projects/28_day/harib25c/color2/color2.c ${28_day_harib25c_APIO} ${28_day_harib25c_LIBGC} ${28_day_harib25c_RANDO} -o ${root_BINARY_DIR}/projects/28_day/harib25c/color2.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/28_day/harib25c/color2/color2.c ${root_SOURCE_DIR}/projects/app.lds ${28_day_harib25c_APIO} ${28_day_harib25c_LIBGC} ${28_day_harib25c_RANDO}
  COMMENT "Generating color2.hrb for 28_day_harib25c"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/28_day/harib25c/color.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/projects/28_day/harib25c -T ${28_day_harib25c_APPLDS} ${root_SOURCE_DIR}/projects/28_day/harib25c/color/color.c ${28_day_harib25c_APIO} ${28_day_harib25c_LIBGC} ${28_day_harib25c_RANDO} -o ${root_BINARY_DIR}/projects/28_day/harib25c/color.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/28_day/harib25c/color/color.c ${root_SOURCE_DIR}/projects/app.lds ${28_day_harib25c_APIO} ${28_day_harib25c_LIBGC} ${28_day_harib25c_RANDO}
  COMMENT "Generating color.hrb for 28_day_harib25c"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/28_day/harib25c/beepdown.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/projects/28_day/harib25c -T ${28_day_harib25c_APPLDS} ${root_SOURCE_DIR}/projects/28_day/harib25c/beepdown/beepdown.c ${28_day_harib25c_APIO} ${28_day_harib25c_LIBGC} ${28_day_harib25c_RANDO} -o ${root_BINARY_DIR}/projects/28_day/harib25c/beepdown.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/28_day/harib25c/beepdown/beepdown.c ${root_SOURCE_DIR}/projects/app.lds ${28_day_harib25c_APIO} ${28_day_harib25c_LIBGC} ${28_day_harib25c_RANDO}
  COMMENT "Generating beepdown.hrb for 28_day_harib25c"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/28_day/harib25c/noodle.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/projects/28_day/harib25c -T ${28_day_harib25c_APPLDS} ${root_SOURCE_DIR}/projects/28_day/harib25c/noodle/noodle.c ${28_day_harib25c_APIO} ${28_day_harib25c_LIBGC} ${28_day_harib25c_RANDO} -o ${root_BINARY_DIR}/projects/28_day/harib25c/noodle.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/28_day/harib25c/noodle/noodle.c ${root_SOURCE_DIR}/projects/app.lds ${28_day_harib25c_APIO} ${28_day_harib25c_LIBGC} ${28_day_harib25c_RANDO}
  COMMENT "Generating noodle.hrb for 28_day_harib25c"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/28_day/harib25c/hello5.hrb
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/28_day/harib25c/hello5/hello5.nas ${root_BINARY_DIR}/projects/28_day/harib25c/hello5.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/28_day/harib25c/hello5/hello5.nas
  COMMENT "Generating hello5.hrb for 28_day_harib25c"
)
