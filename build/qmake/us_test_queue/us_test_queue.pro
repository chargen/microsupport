! include( ../common.pri ) {
  error( need common.pri file )
}


TARGET=us_test_queue

SOURCES += $$TOPDIR/tests/us_test_queue.c

HEADERS += $$TOPDIR/include/*.h

