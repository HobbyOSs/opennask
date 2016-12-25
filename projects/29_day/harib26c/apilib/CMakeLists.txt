#----------------------------------------------------------
message(STATUS "Entering directory projects/29_day/harib26c/apilib/")

set(NASK ${root_BINARY_DIR}/src/opennask)
set(FONT ${root_BINARY_DIR}/src/makefont)
set(B2O  ${root_BINARY_DIR}/src/bin2obj)
set(CONV ${root_BINARY_DIR}/objconv/objconv)
set(29_day_harib26c_apilib_OS    ${root_BINARY_DIR}/projects/29_day/harib26c/apilib/os.img)
set(29_day_harib26c_apilib_SYS	  ${root_BINARY_DIR}/projects/29_day/harib26c/apilib/os.sys)
set(29_day_harib26c_apilib_IPLB  ${root_BINARY_DIR}/projects/29_day/harib26c/apilib/ipl.bin)
set(29_day_harib26c_apilib_IPLS  ${root_SOURCE_DIR}/projects/29_day/harib26c/apilib/ipl10.nas)
set(29_day_harib26c_apilib_HEADB ${root_BINARY_DIR}/projects/29_day/harib26c/apilib/asmhead.bin)
set(29_day_harib26c_apilib_HEADS ${root_SOURCE_DIR}/projects/29_day/harib26c/apilib/asmhead.nas)
set(29_day_harib26c_apilib_BOOTB ${root_BINARY_DIR}/projects/29_day/harib26c/apilib/boot.bin)
set(29_day_harib26c_apilib_CCO	  ${root_BINARY_DIR}/projects/29_day/harib26c/apilib/boot.o)
set(29_day_harib26c_apilib_CCS	  ${root_SOURCE_DIR}/projects/29_day/harib26c/apilib/*.c)
set(29_day_harib26c_apilib_LDS	  ${root_SOURCE_DIR}/projects/os.lds)
set(29_day_harib26c_apilib_WILDOBJ ${root_BINARY_DIR}/projects/29_day/harib26c/apilib/*.o)


add_custom_target(29_day_harib26c_apilib_alloca_run
  COMMAND ${QEMU} ${QEMUOPT} ${29_day_harib26c_apilib_OS}
  DEPENDS 29_day_harib26c_apilib_img
)
add_custom_target(29_day_harib26c_apilib_alloca_debug
  COMMAND ${QEMU} -s -S ${QEMUOPT} ${29_day_harib26c_apilib_OS} -redir tcp:5555:127.0.0.1:1234 &
  DEPENDS 29_day_harib26c_apilib_img
)
add_custom_target(29_day_harib26c_apilib_alloca_clean
  COMMAND rm -f ${29_day_harib26c_apilib_OS}
  COMMAND rm -f ${29_day_harib26c_apilib_SYS}
  COMMAND rm -f ${29_day_harib26c_apilib_IPLB}
  COMMAND rm -f ${29_day_harib26c_apilib_HEADB}
  COMMAND rm -f ${29_day_harib26c_apilib_BOOTB}
  COMMAND rm -f ${29_day_harib26c_apilib_WILDOBJ}
)
add_custom_target(29_day_harib26c_apilib_alloca_ipl
  COMMAND ${NASK} ${29_day_harib26c_apilib_IPLS} ${29_day_harib26c_apilib_IPLB}
)
add_custom_target(29_day_harib26c_apilib_alloca_sys
  COMMAND ${NASK} ${29_day_harib26c_apilib_HEADS} ${29_day_harib26c_apilib_HEADB}
  COMMAND gcc ${BINOPT} -T ${29_day_harib26c_apilib_LDS} ${29_day_harib26c_apilib_CCS} -o ${29_day_harib26c_apilib_BOOTB}
  COMMAND cat ${29_day_harib26c_apilib_HEADB} ${29_day_harib26c_apilib_BOOTB} > ${29_day_harib26c_apilib_SYS}
  DEPENDS 29_day_harib26c_apilib_ipl
)
add_custom_target(29_day_harib26c_apilib_alloca_img
  COMMAND mformat -f 1440 -l HARIBOTEOS -N 0xffffffff -C -B ${29_day_harib26c_apilib_IPLB} -i ${29_day_harib26c_apilib_OS}
  COMMAND mcopy -i ${29_day_harib26c_apilib_OS} ${29_day_harib26c_apilib_SYS} ::
  DEPENDS 29_day_harib26c_apilib_sys
)
########### next target ###############
set(alloca.img_SRCS ${root_SOURCE_DIR}/projects/29_day/harib26c/apilib/alloca.nas)
set(alloca.img_OUTS ${root_BINARY_DIR}/projects/29_day/harib26c/apilib/alloca.img)
set(alloca_wine.img_OUTS ${root_BINARY_DIR}/projects/29_day/harib26c/apilib/alloca_wine.img)

add_custom_target(29_day_harib26c_apilib_alloca
  COMMAND ${root_BINARY_DIR}/src/opennask ${alloca.img_SRCS} ${alloca.img_OUTS}
)
set(alloca.img_HRB ${root_BINARY_DIR}/projects/29_day/harib26c/apilib/alloca.hrb)
add_custom_target(29_day_harib26c_apilib_alloca_hrb
  COMMAND ${root_BINARY_DIR}/src/opennask ${alloca.img_SRCS} ${alloca.img_HRB}
  COMMAND mcopy -i ${29_day_harib26c_apilib_OS} ${alloca.img_HRB} ::alloca.hrb
  DEPENDS 29_day_harib26c_apilib_img
)

add_dependencies(29_day_harib26c_apilib_run 29_day_harib26c_apilib_alloca_hrb)
add_custom_target(29_day_harib26c_apilib_alloca_wine
  COMMAND ${WINE} ${WINE_NASK} ${alloca.img_SRCS} ${alloca_wine.img_OUTS}
)
add_custom_target(29_day_harib26c_apilib_alloca_od
  COMMAND ${OD} -t x1 ${alloca.img_OUTS}      > ${alloca.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${alloca_wine.img_OUTS} > ${alloca_wine.img_OUTS}_t.txt
  COMMAND diff -s ${alloca.img_OUTS}_f.txt ${alloca_wine.img_OUTS}_t.txt
)

add_dependencies(29_day_harib26c_apilib_alloca_od 29_day_harib26c_apilib_alloca)
add_dependencies(29_day_harib26c_apilib_alloca_od 29_day_harib26c_apilib_alloca_wine)

add_dependencies(images 29_day_harib26c_apilib_alloca)
add_dependencies(wine 29_day_harib26c_apilib_alloca_wine)
add_dependencies(od 29_day_harib26c_apilib_alloca_od)

#----------------------------------------------------------
########### next target ###############
set(api001.img_SRCS ${root_SOURCE_DIR}/projects/29_day/harib26c/apilib/api001.nas)
set(api001.img_OUTS ${root_BINARY_DIR}/projects/29_day/harib26c/apilib/api001.img)
set(api001_wine.img_OUTS ${root_BINARY_DIR}/projects/29_day/harib26c/apilib/api001_wine.img)

add_custom_target(29_day_harib26c_apilib_api001
  COMMAND ${root_BINARY_DIR}/src/opennask ${api001.img_SRCS} ${api001.img_OUTS}
)
set(api001.img_HRB ${root_BINARY_DIR}/projects/29_day/harib26c/apilib/api001.hrb)
add_custom_target(29_day_harib26c_apilib_api001_hrb
  COMMAND ${root_BINARY_DIR}/src/opennask ${api001.img_SRCS} ${api001.img_HRB}
  COMMAND mcopy -i ${29_day_harib26c_apilib_OS} ${api001.img_HRB} ::api001.hrb
  DEPENDS 29_day_harib26c_apilib_img
)

add_dependencies(29_day_harib26c_apilib_run 29_day_harib26c_apilib_api001_hrb)
add_custom_target(29_day_harib26c_apilib_api001_wine
  COMMAND ${WINE} ${WINE_NASK} ${api001.img_SRCS} ${api001_wine.img_OUTS}
)
add_custom_target(29_day_harib26c_apilib_api001_od
  COMMAND ${OD} -t x1 ${api001.img_OUTS}      > ${api001.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${api001_wine.img_OUTS} > ${api001_wine.img_OUTS}_t.txt
  COMMAND diff -s ${api001.img_OUTS}_f.txt ${api001_wine.img_OUTS}_t.txt
)

add_dependencies(29_day_harib26c_apilib_api001_od 29_day_harib26c_apilib_api001)
add_dependencies(29_day_harib26c_apilib_api001_od 29_day_harib26c_apilib_api001_wine)

add_dependencies(images 29_day_harib26c_apilib_api001)
add_dependencies(wine 29_day_harib26c_apilib_api001_wine)
add_dependencies(od 29_day_harib26c_apilib_api001_od)

#----------------------------------------------------------
########### next target ###############
set(api002.img_SRCS ${root_SOURCE_DIR}/projects/29_day/harib26c/apilib/api002.nas)
set(api002.img_OUTS ${root_BINARY_DIR}/projects/29_day/harib26c/apilib/api002.img)
set(api002_wine.img_OUTS ${root_BINARY_DIR}/projects/29_day/harib26c/apilib/api002_wine.img)

add_custom_target(29_day_harib26c_apilib_api002
  COMMAND ${root_BINARY_DIR}/src/opennask ${api002.img_SRCS} ${api002.img_OUTS}
)
set(api002.img_HRB ${root_BINARY_DIR}/projects/29_day/harib26c/apilib/api002.hrb)
add_custom_target(29_day_harib26c_apilib_api002_hrb
  COMMAND ${root_BINARY_DIR}/src/opennask ${api002.img_SRCS} ${api002.img_HRB}
  COMMAND mcopy -i ${29_day_harib26c_apilib_OS} ${api002.img_HRB} ::api002.hrb
  DEPENDS 29_day_harib26c_apilib_img
)

add_dependencies(29_day_harib26c_apilib_run 29_day_harib26c_apilib_api002_hrb)
add_custom_target(29_day_harib26c_apilib_api002_wine
  COMMAND ${WINE} ${WINE_NASK} ${api002.img_SRCS} ${api002_wine.img_OUTS}
)
add_custom_target(29_day_harib26c_apilib_api002_od
  COMMAND ${OD} -t x1 ${api002.img_OUTS}      > ${api002.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${api002_wine.img_OUTS} > ${api002_wine.img_OUTS}_t.txt
  COMMAND diff -s ${api002.img_OUTS}_f.txt ${api002_wine.img_OUTS}_t.txt
)

add_dependencies(29_day_harib26c_apilib_api002_od 29_day_harib26c_apilib_api002)
add_dependencies(29_day_harib26c_apilib_api002_od 29_day_harib26c_apilib_api002_wine)

add_dependencies(images 29_day_harib26c_apilib_api002)
add_dependencies(wine 29_day_harib26c_apilib_api002_wine)
add_dependencies(od 29_day_harib26c_apilib_api002_od)

#----------------------------------------------------------
########### next target ###############
set(api003.img_SRCS ${root_SOURCE_DIR}/projects/29_day/harib26c/apilib/api003.nas)
set(api003.img_OUTS ${root_BINARY_DIR}/projects/29_day/harib26c/apilib/api003.img)
set(api003_wine.img_OUTS ${root_BINARY_DIR}/projects/29_day/harib26c/apilib/api003_wine.img)

add_custom_target(29_day_harib26c_apilib_api003
  COMMAND ${root_BINARY_DIR}/src/opennask ${api003.img_SRCS} ${api003.img_OUTS}
)
set(api003.img_HRB ${root_BINARY_DIR}/projects/29_day/harib26c/apilib/api003.hrb)
add_custom_target(29_day_harib26c_apilib_api003_hrb
  COMMAND ${root_BINARY_DIR}/src/opennask ${api003.img_SRCS} ${api003.img_HRB}
  COMMAND mcopy -i ${29_day_harib26c_apilib_OS} ${api003.img_HRB} ::api003.hrb
  DEPENDS 29_day_harib26c_apilib_img
)

add_dependencies(29_day_harib26c_apilib_run 29_day_harib26c_apilib_api003_hrb)
add_custom_target(29_day_harib26c_apilib_api003_wine
  COMMAND ${WINE} ${WINE_NASK} ${api003.img_SRCS} ${api003_wine.img_OUTS}
)
add_custom_target(29_day_harib26c_apilib_api003_od
  COMMAND ${OD} -t x1 ${api003.img_OUTS}      > ${api003.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${api003_wine.img_OUTS} > ${api003_wine.img_OUTS}_t.txt
  COMMAND diff -s ${api003.img_OUTS}_f.txt ${api003_wine.img_OUTS}_t.txt
)

add_dependencies(29_day_harib26c_apilib_api003_od 29_day_harib26c_apilib_api003)
add_dependencies(29_day_harib26c_apilib_api003_od 29_day_harib26c_apilib_api003_wine)

add_dependencies(images 29_day_harib26c_apilib_api003)
add_dependencies(wine 29_day_harib26c_apilib_api003_wine)
add_dependencies(od 29_day_harib26c_apilib_api003_od)

#----------------------------------------------------------
########### next target ###############
set(api004.img_SRCS ${root_SOURCE_DIR}/projects/29_day/harib26c/apilib/api004.nas)
set(api004.img_OUTS ${root_BINARY_DIR}/projects/29_day/harib26c/apilib/api004.img)
set(api004_wine.img_OUTS ${root_BINARY_DIR}/projects/29_day/harib26c/apilib/api004_wine.img)

add_custom_target(29_day_harib26c_apilib_api004
  COMMAND ${root_BINARY_DIR}/src/opennask ${api004.img_SRCS} ${api004.img_OUTS}
)
set(api004.img_HRB ${root_BINARY_DIR}/projects/29_day/harib26c/apilib/api004.hrb)
add_custom_target(29_day_harib26c_apilib_api004_hrb
  COMMAND ${root_BINARY_DIR}/src/opennask ${api004.img_SRCS} ${api004.img_HRB}
  COMMAND mcopy -i ${29_day_harib26c_apilib_OS} ${api004.img_HRB} ::api004.hrb
  DEPENDS 29_day_harib26c_apilib_img
)

add_dependencies(29_day_harib26c_apilib_run 29_day_harib26c_apilib_api004_hrb)
add_custom_target(29_day_harib26c_apilib_api004_wine
  COMMAND ${WINE} ${WINE_NASK} ${api004.img_SRCS} ${api004_wine.img_OUTS}
)
add_custom_target(29_day_harib26c_apilib_api004_od
  COMMAND ${OD} -t x1 ${api004.img_OUTS}      > ${api004.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${api004_wine.img_OUTS} > ${api004_wine.img_OUTS}_t.txt
  COMMAND diff -s ${api004.img_OUTS}_f.txt ${api004_wine.img_OUTS}_t.txt
)

add_dependencies(29_day_harib26c_apilib_api004_od 29_day_harib26c_apilib_api004)
add_dependencies(29_day_harib26c_apilib_api004_od 29_day_harib26c_apilib_api004_wine)

add_dependencies(images 29_day_harib26c_apilib_api004)
add_dependencies(wine 29_day_harib26c_apilib_api004_wine)
add_dependencies(od 29_day_harib26c_apilib_api004_od)

#----------------------------------------------------------
########### next target ###############
set(api005.img_SRCS ${root_SOURCE_DIR}/projects/29_day/harib26c/apilib/api005.nas)
set(api005.img_OUTS ${root_BINARY_DIR}/projects/29_day/harib26c/apilib/api005.img)
set(api005_wine.img_OUTS ${root_BINARY_DIR}/projects/29_day/harib26c/apilib/api005_wine.img)

add_custom_target(29_day_harib26c_apilib_api005
  COMMAND ${root_BINARY_DIR}/src/opennask ${api005.img_SRCS} ${api005.img_OUTS}
)
set(api005.img_HRB ${root_BINARY_DIR}/projects/29_day/harib26c/apilib/api005.hrb)
add_custom_target(29_day_harib26c_apilib_api005_hrb
  COMMAND ${root_BINARY_DIR}/src/opennask ${api005.img_SRCS} ${api005.img_HRB}
  COMMAND mcopy -i ${29_day_harib26c_apilib_OS} ${api005.img_HRB} ::api005.hrb
  DEPENDS 29_day_harib26c_apilib_img
)

add_dependencies(29_day_harib26c_apilib_run 29_day_harib26c_apilib_api005_hrb)
add_custom_target(29_day_harib26c_apilib_api005_wine
  COMMAND ${WINE} ${WINE_NASK} ${api005.img_SRCS} ${api005_wine.img_OUTS}
)
add_custom_target(29_day_harib26c_apilib_api005_od
  COMMAND ${OD} -t x1 ${api005.img_OUTS}      > ${api005.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${api005_wine.img_OUTS} > ${api005_wine.img_OUTS}_t.txt
  COMMAND diff -s ${api005.img_OUTS}_f.txt ${api005_wine.img_OUTS}_t.txt
)

add_dependencies(29_day_harib26c_apilib_api005_od 29_day_harib26c_apilib_api005)
add_dependencies(29_day_harib26c_apilib_api005_od 29_day_harib26c_apilib_api005_wine)

add_dependencies(images 29_day_harib26c_apilib_api005)
add_dependencies(wine 29_day_harib26c_apilib_api005_wine)
add_dependencies(od 29_day_harib26c_apilib_api005_od)

#----------------------------------------------------------
########### next target ###############
set(api006.img_SRCS ${root_SOURCE_DIR}/projects/29_day/harib26c/apilib/api006.nas)
set(api006.img_OUTS ${root_BINARY_DIR}/projects/29_day/harib26c/apilib/api006.img)
set(api006_wine.img_OUTS ${root_BINARY_DIR}/projects/29_day/harib26c/apilib/api006_wine.img)

add_custom_target(29_day_harib26c_apilib_api006
  COMMAND ${root_BINARY_DIR}/src/opennask ${api006.img_SRCS} ${api006.img_OUTS}
)
set(api006.img_HRB ${root_BINARY_DIR}/projects/29_day/harib26c/apilib/api006.hrb)
add_custom_target(29_day_harib26c_apilib_api006_hrb
  COMMAND ${root_BINARY_DIR}/src/opennask ${api006.img_SRCS} ${api006.img_HRB}
  COMMAND mcopy -i ${29_day_harib26c_apilib_OS} ${api006.img_HRB} ::api006.hrb
  DEPENDS 29_day_harib26c_apilib_img
)

add_dependencies(29_day_harib26c_apilib_run 29_day_harib26c_apilib_api006_hrb)
add_custom_target(29_day_harib26c_apilib_api006_wine
  COMMAND ${WINE} ${WINE_NASK} ${api006.img_SRCS} ${api006_wine.img_OUTS}
)
add_custom_target(29_day_harib26c_apilib_api006_od
  COMMAND ${OD} -t x1 ${api006.img_OUTS}      > ${api006.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${api006_wine.img_OUTS} > ${api006_wine.img_OUTS}_t.txt
  COMMAND diff -s ${api006.img_OUTS}_f.txt ${api006_wine.img_OUTS}_t.txt
)

add_dependencies(29_day_harib26c_apilib_api006_od 29_day_harib26c_apilib_api006)
add_dependencies(29_day_harib26c_apilib_api006_od 29_day_harib26c_apilib_api006_wine)

add_dependencies(images 29_day_harib26c_apilib_api006)
add_dependencies(wine 29_day_harib26c_apilib_api006_wine)
add_dependencies(od 29_day_harib26c_apilib_api006_od)

#----------------------------------------------------------
########### next target ###############
set(api007.img_SRCS ${root_SOURCE_DIR}/projects/29_day/harib26c/apilib/api007.nas)
set(api007.img_OUTS ${root_BINARY_DIR}/projects/29_day/harib26c/apilib/api007.img)
set(api007_wine.img_OUTS ${root_BINARY_DIR}/projects/29_day/harib26c/apilib/api007_wine.img)

add_custom_target(29_day_harib26c_apilib_api007
  COMMAND ${root_BINARY_DIR}/src/opennask ${api007.img_SRCS} ${api007.img_OUTS}
)
set(api007.img_HRB ${root_BINARY_DIR}/projects/29_day/harib26c/apilib/api007.hrb)
add_custom_target(29_day_harib26c_apilib_api007_hrb
  COMMAND ${root_BINARY_DIR}/src/opennask ${api007.img_SRCS} ${api007.img_HRB}
  COMMAND mcopy -i ${29_day_harib26c_apilib_OS} ${api007.img_HRB} ::api007.hrb
  DEPENDS 29_day_harib26c_apilib_img
)

add_dependencies(29_day_harib26c_apilib_run 29_day_harib26c_apilib_api007_hrb)
add_custom_target(29_day_harib26c_apilib_api007_wine
  COMMAND ${WINE} ${WINE_NASK} ${api007.img_SRCS} ${api007_wine.img_OUTS}
)
add_custom_target(29_day_harib26c_apilib_api007_od
  COMMAND ${OD} -t x1 ${api007.img_OUTS}      > ${api007.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${api007_wine.img_OUTS} > ${api007_wine.img_OUTS}_t.txt
  COMMAND diff -s ${api007.img_OUTS}_f.txt ${api007_wine.img_OUTS}_t.txt
)

add_dependencies(29_day_harib26c_apilib_api007_od 29_day_harib26c_apilib_api007)
add_dependencies(29_day_harib26c_apilib_api007_od 29_day_harib26c_apilib_api007_wine)

add_dependencies(images 29_day_harib26c_apilib_api007)
add_dependencies(wine 29_day_harib26c_apilib_api007_wine)
add_dependencies(od 29_day_harib26c_apilib_api007_od)

#----------------------------------------------------------
########### next target ###############
set(api008.img_SRCS ${root_SOURCE_DIR}/projects/29_day/harib26c/apilib/api008.nas)
set(api008.img_OUTS ${root_BINARY_DIR}/projects/29_day/harib26c/apilib/api008.img)
set(api008_wine.img_OUTS ${root_BINARY_DIR}/projects/29_day/harib26c/apilib/api008_wine.img)

add_custom_target(29_day_harib26c_apilib_api008
  COMMAND ${root_BINARY_DIR}/src/opennask ${api008.img_SRCS} ${api008.img_OUTS}
)
set(api008.img_HRB ${root_BINARY_DIR}/projects/29_day/harib26c/apilib/api008.hrb)
add_custom_target(29_day_harib26c_apilib_api008_hrb
  COMMAND ${root_BINARY_DIR}/src/opennask ${api008.img_SRCS} ${api008.img_HRB}
  COMMAND mcopy -i ${29_day_harib26c_apilib_OS} ${api008.img_HRB} ::api008.hrb
  DEPENDS 29_day_harib26c_apilib_img
)

add_dependencies(29_day_harib26c_apilib_run 29_day_harib26c_apilib_api008_hrb)
add_custom_target(29_day_harib26c_apilib_api008_wine
  COMMAND ${WINE} ${WINE_NASK} ${api008.img_SRCS} ${api008_wine.img_OUTS}
)
add_custom_target(29_day_harib26c_apilib_api008_od
  COMMAND ${OD} -t x1 ${api008.img_OUTS}      > ${api008.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${api008_wine.img_OUTS} > ${api008_wine.img_OUTS}_t.txt
  COMMAND diff -s ${api008.img_OUTS}_f.txt ${api008_wine.img_OUTS}_t.txt
)

add_dependencies(29_day_harib26c_apilib_api008_od 29_day_harib26c_apilib_api008)
add_dependencies(29_day_harib26c_apilib_api008_od 29_day_harib26c_apilib_api008_wine)

add_dependencies(images 29_day_harib26c_apilib_api008)
add_dependencies(wine 29_day_harib26c_apilib_api008_wine)
add_dependencies(od 29_day_harib26c_apilib_api008_od)

#----------------------------------------------------------
########### next target ###############
set(api009.img_SRCS ${root_SOURCE_DIR}/projects/29_day/harib26c/apilib/api009.nas)
set(api009.img_OUTS ${root_BINARY_DIR}/projects/29_day/harib26c/apilib/api009.img)
set(api009_wine.img_OUTS ${root_BINARY_DIR}/projects/29_day/harib26c/apilib/api009_wine.img)

add_custom_target(29_day_harib26c_apilib_api009
  COMMAND ${root_BINARY_DIR}/src/opennask ${api009.img_SRCS} ${api009.img_OUTS}
)
set(api009.img_HRB ${root_BINARY_DIR}/projects/29_day/harib26c/apilib/api009.hrb)
add_custom_target(29_day_harib26c_apilib_api009_hrb
  COMMAND ${root_BINARY_DIR}/src/opennask ${api009.img_SRCS} ${api009.img_HRB}
  COMMAND mcopy -i ${29_day_harib26c_apilib_OS} ${api009.img_HRB} ::api009.hrb
  DEPENDS 29_day_harib26c_apilib_img
)

add_dependencies(29_day_harib26c_apilib_run 29_day_harib26c_apilib_api009_hrb)
add_custom_target(29_day_harib26c_apilib_api009_wine
  COMMAND ${WINE} ${WINE_NASK} ${api009.img_SRCS} ${api009_wine.img_OUTS}
)
add_custom_target(29_day_harib26c_apilib_api009_od
  COMMAND ${OD} -t x1 ${api009.img_OUTS}      > ${api009.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${api009_wine.img_OUTS} > ${api009_wine.img_OUTS}_t.txt
  COMMAND diff -s ${api009.img_OUTS}_f.txt ${api009_wine.img_OUTS}_t.txt
)

add_dependencies(29_day_harib26c_apilib_api009_od 29_day_harib26c_apilib_api009)
add_dependencies(29_day_harib26c_apilib_api009_od 29_day_harib26c_apilib_api009_wine)

add_dependencies(images 29_day_harib26c_apilib_api009)
add_dependencies(wine 29_day_harib26c_apilib_api009_wine)
add_dependencies(od 29_day_harib26c_apilib_api009_od)

#----------------------------------------------------------
########### next target ###############
set(api010.img_SRCS ${root_SOURCE_DIR}/projects/29_day/harib26c/apilib/api010.nas)
set(api010.img_OUTS ${root_BINARY_DIR}/projects/29_day/harib26c/apilib/api010.img)
set(api010_wine.img_OUTS ${root_BINARY_DIR}/projects/29_day/harib26c/apilib/api010_wine.img)

add_custom_target(29_day_harib26c_apilib_api010
  COMMAND ${root_BINARY_DIR}/src/opennask ${api010.img_SRCS} ${api010.img_OUTS}
)
set(api010.img_HRB ${root_BINARY_DIR}/projects/29_day/harib26c/apilib/api010.hrb)
add_custom_target(29_day_harib26c_apilib_api010_hrb
  COMMAND ${root_BINARY_DIR}/src/opennask ${api010.img_SRCS} ${api010.img_HRB}
  COMMAND mcopy -i ${29_day_harib26c_apilib_OS} ${api010.img_HRB} ::api010.hrb
  DEPENDS 29_day_harib26c_apilib_img
)

add_dependencies(29_day_harib26c_apilib_run 29_day_harib26c_apilib_api010_hrb)
add_custom_target(29_day_harib26c_apilib_api010_wine
  COMMAND ${WINE} ${WINE_NASK} ${api010.img_SRCS} ${api010_wine.img_OUTS}
)
add_custom_target(29_day_harib26c_apilib_api010_od
  COMMAND ${OD} -t x1 ${api010.img_OUTS}      > ${api010.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${api010_wine.img_OUTS} > ${api010_wine.img_OUTS}_t.txt
  COMMAND diff -s ${api010.img_OUTS}_f.txt ${api010_wine.img_OUTS}_t.txt
)

add_dependencies(29_day_harib26c_apilib_api010_od 29_day_harib26c_apilib_api010)
add_dependencies(29_day_harib26c_apilib_api010_od 29_day_harib26c_apilib_api010_wine)

add_dependencies(images 29_day_harib26c_apilib_api010)
add_dependencies(wine 29_day_harib26c_apilib_api010_wine)
add_dependencies(od 29_day_harib26c_apilib_api010_od)

#----------------------------------------------------------
########### next target ###############
set(api011.img_SRCS ${root_SOURCE_DIR}/projects/29_day/harib26c/apilib/api011.nas)
set(api011.img_OUTS ${root_BINARY_DIR}/projects/29_day/harib26c/apilib/api011.img)
set(api011_wine.img_OUTS ${root_BINARY_DIR}/projects/29_day/harib26c/apilib/api011_wine.img)

add_custom_target(29_day_harib26c_apilib_api011
  COMMAND ${root_BINARY_DIR}/src/opennask ${api011.img_SRCS} ${api011.img_OUTS}
)
set(api011.img_HRB ${root_BINARY_DIR}/projects/29_day/harib26c/apilib/api011.hrb)
add_custom_target(29_day_harib26c_apilib_api011_hrb
  COMMAND ${root_BINARY_DIR}/src/opennask ${api011.img_SRCS} ${api011.img_HRB}
  COMMAND mcopy -i ${29_day_harib26c_apilib_OS} ${api011.img_HRB} ::api011.hrb
  DEPENDS 29_day_harib26c_apilib_img
)

add_dependencies(29_day_harib26c_apilib_run 29_day_harib26c_apilib_api011_hrb)
add_custom_target(29_day_harib26c_apilib_api011_wine
  COMMAND ${WINE} ${WINE_NASK} ${api011.img_SRCS} ${api011_wine.img_OUTS}
)
add_custom_target(29_day_harib26c_apilib_api011_od
  COMMAND ${OD} -t x1 ${api011.img_OUTS}      > ${api011.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${api011_wine.img_OUTS} > ${api011_wine.img_OUTS}_t.txt
  COMMAND diff -s ${api011.img_OUTS}_f.txt ${api011_wine.img_OUTS}_t.txt
)

add_dependencies(29_day_harib26c_apilib_api011_od 29_day_harib26c_apilib_api011)
add_dependencies(29_day_harib26c_apilib_api011_od 29_day_harib26c_apilib_api011_wine)

add_dependencies(images 29_day_harib26c_apilib_api011)
add_dependencies(wine 29_day_harib26c_apilib_api011_wine)
add_dependencies(od 29_day_harib26c_apilib_api011_od)

#----------------------------------------------------------
########### next target ###############
set(api012.img_SRCS ${root_SOURCE_DIR}/projects/29_day/harib26c/apilib/api012.nas)
set(api012.img_OUTS ${root_BINARY_DIR}/projects/29_day/harib26c/apilib/api012.img)
set(api012_wine.img_OUTS ${root_BINARY_DIR}/projects/29_day/harib26c/apilib/api012_wine.img)

add_custom_target(29_day_harib26c_apilib_api012
  COMMAND ${root_BINARY_DIR}/src/opennask ${api012.img_SRCS} ${api012.img_OUTS}
)
set(api012.img_HRB ${root_BINARY_DIR}/projects/29_day/harib26c/apilib/api012.hrb)
add_custom_target(29_day_harib26c_apilib_api012_hrb
  COMMAND ${root_BINARY_DIR}/src/opennask ${api012.img_SRCS} ${api012.img_HRB}
  COMMAND mcopy -i ${29_day_harib26c_apilib_OS} ${api012.img_HRB} ::api012.hrb
  DEPENDS 29_day_harib26c_apilib_img
)

add_dependencies(29_day_harib26c_apilib_run 29_day_harib26c_apilib_api012_hrb)
add_custom_target(29_day_harib26c_apilib_api012_wine
  COMMAND ${WINE} ${WINE_NASK} ${api012.img_SRCS} ${api012_wine.img_OUTS}
)
add_custom_target(29_day_harib26c_apilib_api012_od
  COMMAND ${OD} -t x1 ${api012.img_OUTS}      > ${api012.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${api012_wine.img_OUTS} > ${api012_wine.img_OUTS}_t.txt
  COMMAND diff -s ${api012.img_OUTS}_f.txt ${api012_wine.img_OUTS}_t.txt
)

add_dependencies(29_day_harib26c_apilib_api012_od 29_day_harib26c_apilib_api012)
add_dependencies(29_day_harib26c_apilib_api012_od 29_day_harib26c_apilib_api012_wine)

add_dependencies(images 29_day_harib26c_apilib_api012)
add_dependencies(wine 29_day_harib26c_apilib_api012_wine)
add_dependencies(od 29_day_harib26c_apilib_api012_od)

#----------------------------------------------------------
########### next target ###############
set(api013.img_SRCS ${root_SOURCE_DIR}/projects/29_day/harib26c/apilib/api013.nas)
set(api013.img_OUTS ${root_BINARY_DIR}/projects/29_day/harib26c/apilib/api013.img)
set(api013_wine.img_OUTS ${root_BINARY_DIR}/projects/29_day/harib26c/apilib/api013_wine.img)

add_custom_target(29_day_harib26c_apilib_api013
  COMMAND ${root_BINARY_DIR}/src/opennask ${api013.img_SRCS} ${api013.img_OUTS}
)
set(api013.img_HRB ${root_BINARY_DIR}/projects/29_day/harib26c/apilib/api013.hrb)
add_custom_target(29_day_harib26c_apilib_api013_hrb
  COMMAND ${root_BINARY_DIR}/src/opennask ${api013.img_SRCS} ${api013.img_HRB}
  COMMAND mcopy -i ${29_day_harib26c_apilib_OS} ${api013.img_HRB} ::api013.hrb
  DEPENDS 29_day_harib26c_apilib_img
)

add_dependencies(29_day_harib26c_apilib_run 29_day_harib26c_apilib_api013_hrb)
add_custom_target(29_day_harib26c_apilib_api013_wine
  COMMAND ${WINE} ${WINE_NASK} ${api013.img_SRCS} ${api013_wine.img_OUTS}
)
add_custom_target(29_day_harib26c_apilib_api013_od
  COMMAND ${OD} -t x1 ${api013.img_OUTS}      > ${api013.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${api013_wine.img_OUTS} > ${api013_wine.img_OUTS}_t.txt
  COMMAND diff -s ${api013.img_OUTS}_f.txt ${api013_wine.img_OUTS}_t.txt
)

add_dependencies(29_day_harib26c_apilib_api013_od 29_day_harib26c_apilib_api013)
add_dependencies(29_day_harib26c_apilib_api013_od 29_day_harib26c_apilib_api013_wine)

add_dependencies(images 29_day_harib26c_apilib_api013)
add_dependencies(wine 29_day_harib26c_apilib_api013_wine)
add_dependencies(od 29_day_harib26c_apilib_api013_od)

#----------------------------------------------------------
########### next target ###############
set(api014.img_SRCS ${root_SOURCE_DIR}/projects/29_day/harib26c/apilib/api014.nas)
set(api014.img_OUTS ${root_BINARY_DIR}/projects/29_day/harib26c/apilib/api014.img)
set(api014_wine.img_OUTS ${root_BINARY_DIR}/projects/29_day/harib26c/apilib/api014_wine.img)

add_custom_target(29_day_harib26c_apilib_api014
  COMMAND ${root_BINARY_DIR}/src/opennask ${api014.img_SRCS} ${api014.img_OUTS}
)
set(api014.img_HRB ${root_BINARY_DIR}/projects/29_day/harib26c/apilib/api014.hrb)
add_custom_target(29_day_harib26c_apilib_api014_hrb
  COMMAND ${root_BINARY_DIR}/src/opennask ${api014.img_SRCS} ${api014.img_HRB}
  COMMAND mcopy -i ${29_day_harib26c_apilib_OS} ${api014.img_HRB} ::api014.hrb
  DEPENDS 29_day_harib26c_apilib_img
)

add_dependencies(29_day_harib26c_apilib_run 29_day_harib26c_apilib_api014_hrb)
add_custom_target(29_day_harib26c_apilib_api014_wine
  COMMAND ${WINE} ${WINE_NASK} ${api014.img_SRCS} ${api014_wine.img_OUTS}
)
add_custom_target(29_day_harib26c_apilib_api014_od
  COMMAND ${OD} -t x1 ${api014.img_OUTS}      > ${api014.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${api014_wine.img_OUTS} > ${api014_wine.img_OUTS}_t.txt
  COMMAND diff -s ${api014.img_OUTS}_f.txt ${api014_wine.img_OUTS}_t.txt
)

add_dependencies(29_day_harib26c_apilib_api014_od 29_day_harib26c_apilib_api014)
add_dependencies(29_day_harib26c_apilib_api014_od 29_day_harib26c_apilib_api014_wine)

add_dependencies(images 29_day_harib26c_apilib_api014)
add_dependencies(wine 29_day_harib26c_apilib_api014_wine)
add_dependencies(od 29_day_harib26c_apilib_api014_od)

#----------------------------------------------------------
########### next target ###############
set(api015.img_SRCS ${root_SOURCE_DIR}/projects/29_day/harib26c/apilib/api015.nas)
set(api015.img_OUTS ${root_BINARY_DIR}/projects/29_day/harib26c/apilib/api015.img)
set(api015_wine.img_OUTS ${root_BINARY_DIR}/projects/29_day/harib26c/apilib/api015_wine.img)

add_custom_target(29_day_harib26c_apilib_api015
  COMMAND ${root_BINARY_DIR}/src/opennask ${api015.img_SRCS} ${api015.img_OUTS}
)
set(api015.img_HRB ${root_BINARY_DIR}/projects/29_day/harib26c/apilib/api015.hrb)
add_custom_target(29_day_harib26c_apilib_api015_hrb
  COMMAND ${root_BINARY_DIR}/src/opennask ${api015.img_SRCS} ${api015.img_HRB}
  COMMAND mcopy -i ${29_day_harib26c_apilib_OS} ${api015.img_HRB} ::api015.hrb
  DEPENDS 29_day_harib26c_apilib_img
)

add_dependencies(29_day_harib26c_apilib_run 29_day_harib26c_apilib_api015_hrb)
add_custom_target(29_day_harib26c_apilib_api015_wine
  COMMAND ${WINE} ${WINE_NASK} ${api015.img_SRCS} ${api015_wine.img_OUTS}
)
add_custom_target(29_day_harib26c_apilib_api015_od
  COMMAND ${OD} -t x1 ${api015.img_OUTS}      > ${api015.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${api015_wine.img_OUTS} > ${api015_wine.img_OUTS}_t.txt
  COMMAND diff -s ${api015.img_OUTS}_f.txt ${api015_wine.img_OUTS}_t.txt
)

add_dependencies(29_day_harib26c_apilib_api015_od 29_day_harib26c_apilib_api015)
add_dependencies(29_day_harib26c_apilib_api015_od 29_day_harib26c_apilib_api015_wine)

add_dependencies(images 29_day_harib26c_apilib_api015)
add_dependencies(wine 29_day_harib26c_apilib_api015_wine)
add_dependencies(od 29_day_harib26c_apilib_api015_od)

#----------------------------------------------------------
########### next target ###############
set(api016.img_SRCS ${root_SOURCE_DIR}/projects/29_day/harib26c/apilib/api016.nas)
set(api016.img_OUTS ${root_BINARY_DIR}/projects/29_day/harib26c/apilib/api016.img)
set(api016_wine.img_OUTS ${root_BINARY_DIR}/projects/29_day/harib26c/apilib/api016_wine.img)

add_custom_target(29_day_harib26c_apilib_api016
  COMMAND ${root_BINARY_DIR}/src/opennask ${api016.img_SRCS} ${api016.img_OUTS}
)
set(api016.img_HRB ${root_BINARY_DIR}/projects/29_day/harib26c/apilib/api016.hrb)
add_custom_target(29_day_harib26c_apilib_api016_hrb
  COMMAND ${root_BINARY_DIR}/src/opennask ${api016.img_SRCS} ${api016.img_HRB}
  COMMAND mcopy -i ${29_day_harib26c_apilib_OS} ${api016.img_HRB} ::api016.hrb
  DEPENDS 29_day_harib26c_apilib_img
)

add_dependencies(29_day_harib26c_apilib_run 29_day_harib26c_apilib_api016_hrb)
add_custom_target(29_day_harib26c_apilib_api016_wine
  COMMAND ${WINE} ${WINE_NASK} ${api016.img_SRCS} ${api016_wine.img_OUTS}
)
add_custom_target(29_day_harib26c_apilib_api016_od
  COMMAND ${OD} -t x1 ${api016.img_OUTS}      > ${api016.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${api016_wine.img_OUTS} > ${api016_wine.img_OUTS}_t.txt
  COMMAND diff -s ${api016.img_OUTS}_f.txt ${api016_wine.img_OUTS}_t.txt
)

add_dependencies(29_day_harib26c_apilib_api016_od 29_day_harib26c_apilib_api016)
add_dependencies(29_day_harib26c_apilib_api016_od 29_day_harib26c_apilib_api016_wine)

add_dependencies(images 29_day_harib26c_apilib_api016)
add_dependencies(wine 29_day_harib26c_apilib_api016_wine)
add_dependencies(od 29_day_harib26c_apilib_api016_od)

#----------------------------------------------------------
########### next target ###############
set(api017.img_SRCS ${root_SOURCE_DIR}/projects/29_day/harib26c/apilib/api017.nas)
set(api017.img_OUTS ${root_BINARY_DIR}/projects/29_day/harib26c/apilib/api017.img)
set(api017_wine.img_OUTS ${root_BINARY_DIR}/projects/29_day/harib26c/apilib/api017_wine.img)

add_custom_target(29_day_harib26c_apilib_api017
  COMMAND ${root_BINARY_DIR}/src/opennask ${api017.img_SRCS} ${api017.img_OUTS}
)
set(api017.img_HRB ${root_BINARY_DIR}/projects/29_day/harib26c/apilib/api017.hrb)
add_custom_target(29_day_harib26c_apilib_api017_hrb
  COMMAND ${root_BINARY_DIR}/src/opennask ${api017.img_SRCS} ${api017.img_HRB}
  COMMAND mcopy -i ${29_day_harib26c_apilib_OS} ${api017.img_HRB} ::api017.hrb
  DEPENDS 29_day_harib26c_apilib_img
)

add_dependencies(29_day_harib26c_apilib_run 29_day_harib26c_apilib_api017_hrb)
add_custom_target(29_day_harib26c_apilib_api017_wine
  COMMAND ${WINE} ${WINE_NASK} ${api017.img_SRCS} ${api017_wine.img_OUTS}
)
add_custom_target(29_day_harib26c_apilib_api017_od
  COMMAND ${OD} -t x1 ${api017.img_OUTS}      > ${api017.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${api017_wine.img_OUTS} > ${api017_wine.img_OUTS}_t.txt
  COMMAND diff -s ${api017.img_OUTS}_f.txt ${api017_wine.img_OUTS}_t.txt
)

add_dependencies(29_day_harib26c_apilib_api017_od 29_day_harib26c_apilib_api017)
add_dependencies(29_day_harib26c_apilib_api017_od 29_day_harib26c_apilib_api017_wine)

add_dependencies(images 29_day_harib26c_apilib_api017)
add_dependencies(wine 29_day_harib26c_apilib_api017_wine)
add_dependencies(od 29_day_harib26c_apilib_api017_od)

#----------------------------------------------------------
########### next target ###############
set(api018.img_SRCS ${root_SOURCE_DIR}/projects/29_day/harib26c/apilib/api018.nas)
set(api018.img_OUTS ${root_BINARY_DIR}/projects/29_day/harib26c/apilib/api018.img)
set(api018_wine.img_OUTS ${root_BINARY_DIR}/projects/29_day/harib26c/apilib/api018_wine.img)

add_custom_target(29_day_harib26c_apilib_api018
  COMMAND ${root_BINARY_DIR}/src/opennask ${api018.img_SRCS} ${api018.img_OUTS}
)
set(api018.img_HRB ${root_BINARY_DIR}/projects/29_day/harib26c/apilib/api018.hrb)
add_custom_target(29_day_harib26c_apilib_api018_hrb
  COMMAND ${root_BINARY_DIR}/src/opennask ${api018.img_SRCS} ${api018.img_HRB}
  COMMAND mcopy -i ${29_day_harib26c_apilib_OS} ${api018.img_HRB} ::api018.hrb
  DEPENDS 29_day_harib26c_apilib_img
)

add_dependencies(29_day_harib26c_apilib_run 29_day_harib26c_apilib_api018_hrb)
add_custom_target(29_day_harib26c_apilib_api018_wine
  COMMAND ${WINE} ${WINE_NASK} ${api018.img_SRCS} ${api018_wine.img_OUTS}
)
add_custom_target(29_day_harib26c_apilib_api018_od
  COMMAND ${OD} -t x1 ${api018.img_OUTS}      > ${api018.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${api018_wine.img_OUTS} > ${api018_wine.img_OUTS}_t.txt
  COMMAND diff -s ${api018.img_OUTS}_f.txt ${api018_wine.img_OUTS}_t.txt
)

add_dependencies(29_day_harib26c_apilib_api018_od 29_day_harib26c_apilib_api018)
add_dependencies(29_day_harib26c_apilib_api018_od 29_day_harib26c_apilib_api018_wine)

add_dependencies(images 29_day_harib26c_apilib_api018)
add_dependencies(wine 29_day_harib26c_apilib_api018_wine)
add_dependencies(od 29_day_harib26c_apilib_api018_od)

#----------------------------------------------------------
########### next target ###############
set(api019.img_SRCS ${root_SOURCE_DIR}/projects/29_day/harib26c/apilib/api019.nas)
set(api019.img_OUTS ${root_BINARY_DIR}/projects/29_day/harib26c/apilib/api019.img)
set(api019_wine.img_OUTS ${root_BINARY_DIR}/projects/29_day/harib26c/apilib/api019_wine.img)

add_custom_target(29_day_harib26c_apilib_api019
  COMMAND ${root_BINARY_DIR}/src/opennask ${api019.img_SRCS} ${api019.img_OUTS}
)
set(api019.img_HRB ${root_BINARY_DIR}/projects/29_day/harib26c/apilib/api019.hrb)
add_custom_target(29_day_harib26c_apilib_api019_hrb
  COMMAND ${root_BINARY_DIR}/src/opennask ${api019.img_SRCS} ${api019.img_HRB}
  COMMAND mcopy -i ${29_day_harib26c_apilib_OS} ${api019.img_HRB} ::api019.hrb
  DEPENDS 29_day_harib26c_apilib_img
)

add_dependencies(29_day_harib26c_apilib_run 29_day_harib26c_apilib_api019_hrb)
add_custom_target(29_day_harib26c_apilib_api019_wine
  COMMAND ${WINE} ${WINE_NASK} ${api019.img_SRCS} ${api019_wine.img_OUTS}
)
add_custom_target(29_day_harib26c_apilib_api019_od
  COMMAND ${OD} -t x1 ${api019.img_OUTS}      > ${api019.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${api019_wine.img_OUTS} > ${api019_wine.img_OUTS}_t.txt
  COMMAND diff -s ${api019.img_OUTS}_f.txt ${api019_wine.img_OUTS}_t.txt
)

add_dependencies(29_day_harib26c_apilib_api019_od 29_day_harib26c_apilib_api019)
add_dependencies(29_day_harib26c_apilib_api019_od 29_day_harib26c_apilib_api019_wine)

add_dependencies(images 29_day_harib26c_apilib_api019)
add_dependencies(wine 29_day_harib26c_apilib_api019_wine)
add_dependencies(od 29_day_harib26c_apilib_api019_od)

#----------------------------------------------------------
########### next target ###############
set(api020.img_SRCS ${root_SOURCE_DIR}/projects/29_day/harib26c/apilib/api020.nas)
set(api020.img_OUTS ${root_BINARY_DIR}/projects/29_day/harib26c/apilib/api020.img)
set(api020_wine.img_OUTS ${root_BINARY_DIR}/projects/29_day/harib26c/apilib/api020_wine.img)

add_custom_target(29_day_harib26c_apilib_api020
  COMMAND ${root_BINARY_DIR}/src/opennask ${api020.img_SRCS} ${api020.img_OUTS}
)
set(api020.img_HRB ${root_BINARY_DIR}/projects/29_day/harib26c/apilib/api020.hrb)
add_custom_target(29_day_harib26c_apilib_api020_hrb
  COMMAND ${root_BINARY_DIR}/src/opennask ${api020.img_SRCS} ${api020.img_HRB}
  COMMAND mcopy -i ${29_day_harib26c_apilib_OS} ${api020.img_HRB} ::api020.hrb
  DEPENDS 29_day_harib26c_apilib_img
)

add_dependencies(29_day_harib26c_apilib_run 29_day_harib26c_apilib_api020_hrb)
add_custom_target(29_day_harib26c_apilib_api020_wine
  COMMAND ${WINE} ${WINE_NASK} ${api020.img_SRCS} ${api020_wine.img_OUTS}
)
add_custom_target(29_day_harib26c_apilib_api020_od
  COMMAND ${OD} -t x1 ${api020.img_OUTS}      > ${api020.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${api020_wine.img_OUTS} > ${api020_wine.img_OUTS}_t.txt
  COMMAND diff -s ${api020.img_OUTS}_f.txt ${api020_wine.img_OUTS}_t.txt
)

add_dependencies(29_day_harib26c_apilib_api020_od 29_day_harib26c_apilib_api020)
add_dependencies(29_day_harib26c_apilib_api020_od 29_day_harib26c_apilib_api020_wine)

add_dependencies(images 29_day_harib26c_apilib_api020)
add_dependencies(wine 29_day_harib26c_apilib_api020_wine)
add_dependencies(od 29_day_harib26c_apilib_api020_od)

#----------------------------------------------------------
########### next target ###############
set(api021.img_SRCS ${root_SOURCE_DIR}/projects/29_day/harib26c/apilib/api021.nas)
set(api021.img_OUTS ${root_BINARY_DIR}/projects/29_day/harib26c/apilib/api021.img)
set(api021_wine.img_OUTS ${root_BINARY_DIR}/projects/29_day/harib26c/apilib/api021_wine.img)

add_custom_target(29_day_harib26c_apilib_api021
  COMMAND ${root_BINARY_DIR}/src/opennask ${api021.img_SRCS} ${api021.img_OUTS}
)
set(api021.img_HRB ${root_BINARY_DIR}/projects/29_day/harib26c/apilib/api021.hrb)
add_custom_target(29_day_harib26c_apilib_api021_hrb
  COMMAND ${root_BINARY_DIR}/src/opennask ${api021.img_SRCS} ${api021.img_HRB}
  COMMAND mcopy -i ${29_day_harib26c_apilib_OS} ${api021.img_HRB} ::api021.hrb
  DEPENDS 29_day_harib26c_apilib_img
)

add_dependencies(29_day_harib26c_apilib_run 29_day_harib26c_apilib_api021_hrb)
add_custom_target(29_day_harib26c_apilib_api021_wine
  COMMAND ${WINE} ${WINE_NASK} ${api021.img_SRCS} ${api021_wine.img_OUTS}
)
add_custom_target(29_day_harib26c_apilib_api021_od
  COMMAND ${OD} -t x1 ${api021.img_OUTS}      > ${api021.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${api021_wine.img_OUTS} > ${api021_wine.img_OUTS}_t.txt
  COMMAND diff -s ${api021.img_OUTS}_f.txt ${api021_wine.img_OUTS}_t.txt
)

add_dependencies(29_day_harib26c_apilib_api021_od 29_day_harib26c_apilib_api021)
add_dependencies(29_day_harib26c_apilib_api021_od 29_day_harib26c_apilib_api021_wine)

add_dependencies(images 29_day_harib26c_apilib_api021)
add_dependencies(wine 29_day_harib26c_apilib_api021_wine)
add_dependencies(od 29_day_harib26c_apilib_api021_od)

#----------------------------------------------------------
########### next target ###############
set(api022.img_SRCS ${root_SOURCE_DIR}/projects/29_day/harib26c/apilib/api022.nas)
set(api022.img_OUTS ${root_BINARY_DIR}/projects/29_day/harib26c/apilib/api022.img)
set(api022_wine.img_OUTS ${root_BINARY_DIR}/projects/29_day/harib26c/apilib/api022_wine.img)

add_custom_target(29_day_harib26c_apilib_api022
  COMMAND ${root_BINARY_DIR}/src/opennask ${api022.img_SRCS} ${api022.img_OUTS}
)
set(api022.img_HRB ${root_BINARY_DIR}/projects/29_day/harib26c/apilib/api022.hrb)
add_custom_target(29_day_harib26c_apilib_api022_hrb
  COMMAND ${root_BINARY_DIR}/src/opennask ${api022.img_SRCS} ${api022.img_HRB}
  COMMAND mcopy -i ${29_day_harib26c_apilib_OS} ${api022.img_HRB} ::api022.hrb
  DEPENDS 29_day_harib26c_apilib_img
)

add_dependencies(29_day_harib26c_apilib_run 29_day_harib26c_apilib_api022_hrb)
add_custom_target(29_day_harib26c_apilib_api022_wine
  COMMAND ${WINE} ${WINE_NASK} ${api022.img_SRCS} ${api022_wine.img_OUTS}
)
add_custom_target(29_day_harib26c_apilib_api022_od
  COMMAND ${OD} -t x1 ${api022.img_OUTS}      > ${api022.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${api022_wine.img_OUTS} > ${api022_wine.img_OUTS}_t.txt
  COMMAND diff -s ${api022.img_OUTS}_f.txt ${api022_wine.img_OUTS}_t.txt
)

add_dependencies(29_day_harib26c_apilib_api022_od 29_day_harib26c_apilib_api022)
add_dependencies(29_day_harib26c_apilib_api022_od 29_day_harib26c_apilib_api022_wine)

add_dependencies(images 29_day_harib26c_apilib_api022)
add_dependencies(wine 29_day_harib26c_apilib_api022_wine)
add_dependencies(od 29_day_harib26c_apilib_api022_od)

#----------------------------------------------------------
########### next target ###############
set(api023.img_SRCS ${root_SOURCE_DIR}/projects/29_day/harib26c/apilib/api023.nas)
set(api023.img_OUTS ${root_BINARY_DIR}/projects/29_day/harib26c/apilib/api023.img)
set(api023_wine.img_OUTS ${root_BINARY_DIR}/projects/29_day/harib26c/apilib/api023_wine.img)

add_custom_target(29_day_harib26c_apilib_api023
  COMMAND ${root_BINARY_DIR}/src/opennask ${api023.img_SRCS} ${api023.img_OUTS}
)
set(api023.img_HRB ${root_BINARY_DIR}/projects/29_day/harib26c/apilib/api023.hrb)
add_custom_target(29_day_harib26c_apilib_api023_hrb
  COMMAND ${root_BINARY_DIR}/src/opennask ${api023.img_SRCS} ${api023.img_HRB}
  COMMAND mcopy -i ${29_day_harib26c_apilib_OS} ${api023.img_HRB} ::api023.hrb
  DEPENDS 29_day_harib26c_apilib_img
)

add_dependencies(29_day_harib26c_apilib_run 29_day_harib26c_apilib_api023_hrb)
add_custom_target(29_day_harib26c_apilib_api023_wine
  COMMAND ${WINE} ${WINE_NASK} ${api023.img_SRCS} ${api023_wine.img_OUTS}
)
add_custom_target(29_day_harib26c_apilib_api023_od
  COMMAND ${OD} -t x1 ${api023.img_OUTS}      > ${api023.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${api023_wine.img_OUTS} > ${api023_wine.img_OUTS}_t.txt
  COMMAND diff -s ${api023.img_OUTS}_f.txt ${api023_wine.img_OUTS}_t.txt
)

add_dependencies(29_day_harib26c_apilib_api023_od 29_day_harib26c_apilib_api023)
add_dependencies(29_day_harib26c_apilib_api023_od 29_day_harib26c_apilib_api023_wine)

add_dependencies(images 29_day_harib26c_apilib_api023)
add_dependencies(wine 29_day_harib26c_apilib_api023_wine)
add_dependencies(od 29_day_harib26c_apilib_api023_od)

#----------------------------------------------------------
########### next target ###############
set(api024.img_SRCS ${root_SOURCE_DIR}/projects/29_day/harib26c/apilib/api024.nas)
set(api024.img_OUTS ${root_BINARY_DIR}/projects/29_day/harib26c/apilib/api024.img)
set(api024_wine.img_OUTS ${root_BINARY_DIR}/projects/29_day/harib26c/apilib/api024_wine.img)

add_custom_target(29_day_harib26c_apilib_api024
  COMMAND ${root_BINARY_DIR}/src/opennask ${api024.img_SRCS} ${api024.img_OUTS}
)
set(api024.img_HRB ${root_BINARY_DIR}/projects/29_day/harib26c/apilib/api024.hrb)
add_custom_target(29_day_harib26c_apilib_api024_hrb
  COMMAND ${root_BINARY_DIR}/src/opennask ${api024.img_SRCS} ${api024.img_HRB}
  COMMAND mcopy -i ${29_day_harib26c_apilib_OS} ${api024.img_HRB} ::api024.hrb
  DEPENDS 29_day_harib26c_apilib_img
)

add_dependencies(29_day_harib26c_apilib_run 29_day_harib26c_apilib_api024_hrb)
add_custom_target(29_day_harib26c_apilib_api024_wine
  COMMAND ${WINE} ${WINE_NASK} ${api024.img_SRCS} ${api024_wine.img_OUTS}
)
add_custom_target(29_day_harib26c_apilib_api024_od
  COMMAND ${OD} -t x1 ${api024.img_OUTS}      > ${api024.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${api024_wine.img_OUTS} > ${api024_wine.img_OUTS}_t.txt
  COMMAND diff -s ${api024.img_OUTS}_f.txt ${api024_wine.img_OUTS}_t.txt
)

add_dependencies(29_day_harib26c_apilib_api024_od 29_day_harib26c_apilib_api024)
add_dependencies(29_day_harib26c_apilib_api024_od 29_day_harib26c_apilib_api024_wine)

add_dependencies(images 29_day_harib26c_apilib_api024)
add_dependencies(wine 29_day_harib26c_apilib_api024_wine)
add_dependencies(od 29_day_harib26c_apilib_api024_od)

#----------------------------------------------------------
########### next target ###############
set(api025.img_SRCS ${root_SOURCE_DIR}/projects/29_day/harib26c/apilib/api025.nas)
set(api025.img_OUTS ${root_BINARY_DIR}/projects/29_day/harib26c/apilib/api025.img)
set(api025_wine.img_OUTS ${root_BINARY_DIR}/projects/29_day/harib26c/apilib/api025_wine.img)

add_custom_target(29_day_harib26c_apilib_api025
  COMMAND ${root_BINARY_DIR}/src/opennask ${api025.img_SRCS} ${api025.img_OUTS}
)
set(api025.img_HRB ${root_BINARY_DIR}/projects/29_day/harib26c/apilib/api025.hrb)
add_custom_target(29_day_harib26c_apilib_api025_hrb
  COMMAND ${root_BINARY_DIR}/src/opennask ${api025.img_SRCS} ${api025.img_HRB}
  COMMAND mcopy -i ${29_day_harib26c_apilib_OS} ${api025.img_HRB} ::api025.hrb
  DEPENDS 29_day_harib26c_apilib_img
)

add_dependencies(29_day_harib26c_apilib_run 29_day_harib26c_apilib_api025_hrb)
add_custom_target(29_day_harib26c_apilib_api025_wine
  COMMAND ${WINE} ${WINE_NASK} ${api025.img_SRCS} ${api025_wine.img_OUTS}
)
add_custom_target(29_day_harib26c_apilib_api025_od
  COMMAND ${OD} -t x1 ${api025.img_OUTS}      > ${api025.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${api025_wine.img_OUTS} > ${api025_wine.img_OUTS}_t.txt
  COMMAND diff -s ${api025.img_OUTS}_f.txt ${api025_wine.img_OUTS}_t.txt
)

add_dependencies(29_day_harib26c_apilib_api025_od 29_day_harib26c_apilib_api025)
add_dependencies(29_day_harib26c_apilib_api025_od 29_day_harib26c_apilib_api025_wine)

add_dependencies(images 29_day_harib26c_apilib_api025)
add_dependencies(wine 29_day_harib26c_apilib_api025_wine)
add_dependencies(od 29_day_harib26c_apilib_api025_od)

#----------------------------------------------------------
########### next target ###############
set(api026.img_SRCS ${root_SOURCE_DIR}/projects/29_day/harib26c/apilib/api026.nas)
set(api026.img_OUTS ${root_BINARY_DIR}/projects/29_day/harib26c/apilib/api026.img)
set(api026_wine.img_OUTS ${root_BINARY_DIR}/projects/29_day/harib26c/apilib/api026_wine.img)

add_custom_target(29_day_harib26c_apilib_api026
  COMMAND ${root_BINARY_DIR}/src/opennask ${api026.img_SRCS} ${api026.img_OUTS}
)
set(api026.img_HRB ${root_BINARY_DIR}/projects/29_day/harib26c/apilib/api026.hrb)
add_custom_target(29_day_harib26c_apilib_api026_hrb
  COMMAND ${root_BINARY_DIR}/src/opennask ${api026.img_SRCS} ${api026.img_HRB}
  COMMAND mcopy -i ${29_day_harib26c_apilib_OS} ${api026.img_HRB} ::api026.hrb
  DEPENDS 29_day_harib26c_apilib_img
)

add_dependencies(29_day_harib26c_apilib_run 29_day_harib26c_apilib_api026_hrb)
add_custom_target(29_day_harib26c_apilib_api026_wine
  COMMAND ${WINE} ${WINE_NASK} ${api026.img_SRCS} ${api026_wine.img_OUTS}
)
add_custom_target(29_day_harib26c_apilib_api026_od
  COMMAND ${OD} -t x1 ${api026.img_OUTS}      > ${api026.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${api026_wine.img_OUTS} > ${api026_wine.img_OUTS}_t.txt
  COMMAND diff -s ${api026.img_OUTS}_f.txt ${api026_wine.img_OUTS}_t.txt
)

add_dependencies(29_day_harib26c_apilib_api026_od 29_day_harib26c_apilib_api026)
add_dependencies(29_day_harib26c_apilib_api026_od 29_day_harib26c_apilib_api026_wine)

add_dependencies(images 29_day_harib26c_apilib_api026)
add_dependencies(wine 29_day_harib26c_apilib_api026_wine)
add_dependencies(od 29_day_harib26c_apilib_api026_od)

#----------------------------------------------------------
########### next target ###############
set(api027.img_SRCS ${root_SOURCE_DIR}/projects/29_day/harib26c/apilib/api027.nas)
set(api027.img_OUTS ${root_BINARY_DIR}/projects/29_day/harib26c/apilib/api027.img)
set(api027_wine.img_OUTS ${root_BINARY_DIR}/projects/29_day/harib26c/apilib/api027_wine.img)

add_custom_target(29_day_harib26c_apilib_api027
  COMMAND ${root_BINARY_DIR}/src/opennask ${api027.img_SRCS} ${api027.img_OUTS}
)
set(api027.img_HRB ${root_BINARY_DIR}/projects/29_day/harib26c/apilib/api027.hrb)
add_custom_target(29_day_harib26c_apilib_api027_hrb
  COMMAND ${root_BINARY_DIR}/src/opennask ${api027.img_SRCS} ${api027.img_HRB}
  COMMAND mcopy -i ${29_day_harib26c_apilib_OS} ${api027.img_HRB} ::api027.hrb
  DEPENDS 29_day_harib26c_apilib_img
)

add_dependencies(29_day_harib26c_apilib_run 29_day_harib26c_apilib_api027_hrb)
add_custom_target(29_day_harib26c_apilib_api027_wine
  COMMAND ${WINE} ${WINE_NASK} ${api027.img_SRCS} ${api027_wine.img_OUTS}
)
add_custom_target(29_day_harib26c_apilib_api027_od
  COMMAND ${OD} -t x1 ${api027.img_OUTS}      > ${api027.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${api027_wine.img_OUTS} > ${api027_wine.img_OUTS}_t.txt
  COMMAND diff -s ${api027.img_OUTS}_f.txt ${api027_wine.img_OUTS}_t.txt
)

add_dependencies(29_day_harib26c_apilib_api027_od 29_day_harib26c_apilib_api027)
add_dependencies(29_day_harib26c_apilib_api027_od 29_day_harib26c_apilib_api027_wine)

add_dependencies(images 29_day_harib26c_apilib_api027)
add_dependencies(wine 29_day_harib26c_apilib_api027_wine)
add_dependencies(od 29_day_harib26c_apilib_api027_od)

#----------------------------------------------------------
