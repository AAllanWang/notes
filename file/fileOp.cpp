//============================================================================
// Name        : fileOp.cpp
// Author      : wenlong
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================


#include <string.h>
#include <stdio.h>     // defines "rename"
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <utime.h>
#include <fcntl.h>
#include <dirent.h>
#include "fileOp.h"
#include <iostream>
using namespace std;


//const char   File::separatorChar   = '\\';
//const string File::separatorString = "\\";
const char   File::separatorChar   = '/';
const string File::separatorString = "/";

//////////////////////////////////////////////////////////////////////

// Construction/Destruction

//////////////////////////////////////////////////////////////////////

File::File()
{
}


File::~File()
{
}


File::File( string _path ) : path(_path)
{
}

// check for file existance
bool File::exists( void )
{
	struct stat statBuf;
	if ( OK == stat( path.c_str(), &statBuf ) )
	{
		return( true );
	}
	return( false );
}


// check whether this is a file
bool File::isFile( void )
{
	struct stat statBuf;
	if ( OK == stat( path.c_str(), &statBuf ) )
	{
		if ( S_ISREG(statBuf.st_mode) )
		{
			return( true );
		}
	}
	return( false );
}

// check whether this is a softlink
bool File::isLink( void )
{
	struct stat statBuf;
	if ( OK == stat( path.c_str(), &statBuf ) )
	{
		if ( S_ISLNK(statBuf.st_mode) )
		{
			return( true );
		}
	}
	return( false );
}

// checks whether this is a directory
bool File::isDirectory( void )
{
	struct stat statBuf;
	if ( OK == stat( path.c_str(), &statBuf ) )
	{
		if ( S_ISDIR(statBuf.st_mode) )
		{
			return( true );
		}
	}
	return( false );
}

// returns the file's creation date
time_t File::getDate( void )
{
	struct stat statBuf;
	if ( OK == stat(path.c_str(), &statBuf) )
	{
		return( statBuf.st_atime );
	}
	return( 0 );
}

// returns the file's creation date
bool File::changeDate( time_t newDate )
{
	struct utimbuf dateBuf;
	dateBuf.actime	= newDate;
	dateBuf.modtime	= newDate;

	if ( 0 == utime( path.c_str(), &dateBuf ) )
	{
		return( true );
	}
	return( false );
}

// check read access
bool File::canRead( void )
{
	return((0 == access( path.c_str(), R_OK )) ? true : false);
}


// check write access
bool File::canWrite( void )
{
	return((0 == access( path.c_str(), W_OK )) ? true : false);

}



// renames a file
bool File::renameTo( string newPath )
{
	if ( 0 == rename(path.c_str(), newPath.c_str()) )
	{
		path = newPath;
		return( true );
	}
	return( false );
}



// removes the file
bool File::remove( void )
{
	if (unlink(path.c_str()) == 0)
		return true;
	return false;
}

// creates a directory (only one at a time)
bool File::mkDir( string p_path )
{
	if (mkdir(p_path.c_str(), 0777) == 0)
		return true;
	return false;
}


// removes a directory
// directory to be removed must be empty, must not be
// the current working directory or the root dir
bool File::rmDir()
{
	if (rmdir(path.c_str()) == 0)
		return true;
	return false;
}

// removes a file or directory recursively
bool File::xrm()
{
	if ( isDirectory() )
	{
		vector<string> content = getDirectoryContent();
		vector<string>::iterator pos;
		for ( pos = content.begin(); pos != content.end(); pos++ )
		{
			File rem( path + separatorString + *pos );
			if ( ! rem.xrm() )
			{
				return( false );
			}
		}
		return( rmDir() );
	}

	// it's a file
	return( remove() );
}

