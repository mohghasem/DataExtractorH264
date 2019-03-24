/*
***********************************************************************
* COPYRIGHT AND WARRANTY INFORMATION
*
* Copyright 2001, International Telecommunications Union, Geneva
*
* DISCLAIMER OF WARRANTY
*
* These software programs are available to the user without any
* license fee or royalty on an "as is" basis. The ITU disclaims
* any and all warranties, whether express, implied, or
* statutory, including any implied warranties of merchantability
* or of fitness for a particular purpose.  In no event shall the
* contributor or the ITU be liable for any incidental, punitive, or
* consequential damages of any kind whatsoever arising from the
* use of these programs.
*
* This disclaimer of warranty extends to the user of these programs
* and user's customers, employees, agents, transferees, successors,
* and assigns.
*
* The ITU does not represent or warrant that the programs furnished
* hereunder are free of infringement of any third-party patents.
* Commercial implementations of ITU-T Recommendations, including
* shareware, may be subject to royalty fees to patent holders.
* Information regarding the ITU-T patent policy is available from
* the ITU Web site at http://www.itu.int.
*
* THIS IS NOT A GRANT OF PATENT RIGHTS - SEE THE ITU-T PATENT POLICY.
************************************************************************
*/

/*!
 ***************************************************************************
 * \file
 *    cabac.h
 *
 * \brief
 *    Headerfile for entropy coding routines
 *
 * \author
 *    Detlev Marpe                                                         \n
 *    Copyright (C) 2000 HEINRICH HERTZ INSTITUTE All Rights Reserved.
 *
 * \date
 *    21. Oct 2000 (Changes by Tobias Oelbaum 28.08.2001)
 ***************************************************************************
 */


#ifndef _CABAC_H_
#define _CABAC_H_

#include "global.h"


/*******************************************************************************************
 * l o c a l    c o n s t a n t s   f o r   i n i t i a l i z a t i o n   o f   m o d e l s
 *******************************************************************************************
 */
static const int ABT_MODE_Ini[2][3][2]=
{
  {{-10,68}, { 4,62}, {14,33}}, // INTRA block mode in INTRA slices
  {{-4 ,62}, { 1,63}, { 7,47}}  // INTRA block mode in INTER slices
};

static const int MB_TYPE_Ini[3][11][2]=
{
  {{  7,25},	{8,35},	{-2,63}, {0,50},  {-9,68}, {-15,74},	{-3,36},  {-1,51},   {0,50},   {0,50}, {0,50}}, //I-FRAME
	{{-23,66},{-14,77},	{-9,88}, {0,50},   {2,13},	{14,24}, {-21,69},  {-1,52},  {-9,55},  {-7,50}, {2,47}}, //P-FRAME + (I16 for B-FRAME)
  {{  9,49},	{3,65},	{ 0,78}, {0,50}, {-13,81}, {-14,73},  {-8,64}, {-21,52}, {-16,71}, {-11,82}, {0,50}}  //B-FRAME
};

static const int B8_TYPE_Ini[2][9][2]=
{
  {{ 0,50},{  8,46},{12,11},{-4,62},{18,48},{0,50},{0,50},{0,50},{0,50}}, //P-FRAME
  {{-9,69},{-12,66},{-9,56},{ 3,47},{ 0,50},{0,50},{0,50},{0,50},{0,50}}  //B-FRAME
};

static const int MV_RES_Ini[2][10][2]=
{
	{{1,48},{0,50},{-5,60},{-8,70},{0,50},	{-1,45},{0,50},{-5,59},{-9,71},{0,50}},
  {{2,52},{2,62},{-3,64},{-2,80},{0,50},	{ 0,50},{3,61},{-3,63},{-1,80},{0,50}}
};

static const int REF_NO_Ini[2][6][2]=
{
  {{3,27},{-1,47},{0,45},{-2,60},{-1,57},{0,48}},
  {{3,27},{-1,47},{0,45},{-2,60},{-1,57},{0,48}}  // same as above
};

static const int DELTA_QP_Ini[4][2]=
{
	{0,28},{0,0},{0,0},{0,0}
};

