/*
===========================================================================
ARX FATALIS GPL Source Code
Copyright (C) 1999-2010 Arkane Studios SA, a ZeniMax Media company.

This file is part of the Arx Fatalis GPL Source Code ('Arx Fatalis Source Code'). 

Arx Fatalis Source Code is free software: you can redistribute it and/or modify it under the terms of the GNU General Public 
License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

Arx Fatalis Source Code is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied 
warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with Arx Fatalis Source Code.  If not, see 
<http://www.gnu.org/licenses/>.

In addition, the Arx Fatalis Source Code is also subject to certain additional terms. You should have received a copy of these 
additional terms immediately following the terms and conditions of the GNU General Public License which accompanied the Arx 
Fatalis Source Code. If not, please request a copy in writing from Arkane Studios at the address below.

If you have questions concerning this license or the applicable additional terms, you may contact in writing Arkane Studios, c/o 
ZeniMax Media Inc., Suite 120, Rockville, Maryland 20850 USA.
===========================================================================
*/
//////////////////////////////////////////////////////////////////////////////////////
//   @@        @@@        @@@                @@                           @@@@@     //
//   @@@       @@@@@@     @@@     @@        @@@@                         @@@  @@@   //
//   @@@       @@@@@@@    @@@    @@@@       @@@@      @@                @@@@        //
//   @@@       @@  @@@@   @@@  @@@@@       @@@@@@     @@@               @@@         //
//  @@@@@      @@  @@@@   @@@ @@@@@        @@@@@@@    @@@            @  @@@         //
//  @@@@@      @@  @@@@  @@@@@@@@         @@@@ @@@    @@@@@         @@ @@@@@@@      //
//  @@ @@@     @@  @@@@  @@@@@@@          @@@  @@@    @@@@@@        @@ @@@@         //
// @@@ @@@    @@@ @@@@   @@@@@            @@@@@@@@@   @@@@@@@      @@@ @@@@         //
// @@@ @@@@   @@@@@@@    @@@@@@           @@@  @@@@   @@@ @@@      @@@ @@@@         //
// @@@@@@@@   @@@@@      @@@@@@@@@@      @@@    @@@   @@@  @@@    @@@  @@@@@        //
// @@@  @@@@  @@@@       @@@  @@@@@@@    @@@    @@@   @@@@  @@@  @@@@  @@@@@        //
//@@@   @@@@  @@@@@      @@@      @@@@@@ @@     @@@   @@@@   @@@@@@@    @@@@@ @@@@@ //
//@@@   @@@@@ @@@@@     @@@@        @@@  @@      @@   @@@@   @@@@@@@    @@@@@@@@@   //
//@@@    @@@@ @@@@@@@   @@@@             @@      @@   @@@@    @@@@@      @@@@@      //
//@@@    @@@@ @@@@@@@   @@@@             @@      @@   @@@@    @@@@@       @@        //
//@@@    @@@  @@@ @@@@@                          @@            @@@                  //
//            @@@ @@@                           @@             @@        STUDIOS    //
//////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////
// CSpellFx_Lvl01.h
//////////////////////////////////////////////////////////////////////////////////////
//
// Description:
// ARX Spells FX Level 01
//
// Updates: (date) (person) (update)
//////////////////////////////////////////////////////////////////////////////////////
// Refer to CSpellFx.h for details
//
// Copyright (c) 1999-2001 ARKANE Studios SA. All rights reserved
//////////////////////////////////////////////////////////////////////////////////////

#ifndef ARX_CSPELLFX_LVL01_H
#define ARX_CSPELLFX_LVL01_H

#include "graphics/data/Texture.h"
#include "graphics/effects/SpellEffects.h"

//-----------------------------------------------------------------------------
// Done By : Didier Pédreno
// Status  :
//-----------------------------------------------------------------------------
class CMagicMissile: public CSpellFx
{
	public:
		bool bExplo;
		bool bMove;
		bool bDone;
		int end;
		int iMax;
		int iLength;
		int	iBezierPrecision;
		float fColor[3];
		float fColor1[3];
		float fSize;
		float fTrail;
		float fOneOnLength;
		float fOneOnBezierPrecision;
		Vec3f eSrc;
		Vec3f eCurPos;
		Anglef angles;
		TextureContainer * tex_mm;
		D3DTLVERTEX pathways[6];
		ArxSound snd_loop;
	public:
		CMagicMissile();
		~CMagicMissile();

