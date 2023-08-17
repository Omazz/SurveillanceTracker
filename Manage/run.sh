#!/bin/bash


TARGET=$1

DIR_PROGRAMS=../Build/Programs

DIR_TESTS=../Build/Tests


if [ -z "$1" ]; then
  TARGET="Tracker"
fi

if [ "$TARGET" != "Tracker" ] && [ "$TARGET" != "Indicator" ] && [ "$TARGET" != "Analyzer" ] && [ "$TARGET" != "Radar" ] && [ "$TARGET" != "Settings" ] && [ "$TARGET" != "TestFilters" ] && [ "$TARGET" != "TestTracker" ] && [ "$TARGET" != "TestSettingsTracker" ] && [ "$TARGET" != "TestAsterixHandler" ]; then
  echo "Write correct target to run: all, Tracker, Indicator, Analyzer, Radar, Settings, TestFilters, TestTracker, TestSettingsTracker, TestAsterixHandler."
  exit 1
fi

if [ -f "$DIR_TESTS/$TARGET/$TARGET" ] || [ -f "$DIR_PROGRAMS/$TARGET/$TARGET" ]; then
  if [[ $1 == Test* ]]; then
    cd $DIR_TESTS/$TARGET && ./$TARGET
  else
    cd $DIR_PROGRAMS/$TARGET && ./$TARGET
  fi
else
  echo "Module $TARGET not compiled! You can use: \"./compile.sh $TARGET\""
fi