static const int CBP_Ini[2][3][4][2]=
{
  {	{{ -3,75}, {-4,63}, { -4,70},  {-5,56}},
		{{ -7,65}, {-1,63}, { -9,77},  {-4,76}},
		{{-18,66}, {-9,67}, {-13,70},  {-7,74}}},//!< intra cbp

  { {{-21,81},{-15,60},	{-14,61}, {-15,47}},
    {{-23,58},{-18,64},	{-16,63}, {-18,73}},
		{{-11,46},{ -6,56},	{ -8,59}, {-18,74}}}  //!< inter cbp
};

static const int IPR_Ini[9][2][2]=
{
  {{ -5,49},{-3,58}},
  {{ -5,58},{-4,58}},
  {{ -5,59},{-4,60}},
  {{ -6,61},{-5,62}},
  {{ -4,60},{-4,61}},
  {{ -6,64},{-6,63}},
  {{ -3,75},{-4,63}},
  {{ -7,65},{-1,63}},
  {{-18,66},{-9,67}}
};

static const int BCBP_Ini[2][NUM_BLOCK_TYPES][NUM_BCBP_CTX][2]=
{
  { //======  INITIALIZATION FOR INTER SLICES ======
	{{-4,72}, {-4,68}, {-6,75}, {-6,75}},		// Y_16DC
	{{-4,37}, {-3,50}, {-6,49}, {-3,61}},		// Y_16AC
	{{-2,61}, {-7,75}, {-12,70},{-20,86}},	// Y_8x8 + Y_8x4
	{{-3,63}, {-15,75},{-13,80},{-21,88}},	// Y_4x8
	{{-4,57}, {-9,66}, {-7,64}, {-17,80}},	// Y_4x4
	{{0,55},  {-3,70}, {-4,68}, {-4,75}},		// UV_DC
	{{-3,41}, {-4,62}, {-6,58}, {-8,73}},		// UV_AC
	{{0,0},   {-0,0},  {0,0},   {0,0}}		  // not used
  },
  { //======  INITIALIZATION FOR INTRA SLICES (same as for inter slices except LUMA_8x8...LUMA_4x4) ======
	{{-4,72}, {-4,68}, {-6,75}, {-6,75}},		// Y_16DC
	{{-4,37}, {-3,50}, {-6,49}, {-3,61}},		// Y_16AC
	{{-4,63}, {-1,70}, {-7,68}, {-5,76}},		// Y_8x8 + Y_8x4
	{{-1,63}, {-7,74}, {-1,70}, {-5,76}},		// Y_4x8
	{{-2,52}, {-7,68}, {-5,61}, {-8,77}},		// Y_4x4
	{{0,55},  {-3,70}, {-4,68}, {-4,75}},		// UV_DC
	{{-3,41}, {-4,62}, {-6,58}, {-8,73}},		// UV_AC
	{{0,0},   {-0,0},  {0,0},   {0,0}}	    // not used
  }
};

