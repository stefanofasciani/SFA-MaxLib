{
	"patcher" : 	{
		"fileversion" : 1,
		"appversion" : 		{
			"major" : 6,
			"minor" : 0,
			"revision" : 4
		}
,
		"rect" : [ 100.0, 100.0, 1075.0, 611.0 ],
		"bglocked" : 0,
		"openinpresentation" : 0,
		"default_fontsize" : 10.0,
		"default_fontface" : 0,
		"default_fontname" : "Arial",
		"gridonopen" : 0,
		"gridsize" : [ 15.0, 15.0 ],
		"gridsnaponopen" : 0,
		"statusbarvisible" : 2,
		"toolbarvisible" : 1,
		"boxanimatetime" : 200,
		"imprint" : 0,
		"enablehscroll" : 1,
		"enablevscroll" : 1,
		"devicewidth" : 0.0,
		"description" : "",
		"digest" : "",
		"tags" : "",
		"boxes" : [ 			{
				"box" : 				{
					"fontname" : "Verdana",
					"fontsize" : 10.0,
					"id" : "obj-10",
					"linecount" : 5,
					"maxclass" : "comment",
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 234.33252, 45.0, 764.0, 67.0 ],
					"text" : "sfa.eig computes the complex eigenvectors of the input real square matrix.\nThe object is a partial porting of the MATLAB eig() function.\n- The input must be a square real matrix NxN.\n- The output is an Nx2 matrix containing the N eigenvalues on each row (column 1 real part, column 2 imaginary part).\n- The computation is in place, the output replaces the input fmat object (use ftm.copy as in the example to preserve the input data)"
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-9",
					"maxclass" : "button",
					"numinlets" : 1,
					"numoutlets" : 1,
					"outlettype" : [ "bang" ],
					"patching_rect" : [ 21.0, 78.0, 20.0, 20.0 ]
				}

			}
, 			{
				"box" : 				{
					"fontname" : "Verdana",
					"fontsize" : 10.0,
					"id" : "obj-27",
					"maxclass" : "comment",
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 10.33252, 45.0, 146.0, 19.0 ],
					"text" : "eigenvalues FTM external"
				}

			}
, 			{
				"box" : 				{
					"fontname" : "Verdana",
					"fontsize" : 24.0,
					"id" : "obj-28",
					"maxclass" : "comment",
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 10.33252, 9.0, 118.0, 36.0 ],
					"text" : "sfa.eig"
				}

			}
, 			{
				"box" : 				{
					"fontname" : "Arial",
					"fontsize" : 10.0,
					"id" : "obj-8",
					"maxclass" : "newobj",
					"numinlets" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 21.0, 164.0, 71.0, 18.0 ],
					"saved_object_attributes" : 					{
						"ftm_objref_conv" : 0,
						"ftm_scope" : 1
					}
,
					"text" : "ftm.copy fmat"
				}

			}
, 			{
				"box" : 				{
					"fontname" : "Arial",
					"fontsize" : 10.0,
					"id" : "obj-6",
					"maxclass" : "newobj",
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 21.0, 257.0, 47.0, 18.0 ],
					"saved_object_attributes" : 					{
						"ftm_objref_conv" : 0,
						"ftm_scope" : 0
					}
,
					"text" : "ftm.print"
				}

			}
, 			{
				"box" : 				{
					"#init" : "",
					"#loadbang" : 0,
					"#triggerall" : 0,
					"#untuple" : 0,
					"fontname" : "Arial",
					"fontsize" : 10.0,
					"ftm_objref_conv" : 0,
					"ftm_scope" : 0,
					"id" : "obj-5",
					"maxclass" : "ftm.mess",
					"numinlets" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 21.0, 121.0, 46.665039, 16.0 ],
					"presentation_rect" : [ 21.0, 121.0, 46.665039, 16.0 ],
					"text" : [ "_($matrix)" ]
				}

			}
