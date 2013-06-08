// RiftSharpDll.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "RiftSharpDll.h"
#include "OVR.h"

using namespace OVR;

Ptr<DeviceManager>	pManager;
Ptr<HMDDevice>		pHMD;
Ptr<SensorDevice>	pSensor;
SensorFusion		FusionResult;
HMDInfo			Info;
bool			InfoLoaded;

RIFTSHARPDLL_API void Initialize(void)
{
	System::Init();

	pManager = *DeviceManager::Create();

	pHMD = *pManager->EnumerateDevices<HMDDevice>().CreateDevice();

	if (pHMD)
	{
		InfoLoaded = pHMD->GetDeviceInfo(&Info);

		pSensor = *pHMD->GetSensor();
	}
	else
	{
		pSensor = *pManager->EnumerateDevices<SensorDevice>().CreateDevice();
	}
	
	if (pSensor)
	{
		FusionResult.AttachToSensor(pSensor);
	}
}

RIFTSHARPDLL_API void Uninitialize(void)
{
	pSensor.Clear();
	pHMD.Clear();
	pManager.Clear();

	System::Destroy();
}

RIFTSHARPDLL_API int GetYawPitchRoll(float* yaw, float* pitch, float* roll)
{
	if (pSensor)
	{
		Quatf quaternion = FusionResult.GetOrientation();
		quaternion.GetEulerAngles<Axis_Y, Axis_X, Axis_Z>(yaw, pitch, roll);
		return 0;
	}
	else
		return -1;
}

RIFTSHARPDLL_API int GetQuaternion(float* x, float* y, float* z, float* w)
{
	if (pSensor)
	{
		Quatf quaternion = FusionResult.GetOrientation();
		*x = quaternion.x;
		*y = quaternion.y;
		*z = quaternion.z;
		*w = quaternion.w;		
		return 0;
	}
	else
		return -1;
}

