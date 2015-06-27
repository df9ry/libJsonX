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

CXXFLAGS =	-std=c++11 -pedantic -Wall -g -shared -fPIC \
			-fmessage-length=0 -fexceptions -pthread \
			-I$(SRCDIR)/../libB64
			

LDXFLAGS =	-std=c++11 -pedantic -Wall -g -shared -fPIC \
			-fmessage-length=0 -fexceptions -pthread \
			-L../../libB64/_$(_CONF)

OBJS     =  Deserialize.o \
			Serialize.o \
			JsonXValue.o
			
LIBS     =  -lB64 -lpthread

TARGET   =	libJsonX.so

$(TARGET):	$(OBJS)
	$(CXX) $(LDXFLAGS) -o $(TARGET) $(OBJS) $(LIBS)
	
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $<	
	
all: $(TARGET)
	echo "Build OK"

doc: $(DOCDIR)
	doxygen ../doxygen.conf
	( cd ../_doc/latex && make )
	
install:
	sudo cp libJsonX.so /usr/local/lib/libJsonX.so.0.1.0
	( cd /usr/local/lib && sudo chown root:staff libJsonX.so.0.1.0     )
	( cd /usr/local/lib && sudo chmod 0755       libJsonX.so.0.1.0     )
	( cd /usr/local/lib && sudo ln -sf libJsonX.so.0.1.0 libJsonX.so.0 )
	( cd /usr/local/lib && sudo ln -sf libJsonX.so.0.1.0 libJsonX.so   )
	
#----- Begin Boilerplate
endif
