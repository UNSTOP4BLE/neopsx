/*
  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/

#include "week5a.h"

#include "../archive.h"
#include "../mem.h"

//Week 5 background structure
typedef struct
{
	//Stage background base structure
	StageBack back;
	
	//Textures
	Gfx_Tex tex_back0; //Back wall
	Gfx_Tex tex_back1; //Second floor
	Gfx_Tex tex_back2; //Lower bop
} Back_week5a;

//Week 5 background functions
void Back_week5a_DrawBG(StageBack *back)
{
	Back_week5a *this = (Back_week5a*)back;
	
	fixed_t fx, fy;
	
	fixed_t beat_bop;
	if ((stage.song_step & 0x3) == 0)
		beat_bop = FIXED_UNIT - ((stage.note_scroll / 24) & FIXED_LAND);
	else
		beat_bop = 0;
	
	//dont Draw Santa
		fx = stage.camera.x;
		fy = stage.camera.y;
	//Draw boppers
	static const struct Back_week5a_LowerBop
	{
		RECT src;
		RECT_FIXED dst;
	} lbop_piece[] = {
		{{0, 0, 115, 210}, {FIXED_DEC(-160,1), FIXED_DEC(-140,1), FIXED_DEC(115,1), FIXED_DEC(210,1)}}, 
		{{124, 0, 131, 124}, {FIXED_DEC(123,1), FIXED_DEC(-50,1), FIXED_DEC(131,1), FIXED_DEC(124,1)}}, 
		{{149, 145, 106, 110}, {FIXED_DEC(-40,1), FIXED_DEC(-50,1), FIXED_DEC(106,1), FIXED_DEC(110,1)}}, 
		
	};
	
	const struct Back_week5a_LowerBop *lbop_p = lbop_piece;
	for (size_t i = 0; i < COUNT_OF(lbop_piece); i++, lbop_p++)
	{
		RECT_FIXED lbop_dst = {
			lbop_p->dst.x - fx - (beat_bop << 1),
			lbop_p->dst.y - fy + (beat_bop << 3),
			lbop_p->dst.w + (beat_bop << 2),
			lbop_p->dst.h - (beat_bop << 3),
		};
		Stage_DrawTex(&this->tex_back2, &lbop_p->src, &lbop_dst, stage.camera.bzoom);
	}
	
	
	//Draw second floor
	fx = stage.camera.x >> 2;
	fy = stage.camera.y >> 2;
	
	static const struct Back_week5a_FloorPiece
	{
		RECT src;
		fixed_t scale;
	} floor_piece[] = {
		{{  0, 0, 161, 256}, FIXED_DEC(14,10)},
		{{161, 0,   9, 256}, FIXED_DEC(7,1)},
		{{171, 0,  85, 256}, FIXED_DEC(14,10)},
	};
	
	RECT_FIXED floor_dst = {
		FIXED_DEC(-220,1) - fx,
		FIXED_DEC(-115,1) - fy,
		0,
		FIXED_DEC(180,1)
	};
	
	const struct Back_week5a_FloorPiece *floor_p = floor_piece;
	for (size_t i = 0; i < COUNT_OF(floor_piece); i++, floor_p++)
	{
		floor_dst.w = floor_p->src.w ? (floor_p->src.w * floor_p->scale) : floor_p->scale;
		Stage_DrawTex(&this->tex_back1, &floor_p->src, &floor_dst, stage.camera.bzoom);
		floor_dst.x += floor_dst.w;
	}
	
	
	//Draw back wall
	fx = stage.camera.x >> 3;
	fy = stage.camera.y >> 3;
	
	static const struct Back_week5a_WallPiece
	{
		RECT src;
		fixed_t scale;
	} wall_piece[] = {
		{{  0, 0, 113, 256}, FIXED_DEC(1,1)},
		{{113, 0,   6, 256}, FIXED_DEC(17,1)},
		{{119, 0, 137, 256}, FIXED_DEC(1,1)},
	};
	
	RECT_FIXED wall_dst = {
		FIXED_DEC(-180,1) - fx,
		FIXED_DEC(-130,1) - fy,
		0,
		FIXED_DEC(190,1)
	};
	
	RECT wall_src = {0, 255, 0, 0};
	RECT_FIXED wall_fill;
	wall_fill.x = wall_dst.x;
	wall_fill.y = wall_dst.y + wall_dst.h - FIXED_UNIT;
	wall_fill.w = FIXED_DEC(500,1);
	wall_fill.h = FIXED_DEC(100,1);
	Stage_DrawTex(&this->tex_back0, &wall_src, &wall_fill, stage.camera.bzoom);
	
	const struct Back_week5a_WallPiece *wall_p = wall_piece;
	for (size_t i = 0; i < COUNT_OF(wall_piece); i++, wall_p++)
	{
		wall_dst.w = wall_p->src.w ? (wall_p->src.w * wall_p->scale) : wall_p->scale;
		Stage_DrawTex(&this->tex_back0, &wall_p->src, &wall_dst, stage.camera.bzoom);
		wall_dst.x += wall_dst.w;
	}
}

void Back_week5a_Free(StageBack *back)
{
	Back_week5a *this = (Back_week5a*)back;
	
	//Free structure
	Mem_Free(this);
}

StageBack *Back_week5a_New()
{
	//Allocate background structure
	Back_week5a *this = (Back_week5a*)Mem_Alloc(sizeof(Back_week5a));
	if (this == NULL)
		return NULL;
	
	//Set background functions
	this->back.draw_fg = NULL;
	this->back.draw_md = NULL;
	this->back.draw_bg = Back_week5a_DrawBG;
	this->back.free = Back_week5a_Free;
	
	//Load background textures
	IO_Data arc_back = IO_Read("\\WEEK5A\\BACK.ARC;1");
	Gfx_LoadTex(&this->tex_back0, Archive_Find(arc_back, "back0.tim"), 0);
	Gfx_LoadTex(&this->tex_back1, Archive_Find(arc_back, "back1.tim"), 0);
	Gfx_LoadTex(&this->tex_back2, Archive_Find(arc_back, "back2.tim"), 0);
	Mem_Free(arc_back);
	
	return (StageBack*)this;
}
