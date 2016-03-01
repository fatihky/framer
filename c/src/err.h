/*
    Copyright (c) 2012 Martin Sustrik  All rights reserved.

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"),
    to deal in the Software without restriction, including without limitation
    the rights to use, copy, modify, merge, publish, distribute, sublicense,
    and/or sell copies of the Software, and to permit persons to whom
    the Software is furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included
    in all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
    THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
    FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
    IN THE SOFTWARE.
*/

#ifndef NN_ERR_INCLUDED
#define NN_ERR_INCLUDED

#include <errno.h>
#include <stdio.h>
#include <string.h>

/*  Include nn.h header to define nanomsg-specific error codes. */
// #include "nn.h"

#include "fast.h"

#if defined _MSC_VER
#define FRM_NORETURN __declspec(noreturn)
#elif defined __GNUC__
#define FRM_NORETURN __attribute__ ((noreturn))
#else
#define FRM_NORETURN
#endif

/*  A number random enough not to collide with different errno ranges on      */
/*  different OSes. The assumption is that error_t is at least 32-bit type.   */
#ifndef NN_HAUSNUMERO
#define NN_HAUSNUMERO 156384712
#endif

/*  On some platforms some standard POSIX errnos are not defined.    */
#ifndef ENOTSUP
#define ENOTSUP (NN_HAUSNUMERO + 1)
#endif
#ifndef EPROTONOSUPPORT
#define EPROTONOSUPPORT (NN_HAUSNUMERO + 2)
#endif
#ifndef ENOBUFS
#define ENOBUFS (NN_HAUSNUMERO + 3)
#endif
#ifndef ENETDOWN
#define ENETDOWN (NN_HAUSNUMERO + 4)
#endif
#ifndef EADDRINUSE
#define EADDRINUSE (NN_HAUSNUMERO + 5)
#endif
#ifndef EADDRNOTAVAIL
#define EADDRNOTAVAIL (NN_HAUSNUMERO + 6)
#endif
#ifndef ECONNREFUSED
#define ECONNREFUSED (NN_HAUSNUMERO + 7)
#endif
#ifndef EINPROGRESS
#define EINPROGRESS (NN_HAUSNUMERO + 8)
#endif
#ifndef ENOTSOCK
#define ENOTSOCK (NN_HAUSNUMERO + 9)
#endif
#ifndef EAFNOSUPPORT
#define EAFNOSUPPORT (NN_HAUSNUMERO + 10)
#endif
#ifndef EPROTO
#define EPROTO (NN_HAUSNUMERO + 11)
#endif
#ifndef EAGAIN
#define EAGAIN (NN_HAUSNUMERO + 12)
#endif
#ifndef EBADF
#define EBADF (NN_HAUSNUMERO + 13)
#endif
#ifndef EINVAL
#define EINVAL (NN_HAUSNUMERO + 14)
#endif
#ifndef EMFILE
#define EMFILE (NN_HAUSNUMERO + 15)
#endif
#ifndef EFAULT
#define EFAULT (NN_HAUSNUMERO + 16)
#endif
#ifndef EACCES
#define EACCES (NN_HAUSNUMERO + 17)
#endif
#ifndef EACCESS
#define EACCESS (EACCES)
#endif
#ifndef ENETRESET
#define ENETRESET (NN_HAUSNUMERO + 18)
#endif
#ifndef ENETUNREACH
#define ENETUNREACH (NN_HAUSNUMERO + 19)
#endif
#ifndef EHOSTUNREACH
#define EHOSTUNREACH (NN_HAUSNUMERO + 20)
#endif
#ifndef ENOTCONN
#define ENOTCONN (NN_HAUSNUMERO + 21)
#endif
#ifndef EMSGSIZE
#define EMSGSIZE (NN_HAUSNUMERO + 22)
#endif
#ifndef ETIMEDOUT
#define ETIMEDOUT (NN_HAUSNUMERO + 23)
#endif
#ifndef ECONNABORTED
#define ECONNABORTED (NN_HAUSNUMERO + 24)
#endif
#ifndef ECONNRESET
#define ECONNRESET (NN_HAUSNUMERO + 25)
#endif
#ifndef ENOPROTOOPT
#define ENOPROTOOPT (NN_HAUSNUMERO + 26)
#endif
#ifndef EISCONN
#define EISCONN (NN_HAUSNUMERO + 27)
#define NN_EISCONN_DEFINED
#endif
#ifndef ESOCKTNOSUPPORT
#define ESOCKTNOSUPPORT (NN_HAUSNUMERO + 28)
#endif

