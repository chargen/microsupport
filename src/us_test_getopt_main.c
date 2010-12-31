#include "us_world.h"
#include "us_getopt.h"
#include "us_logger_printer.h"
#include "us_testutil.h"

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
    * Neither the name of Meyer Sound Laboratories, Inc. nor the
      names of its contributors may be used to endorse or promote products
      derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL MEYER SOUND LABORATORIES, INC.  BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/


/** \addtogroup us_test_getopt */
/*@{*/

static int16_t log_level_default=0;
static int16_t log_level;

static int16_t log_type_default=1;
static int16_t log_type;

static const char log_filename_default[]="log.txt";
static char *log_filename;

static const char log_udp_default[]="127.0.0.1:9999";
static char *log_udp;

static const us_getopt_option_t log_options[]  =
{
    { "level", "Log Level 0-5", US_GETOPT_INT16, &log_level_default, &log_level },
    { "type", "Logger Type: 0=null, 1=stderr, 2=file, 3=udp", US_GETOPT_INT16, &log_type_default, &log_type },
    { "file", "Logger Filename", US_GETOPT_STRING, &log_filename_default, &log_filename },
    { "udp", "Logger Destination UDP address", US_GETOPT_STRING, &log_udp_default, &log_udp },
    { 0, 0, 0, 0, 0 }
};



static const char control_greeting_default[] = "Hello";
static char *control_greeting;

static const char control_name_default[] = "Person";
static char *control_name;


static const us_getopt_option_t control_options[] =
{
    { "greeting", "Greeting style", US_GETOPT_STRING, &control_greeting_default, &control_greeting },
    { "name", "Target's name", US_GETOPT_STRING, &control_name_default, &control_name },
    { 0, 0, 0, 0, 0 }
};

static us_getopt_t opt;

bool us_test_getopt( int argc, char **argv )
{
    bool r=false;
    us_allocator_t *allocator = us_testutil_sys_allocator;
    if( us_getopt_init( &opt, allocator ) )
    {
        r=true;
        r&=us_getopt_add_list( &opt, log_options, "log", "Logging options" );
        r&=us_getopt_add_list( &opt, control_options, "control", "Control Options" );
        if( r )
        {
            us_getopt_print( &opt, us_testutil_printer_stdout );
        }
        us_getopt_destroy( &opt );
    }
    return r;
}

int us_test_getopt_main ( int argc, char **argv )
{
    int r = 1;
    if ( us_testutil_start ( 4096, 4096, argc, argv ) )
    {
#if US_ENABLE_LOGGING
        us_logger_printer_start ( us_testutil_printer_stdout, us_testutil_printer_stderr );
#endif
        us_log_set_level ( US_LOG_LEVEL_DEBUG );
        us_log_info ( "Hello world from %s compiled on %s", __FILE__, __DATE__ );
        if ( us_test_getopt( argc, argv ) )
            r = 0;
        us_log_info ( "Finishing us_test_getopt" );
        us_logger_finish();
        us_testutil_finish();
    }
    return r;
}


/*@}*/




