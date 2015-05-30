#   Project libJsonX
#   Copyright (C) 2015  tania@df9ry.de
#
#   This program is free software: you can redistribute it and/or modify
#   it under the terms of the GNU Affero General Public License as
#   published by the Free Software Foundation, either version 3 of the
#   License, or (at your option) any later version.
#
#   This program is distributed in the hope that it will be useful,
#   but WITHOUT ANY WARRANTY; without even the implied warranty of
#   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#   GNU Affero General Public License for more details.
#
#   You should have received a copy of the GNU Affero General Public License
#   along with this program.  If not, see <http://www.gnu.org/licenses/>.
	
ifeq (,$(filter _%,$(notdir $(CURDIR))))
include target.mk
else
#----- End Boilerplate

VPATH = $(SRCDIR)

CXXFLAGS =	-std=c++11 -pedantic -Wall -g -shared -fPIC -rdynamic \
			-fmessage-length=0 \
			-I$(SRCDIR)/../libB64
			

LDXFLAGS =	-std=c++11 -pedantic -Wall -g -shared -fPIC -rdynamic \
			-fmessage-length=0 \
			-L../../libB64/_$(_ARCH)-$(_CONF)

OBJS     =  JsonXArray.o \
			JsonXBase.o \
			JsonXBlob.o \
			JsonXBool.o \
			JsonXNull.o \
			JsonXNumber.o \
			JsonXObject.o \
			JsonXString.o \
			JsonXValue.o
			
LIBS     =  -lB64 -lpthread

TARGET   =	libJsonX.so

$(TARGET):	$(OBJS)
	$(CXX) $(LDXFLAGS) -o $(TARGET) $(OBJS) $(LIBS)
	
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $<	
	
all: $(TARGET)
	cp ../../libB64/_$(_ARCH)-$(_CONF)/libB64.so .
	echo "Build OK"

#----- Begin Boilerplate
endif
