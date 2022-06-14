#!/usr/bin/env bash

sudo launchctl unload /Library/LaunchDaemons/volcanod.plist
sudo launchctl load /Library/LaunchDaemons/volcanod.plist
