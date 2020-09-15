Shader "Default"
{
	Properties
	{
		u_time("time", Vec4) = (1,2,3,4);
	}

	Pass
	{
		rs = (ENABLE_COLOR, ENABLE_TEXTURE);

		vertex = "vs_buildin.sc";
		fragment = "fs_buildin.sc";
		varying = "varying.def.sc";
	}

	// #pragma property u_time "time" Vec4

//	#pragma vertex  vs_buildin.sc
//	#pragma fragment fs_buildin.sc
//	#pragma varying varying.def.sc
}

