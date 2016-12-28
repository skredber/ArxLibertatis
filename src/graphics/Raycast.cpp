/*
 * Copyright 2016 Arx Libertatis Team (see the AUTHORS file)
 *
 * This file is part of Arx Libertatis.
 *
 * Arx Libertatis is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Arx Libertatis is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Arx Libertatis.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "graphics/Raycast.h"

#include "graphics/data/Mesh.h"
#include "platform/profiler/Profiler.h"

static RaycastResult RaycastMiss() { return RaycastResult(false, Vec3f_ZERO); }
static RaycastResult RaycastHit(Vec3f hit) { return RaycastResult(true, hit); }

void dbg_addTile(Vec2i tile);
void dbg_addPoly(EERIEPOLY * poly, Vec3f hit, Color c);

// Based on:
// Real-Time Collision Detection by Christer Ericson,
// published by Morgan Kaufmann Publishers, © 2005 Elsevier Inc
// page 326ff
template <class F>
static RaycastResult WalkTiles(const Vec3f & start, const Vec3f & end, F func) {
	
	Vec2f p1 = Vec2f(start.x, start.z);
	Vec2f p2 = Vec2f(end.x, end.z);
	
	// Side dimensions of the square cell
	const Vec2f cellSide = Vec2f(ACTIVEBKG->Xdiv, ACTIVEBKG->Zdiv);
	
	// Determine start grid cell coordinates
	Vec2i tile = Vec2i(glm::floor(p1 / cellSide));
	
	// Determine end grid cell coordinates
	Vec2i endTile = Vec2i(glm::floor(p2 / cellSide));
	
	// Determine in which primary direction to step
	Vec2i d;
	d.x = ((p1.x < p2.x) ? 1 : ((p1.x > p2.x) ? -1 : 0));
	d.y = ((p1.y < p2.y) ? 1 : ((p1.y > p2.y) ? -1 : 0));
	
	// Determine tx and ty, the values of t at which the directed segment
	// (x1,y1)-(x2,y2) crosses the first horizontal and vertical cell
	// boundaries, respectively. Min(tx, ty) indicates how far one can
	// travel along the segment and still remain in the current cell
	Vec2f min = cellSide * glm::floor(p1 / cellSide);
	Vec2f max = min + cellSide;
	
	float tx = ((p1.x > p2.x) ? (p1.x - min.x) : (max.x - p1.x)) / std::abs(p2.x - p1.x);
	float ty = ((p1.y > p2.y) ? (p1.y - min.y) : (max.y - p1.y)) / std::abs(p2.y - p1.y);
	
	// Determine deltax/deltay, how far (in units of t) one must step
	// along the directed line segment for the horizontal/vertical
	// movement (respectively) to equal the width/height of a cell
	Vec2f delta = cellSide / glm::abs(p2 - p1);
	
	// Main loop. Visits cells until last cell reached
	for (;;) {
		dbg_addTile(tile);
		RaycastResult res = func(start, end, tile);
		if(res.hit)
			return res;
		
		if (tx <= ty) {
			// tx smallest, step in x
			if(tile.x == endTile.x)
				break;
			
			tx += delta.x;
			tile.x += d.x;
		} else {
			// ty smallest, step in y
			if(tile.y == endTile.y)
				break;
			
			ty += delta.y;
			tile.y += d.y;
		}
	}
	
	return RaycastMiss();
}

static Vec3f baryToWorld(Vec3f p0, Vec3f p1, Vec3f p2, Vec3f baryHit) {
	
	float u = 1.0f - baryHit.y - baryHit.z;
	float v = baryHit.y;
	float w = baryHit.z;
	
	return u * p0 + v * p1 + w * p2;
}

static RaycastResult linePolyIntersection(const Vec3f & start, const Vec3f & end, const EERIEPOLY & epp) {

	Vec3f dir = end - start;
	
	Vec3f baryHit;
	if(arx::intersectLineTriangle(start, dir, epp.v[0].p, epp.v[1].p, epp.v[2].p, baryHit)) {
		if(baryHit.x <= 1.f) {
			return RaycastHit(baryToWorld(epp.v[0].p, epp.v[1].p, epp.v[2].p, baryHit));
		}
	}
	
	if((epp.type & POLY_QUAD)) {
		if(arx::intersectLineTriangle(start, dir, epp.v[1].p, epp.v[3].p, epp.v[2].p, baryHit)) {
			if(baryHit.x <= 1.f) {
				return RaycastHit(baryToWorld(epp.v[1].p, epp.v[3].p, epp.v[2].p, baryHit));
			}
		}
	}
	
	return RaycastMiss();
}

static RaycastResult lightFlareRaycast(const Vec3f & start, const Vec3f & end, const Vec2i & tile) {
	
	const EERIE_BKG_INFO & eg = ACTIVEBKG->fastdata[tile.x][tile.y];
	for(long k = 0; k < eg.nbpolyin; k++) {
		EERIEPOLY & ep = *eg.polyin[k];
		
		if(ep.type & POLY_TRANS) {
			continue;
		}
		
		RaycastResult res = linePolyIntersection(start, end, ep);
		if(res.hit) {
			// Ignore hits too close to target.
			// Lights are often inside geometry
			if(fartherThan(end, res.pos, 20.f)) {
				dbg_addPoly(&ep, res.pos, Color::green);
				return res;
			} else {
				dbg_addPoly(&ep, res.pos, Color::red);
			}
		}
	}
	
	return RaycastMiss();
}


RaycastResult RaycastLightFlare(const Vec3f & start, const Vec3f & end) {
	
	ARX_PROFILE_FUNC();
	
	return WalkTiles(start, end, lightFlareRaycast);
}

//#define RAYCAST_DEBUG 1

#ifndef RAYCAST_DEBUG

void dbg_addTile(Vec2i tile){ ARX_UNUSED(tile); }
void dbg_addPoly(EERIEPOLY * poly, Vec3f hit, Color c){ ARX_UNUSED(poly); ARX_UNUSED(hit); ARX_UNUSED(c);}
void RaycastDebugClear() {}
void RaycastDebugDraw() {}

#else

#include "graphics/DrawLine.h"

struct DebugPoly {
	EERIEPOLY * poly;
	Vec3f pos;
	Color c;
	
	DebugPoly(EERIEPOLY * poly, Vec3f pos, Color c)
		: poly(poly)
		, pos(pos)
		, c(c)
	{ }
};

static std::vector<Vec2i> dbg_tiles;
static std::vector<DebugPoly> dbg_hits;

void dbg_addTile(Vec2i tile) {
	dbg_tiles.push_back(tile);
}

void dbg_addPoly(EERIEPOLY * poly, Vec3f pos, Color c) {
	dbg_hits.push_back(DebugPoly(poly, pos, c));
}

void RaycastDebugClear()
{
	dbg_tiles.clear();
	dbg_hits.clear();
}

void RaycastDebugDraw() {
	
	GRenderer->SetRenderState(Renderer::DepthTest, false);
	
	for(auto & tile: dbg_tiles) {
		Vec3f foo = Vec3f(tile.x *100, ACTIVECAM->orgTrans.pos.y + 80, tile.y*100);
		drawLine(foo, foo + Vec3f(100, 0, 0), Color::white);
		drawLine(foo + Vec3f(100, 0, 0)  , foo + Vec3f(100, 0, 100), Color::white);
		drawLine(foo + Vec3f(100, 0, 100), foo + Vec3f(  0, 0, 100), Color::white);
		drawLine(foo, foo + Vec3f(  0, 0, 100), Color::white);
	}
	
	for(auto hit : dbg_hits) {
		drawLineCross(hit.pos, hit.c, 2);
		
		Vec3f pp[4];
		size_t count = ((hit.poly->type & POLY_QUAD) ? 4u : 3u);
		for(size_t i = 0; i < count; i++) {
			pp[i] = hit.poly->v[i].p;
		}
		
		drawLine(pp[0], pp[1], hit.c);
		drawLine(pp[2], pp[0], hit.c);
		if(count == 4) {
			drawLine(pp[2], pp[3], hit.c);
			drawLine(pp[3], pp[1], hit.c);
		} else {
			drawLine(pp[1], pp[2], hit.c);
		}
	}
}

#endif