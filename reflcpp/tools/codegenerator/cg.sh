#!/bin/bash

GCCXML=/l/extreme/local/packages/gccxml/bin/gccxml
CODEGENERATOR=./codegenerator

if [ $# -lt 3 ]; then
    echo "Usage: -t=[hpp | cpp] class_head_file class_name"
    exit 1
fi

echo "//gccxml: generate the xml file " $3.xml
$GCCXML -fxml=$3.xml  -fxml-start=$3 $2

if [ $? -eq 0 ]; then
    $CODEGENERATOR  $1 $3.xml
    rm $3.xml
fi
exit 0
