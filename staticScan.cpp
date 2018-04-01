//
//  staticScan.cpp
//  XcodeTest
//
//  Created by 胡家煊 on 2018/3/7.
//  Copyright © 2018年 infolab. All rights reserved.
//

#include "staticScan.hpp"
#include <map>
#include <unordered_map>
#include <vector>
#include <fstream>

using namespace std;


staticScan::staticScan(void) {
    
}


staticScan::~staticScan(void) {
    
}

void staticScan::GetScanCommunities(const Net& net,
                                    int u, double e, TVec<TIntV>& Communities, const TStr& output_file, const TStr& output_file2, const bool is_print) {
    
    //初始化社区id
    int CurClusterNum = 0;
    /*
     *对于节点遍历
     *0代表未分类；大于0的代表类的标号，-1代表non-member; -2代表hub; -3代表outlier
     */
//    for (TNodeEDatNet<TInt, TFlt>::TNodeI NI = net->BegNI(); NI < net->EndNI(); NI++) {
//        cout <<"Node : " << NI.GetId() << " Node value: " << NI.GetDat() << endl;
//    }
//
//    for(TNodeEDatNet<TInt, TFlt>::TEdgeI EI = net->BegEI() ; EI < net->EndEI() ; EI++){
//        cout <<"Edge srcId: " << EI.GetSrcNId() << " dstId: " << EI.GetDstNId() << " EdgeValue: " << EI.GetDat()<< endl;
//    }
    
    for (TNodeEDatNet<TInt, TFlt>::TNodeI NI = net->BegNI(); NI < net->EndNI(); NI++) {
       // cout << "Node 1 levle: " << NI.GetId() << endl;
        //对于所有未分类的顶点，判断是否为核心顶点
        if (net->GetNDat(NI.GetId()) <= 0) {
            TIntV community;
            //Q中存储的是节点id
            std::vector<int> Q;
            //cout << "Node 2 level: " << NI.GetId()<< endl;
            if (staticScan::IsCore(net, NI, u, e)) {
               // cout << "Node 3 level: " << NI.GetId()<< endl;
                CurClusterNum++;
                net->SetNDat(NI.GetId(), CurClusterNum);
                community.Add(NI.GetId());
                //这里写的不对，应该是吧N(v)内的顶点入队
                for (int i = 0; i < NI.GetDeg(); i++)
                {
                    double sim = ComputeSim(net, NI, net->GetNI(NI.GetNbrNId(i)));
                    if(sim < e) continue;
                    Q.push_back(NI.GetNbrNId(i));
                }
                Q.push_back(NI.GetId());
                while (Q.size() != 0) {
                    //取出Q列表中的最后一个元素
                    TNodeEDatNet<TInt, TFlt>::TNodeI NodeI = net->GetNI(Q[Q.size() - 1]);
                    Q.pop_back();
                   // cout <<"Node 4 level: " << NodeI.GetId() << endl;
                    if (IsCore(net, NodeI, u, e)) {
                       // cout <<"Node 5 level: " << NodeI.GetId() << endl;
                        //遍历R
                        for (int j = 0; j < NodeI.GetDeg(); j++)
                        {
                            int Nid = NodeI.GetNbrNId(j);
                            double sim = ComputeSim(net, NodeI, net->GetNI(Nid));
                            if(sim < e) continue;
                            if (net->GetNI(Nid).GetDat() <= 0) {
                                net->SetNDat(Nid, CurClusterNum);
                                community.Add(Nid);
                            }
                            if (net->GetNI(Nid).GetDat() == 0) {
                                Q.push_back(Nid);
                            }
                        }
                    } else {
                        net->SetNDat(NodeI.GetId(), CurClusterNum);
                    }
                }
                Communities.Add(community);
            } else {
                //不是核心节点标记为non-member
                net->SetNDat(NI.GetId(), -1);
            }
        }
    }
    
//    for(TNodeEDatNet<TInt, TInt>::TNodeI NI = net->BegNI(); NI < net->EndNI() ; NI ++){
//        cout<< "Node " << NI.GetId() << " :" << NI.GetDat()<<endl;
//    }
    
    TIntV Hub, Outlier;
    //对于non-member节点进行判断，区别出hub和outlier
    for (TNodeEDatNet<TInt, TFlt>::TNodeI NI = net->BegNI(); NI < net->EndNI(); NI++) {
        if (NI.GetDat() == -1) {
            int flag = -1;
            for (int i = 0; i < NI.GetDeg(); i++) {
                if (NI.GetNbrNDat(i) != flag && flag > 0 && NI.GetNbrNDat(i) > 0) {
                    flag = -2;
                    break;
                }else{
                    flag = NI.GetNbrNDat(i);
                }
            }
            if (flag == -2)
            {
                net->SetNDat(NI.GetId(), -2);
                Hub.Add(NI.GetId());
            } else {
                net->SetNDat(NI.GetId(), -3);
                Outlier.Add(NI.GetId());
            }
        }
    }
    
    cout <<""<<endl;
    std::unordered_map<int,std::vector<int> > result;
    for(TNodeEDatNet<TInt, TFlt>::TNodeI NI = net->BegNI(); NI < net->EndNI() ; NI ++){
        result[NI.GetDat()].push_back(NI.GetId());
    }

    int communityCounts = 0;
    map<int, int> community_num;

    //输出结果到终端
    for(unordered_map<int, vector<int> >::iterator iter = result.begin();iter != result.end(); iter++){
        vector<int> v = iter->second;
        int communityId = iter->first;
        int member_num = 0;
        if(is_print)
            cout<<"CommunityId " << communityId << ":";
        for(int i = 0 ; i < v.size() ; i++){
            if(is_print)
                cout<< v[i] <<"\t";
            member_num ++;

        }
        community_num[communityId] = member_num;
        communityCounts ++;
        cout <<""<<endl;
    }
    
//    划分结果输出到文件
    ofstream out(output_file.CStr(),fstream::out);
    if(out.is_open()){
        cout << "open" << endl;
    }
    out << "#  the number of nodes in  input dataset : " << net->GetNodes() << "\n";
    out << "#  the number of edges in  input dataset : " << net->GetEdges() << "\n";
    out << "#  the communitys num : " << communityCounts << "\n";
    for(map<int,int>::iterator iter = community_num.begin() ; iter != community_num.end() ; iter++){
        int community_id = iter->first;
        int nodes_num = iter->second;
        out << "#   community  " << community_id << " : " << nodes_num << "\n";
    }
    for (TNodeEDatNet<TInt, TFlt>::TNodeI NI = net->BegNI(); NI < net->EndNI(); NI++) {
        int node_id = NI.GetId();
        int belong_to = NI.GetDat();
        out << node_id << "    " << belong_to << "\n";
    }
    out.close();
    
//    for (TNodeEDatNet<TInt, TFlt>::TNodeI NI = net->BegNI(); NI < net->EndNI(); NI++) {
//        cout <<"Node : " << NI.GetId() << " Node value: " << NI.GetDat() << endl;
//    }
//
//    for(TNodeEDatNet<TInt, TFlt>::TEdgeI EI = net->BegEI() ; EI < net->EndEI() ; EI++){
//        cout <<"Edge srcId: " << EI.GetSrcNId() << " dstId: " << EI.GetDstNId() << " EdgeValue: " << EI.GetDat()<< endl;
//    }
    
    //输出边集
    ofstream out2(output_file2.CStr(), fstream::out);
    if(out2.is_open()){
        cout << "open" << endl;
    }
    for(TNodeEDatNet<TInt, TFlt>::TEdgeI EI = net->BegEI() ; EI < net->EndEI() ; EI++){
        out2 << EI.GetSrcNId() << "    " << EI.GetDstNId() << "    " << EI.GetDat()<<"\n";
    }
    out2.close();
    
}

