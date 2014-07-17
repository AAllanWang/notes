/*
 * SftpClient.cpp
 *
 *  Created on: 2014Äê3ÔÂ31ÈÕ
 *      Author: wenlongw
 */

#include "SftpClient.h"
#include <sys/socket.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <string.h>
#include <iostream>
#include <stdio.h>
const static char *password;
static void kbd_callback(const char *name, int name_len,
		const char *instruction, int instruction_len, int num_prompts,
		const LIBSSH2_USERAUTH_KBDINT_PROMPT *prompts,
		LIBSSH2_USERAUTH_KBDINT_RESPONSE *responses, void **abstract) {
	(void) name;
	(void) name_len;
	(void) instruction;
	(void) instruction_len;
	if (num_prompts == 1) {
		responses[0].text = strdup(password);
		responses[0].length = strlen(responses[0].text);
	}
	(void) prompts;
	(void) abstract;
}

SftpClient::SftpClient() {
	// TODO Auto-generated constructor stub
	m_strUserName = "";
	m_strPassword = "";
	m_strServerIp = "";
	m_uPort = 22;
	m_pSsh2Session = NULL;
	m_pSftpHandle = NULL;
	m_pSftpSession = NULL;
	m_sock = -1;
	m_isConnect = false;

}

SftpClient::~SftpClient() {
	// TODO Auto-generated destructor stub
	Destroy();
}

string SftpClient::GetUserName() {
	return m_strUserName;
}

string SftpClient::GetPassword() {
	return m_strPassword;
}

string SftpClient::GetServerIP() {
	return m_strServerIp;
}

unsigned int SftpClient::GetServerPort() {
	return m_uPort;
}

string SftpClient::GetRemoteFilePath() {
	return m_strRemoteFilePath;
}

string SftpClient::GetLocalFilePath() {
	return m_strLocalFilePath;
}

string SftpClient::GetFileName(){
	return m_strFileName;
}
const LIBSSH2_SESSION* SftpClient::GetSSH2Session() {
	return m_pSsh2Session;
}
LIBSSH2_SFTP_HANDLE* SftpClient::GetSftpHandle() const  {
	return m_pSftpHandle;
}
LIBSSH2_SFTP* SftpClient::GetSftpSession()
{
       return m_pSftpSession;
}
bool SftpClient::Initialize(string strServerIP, unsigned int uPort,
		string strUserName, string strPassword) {
	bool ret = true;
	m_strServerIp = strServerIP;
	m_uPort = uPort;
	m_strUserName = strUserName;
	m_strPassword = strPassword;

	//initialize libssh2
	if (libssh2_init(0) != 0) {
		cout << "failed to initialize libssh2" << endl;
		ret = false;
	} else {
		Destroy();

		struct sockaddr_in sin;
		sin.sin_family = AF_INET;
		sin.sin_port = htons(m_uPort);
		sin.sin_addr.s_addr = inet_addr(m_strServerIp.c_str());
		m_sock = socket( AF_INET, SOCK_STREAM, 0);
		if (m_sock < 0) {
			ret = false;
		} else if (connect(m_sock, (const sockaddr*) &sin,
				sizeof(struct sockaddr_in)) != 0) {
			cout << "failed to connect" << endl;
			ret = false;
		}
	}
	return ret;
}

