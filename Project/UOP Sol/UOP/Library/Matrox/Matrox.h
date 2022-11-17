#pragma once

/**
@mainpage Matrox Project
  @section intro 소개
  - 소개      :   Matrox의 Mil library를 Class화하였음@n
                  9800A, H100용의 운영프로그램으로 사용됨
				  Project중 MilTest에서 테스트할 수 있음
  @section   Program Matrox
  - 프로그램명		:  Matrox.dll
  - 프로그램내용	:  Mil함수를 Class화한것과 Grabber를 포함시킴
  @section  CREATEINFO 작성정보
  - 작성자     		:  전병근
  - 작성일      	:  09/11/13
  - 수정일          :  12/05/15
  @section  MODIFYINFO      수정정보
  - 수정자/수정일  	: 수정내역
  @version 1.0
  @todo Grabber를 따로 독립화하여야 함.
*/

#include "MilBase.h"
#include "MilApplication.h"
#include "MilBuffer.h"
#include "MilSystem.h"
#include "Mildigitizer.h"
#include "Mildisplay.h"
#include "Milerror.h"
#include "MilIm.h"
#include "MilModfinder.h"
#include "MilEdgeFinder.h"
#include "MilPat.h"
#include "MilBlob.h"
#include "MilGraph.h"
#include "MilVLib.h"
#include "MilResult.h"
#include "MilCal.h"
