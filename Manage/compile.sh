#!/bin/bash

target=$1

if [ -z "$1" ]; then
  target="all"
fi

if [ "$target" != "all" ] && [ "$target" != "Tracker" ] && [ "$target" != "Indicator" ] && [ "$target" != "Analyzer" ] && [ "$target" != "Radar" ] && [ "$target" != "Settings" ] && [ "$target" != "TestFilters" ] && [ "$target" != "TestTracker" ] && [ "$target" != "TestSettingsTracker" ] && [ "$target" != "TestAsterixHandler" ]; then
  echo "Write correct target: all, Tracker, Indicator, Analyzer, Radar, Settings, TestFilters, TestTracker, TestSettingsTracker, TestAsterixHandler."
  exit 1
fi

cd  ..
mkdir ./Build
cd Build
cmake ..
cmake --build . --target $target
