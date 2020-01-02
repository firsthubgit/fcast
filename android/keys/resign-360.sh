jarsigner -verbose -keystore offer.keystore -storepass 13811509061 -keypass android \
    -signedjar 360-resign.apk 360.apk offer

zipalign -v 4 ThinkDrive_signed.apk Target.apk

