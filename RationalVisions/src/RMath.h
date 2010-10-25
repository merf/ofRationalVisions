/*
 *  RMaths.h
 *  RationalVisions
 *
 *  Created by Neil Wallace on 20/08/2010.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */
#pragma once

namespace RMath
{
	//#define PI = 3.141592653589793f;
	//#define TWO_PI = 2.0f * PI;
	
	//-------------------------------------------------------
	inline void Limit(float lower, float& val, float upper)
	{
		if(val > upper)
		{
			val = upper;
		}
		else if(val < lower)
		{
			val = lower;
		}
	}
	
	//-------------------------------------------------------
	inline void LimitLower(float& val, float lower)
	{
		if(val < lower)
		{
			val = lower;
		}
	}
	
	//-------------------------------------------------------
	inline void LimitUpper(float& val, float upper)
	{
		if(val > upper)
		{
			val = upper;
		}
	}
	
	//-------------------------------------------------------
	inline float Lerp(float a, float b, float interpolate)
	{
		Limit(0.0f, interpolate, 1.0f);
		return a + (b-a)*interpolate;
	}

	//-------------------------------------------------------
	inline float Min(float a, float b)
	{
		if(a < b)
		{
			return a;
		}
		return b;
	}

	//-------------------------------------------------------
	inline float Max(float a, float b)
	{
		if(a > b)
		{
			return a;
		}
		return b;
	}
};