bool SftpClient::Connect() {
	bool ret = true;
	if (m_isConnect == true) {
		return true;
	} else {
//		cout<<"libssh2_session_init()"<<endl;
/*		m_pSsh2Session = libssh2_session_init();
		if (!m_pSsh2Session) {
			cout << "SSH session instance creation failed\n";
			ret = false;
		} else {
			libssh2_session_set_blocking(m_pSsh2Session, 1);
			libssh2_session_set_timeout(m_pSsh2Session, 60000); //timeout after 60s
			//start the ssh session, authentication
			if (libssh2_session_handshake(m_pSsh2Session, m_sock) != 0) {
				char *errmsg = 0;
				int errlen = 0;
				int err = libssh2_session_last_error(m_pSsh2Session, &errmsg,
						&errlen, 0);
				if (errmsg != 0) {
					cout << "SSH session establishment failed: (" << err << ") "
							<< errmsg;
				} else {
					cout << "SSH session establishment failed: (" << err << ")";
				}
				ret = false;
			} else {
				int authType = 0;
				char* userauthlist = NULL;
				libssh2_hostkey_hash(m_pSsh2Session, LIBSSH2_HOSTKEY_HASH_SHA1);
				userauthlist = libssh2_userauth_list(m_pSsh2Session,
						m_strUserName.c_str(), m_strUserName.length());

				cout<<"userauthlist: "<<userauthlist<<endl;
				if (strstr(userauthlist, "password") != NULL) {
					authType |= 1;
				}
				if (strstr(userauthlist, "keyboard-interactive") != NULL) {
					authType |= 2;
				}
				if (strstr(userauthlist, "publickey") != NULL) {
					authType |= 4;
				}
				cout<<"authType:"<<authType<<endl;
				if (authType & 1) //Authenticate either by password
						{
					if (libssh2_userauth_password(m_pSsh2Session,
							m_strUserName.c_str(), m_strPassword.c_str())) {
						cout << "Authentication by password failed!\n";
						ret = false;
					}
				} else if (authType & 2) //or via keyboard-interactive
						{
					if (libssh2_userauth_keyboard_interactive(m_pSsh2Session,
							m_strUserName.c_str(), &kbd_callback)) {
						cout
								<< "Authentication by keyboard-interactive failed!\n";
						ret = false;
					}
				} else {
					cout << "not supported authentication methods found!\n";
					ret = false;
				}
			}
		}
	}
	if(ret)
	{
		m_pSftpSession = libssh2_sftp_init(m_pSsh2Session);
			if (!m_pSftpSession) {
				cout << "Unable to initialize SSH/SFTP session\n";
				ret = false;
			}
			else
			{
				cout << "SSH/SFTP session established\n";
			}
*/

		int auth_pw =0;
	    size_t rc = libssh2_init (0);
	    if (rc != 0) {
	        fprintf(stderr, "libssh2 initialization failed (%d)\n", rc);
	        return false;
	    }

	    /*
	     * The application code is responsible for creating the socket
	     * and establishing the connection
	     */
	    /* Create a session instance
	     */
	    m_pSsh2Session = libssh2_session_init();
	    if(!m_pSsh2Session)
	        return false;

	    /* Since we have set non-blocking, tell libssh2 we are blocking */
	    libssh2_session_set_blocking(m_pSsh2Session, 1);

	    /* ... start it up. This will trade welcome banners, exchange keys,
	     * and setup crypto, compression, and MAC layers
	     */
	    rc = libssh2_session_handshake(m_pSsh2Session, m_sock);
	    if(rc) {
	        fprintf(stderr, "Failure establishing SSH session: %d\n", rc);
	        return false;
	    }

	    /* At this point we havn't yet authenticated.  The first thing to do
	     * is check the hostkey's fingerprint against our known hosts Your app
	     * may have it hard coded, may go to a file, may present it to the
	     * user, that's your call
	     */
	    const char *fingerprint = libssh2_hostkey_hash(m_pSsh2Session, LIBSSH2_HOSTKEY_HASH_SHA1);
	    fprintf(stderr, "Fingerprint: ");
	    for(int i = 0; i < 20; i++) {
	        fprintf(stderr, "%02X ", (unsigned char)fingerprint[i]);
	    }
	    fprintf(stderr, "\n");

	    /* check what authentication methods are available */
	    char *userauthlist = libssh2_userauth_list(m_pSsh2Session, m_strUserName.c_str(), m_strUserName.length());
	    fprintf(stderr, "Authentication methods: %s\n", userauthlist);
	    if (strstr(userauthlist, "password") != NULL) {
	        auth_pw |= 1;
	    }
	    if (strstr(userauthlist, "keyboard-interactive") != NULL) {
	        auth_pw |= 2;
	    }
	    if (strstr(userauthlist, "publickey") != NULL) {
	        auth_pw |= 4;
	    }

	    if (auth_pw & 1) {
	        /* We could authenticate via password */
	        if (libssh2_userauth_password(m_pSsh2Session, m_strUserName.c_str(), m_strPassword.c_str())) {
	            fprintf(stderr, "Authentication by password failed.\n");
	            Destroy();
	            return false;
	        }
	    } else if (auth_pw & 2) {
	        /* Or via keyboard-interactive */
	        if (libssh2_userauth_keyboard_interactive(m_pSsh2Session, m_strUserName.c_str(), &kbd_callback) ) {
	            fprintf(stderr,
	                "\tAuthentication by keyboard-interactive failed!\n");
	            Destroy();
	            return false;
	        } else {
	            fprintf(stderr,
	                "\tAuthentication by keyboard-interactive succeeded.\n");
	        }
	    } else if (auth_pw & 4) {
	        /* Or by public key */
	        if (libssh2_userauth_publickey_fromfile(m_pSsh2Session, m_strUserName.c_str(), "keyfile1", "keyfile2", m_strPassword.c_str())) {
	            fprintf(stderr, "\tAuthentication by public key failed!\n");
	            Destroy();
	            return false;
	        } else {
	            fprintf(stderr, "\tAuthentication by public key succeeded.\n");
	        }
	    } else {
	        fprintf(stderr, "No supported authentication methods found!\n");
	        Destroy();
	        return false;
	    }

	    fprintf(stderr, "libssh2_sftp_init()!\n");
	    m_pSftpSession = libssh2_sftp_init(m_pSsh2Session);

	    if (!m_pSftpSession) {
	        fprintf(stderr, "Unable to init SFTP session\n");
	        Destroy();
	        return false;
	    }
	}
	m_isConnect = ret;
	return ret;
}

