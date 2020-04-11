TARGET=bbs
CXXFLAGS=-std=c++14
LDFLAGS=-lsqlpp11-connector-sqlite3 -lsqlite3 -lboost_system-mt -lpthread

ifeq ($(OS),Windows_NT) 
TARGET:=$(TARGET).exe
CXXFLAGS:= \
	$(CXXFLAGS) \
	-I./date/include \
	-I./sqlpp11/include \
	-I./sqlpp11-connector-sqlite3/include
LDFLAGS:= \
	-L./sqlpp11-connector-sqlite3/build/src \
	$(LDFLAGS) \
	-lws2_32 -lmswsock
endif

all : bbs.exe

$(TARGET) : bbs.o
	g++ -o $@ $< $(LDFLAGS)

bbs.o : bbs.cxx bbs.h
	g++ -c $(CXXFLAGS) -o $@ $<

bbs.h : schema.sql
	python sqlpp11/scripts/ddl2cpp $< bbs bbsapp

clean : 
	rm $(TARGET) bbs.h
