#----------------------------------------------------------
message(STATUS "Entering directory projects/27_day/harib24f/")

set(NASK ${root_BINARY_DIR}/src/opennask)
set(FONT ${root_BINARY_DIR}/src/makefont)
set(B2O  ${root_BINARY_DIR}/src/bin2obj)
set(CONV ${root_BINARY_DIR}/objconv/objconv)
set(27_day_harib24f_OS    ${root_BINARY_DIR}/projects/27_day/harib24f/os.img)
set(27_day_harib24f_SYS	  ${root_BINARY_DIR}/projects/27_day/harib24f/os.sys)
set(27_day_harib24f_IPLB  ${root_BINARY_DIR}/projects/27_day/harib24f/ipl.bin)
set(27_day_harib24f_IPLS  ${root_SOURCE_DIR}/projects/27_day/harib24f/ipl10.nas)
set(27_day_harib24f_HEADB ${root_BINARY_DIR}/projects/27_day/harib24f/asmhead.bin)
set(27_day_harib24f_HEADS ${root_SOURCE_DIR}/projects/27_day/harib24f/asmhead.nas)
set(27_day_harib24f_BOOTB ${root_BINARY_DIR}/projects/27_day/harib24f/boot.bin)
set(27_day_harib24f_CCO	  ${root_BINARY_DIR}/projects/27_day/harib24f/boot.o)
set(27_day_harib24f_CCS	  ${root_SOURCE_DIR}/projects/27_day/harib24f/*.c)
set(27_day_harib24f_LDS	  ${root_SOURCE_DIR}/projects/os.lds)
set(27_day_harib24f_FUNCS ${root_SOURCE_DIR}/projects/27_day/harib24f/naskfunc.nas)
set(27_day_harib24f_FUNCO ${root_BINARY_DIR}/projects/27_day/harib24f/naskfunc.o)
set(27_day_harib24f_FONTS ${root_SOURCE_DIR}/projects/27_day/harib24f/hankaku.txt)
set(27_day_harib24f_FONTB ${root_BINARY_DIR}/projects/27_day/harib24f/hankaku.bin)
set(27_day_harib24f_FONTO ${root_BINARY_DIR}/projects/27_day/harib24f/hankaku.o)
set(27_day_harib24f_LIBGE ${root_BINARY_DIR}/golibc/libgo.a)
set(27_day_harib24f_LIBGC ${root_BINARY_DIR}/projects/27_day/harib24f/libgo.o)
set(27_day_harib24f_WILDOBJ ${root_BINARY_DIR}/projects/27_day/harib24f/*.o)


add_custom_target(27_day_harib24f_api001_run
  COMMAND ${QEMU} ${QEMUOPT} ${27_day_harib24f_OS}
  DEPENDS 27_day_harib24f_img
)
add_custom_target(27_day_harib24f_api001_debug
  COMMAND ${QEMU} -s -S ${QEMUOPT} ${27_day_harib24f_OS} -redir tcp:5555:127.0.0.1:1234 &
  DEPENDS 27_day_harib24f_img
)
add_custom_target(27_day_harib24f_api001_clean
  COMMAND rm -f ${27_day_harib24f_OS}
  COMMAND rm -f ${27_day_harib24f_SYS}
  COMMAND rm -f ${27_day_harib24f_IPLB}
  COMMAND rm -f ${27_day_harib24f_HEADB}
  COMMAND rm -f ${27_day_harib24f_BOOTB}
  COMMAND rm -f ${27_day_harib24f_WILDOBJ}
)
add_custom_target(27_day_harib24f_api001_ipl
  COMMAND ${NASK} ${27_day_harib24f_IPLS} ${27_day_harib24f_IPLB}
)
add_custom_target(27_day_harib24f_api001_sys
  COMMAND ${NASK} ${27_day_harib24f_HEADS} ${27_day_harib24f_HEADB}
  COMMAND ${FONT} ${27_day_harib24f_FONTS} ${27_day_harib24f_FONTB}
  COMMAND ${B2O}  ${27_day_harib24f_FONTB} ${27_day_harib24f_FONTO} _hankaku
  COMMAND ${CONV} -fcoff32 -nu ${27_day_harib24f_LIBGE} ${27_day_harib24f_LIBGC}
  COMMAND ${NASK} ${27_day_harib24f_FUNCS} ${27_day_harib24f_FUNCO}
  COMMAND gcc ${BINOPT} -T ${27_day_harib24f_LDS} ${27_day_harib24f_CCS} ${27_day_harib24f_WILDOBJ} -o ${27_day_harib24f_BOOTB}
  COMMAND cat ${27_day_harib24f_HEADB} ${27_day_harib24f_BOOTB} > ${27_day_harib24f_SYS}
  DEPENDS 27_day_harib24f_ipl
)
add_custom_target(27_day_harib24f_api001_img
  COMMAND mformat -f 1440 -l HARIBOTEOS -N 0xffffffff -C -B ${27_day_harib24f_IPLB} -i ${27_day_harib24f_OS}
  COMMAND mcopy -i ${27_day_harib24f_OS} ${27_day_harib24f_SYS} ::
  DEPENDS 27_day_harib24f_sys
)
########### next target ###############
set(api001.img_SRCS ${root_SOURCE_DIR}/projects/27_day/harib24f/api001.nas)
set(api001.img_OUTS ${root_BINARY_DIR}/projects/27_day/harib24f/api001.img)
set(api001_wine.img_OUTS ${root_BINARY_DIR}/projects/27_day/harib24f/api001_wine.img)

add_custom_target(27_day_harib24f_api001
  COMMAND ${root_BINARY_DIR}/src/opennask ${api001.img_SRCS} ${api001.img_OUTS}
)
set(api001.img_HRB ${root_BINARY_DIR}/projects/27_day/harib24f/api001.hrb)
add_custom_target(27_day_harib24f_api001_hrb
  COMMAND ${root_BINARY_DIR}/src/opennask ${api001.img_SRCS} ${api001.img_HRB}
  COMMAND mcopy -i ${27_day_harib24f_OS} ${api001.img_HRB} ::api001.hrb
  DEPENDS 27_day_harib24f_img
)

add_dependencies(27_day_harib24f_run 27_day_harib24f_api001_hrb)
add_custom_target(27_day_harib24f_api001_wine
  COMMAND ${WINE} ${WINE_NASK} ${api001.img_SRCS} ${api001_wine.img_OUTS}
)
add_custom_target(27_day_harib24f_api001_od
  COMMAND ${OD} -t x1 ${api001.img_OUTS}      > ${api001.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${api001_wine.img_OUTS} > ${api001_wine.img_OUTS}_t.txt
  COMMAND diff -s ${api001.img_OUTS}_f.txt ${api001_wine.img_OUTS}_t.txt
)

add_dependencies(27_day_harib24f_api001_od 27_day_harib24f_api001)
add_dependencies(27_day_harib24f_api001_od 27_day_harib24f_api001_wine)

add_dependencies(images 27_day_harib24f_api001)
add_dependencies(wine 27_day_harib24f_api001_wine)
add_dependencies(od 27_day_harib24f_api001_od)

#----------------------------------------------------------
########### next target ###############
set(api002.img_SRCS ${root_SOURCE_DIR}/projects/27_day/harib24f/api002.nas)
set(api002.img_OUTS ${root_BINARY_DIR}/projects/27_day/harib24f/api002.img)
set(api002_wine.img_OUTS ${root_BINARY_DIR}/projects/27_day/harib24f/api002_wine.img)

add_custom_target(27_day_harib24f_api002
  COMMAND ${root_BINARY_DIR}/src/opennask ${api002.img_SRCS} ${api002.img_OUTS}
)
set(api002.img_HRB ${root_BINARY_DIR}/projects/27_day/harib24f/api002.hrb)
add_custom_target(27_day_harib24f_api002_hrb
  COMMAND ${root_BINARY_DIR}/src/opennask ${api002.img_SRCS} ${api002.img_HRB}
  COMMAND mcopy -i ${27_day_harib24f_OS} ${api002.img_HRB} ::api002.hrb
  DEPENDS 27_day_harib24f_img
)

add_dependencies(27_day_harib24f_run 27_day_harib24f_api002_hrb)
add_custom_target(27_day_harib24f_api002_wine
  COMMAND ${WINE} ${WINE_NASK} ${api002.img_SRCS} ${api002_wine.img_OUTS}
)
add_custom_target(27_day_harib24f_api002_od
  COMMAND ${OD} -t x1 ${api002.img_OUTS}      > ${api002.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${api002_wine.img_OUTS} > ${api002_wine.img_OUTS}_t.txt
  COMMAND diff -s ${api002.img_OUTS}_f.txt ${api002_wine.img_OUTS}_t.txt
)

add_dependencies(27_day_harib24f_api002_od 27_day_harib24f_api002)
add_dependencies(27_day_harib24f_api002_od 27_day_harib24f_api002_wine)

add_dependencies(images 27_day_harib24f_api002)
add_dependencies(wine 27_day_harib24f_api002_wine)
add_dependencies(od 27_day_harib24f_api002_od)

#----------------------------------------------------------
########### next target ###############
set(api003.img_SRCS ${root_SOURCE_DIR}/projects/27_day/harib24f/api003.nas)
set(api003.img_OUTS ${root_BINARY_DIR}/projects/27_day/harib24f/api003.img)
set(api003_wine.img_OUTS ${root_BINARY_DIR}/projects/27_day/harib24f/api003_wine.img)

add_custom_target(27_day_harib24f_api003
  COMMAND ${root_BINARY_DIR}/src/opennask ${api003.img_SRCS} ${api003.img_OUTS}
)
set(api003.img_HRB ${root_BINARY_DIR}/projects/27_day/harib24f/api003.hrb)
add_custom_target(27_day_harib24f_api003_hrb
  COMMAND ${root_BINARY_DIR}/src/opennask ${api003.img_SRCS} ${api003.img_HRB}
  COMMAND mcopy -i ${27_day_harib24f_OS} ${api003.img_HRB} ::api003.hrb
  DEPENDS 27_day_harib24f_img
)

add_dependencies(27_day_harib24f_run 27_day_harib24f_api003_hrb)
add_custom_target(27_day_harib24f_api003_wine
  COMMAND ${WINE} ${WINE_NASK} ${api003.img_SRCS} ${api003_wine.img_OUTS}
)
add_custom_target(27_day_harib24f_api003_od
  COMMAND ${OD} -t x1 ${api003.img_OUTS}      > ${api003.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${api003_wine.img_OUTS} > ${api003_wine.img_OUTS}_t.txt
  COMMAND diff -s ${api003.img_OUTS}_f.txt ${api003_wine.img_OUTS}_t.txt
)

add_dependencies(27_day_harib24f_api003_od 27_day_harib24f_api003)
add_dependencies(27_day_harib24f_api003_od 27_day_harib24f_api003_wine)

add_dependencies(images 27_day_harib24f_api003)
add_dependencies(wine 27_day_harib24f_api003_wine)
add_dependencies(od 27_day_harib24f_api003_od)

#----------------------------------------------------------
########### next target ###############
set(api004.img_SRCS ${root_SOURCE_DIR}/projects/27_day/harib24f/api004.nas)
set(api004.img_OUTS ${root_BINARY_DIR}/projects/27_day/harib24f/api004.img)
set(api004_wine.img_OUTS ${root_BINARY_DIR}/projects/27_day/harib24f/api004_wine.img)

add_custom_target(27_day_harib24f_api004
  COMMAND ${root_BINARY_DIR}/src/opennask ${api004.img_SRCS} ${api004.img_OUTS}
)
set(api004.img_HRB ${root_BINARY_DIR}/projects/27_day/harib24f/api004.hrb)
add_custom_target(27_day_harib24f_api004_hrb
  COMMAND ${root_BINARY_DIR}/src/opennask ${api004.img_SRCS} ${api004.img_HRB}
  COMMAND mcopy -i ${27_day_harib24f_OS} ${api004.img_HRB} ::api004.hrb
  DEPENDS 27_day_harib24f_img
)

add_dependencies(27_day_harib24f_run 27_day_harib24f_api004_hrb)
add_custom_target(27_day_harib24f_api004_wine
  COMMAND ${WINE} ${WINE_NASK} ${api004.img_SRCS} ${api004_wine.img_OUTS}
)
add_custom_target(27_day_harib24f_api004_od
  COMMAND ${OD} -t x1 ${api004.img_OUTS}      > ${api004.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${api004_wine.img_OUTS} > ${api004_wine.img_OUTS}_t.txt
  COMMAND diff -s ${api004.img_OUTS}_f.txt ${api004_wine.img_OUTS}_t.txt
)

add_dependencies(27_day_harib24f_api004_od 27_day_harib24f_api004)
add_dependencies(27_day_harib24f_api004_od 27_day_harib24f_api004_wine)

add_dependencies(images 27_day_harib24f_api004)
add_dependencies(wine 27_day_harib24f_api004_wine)
add_dependencies(od 27_day_harib24f_api004_od)

#----------------------------------------------------------
########### next target ###############
set(api005.img_SRCS ${root_SOURCE_DIR}/projects/27_day/harib24f/api005.nas)
set(api005.img_OUTS ${root_BINARY_DIR}/projects/27_day/harib24f/api005.img)
set(api005_wine.img_OUTS ${root_BINARY_DIR}/projects/27_day/harib24f/api005_wine.img)

add_custom_target(27_day_harib24f_api005
  COMMAND ${root_BINARY_DIR}/src/opennask ${api005.img_SRCS} ${api005.img_OUTS}
)
set(api005.img_HRB ${root_BINARY_DIR}/projects/27_day/harib24f/api005.hrb)
add_custom_target(27_day_harib24f_api005_hrb
  COMMAND ${root_BINARY_DIR}/src/opennask ${api005.img_SRCS} ${api005.img_HRB}
  COMMAND mcopy -i ${27_day_harib24f_OS} ${api005.img_HRB} ::api005.hrb
  DEPENDS 27_day_harib24f_img
)

add_dependencies(27_day_harib24f_run 27_day_harib24f_api005_hrb)
add_custom_target(27_day_harib24f_api005_wine
  COMMAND ${WINE} ${WINE_NASK} ${api005.img_SRCS} ${api005_wine.img_OUTS}
)
add_custom_target(27_day_harib24f_api005_od
  COMMAND ${OD} -t x1 ${api005.img_OUTS}      > ${api005.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${api005_wine.img_OUTS} > ${api005_wine.img_OUTS}_t.txt
  COMMAND diff -s ${api005.img_OUTS}_f.txt ${api005_wine.img_OUTS}_t.txt
)

add_dependencies(27_day_harib24f_api005_od 27_day_harib24f_api005)
add_dependencies(27_day_harib24f_api005_od 27_day_harib24f_api005_wine)

add_dependencies(images 27_day_harib24f_api005)
add_dependencies(wine 27_day_harib24f_api005_wine)
add_dependencies(od 27_day_harib24f_api005_od)

#----------------------------------------------------------
########### next target ###############
set(api006.img_SRCS ${root_SOURCE_DIR}/projects/27_day/harib24f/api006.nas)
set(api006.img_OUTS ${root_BINARY_DIR}/projects/27_day/harib24f/api006.img)
set(api006_wine.img_OUTS ${root_BINARY_DIR}/projects/27_day/harib24f/api006_wine.img)

add_custom_target(27_day_harib24f_api006
  COMMAND ${root_BINARY_DIR}/src/opennask ${api006.img_SRCS} ${api006.img_OUTS}
)
set(api006.img_HRB ${root_BINARY_DIR}/projects/27_day/harib24f/api006.hrb)
add_custom_target(27_day_harib24f_api006_hrb
  COMMAND ${root_BINARY_DIR}/src/opennask ${api006.img_SRCS} ${api006.img_HRB}
  COMMAND mcopy -i ${27_day_harib24f_OS} ${api006.img_HRB} ::api006.hrb
  DEPENDS 27_day_harib24f_img
)

add_dependencies(27_day_harib24f_run 27_day_harib24f_api006_hrb)
add_custom_target(27_day_harib24f_api006_wine
  COMMAND ${WINE} ${WINE_NASK} ${api006.img_SRCS} ${api006_wine.img_OUTS}
)
add_custom_target(27_day_harib24f_api006_od
  COMMAND ${OD} -t x1 ${api006.img_OUTS}      > ${api006.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${api006_wine.img_OUTS} > ${api006_wine.img_OUTS}_t.txt
  COMMAND diff -s ${api006.img_OUTS}_f.txt ${api006_wine.img_OUTS}_t.txt
)

add_dependencies(27_day_harib24f_api006_od 27_day_harib24f_api006)
add_dependencies(27_day_harib24f_api006_od 27_day_harib24f_api006_wine)

add_dependencies(images 27_day_harib24f_api006)
add_dependencies(wine 27_day_harib24f_api006_wine)
add_dependencies(od 27_day_harib24f_api006_od)

#----------------------------------------------------------
########### next target ###############
set(api007.img_SRCS ${root_SOURCE_DIR}/projects/27_day/harib24f/api007.nas)
set(api007.img_OUTS ${root_BINARY_DIR}/projects/27_day/harib24f/api007.img)
set(api007_wine.img_OUTS ${root_BINARY_DIR}/projects/27_day/harib24f/api007_wine.img)

add_custom_target(27_day_harib24f_api007
  COMMAND ${root_BINARY_DIR}/src/opennask ${api007.img_SRCS} ${api007.img_OUTS}
)
set(api007.img_HRB ${root_BINARY_DIR}/projects/27_day/harib24f/api007.hrb)
add_custom_target(27_day_harib24f_api007_hrb
  COMMAND ${root_BINARY_DIR}/src/opennask ${api007.img_SRCS} ${api007.img_HRB}
  COMMAND mcopy -i ${27_day_harib24f_OS} ${api007.img_HRB} ::api007.hrb
  DEPENDS 27_day_harib24f_img
)

add_dependencies(27_day_harib24f_run 27_day_harib24f_api007_hrb)
add_custom_target(27_day_harib24f_api007_wine
  COMMAND ${WINE} ${WINE_NASK} ${api007.img_SRCS} ${api007_wine.img_OUTS}
)
add_custom_target(27_day_harib24f_api007_od
  COMMAND ${OD} -t x1 ${api007.img_OUTS}      > ${api007.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${api007_wine.img_OUTS} > ${api007_wine.img_OUTS}_t.txt
  COMMAND diff -s ${api007.img_OUTS}_f.txt ${api007_wine.img_OUTS}_t.txt
)

add_dependencies(27_day_harib24f_api007_od 27_day_harib24f_api007)
add_dependencies(27_day_harib24f_api007_od 27_day_harib24f_api007_wine)

add_dependencies(images 27_day_harib24f_api007)
add_dependencies(wine 27_day_harib24f_api007_wine)
add_dependencies(od 27_day_harib24f_api007_od)

#----------------------------------------------------------
########### next target ###############
set(api008.img_SRCS ${root_SOURCE_DIR}/projects/27_day/harib24f/api008.nas)
set(api008.img_OUTS ${root_BINARY_DIR}/projects/27_day/harib24f/api008.img)
set(api008_wine.img_OUTS ${root_BINARY_DIR}/projects/27_day/harib24f/api008_wine.img)

add_custom_target(27_day_harib24f_api008
  COMMAND ${root_BINARY_DIR}/src/opennask ${api008.img_SRCS} ${api008.img_OUTS}
)
set(api008.img_HRB ${root_BINARY_DIR}/projects/27_day/harib24f/api008.hrb)
add_custom_target(27_day_harib24f_api008_hrb
  COMMAND ${root_BINARY_DIR}/src/opennask ${api008.img_SRCS} ${api008.img_HRB}
  COMMAND mcopy -i ${27_day_harib24f_OS} ${api008.img_HRB} ::api008.hrb
  DEPENDS 27_day_harib24f_img
)

add_dependencies(27_day_harib24f_run 27_day_harib24f_api008_hrb)
add_custom_target(27_day_harib24f_api008_wine
  COMMAND ${WINE} ${WINE_NASK} ${api008.img_SRCS} ${api008_wine.img_OUTS}
)
add_custom_target(27_day_harib24f_api008_od
  COMMAND ${OD} -t x1 ${api008.img_OUTS}      > ${api008.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${api008_wine.img_OUTS} > ${api008_wine.img_OUTS}_t.txt
  COMMAND diff -s ${api008.img_OUTS}_f.txt ${api008_wine.img_OUTS}_t.txt
)

add_dependencies(27_day_harib24f_api008_od 27_day_harib24f_api008)
add_dependencies(27_day_harib24f_api008_od 27_day_harib24f_api008_wine)

add_dependencies(images 27_day_harib24f_api008)
add_dependencies(wine 27_day_harib24f_api008_wine)
add_dependencies(od 27_day_harib24f_api008_od)

#----------------------------------------------------------
########### next target ###############
set(api009.img_SRCS ${root_SOURCE_DIR}/projects/27_day/harib24f/api009.nas)
set(api009.img_OUTS ${root_BINARY_DIR}/projects/27_day/harib24f/api009.img)
set(api009_wine.img_OUTS ${root_BINARY_DIR}/projects/27_day/harib24f/api009_wine.img)

add_custom_target(27_day_harib24f_api009
  COMMAND ${root_BINARY_DIR}/src/opennask ${api009.img_SRCS} ${api009.img_OUTS}
)
set(api009.img_HRB ${root_BINARY_DIR}/projects/27_day/harib24f/api009.hrb)
add_custom_target(27_day_harib24f_api009_hrb
  COMMAND ${root_BINARY_DIR}/src/opennask ${api009.img_SRCS} ${api009.img_HRB}
  COMMAND mcopy -i ${27_day_harib24f_OS} ${api009.img_HRB} ::api009.hrb
  DEPENDS 27_day_harib24f_img
)

add_dependencies(27_day_harib24f_run 27_day_harib24f_api009_hrb)
add_custom_target(27_day_harib24f_api009_wine
  COMMAND ${WINE} ${WINE_NASK} ${api009.img_SRCS} ${api009_wine.img_OUTS}
)
add_custom_target(27_day_harib24f_api009_od
  COMMAND ${OD} -t x1 ${api009.img_OUTS}      > ${api009.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${api009_wine.img_OUTS} > ${api009_wine.img_OUTS}_t.txt
  COMMAND diff -s ${api009.img_OUTS}_f.txt ${api009_wine.img_OUTS}_t.txt
)

add_dependencies(27_day_harib24f_api009_od 27_day_harib24f_api009)
add_dependencies(27_day_harib24f_api009_od 27_day_harib24f_api009_wine)

add_dependencies(images 27_day_harib24f_api009)
add_dependencies(wine 27_day_harib24f_api009_wine)
add_dependencies(od 27_day_harib24f_api009_od)

#----------------------------------------------------------
########### next target ###############
set(api010.img_SRCS ${root_SOURCE_DIR}/projects/27_day/harib24f/api010.nas)
set(api010.img_OUTS ${root_BINARY_DIR}/projects/27_day/harib24f/api010.img)
set(api010_wine.img_OUTS ${root_BINARY_DIR}/projects/27_day/harib24f/api010_wine.img)

add_custom_target(27_day_harib24f_api010
  COMMAND ${root_BINARY_DIR}/src/opennask ${api010.img_SRCS} ${api010.img_OUTS}
)
set(api010.img_HRB ${root_BINARY_DIR}/projects/27_day/harib24f/api010.hrb)
add_custom_target(27_day_harib24f_api010_hrb
  COMMAND ${root_BINARY_DIR}/src/opennask ${api010.img_SRCS} ${api010.img_HRB}
  COMMAND mcopy -i ${27_day_harib24f_OS} ${api010.img_HRB} ::api010.hrb
  DEPENDS 27_day_harib24f_img
)

add_dependencies(27_day_harib24f_run 27_day_harib24f_api010_hrb)
add_custom_target(27_day_harib24f_api010_wine
  COMMAND ${WINE} ${WINE_NASK} ${api010.img_SRCS} ${api010_wine.img_OUTS}
)
add_custom_target(27_day_harib24f_api010_od
  COMMAND ${OD} -t x1 ${api010.img_OUTS}      > ${api010.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${api010_wine.img_OUTS} > ${api010_wine.img_OUTS}_t.txt
  COMMAND diff -s ${api010.img_OUTS}_f.txt ${api010_wine.img_OUTS}_t.txt
)

add_dependencies(27_day_harib24f_api010_od 27_day_harib24f_api010)
add_dependencies(27_day_harib24f_api010_od 27_day_harib24f_api010_wine)

add_dependencies(images 27_day_harib24f_api010)
add_dependencies(wine 27_day_harib24f_api010_wine)
add_dependencies(od 27_day_harib24f_api010_od)

#----------------------------------------------------------
########### next target ###############
set(api011.img_SRCS ${root_SOURCE_DIR}/projects/27_day/harib24f/api011.nas)
set(api011.img_OUTS ${root_BINARY_DIR}/projects/27_day/harib24f/api011.img)
set(api011_wine.img_OUTS ${root_BINARY_DIR}/projects/27_day/harib24f/api011_wine.img)

add_custom_target(27_day_harib24f_api011
  COMMAND ${root_BINARY_DIR}/src/opennask ${api011.img_SRCS} ${api011.img_OUTS}
)
set(api011.img_HRB ${root_BINARY_DIR}/projects/27_day/harib24f/api011.hrb)
add_custom_target(27_day_harib24f_api011_hrb
  COMMAND ${root_BINARY_DIR}/src/opennask ${api011.img_SRCS} ${api011.img_HRB}
  COMMAND mcopy -i ${27_day_harib24f_OS} ${api011.img_HRB} ::api011.hrb
  DEPENDS 27_day_harib24f_img
)

add_dependencies(27_day_harib24f_run 27_day_harib24f_api011_hrb)
add_custom_target(27_day_harib24f_api011_wine
  COMMAND ${WINE} ${WINE_NASK} ${api011.img_SRCS} ${api011_wine.img_OUTS}
)
add_custom_target(27_day_harib24f_api011_od
  COMMAND ${OD} -t x1 ${api011.img_OUTS}      > ${api011.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${api011_wine.img_OUTS} > ${api011_wine.img_OUTS}_t.txt
  COMMAND diff -s ${api011.img_OUTS}_f.txt ${api011_wine.img_OUTS}_t.txt
)

add_dependencies(27_day_harib24f_api011_od 27_day_harib24f_api011)
add_dependencies(27_day_harib24f_api011_od 27_day_harib24f_api011_wine)

add_dependencies(images 27_day_harib24f_api011)
add_dependencies(wine 27_day_harib24f_api011_wine)
add_dependencies(od 27_day_harib24f_api011_od)

#----------------------------------------------------------
########### next target ###############
set(api012.img_SRCS ${root_SOURCE_DIR}/projects/27_day/harib24f/api012.nas)
set(api012.img_OUTS ${root_BINARY_DIR}/projects/27_day/harib24f/api012.img)
set(api012_wine.img_OUTS ${root_BINARY_DIR}/projects/27_day/harib24f/api012_wine.img)

add_custom_target(27_day_harib24f_api012
  COMMAND ${root_BINARY_DIR}/src/opennask ${api012.img_SRCS} ${api012.img_OUTS}
)
set(api012.img_HRB ${root_BINARY_DIR}/projects/27_day/harib24f/api012.hrb)
add_custom_target(27_day_harib24f_api012_hrb
  COMMAND ${root_BINARY_DIR}/src/opennask ${api012.img_SRCS} ${api012.img_HRB}
  COMMAND mcopy -i ${27_day_harib24f_OS} ${api012.img_HRB} ::api012.hrb
  DEPENDS 27_day_harib24f_img
)

add_dependencies(27_day_harib24f_run 27_day_harib24f_api012_hrb)
add_custom_target(27_day_harib24f_api012_wine
  COMMAND ${WINE} ${WINE_NASK} ${api012.img_SRCS} ${api012_wine.img_OUTS}
)
add_custom_target(27_day_harib24f_api012_od
  COMMAND ${OD} -t x1 ${api012.img_OUTS}      > ${api012.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${api012_wine.img_OUTS} > ${api012_wine.img_OUTS}_t.txt
  COMMAND diff -s ${api012.img_OUTS}_f.txt ${api012_wine.img_OUTS}_t.txt
)

add_dependencies(27_day_harib24f_api012_od 27_day_harib24f_api012)
add_dependencies(27_day_harib24f_api012_od 27_day_harib24f_api012_wine)

add_dependencies(images 27_day_harib24f_api012)
add_dependencies(wine 27_day_harib24f_api012_wine)
add_dependencies(od 27_day_harib24f_api012_od)

#----------------------------------------------------------
########### next target ###############
set(api013.img_SRCS ${root_SOURCE_DIR}/projects/27_day/harib24f/api013.nas)
set(api013.img_OUTS ${root_BINARY_DIR}/projects/27_day/harib24f/api013.img)
set(api013_wine.img_OUTS ${root_BINARY_DIR}/projects/27_day/harib24f/api013_wine.img)

add_custom_target(27_day_harib24f_api013
  COMMAND ${root_BINARY_DIR}/src/opennask ${api013.img_SRCS} ${api013.img_OUTS}
)
set(api013.img_HRB ${root_BINARY_DIR}/projects/27_day/harib24f/api013.hrb)
add_custom_target(27_day_harib24f_api013_hrb
  COMMAND ${root_BINARY_DIR}/src/opennask ${api013.img_SRCS} ${api013.img_HRB}
  COMMAND mcopy -i ${27_day_harib24f_OS} ${api013.img_HRB} ::api013.hrb
  DEPENDS 27_day_harib24f_img
)

add_dependencies(27_day_harib24f_run 27_day_harib24f_api013_hrb)
add_custom_target(27_day_harib24f_api013_wine
  COMMAND ${WINE} ${WINE_NASK} ${api013.img_SRCS} ${api013_wine.img_OUTS}
)
add_custom_target(27_day_harib24f_api013_od
  COMMAND ${OD} -t x1 ${api013.img_OUTS}      > ${api013.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${api013_wine.img_OUTS} > ${api013_wine.img_OUTS}_t.txt
  COMMAND diff -s ${api013.img_OUTS}_f.txt ${api013_wine.img_OUTS}_t.txt
)

add_dependencies(27_day_harib24f_api013_od 27_day_harib24f_api013)
add_dependencies(27_day_harib24f_api013_od 27_day_harib24f_api013_wine)

add_dependencies(images 27_day_harib24f_api013)
add_dependencies(wine 27_day_harib24f_api013_wine)
add_dependencies(od 27_day_harib24f_api013_od)

#----------------------------------------------------------
########### next target ###############
set(api014.img_SRCS ${root_SOURCE_DIR}/projects/27_day/harib24f/api014.nas)
set(api014.img_OUTS ${root_BINARY_DIR}/projects/27_day/harib24f/api014.img)
set(api014_wine.img_OUTS ${root_BINARY_DIR}/projects/27_day/harib24f/api014_wine.img)

add_custom_target(27_day_harib24f_api014
  COMMAND ${root_BINARY_DIR}/src/opennask ${api014.img_SRCS} ${api014.img_OUTS}
)
set(api014.img_HRB ${root_BINARY_DIR}/projects/27_day/harib24f/api014.hrb)
add_custom_target(27_day_harib24f_api014_hrb
  COMMAND ${root_BINARY_DIR}/src/opennask ${api014.img_SRCS} ${api014.img_HRB}
  COMMAND mcopy -i ${27_day_harib24f_OS} ${api014.img_HRB} ::api014.hrb
  DEPENDS 27_day_harib24f_img
)

add_dependencies(27_day_harib24f_run 27_day_harib24f_api014_hrb)
add_custom_target(27_day_harib24f_api014_wine
  COMMAND ${WINE} ${WINE_NASK} ${api014.img_SRCS} ${api014_wine.img_OUTS}
)
add_custom_target(27_day_harib24f_api014_od
  COMMAND ${OD} -t x1 ${api014.img_OUTS}      > ${api014.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${api014_wine.img_OUTS} > ${api014_wine.img_OUTS}_t.txt
  COMMAND diff -s ${api014.img_OUTS}_f.txt ${api014_wine.img_OUTS}_t.txt
)

add_dependencies(27_day_harib24f_api014_od 27_day_harib24f_api014)
add_dependencies(27_day_harib24f_api014_od 27_day_harib24f_api014_wine)

add_dependencies(images 27_day_harib24f_api014)
add_dependencies(wine 27_day_harib24f_api014_wine)
add_dependencies(od 27_day_harib24f_api014_od)

#----------------------------------------------------------
########### next target ###############
set(api015.img_SRCS ${root_SOURCE_DIR}/projects/27_day/harib24f/api015.nas)
set(api015.img_OUTS ${root_BINARY_DIR}/projects/27_day/harib24f/api015.img)
set(api015_wine.img_OUTS ${root_BINARY_DIR}/projects/27_day/harib24f/api015_wine.img)

add_custom_target(27_day_harib24f_api015
  COMMAND ${root_BINARY_DIR}/src/opennask ${api015.img_SRCS} ${api015.img_OUTS}
)
set(api015.img_HRB ${root_BINARY_DIR}/projects/27_day/harib24f/api015.hrb)
add_custom_target(27_day_harib24f_api015_hrb
  COMMAND ${root_BINARY_DIR}/src/opennask ${api015.img_SRCS} ${api015.img_HRB}
  COMMAND mcopy -i ${27_day_harib24f_OS} ${api015.img_HRB} ::api015.hrb
  DEPENDS 27_day_harib24f_img
)

add_dependencies(27_day_harib24f_run 27_day_harib24f_api015_hrb)
add_custom_target(27_day_harib24f_api015_wine
  COMMAND ${WINE} ${WINE_NASK} ${api015.img_SRCS} ${api015_wine.img_OUTS}
)
add_custom_target(27_day_harib24f_api015_od
  COMMAND ${OD} -t x1 ${api015.img_OUTS}      > ${api015.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${api015_wine.img_OUTS} > ${api015_wine.img_OUTS}_t.txt
  COMMAND diff -s ${api015.img_OUTS}_f.txt ${api015_wine.img_OUTS}_t.txt
)

add_dependencies(27_day_harib24f_api015_od 27_day_harib24f_api015)
add_dependencies(27_day_harib24f_api015_od 27_day_harib24f_api015_wine)

add_dependencies(images 27_day_harib24f_api015)
add_dependencies(wine 27_day_harib24f_api015_wine)
add_dependencies(od 27_day_harib24f_api015_od)

#----------------------------------------------------------
########### next target ###############
set(api016.img_SRCS ${root_SOURCE_DIR}/projects/27_day/harib24f/api016.nas)
set(api016.img_OUTS ${root_BINARY_DIR}/projects/27_day/harib24f/api016.img)
set(api016_wine.img_OUTS ${root_BINARY_DIR}/projects/27_day/harib24f/api016_wine.img)

add_custom_target(27_day_harib24f_api016
  COMMAND ${root_BINARY_DIR}/src/opennask ${api016.img_SRCS} ${api016.img_OUTS}
)
set(api016.img_HRB ${root_BINARY_DIR}/projects/27_day/harib24f/api016.hrb)
add_custom_target(27_day_harib24f_api016_hrb
  COMMAND ${root_BINARY_DIR}/src/opennask ${api016.img_SRCS} ${api016.img_HRB}
  COMMAND mcopy -i ${27_day_harib24f_OS} ${api016.img_HRB} ::api016.hrb
  DEPENDS 27_day_harib24f_img
)

add_dependencies(27_day_harib24f_run 27_day_harib24f_api016_hrb)
add_custom_target(27_day_harib24f_api016_wine
  COMMAND ${WINE} ${WINE_NASK} ${api016.img_SRCS} ${api016_wine.img_OUTS}
)
add_custom_target(27_day_harib24f_api016_od
  COMMAND ${OD} -t x1 ${api016.img_OUTS}      > ${api016.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${api016_wine.img_OUTS} > ${api016_wine.img_OUTS}_t.txt
  COMMAND diff -s ${api016.img_OUTS}_f.txt ${api016_wine.img_OUTS}_t.txt
)

add_dependencies(27_day_harib24f_api016_od 27_day_harib24f_api016)
add_dependencies(27_day_harib24f_api016_od 27_day_harib24f_api016_wine)

add_dependencies(images 27_day_harib24f_api016)
add_dependencies(wine 27_day_harib24f_api016_wine)
add_dependencies(od 27_day_harib24f_api016_od)

#----------------------------------------------------------
########### next target ###############
set(api017.img_SRCS ${root_SOURCE_DIR}/projects/27_day/harib24f/api017.nas)
set(api017.img_OUTS ${root_BINARY_DIR}/projects/27_day/harib24f/api017.img)
set(api017_wine.img_OUTS ${root_BINARY_DIR}/projects/27_day/harib24f/api017_wine.img)

add_custom_target(27_day_harib24f_api017
  COMMAND ${root_BINARY_DIR}/src/opennask ${api017.img_SRCS} ${api017.img_OUTS}
)
set(api017.img_HRB ${root_BINARY_DIR}/projects/27_day/harib24f/api017.hrb)
add_custom_target(27_day_harib24f_api017_hrb
  COMMAND ${root_BINARY_DIR}/src/opennask ${api017.img_SRCS} ${api017.img_HRB}
  COMMAND mcopy -i ${27_day_harib24f_OS} ${api017.img_HRB} ::api017.hrb
  DEPENDS 27_day_harib24f_img
)

add_dependencies(27_day_harib24f_run 27_day_harib24f_api017_hrb)
add_custom_target(27_day_harib24f_api017_wine
  COMMAND ${WINE} ${WINE_NASK} ${api017.img_SRCS} ${api017_wine.img_OUTS}
)
add_custom_target(27_day_harib24f_api017_od
  COMMAND ${OD} -t x1 ${api017.img_OUTS}      > ${api017.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${api017_wine.img_OUTS} > ${api017_wine.img_OUTS}_t.txt
  COMMAND diff -s ${api017.img_OUTS}_f.txt ${api017_wine.img_OUTS}_t.txt
)

add_dependencies(27_day_harib24f_api017_od 27_day_harib24f_api017)
add_dependencies(27_day_harib24f_api017_od 27_day_harib24f_api017_wine)

add_dependencies(images 27_day_harib24f_api017)
add_dependencies(wine 27_day_harib24f_api017_wine)
add_dependencies(od 27_day_harib24f_api017_od)

#----------------------------------------------------------
########### next target ###############
set(api018.img_SRCS ${root_SOURCE_DIR}/projects/27_day/harib24f/api018.nas)
set(api018.img_OUTS ${root_BINARY_DIR}/projects/27_day/harib24f/api018.img)
set(api018_wine.img_OUTS ${root_BINARY_DIR}/projects/27_day/harib24f/api018_wine.img)

add_custom_target(27_day_harib24f_api018
  COMMAND ${root_BINARY_DIR}/src/opennask ${api018.img_SRCS} ${api018.img_OUTS}
)
set(api018.img_HRB ${root_BINARY_DIR}/projects/27_day/harib24f/api018.hrb)
add_custom_target(27_day_harib24f_api018_hrb
  COMMAND ${root_BINARY_DIR}/src/opennask ${api018.img_SRCS} ${api018.img_HRB}
  COMMAND mcopy -i ${27_day_harib24f_OS} ${api018.img_HRB} ::api018.hrb
  DEPENDS 27_day_harib24f_img
)

add_dependencies(27_day_harib24f_run 27_day_harib24f_api018_hrb)
add_custom_target(27_day_harib24f_api018_wine
  COMMAND ${WINE} ${WINE_NASK} ${api018.img_SRCS} ${api018_wine.img_OUTS}
)
add_custom_target(27_day_harib24f_api018_od
  COMMAND ${OD} -t x1 ${api018.img_OUTS}      > ${api018.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${api018_wine.img_OUTS} > ${api018_wine.img_OUTS}_t.txt
  COMMAND diff -s ${api018.img_OUTS}_f.txt ${api018_wine.img_OUTS}_t.txt
)

add_dependencies(27_day_harib24f_api018_od 27_day_harib24f_api018)
add_dependencies(27_day_harib24f_api018_od 27_day_harib24f_api018_wine)

add_dependencies(images 27_day_harib24f_api018)
add_dependencies(wine 27_day_harib24f_api018_wine)
add_dependencies(od 27_day_harib24f_api018_od)

#----------------------------------------------------------
########### next target ###############
set(api019.img_SRCS ${root_SOURCE_DIR}/projects/27_day/harib24f/api019.nas)
set(api019.img_OUTS ${root_BINARY_DIR}/projects/27_day/harib24f/api019.img)
set(api019_wine.img_OUTS ${root_BINARY_DIR}/projects/27_day/harib24f/api019_wine.img)

add_custom_target(27_day_harib24f_api019
  COMMAND ${root_BINARY_DIR}/src/opennask ${api019.img_SRCS} ${api019.img_OUTS}
)
set(api019.img_HRB ${root_BINARY_DIR}/projects/27_day/harib24f/api019.hrb)
add_custom_target(27_day_harib24f_api019_hrb
  COMMAND ${root_BINARY_DIR}/src/opennask ${api019.img_SRCS} ${api019.img_HRB}
  COMMAND mcopy -i ${27_day_harib24f_OS} ${api019.img_HRB} ::api019.hrb
  DEPENDS 27_day_harib24f_img
)

add_dependencies(27_day_harib24f_run 27_day_harib24f_api019_hrb)
add_custom_target(27_day_harib24f_api019_wine
  COMMAND ${WINE} ${WINE_NASK} ${api019.img_SRCS} ${api019_wine.img_OUTS}
)
add_custom_target(27_day_harib24f_api019_od
  COMMAND ${OD} -t x1 ${api019.img_OUTS}      > ${api019.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${api019_wine.img_OUTS} > ${api019_wine.img_OUTS}_t.txt
  COMMAND diff -s ${api019.img_OUTS}_f.txt ${api019_wine.img_OUTS}_t.txt
)

add_dependencies(27_day_harib24f_api019_od 27_day_harib24f_api019)
add_dependencies(27_day_harib24f_api019_od 27_day_harib24f_api019_wine)

add_dependencies(images 27_day_harib24f_api019)
add_dependencies(wine 27_day_harib24f_api019_wine)
add_dependencies(od 27_day_harib24f_api019_od)

#----------------------------------------------------------
########### next target ###############
set(api020.img_SRCS ${root_SOURCE_DIR}/projects/27_day/harib24f/api020.nas)
set(api020.img_OUTS ${root_BINARY_DIR}/projects/27_day/harib24f/api020.img)
set(api020_wine.img_OUTS ${root_BINARY_DIR}/projects/27_day/harib24f/api020_wine.img)

add_custom_target(27_day_harib24f_api020
  COMMAND ${root_BINARY_DIR}/src/opennask ${api020.img_SRCS} ${api020.img_OUTS}
)
set(api020.img_HRB ${root_BINARY_DIR}/projects/27_day/harib24f/api020.hrb)
add_custom_target(27_day_harib24f_api020_hrb
  COMMAND ${root_BINARY_DIR}/src/opennask ${api020.img_SRCS} ${api020.img_HRB}
  COMMAND mcopy -i ${27_day_harib24f_OS} ${api020.img_HRB} ::api020.hrb
  DEPENDS 27_day_harib24f_img
)

add_dependencies(27_day_harib24f_run 27_day_harib24f_api020_hrb)
add_custom_target(27_day_harib24f_api020_wine
  COMMAND ${WINE} ${WINE_NASK} ${api020.img_SRCS} ${api020_wine.img_OUTS}
)
add_custom_target(27_day_harib24f_api020_od
  COMMAND ${OD} -t x1 ${api020.img_OUTS}      > ${api020.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${api020_wine.img_OUTS} > ${api020_wine.img_OUTS}_t.txt
  COMMAND diff -s ${api020.img_OUTS}_f.txt ${api020_wine.img_OUTS}_t.txt
)

add_dependencies(27_day_harib24f_api020_od 27_day_harib24f_api020)
add_dependencies(27_day_harib24f_api020_od 27_day_harib24f_api020_wine)

add_dependencies(images 27_day_harib24f_api020)
add_dependencies(wine 27_day_harib24f_api020_wine)
add_dependencies(od 27_day_harib24f_api020_od)

#----------------------------------------------------------
########### next target ###############
set(asmhead.img_SRCS ${root_SOURCE_DIR}/projects/27_day/harib24f/asmhead.nas)
set(asmhead.img_OUTS ${root_BINARY_DIR}/projects/27_day/harib24f/asmhead.img)
set(asmhead_wine.img_OUTS ${root_BINARY_DIR}/projects/27_day/harib24f/asmhead_wine.img)

add_custom_target(27_day_harib24f_asmhead
  COMMAND ${root_BINARY_DIR}/src/opennask ${asmhead.img_SRCS} ${asmhead.img_OUTS}
)
add_custom_target(27_day_harib24f_asmhead_wine
  COMMAND ${WINE} ${WINE_NASK} ${asmhead.img_SRCS} ${asmhead_wine.img_OUTS}
)
add_custom_target(27_day_harib24f_asmhead_od
  COMMAND ${OD} -t x1 ${asmhead.img_OUTS}      > ${asmhead.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${asmhead_wine.img_OUTS} > ${asmhead_wine.img_OUTS}_t.txt
  COMMAND diff -s ${asmhead.img_OUTS}_f.txt ${asmhead_wine.img_OUTS}_t.txt
)

add_dependencies(27_day_harib24f_asmhead_od 27_day_harib24f_asmhead)
add_dependencies(27_day_harib24f_asmhead_od 27_day_harib24f_asmhead_wine)

add_dependencies(images 27_day_harib24f_asmhead)
add_dependencies(wine 27_day_harib24f_asmhead_wine)
add_dependencies(od 27_day_harib24f_asmhead_od)

#----------------------------------------------------------
########### next target ###############
set(hello5.img_SRCS ${root_SOURCE_DIR}/projects/27_day/harib24f/hello5.nas)
set(hello5.img_OUTS ${root_BINARY_DIR}/projects/27_day/harib24f/hello5.img)
set(hello5_wine.img_OUTS ${root_BINARY_DIR}/projects/27_day/harib24f/hello5_wine.img)

add_custom_target(27_day_harib24f_hello5
  COMMAND ${root_BINARY_DIR}/src/opennask ${hello5.img_SRCS} ${hello5.img_OUTS}
)
set(hello5.img_HRB ${root_BINARY_DIR}/projects/27_day/harib24f/hello5.hrb)
add_custom_target(27_day_harib24f_hello5_hrb
  COMMAND ${root_BINARY_DIR}/src/opennask ${hello5.img_SRCS} ${hello5.img_HRB}
  COMMAND mcopy -i ${27_day_harib24f_OS} ${hello5.img_HRB} ::hello5.hrb
  DEPENDS 27_day_harib24f_img
)

add_dependencies(27_day_harib24f_run 27_day_harib24f_hello5_hrb)
add_custom_target(27_day_harib24f_hello5_wine
  COMMAND ${WINE} ${WINE_NASK} ${hello5.img_SRCS} ${hello5_wine.img_OUTS}
)
add_custom_target(27_day_harib24f_hello5_od
  COMMAND ${OD} -t x1 ${hello5.img_OUTS}      > ${hello5.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${hello5_wine.img_OUTS} > ${hello5_wine.img_OUTS}_t.txt
  COMMAND diff -s ${hello5.img_OUTS}_f.txt ${hello5_wine.img_OUTS}_t.txt
)

add_dependencies(27_day_harib24f_hello5_od 27_day_harib24f_hello5)
add_dependencies(27_day_harib24f_hello5_od 27_day_harib24f_hello5_wine)

add_dependencies(images 27_day_harib24f_hello5)
add_dependencies(wine 27_day_harib24f_hello5_wine)
add_dependencies(od 27_day_harib24f_hello5_od)

#----------------------------------------------------------
########### next target ###############
set(ipl10.img_SRCS ${root_SOURCE_DIR}/projects/27_day/harib24f/ipl10.nas)
set(ipl10.img_OUTS ${root_BINARY_DIR}/projects/27_day/harib24f/ipl10.img)
set(ipl10_wine.img_OUTS ${root_BINARY_DIR}/projects/27_day/harib24f/ipl10_wine.img)

add_custom_target(27_day_harib24f_ipl10
  COMMAND ${root_BINARY_DIR}/src/opennask ${ipl10.img_SRCS} ${ipl10.img_OUTS}
)
add_custom_target(27_day_harib24f_ipl10_wine
  COMMAND ${WINE} ${WINE_NASK} ${ipl10.img_SRCS} ${ipl10_wine.img_OUTS}
)
add_custom_target(27_day_harib24f_ipl10_od
  COMMAND ${OD} -t x1 ${ipl10.img_OUTS}      > ${ipl10.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${ipl10_wine.img_OUTS} > ${ipl10_wine.img_OUTS}_t.txt
  COMMAND diff -s ${ipl10.img_OUTS}_f.txt ${ipl10_wine.img_OUTS}_t.txt
)

add_dependencies(27_day_harib24f_ipl10_od 27_day_harib24f_ipl10)
add_dependencies(27_day_harib24f_ipl10_od 27_day_harib24f_ipl10_wine)

add_dependencies(images 27_day_harib24f_ipl10)
add_dependencies(wine 27_day_harib24f_ipl10_wine)
add_dependencies(od 27_day_harib24f_ipl10_od)

#----------------------------------------------------------
########### next target ###############
set(naskfunc.img_SRCS ${root_SOURCE_DIR}/projects/27_day/harib24f/naskfunc.nas)
set(naskfunc.img_OUTS ${root_BINARY_DIR}/projects/27_day/harib24f/naskfunc.img)
set(naskfunc_wine.img_OUTS ${root_BINARY_DIR}/projects/27_day/harib24f/naskfunc_wine.img)

add_custom_target(27_day_harib24f_naskfunc
  COMMAND ${root_BINARY_DIR}/src/opennask ${naskfunc.img_SRCS} ${naskfunc.img_OUTS}
)
add_custom_target(27_day_harib24f_naskfunc_wine
  COMMAND ${WINE} ${WINE_NASK} ${naskfunc.img_SRCS} ${naskfunc_wine.img_OUTS}
)
add_custom_target(27_day_harib24f_naskfunc_od
  COMMAND ${OD} -t x1 ${naskfunc.img_OUTS}      > ${naskfunc.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${naskfunc_wine.img_OUTS} > ${naskfunc_wine.img_OUTS}_t.txt
  COMMAND diff -s ${naskfunc.img_OUTS}_f.txt ${naskfunc_wine.img_OUTS}_t.txt
)

add_dependencies(27_day_harib24f_naskfunc_od 27_day_harib24f_naskfunc)
add_dependencies(27_day_harib24f_naskfunc_od 27_day_harib24f_naskfunc_wine)

add_dependencies(images 27_day_harib24f_naskfunc)
add_dependencies(wine 27_day_harib24f_naskfunc_wine)
add_dependencies(od 27_day_harib24f_naskfunc_od)

#----------------------------------------------------------
