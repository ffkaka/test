Setting For Virgin Pi
======================

WiFi 를 사용하지 않고 싶은 경우
--------------------------------
* /boot/config.txt 에 아래 dtoverlay 를 추가해 준다
	* `dtoverlay=pi3-diable-wifi`
* WiFi 를 사용하고 싶은 경우는 WiFi Country를 US 로 선택해줘야 잘 되는 경우가 많다
	* Just in my case
	
RAM DISK 사용을 위한 fstab 설정
--------------------------------
```
proc            /proc           proc    defaults          0       0
PARTUUID=738a4d67-01  /boot           vfat    defaults          0       2
PARTUUID=d34db33f-02  /               ext4    defaults,noatime  0       1
tmpfs   /ram    tmpfs   nodev,nosuid,uid=pi,gid=pi,size=512M    0       0
# a swapfile is not a swap partition, no line here
#   use  dphys-swapfile swap[on|off]  for that
```


한글입력을 위한 Keyborad Layout 설정
--------------------------------
[ 이 설정은 Keyborad 가 연결 된 상태에서만 가능하다 ]
* raspi-config 에서 설정
>> Localization Option >>
>> Change Keyboard Layout >>
>> Generic 105 (intl..) >>
>> Other >>
>> Korean >>
>> Korean (101/104 key compatiable) >>
>> The default for the keyboard layout  >>
>> no compose key



한글 디코딩 및 입력을 위한 설정 
--------------------------------
* 한글 Locale 추가
	* ko_KR.UTF-8 UTF-8 추가

* 한글 문서가 깨지거나 한글 입력이 안되는 경우 아래와 같이 패키지 설정 만으로 해결이 된다

	* `sudo apt install fonts-unfonts-core`


Transmission Daemon 설정
--------------------------------

* Torrent 머신으로 사용하기 위해서는 아래 패키지를 설치한다 
	* `sudo apt install transmission-daemon`
* Transmission Web 사용을 위해서는 아래 작업을 해준다
	* Stop the Transmission daemon
		* `sudo systemctl stop transmission-daemon`
	* Edit a setting file
		* `vi /etc/transmission-daemon/settings.json`
		* `"incomplete-dir": "/home/pi/TorrentDown",`
		* `"download-dir": "/home/pi/TorrentDow",`
		* `"rpc-password": "Your_Password",`
		* `"rpc-username": "Your_Username",`
		* `"rpc-whitelist": "192.168.*.*"`
		* `sudo systemctl daemon-reload`
		* `sudo systemctl restart transmission-daemon`



Kodi 설정
--------------------------------

* Kodi 설치는 아래와 같이 하면 된다
	* `sudo apt install kodi`
* systemd 설정
	``` 
	[Unit]
	Description = Kodi Media Center

	# if you don't need the MySQL DB backend, this should be sufficient
	After = systemd-user-sessions.service network.target sound.target

	# if you need the MySQL DB backend, use this block instead of the previous
	# After = systemd-user-sessions.service network.target sound.target mysql.service
	# Wants = mysql.service

	[Service]
	User = pi
	Group = pi
	Type = simple
	ExecStart = /usr/bin/kodi-standalone
	Restart = always
	RestartSec = 15

	[Install]
	WantedBy = multi-user.target
	```
	* /etc/systemd/system 에 kodi.service 생성
	```
	systemctl daemon-reload
	systemctl enable kodi.service
	systemctl start kodi.service
	```
* 한글 설정
	* Setting >> Interface >> Skin >> fonts >> Arial 
	* Setting >> Interface >> Regional >> Languege >> Korean
* 자막용 할글 추가
	* ttf 파일을 아래 경로에 추가
		* `/usr/share/kodi/media/Fonts`
	* Setting >> Player >> 자막언어 
* Web UI Setting
	* Setting >> Service >> Control
#### Netflix
* `sudo apt install kodi-peripheral-joystick kodi-pvr-iptvsimple kodi-inputstream-adaptive kodi-inputstream-rtmp`
* `sudo apt install build-essential python-pip python-dev libffi-dev libssl-dev libnss3`
* `sudo pip install setuptools wheel pycryptodomex`
* Plugin Download 
	* `wget https://github.com/castagnait/repository.castagnait/raw/master/repository.castagnait-1.0.0.zip`
	* kodi >> Settings >> System >> Add-ons >> Unkown-Sources(Turn on)
	* kodi >> Settings >> Addon >> Install from zip file >> Install repository.castagnait-1.0.0.zip
	* kodi >> Settings >> Addon >> Install from repository >> repository.castagnagit >> Video Addon >> Netflix
	* Eable Adaptive InputStream
		* kodi >> Settings >> my-Addon >> Video Player Input Stream >> Input Stream Eanble
		
* My Raspberry pi4 GPU
	* gpu memory split : 512M
	* max-framework-buffer : 8

