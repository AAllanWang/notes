//============================================================================
// Name        : TarAndSSH2.cpp
// Author      : Wang Wenlong
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <sstream>
#include <iostream>
#include <sys/socket.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <stdio.h>

#include "libssh2.h"
#include "libssh2_sftp.h"
#include "libtar.h"

#include "SftpClient.h"

using namespace std;

LIBSSH2_SFTP_HANDLE *sftpHandle = 0;

TAR *tar = 0;
char buffer[ T_BLOCKSIZE * 100];
size_t rbytes = -1;
size_t in = 0;      //tar: number of bytes in buffer
size_t off = 0;     //tar: offset next tar block in buffer
int fileSize = 0;
int* p_fileSize = &fileSize;
bool sshSftpAbort = false;

//TAR file integrity
//unsigned int calcFcs      = PPPINITFCS32;
unsigned int fhFcs = 0;
unsigned int fhSize = 0;
unsigned int tarBlkCntr = 0;
unsigned int tarBytesRead = 0;
char fhName[257];
char fhDate[25];


static int myopen(const char* pathName, int flaggy, ...) {
//	calcFcs      = PPPINITFCS32;
	cout<<"myopen() pathName:"<<" flaggy:"<<flaggy<<endl;
	fhFcs = 0;
	fhSize = 0;
	tarBlkCntr = 0;
	tarBytesRead = 0;
	in = 0;
	off = 0;

	memset(fhName, 0, 257);
	memset(fhDate, 0, 25);

	if (pathName == 0)
		return 0;
	return open(pathName, flaggy);
}

static int myclose(int fz) {
	if (fz > 0) {
		close(fz);
	}
	return 0;
}

static ssize_t myreader(int fz, void* buff, size_t buffsize) {
	if (sshSftpAbort == true) {
		return 0;
	}
	cout<<"myreader() buffsize:"<<buffsize<<" buff:"<<buff<<endl;

	strcpy(tar->th_buf.mode, "0000775");

	while ((in - off) < buffsize) {
		cout<<"myreader() in:"<<in<<" out:"<<off<<" buffsize:"<<buffsize<<" fz:"<<fz<<endl;
		if (in != off) {
			memcpy(buffer, buffer + off, in - off);
			in -= off;
			off = 0;
		} else {
			in = 0;
			off = 0;
		}
		if (fz > 0) {
			rbytes = read(fz, buffer + in, sizeof(buffer) - in);
			cout<<"read(fz, buffer + in, sizeof(buffer) - in)  rbytes:"<<rbytes<<endl;
		} else {
			ssize_t rc = libssh2_sftp_read(sftpHandle, buffer + in,
					sizeof(buffer) - in);
			cout<<"libssh2_sftp_read() rc="<<rc<<endl;
			if (rc < 0) {
				return 0;
			}
			rbytes = rc;
		}
		cout<<"if (rbytes == 0) { rbytes:"<<rbytes<<endl;
		if (rbytes == 0) {
			break;
		}
		tarBytesRead += rbytes;
		if (p_fileSize)
			*p_fileSize += rbytes;
		in += rbytes;
	}
    cout<<"if ((in - off) >= buffsize) { "<<" in:"<<in<<" off:"<<off<<" buffsize:"<<buffsize<<endl;
	if ((in - off) >= buffsize) {
		memcpy(buff, buffer + off, buffsize);
		cout<<"tarBlkCntr:"<<tarBlkCntr<<endl;
		if (tarBlkCntr++ == 1) // retrieve file header data
		{
			memcpy(&fhSize, (unsigned char*) buff, 4);
			memcpy(&fhFcs, (unsigned char*) buff + 4, 4);
			memcpy(&fhName, (unsigned char*) buff + 8, 256);
			memcpy(&fhDate, (unsigned char*) buff + 8 + 256, 24);
			memset(buffer + off + 4, 0, 4);
			cout<<"fhName:"<<fhName<<endl;
			cout<<"fhDate:"<<fhDate<<endl;
		}
//		calcFcs = CalcFCS32( calcFcs, (unsigned char*)buffer+off, buffsize );
		off += buffsize;
		return buffsize;
	} else if ((in - off) > 0) {
		memcpy(buff, buffer + off, in - off);
//		calcFcs = CalcFCS32( calcFcs, (unsigned char*)buffer+off, in-off );
		in = 0;
		off = 0;
		return in - off;
	} else {
		in = 0;
		off = 0;
		return 0;
	}
}

int main() {
	cout << "!!!Hello World!!!" << endl; // prints !!!Hello World!!!
    SftpClient sftpClient;
    tartype_t tarCallbacks;
    tarCallbacks.openfunc=myopen;
    tarCallbacks.closefunc=myclose;
    tarCallbacks.writefunc=write;
    tarCallbacks.readfunc=myreader;

    sftpClient.Initialize("135.242.112.125",22,"wenlongw","");
    if(!sftpClient.Connect())
    {
    	cout<<"failed to connect."<<endl;
    }
    if(sftpClient.SftpGetFile("/home/wenlongw/tmpfile","#BI_0690#FT_UMsw#FI_eccm#FV_00004E05","/home/wenlongw/tmp"))
    {
    	sftpHandle = libssh2_sftp_open(sftpClient.GetSftpSession(), "/home/wenlongw/tmpfile/#BI_0690#FT_UMsw#FI_eccm#FV_00004E05", LIBSSH2_FXF_READ, 0);

	//unpack file
    	char unpackDir[sftpClient.GetLocalFilePath().length()];
    	strcpy(unpackDir,sftpClient.GetLocalFilePath().c_str());
    	int tarOpenRc = 0;
    	tarOpenRc = tar_open(&tar,0,&tarCallbacks,O_RDONLY,0,0);
    	if(-1 != tarOpenRc)
    	{
    		//sleep(5);
//    		cout<<"Tar pointer:"<<tar<<endl;

//    		cout<<"pathname:"<<th_get_pathname(tar)<<endl;
//    		cout<<"name : "<<tar->th_buf.name<<endl;
//    		cout<<"linkname : "<<tar->th_buf.linkname<<endl;
    		if(tar_extract_all(tar,unpackDir) !=0)
    		{
    			cout<<"unpack read error:"<<unpackDir<<endl;
    		}
    	}

    	if(tar != NULL)
    	{
    		if(tar_close(tar) !=0)
    		{
    			cout<<"Can NOT close tar file"<<endl;
    		}
    	}
    }

	cout << "Wangwl:tarBytesRead="<<tarBytesRead<< endl;
	return 0;
}