static const int MAP_Ini[2][NUM_BLOCK_TYPES][NUM_MAP_CTX][2]=
{
  { //======  INITIALIZATION FOR INTER SLICES ======
	  {	{-6,68}, {-11,66}, {-5,63}, {-5,56}, {2,43},
		{1,47},  {-8,58},  {-3,46}, {4,38},  {0,58},
		{1,51},  {-7,57},  {-1,53}, {2,47},  {-1,59}},   // Y_16DC

	  { {0,50},  {0,44},   {2,53},  {0,49},  {1,43},
		{4,45},  {-2,40},  {-1,45}, {0,50},  {2,55},
		{-7,52}, {-2,57},  {7,50},  {2,52},  {4,66}},    // Y_16AC

	  { {-4,44}, {-3,34},  {-2,35}, {-4,33}, {-5,31},
		{-2,31}, {-7,36},  {-1,31}, {-4,33}, {4,29},
		{0,29},  {2,31},   {0,31},  {5,23},  {15,28}},   // Y_8x8

	  { {-3,48}, {-5,47},  {-3,47}, {-7,47}, {-7,48},
		{-3,46}, {-7,48},  {-4,45}, {-2,45}, {-5,46},
		{-8,43}, {0,35},   {5,40},  {-3,57}, {5,63}},    // Y_8x4

	  {	{-3,49}, {-6,47},  {-3,48}, {-7,46}, {-6,47},
		{-3,45}, {-8,49},  {-6,46}, {-3,46}, {-2,45},
		{-7,45}, {-2,37},  {0,38},  {-3,54}, {7,64}},    // Y_4x8

	  {	{4,46},  {1,43},   {2,45},  {-4,46}, {-1,45},
		{3,43},  {-6,44},  {1,45},  {2,46},  {0,46},
		{-12,49},{3,50},   {11,46}, {4,50},  {9,64}},    // Y_4x4

	  { {-3,71}, {-9,69},  {0,70},  {0,0},   {0,0},
		{0,0},   {0,0},    {0,0},   {0,0},   {0,0},
		{0,0},   {0,0},    {0,0},   {0,0},   {0,0}},     // UV_DC

	  { {0,0},   {-6,60},  {0,63},  {-3,54}, {-4,54},
		{4,52},  {-5,44},  {-1,48}, {-7,57}, {11,51},
		{-13,51},{7,55},   {5,57},  {2,51},  {4,68}}     // UV_AC
  },
  { //======  INITIALIZATION FOR INTRA SLICES (same as for inter slices except LUMA_8x8...LUMA_4x4) ======
    
	  {	{-6,68}, {-11,66}, {-5,63}, {-5,56}, {2,43},
		{1,47},  {-8,58},  {-3,46}, {4,38},  {0,58},
		{1,51},  {-7,57},  {-1,53}, {2,47},  {-1,59}},   // Y_16DC

	  { {0,50},  {0,44},   {2,53},  {0,49},  {1,43},
		{4,45},  {-2,40},  {-1,45}, {0,50},  {2,55},
		{-7,52}, {-2,57},  {7,50},  {2,52},  {4,66}},    // Y_16AC

	  { {-1,59}, {-7,55},  {-6,58}, {-7,53}, {-9,52},
		{-5,48}, {-14,59}, {-8,53},{-10,54}, {-5,47},
		{-4,43},{-13,56},  {-9,49}, {-8,47}, {3,44}},    // Y_8x8

	  { {-8,69}, {-12,67}, {-11,68},{-12,63},{-12,66},
		{-12,66},{-7,60}, {-12,63},{-11,64}, {-14,64},
		{-12,57},{-16,57},  {-9,53}, {7,54}, {2,67}},    // Y_8x4

	  {	{-8,68}, {-11,68}, {-11,64},{-11,56},{-13,63},
		{-11,67}, {-8,63}, {-12,65},{-11,63}, {-13,60},
		{-11,53},{-13,52},  {-8,50}, {8,52}, {1,67}},    // Y_4x8

	  {	{-7,67}, {-11,64}, {-8,66}, {-11,63},{-9,63},
		{-8,60}, {-11,55}, {-10,61},{-7,63}, {-7,60},
		{-16,61},{-2,62},  {-1,58}, {-8,61}, {3,68}},    // Y_4x4

	  { {-3,71}, {-9,69},  {0,70},  {0,0},   {0,0},
		{0,0},   {0,0},    {0,0},   {0,0},   {0,0},
		{0,0},   {0,0},    {0,0},   {0,0},   {0,0}},     // UV_DC

	  { {0,0},   {-6,60},  {0,63},  {-3,54}, {-4,54},
		{4,52},  {-5,44},  {-1,48}, {-7,57}, {11,51},
		{-13,51},{7,55},   {5,57},  {2,51},  {4,68}}     // UV_AC
  }
};

