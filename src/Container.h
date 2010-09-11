#ifndef __Container_H__
#define __Container_H__

#include <errno.h>
#include <string>
#include <fstream>
#include <iostream>
#ifdef HAVE_FCNTL_H 
#include <fcntl.h>
#endif
#include <sys/types.h>
#include <sys/dir.h>
#include <dirent.h>
#include <sys/syscall.h>
#include <sys/param.h>
#include <sys/mount.h>
#include <sys/statvfs.h>
#include <time.h>
#include <map>
using namespace std;

#define DEFAULT_MODE (S_IRUSR|S_IWUSR|S_IXUSR|S_IXGRP|S_IXOTH)
#define DROPPING_MODE (S_IRWXU|S_IRWXG|S_IRWXO) 
#define HOSTDIRPREFIX  "hostdir."
#define DROPPINGPREFIX "dropping."
#define DATAPREFIX     DROPPINGPREFIX"data."
#define INDEXPREFIX    DROPPINGPREFIX"index."
#define METADIR        "meta"         // where to stash shortcut metadata
#define VERSIONDIR     "version"      // where to stash the version info 
#define OPENHOSTDIR    "openhosts"    // where to stash whether file open
    // where to stash the chmods and chowns, and identify containers
#define ACCESSFILE     ".plfsaccess113918400"  
#define CREATORFILE    "creator"

enum
DirectoryOperation {
        CHMOD, CHOWN, UTIME, RMDIR, MKDIR
};

#include "Index.h"

class Container {
    public:
            // static stuff
        static int create( const char *, const char *, 
                mode_t mode, int flags, int *extra_attempts,pid_t );

        static bool isContainer( const char *physical_path); 

        static string getIndexPath( const char *, const char * );
        static string getIndexPath( const char *, const char *, int pid );
        static string getDataPath(  const char *, const char *, int pid );

        static int addMeta( off_t, size_t, const char *, const char * );
        static string fetchMeta( string, off_t *, size_t *, struct timespec * );
        static int addOpenrecord( const char *, const char *, pid_t );
        static int removeOpenrecord( const char *, const char *, pid_t );

        static string getHostDirPath( const char *, const char * );
        static string getMetaDirPath( string );
        static string getVersionDir( string path );
        static string getAccessFilePath( string path );
        static string getCreatorFilePath( string path );
        static string chunkPathFromIndexPath( string hostindex, pid_t pid );

        static mode_t fileMode( mode_t );
        static mode_t dirMode(  mode_t );
        static mode_t containerMode(  mode_t );
        static int makeHostDir( const char *path, const char *host, mode_t mode );

        static int getattr( const char *, struct stat * );

        static mode_t getmode( const char * );
        static int Chown( const char *path, uid_t uid, gid_t gid );
        static int Chmod( const char *path, mode_t mode );
        static int Utime( const char *path, const struct utimbuf *buf );
        static int Truncate( const char *, off_t );
        static int Access( const char *path, int mask );

        static int populateIndex( const char *, Index * );
        static int freeIndex( Index ** );
        static size_t hashValue( const char *str );
        static blkcnt_t bytesToBlocks( size_t total_bytes );
        static int nextdropping( string, string *, const char *,
                DIR **, DIR **, struct dirent ** );
        static int makeSubdir(string path, mode_t mode);
        static int makeDropping(string path);
        static int makeAccess(string path,mode_t mode);
        static int makeDroppingReal(string path, mode_t mode); 
        static int makeCreator(string path);
        static int cleanupChmod( const char *path, 
            mode_t mode , int top, uid_t uid , gid_t gid );
        static int cleanupChown( const char *path, uid_t uid, gid_t gid); 
       

    private:
            // static stuff
        static int Modify(DirectoryOperation,const char*,uid_t,gid_t,
                const struct utimbuf*,mode_t);
        static int chmodModify (const char *path, mode_t mode);
        static int chownModify(const char *path,uid_t uid,gid_t gid );
        static int createHelper( const char *, const char *, 
                mode_t mode, int flags, int *extra_attempts, pid_t );
        static int makeTopLevel( const char *, const char *, mode_t, pid_t );
        static string getChunkPath(  const char *, const char *, 
                int pid, const char * );
        static string chunkPath( const char *hostdir, const char *type, 
                const char *host, int pid );
        static string getOpenrecord( const char *, const char *, pid_t );
        static string getOpenHostsDir( string );
        static int discoverOpenHosts( const char *, set<string> * );
        static string hostFromChunk( string datapath, const char *type );
        static string hostdirFromChunk( string chunkpath, const char *type );
        static string containerFromChunk( string datapath );
        static struct dirent *getnextent( DIR *dir, const char *prefix );
        static int makeMeta( string path, mode_t type, mode_t mode );
        static int ignoreNoEnt( int ret );
};

#endif
