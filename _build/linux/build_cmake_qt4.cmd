#!/usr/bin/bash
#set -x

##################
function pause(){
   read -p "$*"
}

######################
function create_env {
  LOCAL_CONFIG=_env_cfg-$arch

  if [ -f $LOCAL_CONFIG ]
  then
    source $LOCAL_CONFIG
    echo using QTDIR=$QTDIR
  else
    echo create file=$LOCAL_CONFIG
    echo export QTDIR="/home/ralf/dev/inst/qt/qt-4.8.7-$arch-gcc">>$LOCAL_CONFIG
    echo export PATH="/home/ralf/dev/inst/qt/qt-4.8.7-$arch-gcc/bin:$PATH">>$LOCAL_CONFIG

    echo adapt the values and restart this script
    exit 0
  fi
}

######################
function make_qt_res {
  echo --[make_qt_res]--
  source _create_versioninfo.linux $BASE_DIR/base || exit 1

  # Create the QT resource file
  rcc -name ipponboard $BASE_DIR/base/ipponboard.qrc -o $BASE_DIR/base/qrc_ipponboard.cpp || exit 1

  echo make_qt_res finished, hit enter to continue
}

#######################
function make_release {
  echo --[make_release]--
  CC=$(which gcc) CXX=$(which g++) cmake -S "../_cmake_qt4" -B "../_build_cmake_qt4" -DCMAKE_BUILD_TYPE=Release || exit 1

  echo make_release finished, hit enter to continue
}

########################
function build_release {
  echo --[build_release]--
  make_tests || exit 1

  cmake --build ../_build_cmake_qt4 --target Ipponboard || exit 1
  source _copy_files ../.. -release || exit 1
  BIN_DIR=$BASE_DIR/_build/bin/Release


  echo build_release finished, hit enter to continue
}

######################
function build_all_release {
  echo --[build_all_release]--
  clean
  make_qt_res
  make_release
  build_release

  echo build_all_release finished, hit enter to continue
}

#####################
function make_debug {
  echo --[make_debug]--
  CC=$(which gcc) CXX=$(which g++) cmake -S "../_cmake_qt4" -B "../_build_cmake_qt4" -DCMAKE_BUILD_TYPE=Debug || exit 1

  echo make_debug finished, hit enter to continue
}


######################
function build_debug {
  echo --[build_debug]--
  cmake --build ../_build_cmake_qt4 --target Ipponboard_Debug || exit 1
  source _copy_files ../.. -debug || exit 1
  BIN_DIR=$BASE_DIR/_build/bin/Debug

  echo build_debug finished, hit enter to continue
}

################
function clean {
  echo --[make_clean]--
  if [ ! -z "$BIN_DIR" ]
  then
    rm -rf ../_build_cmake_qt4
    rm -rf $BIN_DIR
  fi

  echo clean finished, hit enter to continue
}

##############
function run {
  echo --[run]--
  if [ ! -z "$BIN_DIR" ]
  then
    echo switching to directory [$BIN_DIR]...
    pushd "$BIN_DIR"
    LD_LIBRARY_PATH=. ./Ipponboard
    popd
  fi

  echo run finished, hit enter to continue
}

#####################
function make_tests {
  echo --[make_tests]--
  cmake --build ../_build_cmake_qt4 --target IpponboardTest || exit 1

  pushd ../_build_cmake_qt4/test
  if ./IpponboardTest
  then
    popd
    echo make_tests sucessfully finished
  else
    popd
    echo make_tests with errors, exiting build
    exit 1
  fi
}

####################
function translate {
  echo --[make_translation]--
  pushd $BASE_DIR
  lupdate base core Widgets -locations relative -no-recursive -ts i18n/de.ts -ts i18n/nl.ts
  linguist i18n/de.ts i18n/nl.ts
  lrelease -compress i18n/de.ts -qm i18n/de.qm
  lrelease -compress i18n/nl.ts -qm i18n/nl.qm
  popd
}

###############
function quit {
  exit 0
}
###############
function menu {
  echo "select the operation ************"
  echo "  a) Build All Release"
  echo "  1) Make QT resource file"
  echo "  2) Make Release Makefiles"
  echo "  3) Build Release"
  echo "  4) Make Debug Makefiles"
  echo "  5) Build Debug"
  echo "  6) Clean"
  echo "  7) Run"
  echo "  8) Make Tests"
  echo "  t) Make Translations"
  echo "  q) Quit"

  read n
  case $n in
    a) build_all_release;pause;clear;menu;;
    1) make_qt_res;pause;clear;menu;;
    2) make_release;pause;clear;menu;;
    3) build_release;pause;clear;menu;;
    4) make_debug;pause;clear;menu;;
    5) build_debug;pause;clear;menu;;
    6) clean;pause;clear;menu;;
    7) run;pause;clear;menu;;
    8) make_tests;pause;clear;menu;;
    t) translate;pause;clear;menu;;
    q) quit;;
    *) clear;menu;;
  esac
}

#####################################################################################################################
# main block
#####################################################################################################################
if [ -z "$1" ]
then
	echo Fehlendes Argument: build_cmake_qt4.cmd \<x32\|x64\>
	exit 1
else
	arch=$1
fi

create_env
BIN_DIR=$BASE_DIR/_build/bin

PWD=$(pwd)
BASE_DIR=$PWD/../..
echo BASE_DIR=$BASE_DIR

clear
menu
