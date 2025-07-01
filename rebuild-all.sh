#!/bin/sh
echo Rebuilding CBPP! Yay!
cbb2 -f tars/linux/libs -t libz -t libcdf -t libsoil_nogl -t libyyjson -t libglad
cbb2 -f tars/linux/main -t cbpp -t cbvs -t assets -t cdf_shared -t glad_shared -t yyjson_shared -t cbep -t libcbpp
cbb2 -f tars/linux/sdk -t cdftree -t shtool
cbb2 -f tars/linux/test -t test