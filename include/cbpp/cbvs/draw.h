#ifndef CBVS_DRAW_H
#define CBVS_DRAW_H

#include "cbpp/vec2.h"

namespace cbdraw {
	static int width, height;
	
	void SetWH(int width, int height);
	
	cbpp::Vec2 GL2Human(cbpp::Vec2 GLpos);
	cbpp::Vec2 Human2GL(cbpp::Vec2 Human_pos);
	
	void Line(cbpp::Vec2 pos1, cbpp::Vec2 pos2);
}

#endif