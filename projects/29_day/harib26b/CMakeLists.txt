#----------------------------------------------------------
message(STATUS "Entering directory projects/29_day/harib26b/")

set(FONT ${root_BINARY_DIR}/src/makefont)
set(B2O  ${root_BINARY_DIR}/src/bin2obj)
set(CONV ${root_BINARY_DIR}/objconv/objconv)
set(29_day_harib26b_OS    ${root_BINARY_DIR}/projects/29_day/harib26b/os.img)
set(29_day_harib26b_SYS	  ${root_BINARY_DIR}/projects/29_day/harib26b/os.sys)
set(29_day_harib26b_IPLB  ${root_BINARY_DIR}/projects/29_day/harib26b/ipl.bin)
set(29_day_harib26b_IPLS  ${root_SOURCE_DIR}/projects/29_day/harib26b/haribote/ipl20.nas)
set(29_day_harib26b_HEADB ${root_BINARY_DIR}/projects/29_day/harib26b/asmhead.bin)
set(29_day_harib26b_HEADS ${root_SOURCE_DIR}/projects/29_day/harib26b/haribote/asmhead.nas)
set(29_day_harib26b_BOOTB ${root_BINARY_DIR}/projects/29_day/harib26b/boot.bin)
set(29_day_harib26b_CCO	  ${root_BINARY_DIR}/projects/29_day/harib26b/boot.o)
file(GLOB 29_day_harib26b_CCS ${root_SOURCE_DIR}/projects/29_day/harib26b/haribote/*.c)
set(29_day_harib26b_LDS	  ${root_SOURCE_DIR}/projects/os.lds)
set(29_day_harib26b_APPLDS  ${root_SOURCE_DIR}/projects/app.lds)
set(29_day_harib26b_FUNCS ${root_SOURCE_DIR}/projects/29_day/harib26b/haribote/naskfunc.nas)
set(29_day_harib26b_FUNCO ${root_BINARY_DIR}/projects/29_day/harib26b/naskfunc.o)
set(29_day_harib26b_FONTS ${root_SOURCE_DIR}/projects/29_day/harib26b/haribote/hankaku.txt)
set(29_day_harib26b_FONTB ${root_BINARY_DIR}/projects/29_day/harib26b/hankaku.bin)
set(29_day_harib26b_FONTO ${root_BINARY_DIR}/projects/29_day/harib26b/hankaku.o)
set(29_day_harib26b_LIBGE ${root_BINARY_DIR}/golibc/libgo.a)
set(29_day_harib26b_LIBGC ${root_BINARY_DIR}/projects/29_day/harib26b/libgo.o)
set(29_day_harib26b_WILDOBJ ${root_BINARY_DIR}/projects/29_day/harib26b/*.o)


add_custom_target(29_day_harib26b_run
  COMMAND ${QEMU} ${QEMUOPT} ${29_day_harib26b_OS}
  DEPENDS 29_day_harib26b_img
)
add_custom_target(29_day_harib26b_debug
  COMMAND ${QEMU} -s -S ${QEMUOPT} ${29_day_harib26b_OS} ${QEMU_DEBUG_OPT}
  DEPENDS 29_day_harib26b_img
)
add_custom_target(29_day_harib26b_clean
  COMMAND rm -f ${29_day_harib26b_OS}
  COMMAND rm -f ${29_day_harib26b_SYS}
  COMMAND rm -f ${29_day_harib26b_IPLB}
  COMMAND rm -f ${29_day_harib26b_HEADB}
  COMMAND rm -f ${29_day_harib26b_BOOTB}
  COMMAND rm -f ${29_day_harib26b_WILDOBJ}
)
add_custom_target(29_day_harib26b_ipl
  COMMAND ${RAKUSK_EXECUTABLE} ${29_day_harib26b_IPLS} ${29_day_harib26b_IPLB}
  DEPENDS ${29_day_harib26b_IPLS}
)
add_custom_command(
  OUTPUT ${29_day_harib26b_HEADB}
  COMMAND ${RAKUSK_EXECUTABLE} ${29_day_harib26b_HEADS} ${29_day_harib26b_HEADB}
  DEPENDS ${29_day_harib26b_HEADS}
  COMMENT "Generating asmhead.bin for 29_day_harib26b"
)
add_custom_command(
  OUTPUT ${29_day_harib26b_FUNCO}
  COMMAND ${RAKUSK_EXECUTABLE} ${29_day_harib26b_FUNCS} ${29_day_harib26b_FUNCO}
  DEPENDS ${29_day_harib26b_FUNCS}
  COMMENT "Generating naskfunc.o for 29_day_harib26b"
)
add_custom_command(
  OUTPUT ${29_day_harib26b_FONTO}
  COMMAND ${FONT} ${29_day_harib26b_FONTS} ${29_day_harib26b_FONTB}
  COMMAND ${B2O}  ${29_day_harib26b_FONTB} ${29_day_harib26b_FONTO} _hankaku
  DEPENDS ${29_day_harib26b_FONTS} makefont bin2obj
  COMMENT "Generating hankaku.o for 29_day_harib26b"
)
add_custom_command(
  OUTPUT ${29_day_harib26b_LIBGC}
  COMMAND ${CONV} -fcoff32 -nu ${29_day_harib26b_LIBGE} ${29_day_harib26b_LIBGC}
  DEPENDS ${29_day_harib26b_LIBGE}
  COMMENT "Converting golibc for 29_day_harib26b"
)

add_custom_target(29_day_harib26b_sys
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/golibc -T ${29_day_harib26b_LDS} ${29_day_harib26b_CCS} ${29_day_harib26b_WILDOBJ} -o ${29_day_harib26b_BOOTB}
  COMMAND cat ${29_day_harib26b_HEADB} ${29_day_harib26b_BOOTB} > ${29_day_harib26b_SYS}
  DEPENDS ${29_day_harib26b_HEADB} 29_day_harib26b_ipl
  DEPENDS ${29_day_harib26b_FUNCO}
  DEPENDS ${29_day_harib26b_FONTO} ${29_day_harib26b_LIBGC}
)
add_custom_target(29_day_harib26b_img
  COMMAND mformat -f 1440 -l HARIBOTEOS -N 0xffffffff -C -B ${29_day_harib26b_IPLB} -i ${29_day_harib26b_OS}
  COMMAND mcopy -i ${29_day_harib26b_OS} ${29_day_harib26b_SYS} ::
  COMMAND mcopy -i ${29_day_harib26b_OS} ${root_BINARY_DIR}/projects/29_day/harib26b/a.hrb ::a.hrb
  COMMAND mcopy -i ${29_day_harib26b_OS} ${root_BINARY_DIR}/projects/29_day/harib26b/hello3.hrb ::hello3.hrb
  COMMAND mcopy -i ${29_day_harib26b_OS} ${root_BINARY_DIR}/projects/29_day/harib26b/hello4.hrb ::hello4.hrb
  COMMAND mcopy -i ${29_day_harib26b_OS} ${root_BINARY_DIR}/projects/29_day/harib26b/winhelo.hrb ::winhelo.hrb
  COMMAND mcopy -i ${29_day_harib26b_OS} ${root_BINARY_DIR}/projects/29_day/harib26b/winhelo2.hrb ::winhelo2.hrb
  COMMAND mcopy -i ${29_day_harib26b_OS} ${root_BINARY_DIR}/projects/29_day/harib26b/winhelo3.hrb ::winhelo3.hrb
  COMMAND mcopy -i ${29_day_harib26b_OS} ${root_BINARY_DIR}/projects/29_day/harib26b/star1.hrb ::star1.hrb
  COMMAND mcopy -i ${29_day_harib26b_OS} ${root_BINARY_DIR}/projects/29_day/harib26b/stars.hrb ::stars.hrb
  COMMAND mcopy -i ${29_day_harib26b_OS} ${root_BINARY_DIR}/projects/29_day/harib26b/stars2.hrb ::stars2.hrb
  COMMAND mcopy -i ${29_day_harib26b_OS} ${root_BINARY_DIR}/projects/29_day/harib26b/lines.hrb ::lines.hrb
  COMMAND mcopy -i ${29_day_harib26b_OS} ${root_BINARY_DIR}/projects/29_day/harib26b/walk.hrb ::walk.hrb
  COMMAND mcopy -i ${29_day_harib26b_OS} ${root_BINARY_DIR}/projects/29_day/harib26b/chklang.hrb ::chklang.hrb
  COMMAND mcopy -i ${29_day_harib26b_OS} ${root_BINARY_DIR}/projects/29_day/harib26b/iroha.hrb ::iroha.hrb
  COMMAND mcopy -i ${29_day_harib26b_OS} ${root_BINARY_DIR}/projects/29_day/harib26b/type.hrb ::type.hrb
  COMMAND mcopy -i ${29_day_harib26b_OS} ${root_BINARY_DIR}/projects/29_day/harib26b/typeipl.hrb ::typeipl.hrb
  COMMAND mcopy -i ${29_day_harib26b_OS} ${root_BINARY_DIR}/projects/29_day/harib26b/sosu3.hrb ::sosu3.hrb
  COMMAND mcopy -i ${29_day_harib26b_OS} ${root_BINARY_DIR}/projects/29_day/harib26b/sosu2.hrb ::sosu2.hrb
  COMMAND mcopy -i ${29_day_harib26b_OS} ${root_BINARY_DIR}/projects/29_day/harib26b/sosu.hrb ::sosu.hrb
  COMMAND mcopy -i ${29_day_harib26b_OS} ${root_BINARY_DIR}/projects/29_day/harib26b/color2.hrb ::color2.hrb
  COMMAND mcopy -i ${29_day_harib26b_OS} ${root_BINARY_DIR}/projects/29_day/harib26b/color.hrb ::color.hrb
  COMMAND mcopy -i ${29_day_harib26b_OS} ${root_BINARY_DIR}/projects/29_day/harib26b/beepdown.hrb ::beepdown.hrb
  COMMAND mcopy -i ${29_day_harib26b_OS} ${root_BINARY_DIR}/projects/29_day/harib26b/noodle.hrb ::noodle.hrb
  COMMAND mcopy -i ${29_day_harib26b_OS} ${root_BINARY_DIR}/projects/29_day/harib26b/hello5.hrb ::hello5.hrb
  COMMAND mcopy -i ${29_day_harib26b_OS} ${root_SOURCE_DIR}/projects/29_day/harib26b/euc.txt ::euc.txt
  COMMAND mcopy -i ${29_day_harib26b_OS} ${root_SOURCE_DIR}/projects/29_day/harib26b/nihongo/nihongo.fnt ::nihongo.fnt
  DEPENDS 29_day_harib26b_sys ${root_BINARY_DIR}/projects/29_day/harib26b/a.hrb ${root_BINARY_DIR}/projects/29_day/harib26b/hello3.hrb ${root_BINARY_DIR}/projects/29_day/harib26b/hello4.hrb ${root_BINARY_DIR}/projects/29_day/harib26b/winhelo.hrb ${root_BINARY_DIR}/projects/29_day/harib26b/winhelo2.hrb ${root_BINARY_DIR}/projects/29_day/harib26b/winhelo3.hrb ${root_BINARY_DIR}/projects/29_day/harib26b/star1.hrb ${root_BINARY_DIR}/projects/29_day/harib26b/stars.hrb ${root_BINARY_DIR}/projects/29_day/harib26b/stars2.hrb ${root_BINARY_DIR}/projects/29_day/harib26b/lines.hrb ${root_BINARY_DIR}/projects/29_day/harib26b/walk.hrb ${root_BINARY_DIR}/projects/29_day/harib26b/chklang.hrb ${root_BINARY_DIR}/projects/29_day/harib26b/iroha.hrb ${root_BINARY_DIR}/projects/29_day/harib26b/type.hrb ${root_BINARY_DIR}/projects/29_day/harib26b/typeipl.hrb ${root_BINARY_DIR}/projects/29_day/harib26b/sosu3.hrb ${root_BINARY_DIR}/projects/29_day/harib26b/sosu2.hrb ${root_BINARY_DIR}/projects/29_day/harib26b/sosu.hrb ${root_BINARY_DIR}/projects/29_day/harib26b/noodle.hrb ${root_BINARY_DIR}/projects/29_day/harib26b/beepdown.hrb ${root_BINARY_DIR}/projects/29_day/harib26b/color.hrb ${root_BINARY_DIR}/projects/29_day/harib26b/color2.hrb ${root_BINARY_DIR}/projects/29_day/harib26b/hello5.hrb ${root_SOURCE_DIR}/projects/29_day/harib26b/nihongo/nihongo.fnt ${root_SOURCE_DIR}/projects/29_day/harib26b/euc.txt
)

#----------------------------------------------------------
########### application .hrb files (copied into os.img) ###############
set(29_day_harib26b_APIO
  ${root_BINARY_DIR}/projects/29_day/harib26b/api001.o
  ${root_BINARY_DIR}/projects/29_day/harib26b/api002.o
  ${root_BINARY_DIR}/projects/29_day/harib26b/api003.o
  ${root_BINARY_DIR}/projects/29_day/harib26b/api004.o
  ${root_BINARY_DIR}/projects/29_day/harib26b/api005.o
  ${root_BINARY_DIR}/projects/29_day/harib26b/api006.o
  ${root_BINARY_DIR}/projects/29_day/harib26b/api007.o
  ${root_BINARY_DIR}/projects/29_day/harib26b/api008.o
  ${root_BINARY_DIR}/projects/29_day/harib26b/api009.o
  ${root_BINARY_DIR}/projects/29_day/harib26b/api010.o
  ${root_BINARY_DIR}/projects/29_day/harib26b/api011.o
  ${root_BINARY_DIR}/projects/29_day/harib26b/api012.o
  ${root_BINARY_DIR}/projects/29_day/harib26b/api013.o
  ${root_BINARY_DIR}/projects/29_day/harib26b/api014.o
  ${root_BINARY_DIR}/projects/29_day/harib26b/api015.o
  ${root_BINARY_DIR}/projects/29_day/harib26b/api016.o
  ${root_BINARY_DIR}/projects/29_day/harib26b/api017.o
  ${root_BINARY_DIR}/projects/29_day/harib26b/api018.o
  ${root_BINARY_DIR}/projects/29_day/harib26b/api019.o
  ${root_BINARY_DIR}/projects/29_day/harib26b/api020.o
  ${root_BINARY_DIR}/projects/29_day/harib26b/api021.o
  ${root_BINARY_DIR}/projects/29_day/harib26b/api022.o
  ${root_BINARY_DIR}/projects/29_day/harib26b/api023.o
  ${root_BINARY_DIR}/projects/29_day/harib26b/api024.o
  ${root_BINARY_DIR}/projects/29_day/harib26b/api025.o
  ${root_BINARY_DIR}/projects/29_day/harib26b/api026.o
  ${root_BINARY_DIR}/projects/29_day/harib26b/api027.o)
set(29_day_harib26b_RANDO ${root_BINARY_DIR}/projects/29_day/harib26b/rand.o)
add_custom_command(
  OUTPUT ${29_day_harib26b_RANDO}
  COMMAND gcc -fleading-underscore -m32 -nostdlib -fno-pie -x c -c ${root_SOURCE_DIR}/golibc/rand.c_ -o ${29_day_harib26b_RANDO}
  DEPENDS ${root_SOURCE_DIR}/golibc/rand.c_
  COMMENT "Generating rand.o for 29_day_harib26b"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/29_day/harib26b/api001.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/29_day/harib26b/apilib/api001.nas ${root_BINARY_DIR}/projects/29_day/harib26b/api001.o
  DEPENDS ${root_SOURCE_DIR}/projects/29_day/harib26b/apilib/api001.nas
  COMMENT "Generating api001.o for 29_day_harib26b"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/29_day/harib26b/api002.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/29_day/harib26b/apilib/api002.nas ${root_BINARY_DIR}/projects/29_day/harib26b/api002.o
  DEPENDS ${root_SOURCE_DIR}/projects/29_day/harib26b/apilib/api002.nas
  COMMENT "Generating api002.o for 29_day_harib26b"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/29_day/harib26b/api003.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/29_day/harib26b/apilib/api003.nas ${root_BINARY_DIR}/projects/29_day/harib26b/api003.o
  DEPENDS ${root_SOURCE_DIR}/projects/29_day/harib26b/apilib/api003.nas
  COMMENT "Generating api003.o for 29_day_harib26b"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/29_day/harib26b/api004.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/29_day/harib26b/apilib/api004.nas ${root_BINARY_DIR}/projects/29_day/harib26b/api004.o
  DEPENDS ${root_SOURCE_DIR}/projects/29_day/harib26b/apilib/api004.nas
  COMMENT "Generating api004.o for 29_day_harib26b"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/29_day/harib26b/api005.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/29_day/harib26b/apilib/api005.nas ${root_BINARY_DIR}/projects/29_day/harib26b/api005.o
  DEPENDS ${root_SOURCE_DIR}/projects/29_day/harib26b/apilib/api005.nas
  COMMENT "Generating api005.o for 29_day_harib26b"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/29_day/harib26b/api006.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/29_day/harib26b/apilib/api006.nas ${root_BINARY_DIR}/projects/29_day/harib26b/api006.o
  DEPENDS ${root_SOURCE_DIR}/projects/29_day/harib26b/apilib/api006.nas
  COMMENT "Generating api006.o for 29_day_harib26b"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/29_day/harib26b/api007.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/29_day/harib26b/apilib/api007.nas ${root_BINARY_DIR}/projects/29_day/harib26b/api007.o
  DEPENDS ${root_SOURCE_DIR}/projects/29_day/harib26b/apilib/api007.nas
  COMMENT "Generating api007.o for 29_day_harib26b"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/29_day/harib26b/api008.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/29_day/harib26b/apilib/api008.nas ${root_BINARY_DIR}/projects/29_day/harib26b/api008.o
  DEPENDS ${root_SOURCE_DIR}/projects/29_day/harib26b/apilib/api008.nas
  COMMENT "Generating api008.o for 29_day_harib26b"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/29_day/harib26b/api009.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/29_day/harib26b/apilib/api009.nas ${root_BINARY_DIR}/projects/29_day/harib26b/api009.o
  DEPENDS ${root_SOURCE_DIR}/projects/29_day/harib26b/apilib/api009.nas
  COMMENT "Generating api009.o for 29_day_harib26b"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/29_day/harib26b/api010.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/29_day/harib26b/apilib/api010.nas ${root_BINARY_DIR}/projects/29_day/harib26b/api010.o
  DEPENDS ${root_SOURCE_DIR}/projects/29_day/harib26b/apilib/api010.nas
  COMMENT "Generating api010.o for 29_day_harib26b"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/29_day/harib26b/api011.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/29_day/harib26b/apilib/api011.nas ${root_BINARY_DIR}/projects/29_day/harib26b/api011.o
  DEPENDS ${root_SOURCE_DIR}/projects/29_day/harib26b/apilib/api011.nas
  COMMENT "Generating api011.o for 29_day_harib26b"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/29_day/harib26b/api012.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/29_day/harib26b/apilib/api012.nas ${root_BINARY_DIR}/projects/29_day/harib26b/api012.o
  DEPENDS ${root_SOURCE_DIR}/projects/29_day/harib26b/apilib/api012.nas
  COMMENT "Generating api012.o for 29_day_harib26b"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/29_day/harib26b/api013.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/29_day/harib26b/apilib/api013.nas ${root_BINARY_DIR}/projects/29_day/harib26b/api013.o
  DEPENDS ${root_SOURCE_DIR}/projects/29_day/harib26b/apilib/api013.nas
  COMMENT "Generating api013.o for 29_day_harib26b"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/29_day/harib26b/api014.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/29_day/harib26b/apilib/api014.nas ${root_BINARY_DIR}/projects/29_day/harib26b/api014.o
  DEPENDS ${root_SOURCE_DIR}/projects/29_day/harib26b/apilib/api014.nas
  COMMENT "Generating api014.o for 29_day_harib26b"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/29_day/harib26b/api015.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/29_day/harib26b/apilib/api015.nas ${root_BINARY_DIR}/projects/29_day/harib26b/api015.o
  DEPENDS ${root_SOURCE_DIR}/projects/29_day/harib26b/apilib/api015.nas
  COMMENT "Generating api015.o for 29_day_harib26b"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/29_day/harib26b/api016.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/29_day/harib26b/apilib/api016.nas ${root_BINARY_DIR}/projects/29_day/harib26b/api016.o
  DEPENDS ${root_SOURCE_DIR}/projects/29_day/harib26b/apilib/api016.nas
  COMMENT "Generating api016.o for 29_day_harib26b"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/29_day/harib26b/api017.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/29_day/harib26b/apilib/api017.nas ${root_BINARY_DIR}/projects/29_day/harib26b/api017.o
  DEPENDS ${root_SOURCE_DIR}/projects/29_day/harib26b/apilib/api017.nas
  COMMENT "Generating api017.o for 29_day_harib26b"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/29_day/harib26b/api018.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/29_day/harib26b/apilib/api018.nas ${root_BINARY_DIR}/projects/29_day/harib26b/api018.o
  DEPENDS ${root_SOURCE_DIR}/projects/29_day/harib26b/apilib/api018.nas
  COMMENT "Generating api018.o for 29_day_harib26b"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/29_day/harib26b/api019.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/29_day/harib26b/apilib/api019.nas ${root_BINARY_DIR}/projects/29_day/harib26b/api019.o
  DEPENDS ${root_SOURCE_DIR}/projects/29_day/harib26b/apilib/api019.nas
  COMMENT "Generating api019.o for 29_day_harib26b"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/29_day/harib26b/api020.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/29_day/harib26b/apilib/api020.nas ${root_BINARY_DIR}/projects/29_day/harib26b/api020.o
  DEPENDS ${root_SOURCE_DIR}/projects/29_day/harib26b/apilib/api020.nas
  COMMENT "Generating api020.o for 29_day_harib26b"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/29_day/harib26b/api021.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/29_day/harib26b/apilib/api021.nas ${root_BINARY_DIR}/projects/29_day/harib26b/api021.o
  DEPENDS ${root_SOURCE_DIR}/projects/29_day/harib26b/apilib/api021.nas
  COMMENT "Generating api021.o for 29_day_harib26b"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/29_day/harib26b/api022.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/29_day/harib26b/apilib/api022.nas ${root_BINARY_DIR}/projects/29_day/harib26b/api022.o
  DEPENDS ${root_SOURCE_DIR}/projects/29_day/harib26b/apilib/api022.nas
  COMMENT "Generating api022.o for 29_day_harib26b"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/29_day/harib26b/api023.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/29_day/harib26b/apilib/api023.nas ${root_BINARY_DIR}/projects/29_day/harib26b/api023.o
  DEPENDS ${root_SOURCE_DIR}/projects/29_day/harib26b/apilib/api023.nas
  COMMENT "Generating api023.o for 29_day_harib26b"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/29_day/harib26b/api024.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/29_day/harib26b/apilib/api024.nas ${root_BINARY_DIR}/projects/29_day/harib26b/api024.o
  DEPENDS ${root_SOURCE_DIR}/projects/29_day/harib26b/apilib/api024.nas
  COMMENT "Generating api024.o for 29_day_harib26b"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/29_day/harib26b/api025.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/29_day/harib26b/apilib/api025.nas ${root_BINARY_DIR}/projects/29_day/harib26b/api025.o
  DEPENDS ${root_SOURCE_DIR}/projects/29_day/harib26b/apilib/api025.nas
  COMMENT "Generating api025.o for 29_day_harib26b"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/29_day/harib26b/api026.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/29_day/harib26b/apilib/api026.nas ${root_BINARY_DIR}/projects/29_day/harib26b/api026.o
  DEPENDS ${root_SOURCE_DIR}/projects/29_day/harib26b/apilib/api026.nas
  COMMENT "Generating api026.o for 29_day_harib26b"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/29_day/harib26b/api027.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/29_day/harib26b/apilib/api027.nas ${root_BINARY_DIR}/projects/29_day/harib26b/api027.o
  DEPENDS ${root_SOURCE_DIR}/projects/29_day/harib26b/apilib/api027.nas
  COMMENT "Generating api027.o for 29_day_harib26b"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/29_day/harib26b/a.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/golibc -I ${root_SOURCE_DIR}/projects/29_day/harib26b -T ${29_day_harib26b_APPLDS} ${root_SOURCE_DIR}/projects/29_day/harib26b/a/a.c ${29_day_harib26b_APIO} ${29_day_harib26b_LIBGC} ${29_day_harib26b_RANDO} -o ${root_BINARY_DIR}/projects/29_day/harib26b/a.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/29_day/harib26b/a/a.c ${root_SOURCE_DIR}/projects/app.lds ${29_day_harib26b_APIO} ${29_day_harib26b_LIBGC} ${29_day_harib26b_RANDO}
  COMMENT "Generating a.hrb for 29_day_harib26b"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/29_day/harib26b/hello3.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/golibc -I ${root_SOURCE_DIR}/projects/29_day/harib26b -T ${29_day_harib26b_APPLDS} ${root_SOURCE_DIR}/projects/29_day/harib26b/hello3/hello3.c ${29_day_harib26b_APIO} ${29_day_harib26b_LIBGC} ${29_day_harib26b_RANDO} -o ${root_BINARY_DIR}/projects/29_day/harib26b/hello3.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/29_day/harib26b/hello3/hello3.c ${root_SOURCE_DIR}/projects/app.lds ${29_day_harib26b_APIO} ${29_day_harib26b_LIBGC} ${29_day_harib26b_RANDO}
  COMMENT "Generating hello3.hrb for 29_day_harib26b"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/29_day/harib26b/hello4.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/golibc -I ${root_SOURCE_DIR}/projects/29_day/harib26b -T ${29_day_harib26b_APPLDS} ${root_SOURCE_DIR}/projects/29_day/harib26b/hello4/hello4.c ${29_day_harib26b_APIO} ${29_day_harib26b_LIBGC} ${29_day_harib26b_RANDO} -o ${root_BINARY_DIR}/projects/29_day/harib26b/hello4.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/29_day/harib26b/hello4/hello4.c ${root_SOURCE_DIR}/projects/app.lds ${29_day_harib26b_APIO} ${29_day_harib26b_LIBGC} ${29_day_harib26b_RANDO}
  COMMENT "Generating hello4.hrb for 29_day_harib26b"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/29_day/harib26b/winhelo.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/golibc -I ${root_SOURCE_DIR}/projects/29_day/harib26b -T ${29_day_harib26b_APPLDS} ${root_SOURCE_DIR}/projects/29_day/harib26b/winhelo/winhelo.c ${29_day_harib26b_APIO} ${29_day_harib26b_LIBGC} ${29_day_harib26b_RANDO} -o ${root_BINARY_DIR}/projects/29_day/harib26b/winhelo.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/29_day/harib26b/winhelo/winhelo.c ${root_SOURCE_DIR}/projects/app.lds ${29_day_harib26b_APIO} ${29_day_harib26b_LIBGC} ${29_day_harib26b_RANDO}
  COMMENT "Generating winhelo.hrb for 29_day_harib26b"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/29_day/harib26b/winhelo2.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/golibc -I ${root_SOURCE_DIR}/projects/29_day/harib26b -T ${29_day_harib26b_APPLDS} ${root_SOURCE_DIR}/projects/29_day/harib26b/winhelo2/winhelo2.c ${29_day_harib26b_APIO} ${29_day_harib26b_LIBGC} ${29_day_harib26b_RANDO} -o ${root_BINARY_DIR}/projects/29_day/harib26b/winhelo2.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/29_day/harib26b/winhelo2/winhelo2.c ${root_SOURCE_DIR}/projects/app.lds ${29_day_harib26b_APIO} ${29_day_harib26b_LIBGC} ${29_day_harib26b_RANDO}
  COMMENT "Generating winhelo2.hrb for 29_day_harib26b"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/29_day/harib26b/winhelo3.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/golibc -I ${root_SOURCE_DIR}/projects/29_day/harib26b -T ${29_day_harib26b_APPLDS} ${root_SOURCE_DIR}/projects/29_day/harib26b/winhelo3/winhelo3.c ${29_day_harib26b_APIO} ${29_day_harib26b_LIBGC} ${29_day_harib26b_RANDO} -o ${root_BINARY_DIR}/projects/29_day/harib26b/winhelo3.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/29_day/harib26b/winhelo3/winhelo3.c ${root_SOURCE_DIR}/projects/app.lds ${29_day_harib26b_APIO} ${29_day_harib26b_LIBGC} ${29_day_harib26b_RANDO}
  COMMENT "Generating winhelo3.hrb for 29_day_harib26b"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/29_day/harib26b/star1.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/golibc -I ${root_SOURCE_DIR}/projects/29_day/harib26b -T ${29_day_harib26b_APPLDS} ${root_SOURCE_DIR}/projects/29_day/harib26b/star1/star1.c ${29_day_harib26b_APIO} ${29_day_harib26b_LIBGC} ${29_day_harib26b_RANDO} -o ${root_BINARY_DIR}/projects/29_day/harib26b/star1.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/29_day/harib26b/star1/star1.c ${root_SOURCE_DIR}/projects/app.lds ${29_day_harib26b_APIO} ${29_day_harib26b_LIBGC} ${29_day_harib26b_RANDO}
  COMMENT "Generating star1.hrb for 29_day_harib26b"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/29_day/harib26b/stars.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/golibc -I ${root_SOURCE_DIR}/projects/29_day/harib26b -T ${29_day_harib26b_APPLDS} ${root_SOURCE_DIR}/projects/29_day/harib26b/stars/stars.c ${29_day_harib26b_APIO} ${29_day_harib26b_LIBGC} ${29_day_harib26b_RANDO} -o ${root_BINARY_DIR}/projects/29_day/harib26b/stars.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/29_day/harib26b/stars/stars.c ${root_SOURCE_DIR}/projects/app.lds ${29_day_harib26b_APIO} ${29_day_harib26b_LIBGC} ${29_day_harib26b_RANDO}
  COMMENT "Generating stars.hrb for 29_day_harib26b"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/29_day/harib26b/stars2.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/golibc -I ${root_SOURCE_DIR}/projects/29_day/harib26b -T ${29_day_harib26b_APPLDS} ${root_SOURCE_DIR}/projects/29_day/harib26b/stars2/stars2.c ${29_day_harib26b_APIO} ${29_day_harib26b_LIBGC} ${29_day_harib26b_RANDO} -o ${root_BINARY_DIR}/projects/29_day/harib26b/stars2.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/29_day/harib26b/stars2/stars2.c ${root_SOURCE_DIR}/projects/app.lds ${29_day_harib26b_APIO} ${29_day_harib26b_LIBGC} ${29_day_harib26b_RANDO}
  COMMENT "Generating stars2.hrb for 29_day_harib26b"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/29_day/harib26b/lines.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/golibc -I ${root_SOURCE_DIR}/projects/29_day/harib26b -T ${29_day_harib26b_APPLDS} ${root_SOURCE_DIR}/projects/29_day/harib26b/lines/lines.c ${29_day_harib26b_APIO} ${29_day_harib26b_LIBGC} ${29_day_harib26b_RANDO} -o ${root_BINARY_DIR}/projects/29_day/harib26b/lines.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/29_day/harib26b/lines/lines.c ${root_SOURCE_DIR}/projects/app.lds ${29_day_harib26b_APIO} ${29_day_harib26b_LIBGC} ${29_day_harib26b_RANDO}
  COMMENT "Generating lines.hrb for 29_day_harib26b"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/29_day/harib26b/walk.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/golibc -I ${root_SOURCE_DIR}/projects/29_day/harib26b -T ${29_day_harib26b_APPLDS} ${root_SOURCE_DIR}/projects/29_day/harib26b/walk/walk.c ${29_day_harib26b_APIO} ${29_day_harib26b_LIBGC} ${29_day_harib26b_RANDO} -o ${root_BINARY_DIR}/projects/29_day/harib26b/walk.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/29_day/harib26b/walk/walk.c ${root_SOURCE_DIR}/projects/app.lds ${29_day_harib26b_APIO} ${29_day_harib26b_LIBGC} ${29_day_harib26b_RANDO}
  COMMENT "Generating walk.hrb for 29_day_harib26b"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/29_day/harib26b/chklang.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/golibc -I ${root_SOURCE_DIR}/projects/29_day/harib26b -T ${29_day_harib26b_APPLDS} ${root_SOURCE_DIR}/projects/29_day/harib26b/chklang/chklang.c ${29_day_harib26b_APIO} ${29_day_harib26b_LIBGC} ${29_day_harib26b_RANDO} -o ${root_BINARY_DIR}/projects/29_day/harib26b/chklang.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/29_day/harib26b/chklang/chklang.c ${root_SOURCE_DIR}/projects/app.lds ${29_day_harib26b_APIO} ${29_day_harib26b_LIBGC} ${29_day_harib26b_RANDO}
  COMMENT "Generating chklang.hrb for 29_day_harib26b"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/29_day/harib26b/iroha.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/golibc -I ${root_SOURCE_DIR}/projects/29_day/harib26b -T ${29_day_harib26b_APPLDS} ${root_SOURCE_DIR}/projects/29_day/harib26b/iroha/iroha.c ${29_day_harib26b_APIO} ${29_day_harib26b_LIBGC} ${29_day_harib26b_RANDO} -o ${root_BINARY_DIR}/projects/29_day/harib26b/iroha.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/29_day/harib26b/iroha/iroha.c ${root_SOURCE_DIR}/projects/app.lds ${29_day_harib26b_APIO} ${29_day_harib26b_LIBGC} ${29_day_harib26b_RANDO}
  COMMENT "Generating iroha.hrb for 29_day_harib26b"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/29_day/harib26b/type.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/golibc -I ${root_SOURCE_DIR}/projects/29_day/harib26b -T ${29_day_harib26b_APPLDS} ${root_SOURCE_DIR}/projects/29_day/harib26b/type/type.c ${29_day_harib26b_APIO} ${29_day_harib26b_LIBGC} ${29_day_harib26b_RANDO} -o ${root_BINARY_DIR}/projects/29_day/harib26b/type.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/29_day/harib26b/type/type.c ${root_SOURCE_DIR}/projects/app.lds ${29_day_harib26b_APIO} ${29_day_harib26b_LIBGC} ${29_day_harib26b_RANDO}
  COMMENT "Generating type.hrb for 29_day_harib26b"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/29_day/harib26b/typeipl.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/golibc -I ${root_SOURCE_DIR}/projects/29_day/harib26b -T ${29_day_harib26b_APPLDS} ${root_SOURCE_DIR}/projects/29_day/harib26b/typeipl/typeipl.c ${29_day_harib26b_APIO} ${29_day_harib26b_LIBGC} ${29_day_harib26b_RANDO} -o ${root_BINARY_DIR}/projects/29_day/harib26b/typeipl.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/29_day/harib26b/typeipl/typeipl.c ${root_SOURCE_DIR}/projects/app.lds ${29_day_harib26b_APIO} ${29_day_harib26b_LIBGC} ${29_day_harib26b_RANDO}
  COMMENT "Generating typeipl.hrb for 29_day_harib26b"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/29_day/harib26b/sosu3.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/golibc -I ${root_SOURCE_DIR}/projects/29_day/harib26b -T ${29_day_harib26b_APPLDS} ${root_SOURCE_DIR}/projects/29_day/harib26b/sosu3/sosu3.c ${29_day_harib26b_APIO} ${29_day_harib26b_LIBGC} ${29_day_harib26b_RANDO} -o ${root_BINARY_DIR}/projects/29_day/harib26b/sosu3.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/29_day/harib26b/sosu3/sosu3.c ${root_SOURCE_DIR}/projects/app.lds ${29_day_harib26b_APIO} ${29_day_harib26b_LIBGC} ${29_day_harib26b_RANDO}
  COMMENT "Generating sosu3.hrb for 29_day_harib26b"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/29_day/harib26b/sosu2.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/golibc -I ${root_SOURCE_DIR}/projects/29_day/harib26b -T ${29_day_harib26b_APPLDS} ${root_SOURCE_DIR}/projects/29_day/harib26b/sosu2/sosu2.c ${29_day_harib26b_APIO} ${29_day_harib26b_LIBGC} ${29_day_harib26b_RANDO} -o ${root_BINARY_DIR}/projects/29_day/harib26b/sosu2.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/29_day/harib26b/sosu2/sosu2.c ${root_SOURCE_DIR}/projects/app.lds ${29_day_harib26b_APIO} ${29_day_harib26b_LIBGC} ${29_day_harib26b_RANDO}
  COMMENT "Generating sosu2.hrb for 29_day_harib26b"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/29_day/harib26b/sosu.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/golibc -I ${root_SOURCE_DIR}/projects/29_day/harib26b -T ${29_day_harib26b_APPLDS} ${root_SOURCE_DIR}/projects/29_day/harib26b/sosu/sosu.c ${29_day_harib26b_APIO} ${29_day_harib26b_LIBGC} ${29_day_harib26b_RANDO} -o ${root_BINARY_DIR}/projects/29_day/harib26b/sosu.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/29_day/harib26b/sosu/sosu.c ${root_SOURCE_DIR}/projects/app.lds ${29_day_harib26b_APIO} ${29_day_harib26b_LIBGC} ${29_day_harib26b_RANDO}
  COMMENT "Generating sosu.hrb for 29_day_harib26b"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/29_day/harib26b/color2.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/golibc -I ${root_SOURCE_DIR}/projects/29_day/harib26b -T ${29_day_harib26b_APPLDS} ${root_SOURCE_DIR}/projects/29_day/harib26b/color2/color2.c ${29_day_harib26b_APIO} ${29_day_harib26b_LIBGC} ${29_day_harib26b_RANDO} -o ${root_BINARY_DIR}/projects/29_day/harib26b/color2.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/29_day/harib26b/color2/color2.c ${root_SOURCE_DIR}/projects/app.lds ${29_day_harib26b_APIO} ${29_day_harib26b_LIBGC} ${29_day_harib26b_RANDO}
  COMMENT "Generating color2.hrb for 29_day_harib26b"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/29_day/harib26b/color.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/golibc -I ${root_SOURCE_DIR}/projects/29_day/harib26b -T ${29_day_harib26b_APPLDS} ${root_SOURCE_DIR}/projects/29_day/harib26b/color/color.c ${29_day_harib26b_APIO} ${29_day_harib26b_LIBGC} ${29_day_harib26b_RANDO} -o ${root_BINARY_DIR}/projects/29_day/harib26b/color.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/29_day/harib26b/color/color.c ${root_SOURCE_DIR}/projects/app.lds ${29_day_harib26b_APIO} ${29_day_harib26b_LIBGC} ${29_day_harib26b_RANDO}
  COMMENT "Generating color.hrb for 29_day_harib26b"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/29_day/harib26b/beepdown.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/golibc -I ${root_SOURCE_DIR}/projects/29_day/harib26b -T ${29_day_harib26b_APPLDS} ${root_SOURCE_DIR}/projects/29_day/harib26b/beepdown/beepdown.c ${29_day_harib26b_APIO} ${29_day_harib26b_LIBGC} ${29_day_harib26b_RANDO} -o ${root_BINARY_DIR}/projects/29_day/harib26b/beepdown.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/29_day/harib26b/beepdown/beepdown.c ${root_SOURCE_DIR}/projects/app.lds ${29_day_harib26b_APIO} ${29_day_harib26b_LIBGC} ${29_day_harib26b_RANDO}
  COMMENT "Generating beepdown.hrb for 29_day_harib26b"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/29_day/harib26b/noodle.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/golibc -I ${root_SOURCE_DIR}/projects/29_day/harib26b -T ${29_day_harib26b_APPLDS} ${root_SOURCE_DIR}/projects/29_day/harib26b/noodle/noodle.c ${29_day_harib26b_APIO} ${29_day_harib26b_LIBGC} ${29_day_harib26b_RANDO} -o ${root_BINARY_DIR}/projects/29_day/harib26b/noodle.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/29_day/harib26b/noodle/noodle.c ${root_SOURCE_DIR}/projects/app.lds ${29_day_harib26b_APIO} ${29_day_harib26b_LIBGC} ${29_day_harib26b_RANDO}
  COMMENT "Generating noodle.hrb for 29_day_harib26b"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/29_day/harib26b/hello5.hrb
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/29_day/harib26b/hello5/hello5.nas ${root_BINARY_DIR}/projects/29_day/harib26b/hello5.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/29_day/harib26b/hello5/hello5.nas
  COMMENT "Generating hello5.hrb for 29_day_harib26b"
)
