/*
 * fileOp.h
 *
 *  Created on: 2014-7-17
 *      Author: wenlongw
 */

#ifndef FILEOP_H_
#define FILEOP_H_


#ifdef OK
#  undef OK
#endif
#define OK  0

#ifdef ERROR
#  undef ERROR
#endif
#define ERROR -1


#include <string>
#include <vector>

using namespace std;


class File
{
public:
	File();
	virtual ~File();
	File( string _path );
	bool exists( void );
	bool isFile( void );
	bool isLink( void );
	bool isDirectory( void );
	bool canRead( void );
	bool canWrite( void );
	bool renameTo( string newPath );
	bool remove( void );
	bool mkDir( string path );
	bool rmDir( void );
	bool xrm( void );
	bool copyTo( string newPath );
	bool xcopyTo( string toPath, string fromPath = "" );
	vector<string> splitPath();

	string			getPath();
	time_t			getDate( void );
	bool 			changeDate( time_t newDate );
	string			getDirectory();
	vector<string>	getDirectoryContent();
	string			getName();
	bool			getSize( unsigned int &size );
	bool			xgetSize( unsigned int &size );

	// path separator character either '\\' on Windows or '/' on Unix
	static const char   separatorChar;
	static const string separatorString;

private:
	string path;
};

#endif /* FILEOP_H_ */
