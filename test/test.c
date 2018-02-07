#include "../include/sgfplib.h"
#include <stdio.h>

int main(int argc, const char* argv[]) {
	HSGFPM m_hFPM; // handle for SGFPM
	DWORD err = SGFPM_Create(&m_hFPM);

	DWORD devname = SG_DEV_FDU05;
	err = SGFPM_Init(m_hFPM, devname);
	printf("\n%d\n", err);

	DWORD devId = USB_AUTO_DETECT; // auto detect
	err = SGFPM_OpenDevice(m_hFPM, devId);
	printf("\n%d\n", err);

	if (m_hFPM)
	{
	SGFPM_Terminate(m_hFPM);
	}
	m_hFPM = 0;
}

//-L../lib/linux3 -lsgfplib -lsgnfiq -lsgfpamx -lsgfdu06 -lusb -lpthread 