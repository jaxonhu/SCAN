//
//  staticScanMain.cpp
//  XcodeTest
//
//  Created by 胡家煊 on 2018/3/7.
//  Copyright © 2018年 infolab. All rights reserved.
//

#include <stdio.h>
#include "staticScan.hpp"

int main(int argc, char* argv[])
{
//    TStr input_file = "/Users/jaxon/xcode-project/XcodeTest/un_test_13_27.edges";

    Env = TEnv(argc, argv, TNotify::StdNotify);
    Env.PrepArgs(TStr::Fmt("Static SCAN Method. build: %s, %s. Time: %s", __TIME__, __DATE__, TExeTm::GetCurTm()));
    TExeTm ExeTm;
    Try
    const int NNeighbor = Env.GetIfArgPrefixInt("-u:", 2, "Ignore Number of Neighbor Blow This");
    const double TWeight =  Env.GetIfArgPrefixFlt("-e:", 0.7, "Cut edges weight blow this value");
    const TStr input_file =  Env.GetIfArgPrefixStr("-i:","/Users/jaxon/xcode-project/XcodeTest/un_test_13_27.edges","the input edge file splited by line, vertex id splited by tab");
    const TStr output_file = Env.GetIfArgPrefixStr("-o:","./","the output path");
    const TStr output_file2 = Env.GetIfArgPrefixStr("-o2:","./","the new edge dataset output");
    const bool is_print = Env.GetIfArgPrefixBool("-p:",true,"whether print the resutl in terminal or not");
    Net m_net;
    m_net = TNodeEDatNet<TInt, TFlt>::New();
    //load edge file
    m_net = TSnap::LoadEdgeList<Net>(input_file, 0, 1);
    //    cout<<m_net->GetNodes()<<endl;
    //    cout<<m_net->GetEdges()<<endl;
    TVec<TIntV> CmtyV;
    staticScan::GetScanCommunities(m_net, NNeighbor, TWeight, CmtyV, output_file, output_file2, is_print);
    
    Catch
    printf("\nrun time: %s (%s)\n", ExeTm.GetTmStr(), TSecTm::GetCurTm().GetTmStr().CStr());
    return 0;
}


