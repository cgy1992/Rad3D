/*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

struct Utility 
{
	static Float3 ToFloat3(const Point3 & p)
	{
		//return Float3(p.x, p.z, p.y);
		return Float3(p.x, p.y, p.z);
	}

	static Rad::Quat ToQuat(const ::Quat & q)
	{
		/*float x, y, z;
		q.GetEuler(&x, &y, &z);

		Quat mq, mz;

		mz.FromAxis(Float3::UnitZ, -PI_1_2);
		mq.FromPitchYawRoll(x, y, z);

		mq *= mz;

		return mq;*/

		Rad::Quat mq = Rad::Quat(-q.x, -q.y, -q.z, q.w);

		return mq;
	}
};