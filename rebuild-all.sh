#!/bin/sh
echo Rebuilding CBPP! Yay!
cbb2 -f tars/linux/main -t cbpp -t cbvs -t assets -t cdf_shared -t glad_shared -t yyjson_shared -t cbep -t libcbpp