double staticScan::ComputeSim(const Net& net,TNodeEDatNet<TInt, TFlt>::TNodeI NodeI,
                              TNodeEDatNet<TInt, TFlt>::TNodeI NodeJ) {
    int ni = NodeI.GetDeg() + 1;
    int nj = NodeJ.GetDeg() + 1;
    int count = 0;
    for (int i = 0; i < NodeI.GetDeg(); i++)
    {
        for (int j = 0; j < NodeJ.GetDeg(); j++)
        {
            if (NodeI.GetNbrNId(i) == NodeJ.GetNbrNId(j))
            {
                count++;
            }
        }
    }
    double sim =  (2 + count)/(sqrt((double)ni)*sqrt((double)nj));
    if(net->IsEdge(NodeI.GetId(), NodeJ.GetId()) == true){
        net->SetEDat(NodeI.GetId(), NodeJ.GetId(), sim);
    }else{
        net->SetEDat(NodeJ.GetId(), NodeI.GetId(), sim);
    }
    return sim;
}

bool staticScan::IsCore(const Net& net, TNodeEDatNet<TInt, TFlt>::TNodeI NodeI,
                        int u, double e){
    int Count = 0;
    for (int i = 0; i < NodeI.GetDeg(); i++) {
        //获取邻居节点
        TNodeEDatNet<TInt, TFlt>::TNodeI NeighborNode = net->GetNI(NodeI.GetNbrNId(i));
        double Sim = staticScan::ComputeSim(net, NodeI, NeighborNode);
//        cout << " NodeI: " << NodeI.GetId() << "  " << NeighborNode.GetId() << endl;
//        cout << "Sim: " << Sim << endl;
//        if(net->IsEdge(NodeI.GetId(), NeighborNode.GetId()) == true){
//            net->SetEDat(NodeI.GetId(), NeighborNode.GetId(), Sim);
//        }else{
//            net->SetEDat(NeighborNode.GetId(), NodeI.GetId(), Sim);
//        }
        
        if (Sim > e) {
            Count++;
        }
    }
    if (Count > u) {
        return true;
    } else {
        return false;
    }
}

