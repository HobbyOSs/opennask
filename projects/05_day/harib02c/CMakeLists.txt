#----------------------------------------------------------
message(STATUS "Entering directory projects/05_day/harib02c/")

set(NASK ${root_BINARY_DIR}/src/opennask)
set(05_day_harib02c_OS    ${root_BINARY_DIR}/projects/05_day/harib02c/os.img)
set(05_day_harib02c_SYS	  ${root_BINARY_DIR}/projects/05_day/harib02c/os.sys)
set(05_day_harib02c_IPLB  ${root_BINARY_DIR}/projects/05_day/harib02c/ipl.bin)
set(05_day_harib02c_IPLS  ${root_SOURCE_DIR}/projects/05_day/harib02c/ipl10.nas)
set(05_day_harib02c_HEADB ${root_BINARY_DIR}/projects/05_day/harib02c/asmhead.bin)
set(05_day_harib02c_HEADS ${root_SOURCE_DIR}/projects/05_day/harib02c/asmhead.nas)
set(05_day_harib02c_BOOTB ${root_BINARY_DIR}/projects/05_day/harib02c/boot.bin)
set(05_day_harib02c_CCO	  ${root_BINARY_DIR}/projects/05_day/harib02c/boot.o)
set(05_day_harib02c_CCS	  ${root_SOURCE_DIR}/projects/05_day/harib02c/*.c)

add_custom_target(05_day_harib02c_run                                           
  COMMAND ${QEMU} ${QEMUOPT} ${05_day_harib02c_OS}				
)											
add_custom_target(05_day_harib02c_debug						
  COMMAND ${QEMU} -s -S ${QEMUOPT} ${05_day_harib02c_OS} -redir tcp:5555:127.0.0.1:1234 &
)											
add_custom_target(05_day_harib02c_ipl						
  COMMAND ${NASK} ${05_day_harib02c_IPLS} ${05_day_harib02c_IPLB}		
)											
add_custom_target(05_day_harib02c_sys						
  COMMAND ${NASK} ${05_day_harib02c_HEADS} ${05_day_harib02c_HEADB}		
  COMMAND gcc ${05_day_harib02c_CCS}  -c -o ${05_day_harib02c_CCO}	
  COMMAND ld -o ${05_day_harib02c_BOOTB} -e _HariMain ${05_day_harib02c_CCO} ${05_day_harib02c_FUNCO}	
  COMMAND cat ${05_day_harib02c_HEADB} ${05_day_harib02c_BOOTB} > ${05_day_harib02c_SYS}			
)											
add_custom_target(05_day_harib02c_img						
  COMMAND rm -f ${05_day_harib02c_OS}
  COMMAND mformat -f 1440 -C -B ${05_day_harib02c_IPLB} -i ${05_day_harib02c_OS}	
  COMMAND mcopy -n ${05_day_harib02c_SYS} ${05_day_harib02c_OS}			
)											
########### next target ###############
set(asmhead.img_SRCS ${root_SOURCE_DIR}/projects/05_day/harib02c/asmhead.nas)
set(asmhead.img_OUTS ${root_BINARY_DIR}/projects/05_day/harib02c/asmhead.img)
set(asmhead_wine.img_OUTS ${root_BINARY_DIR}/projects/05_day/harib02c/asmhead_wine.img)

add_custom_target(05_day_harib02c_asmhead
  COMMAND ${root_BINARY_DIR}/src/opennask ${asmhead.img_SRCS} ${asmhead.img_OUTS}
)
add_custom_target(05_day_harib02c_asmhead_wine
  COMMAND ${WINE} ${WINE_NASK} ${asmhead.img_SRCS} ${asmhead_wine.img_OUTS}
)
add_custom_target(05_day_harib02c_asmhead_od
  COMMAND ${OD} -t x1 ${asmhead.img_OUTS}      > ${asmhead.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${asmhead_wine.img_OUTS} > ${asmhead_wine.img_OUTS}_t.txt
)

add_dependencies(images 05_day_harib02c_asmhead)
add_dependencies(wine 05_day_harib02c_asmhead_wine)
add_dependencies(od 05_day_harib02c_asmhead_od)

#----------------------------------------------------------
########### next target ###############
set(ipl10.img_SRCS ${root_SOURCE_DIR}/projects/05_day/harib02c/ipl10.nas)
set(ipl10.img_OUTS ${root_BINARY_DIR}/projects/05_day/harib02c/ipl10.img)
set(ipl10_wine.img_OUTS ${root_BINARY_DIR}/projects/05_day/harib02c/ipl10_wine.img)

add_custom_target(05_day_harib02c_ipl10
  COMMAND ${root_BINARY_DIR}/src/opennask ${ipl10.img_SRCS} ${ipl10.img_OUTS}
)
add_custom_target(05_day_harib02c_ipl10_wine
  COMMAND ${WINE} ${WINE_NASK} ${ipl10.img_SRCS} ${ipl10_wine.img_OUTS}
)
add_custom_target(05_day_harib02c_ipl10_od
  COMMAND ${OD} -t x1 ${ipl10.img_OUTS}      > ${ipl10.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${ipl10_wine.img_OUTS} > ${ipl10_wine.img_OUTS}_t.txt
)

add_dependencies(images 05_day_harib02c_ipl10)
add_dependencies(wine 05_day_harib02c_ipl10_wine)
add_dependencies(od 05_day_harib02c_ipl10_od)

#----------------------------------------------------------
########### next target ###############
set(naskfunc.img_SRCS ${root_SOURCE_DIR}/projects/05_day/harib02c/naskfunc.nas)
set(naskfunc.img_OUTS ${root_BINARY_DIR}/projects/05_day/harib02c.imgkfunc.img)
set(naskfunc_wine.img_OUTS ${root_BINARY_DIR}/projects/05_day/harib02c_wine.imgkfunc_wine.img)

add_custom_target(05_day_harib02ckfunc
  COMMAND ${root_BINARY_DIR}/src/opennask ${naskfunc.img_SRCS} ${naskfunc.img_OUTS}
)
add_custom_target(05_day_harib02ckfunc_wine
  COMMAND ${WINE} ${WINE_NASK} ${naskfunc.img_SRCS} ${naskfunc_wine.img_OUTS}
)
add_custom_target(05_day_harib02ckfunc_od
  COMMAND ${OD} -t x1 ${naskfunc.img_OUTS}      > ${naskfunc.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${naskfunc_wine.img_OUTS} > ${naskfunc_wine.img_OUTS}_t.txt
)

add_dependencies(images 05_day_harib02ckfunc)
add_dependencies(wine 05_day_harib02ckfunc_wine)
add_dependencies(od 05_day_harib02ckfunc_od)

#----------------------------------------------------------
