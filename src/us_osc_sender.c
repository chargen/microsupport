#include "us_world.h"
#include "us_osc_sender.h"

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


us_osc_sender_t *us_osc_sender_create( void )
{
    return calloc( sizeof( us_osc_sender_t), 1 );
}

bool us_osc_sender_init( us_osc_sender_t *self, void *extra )
{
    self->can_send = us_osc_sender_can_send;
    self->send_msg = us_osc_sender_send_msg;
    self->form_and_send_msg = us_osc_sender_form_and_send_msg;
    return true;
}

void us_osc_sender_destroy( us_osc_sender_t *self )
{
    /* nothing to do */
}

bool us_osc_sender_send_msg( us_osc_sender_t *self, const us_osc_msg_t *msg )
{
    /* base class does nothing to send */
    return false;
}

bool us_osc_sender_can_send( us_osc_sender_t *self )
{
    /* base class can not send messages every */
    return false;
}

bool us_osc_sender_form_and_send_msg( us_osc_sender_t *self, const char *address, const char *typetags, ... )
{
    bool r=false;
    us_osc_msg_t *msg;
    us_simple_allocator_t tmp_allocator;
    char buf[1024];
    va_list ap;
    us_simple_allocator_init( &tmp_allocator, buf, sizeof(buf) );
    va_start( ap, typetags );
    msg = us_osc_msg_vform( &tmp_allocator.m_base, address, typetags, ap );
    if( msg )
    {
        r=self->send_msg( self, msg );
        msg->destroy( msg );
    }
    else
    {
        us_log_error( "unable to form osc message" );
    }
    va_end( ap );
    return r;
}
