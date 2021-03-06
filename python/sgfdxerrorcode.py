#! /usr/bin/env python
'''
 * sgfdxerrorcode.py
 *
 * Created on December 18, 2014
 * Copyright(c): SecuGen Corporation, All rights reserved
'''

class SGFDxErrorCode:
    
  SGFDX_ERROR_NONE = 0
  SGFDX_ERROR_CREATION_FAILED = 1
  SGFDX_ERROR_FUNCTION_FAILED = 2
  SGFDX_ERROR_INVALID_PARAM = 3
  SGFDX_ERROR_NOT_USED = 4
  SGFDX_ERROR_DLLLOAD_FAILED = 5
  SGFDX_ERROR_DLLLOAD_FAILED_DRV = 6
  SGFDX_ERROR_DLLLOAD_FAILED_ALGO = 7

  '''Device error'''
  SGFDX_ERROR_SYSLOAD_FAILED = 51
  SGFDX_ERROR_INITIALIZE_FAILED = 52
  SGFDX_ERROR_LINE_DROPPED = 53
  SGFDX_ERROR_TIME_OUT = 54
  SGFDX_ERROR_DEVICE_NOT_FOUND = 55
  SGFDX_ERROR_DRVLOAD_FAILED = 56
  SGFDX_ERROR_WRONG_IMAGE = 57
  SGFDX_ERROR_LACK_OF_BANDWIDTH = 58
  SGFDX_ERROR_DEV_ALREADY_OPEN = 59
  SGFDX_ERROR_GETSN_FAILED = 60
  SGFDX_ERROR_UNSUPPORTED_DEV = 61
   
  '''Extract & Matching error'''
  SGFDX_ERROR_FEAT_NUMBER = 101
  SGFDX_ERROR_INVALID_TEMPLATE_TYPE = 102
  SGFDX_ERROR_INVALID_TEMPLATE1 = 103
  SGFDX_ERROR_INVALID_TEMPLATE2 = 104
  SGFDX_ERROR_EXTRACT_FAIL = 105      
  SGFDX_ERROR_MATCH_FAIL = 106
#end class SGFDxErrorCode
