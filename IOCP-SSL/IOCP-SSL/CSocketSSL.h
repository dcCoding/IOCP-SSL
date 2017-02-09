#pragma once
#include "CSocket.h"
#include "ConvertClass.h"
#include "Components.h"
//ssl���
#include "openssl/ssl.h"
#include "openssl/x509.h"
#include "openssl/rand.h"
#include "openssl/err.h"

#define ServerCertFile "server.crt"
#define ServerKeyFile "server.key"
#define CACertFile "ca.crt"
#define ClientCertFile "client.crt"
#define ClientKeyFile "client.key"
//ssl��bio�����ʾ
#define INPUT 0
#define OUTPUT 1
//ssl�������ڱ�ʶ�ͻ��˺ͷ����
#define CLIENTFLAG 0
#define SERVERFLAG 1
//ssl������Ϣ
#define HANDNOTSUCCESS 0
#define HANDSUCCESS 1
#define HANDERROR 2
//ҳ���С
#define DEFAULT_SIZE 8192
//ssl����ĺ�
#define CHK_NULL(param) if((param)==NULL){ERR_print_errors_fp(stdout);getchar();exit(1);}
#define CHK_ERR(err,msg) if((err)==1){perror(msg);getchar();exit(1);}
#define CHK_SSL(err) if((err)==1){ERR_print_errors_fp(stderr);getchar();exit(2);}
namespace NSCSocket{

	typedef struct{
		int type;	//��־�ǿͻ��˻��Ƿ�������
		int complete;	//��־�����Ƿ����
		SSL* ssl;
		BIO* bio[2];
	}SSL_DATA,*LPSSL_DATA;

	class CSocketSSL{
		SSL_CTX* sslCTX;
		SSL_CTX* cslCTX;

		string newinfo;
	public:
		CSocketSSL();//���캯�����ط������˺Ϳͻ���֤��

		void SSLError();	//������Ϣ

		DWORD iSSLServer();	//��Ϊ�������˼���֤��
		DWORD iSSLClient();	//��Ϊ�ͻ��˼���֤��

		void initSslData(SSL_DATA&);	//��ʼ��SSL_DATA

		string SendHandShake(SSL_DATA&);	//�����ж�
		string RecvHandShake(SSL_DATA&,string);

		void bioInput(SSL_DATA&, char*);//����BIO�ڴ���
		char* bioOutput();//��BIO�ڴ��ж���

		string InfoPack(string,SSL_DATA&);	//���ݼ���
		string InfoUnPack(string,SSL_DATA&);	//���ݽ���

		void s2c(vector<string> _input, char *st, int& size);
		string c2s(char* st, int len);
		std::vector<std::string> split(std::string str, std::string pattern);
	};
}