static const int LAST_Ini[2][NUM_BLOCK_TYPES][NUM_LAST_CTX][2]=
{
  { //======  INITIALIZATION FOR INTER SLICES ======
	{   {5,30}, {1,31}, {2,33}, {6,29}, {3,30},
		{4,31}, {4,38}, {3,29}, {4,30}, {5,46},
		{9,35}, {2,38}, {3,43}, {1,47}, {6,51}},   // Y_16DC

	{	{0,0},  {4,42}, {5,46}, {9,40}, {7,41},
		{6,46}, {10,40},{14,33},{10,43},{12,48},
		{13,39},{13,41},{16,43},{21,35},{11,55}},  // Y_16AC

	{	{17,27},{23,17},{24,21},{22,28},{23,31},
		{23,36},{17,43},{17,49},{2,58}, {0,0},    // only 0-8 used
		{0,0},  {0,0},  {0,0},  {0,0},  {0,0}},   // Y_8x8

	{	{24,10},{25,7}, {27,12},{24,19},{24,24},
		{25,32},{22,36},{22,45},{-3,61},{0,0},    // only 0-8 used
		{0,0},  {0,0},  {0,0},  {0,0},  {0,0}},   // Y_8x4

	{	{25,9}, {25,7}, {26,13},{25,19},{21,29},
		{25,33},{13,44},{13,50},{2,57}, {0,0},    // only 0-8 used
		{0,0},  {0,0},  {0,0},  {0,0},  {0,0}},   // Y_4x8

	{	{16,27},{21,19},{20,23},{21,22},{21,23},
		{24,23},{25,24},{23,27},{25,29},{23,35},
		{19,36},{21,40},{23,45},{15,53},{8,70}},   // Y_4x4

	{	{12,27},{12,28},{16,38},{0,0},  {0,0},
		{0,0},  {0,0},  {0,0},  {0,0},  {0,0},
		{0,0},  {0,0},  {0,0},  {0,0},  {0,0}},    // UV_DC

	{	{0,0},  {10,28},{14,30},{17,30},{20,30},
		{15,37},{21,39},{22,33},{21,39},{15,52},
		{8,49}, {13,52},{8,60}, {15,56},{3,71}}    // UV_AC
  },
  { //======  INITIALIZATION FOR INTRA SLICES (same as for inter slices except LUMA_8x8...LUMA_4x4) ======
    
	{   {5,30}, {1,31}, {2,33}, {6,29}, {3,30},
		{4,31}, {4,38}, {3,29}, {4,30}, {5,46},
		{9,35}, {2,38}, {3,43}, {1,47}, {6,51}},   // Y_16DC

	{	{0,0},  {4,42}, {5,46}, {9,40}, {7,41},
		{6,46}, {10,40},{14,33},{10,43},{12,48},
		{13,39},{13,41},{16,43},{21,35},{11,55}},  // Y_16AC

	{	{12,29}, {5,29}, {9,28},{18,22},{19,23},
		{23,23},{26,22},{14,41},{40,31},{0,0},     // only 0-8 used
		{0,0},  {0,0},  {0,0},  {0,0},  {0,0}},    // Y_8x8

	{	{9,25}, {7,25}, {13,22},{18,19},{20,22},
		{25,23},{21,30},{22,38},{13,55},{0,0},     // only 0-8 used
		{0,0},  {0,0},  {0,0},  {0,0},  {0,0}},    // Y_8x4

	{	{8,25}, {8,24}, {15,21},{21,17},{25,22},
		{28,23},{22,31},{15,46},{10,59},{0,0},     // only 0-8 used
		{0,0},  {0,0},  {0,0},  {0,0},  {0,0}},    // Y_4x8

	{	{11,25},{9,24}, {12,24},{14,23},{13,23},
		{16,22},{19,20},{18,21},{21,21},{23,25},
		{20,23},{24,25},{25,29},{24,33},{14,53}},  // Y_4x4

	{	{12,27},{12,28},{16,38},{0,0},  {0,0},
		{0,0},  {0,0},  {0,0},  {0,0},  {0,0},
		{0,0},  {0,0},  {0,0},  {0,0},  {0,0}},    // UV_DC

	{	{0,0},  {10,28},{14,30},{17,30},{20,30},
		{15,37},{21,39},{22,33},{21,39},{15,52},
		{8,49}, {13,52},{8,60}, {15,56},{3,71}}    // UV_AC
  }
};

