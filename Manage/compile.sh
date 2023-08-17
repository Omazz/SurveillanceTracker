#!/bin/bash

TARGET=$1

DIR_BUILD=Build


if [ -z "$1" ]; then
  TARGET="all"
fi

if [ "$TARGET" != "all" ] && [ "$TARGET" != "Tracker" ] && [ "$TARGET" != "Indicator" ] && [ "$TARGET" != "Analyzer" ] && [ "$TARGET" != "Radar" ] && [ "$TARGET" != "Settings" ] && [ "$TARGET" != "TestFilters" ] && [ "$TARGET" != "TestTracker" ] && [ "$TARGET" != "TestSettingsTracker" ] && [ "$TARGET" != "TestAsterixHandler" ]; then
  echo "Write correct target: all, Tracker, Indicator, Analyzer, Radar, Settings, TestFilters, TestTracker, TestSettingsTracker, TestAsterixHandler."
  exit 1
fi

cd ..
mkdir ./$DIR_BUILD
cd $DIR_BUILD
cmake ..
cmake --build . --target $TARGET
