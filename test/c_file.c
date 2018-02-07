#include <Python.h>
#include "sgfplib.h"
#include <stdio.h>
HSGFPM m_hFPM; // handle for SGFPM
void Cinit() {
	printf("Starting initialization\n");
	
	DWORD err = SGFPM_Create(&m_hFPM);
	if(err != 0)
		printf("ERROR at SGFPM_Create\n");

	DWORD devname = SG_DEV_FDU05;
	err = SGFPM_Init(m_hFPM, devname);
	if(err != 0)
		printf("ERROR at SGFPM_Init\n");


	DWORD devId = USB_AUTO_DETECT; // auto detect
	err = SGFPM_OpenDevice(m_hFPM, devId);
	if(err != 0)
		printf("ERROR %d at SGFPM_OpenDevice\n", err);
	printf("Ending initialization\n");
}

BYTE * CscanFingerprint(char * fileName) {
	DWORD err;
	printf("Starting fingerprint scan\n");
	int m_ImageWidth = 300;
	int m_ImageHeight = 400;
	BYTE *m_ImgBuf = (BYTE*)malloc(m_ImageWidth*m_ImageHeight * sizeof(BYTE));
	DWORD timeout = 10000;
	DWORD quality = 80;
	if(SGFPM_GetImageEx(m_hFPM, m_ImgBuf, timeout, NULL, quality) == SGFDX_ERROR_NONE) {
		printf("success!");
		printf("%d", *m_ImgBuf);
	} else {
		printf("ERROR %d at SGFPM_GetImageEx\n", SGFPM_GetImageEx(m_hFPM, m_ImgBuf, timeout, NULL, quality));
	}
	printf("Ending fingerprint scan\n");


	// Create template from captured image
	printf("Starting template creation\n");
	DWORD maxTemplateSize;
	BYTE*minBuffer;
	err = SGFPM_GetMaxTemplateSize(m_hFPM, &maxTemplateSize);
	if(err != 0)
		printf("ERROR %d at SGFPM_GetMaxTemplateSize\n", err);
	minBuffer = (BYTE*)malloc(maxTemplateSize * sizeof(BYTE));

	// Set information about template
	SGFingerInfo finger_info;
	finger_info.ImageQuality = (WORD)quality;
	finger_info.ImpressionType = SG_IMPTYPE_LP;
	finger_info.ViewNumber = 1;
	err = SGFPM_CreateTemplate(m_hFPM, &finger_info, m_ImgBuf, minBuffer);
	if(err != 0)
		printf("ERROR %d at SGFPM_CreateTemplate\n", err);
	else
		printf("Ending template creation\n");

	FILE *write_ptr;
	write_ptr = fopen("test.txt","wb");  // w for write, b for binary
	fwrite(minBuffer,maxTemplateSize * sizeof(BYTE),1,write_ptr); // write 10 bytes from our buffer
	
	return minBuffer;
}

int CmatchFingerprints(BYTE * fingerprint1, BYTE * fingerprint2) {
	printf("%s,\n %s\n", fingerprint1, fingerprint2);
	printf("STARTING MATCHING\n");
	DWORD sl = SL_NONE;
	BOOL score = 0;
	DWORD err = SGFPM_GetMatchingScore(m_hFPM, fingerprint1, fingerprint2, &score);
	printf("Score: %d\n", score);
	printf("ENDING MATCHING\n");
	return score;
}

static PyObject * init(PyObject * self, PyObject * args) {
	Cinit();
	return Py_BuildValue("");
}
static PyObject * scanFingerprint(PyObject * self, PyObject * args) {
	char * filename;
	PyArg_ParseTuple(args, "s", &filename);
	BYTE * buffer = CscanFingerprint(filename);
	return Py_BuildValue("i", buffer);
}
static PyObject * matchFingerprints(PyObject * self, PyObject * args) {
	char*f1, f2;
	// Continue here, implement loading fingerprint files
	DWORD maxTemplateSize;
	
	DWORD err = SGFPM_GetMaxTemplateSize(m_hFPM, &maxTemplateSize);
	if(err != 0)
		printf("ERROR %d at SGFPM_GetMaxTemplateSize\n", err);
	f1 = (BYTE*)malloc(maxTemplateSize * sizeof(BYTE));

	f2 = (BYTE*)malloc(maxTemplateSize * sizeof(BYTE));
	printf("Fails here1?\n");
	PyArg_ParseTuple(args, "ss", );
	printf("Fails here2\n");
	BOOL matched  = CmatchFingerprints(&f1, &f2);
	printf("Fails here3?\n");
	return Py_BuildValue("i", matched);
}


static PyMethodDef myMethods[] = {
	{"init", init, METH_VARARGS, "x"},
	{"scanFingerprint", scanFingerprint, METH_VARARGS, "y"},
	{"matchFingerprints", matchFingerprints, METH_VARARGS, "z"},
	{NULL, NULL, 0, NULL}
};
static struct PyModuleDef myModule = {
	PyModuleDef_HEAD_INIT,
	"myModule",
	"initonacci Module",
	-1,
	myMethods	
};

PyMODINIT_FUNC PyInit_myModule(void) {
	return PyModule_Create(&myModule);
}