static const int ONE_Ini[2][NUM_BLOCK_TYPES][NUM_ABS_CTX][2]=
{
  { //======  INITIALIZATION FOR INTER SLICES ======
	{{-5,55},  {-3,36}, {-1,35}, {-2,40}, {-6,50}}, // Y_16DC
	{{-10,57}, {-1,30}, {0,32},  {-1,35}, {0,40}},  // Y_16AC
	{{-3,43},  {-1,14}, {0,16},  {2,18},  {1,23}},  // Y_8x8
	{{-9,51},  {-4,21}, {-6,28}, {-4,31}, {-5,37}}, // Y_8x4 + Y_4x8
	{{-6,51},  {-5,24}, {-7,32}, {-4,34}, {-5,39}}, // Y_4x4
	{{-9,70},  {-14,55},{-10,57},{-5,56}, {-4,57}}, // UV_DC
	{{-7,58},  {0,33},  {-1,40}, {-2,45}, {-3,49}}, // UV_AC 
	{{0,0},    {0,0},   {0,0},   {0,0},   {0,0}}    // not used
  },
  { //======  INITIALIZATION FOR INTRA SLICES (same as for inter slices except LUMA_8x8...LUMA_4x4) ======
	{{-5,55},  {-3,36}, {-1,35}, {-2,40}, {-6,50}}, // Y_16DC
	{{-10,57}, {-1,30}, {0,32},  {-1,35}, {0,40}},  // Y_16AC
	{{-9,55},  {-1,30}, {-2,34}, {-2,36}, {-1,37}}, // Y_8x8 
	{{-11,63}, {-3,34}, {-5,39}, {-5,41}, {-4,44}}, // Y_8x4 + Y_4x8
	{{-10,63}, {-5,37}, {-7,43}, {-6,46}, {-5,49}}, // Y_4x4
	{{-9,70},  {-14,55},{-10,57},{-5,56}, {-4,57}}, // UV_DC
	{{-7,58},  {0,33},  {-1,40}, {-2,45}, {-3,49}}, // UV_AC 
	{{0,0},    {0,0},   {0,0},   {0,0},   {0,0}}    // not used
  }
};

static const int ABS_Ini[2][NUM_BLOCK_TYPES][NUM_ABS_CTX][2]=
{
  { //======  INITIALIZATION FOR INTER SLICES ======
	{{-3,44},  {-4,51}, {-3,53}, {-4,55}, {-11,63}}, // Y_16DC
	{{-5,39},  {-4,47}, {-9,55}, {-6,58}, {-4,56}},  // Y_16AC
	{{-7,36},  {-2,43}, {-4,47}, {-5,49}, {1,52}},   // Y_8x8
	{{-10,41}, {-9,49}, {-7,51}, {-8,53}, {-11,60}}, // Y_8x4 + Y_4x8
	{{-12,43}, {-7,50}, {0,48},  {-3,53}, {-8,60}},  // Y_4x4
	{{-14,63}, {-11,67},{-5,68}, {-9,71}, {0,0}},    // UV_DC
	{{-7,48},  {-9,58}, {-16,66},{-12,65},{-12,68}}, // UV_AC 
	{{0,0},    {0,0},   {0,0},   {0,0},   {0,0}}     // not used
  },
  { //======  INITIALIZATION FOR INTRA SLICES (same as for inter slices except LUMA_8x8...LUMA_4x4) ======
	{{-3,44},  {-4,51}, {-3,53}, {-4,55}, {-11,63}}, // Y_16DC
	{{-5,39},  {-4,47}, {-9,55}, {-6,58}, {-4,56}},  // Y_16AC
	{{-4,40},  {-1,45}, {-6,53}, {-8,55}, {-11,64}}, // Y_8x8
	{{-7,46},  {-7,54}, {-5,56}, {-6,58}, {-11,69}}, // Y_8x4 + Y_4x8
	{{-8,50},  {-7,56}, {-9,62}, {-9,64}, {-11,70}}, // Y_4x4
	{{-14,63}, {-11,67},{-5,68}, {-9,71}, {0,0}},    // UV_DC
	{{-7,48},  {-9,58}, {-16,66},{-12,65},{-12,68}}, // UV_AC 
	{{0,0},    {0,0},   {0,0},   {0,0},   {0,0}}     // not used
  }
};



/***********************************************************************
 * L O C A L L Y   D E F I N E D   F U N C T I O N   P R O T O T Y P E S
 ***********************************************************************
 */



void unary_bin_encode(EncodingEnvironmentPtr eep_frame,
                      unsigned int symbol,
                      BiContextTypePtr ctx,
                      int ctx_offset);

void unary_bin_max_encode(EncodingEnvironmentPtr eep_frame,
                          unsigned int symbol,
                          BiContextTypePtr ctx,
                          int ctx_offset,
                          unsigned int max_symbol);

void unary_exp_golomb_level_encode(	EncodingEnvironmentPtr eep_dp,
																		unsigned int symbol,
																		BiContextTypePtr ctx);

void unary_exp_golomb_mv_encode(EncodingEnvironmentPtr eep_dp,
																unsigned int symbol,
																BiContextTypePtr ctx,
																unsigned int max_bin);

#endif  // CABAC_H

