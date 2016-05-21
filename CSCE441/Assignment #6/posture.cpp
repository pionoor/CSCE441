#include "posture.h"
#define M11	 0.0
#define M12	 1.0
#define M13	 0.0
#define M14	 0.0
#define M21	-0.5
#define M22	 0.0
#define M23	 0.5
#define M24	 0.0
#define M31	 1.0
#define M32	-2.5
#define M33	 2.0
#define M34	-0.5
#define M41	-0.5
#define M42	 1.5
#define M43	-1.5
#define M44	 0.5

/************************ Posture class functions **********************************/

//Output Posture  = (1-t)*a + t*b
 
Posture LinearInterpolate(float t, Posture const& a, Posture const& b)
{
	Posture InterpPosture;

	//Iterpolate root position
	InterpPosture.root_pos = interpolate(t, a.root_pos, b.root_pos);

	//Interpolate bones rotations
	for (int i = 0; i < MAX_BONES_IN_ASF_FILE; i++)
		InterpPosture.bone_rotation[i] = interpolate(t, a.bone_rotation[i], b.bone_rotation[i]);

	return InterpPosture;
}

Posture CatMull(float t, Posture const& a, Posture const& b, Posture const& c, Posture const& d)
{
	Posture p1, p2, p3, p4;
	Posture final;

	//Iterpolate root position
	p1.root_pos = b.root_pos * M12;
	p2.root_pos = a.root_pos * M21 + c.root_pos * M23;
	p3.root_pos = a.root_pos * M31 + b.root_pos * M32 + c.root_pos * M33 + d.root_pos * M34;
	p4.root_pos = a.root_pos * M41 + b.root_pos * M42 + c.root_pos * M43 + d.root_pos * M44;

	final.root_pos = (((p4.root_pos*t + p4.root_pos)*t + p2.root_pos)*t + p1.root_pos);


	//Interpolate bones rotations
	for (int i = 0; i < MAX_BONES_IN_ASF_FILE; i++){
	p1.bone_rotation[i] = b.bone_rotation[i] * M12;
	p2.bone_rotation[i] = a.bone_rotation[i] * M21 + c.bone_rotation[i] * M23;
	p3.bone_rotation[i] = a.bone_rotation[i] * M31 + b.bone_rotation[i] * M32 + c.bone_rotation[i] * M33 + d.bone_rotation[i] * M34;
	p4.bone_rotation[i] = a.bone_rotation[i] * M41 + b.bone_rotation[i] * M42 + c.bone_rotation[i] * M43 + d.bone_rotation[i] * M44;

	final.bone_rotation[i] = (((p4.bone_rotation[i] * t + p4.bone_rotation[i])*t + p2.bone_rotation[i])*t + p1.bone_rotation[i]);
	}

	return final;
}