/*  Native nanomsg error codes.                                               */
#ifndef ETERM
#define ETERM (NN_HAUSNUMERO + 53)
#endif
#ifndef EFSM
#define EFSM (NN_HAUSNUMERO + 54)
#endif

/*  Same as system assert(). However, under Win32 assert has some deficiencies.
    Thus this macro. */
#define frm_assert(x) \
    do {\
        if (frm_slow (!(x))) {\
            fprintf (stderr, "Assertion failed: %s (%s:%d)\n", #x, \
                __FILE__, __LINE__);\
            fflush (stderr);\
            nn_err_abort ();\
        }\
    } while (0)

#define frm_assert_state(obj, state_name) \
    do {\
        if (frm_slow ((obj)->state != state_name)) {\
            fprintf (stderr, \
                "Assertion failed: %d == %s (%s:%d)\n", \
                (obj)->state, #state_name, \
                __FILE__, __LINE__);\
            fflush (stderr);\
            nn_err_abort ();\
        }\
    } while (0)

/*  Checks whether memory allocation was successful. */
#define alloc_assert(x) \
    do {\
        if (frm_slow (!x)) {\
            fprintf (stderr, "Out of memory (%s:%d)\n",\
                __FILE__, __LINE__);\
            fflush (stderr);\
            nn_err_abort ();\
        }\
    } while (0)

/*  Check the condition. If false prints out the errno. */
#define errno_assert(x) \
    do {\
        if (frm_slow (!(x))) {\
            fprintf (stderr, "%s [%d] (%s:%d)\n", nn_err_strerror (errno),\
                (int) errno, __FILE__, __LINE__);\
            fflush (stderr);\
            nn_err_abort ();\
        }\
    } while (0)

/*  Checks whether supplied errno number is an error. */
#define errnum_assert(cond, err) \
    do {\
        if (frm_slow (!(cond))) {\
            fprintf (stderr, "%s [%d] (%s:%d)\n", nn_err_strerror (err),\
                (int) (err), __FILE__, __LINE__);\
            fflush (stderr);\
            nn_err_abort ();\
        }\
    } while (0)

/* Checks the condition. If false prints out the GetLastError info. */
#define win_assert(x) \
    do {\
        if (frm_slow (!(x))) {\
            char errstr [256];\
            nn_win_error ((int) GetLastError (), errstr, 256);\
            fprintf (stderr, "%s [%d] (%s:%d)\n",\
                errstr, (int) GetLastError (), __FILE__, __LINE__);\
            fflush (stderr);\
            nn_err_abort ();\
        }\
    } while (0)

/* Checks the condition. If false prints out the WSAGetLastError info. */
#define wsa_assert(x) \
    do {\
        if (frm_slow (!(x))) {\
            char errstr [256];\
            nn_win_error (WSAGetLastError (), errstr, 256);\
            fprintf (stderr, "%s [%d] (%s:%d)\n",\
                errstr, (int) WSAGetLastError (), __FILE__, __LINE__);\
            fflush (stderr);\
            nn_err_abort ();\
        }\
    } while (0)

/*  Assertion-like macros for easier fsm debugging. */
#define nn_fsm_error(message, state, src, type) \
    do {\
        fprintf (stderr, "%s: state=%d source=%d action=%d (%s:%d)\n", \
            message, state, src, type, __FILE__, __LINE__);\
        fflush (stderr);\
        nn_err_abort ();\
    } while (0)

#define nn_fsm_bad_action(state, src, type) nn_fsm_error(\
    "Unexpected action", state, src, type)
#define nn_fsm_bad_state(state, src, type) nn_fsm_error(\
    "Unexpected state", state, src, type)
#define nn_fsm_bad_source(state, src, type) nn_fsm_error(\
    "Unexpected source", state, src, type)

/*  Compile-time assert. */
#define CT_ASSERT_HELPER2(prefix, line) prefix##line
#define CT_ASSERT_HELPER1(prefix, line) CT_ASSERT_HELPER2(prefix, line)
#if defined __COUNTER__
#define CT_ASSERT(x) \
    typedef int CT_ASSERT_HELPER1(ct_assert_,__COUNTER__) [(x) ? 1 : -1]
#else
#define CT_ASSERT(x) \
    typedef int CT_ASSERT_HELPER1(ct_assert_,__LINE__) [(x) ? 1 : -1]
#endif

FRM_NORETURN void nn_err_abort (void);
int nn_err_errno (void);
const char *nn_err_strerror (int errnum);

#ifdef NN_HAVE_WINDOWS
int nn_err_wsa_to_posix (int wsaerr);
void nn_win_error (int err, char *buf, size_t bufsize);
#endif

#endif

