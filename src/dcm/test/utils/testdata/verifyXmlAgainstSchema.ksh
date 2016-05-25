#!/usr/bin/ksh

if [[ 2 -ne $# ]]; then
    echo "Usage: <xml file> <xsd file>"
    exit 1
fi

XMLFILE=$1
XSDFILE=$2

/usr/bin/xmllint --valid --noout $XMLFILE --schema $XSDFILE
