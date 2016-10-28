#----------------------------------------------------------
message(STATUS "Entering directory projects/04_day/harib01c/")

set(NASK ${root_BINARY_DIR}/src/opennask)
set(04_day_harib01c_OS    ${root_BINARY_DIR}/projects/04_day/harib01c/os.img)
set(04_day_harib01c_SYS	  ${root_BINARY_DIR}/projects/04_day/harib01c/os.sys)
set(04_day_harib01c_IPLB  ${root_BINARY_DIR}/projects/04_day/harib01c/ipl.bin)
set(04_day_harib01c_IPLS  ${root_SOURCE_DIR}/projects/04_day/harib01c/ipl10.nas)
set(04_day_harib01c_HEADB ${root_BINARY_DIR}/projects/04_day/harib01c/asmhead.bin)
set(04_day_harib01c_HEADS ${root_SOURCE_DIR}/projects/04_day/harib01c/asmhead.nas)
set(04_day_harib01c_BOOTB ${root_BINARY_DIR}/projects/04_day/harib01c/boot.bin)
set(04_day_harib01c_CCO	  ${root_BINARY_DIR}/projects/04_day/harib01c/boot.o)
set(04_day_harib01c_CCS	  ${root_SOURCE_DIR}/projects/04_day/harib01c/*.c)

add_custom_target(04_day_harib01c_run                                           
  COMMAND ${QEMU} ${QEMUOPT} ${04_day_harib01c_OS}				
)											
add_custom_target(04_day_harib01c_debug						
  COMMAND ${QEMU} -s -S ${QEMUOPT} ${04_day_harib01c_OS} -redir tcp:5555:127.0.0.1:1234 &
)											
add_custom_target(04_day_harib01c_ipl						
  COMMAND ${NASK} ${04_day_harib01c_IPLS} ${04_day_harib01c_IPLB}		
)											
add_custom_target(04_day_harib01c_sys						
  COMMAND ${NASK} ${04_day_harib01c_HEADS} ${04_day_harib01c_HEADB}		
  COMMAND gcc ${04_day_harib01c_CCS}  -c -o ${04_day_harib01c_CCO}	
  COMMAND ld -o ${04_day_harib01c_BOOTB} -e _HariMain ${04_day_harib01c_CCO} ${04_day_harib01c_FUNCO}	
  COMMAND cat ${04_day_harib01c_HEADB} ${04_day_harib01c_BOOTB} > ${04_day_harib01c_SYS}			
)											
add_custom_target(04_day_harib01c_img						
  COMMAND mformat -f 1440 -C -B ${04_day_harib01c_IPLB} -i ${04_day_harib01c_OS}	
  COMMAND mcopy ${04_day_harib01c_SYS} -i ${04_day_harib01c_OS}			
)											
########### next target ###############
set(asmhead.img_SRCS ${root_SOURCE_DIR}/projects/04_day/harib01c/asmhead.nas)
set(asmhead.img_OUTS ${root_BINARY_DIR}/projects/04_day/harib01c/asmhead.img)
set(asmhead_wine.img_OUTS ${root_BINARY_DIR}/projects/04_day/harib01c/asmhead_wine.img)

add_custom_target(04_day_harib01c_asmhead
  COMMAND ${root_BINARY_DIR}/src/opennask ${asmhead.img_SRCS} ${asmhead.img_OUTS}
)
add_custom_target(04_day_harib01c_asmhead_wine
  COMMAND ${WINE} ${WINE_NASK} ${asmhead.img_SRCS} ${asmhead_wine.img_OUTS}
)
add_custom_target(04_day_harib01c_asmhead_od
  COMMAND ${OD} -t x1 ${asmhead.img_OUTS}      > ${asmhead.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${asmhead_wine.img_OUTS} > ${asmhead_wine.img_OUTS}_t.txt
)

add_dependencies(images 04_day_harib01c_asmhead)
add_dependencies(wine 04_day_harib01c_asmhead_wine)
add_dependencies(od 04_day_harib01c_asmhead_od)

#----------------------------------------------------------
########### next target ###############
set(ipl10.img_SRCS ${root_SOURCE_DIR}/projects/04_day/harib01c/ipl10.nas)
set(ipl10.img_OUTS ${root_BINARY_DIR}/projects/04_day/harib01c/ipl10.img)
set(ipl10_wine.img_OUTS ${root_BINARY_DIR}/projects/04_day/harib01c/ipl10_wine.img)

add_custom_target(04_day_harib01c_ipl10
  COMMAND ${root_BINARY_DIR}/src/opennask ${ipl10.img_SRCS} ${ipl10.img_OUTS}
)
add_custom_target(04_day_harib01c_ipl10_wine
  COMMAND ${WINE} ${WINE_NASK} ${ipl10.img_SRCS} ${ipl10_wine.img_OUTS}
)
add_custom_target(04_day_harib01c_ipl10_od
  COMMAND ${OD} -t x1 ${ipl10.img_OUTS}      > ${ipl10.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${ipl10_wine.img_OUTS} > ${ipl10_wine.img_OUTS}_t.txt
)

add_dependencies(images 04_day_harib01c_ipl10)
add_dependencies(wine 04_day_harib01c_ipl10_wine)
add_dependencies(od 04_day_harib01c_ipl10_od)

#----------------------------------------------------------
########### next target ###############
set(naskfunc.img_SRCS ${root_SOURCE_DIR}/projects/04_day/harib01c/naskfunc.nas)
set(naskfunc.img_OUTS ${root_BINARY_DIR}/projects/04_day/harib01c.imgkfunc.img)
set(naskfunc_wine.img_OUTS ${root_BINARY_DIR}/projects/04_day/harib01c_wine.imgkfunc_wine.img)

add_custom_target(04_day_harib01ckfunc
  COMMAND ${root_BINARY_DIR}/src/opennask ${naskfunc.img_SRCS} ${naskfunc.img_OUTS}
)
add_custom_target(04_day_harib01ckfunc_wine
  COMMAND ${WINE} ${WINE_NASK} ${naskfunc.img_SRCS} ${naskfunc_wine.img_OUTS}
)
add_custom_target(04_day_harib01ckfunc_od
  COMMAND ${OD} -t x1 ${naskfunc.img_OUTS}      > ${naskfunc.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${naskfunc_wine.img_OUTS} > ${naskfunc_wine.img_OUTS}_t.txt
)

add_dependencies(images 04_day_harib01ckfunc)
add_dependencies(wine 04_day_harib01ckfunc_wine)
add_dependencies(od 04_day_harib01ckfunc_od)

#----------------------------------------------------------
