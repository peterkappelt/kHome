XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
X   	Artwork and documentation done by: 			X
X								X
X	TEXAS INSTRUMENTS NORWAY LPRF              		X
X  								X
X	Address: Hoffsveien 70C 0377 OSLO	              	X
X 	web: www.ti.com/lprF		              		X
X                                                           	X
XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

PCB NAME : CC13xxEM-5XD-7793_4L
REVISION: 1.0.0
DATE: 2015-10-19

FILE:  PACKED WITH WinZIP 

PCB DESCRIPTION:4 LAYER PCB 
Subclass Name,Type,Material,Thickness (MM)
TOP,CONDUCTOR,COPPER,0.035
DIELECTRIC,FR-4,0.175
L2,PLANE,COPPER,0.0175
DIELECTRIC,FR-4,1.140000
L3,PLANE,COPPER,0.0175
DIELECTRIC,FR-4,0.175,
BOTTOM,CONDUCTOR,COPPER,0.035

Total Thickness: 1.595 MM
 
Via L1-L4

Soldermask color: GREEN

  DE104iML or equivalent substrate (Resin contents around 45%, which gives Er=4.42@2.4GHz, TanD=0.016)
  Dimensions in mm
  DOUBLE SIDE SOLDER MASK,
  DOUBLE SIDE SILKSCREEN,
  0.15 MM MIN TRACE WIDTH AND 0.15 MM MIN ISOLATION.

                 
FILE NAME            		DESCRIPTION                               	FILE TYPE
-------------------------------------------------------------------------------------------
***PCB MANUFACTURING FILES: (Folder Gerber)
Top.art                 	LAYER 1 TOP SIDE/POSITIV              		EXT. GERBER
L2.art				LAYER 2 POSITIVE				EXT. GERBER
L3.art				LAYER 3 POSITIVE				EXT. GERBER
Bottom.art                  	LAYER 4 SOLDER SIDE/POSITIV                 	EXT. GERBER
soldmask_top.art            	SOLDER MASK COMPONENT SIDE/NEGATIVE             EXT. GERBER
soldmask_bottom.art             SOLDER MASK SOLDER SIDE/NEGATIVE            	EXT. GERBER
silk_top.art	            	SILKSCREEN COMPONENT SIDE/POSITIVE              EXT. GERBER
silk_bottom.art		        SILKSCREEN SOLDER SIDE/POSITIVE             	EXT. GERBER
drill.art		        DRILL/MECHANICAL DRAWING                    	EXT. GERBER
art_param.txt                   Gerber report 				        ASCII
photoplot.log                   Gerber report					ASCII


***PCB DRILL FILES: (Folder Drill)
NCdrill-1-4.drl                 NC DRILL Plated		                       	EXCELLON
NCdrill-1-4-np.drl		NC DRILL non-Plated		 	        EXCELLON
ncdrill.log		        DRILL/NC DRILL REPORT                           ASCII
nc_pararm.txt		        PCB PHOTOPLOTTER DEFINITION FILE	        ASCII
nc_tools_auto.txt               Drill diameters				        ASCII


*** ASSEMBLY FILES: (Folder Assembly)
assembly_top.art	        ASSEMBLY DRAWING COMPONENT SIDE/NEGATIVE        EXT. GERBER
assembly_bot_mirror.art        	ASSEMBLY DRAWING SOLDER SIDE/NEGATIVE, MIRROR   EXT. GERBER
paste_top.art	             	SOLDER PAST COMPONENT SIDE/POSITIVE             EXT. GERBER
paste_bottom.art	        SOLDER PAST SOLDER SIDE/POSITIVE                EXT. GERBER
pick_and_place.txt            	PICK AND PLACE FILE 			        ASCII
CC13xxEM_5XD_7793_4L_partlist_1_0_0.xls 	BOM				Excel


*** Document files: (Folder docs)
CC13xxEM_5XD_7793_4L_schematic_1_0_0.pdf	Circuit Diagram			PDF
CC13xxEM_5XD_7793_4L_layout_1_0.pdf        	Layout Diagram			PDF
CC13xxEM_5XD_7793_4L_mech_draw_1_0.pdf		Mechanical drawing		PDF
CC13xxEM_5XD_7793_4L_assembly_1_0_0.pdf		Assembly drawing               	PDF


README.TXT           			THIS FILE                     	        ASCII


Revision history:

0.1.0  Initial revision
---------------------------------------------------------------------------------------------
0.1.1 Changed L11 from 27nH to 22nH
      Changed L12 and L21 from 7.5nH to 8.2nH
      Changed C11 and C22 from 3.6pF to 3.3pF
---------------------------------------------------------------------------------------------
1.0.0 Revision number update for web



END.
