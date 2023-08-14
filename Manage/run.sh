#!/bin/bash


target=$1

if [ -z "$1" ]; then
  target="Tracker"
fi

if [ "$target" != "Tracker" ] && [ "$target" != "Indicator" ] && [ "$target" != "Analyzer" ] && [ "$target" != "Radar" ] && [ "$target" != "Settings" ] && [ "$target" != "TestFilters" ] && [ "$target" != "TestTracker" ] && [ "$target" != "TestSettingsTracker" ] && [ "$target" != "TestAsterixHandler" ]; then
  echo "Write correct target to run: all, Tracker, Indicator, Analyzer, Radar, Settings, TestFilters, TestTracker, TestSettingsTracker, TestAsterixHandler."
  exit 1
fi

if [ -f "../Build/Tests/$target/$target" ] || [ -f "../Build/Programs/$target/$target" ]; then
  if [[ $1 == Test* ]]; then
    ../Build/Tests/$target/$target
  else
    ../Build/Programs/$target/$target
  fi
else
  echo "Module $target not compiled! You can use: \"./compile.sh $target\""
fi
