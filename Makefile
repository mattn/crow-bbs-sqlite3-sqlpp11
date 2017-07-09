TARGET=bbs
CXXFLAGS=-std=c++14
LDFLAGS=-lsqlpp11-connector-sqlite3 -lsqlite3 -lboost_system -lpthread

ifeq ($(OS),Windows_NT) 
TARGET:=$(TARGET).exe
CXXFLAGS:= \
	$(CXXFLAGS) \
	-Ic:/progra~2/sqlpp11/include \
	-Ic:/progra~2/sqlpp11-connector-sqlite3/include \
	-Ic:/dev/date 
LDFLAGS:= \
	-Lc:/progra~2/sqlpp11-connector-sqlite3/lib \
	$(LDFLAGS) \
	-lws2_32 -lmswsock
endif

all : bbs.exe

$(TARGET) : bbs.o
	g++ -o $@ $< $(LDFLAGS)

bbs.o : bbs.cxx bbs.h
	g++ -c $(CXXFLAGS) -o $@ $<

bbs.h : schema.sql
	ddl2cpp $< bbs bbsapp

clean : 
	rm $(TARGET) bbs.h
