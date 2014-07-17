sftp:sftp.o
	g++ -o sftp sftp.o -lssh2 -ltar
sftp.o:sftp.c
	g++ -c -o sftp.o sftp.c
TarAndSSH2:TarAndSSH.o SftpClient.o
	g++ -o TarAndSSH2 TarAndSSH2.o SftpClient.o -lssh2 -ltar
TarAndSSH.o:TarAndSSH2.cpp
	g++ -c -o TarAndSSH2.o TarAndSSH2.cpp
SftpClient.o:SftpClient.cpp
	g++ -c -o SftpClient.o SftpClient.cpp
clean:
	rm -f sftp sftp.o TarAndSSH2 TarAndSSH2.o SftpClient.o
