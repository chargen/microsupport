#include "us_world.h"

/*
Copyright (c) 2010, Meyer Sound Laboratories, Inc.
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
    * Neither the name of the Meyer Sound Laboratories, Inc. nor the
      names of its contributors may be used to endorse or promote products
      derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL MEYER SOUND LABORATORIES, INC. BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifdef _WIN32

#if defined(_MSC_VER) || defined(_MSC_EXTENSIONS)
#define DELTA_EPOCH_IN_MICROSECS  11644473600000000Ui64
#else
#define DELTA_EPOCH_IN_MICROSECS  11644473600000000ULL
#endif

#if 0
struct timezone
{
    int  tz_minuteswest; /* minutes W of Greenwich */
    int  tz_dsttime;     /* type of dst correction */
};
#endif

int gettimeofday ( struct timeval *tv, struct timezone *tz )
{
    FILETIME ft;
    unsigned __int64 tmpres = 0;
    static int tzflag;
    if ( NULL != tv )
    {
        GetSystemTimeAsFileTime ( &ft );
        tmpres |= ft.dwHighDateTime;
        tmpres <<= 32;
        tmpres |= ft.dwLowDateTime;
        /*converting file time to unix epoch*/
        tmpres /= 10;  /*convert into microseconds*/
        tmpres -= US_DELTA_EPOCH_IN_MICROSECS;
        tv->tv_sec = ( long ) ( tmpres / 1000000UL );
        tv->tv_usec = ( long ) ( tmpres % 1000000UL );
    }
    return 0;
}

bool us_platform_init_sockets ( void )
{
    WSADATA wsaData;
    WORD version;
    int error;
    version = MAKEWORD ( 2, 2 );
    error = WSAStartup ( version, &wsaData );
    if ( error != 0 )
    {
        return false;
    }
    if ( version != wsaData.wVersion )
    {
        return false;
    }
    return true;
}

#elif defined(US_CONFIG_POSIX)

bool us_platform_init_sockets ( void )
{
    signal ( EPIPE, SIG_IGN );
    return true;
}

void us_gettimeofday ( struct timeval *tv )
{
    int r;
    r = gettimeofday ( tv, 0 );
    if ( r != 0 )
    {
        perror ( "gettimeofday" );
        abort();
    }
}

#elif defined(TARGET_PLATFORM_AVR)
bool us_platform_init_sockets ( void )
{
    /* TODO */
}

void us_gettimeofday ( struct timeval *tv )
{
    /* TODO */
    tv->sec = 0;
    tv->usec = 0;
}

#else
bool us_platform_init_sockets ( void )
{
    /* TODO */
    return true;
}

void us_gettimeofday ( struct timeval *tv )
{
    /* TODO */
    tv->tv_sec = 0;
    tv->tv_usec = 0;
}
#endif




