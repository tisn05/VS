@echo off
dim ??keytool.exe????
set keytoolPath = D:\Program Files\Java\jre7\bin\keytool
%keytoolPath% -genkey -v -alias root -keyalg RSA -storetype PKCS12 -keystore my.p12 -dname "C=ÖÐ¹ú" -storepass a12345678 -keypass a12345678 -validity 3650
pause