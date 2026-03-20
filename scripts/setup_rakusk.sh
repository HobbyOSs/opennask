#!/bin/bash
# setup_rakusk.sh
# opennask 内で rakusk を使用するためのセットアップスクリプト

RAKUSK_REPO="https://github.com/HobbyOSs/rakusk.git"

echo "Installing rakusk from $RAKUSK_REPO using zef..."
zef install "$RAKUSK_REPO" --force-test

if [ $? -eq 0 ]; then
  echo "rakusk successfully installed."
else
  echo "Error: rakusk installation failed."
  exit 1
fi
