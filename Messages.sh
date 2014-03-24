#! /bin/sh
$EXTRACTRC `find . -name '*.rc'` >> rc.cpp
$EXTRACTRC `find . -name *.ui` >> rc.cpp
$EXTRACTRC `find . -name '*.kcfg'` >> rc.cpp
$EXTRACTRC --context="Language Title" --tag-group=none --tag=i18nTitle `find data/languages/ -name '*.xml'` >> rc.cpp
$XGETTEXT rc.cpp `find . -name \*.qml -or -name \*.cpp` -o $podir/artikulate.pot
