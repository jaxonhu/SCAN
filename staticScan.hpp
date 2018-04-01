//
//  staticScan.hpp
//  XcodeTest
//
//  Created by 胡家煊 on 2018/3/7.
//  Copyright © 2018年 infolab. All rights reserved.
//

#ifndef staticScan_hpp
#define staticScan_hpp

#include <stdio.h>
#include "Snap.h"
#include <iostream>
#include <math.h>
#include <vector>

typedef TPt <TNodeEDatNet<TInt, TFlt> > Net;

//typedef TPt <TUndirNet<TInt, TInt>> Net;

class staticScan
{
private:
   
    Net m_N;
    
public:
    staticScan(void);
    ~staticScan(void);
   
    static void GetScanCommunities(const Net& net, int u, double e, TVec<TIntV>& Communities,const TStr& output_file,const TStr& output_file2, const bool is_print);
   
    static double ComputeSim(const Net& net,TNodeEDatNet<TInt, TFlt>::TNodeI NodeI, TNodeEDatNet<TInt, TFlt>::TNodeI NodeJ);

    static bool IsCore(const Net& net, TNodeEDatNet<TInt, TFlt>::TNodeI NodeI, int u, double e);
};

#endif