bool SftpClient::SftpGetFile(string strRemoteFilePath, string strFileName,
		string strLocalFilePath) {
	m_strRemoteFilePath = strRemoteFilePath;
	m_strFileName = strFileName;
	m_strLocalFilePath = strLocalFilePath;
	bool ret = true;
	if (!m_isConnect) {
		return false;
	}

    fprintf(stderr, "libssh2_sftp_open()!\n");
    /* Request a file via SFTP */
    m_pSftpHandle =
        libssh2_sftp_open(m_pSftpSession,(m_strRemoteFilePath+"/"+m_strFileName).c_str(), LIBSSH2_FXF_READ, 0);

    if (!m_pSftpHandle) {
        fprintf(stderr, "Unable to open file with SFTP: %ld\n",
                libssh2_sftp_last_error(m_pSftpSession));
        Destroy();
        return false;
    }
    fprintf(stderr, "libssh2_sftp_open() is done, now receive data!\n");

	FILE* fz = fopen((GetLocalFilePath() + "/" + GetFileName()).c_str(),
			"wb");
	if (fz == NULL) {
		cout << "open error:" << GetLocalFilePath() + "/" + GetFileName()
				<< endl;
		ret = false;
	} else {
/*		char buf[T_BLOCKSIZE * 100];
		int fileSize = 0;
		ssize_t rc = libssh2_sftp_read(m_pSftpHandle, buf, sizeof(buf));
		cout << "libssh2_sftp_read(m_pSftpHandle,buf,sizeof(buf)) rc:" << rc
				<< endl;
		while (rc > 0) {
			if (fwrite(buf, rc, 1, fz) != 1) {
				cout << "failed to write" << endl;
				ret = false;
				break;
			} else {
				fileSize += rc;
				rc = libssh2_sftp_read(m_pSftpHandle, buf, sizeof(buf));
			}
*/
	    do {
	        char mem[1024];

	        fprintf(stderr, "libssh2_sftp_read()!\n");
	        size_t rc = libssh2_sftp_read(m_pSftpHandle, mem, sizeof(mem));
	        cout<<"rc="<<rc<<endl;
	        if (rc > 0) {
	        //    write(1, mem, rc);
                    //cout<<"mem: "<<mem[0]<<endl;
	            fwrite(mem,rc,1,fz);

	        } else {
	            break;
	        }
	    } while (1);


		fclose(fz);
	}
/*
    do {
        char mem[1024];


        fprintf(stderr, "libssh2_sftp_read()!\n");
        size_t rc = libssh2_sftp_read(m_pSftpHandle, mem, sizeof(mem));
        if (rc > 0) {
            write(1, mem, rc);
        } else {
            break;
        }
    } while (1);
*/

/*
	m_pSftpHandle = libssh2_sftp_open(m_pSftpSession,
			m_strRemoteFilePath.c_str(), LIBSSH2_FXF_READ,
			LIBSSH2_SFTP_S_IRWXU);
	if (!m_pSftpHandle) {
		cout << "no file: " << m_strServerIp << "  " << m_strRemoteFilePath
				<< endl;

		ret = false;
	} else {
		FILE* fz = fopen((GetLocalFilePath() + "/" + GetFileName()).c_str(),
				"wb");
		if (fz == NULL) {
			cout << "open error:" << GetLocalFilePath() + "/" + GetFileName()
					<< endl;
			ret = false;
		} else {
			char buf[T_BLOCKSIZE * 100];
			int fileSize = 0;
			ssize_t rc = libssh2_sftp_read(m_pSftpHandle, buf, sizeof(buf));
			cout << "libssh2_sftp_read(m_pSftpHandle,buf,sizeof(buf)) rc:" << rc
					<< endl;
			while (rc > 0) {
				if (fwrite(buf, rc, 1, fz) != 1) {
					cout << "failed to write" << endl;
					ret = false;
					break;
				} else {
					fileSize += rc;
					rc = libssh2_sftp_read(m_pSftpHandle, buf, sizeof(buf));
				}
			}

			fclose(fz);
		}
	}
*/
	return ret;
}

bool SftpClient::Destroy()
{
	bool ret = true;
	if(m_pSftpHandle != NULL)
	{
		libssh2_sftp_close(m_pSftpHandle);
		m_pSftpHandle = NULL;
	}
	if(m_pSftpSession != NULL)
	{
		libssh2_sftp_shutdown(m_pSftpSession);
		m_pSftpSession = NULL;
	}
	if(m_pSsh2Session != NULL)
	{
		libssh2_session_disconnect(m_pSsh2Session, "Normal Shutdown, Thank you for playing");
		libssh2_session_free(m_pSsh2Session);
		m_pSsh2Session = NULL;
	}
	if(m_sock != -1)
	{
		close(m_sock);
		m_sock = -1;
	}
	libssh2_exit();
	return ret;
}
