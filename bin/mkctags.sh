#!/bin/bash
rm ./tags
ctags -R --c++-kinds=+p --fields=+iaS --extra=+q ./*
#--language-force=C++
