/*
This file is part of the SFA-MaxLib.

The SFA-MaxLib is a collection of Max/MSP objects including FTM externals,
FTM abstractions and MAX abstractions.
SFA-MaxLib Copyright (C) 2014 Stefano Fasciani, National University of Singapore
Inquiries: stefanofasciani@stefanofasciani.com
Website: www.stefanofasciani.com

The SFA-MaxLib is free software: you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

The SFA-MaxLib is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Less General Public License for more details.

You should have received a copy of the GNU Lesser General Public License
along with SFA-MaxLib.  If not, see <http://www.gnu.org/licenses/>.
*/


//Define number of ins/outs
inlets = 2;
outlets = 2;
this.autowatch = 1; //automatically compile script every time script is saved

var offset = 0;

function msg_int(v)
{
	if(inlet==1){
		offset = v;
	}
}


function list()
{
	var i;
	var input = arrayfromargs(arguments);
	for(i=0;i<input.length;i++){
		outlet(1,offset+i);
		outlet(0,input[i]);
	}
}



/*var offset=0;

function msg_int(v)
{
	offset = v;
}


function list()
{
	var a = arrayfromargs(arguments);
	//start = a[0];
	//num = a[1];
	for(i=0;i<a.length;i++){
		//out=i+start;
		outlet(0,1);
		outlet(1,2,3,4);
	}
}*/