, 			{
				"box" : 				{
					"#init" : "",
					"#loadbang" : 0,
					"#triggerall" : 0,
					"#untuple" : 0,
					"fontname" : "Arial",
					"fontsize" : 10.0,
					"ftm_objref_conv" : 0,
					"ftm_scope" : 0,
					"id" : "obj-4",
					"maxclass" : "ftm.mess",
					"numinlets" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 124.0, 121.0, 83.349609, 16.0 ],
					"presentation_rect" : [ 124.0, 121.0, 83.349609, 16.0 ],
					"text" : [ "_($matrix random)" ]
				}

			}
, 			{
				"box" : 				{
					"description" : "fmat",
					"editor_interface" : "matrix",
					"fontname" : "Arial",
					"fontsize" : 10.0,
					"ftm_objref_conv" : 0,
					"ftm_scope" : 2,
					"id" : "obj-3",
					"maxclass" : "ftm.object",
					"name" : "matrix",
					"numinlets" : 1,
					"numoutlets" : 2,
					"outlettype" : [ "", "" ],
					"patching_rect" : [ 124.0, 164.0, 68.22168, 17.0 ],
					"persistence" : 1,
					"presentation_rect" : [ 124.0, 164.0, 68.22168, 17.0 ],
					"scope" : 0,
					"serialized_objects" : [ [ "fmat", 1 ], [ 1, "size", 4, 4 ], [ 1, "set", 0, 0, 0.001892, 0.797599, 0.248691, 0.75144, 0.447989, 0.353537, 0.898949, 0.630325, 0.88087, 0.781345, 0.07306, 0.920592, 0.389747, 0.485413, 0.33642, 0.20779 ] ],
					"text" : "fmat"
				}

			}
, 			{
				"box" : 				{
					"fontname" : "Arial",
					"fontsize" : 10.0,
					"id" : "obj-19",
					"linecount" : 18,
					"maxclass" : "comment",
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 234.33252, 145.0, 430.0, 214.0 ],
					"text" : "The SFA-MaxLib is a collection of Max/MSP objects including FTM externals,\nFTM abstractions and MAX abstractionss.\nSFA-MaxLib Copyright (C) 2014 Stefano Fasciani, National University of Singapore\nInquiries: stefanofasciani@stefanofasciani.com\nWebsite: www.stefanofasciani.com\n\nThe SFA-MaxLib is free software: you can redistribute it and/or modify\nit under the terms of the GNU Lesser General Public License as published by\nthe Free Software Foundation, either version 3 of the License, or\n(at your option) any later version.\n\nThe SFA-MaxLib is distributed in the hope that it will be useful,\nbut WITHOUT ANY WARRANTY; without even the implied warranty of\nMERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the\nGNU Less General Public License for more details.\n\nYou should have received a copy of the GNU Lesser General Public License\nalong with SFA-MaxLib.  If not, see <http://www.gnu.org/licenses/>."
				}

			}
, 			{
				"box" : 				{
					"fontname" : "Arial",
					"fontsize" : 10.0,
					"id" : "obj-1",
					"maxclass" : "newobj",
					"numinlets" : 1,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 21.0, 207.5, 40.0, 18.0 ],
					"text" : "sfa.eig"
				}

			}
 ],
		"lines" : [ 			{
				"patchline" : 				{
					"destination" : [ "obj-6", 0 ],
					"disabled" : 0,
					"hidden" : 0,
					"source" : [ "obj-1", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-8", 0 ],
					"disabled" : 0,
					"hidden" : 0,
					"source" : [ "obj-5", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-1", 0 ],
					"disabled" : 0,
					"hidden" : 0,
					"source" : [ "obj-8", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-5", 0 ],
					"disabled" : 0,
					"hidden" : 0,
					"source" : [ "obj-9", 0 ]
				}

			}
 ],
		"dependency_cache" : [ 			{
				"name" : "ftm.object.mxo",
				"type" : "iLaX"
			}
, 			{
				"name" : "ftm.mess.mxo",
				"type" : "iLaX"
			}
, 			{
				"name" : "ftm.print.mxo",
				"type" : "iLaX"
			}
, 			{
				"name" : "ftm.copy.mxo",
				"type" : "iLaX"
			}
 ]
	}

}
