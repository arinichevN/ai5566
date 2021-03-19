#ifndef APP_SERIALS_COUNT_H
#define APP_SERIALS_COUNT_H

#define S0 0
#define S1 0
#define S2 0
#define S3 0
#if defined(HAVE_HWSERIAL0) && defined(WANT_SERIAL0)
#define USE_SERIAL0
#undef S0
#define S0 1
#endif
#if defined(HAVE_HWSERIAL1) && defined(WANT_SERIAL1)
#define USE_SERIAL1
#undef S1
#define S1 1
#endif
#if defined(HAVE_HWSERIAL2) && defined(WANT_SERIAL2)
#define USE_SERIAL2
#undef S2
#define S2 1
#endif
#if defined(HAVE_HWSERIAL3) && defined(WANT_SERIAL3)
#define USE_SERIAL3
#undef S3
#define S3 1
#endif
#define SERIAL_COUNT S0 + S1 + S2 + S3

#endif
