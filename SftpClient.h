/*
 * SftpClient.h
 *
 *  Created on: 2014Äê3ÔÂ31ÈÕ
 *      Author: wenlongw
 */

#ifndef SFTPCLIENT_H_
#define SFTPCLIENT_H_

#include <string>

#include "libssh2.h"
#include "libssh2_sftp.h"
#include "libtar.h"

using namespace std;

class SftpClient {
public:
	SftpClient();
	virtual ~SftpClient();

	//void SetUserName(string strUserName);
	string GetUserName();
	//void SetPassword(string strPassword);
	string GetPassword();
	//void SetServerIP(string strServerIP);
	string GetServerIP();
	//void SetServerPort(unsigned int uPort);
	unsigned int GetServerPort();
	//void SetRemoteFilePath(string strRemoteFilePath);
	string GetRemoteFilePath();
	//void SetLocalFilePath(string strLocalFilePath);
	string GetLocalFilePath();
	string GetFileName();
	const LIBSSH2_SESSION* GetSSH2Session();
	LIBSSH2_SFTP_HANDLE* GetSftpHandle() const;
        LIBSSH2_SFTP* GetSftpSession();

	bool Initialize(string strServerIP,unsigned int uPort,string strUserName, string strPassword);
	bool Destroy();
	bool Connect();
	bool SftpGetFile(string strRemoteFilePath,string strFileName,string strLocalFilePath);
private:
	string m_strUserName;	//User Name
	string m_strPassword;	//Password
	string m_strServerIp;	//Sftp Server IP
	unsigned int m_uPort;	//Sftp Server Port, default is 22
	int m_sock;				//Socket
	LIBSSH2_SESSION* m_pSsh2Session;	//SSH2 Session
	LIBSSH2_SFTP* m_pSftpSession;		//Sftp Session
	LIBSSH2_SFTP_HANDLE *m_pSftpHandle;	//Sftp Handle
	string m_strRemoteFilePath;	//Remote File Path
	string m_strFileName;		//File Name
	string m_strLocalFilePath;	//Local File Path

	bool m_isConnect;
};

#endif /* SFTPCLIENT_H_ */
