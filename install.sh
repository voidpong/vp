#!/bin/bash
mkdir -p /usr/share/vpong
gcc $(pwd)/main.c -pthread -o /usr/share/vpong/vpong
cp $(pwd)/ico.png /usr/share/vpong/ico.png
ln -sf /usr/share/vpong/vpong /usr/bin/vpong
echo "[Desktop Entry]
Version=0.1.9
Name=Void Pong
Exec=vpong
Icon=/usr/share/vpong/ico.png
Terminal=true
Type=Application
Categories=Utility;Application;" > /usr/share/applications/vpong.desktop
chmod 777 /usr/share/applications/vpong.desktop /usr/share/vpong/ico.png
