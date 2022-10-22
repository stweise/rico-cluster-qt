original="rico_icon.png"
dest="rico.iconset"
mkdir ${dest}
sips -z 16 16     ${original} --out ${dest}/icon_16x16.png
sips -z 32 32     ${original} --out ${dest}/icon_16x16@2x.png
sips -z 32 32     ${original} --out ${dest}/icon_32x32.png
sips -z 64 64     ${original} --out ${dest}/icon_32x32@2x.png
sips -z 128 128   ${original} --out ${dest}/icon_128x128.png
sips -z 256 256   ${original} --out ${dest}/icon_128x128@2x.png
sips -z 256 256   ${original} --out ${dest}/icon_256x256.png
sips -z 512 512   ${original} --out ${dest}/icon_256x256@2x.png
sips -z 512 512   ${original} --out ${dest}/icon_512x512.png
cp ${original} ${dest}/icon_512x512@2x.png
iconutil -c icns ${dest}
rm -R ${dest}

## Notes for Icons
# https://gist.github.com/jamieweavis/b4c394607641e1280d447deed5fc85fc
# https://stackoverflow.com/questions/12306223/how-to-manually-create-icns-files-using-iconutil
# https://doc.qt.io/qt-6/appicon.html
