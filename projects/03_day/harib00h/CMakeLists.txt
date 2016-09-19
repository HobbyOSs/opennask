#----------------------------------------------------------
message(STATUS "Entering directory projects/03_day/harib00h/")

########### next target ###############
set(haribote.img_SRCS ${root_SOURCE_DIR}/projects/03_day/harib00h/haribote.nas)
set(haribote.img_OUTS ${root_BINARY_DIR}/projects/03_day/harib00h/haribote.img)
set(haribote_wine.img_OUTS ${root_BINARY_DIR}/projects/03_day/harib00h/haribote_wine.img)

add_custom_target(03_day_harib00h_haribote
  COMMAND ${root_BINARY_DIR}/src/opennask ${haribote.img_SRCS} ${haribote.img_OUTS}
)
add_custom_target(03_day_harib00h_haribote_wine
  COMMAND ${WINE} ${WINE_NASK} ${haribote.img_SRCS} ${haribote_wine.img_OUTS}
)
add_custom_target(03_day_harib00h_haribote_od
  COMMAND ${OD} -t x1 ${haribote.img_OUTS}      > ${haribote.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${haribote_wine.img_OUTS} > ${haribote_wine.img_OUTS}_t.txt
)

add_dependencies(images 03_day_harib00h_haribote)
add_dependencies(wine 03_day_harib00h_haribote_wine)
add_dependencies(od 03_day_harib00h_haribote_od)

########### next target ###############
set(ipl10.img_SRCS ${root_SOURCE_DIR}/projects/03_day/harib00h/ipl10.nas)
set(ipl10.img_OUTS ${root_BINARY_DIR}/projects/03_day/harib00h/ipl10.img)
set(ipl10_wine.img_OUTS ${root_BINARY_DIR}/projects/03_day/harib00h/ipl10_wine.img)

add_custom_target(03_day_harib00h_ipl10
  COMMAND ${root_BINARY_DIR}/src/opennask ${ipl10.img_SRCS} ${ipl10.img_OUTS}
)
add_custom_target(03_day_harib00h_ipl10_wine
  COMMAND ${WINE} ${WINE_NASK} ${ipl10.img_SRCS} ${ipl10_wine.img_OUTS}
)
add_custom_target(03_day_harib00h_ipl10_od
  COMMAND ${OD} -t x1 ${ipl10.img_OUTS}      > ${ipl10.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${ipl10_wine.img_OUTS} > ${ipl10_wine.img_OUTS}_t.txt
)

add_dependencies(images 03_day_harib00h_ipl10)
add_dependencies(wine 03_day_harib00h_ipl10_wine)
add_dependencies(od 03_day_harib00h_ipl10_od)