// copies the current file to the given location
bool File::copyTo( string newPath )
{
	int src = open( path.c_str(), O_RDONLY, 0664 );
	if ( ERROR  == src )
	{
		return( false );
	}

	int dest = open( newPath.c_str(), O_CREAT | O_RDWR, 0664 );
	if ( ERROR == dest )
	{
		close( src );
		return( false );
	}

	int		bRead;
	char	buf[1024];
	bool	copyOk( true );
	while ( 0 < ( bRead = read( src, buf, 1024 ) ) )
	{
		int bWritten = write( dest, buf, bRead );
		if ( bWritten != bRead )
		{
			copyOk = false;
			break;
		}
	}

	close( src );
	close( dest );

	return( copyOk );
}

// copies the current directory recursively to the given location
bool File::xcopyTo( string toPath, string fromPath )
{
	File to( toPath );

	if ( fromPath.empty() )
	{
		fromPath = path;
	}

	File from( fromPath );
	if ( from.isDirectory() )
	{
		if ( ! to.exists() && ! to.mkDir( toPath ) )
		{
			return( false );
		}

		vector<string> content = from.getDirectoryContent();
		vector<string>::iterator pos;
		for ( pos = content.begin(); pos != content.end(); pos++ )
		{
			if ( ! from.xcopyTo(	toPath + separatorString + *pos,
									fromPath + separatorString + *pos ) )
			{
				return( false );
			}
		}
	}
	else
	{
		return( from.copyTo( toPath ) );
	}

	return( true );
}

// split the path into its parts and return a list
vector<string> File::splitPath()
{
	vector<string> split;

	string::size_type pos;
	while ( string::npos != ( pos = path.find( File::separatorString ) ) )
	{
		if ( 0 != pos ) // drop the leading "/"
		{
			string name = path.substr( 0, pos );
			split.push_back( name );
		}

		path = path.substr( pos + 1 );
	}

	split.push_back( path );

	return( split );
}

// gets the name (+ extension) of the file
string File::getName()
{
	string::size_type pos = 0;
	pos = path.find_last_of(separatorChar);
	if (pos == string::npos)
		return path;

	return path.substr(pos+1);
}

// gets the directory of the file
string File::getDirectory()
{
	string::size_type pos = 0;
	pos = path.find_last_of(separatorChar);
	if (pos == string::npos)
		return "";

	return path.substr(0, pos);
}

// gets a listing of contained files
vector<string> File::getDirectoryContent()
{
	vector<string> fileList;

	if ( isDirectory() )
	{
		DIR *pDir = opendir( path.c_str() );
		if ( NULL != pDir )
		{
			struct dirent *pDirEntry;
			while ( 0 != ( pDirEntry = readdir( pDir ) ) )
			{
				// if it is not this or the parent directory, put it into the list
				if ( ( 0 != strcmp( ".", pDirEntry->d_name ) ) &&
					 ( 0 != strcmp( "..", pDirEntry->d_name ) ) )
				{
					fileList.push_back( pDirEntry->d_name );
				}
			}

			closedir( pDir );
		}
	}

	return( fileList );
}

// gets the path of the file
string File::getPath()
{
	return( path );
}


// gets the size of the file
bool File::getSize( unsigned int &size )
{
	size = 0;

	struct stat statBuf;
	if ( OK == stat( path.c_str(), &statBuf ) )
	{
		if( S_ISREG(statBuf.st_mode) ){
		    size = statBuf.st_size;
		    return( true );
		}
		else{
            size = 0;
		    return( true );
		}
	}
	return( false );
}

// gets the size of the file/directory and its content
bool File::xgetSize( unsigned int &size )
{
	if ( isDirectory() )
	{
		vector<string> content = getDirectoryContent();
		vector<string>::iterator pos;
		for ( pos = content.begin(); pos != content.end(); pos++ )
		{
			File rec( path + separatorString + *pos );
			if ( ! rec.xgetSize( size ) )
			{
				return( false );
			}
		}
	}
	else
	{
		unsigned int fileSize( 0 );
		bool result = getSize( fileSize );

		if ( result )
		{
			size += fileSize;
		}
		return( result );
	}

	return( true );
}


int main() {
	cout << "!!!Hello World!!!" << endl; // prints !!!Hello World!!!
	return 0;
}