		// accesseurs
	public:
		void SetTTL(unsigned long);
		void SetPos(Vec3f);
		void SetColor(float, float, float);
		void SetColor1(float, float, float);

		// surcharge
	public:
		void	Create(const Vec3f &, const Anglef &);
		void	Kill();
		void	Update(unsigned long);
		float	Render();
};

//-----------------------------------------------------------------------------
class CMultiMagicMissile: public CSpellFx
{
	private:
		CSpellFx ** pTab;
		unsigned int uiNumber;
		bool bExplo;
	public:
		CMultiMagicMissile(long nb);
		~CMultiMagicMissile();

	public:
		void	CheckCollision();

	public:
		void	Create(const Vec3f &, const Anglef &);
		void	Kill();
		void	Update(unsigned long);
		float	Render();
};

//-----------------------------------------------------------------------------
// Done By : Sébastien Scieux
// Status  :
//-----------------------------------------------------------------------------
class CIgnit: public CSpellFx
{
	private:
		Vec3f	pos;
		float		perimetre;
		short		key;
		int			duration;
		int			currduration;
		float		interp;

		TextureContainer	* tp;
		float				r, g, b;
		int					mask;

		struct T_LINKLIGHTTOFX
		{
			Vec3f	poslight;
			Vec3f	posfx;
			int			actif;
			int			idl;
			int			iLightNum;
		};

	public:
		unsigned char		nblight;
		T_LINKLIGHTTOFX		tablight[256];

	public:
		CIgnit();
		~CIgnit();

		unsigned long GetDuration(void)
		{
			return this->duration;
		};

		void	Create(Vec3f * posc, float perim, int speed);
		void	Update(unsigned long);
		float	Render();
		void	Kill();
		void	AddLight(int iLight);
		void	Action(int);
		void	ChangeRGBMask(float r, float g, float b, int mask)
		{
			this->r = r;
			this->g = g;
			this->b = b;
			this->mask = mask;
		};
		void	ChangeTexture(TextureContainer * tc)
		{
			this->tp = tc;
		};
		float	GetPerimetre(void)
		{
			return this->perimetre;
		}
};

//-----------------------------------------------------------------------------
// Done By : Sébastien Scieux
// Status  :
//-----------------------------------------------------------------------------
class CDoze: public CIgnit
{
	public:
		void CreateDoze(Vec3f * posc, float perim, int speed)
		{
			this->Create(posc, perim, speed);
			this->ChangeTexture(MakeTCFromFile("Graph\\Particles\\doze_hit.bmp"));
			this->ChangeRGBMask(0.f, .7f, 1.f, 0xFF0000FF);
		};
		void	AddLightDoze(int iLight);
};

//-----------------------------------------------------------------------------
// Done By : Sébastien Scieux
// Status  :
//-----------------------------------------------------------------------------
class CPortal: public CSpellFx
{
	private:
		short		key;
		int			duration;
		int			currduration, currframe;
		Vec3f	pos;
		float		r;
		TextureContainer	* tp, *te;

		struct T_ECLAIR
		{
			short			actif, nbseg;
			int				duration;
			int				currduration;
			int				numpt;
			Vec3f	*	seg;
		};

		int			timeneweclair;
		int			nbeclair;				//eclair
		T_ECLAIR	tabeclair[256];

		TextureContainer	* spheretc;		//sphere
		Vec3f			sphereposdep;
		Vec3f			sphereposend;
		float				spherescale;
		Vec3f		*	spherevertex;
		unsigned short	*	sphereind;
		int					spherenbpt;
		int					spherenbfaces;
		float				spherealpha;
		D3DTLVERTEX		*	sphered3d;

 
	public:
 
		~CPortal();

		unsigned long GetDuration(void)
		{
			return this->duration;
		};

		void	AddNewEclair(Vec3f * endpos, int nbseg, int duration, int numpt);
		void	DrawAllEclair();
		void	Update(unsigned long);
		float	Render();
 
};

#endif
