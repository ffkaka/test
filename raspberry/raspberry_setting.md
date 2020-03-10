Setting For Virgin Pi
======================

WiFi 를 사용하지 않고 싶은 경우
--------------------------------
* /boot/config.txt 에 아래 dtoverlay 를 추가해 준다
** `dtoverlay=pi3-diable-wifi`
* WiFi 를 사용하고 싶은 경우는 WiFi Country를 US 로 선택해줘야 잘 되는 경우가 많다
** Just in my case


한글입력을 위한 Keyborad Layout 설정
--------------------------------
[ 이 설정은 Keyborad 가 연결 된 상태에서만 가능하다 ]
* raspi-config 에서 설정
** Localization Option >>
** Change Keyboard Layout >>
** Generic 105 (intl..) >>
** Other >>
** Korean >>
** Korean (101/104 key compatiable) >>
** The default for the keyboard layout  >>
** no compose key



한글 디코딩 및 입력을 위한 설정 
--------------------------------

* 한글 문서가 깨지거나 한글 입력이 안되는 경우 아래와 같이 패키지 설정 만으로 해결이 된다

** `sudo apt install fonts-unfonts-core`


Transmission Daemon 설정
--------------------------------

* Torrent 머신으로 사용하기 위해서는 아래 패키지를 설치한다 
** `sudo apt install transmission-daemon`



Kodi 설정
--------------------------------

Kodi 설치는 아래와 같이 하면 된다
`sudo apt install kodi`

