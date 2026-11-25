#----------------------------------------------------------
message(STATUS "Entering directory projects/29_day/harib26a/")

set(FONT ${root_BINARY_DIR}/src/makefont)
set(B2O  ${root_BINARY_DIR}/src/bin2obj)
set(CONV ${root_BINARY_DIR}/objconv/objconv)
set(29_day_harib26a_OS    ${root_BINARY_DIR}/projects/29_day/harib26a/os.img)
set(29_day_harib26a_SYS	  ${root_BINARY_DIR}/projects/29_day/harib26a/os.sys)
set(29_day_harib26a_IPLB  ${root_BINARY_DIR}/projects/29_day/harib26a/ipl.bin)
set(29_day_harib26a_IPLS  ${root_SOURCE_DIR}/projects/29_day/harib26a/haribote/ipl20.nas)
set(29_day_harib26a_HEADB ${root_BINARY_DIR}/projects/29_day/harib26a/asmhead.bin)
set(29_day_harib26a_HEADS ${root_SOURCE_DIR}/projects/29_day/harib26a/haribote/asmhead.nas)
set(29_day_harib26a_BOOTB ${root_BINARY_DIR}/projects/29_day/harib26a/boot.bin)
set(29_day_harib26a_CCO	  ${root_BINARY_DIR}/projects/29_day/harib26a/boot.o)
file(GLOB 29_day_harib26a_CCS ${root_SOURCE_DIR}/projects/29_day/harib26a/haribote/*.c)
set(29_day_harib26a_LDS	  ${root_SOURCE_DIR}/projects/os.lds)
set(29_day_harib26a_APPLDS  ${root_SOURCE_DIR}/projects/app.lds)
set(29_day_harib26a_FUNCS ${root_SOURCE_DIR}/projects/29_day/harib26a/haribote/naskfunc.nas)
set(29_day_harib26a_FUNCO ${root_BINARY_DIR}/projects/29_day/harib26a/naskfunc.o)
set(29_day_harib26a_FONTS ${root_SOURCE_DIR}/projects/29_day/harib26a/haribote/hankaku.txt)
set(29_day_harib26a_FONTB ${root_BINARY_DIR}/projects/29_day/harib26a/hankaku.bin)
set(29_day_harib26a_FONTO ${root_BINARY_DIR}/projects/29_day/harib26a/hankaku.o)
set(29_day_harib26a_LIBGE ${root_BINARY_DIR}/golibc/libgo.a)
set(29_day_harib26a_LIBGC ${root_BINARY_DIR}/projects/29_day/harib26a/libgo.o)
set(29_day_harib26a_WILDOBJ ${root_BINARY_DIR}/projects/29_day/harib26a/*.o)


add_custom_target(29_day_harib26a_run
  COMMAND ${QEMU} ${QEMUOPT} ${29_day_harib26a_OS}
  DEPENDS 29_day_harib26a_img
)
add_custom_target(29_day_harib26a_debug
  COMMAND ${QEMU} -s -S ${QEMUOPT} ${29_day_harib26a_OS} ${QEMU_DEBUG_OPT}
  DEPENDS 29_day_harib26a_img
)
add_custom_target(29_day_harib26a_clean
  COMMAND rm -f ${29_day_harib26a_OS}
  COMMAND rm -f ${29_day_harib26a_SYS}
  COMMAND rm -f ${29_day_harib26a_IPLB}
  COMMAND rm -f ${29_day_harib26a_HEADB}
  COMMAND rm -f ${29_day_harib26a_BOOTB}
  COMMAND rm -f ${29_day_harib26a_WILDOBJ}
)
add_custom_target(29_day_harib26a_ipl
  COMMAND ${RAKUSK_EXECUTABLE} ${29_day_harib26a_IPLS} ${29_day_harib26a_IPLB}
  DEPENDS ${29_day_harib26a_IPLS}
)
add_custom_command(
  OUTPUT ${29_day_harib26a_HEADB}
  COMMAND ${RAKUSK_EXECUTABLE} ${29_day_harib26a_HEADS} ${29_day_harib26a_HEADB}
  DEPENDS ${29_day_harib26a_HEADS}
  COMMENT "Generating asmhead.bin for 29_day_harib26a"
)
add_custom_command(
  OUTPUT ${29_day_harib26a_FUNCO}
  COMMAND ${RAKUSK_EXECUTABLE} ${29_day_harib26a_FUNCS} ${29_day_harib26a_FUNCO}
  DEPENDS ${29_day_harib26a_FUNCS}
  COMMENT "Generating naskfunc.o for 29_day_harib26a"
)
add_custom_command(
  OUTPUT ${29_day_harib26a_FONTO}
  COMMAND ${FONT} ${29_day_harib26a_FONTS} ${29_day_harib26a_FONTB}
  COMMAND ${B2O}  ${29_day_harib26a_FONTB} ${29_day_harib26a_FONTO} _hankaku
  DEPENDS ${29_day_harib26a_FONTS} makefont bin2obj
  COMMENT "Generating hankaku.o for 29_day_harib26a"
)
add_custom_command(
  OUTPUT ${29_day_harib26a_LIBGC}
  COMMAND ${CONV} -fcoff32 -nu ${29_day_harib26a_LIBGE} ${29_day_harib26a_LIBGC}
  DEPENDS ${29_day_harib26a_LIBGE}
  COMMENT "Converting golibc for 29_day_harib26a"
)

add_custom_target(29_day_harib26a_sys
  COMMAND gcc ${BINOPT} -T ${29_day_harib26a_LDS} ${29_day_harib26a_CCS} ${29_day_harib26a_WILDOBJ} -o ${29_day_harib26a_BOOTB}
  COMMAND cat ${29_day_harib26a_HEADB} ${29_day_harib26a_BOOTB} > ${29_day_harib26a_SYS}
  DEPENDS ${29_day_harib26a_HEADB} 29_day_harib26a_ipl
  DEPENDS ${29_day_harib26a_FUNCO}
  DEPENDS ${29_day_harib26a_FONTO} ${29_day_harib26a_LIBGC}
)
add_custom_target(29_day_harib26a_img
  COMMAND mformat -f 1440 -l HARIBOTEOS -N 0xffffffff -C -B ${29_day_harib26a_IPLB} -i ${29_day_harib26a_OS}
  COMMAND mcopy -i ${29_day_harib26a_OS} ${29_day_harib26a_SYS} ::
  COMMAND mcopy -i ${29_day_harib26a_OS} ${root_BINARY_DIR}/projects/29_day/harib26a/a.hrb ::a.hrb
  COMMAND mcopy -i ${29_day_harib26a_OS} ${root_BINARY_DIR}/projects/29_day/harib26a/hello3.hrb ::hello3.hrb
  COMMAND mcopy -i ${29_day_harib26a_OS} ${root_BINARY_DIR}/projects/29_day/harib26a/hello4.hrb ::hello4.hrb
  COMMAND mcopy -i ${29_day_harib26a_OS} ${root_BINARY_DIR}/projects/29_day/harib26a/winhelo.hrb ::winhelo.hrb
  COMMAND mcopy -i ${29_day_harib26a_OS} ${root_BINARY_DIR}/projects/29_day/harib26a/winhelo2.hrb ::winhelo2.hrb
  COMMAND mcopy -i ${29_day_harib26a_OS} ${root_BINARY_DIR}/projects/29_day/harib26a/winhelo3.hrb ::winhelo3.hrb
  COMMAND mcopy -i ${29_day_harib26a_OS} ${root_BINARY_DIR}/projects/29_day/harib26a/star1.hrb ::star1.hrb
  COMMAND mcopy -i ${29_day_harib26a_OS} ${root_BINARY_DIR}/projects/29_day/harib26a/stars.hrb ::stars.hrb
  COMMAND mcopy -i ${29_day_harib26a_OS} ${root_BINARY_DIR}/projects/29_day/harib26a/stars2.hrb ::stars2.hrb
  COMMAND mcopy -i ${29_day_harib26a_OS} ${root_BINARY_DIR}/projects/29_day/harib26a/lines.hrb ::lines.hrb
  COMMAND mcopy -i ${29_day_harib26a_OS} ${root_BINARY_DIR}/projects/29_day/harib26a/walk.hrb ::walk.hrb
  COMMAND mcopy -i ${29_day_harib26a_OS} ${root_BINARY_DIR}/projects/29_day/harib26a/chklang.hrb ::chklang.hrb
  COMMAND mcopy -i ${29_day_harib26a_OS} ${root_BINARY_DIR}/projects/29_day/harib26a/iroha.hrb ::iroha.hrb
  COMMAND mcopy -i ${29_day_harib26a_OS} ${root_BINARY_DIR}/projects/29_day/harib26a/type.hrb ::type.hrb
  COMMAND mcopy -i ${29_day_harib26a_OS} ${root_BINARY_DIR}/projects/29_day/harib26a/typeipl.hrb ::typeipl.hrb
  COMMAND mcopy -i ${29_day_harib26a_OS} ${root_BINARY_DIR}/projects/29_day/harib26a/sosu3.hrb ::sosu3.hrb
  COMMAND mcopy -i ${29_day_harib26a_OS} ${root_BINARY_DIR}/projects/29_day/harib26a/sosu2.hrb ::sosu2.hrb
  COMMAND mcopy -i ${29_day_harib26a_OS} ${root_BINARY_DIR}/projects/29_day/harib26a/sosu.hrb ::sosu.hrb
  COMMAND mcopy -i ${29_day_harib26a_OS} ${root_BINARY_DIR}/projects/29_day/harib26a/color2.hrb ::color2.hrb
  COMMAND mcopy -i ${29_day_harib26a_OS} ${root_BINARY_DIR}/projects/29_day/harib26a/color.hrb ::color.hrb
  COMMAND mcopy -i ${29_day_harib26a_OS} ${root_BINARY_DIR}/projects/29_day/harib26a/beepdown.hrb ::beepdown.hrb
  COMMAND mcopy -i ${29_day_harib26a_OS} ${root_BINARY_DIR}/projects/29_day/harib26a/noodle.hrb ::noodle.hrb
  COMMAND mcopy -i ${29_day_harib26a_OS} ${root_BINARY_DIR}/projects/29_day/harib26a/hello5.hrb ::hello5.hrb
  COMMAND mcopy -i ${29_day_harib26a_OS} ${root_SOURCE_DIR}/projects/29_day/harib26a/euc.txt ::euc.txt
  COMMAND mcopy -i ${29_day_harib26a_OS} ${root_SOURCE_DIR}/projects/29_day/harib26a/nihongo/nihongo.fnt ::nihongo.fnt
  DEPENDS 29_day_harib26a_sys ${root_BINARY_DIR}/projects/29_day/harib26a/a.hrb ${root_BINARY_DIR}/projects/29_day/harib26a/hello3.hrb ${root_BINARY_DIR}/projects/29_day/harib26a/hello4.hrb ${root_BINARY_DIR}/projects/29_day/harib26a/winhelo.hrb ${root_BINARY_DIR}/projects/29_day/harib26a/winhelo2.hrb ${root_BINARY_DIR}/projects/29_day/harib26a/winhelo3.hrb ${root_BINARY_DIR}/projects/29_day/harib26a/star1.hrb ${root_BINARY_DIR}/projects/29_day/harib26a/stars.hrb ${root_BINARY_DIR}/projects/29_day/harib26a/stars2.hrb ${root_BINARY_DIR}/projects/29_day/harib26a/lines.hrb ${root_BINARY_DIR}/projects/29_day/harib26a/walk.hrb ${root_BINARY_DIR}/projects/29_day/harib26a/chklang.hrb ${root_BINARY_DIR}/projects/29_day/harib26a/iroha.hrb ${root_BINARY_DIR}/projects/29_day/harib26a/type.hrb ${root_BINARY_DIR}/projects/29_day/harib26a/typeipl.hrb ${root_BINARY_DIR}/projects/29_day/harib26a/sosu3.hrb ${root_BINARY_DIR}/projects/29_day/harib26a/sosu2.hrb ${root_BINARY_DIR}/projects/29_day/harib26a/sosu.hrb ${root_BINARY_DIR}/projects/29_day/harib26a/noodle.hrb ${root_BINARY_DIR}/projects/29_day/harib26a/beepdown.hrb ${root_BINARY_DIR}/projects/29_day/harib26a/color.hrb ${root_BINARY_DIR}/projects/29_day/harib26a/color2.hrb ${root_BINARY_DIR}/projects/29_day/harib26a/hello5.hrb ${root_SOURCE_DIR}/projects/29_day/harib26a/nihongo/nihongo.fnt ${root_SOURCE_DIR}/projects/29_day/harib26a/euc.txt
)

#----------------------------------------------------------
########### application .hrb files (copied into os.img) ###############
set(29_day_harib26a_APIO
  ${root_BINARY_DIR}/projects/29_day/harib26a/api001.o
  ${root_BINARY_DIR}/projects/29_day/harib26a/api002.o
  ${root_BINARY_DIR}/projects/29_day/harib26a/api003.o
  ${root_BINARY_DIR}/projects/29_day/harib26a/api004.o
  ${root_BINARY_DIR}/projects/29_day/harib26a/api005.o
  ${root_BINARY_DIR}/projects/29_day/harib26a/api006.o
  ${root_BINARY_DIR}/projects/29_day/harib26a/api007.o
  ${root_BINARY_DIR}/projects/29_day/harib26a/api008.o
  ${root_BINARY_DIR}/projects/29_day/harib26a/api009.o
  ${root_BINARY_DIR}/projects/29_day/harib26a/api010.o
  ${root_BINARY_DIR}/projects/29_day/harib26a/api011.o
  ${root_BINARY_DIR}/projects/29_day/harib26a/api012.o
  ${root_BINARY_DIR}/projects/29_day/harib26a/api013.o
  ${root_BINARY_DIR}/projects/29_day/harib26a/api014.o
  ${root_BINARY_DIR}/projects/29_day/harib26a/api015.o
  ${root_BINARY_DIR}/projects/29_day/harib26a/api016.o
  ${root_BINARY_DIR}/projects/29_day/harib26a/api017.o
  ${root_BINARY_DIR}/projects/29_day/harib26a/api018.o
  ${root_BINARY_DIR}/projects/29_day/harib26a/api019.o
  ${root_BINARY_DIR}/projects/29_day/harib26a/api020.o
  ${root_BINARY_DIR}/projects/29_day/harib26a/api021.o
  ${root_BINARY_DIR}/projects/29_day/harib26a/api022.o
  ${root_BINARY_DIR}/projects/29_day/harib26a/api023.o
  ${root_BINARY_DIR}/projects/29_day/harib26a/api024.o
  ${root_BINARY_DIR}/projects/29_day/harib26a/api025.o
  ${root_BINARY_DIR}/projects/29_day/harib26a/api026.o
  ${root_BINARY_DIR}/projects/29_day/harib26a/api027.o)
set(29_day_harib26a_RANDO ${root_BINARY_DIR}/projects/29_day/harib26a/rand.o)
add_custom_command(
  OUTPUT ${29_day_harib26a_RANDO}
  COMMAND gcc -fleading-underscore -m32 -nostdlib -fno-pie -x c -c ${root_SOURCE_DIR}/golibc/rand.c_ -o ${29_day_harib26a_RANDO}
  DEPENDS ${root_SOURCE_DIR}/golibc/rand.c_
  COMMENT "Generating rand.o for 29_day_harib26a"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/29_day/harib26a/api001.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/29_day/harib26a/apilib/api001.nas ${root_BINARY_DIR}/projects/29_day/harib26a/api001.o
  DEPENDS ${root_SOURCE_DIR}/projects/29_day/harib26a/apilib/api001.nas
  COMMENT "Generating api001.o for 29_day_harib26a"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/29_day/harib26a/api002.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/29_day/harib26a/apilib/api002.nas ${root_BINARY_DIR}/projects/29_day/harib26a/api002.o
  DEPENDS ${root_SOURCE_DIR}/projects/29_day/harib26a/apilib/api002.nas
  COMMENT "Generating api002.o for 29_day_harib26a"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/29_day/harib26a/api003.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/29_day/harib26a/apilib/api003.nas ${root_BINARY_DIR}/projects/29_day/harib26a/api003.o
  DEPENDS ${root_SOURCE_DIR}/projects/29_day/harib26a/apilib/api003.nas
  COMMENT "Generating api003.o for 29_day_harib26a"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/29_day/harib26a/api004.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/29_day/harib26a/apilib/api004.nas ${root_BINARY_DIR}/projects/29_day/harib26a/api004.o
  DEPENDS ${root_SOURCE_DIR}/projects/29_day/harib26a/apilib/api004.nas
  COMMENT "Generating api004.o for 29_day_harib26a"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/29_day/harib26a/api005.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/29_day/harib26a/apilib/api005.nas ${root_BINARY_DIR}/projects/29_day/harib26a/api005.o
  DEPENDS ${root_SOURCE_DIR}/projects/29_day/harib26a/apilib/api005.nas
  COMMENT "Generating api005.o for 29_day_harib26a"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/29_day/harib26a/api006.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/29_day/harib26a/apilib/api006.nas ${root_BINARY_DIR}/projects/29_day/harib26a/api006.o
  DEPENDS ${root_SOURCE_DIR}/projects/29_day/harib26a/apilib/api006.nas
  COMMENT "Generating api006.o for 29_day_harib26a"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/29_day/harib26a/api007.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/29_day/harib26a/apilib/api007.nas ${root_BINARY_DIR}/projects/29_day/harib26a/api007.o
  DEPENDS ${root_SOURCE_DIR}/projects/29_day/harib26a/apilib/api007.nas
  COMMENT "Generating api007.o for 29_day_harib26a"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/29_day/harib26a/api008.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/29_day/harib26a/apilib/api008.nas ${root_BINARY_DIR}/projects/29_day/harib26a/api008.o
  DEPENDS ${root_SOURCE_DIR}/projects/29_day/harib26a/apilib/api008.nas
  COMMENT "Generating api008.o for 29_day_harib26a"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/29_day/harib26a/api009.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/29_day/harib26a/apilib/api009.nas ${root_BINARY_DIR}/projects/29_day/harib26a/api009.o
  DEPENDS ${root_SOURCE_DIR}/projects/29_day/harib26a/apilib/api009.nas
  COMMENT "Generating api009.o for 29_day_harib26a"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/29_day/harib26a/api010.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/29_day/harib26a/apilib/api010.nas ${root_BINARY_DIR}/projects/29_day/harib26a/api010.o
  DEPENDS ${root_SOURCE_DIR}/projects/29_day/harib26a/apilib/api010.nas
  COMMENT "Generating api010.o for 29_day_harib26a"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/29_day/harib26a/api011.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/29_day/harib26a/apilib/api011.nas ${root_BINARY_DIR}/projects/29_day/harib26a/api011.o
  DEPENDS ${root_SOURCE_DIR}/projects/29_day/harib26a/apilib/api011.nas
  COMMENT "Generating api011.o for 29_day_harib26a"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/29_day/harib26a/api012.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/29_day/harib26a/apilib/api012.nas ${root_BINARY_DIR}/projects/29_day/harib26a/api012.o
  DEPENDS ${root_SOURCE_DIR}/projects/29_day/harib26a/apilib/api012.nas
  COMMENT "Generating api012.o for 29_day_harib26a"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/29_day/harib26a/api013.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/29_day/harib26a/apilib/api013.nas ${root_BINARY_DIR}/projects/29_day/harib26a/api013.o
  DEPENDS ${root_SOURCE_DIR}/projects/29_day/harib26a/apilib/api013.nas
  COMMENT "Generating api013.o for 29_day_harib26a"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/29_day/harib26a/api014.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/29_day/harib26a/apilib/api014.nas ${root_BINARY_DIR}/projects/29_day/harib26a/api014.o
  DEPENDS ${root_SOURCE_DIR}/projects/29_day/harib26a/apilib/api014.nas
  COMMENT "Generating api014.o for 29_day_harib26a"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/29_day/harib26a/api015.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/29_day/harib26a/apilib/api015.nas ${root_BINARY_DIR}/projects/29_day/harib26a/api015.o
  DEPENDS ${root_SOURCE_DIR}/projects/29_day/harib26a/apilib/api015.nas
  COMMENT "Generating api015.o for 29_day_harib26a"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/29_day/harib26a/api016.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/29_day/harib26a/apilib/api016.nas ${root_BINARY_DIR}/projects/29_day/harib26a/api016.o
  DEPENDS ${root_SOURCE_DIR}/projects/29_day/harib26a/apilib/api016.nas
  COMMENT "Generating api016.o for 29_day_harib26a"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/29_day/harib26a/api017.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/29_day/harib26a/apilib/api017.nas ${root_BINARY_DIR}/projects/29_day/harib26a/api017.o
  DEPENDS ${root_SOURCE_DIR}/projects/29_day/harib26a/apilib/api017.nas
  COMMENT "Generating api017.o for 29_day_harib26a"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/29_day/harib26a/api018.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/29_day/harib26a/apilib/api018.nas ${root_BINARY_DIR}/projects/29_day/harib26a/api018.o
  DEPENDS ${root_SOURCE_DIR}/projects/29_day/harib26a/apilib/api018.nas
  COMMENT "Generating api018.o for 29_day_harib26a"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/29_day/harib26a/api019.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/29_day/harib26a/apilib/api019.nas ${root_BINARY_DIR}/projects/29_day/harib26a/api019.o
  DEPENDS ${root_SOURCE_DIR}/projects/29_day/harib26a/apilib/api019.nas
  COMMENT "Generating api019.o for 29_day_harib26a"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/29_day/harib26a/api020.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/29_day/harib26a/apilib/api020.nas ${root_BINARY_DIR}/projects/29_day/harib26a/api020.o
  DEPENDS ${root_SOURCE_DIR}/projects/29_day/harib26a/apilib/api020.nas
  COMMENT "Generating api020.o for 29_day_harib26a"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/29_day/harib26a/api021.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/29_day/harib26a/apilib/api021.nas ${root_BINARY_DIR}/projects/29_day/harib26a/api021.o
  DEPENDS ${root_SOURCE_DIR}/projects/29_day/harib26a/apilib/api021.nas
  COMMENT "Generating api021.o for 29_day_harib26a"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/29_day/harib26a/api022.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/29_day/harib26a/apilib/api022.nas ${root_BINARY_DIR}/projects/29_day/harib26a/api022.o
  DEPENDS ${root_SOURCE_DIR}/projects/29_day/harib26a/apilib/api022.nas
  COMMENT "Generating api022.o for 29_day_harib26a"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/29_day/harib26a/api023.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/29_day/harib26a/apilib/api023.nas ${root_BINARY_DIR}/projects/29_day/harib26a/api023.o
  DEPENDS ${root_SOURCE_DIR}/projects/29_day/harib26a/apilib/api023.nas
  COMMENT "Generating api023.o for 29_day_harib26a"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/29_day/harib26a/api024.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/29_day/harib26a/apilib/api024.nas ${root_BINARY_DIR}/projects/29_day/harib26a/api024.o
  DEPENDS ${root_SOURCE_DIR}/projects/29_day/harib26a/apilib/api024.nas
  COMMENT "Generating api024.o for 29_day_harib26a"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/29_day/harib26a/api025.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/29_day/harib26a/apilib/api025.nas ${root_BINARY_DIR}/projects/29_day/harib26a/api025.o
  DEPENDS ${root_SOURCE_DIR}/projects/29_day/harib26a/apilib/api025.nas
  COMMENT "Generating api025.o for 29_day_harib26a"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/29_day/harib26a/api026.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/29_day/harib26a/apilib/api026.nas ${root_BINARY_DIR}/projects/29_day/harib26a/api026.o
  DEPENDS ${root_SOURCE_DIR}/projects/29_day/harib26a/apilib/api026.nas
  COMMENT "Generating api026.o for 29_day_harib26a"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/29_day/harib26a/api027.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/29_day/harib26a/apilib/api027.nas ${root_BINARY_DIR}/projects/29_day/harib26a/api027.o
  DEPENDS ${root_SOURCE_DIR}/projects/29_day/harib26a/apilib/api027.nas
  COMMENT "Generating api027.o for 29_day_harib26a"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/29_day/harib26a/a.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/projects/29_day/harib26a -T ${29_day_harib26a_APPLDS} ${root_SOURCE_DIR}/projects/29_day/harib26a/a/a.c ${29_day_harib26a_APIO} ${29_day_harib26a_LIBGC} ${29_day_harib26a_RANDO} -o ${root_BINARY_DIR}/projects/29_day/harib26a/a.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/29_day/harib26a/a/a.c ${root_SOURCE_DIR}/projects/app.lds ${29_day_harib26a_APIO} ${29_day_harib26a_LIBGC} ${29_day_harib26a_RANDO}
  COMMENT "Generating a.hrb for 29_day_harib26a"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/29_day/harib26a/hello3.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/projects/29_day/harib26a -T ${29_day_harib26a_APPLDS} ${root_SOURCE_DIR}/projects/29_day/harib26a/hello3/hello3.c ${29_day_harib26a_APIO} ${29_day_harib26a_LIBGC} ${29_day_harib26a_RANDO} -o ${root_BINARY_DIR}/projects/29_day/harib26a/hello3.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/29_day/harib26a/hello3/hello3.c ${root_SOURCE_DIR}/projects/app.lds ${29_day_harib26a_APIO} ${29_day_harib26a_LIBGC} ${29_day_harib26a_RANDO}
  COMMENT "Generating hello3.hrb for 29_day_harib26a"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/29_day/harib26a/hello4.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/projects/29_day/harib26a -T ${29_day_harib26a_APPLDS} ${root_SOURCE_DIR}/projects/29_day/harib26a/hello4/hello4.c ${29_day_harib26a_APIO} ${29_day_harib26a_LIBGC} ${29_day_harib26a_RANDO} -o ${root_BINARY_DIR}/projects/29_day/harib26a/hello4.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/29_day/harib26a/hello4/hello4.c ${root_SOURCE_DIR}/projects/app.lds ${29_day_harib26a_APIO} ${29_day_harib26a_LIBGC} ${29_day_harib26a_RANDO}
  COMMENT "Generating hello4.hrb for 29_day_harib26a"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/29_day/harib26a/winhelo.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/projects/29_day/harib26a -T ${29_day_harib26a_APPLDS} ${root_SOURCE_DIR}/projects/29_day/harib26a/winhelo/winhelo.c ${29_day_harib26a_APIO} ${29_day_harib26a_LIBGC} ${29_day_harib26a_RANDO} -o ${root_BINARY_DIR}/projects/29_day/harib26a/winhelo.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/29_day/harib26a/winhelo/winhelo.c ${root_SOURCE_DIR}/projects/app.lds ${29_day_harib26a_APIO} ${29_day_harib26a_LIBGC} ${29_day_harib26a_RANDO}
  COMMENT "Generating winhelo.hrb for 29_day_harib26a"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/29_day/harib26a/winhelo2.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/projects/29_day/harib26a -T ${29_day_harib26a_APPLDS} ${root_SOURCE_DIR}/projects/29_day/harib26a/winhelo2/winhelo2.c ${29_day_harib26a_APIO} ${29_day_harib26a_LIBGC} ${29_day_harib26a_RANDO} -o ${root_BINARY_DIR}/projects/29_day/harib26a/winhelo2.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/29_day/harib26a/winhelo2/winhelo2.c ${root_SOURCE_DIR}/projects/app.lds ${29_day_harib26a_APIO} ${29_day_harib26a_LIBGC} ${29_day_harib26a_RANDO}
  COMMENT "Generating winhelo2.hrb for 29_day_harib26a"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/29_day/harib26a/winhelo3.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/projects/29_day/harib26a -T ${29_day_harib26a_APPLDS} ${root_SOURCE_DIR}/projects/29_day/harib26a/winhelo3/winhelo3.c ${29_day_harib26a_APIO} ${29_day_harib26a_LIBGC} ${29_day_harib26a_RANDO} -o ${root_BINARY_DIR}/projects/29_day/harib26a/winhelo3.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/29_day/harib26a/winhelo3/winhelo3.c ${root_SOURCE_DIR}/projects/app.lds ${29_day_harib26a_APIO} ${29_day_harib26a_LIBGC} ${29_day_harib26a_RANDO}
  COMMENT "Generating winhelo3.hrb for 29_day_harib26a"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/29_day/harib26a/star1.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/projects/29_day/harib26a -T ${29_day_harib26a_APPLDS} ${root_SOURCE_DIR}/projects/29_day/harib26a/star1/star1.c ${29_day_harib26a_APIO} ${29_day_harib26a_LIBGC} ${29_day_harib26a_RANDO} -o ${root_BINARY_DIR}/projects/29_day/harib26a/star1.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/29_day/harib26a/star1/star1.c ${root_SOURCE_DIR}/projects/app.lds ${29_day_harib26a_APIO} ${29_day_harib26a_LIBGC} ${29_day_harib26a_RANDO}
  COMMENT "Generating star1.hrb for 29_day_harib26a"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/29_day/harib26a/stars.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/projects/29_day/harib26a -T ${29_day_harib26a_APPLDS} ${root_SOURCE_DIR}/projects/29_day/harib26a/stars/stars.c ${29_day_harib26a_APIO} ${29_day_harib26a_LIBGC} ${29_day_harib26a_RANDO} -o ${root_BINARY_DIR}/projects/29_day/harib26a/stars.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/29_day/harib26a/stars/stars.c ${root_SOURCE_DIR}/projects/app.lds ${29_day_harib26a_APIO} ${29_day_harib26a_LIBGC} ${29_day_harib26a_RANDO}
  COMMENT "Generating stars.hrb for 29_day_harib26a"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/29_day/harib26a/stars2.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/projects/29_day/harib26a -T ${29_day_harib26a_APPLDS} ${root_SOURCE_DIR}/projects/29_day/harib26a/stars2/stars2.c ${29_day_harib26a_APIO} ${29_day_harib26a_LIBGC} ${29_day_harib26a_RANDO} -o ${root_BINARY_DIR}/projects/29_day/harib26a/stars2.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/29_day/harib26a/stars2/stars2.c ${root_SOURCE_DIR}/projects/app.lds ${29_day_harib26a_APIO} ${29_day_harib26a_LIBGC} ${29_day_harib26a_RANDO}
  COMMENT "Generating stars2.hrb for 29_day_harib26a"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/29_day/harib26a/lines.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/projects/29_day/harib26a -T ${29_day_harib26a_APPLDS} ${root_SOURCE_DIR}/projects/29_day/harib26a/lines/lines.c ${29_day_harib26a_APIO} ${29_day_harib26a_LIBGC} ${29_day_harib26a_RANDO} -o ${root_BINARY_DIR}/projects/29_day/harib26a/lines.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/29_day/harib26a/lines/lines.c ${root_SOURCE_DIR}/projects/app.lds ${29_day_harib26a_APIO} ${29_day_harib26a_LIBGC} ${29_day_harib26a_RANDO}
  COMMENT "Generating lines.hrb for 29_day_harib26a"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/29_day/harib26a/walk.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/projects/29_day/harib26a -T ${29_day_harib26a_APPLDS} ${root_SOURCE_DIR}/projects/29_day/harib26a/walk/walk.c ${29_day_harib26a_APIO} ${29_day_harib26a_LIBGC} ${29_day_harib26a_RANDO} -o ${root_BINARY_DIR}/projects/29_day/harib26a/walk.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/29_day/harib26a/walk/walk.c ${root_SOURCE_DIR}/projects/app.lds ${29_day_harib26a_APIO} ${29_day_harib26a_LIBGC} ${29_day_harib26a_RANDO}
  COMMENT "Generating walk.hrb for 29_day_harib26a"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/29_day/harib26a/chklang.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/projects/29_day/harib26a -T ${29_day_harib26a_APPLDS} ${root_SOURCE_DIR}/projects/29_day/harib26a/chklang/chklang.c ${29_day_harib26a_APIO} ${29_day_harib26a_LIBGC} ${29_day_harib26a_RANDO} -o ${root_BINARY_DIR}/projects/29_day/harib26a/chklang.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/29_day/harib26a/chklang/chklang.c ${root_SOURCE_DIR}/projects/app.lds ${29_day_harib26a_APIO} ${29_day_harib26a_LIBGC} ${29_day_harib26a_RANDO}
  COMMENT "Generating chklang.hrb for 29_day_harib26a"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/29_day/harib26a/iroha.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/projects/29_day/harib26a -T ${29_day_harib26a_APPLDS} ${root_SOURCE_DIR}/projects/29_day/harib26a/iroha/iroha.c ${29_day_harib26a_APIO} ${29_day_harib26a_LIBGC} ${29_day_harib26a_RANDO} -o ${root_BINARY_DIR}/projects/29_day/harib26a/iroha.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/29_day/harib26a/iroha/iroha.c ${root_SOURCE_DIR}/projects/app.lds ${29_day_harib26a_APIO} ${29_day_harib26a_LIBGC} ${29_day_harib26a_RANDO}
  COMMENT "Generating iroha.hrb for 29_day_harib26a"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/29_day/harib26a/type.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/projects/29_day/harib26a -T ${29_day_harib26a_APPLDS} ${root_SOURCE_DIR}/projects/29_day/harib26a/type/type.c ${29_day_harib26a_APIO} ${29_day_harib26a_LIBGC} ${29_day_harib26a_RANDO} -o ${root_BINARY_DIR}/projects/29_day/harib26a/type.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/29_day/harib26a/type/type.c ${root_SOURCE_DIR}/projects/app.lds ${29_day_harib26a_APIO} ${29_day_harib26a_LIBGC} ${29_day_harib26a_RANDO}
  COMMENT "Generating type.hrb for 29_day_harib26a"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/29_day/harib26a/typeipl.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/projects/29_day/harib26a -T ${29_day_harib26a_APPLDS} ${root_SOURCE_DIR}/projects/29_day/harib26a/typeipl/typeipl.c ${29_day_harib26a_APIO} ${29_day_harib26a_LIBGC} ${29_day_harib26a_RANDO} -o ${root_BINARY_DIR}/projects/29_day/harib26a/typeipl.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/29_day/harib26a/typeipl/typeipl.c ${root_SOURCE_DIR}/projects/app.lds ${29_day_harib26a_APIO} ${29_day_harib26a_LIBGC} ${29_day_harib26a_RANDO}
  COMMENT "Generating typeipl.hrb for 29_day_harib26a"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/29_day/harib26a/sosu3.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/projects/29_day/harib26a -T ${29_day_harib26a_APPLDS} ${root_SOURCE_DIR}/projects/29_day/harib26a/sosu3/sosu3.c ${29_day_harib26a_APIO} ${29_day_harib26a_LIBGC} ${29_day_harib26a_RANDO} -o ${root_BINARY_DIR}/projects/29_day/harib26a/sosu3.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/29_day/harib26a/sosu3/sosu3.c ${root_SOURCE_DIR}/projects/app.lds ${29_day_harib26a_APIO} ${29_day_harib26a_LIBGC} ${29_day_harib26a_RANDO}
  COMMENT "Generating sosu3.hrb for 29_day_harib26a"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/29_day/harib26a/sosu2.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/projects/29_day/harib26a -T ${29_day_harib26a_APPLDS} ${root_SOURCE_DIR}/projects/29_day/harib26a/sosu2/sosu2.c ${29_day_harib26a_APIO} ${29_day_harib26a_LIBGC} ${29_day_harib26a_RANDO} -o ${root_BINARY_DIR}/projects/29_day/harib26a/sosu2.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/29_day/harib26a/sosu2/sosu2.c ${root_SOURCE_DIR}/projects/app.lds ${29_day_harib26a_APIO} ${29_day_harib26a_LIBGC} ${29_day_harib26a_RANDO}
  COMMENT "Generating sosu2.hrb for 29_day_harib26a"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/29_day/harib26a/sosu.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/projects/29_day/harib26a -T ${29_day_harib26a_APPLDS} ${root_SOURCE_DIR}/projects/29_day/harib26a/sosu/sosu.c ${29_day_harib26a_APIO} ${29_day_harib26a_LIBGC} ${29_day_harib26a_RANDO} -o ${root_BINARY_DIR}/projects/29_day/harib26a/sosu.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/29_day/harib26a/sosu/sosu.c ${root_SOURCE_DIR}/projects/app.lds ${29_day_harib26a_APIO} ${29_day_harib26a_LIBGC} ${29_day_harib26a_RANDO}
  COMMENT "Generating sosu.hrb for 29_day_harib26a"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/29_day/harib26a/color2.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/projects/29_day/harib26a -T ${29_day_harib26a_APPLDS} ${root_SOURCE_DIR}/projects/29_day/harib26a/color2/color2.c ${29_day_harib26a_APIO} ${29_day_harib26a_LIBGC} ${29_day_harib26a_RANDO} -o ${root_BINARY_DIR}/projects/29_day/harib26a/color2.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/29_day/harib26a/color2/color2.c ${root_SOURCE_DIR}/projects/app.lds ${29_day_harib26a_APIO} ${29_day_harib26a_LIBGC} ${29_day_harib26a_RANDO}
  COMMENT "Generating color2.hrb for 29_day_harib26a"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/29_day/harib26a/color.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/projects/29_day/harib26a -T ${29_day_harib26a_APPLDS} ${root_SOURCE_DIR}/projects/29_day/harib26a/color/color.c ${29_day_harib26a_APIO} ${29_day_harib26a_LIBGC} ${29_day_harib26a_RANDO} -o ${root_BINARY_DIR}/projects/29_day/harib26a/color.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/29_day/harib26a/color/color.c ${root_SOURCE_DIR}/projects/app.lds ${29_day_harib26a_APIO} ${29_day_harib26a_LIBGC} ${29_day_harib26a_RANDO}
  COMMENT "Generating color.hrb for 29_day_harib26a"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/29_day/harib26a/beepdown.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/projects/29_day/harib26a -T ${29_day_harib26a_APPLDS} ${root_SOURCE_DIR}/projects/29_day/harib26a/beepdown/beepdown.c ${29_day_harib26a_APIO} ${29_day_harib26a_LIBGC} ${29_day_harib26a_RANDO} -o ${root_BINARY_DIR}/projects/29_day/harib26a/beepdown.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/29_day/harib26a/beepdown/beepdown.c ${root_SOURCE_DIR}/projects/app.lds ${29_day_harib26a_APIO} ${29_day_harib26a_LIBGC} ${29_day_harib26a_RANDO}
  COMMENT "Generating beepdown.hrb for 29_day_harib26a"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/29_day/harib26a/noodle.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/projects/29_day/harib26a -T ${29_day_harib26a_APPLDS} ${root_SOURCE_DIR}/projects/29_day/harib26a/noodle/noodle.c ${29_day_harib26a_APIO} ${29_day_harib26a_LIBGC} ${29_day_harib26a_RANDO} -o ${root_BINARY_DIR}/projects/29_day/harib26a/noodle.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/29_day/harib26a/noodle/noodle.c ${root_SOURCE_DIR}/projects/app.lds ${29_day_harib26a_APIO} ${29_day_harib26a_LIBGC} ${29_day_harib26a_RANDO}
  COMMENT "Generating noodle.hrb for 29_day_harib26a"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/29_day/harib26a/hello5.hrb
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/29_day/harib26a/hello5/hello5.nas ${root_BINARY_DIR}/projects/29_day/harib26a/hello5.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/29_day/harib26a/hello5/hello5.nas
  COMMENT "Generating hello5.hrb for 29_day_harib26a"
)
