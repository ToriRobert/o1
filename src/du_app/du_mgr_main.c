/*******************************************************************************
################################################################################
#   Copyright (c) [2017-2019] [Radisys]                                        #
#                                                                              #
#   Licensed under the Apache License, Version 2.0 (the "License");            #
#   you may not use this file except in compliance with the License.           #
#   You may obtain a copy of the License at                                    #
#                                                                              #
#       http://www.apache.org/licenses/LICENSE-2.0                             #
#                                                                              #
#   Unless required by applicable law or agreed to in writing, software        #
#   distributed under the License is distributed on an "AS IS" BASIS,          #
#   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.   #
#   See the License for the specific language governing permissions and        #
#   limitations under the License.                                             #
################################################################################
*******************************************************************************/

/* This file is the entry point for DU APP */
#include "common_def.h"
#include "lrg.h"
#include "legtp.h"
#include "lrg.x"
#include "lkw.x"

#ifdef O1_ENABLE
#include "O1Interface.h"
#include "CmInterface.h"
#include "PmInterface.h"
#endif

#ifdef O1_ENABLE
bool bringCellUp(uint16_t cellId)
{
   DU_LOG("\nO1 bringCellUp");
   return true;
}

bool bringCellDown(uint16_t cellId)
{
   DU_LOG("\nO1 bringCellDown");
   return true;
}

uint8_t setCellParam()
{
   DU_LOG("\nO1 setCellParam");
   return ROK;
}   

uint8_t setRrmPolicy(RrmPolicyList rrmPolicy[], uint8_t policyNum)
{
   DU_LOG("\nO1 setRrmPolicy");
   return ROK;
}     
#endif

void init_log()
{
	openlog("Interface",LOG_CONS | LOG_PID | LOG_NDELAY, LOG_LOCAL1);
}

int main()
{
   init_log();

#ifdef O1_ENABLE
   DU_LOG("\nO1 module start");
   if(start_O1_module() != ROK)
      return RFAILED;

   // 創建測試數據
   SliceMetricList testSliceMetricList;
   SliceMetricRecord testRecords[] = {
      {{1, 1}, 100.5, 50.2},
      {{100000, 2}, 150.3, 75.8},
      {{16777215, 3}, 200.1, 100.6}  // 16777215 是 24 位的最大值 (2^24 - 1)
   };

   testSliceMetricList.nRecords = sizeof(testRecords) / sizeof(SliceMetricRecord);
   testSliceMetricList.sliceRecord = testRecords;

   // 調用 sendSliceMetric 函數
   int result = sendSliceMetric(&testSliceMetricList);
   DU_LOG("\nsendSliceMetric 結果: %d", result);

#endif

   return ROK;

}/* end of main()*/

/**********************************************************************
         End of file
**********************************************************************/
